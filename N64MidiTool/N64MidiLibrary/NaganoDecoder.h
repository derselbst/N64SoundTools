#pragma once

class CNaganoDecoder
{
public:
	CNaganoDecoder(void);
	~CNaganoDecoder(void);

	int dec(unsigned char* inputBuffer, int compressedSize, unsigned char* output);
	void KonamiLZW(unsigned char* inputBuffer, int compressedSizeSection, int& inputPosition, unsigned char* output, int& outputPosition);

	void EncodeKonamiLZW(unsigned char* inputBuffer, int uncompressedSize, unsigned char* output, int& compressedSize);
};
