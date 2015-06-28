// Decompression by Zoinkity!
#pragma once

class CommandAndConquerDecoder
{
public:
	CommandAndConquerDecoder(void);
	~CommandAndConquerDecoder(void);
	int dec(unsigned char* input, unsigned char* output, int& compressedSize, int kind);
};
