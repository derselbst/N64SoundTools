#pragma once

#ifdef MIDI_EXPORTS
#define MIDI_API __declspec(dllexport)
#else
#define MIDI_API __declspec(dllimport)
#endif


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


class CMidiParse
{
public:


	MIDI_API CMidiParse(void);
	MIDI_API ~CMidiParse(void);

	MIDI_API void GEMidiToMidi(byte* inputMID, int inputSize, CString outFileName, int& numberInstruments);
	MIDI_API void BTMidiToMidi(byte* inputMID, int inputSize, CString outFileName, int& numberInstruments);
	MIDI_API void GloverMidiToMidi(byte* inputMID, int inputSize, CString outFileName, int& numberInstruments, unsigned long division);
	MIDI_API void MIDxMidiToMidi(byte* inputMID, int inputSize, CString outFileName, int& numberInstruments);
	MIDI_API void MMLToMidi(byte* inputMID, int inputSize, CString outFileName, int& numberInstruments);
	MIDI_API void ImportMidiConfig(CString configFile);
	MIDI_API bool MidiToGEFormat(CString input, CString output, bool loop, unsigned long loopPoint, bool useRepeaters);
	MIDI_API bool AddLoopGEFormat(byte* inputMID, CString output, int inputSize, bool loop, unsigned long loopPoint, bool useRepeaters);
	MIDI_API bool MidiToBTFormatStageOne(CString input, CString output, bool loop, unsigned long loopPoint, bool useRepeaters);
	MIDI_API bool MidiToBTFormat(CString input, CString output, bool loop, unsigned long loopPoint, bool useRepeaters);
	MIDI_API void MidiToDebugTextFile(CString midiFile, CString textFileOut);
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

	byte instrumentMidiToGEMapping[0x100];
	static byte drumMidiToGEMapping[0x100];
	static CString instrumentMidiNames[0x100];
	static CString drumMidiNames[0x100];
	static CString instrumentGEMidiNames[0x100];
	static CString drumGEMidiNames[0x100];

	int numberTracks;
	TrackEvent** trackEvents;
	int trackEventCount[0x20];

};
