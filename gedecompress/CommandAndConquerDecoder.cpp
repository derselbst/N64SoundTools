#include "StdAfx.h"
#include "CommandAndConquerDecoder.h"

CommandAndConquerDecoder::CommandAndConquerDecoder(void)
{
}

CommandAndConquerDecoder::~CommandAndConquerDecoder(void)
{
}

#define NONE -1

struct tuple
{
	unsigned char* data;
	int length;
	int value;

	tuple()
	{
		data = NULL;
	}

	tuple(unsigned char* dataIn, int lengthIn, int valueIn)
	{
		length = lengthIn;
		data = dataIn;
		value = valueIn;
	}
};


int hasho(unsigned char* b, int& w)
{
    unsigned long v = (b[0] << 8) ^ (b[1] << 4) ^ b[2];
    v *= 0x9E5F;
    if (w == NONE)
	{
        v >>= 4;
        v &= 0xFFF;
	}
    else
	{
        v >>= 1;
        v &= 0xFF8;
        v += w;
        w += 1;
        w &= 7;
	}
    return v;
}

int CommandAndConquerDecoder::dec(unsigned char* input, unsigned char* output, int& compressedSize, int kind)
{
	int inputPosition = 0;
	int outputPosition = 0;

	//"""Decompresses bytes object [input] returning a bytes object.
    //The [kind] argument selects the dictionary fill method.
    //Three kinds are supported:
    //    0   store (no decompression occurs)
    //    1   type 11 files
    //    2   type 22 files
    
    if (kind == 0)
	{
		memcpy(output, input, compressedSize);
		return compressedSize;
	}

    if (input[0] == 1)
	{
		memcpy(output, input, compressedSize - 4);
		return compressedSize - 4;
	}

	unsigned char defaultChar[] = { 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38 };
    //default = b"123456789012345678"

	tuple buf[0x1000];
	for (int x = 0; x < 0x1000; x++)
	{
		buf[x] = tuple(defaultChar, 18, 0);
	}

	int count = 0;
	unsigned long bits = 1;
	inputPosition = 4;
    
    int wrap = 0;
	if (kind == 2)
		wrap = 0;
	else 
		wrap = NONE;

    while (inputPosition < compressedSize)
	{
        if (bits == 1)
		{
            bits = 0x10000 | input[inputPosition];
            bits |= input[inputPosition + 1] << 8;
            inputPosition += 2;
		}
        if (bits & 1)
		{
            unsigned char l = input[inputPosition];
            unsigned long idx = (l << 4) & 0xF00;
            idx |= input[inputPosition + 1];
            inputPosition += 2;
            l &= 0xF;
            l += 3;
			unsigned char* src = buf[idx].data;
            int p = buf[idx].value;

            int org = outputPosition;
            for (int i = 0; i < l; i++)
			{
				output[outputPosition++] = src[p + i];
			}

            if (count)
			{
                unsigned long o = org - count;
                p = hasho(&output[o], wrap);
                buf[p] = tuple(&output[0], outputPosition, o);
                if (count == 2)
				{
                    p = hasho(&output[o+1], wrap);
                    buf[p] = tuple(&output[0], outputPosition, o+1);
				}
                count = 0;
			}

            if (wrap == NONE)
			{
                idx &= 0xFFF;
			}
            else
			{
                idx &= 0xFF8;
                idx += wrap;
                wrap += 1;
                wrap &= 7;
			}
            buf[idx] = tuple(&output[0], outputPosition, org);
		}
        else
		{
            output[outputPosition++] = input[inputPosition];
            inputPosition += 1;
            count += 1;
            if (count == 3)
			{
                count = 2;
                int org = outputPosition - 3;
                int p = hasho(&output[org], wrap);
                buf[p] = tuple(&output[0], outputPosition, org);
			}
		}
        bits >>= 1;
	}

	return outputPosition;
}