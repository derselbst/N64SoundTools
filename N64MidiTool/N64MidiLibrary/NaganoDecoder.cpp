#include "StdAfx.h"
#include "NaganoDecoder.h"

CNaganoDecoder::CNaganoDecoder(void)
{
}

CNaganoDecoder::~CNaganoDecoder(void)
{
}

void CNaganoDecoder::KonamiLZW(unsigned char* inputBuffer, int compressedSizeSection, int& inputPosition, unsigned char* output, int& outputPosition)
{
	unsigned long endPosition = (inputPosition + compressedSizeSection - 4);
    while (inputPosition < endPosition)
	{
		unsigned char i = inputBuffer[inputPosition++];

        if (i < 0x80)
		{
            unsigned long b = (i<<8) + inputBuffer[inputPosition++];
            b &= 0x3FF;
            i >>= 2;
            for (int j = 0; j <(i+2); j++)
                output[outputPosition++] = output[outputPosition-b];
		}
        else if (i < 0xA0)
		{
            i &= 0x1F;
            for (int j = 0; j < i; j++)
			{
                output[outputPosition++] = inputBuffer[inputPosition++];
			}
		}
        else if (i < 0xE0)
		{
            i &= 0x1F;
            unsigned char v = inputBuffer[inputPosition++];
			int end = i+2;
            for (int j = 0; j < end; j++)
			{
                output[outputPosition++] = v;
			}
		}
        else if (i < 0xFF)
		{
            i &= 0x1F;
			for (int j = 0; j < (i + 2); j++)
				output[outputPosition++] = 0x00;
		}
        else // if (i == 0xFF)
		{
			int end = inputBuffer[inputPosition++] + 2;
			for (int j = 0; j < end; j++)
				output[outputPosition++] = 0x00;
		}
	}
}

int CNaganoDecoder::dec(unsigned char* inputBuffer, int compressedSize, unsigned char* output)
{
	int inputPosition = 0;
	int outputPosition = 0;

	while (outputPosition < compressedSize)
	{
		if ((compressedSize - outputPosition) <= 0x10)
			break;

        unsigned long size = ((((((inputBuffer[inputPosition] << 8) | inputBuffer[inputPosition+1]) << 8) | inputBuffer[inputPosition+2]) << 8) | inputBuffer[inputPosition+3]);
        unsigned long width  = size >> 28;
        if (width != 0)
		{
            // #Interleaved data.
            inputPosition += 4;
            
            for (int w = 0; w < (width+1); w++)
			{
                size = ((((((inputBuffer[inputPosition] << 8) | inputBuffer[inputPosition+1]) << 8) | inputBuffer[inputPosition+2]) << 8) | inputBuffer[inputPosition+3]);
                size &= 0xFFFFFFF;

				inputPosition += 4;
                KonamiLZW(inputBuffer, size, inputPosition, output, outputPosition);
			}
		}
        else
		{
            size &= 0xFFFFFFF;

			inputPosition += 4;
            KonamiLZW(inputBuffer, size, inputPosition, output, outputPosition);
		}
	}

	return outputPosition;
}

void CNaganoDecoder::EncodeKonamiLZW(unsigned char* inputBuffer, int uncompressedSize, unsigned char* output, int& compressedSize)
{
	// TODO

	int outputPosition = 0;

	// Skip the size for now
	outputPosition += 4;

	int inputPosition = 0;
	while (inputPosition < uncompressedSize)
	{
		
	}
}