#pragma once

#include "MidiExports.h"

class GECompression;

struct TrackEvent // referenced in order, but first = 2710, doors refer to 27XX + preset to it implicitly, 0x44 per
{
	bool obsoleteEvent;
	unsigned long deltaTime;
	unsigned long durationTime;
	unsigned long absoluteTime;
	byte type;
	byte* contents;  // remember delete all track events mem later
	int contentSize;
};


class MIDI_API CMidiParse
{
public:


	CMidiParse(void);
	~CMidiParse(void);

	void GEMidiToMidi(byte* inputMID, int inputSize, CString outFileName, int& numberInstruments, bool& hasLoopPoint, int& loopStart, int& loopEnd);
	void BTMidiToMidi(byte* inputMID, int inputSize, CString outFileName, int& numberInstruments, bool& hasLoopPoint, int& loopStart, int& loopEnd);
	void GloverMidiToMidi(byte* inputMID, int inputSize, CString outFileName, int& numberInstruments, unsigned long division);
	void MIDxMidiToMidi(byte* inputMID, int inputSize, CString outFileName, int& numberInstruments);
	void MMLToMidi(byte* inputMID, int inputSize, CString outFileName, int& numberInstruments);
	bool MidiToGEFormat(CString input, CString output, bool loop, unsigned long loopPoint, bool useRepeaters);
	bool AddLoopGEFormat(byte* inputMID, CString output, int inputSize, bool loop, unsigned long loopPoint, bool useRepeaters);
	bool MidiToBTFormatStageOne(CString input, CString output, bool loop, unsigned long loopPoint, bool useRepeaters, unsigned short& numTracks);
	bool MidiToBTFormat(CString input, CString output, bool loop, unsigned long loopPoint, bool useRepeaters);
	void MidiToDebugTextFile(CString midiFile, CString textFileOut);
	void ExportToBin(unsigned char* buffer, unsigned long address, unsigned long size, CString fileName, bool& compressed);
	void ExportToMidi(CString gameName, unsigned char* gamebuffer, int gamebufferSize, unsigned long address, unsigned long size, CString fileName, CString gameType, int& numberInstruments, unsigned long division, bool& compressed, bool& hasLoopPoint, int& loopStart, int& loopEnd);
	byte* Decompress(unsigned char* Buffer, unsigned long size, int& fileSize, int& compressedSize);
	bool DecompressToFile(unsigned char* Buffer, unsigned long size, CString outputFile);
private:
	int HexToInt(char inChar);
	unsigned short Flip16Bit(unsigned short ShortValue);
	unsigned long Flip32Bit(unsigned long inLong);

	unsigned long CharArrayToLong(unsigned char* currentSpot);
	unsigned short CharArrayToShort(unsigned char* currentSpot);
	void WriteLongToBuffer(unsigned char* Buffer, unsigned long address, unsigned long data);
	void WriteShortToBuffer(unsigned char* Buffer, unsigned long address, unsigned short data);

	unsigned long GetVLBytes(byte* vlByteArray, int& offset, unsigned long& original, byte*& altPattern, byte& altOffset, byte& altLength, bool includeFERepeats);
	void WriteVLBytes(FILE* outFile, unsigned long value, unsigned long length, bool includeFERepeats);
	byte ReadMidiByte(byte* vlByteArray, int& offset, byte*& altPattern, byte& altOffset, byte& altLength, bool includeFERepeats);

	unsigned long StringHexToLong(CString inString);
	unsigned char StringToUnsignedChar(CString inString);
	unsigned short Flip16bit(unsigned short tempShort);

	int numberTracks;
	TrackEvent** trackEvents;
	int trackEventCount[0x20];

	GECompression* compress;
	CString mainFolder;

	BOOL hiddenExec (PTSTR pCmdLine, CString currentDirectory);
};
