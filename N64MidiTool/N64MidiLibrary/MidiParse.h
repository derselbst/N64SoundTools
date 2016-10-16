#pragma once

#include "MidiExports.h"
#include <vector>

class GECompression;

#define MAXCHANNELS 1024

struct ExtraGameMidiInfo
{
	bool zlbCompressed;
	bool naganoCompressed;

	unsigned long trackOffset;
	unsigned long compressedFileOffset;
	unsigned long compressedFileEndOffset;

	unsigned long trackIncrement;

	unsigned long trackAddressOffset;
	unsigned long trackAddressEndsOffset;
	unsigned long checksum1;
	unsigned long checksum2;
	unsigned long tracksEnd;

	ExtraGameMidiInfo()
	{
		zlbCompressed = false;
		naganoCompressed = false;

		trackOffset = 0x00000000;
		compressedFileOffset = 0x00000000;
		compressedFileEndOffset = 0x00000000;
		trackIncrement = 0x00000000;

		trackAddressOffset = 0x00000000;
		trackAddressEndsOffset = 0x00000000;

		checksum1 = 0x00000000;
		checksum2 = 0x00000000;

		tracksEnd = 0x00000000;
	}
};

struct TimeAndValue
{
	unsigned long absoluteTime;
	unsigned long value;

	TimeAndValue(unsigned long absoluteTime, unsigned long value)
	{
		this->absoluteTime = absoluteTime;
		this->value = value;
	}
};

struct timeAndValueSortByTime
{
    inline bool operator() (const TimeAndValue& struct1, const TimeAndValue& struct2)
    {
        return (struct1.absoluteTime < struct2.absoluteTime);
    }
};

struct SngTimeValue
{
	unsigned long startAbsoluteTime;
	unsigned long endAbsoluteTime;

	unsigned char value;
};

struct SngNoteInfo
{
	unsigned long startAbsoluteTime;
	unsigned long endAbsoluteTime;
	int noteNumber;
	unsigned char velocity;

	unsigned long instrument;

	int pan;
	int volume;
	unsigned char pitchBend;

	unsigned long tempo;

	unsigned char effect;

	int originalTrack;
	int originalNoteUniqueId;

	bool ignoreNoteOn;

	SngNoteInfo()
	{
		volume = 0x7F;
		pitchBend = 0x00;
		pan = 0x40;

		originalTrack = 0;
		originalNoteUniqueId = 0;

		effect = 0;

		ignoreNoteOn = false;
	}
};

struct SngNoteInfoMidiImport : SngNoteInfo
{
	unsigned char originalController;
	SngNoteInfoMidiImport()
	{
		originalController = 0;	
		endAbsoluteTime = 0xFFFFFFFF;
	}
};

struct sngSortByStartTime
{
    inline bool operator() (const SngNoteInfo& struct1, const SngNoteInfo& struct2)
    {
        return (struct1.startAbsoluteTime < struct2.startAbsoluteTime);
    }
};

struct sngSortByEndTime
{
    inline bool operator() (const SngNoteInfo& struct1, const SngNoteInfo& struct2)
    {
        return (struct1.endAbsoluteTime < struct2.endAbsoluteTime);
    }
};

struct TrackEvent // referenced in order, but first = 2710, doors refer to 27XX + preset to it implicitly, 0x44 per
{
	bool obsoleteEvent;
	unsigned long deltaTime;
	unsigned long durationTime;
	unsigned long absoluteTime;
	byte type;
	byte* contents;  // remember delete all track events mem later
	int contentSize;

	TrackEvent()
	{
		type = 0x00;

		contents = NULL;
		contentSize = 0;

		deltaTime = 0;
		absoluteTime = 0;
		obsoleteEvent = false;
		durationTime = 0;
	}
};


class MIDI_API CMidiParse
{
public:
	enum SngStyle { Normal, Old, OldBfx, Bfx, PtrBfx, OldDD };

	CMidiParse(void);
	~CMidiParse(void);

	void GEMidiToMidi(byte* inputMID, int inputSize, CString outFileName, int& numberInstruments, bool& hasLoopPoint, int& loopStart, int& loopEnd, bool extendTracksToHighest);
	void BTMidiToMidi(byte* inputMID, int inputSize, CString outFileName, int& numberInstruments, bool& hasLoopPoint, int& loopStart, int& loopEnd, bool extendTracksToHighest);
	void MIDxMidiToMidi(byte* inputMID, int inputSize, CString outFileName, int& numberInstruments);
	int GetSngVariableLength(unsigned char* buffer, unsigned long offset);
	int WriteSngVariableLength(unsigned char* buffer, int& offset, int value);
	void WriteSngToMidiTrack(FILE* outFile, FILE* outDebug, std::vector<TrackEvent> trackEvents);
	void ParseSngTrack(int trackNumber, int& numberInstruments, std::vector<TimeAndValue>& tempoPositions, std::vector<SngNoteInfo>& outputNotes, unsigned char* buffer, unsigned long offset, unsigned long end, unsigned short* instrumentLookup, unsigned long adsrPointer, unsigned long drumPointer, std::vector<SngTimeValue> volumeByAbsoluteTime, std::vector<SngTimeValue> pitchBendByAbsoluteTime, SngStyle sngStyle, int& noteUniqueId, int totalInstruments);
	void SngToMidi(byte* inputMID, int inputSize, CString outFileName, int& numberInstruments, bool calculateInstrumentCountOnly, bool separateByInstrument, unsigned long extra);
	void SngToDebugTextFile(CString gameName, unsigned long address, byte* inputMID, int inputSize, CString textFileOut, unsigned long extra);
	void SngToDebugTextFile(CString gameName, unsigned long address, CString midiFile, CString textFileOut, unsigned long extra);
	void SngTrackToDebugTextFile(FILE* outFile, unsigned char* inputMID, unsigned long offset, unsigned long end, unsigned long instrumentPointer, unsigned long adsrPointer, unsigned long drumPointer, SngStyle sngStyle, int totalInstruments);
	bool MidiToGEFormat(CString input, CString output, bool loop, unsigned long loopPoint, bool useRepeaters);
	bool AddLoopGEFormat(byte* inputMID, CString output, int inputSize, bool loop, unsigned long loopPoint, bool useRepeaters);
	bool MidiToBTFormatStageOne(CString input, CString output, bool loop, unsigned long loopPoint, bool useRepeaters, unsigned short& numTracks);
	bool MidiToBTFormat(CString input, CString output, bool loop, unsigned long loopPoint, bool useRepeaters);
	bool MidiToSngList(CString input, std::vector<SngNoteInfoMidiImport>& sngNoteList, std::vector<TimeAndValue>& tempoPositions, std::vector<SngNoteInfoMidiImport> channels[MAXCHANNELS], int& numChannels, std::vector<int>& instruments, int& lowestAbsoluteTime, int& highestAbsoluteTime, bool loop, unsigned long& loopPoint);
	bool MidiToSng(CString input, CString output, bool loop, unsigned long loopPoint, SngStyle sngStyle, unsigned char masterTrackEffect);
	bool MidiToKonami(CString input, CString output, bool loop, unsigned long loopPoint, int& numberTracks, unsigned char separationAmount, unsigned char echoLength);
	void GEMidiToDebugTextFile(CString midiFile, CString textFileOut, bool extendTracksToHighest);
	void GEMidiToDebugTextFile(byte* inputMID, int inputSize, CString textFileOut, bool extendTracksToHighest);
	void MidiToDebugTextFile(CString midiFile, CString textFileOut);
	void ExportToBin(CString gameName, unsigned char* buffer, unsigned long address, unsigned long size, CString fileName, bool& compressed);
	void ExportToMidi(CString gameName, unsigned char* gamebuffer, int gamebufferSize, unsigned long address, unsigned long size, CString fileName, CString gameType, int& numberInstruments, unsigned long division, bool& compressed, bool& hasLoopPoint, int& loopStart, int& loopEnd, bool calculateInstrumentCountOnly, bool separateByInstrument, bool generateDebugTextFile, unsigned long extra, unsigned long extra2, bool writeOutLoops, int loopWriteCount, bool extendTracksToHighest, ExtraGameMidiInfo extraGameMidiInfo);
	byte* Decompress(unsigned char* Buffer, unsigned long size, int& fileSize, int& compressedSize);
	bool DecompressToFile(unsigned char* Buffer, unsigned long size, CString outputFile);
	void GenerateTestPattern(int type, CString outputFile);

	int FindHighestKonamiLengthTrack(int trackNumber, unsigned char* buffer, unsigned long offset, unsigned long end);
	void ParseKonamiTrack(int trackNumber, int& numberInstruments, std::vector<TimeAndValue>& tempoPositions, std::vector<SngNoteInfo>& outputNotes, unsigned char* buffer, unsigned long offset, unsigned long end, int& noteUniqueId, bool writeOutLoops, int loopWriteCount, bool extendTracksToHighest, int highestTrackLength);
	void KonamiToMidi(unsigned char* ROM, int romSize, byte* inputMID, int inputSize, CString outFileName, int& numberInstruments, bool calculateInstrumentCountOnly, bool separateByInstrument, bool writeOutLoops, int loopWriteCount, bool extendTracksToHighest, ExtraGameMidiInfo extraGameMidiInfo, unsigned long extra);
	void KonamiToDebugTextFile(unsigned char* ROM, int romSize, CString gameName, unsigned long address, byte* inputMID, int inputSize, CString textFileOut, bool writeOutLoops, int loopWriteCount, bool extendTracksToHighest, ExtraGameMidiInfo extraGameMidiInfo, unsigned long extra);
	void KonamiToDebugTextFile(unsigned char* ROM, int romSize, CString gameName, unsigned long address, CString midiFile, CString textFileOut, bool writeOutLoops, int loopWriteCount, bool extendTracksToHighest, ExtraGameMidiInfo extraGameMidiInfo, unsigned long extra);
	void KonamiTrackToDebugTextFile(FILE* outFile, unsigned char* inputMID, unsigned long offset, unsigned long end, ExtraGameMidiInfo extraGameMidiInfo, bool writeOutLoops, int loopWriteCount, bool extendTracksToHighest, int highestTrackLength);

	void PaperMarioToMidi(unsigned char* ROM, int romSize, byte* inputMID, int inputSize, CString outFileName, int& numberInstruments, bool calculateInstrumentCountOnly, bool separateByInstrument, std::vector<int>& usedInstruments, std::vector<int>& usedPercussionSet, std::vector<int>& usedExtraInstruments, bool combineSegments);
	void ParsePaperMarioTrack(unsigned char* ROM, int romSize, int trackNumber, int& numberInstruments, std::vector<TimeAndValue>& tempoPositions, std::vector<SngNoteInfo>& outputNotes, unsigned char* buffer, unsigned long offset, unsigned long end, int& noteUniqueId, unsigned long drumDataPointer, int numberDrum, unsigned long instrumentDataPointer, int numberInst, unsigned short trackFlags, std::vector<int>& usedInstruments, std::vector<int>& usedPercussionSet, std::vector<int>& usedExtraInstruments, unsigned char& currentPan, unsigned long& currentInstrument, unsigned char& currentVolume, int& currentEffect, unsigned long& currentTempo, signed long& currentCoarseTune, bool& setReverb, bool& setVolume, bool& setPan, int& absoluteTimeStart);
	void PaperMarioToDebugTextFile(unsigned char* ROM, int romSize, CString gameName, unsigned long address, byte* inputMID, int inputSize, CString textFileOut, bool writeOutLoops, int loopWriteCount, bool combineSegments, ExtraGameMidiInfo extraGameMidiInfo, unsigned long extra);
	void PaperMarioToDebugTextFile(unsigned char* ROM, int romSize, CString gameName, unsigned long address, CString midiFile, CString textFileOut, bool writeOutLoops, int loopWriteCount, bool combineSegments, ExtraGameMidiInfo extraGameMidiInfo, unsigned long extra);
	void PaperMarioTrackToDebugTextFile(FILE* outFile, unsigned char* inputMID, unsigned long offset, unsigned short trackFlags);

	int DecodeFactor5DeltaTimeRLE(unsigned char* input, unsigned long& offset);
	void Factor5ToMidi(byte* inputMID, int inputSize, CString outFileName, int& numberInstruments, bool calculateInstrumentCountOnly, bool separateByInstrument, bool isRogueStyle);
	void Factor5ToDebugTextFile(CString gameName, unsigned long address, byte* inputMID, int inputSize, CString textFileOut, bool isRogueStyle);
	void Factor5ToDebugTextFile(CString gameName, unsigned long address, CString midiFile, CString textFileOut, bool isRogueStyle);

	void WriteSngList(std::vector<SngNoteInfo> sngNoteList, std::vector<TimeAndValue> tempoPositions, CString outFileName, bool separateByInstrument, unsigned short division);
	
	void WriteLongToBuffer(unsigned char* Buffer, unsigned long address, unsigned long data);
private:
	int HexToInt(char inChar);
	unsigned short Flip16Bit(unsigned short ShortValue);
	unsigned long Flip32Bit(unsigned long inLong);

	unsigned long CharArrayToLong(unsigned char* currentSpot);
	unsigned short CharArrayToShort(unsigned char* currentSpot);
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
	BOOL CMidiParse::hiddenExec (PTSTR pCmdLine, CString currentDirectory, HANDLE out);

	bool IsOverlap(float x1, float x2, float y1, float y2);
	int GetSizeFile(CString filename);
};
