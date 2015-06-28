#pragma once
#include "StdAfx.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "yay0.h"
#include "rnc_deco.h"
#include "gecompression.h"
#include "BlitzDecoder.h"
#include "SupermanDecoder.h"
#include "n643docompression.h"
#include "MKMythologiesDecode.h"

#define STANDARDFORMAT 0
#define STARFOX64FORMAT 1
#define N64PTRWAVETABLETABLEV1 2
#define N64PTRWAVETABLETABLEV2 3
#define N64PTRWAVETABLETABLEV2YAY0 4
#define ZELDAFORMAT 5
#define FZEROFORMAT 6
#define TUROKFORMAT 7
#define STANDARDRNCCOMPRESSED 8
#define SUPERMARIO64FORMAT 9
#define STANDARDFORMATRAWALLOWED 10
#define ZLIBSN64 11
#define SN64 12
#define N64PTRWAVETABLETABLEV2ZLIB 13
#define N64PTRWAVETABLETABLEV2BLITZ 14
#define BNKB 15
#define TITUS 16
#define MARIOKART64FORMAT 17
#define ARMYMENFORMAT 18
#define MKMYTHOLOGIES 19
#define N64DD 20
#define STANDARDRNXCOMPRESSED 21

#pragma once

enum    {AL_ADPCM_WAVE = 0, AL_RAW16_WAVE = 1};

enum    {PRIMARY = 0, PREVIOUS = 1, SECONDARY = 2};

struct ALADPCMBook {
        unsigned long             order;
        unsigned long             npredictors;
        signed short*			predictors;
};

struct ALADPCMloop {
        unsigned long             start;
        unsigned long             end;
        unsigned long             count;
        short     state[16];

		unsigned long unknown1;
};

struct ALRawLoop{
        unsigned long             start;
        unsigned long             end;
        unsigned long             count;
};


struct ALADPCMWaveInfo
{
        ALADPCMloop     *loop;
        ALADPCMBook     *book;
};

struct ALRAWWaveInfo
{
        ALRawLoop       *loop;
};

struct ALWave
{
	unsigned long base;
	unsigned long len;
	byte type;
	byte flags;
    ALADPCMWaveInfo* adpcmWave;
    ALRAWWaveInfo*  rawWave;
	unsigned char* wavData;
	int sampleRateNotInDefaultNintendoSpec;

	unsigned long unknown1;
	unsigned long unknown2;
	unsigned long unknown3;
	unsigned long unknown4;
};


struct ALEnv
{
	unsigned long	attackTime;
	unsigned long	decayTime;
	unsigned long	releaseTime;
	unsigned short attackVolume;
	unsigned short decayVolume;

	unsigned long attackVolumeLong;
	unsigned long decayVolumeLong;
	unsigned long offset; // for uniqueness on SM64
};

struct ALKey
{
	char velocitymin;
	char velocitymax;
	char keymin;
	char keymax;
	char keybase;
	char detune;
};

struct ALSound
{
	ALEnv	env;
	ALKey	key;
	ALWave	wav;
	char	samplePan;
	char	sampleVolume;
	short	flags;

	bool hasWaveSecondary;
	ALWave	wavSecondary;

	bool hasWavePrevious;
	ALWave	wavPrevious;

	unsigned long unknown1;
	unsigned long unknown2;
	unsigned long unknown3;
	unsigned long unknown4;
};


struct ALPercussion
{
	unsigned long unknown1;
	ALWave	wav;
	unsigned long unknown2;
	ALEnv	env;
};

struct ALInst
{
	char	volume;
	char	pan;
	char	priority;
	char	flags;	
	char	tremType;
	char	tremRate;
	char	tremDepth;
	char	tremDelay;
	char	vibType;
	char	vibRate;
	char	vibDepth;
	char	vibDelay;
	short	bendRange;	
	short	soundCount;
	unsigned long countPercussion;
	unsigned long unknown2;
	unsigned long unknown3;
	
	ALSound	**sounds;
	ALPercussion* alPercussion;

	// extras
	unsigned short samplerate;

	unsigned long tblBase;
	unsigned long originalOffset;
};

struct ALBank
{
	unsigned short	count;
	unsigned short	flags;
	unsigned short	pad;
	unsigned short	samplerate;
	unsigned long	percussion;
	ALInst**			inst;

	// extras
	int soundBankFormat;
	CString bankName;
};


struct ctlTblResult
{
	unsigned long ctlOffset;
	unsigned long ctlSize;
	unsigned long tblOffset;
	unsigned long tblSize;
	ALBank* bank;
};

struct WaveInformation
{
	unsigned long	version;
	unsigned long	adpcmsize;
	unsigned long	samplesize;
	unsigned long	loop_startsamplepoint;
	unsigned long	loop_endsamplepoint;
	unsigned short	loop_count;
	unsigned char	book_bytes;
	unsigned char	samplingbaseMIDIkey;
	float	samplingrate;
	float	tuning_C4_32K;
};

class CN64AIFCAudio
{
public:
	CN64AIFCAudio(void);
	~CN64AIFCAudio(void);

	static bool InjectCtlTblIntoROMArrayInPlace(unsigned char* ROM, unsigned char* ctl, int ctlSize, unsigned char* tbl, int tblSize, unsigned long ctlOffset, unsigned long tblOffset, unsigned long maxCtl, unsigned long maxTbl);
	static void DisposeALBank(ALBank*& alBank);
	static ALBank* ReadAudio(unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl, int ctlFlaggedOffset, unsigned long mask, int bankNumber);
	static ALBank* ReadAudio64dd(unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl);
	static ALBank* ReadAudioN64PtrWavetableV2ZLIB(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl);
	static ALBank* ReadAudioRawAllowed(unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl, int ctlFlaggedOffset, unsigned long mask, int bankNumber);
	static ALBank* ReadAudioMario(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, unsigned long& tblSize, bool marioKartStyle);
	static ALBank* ReadAudioMario(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, unsigned long& tblSize);
	static ALBank* ReadAudioStarFox(unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl, int instrumentCount, unsigned long endSpot);
	static ALBank* ReadAudioZelda(unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl, int instrumentCount, unsigned long endSpot, unsigned char* rom);
	static ALBank* ReadAudioRawTest(unsigned char* rawData, int size);
	static ALBank* ReadAudioFZero(unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl, int instrumentCount, unsigned char* rom);
	static ALBank* ReadAudioN64PtrWavetableV2(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl);
	static ALBank* ReadAudioArmyMenCompressed(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, bool littleEndianCompressedHeader, bool sarge2style);
	static ALBank* ReadAudioBnkB(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl);
	static ALBank* ReadAudioN64PtrWavetableV2YAY0(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl);
	static ALBank* ReadAudioN64Titus(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl);
	static ALBank* ReadAudioN64MKMythologies(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, unsigned long uncompressedSize);
	static ALBank* ReadAudioN64PtrWavetableBlitz(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl);
	static ALBank* ReadAudioN64SN64(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, int numberInstruments, unsigned long mask);
	static ALBank* ReadAudioN64SN64Zlib(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, int numberInstruments, unsigned long mask);
	static ALBank* ReadRNCAudio(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, int bankNumber);
	static ALBank* ReadRNXAudio(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, int bankNumber);
	static ALBank* ReadAudioN64PtrWavetableV1(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl);
	static ALBank* ReadAudioTurok(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, int bankTrueOffset, unsigned long mask, unsigned char* ROM, int bankNumber);
	static void UpdateAudioOffsets(ALBank*& alBank);
	static void WriteAudio(ALBank*& alBank, unsigned char*& ctl, int& ctlSize, unsigned char*& tbl, int& tblSize);
	static bool CompareBooks(ALWave* wavIn, ALWave* wavOut);
	static bool CompareLoops(ALWave* wavIn, ALWave* wavOut);
	static bool CompareWavs(ALWave* wavIn, ALWave* wavOut);
	static bool CompareTbls(ALWave* wavIn, ALWave* wavOut);
	static void WriteAudioSuperMario(ALBank*& alBank, unsigned char*& ctl, int& ctlSize, unsigned char*& tbl, int& tblSize);
	static void WriteAudioToFile(ALBank*& alBank, CString outFileNameCtl, CString outFileNameTbl);
	static void WriteAudioN64PtrWavetableV2(ALBank*& alBank, unsigned char*& ctl, int& ctlSize, unsigned char*& tbl, int& tblSize);
	static void WriteAudioN64PtrWavetableV1(ALBank*& alBank, unsigned char*& ctl, int& ctlSize, unsigned char*& tbl, int& tblSize);
	static bool ReadWavData(CString rawWavFileName, unsigned char*& rawData, int& rawLength, unsigned long& samplingRate, bool& hasLoopData, unsigned char& keyBase, unsigned long& loopStart, unsigned long& loopEnd, unsigned long& loopCount);
	static bool ReplaceSoundWithWavData(ALBank*& alBank, int instrument, int sound, CString rawWavFileName, unsigned long& samplingRate, bool newType, byte primSel);
	static CString CompareALBanks(ALBank* alBank1, ALBank* alBank2);
	static bool ExtractRawSound(ALBank* alBank, int instrument, int sound, CString outputFile, unsigned long samplingRate, byte primSel, bool ignoreKeyBase, bool halfSamplingRate);
	static bool ExtractPercussion(ALBank* alBank, int instrument, int sound, CString outputFile, unsigned long samplingRate, bool ignoreKeyBase, bool halfSamplingRate);
	static bool AddSound(ALBank*& alBank, int instrument, CString rawWavFileName, unsigned long& samplingRate, bool type);
	static bool AddSound(ALBank*& alBank, int instrument);
	static void DeleteSound(ALBank*& alBank, int instrument, int sound);
	static void MoveUpSound(ALBank*& alBank, int instrument, int sound);
	static void MoveDownSound(ALBank*& alBank, int instrument, int sound);
	static void ExportPredictors(ALBank*& alBank, int instrument, int sound, CString fileName, byte primSel);
	static void ImportPredictors(ALBank*& alBank, int instrument, int sound, CString fileName, byte primSel);
	static void ExportRawData(ALBank*& alBank, int instrument, int sound, CString fileName, byte primSel);
	static void ImportRawData(ALBank*& alBank, int instrument, int sound, CString fileName, byte primSel);
	static int ReadCtlTblLocations(unsigned char* ROM, int romSize, ctlTblResult*& results);

	static unsigned long decode( unsigned char *in, signed short *out, unsigned long len, ALADPCMBook *book );
	static float encode(signed short* inPCMSamples, int numberSamplesIn, unsigned char* outVADPCM, unsigned long& lenOut, ALADPCMBook *book);
	static int GetSizeFile(CString filename);
private:
	static unsigned long CharArrayToLong(unsigned char* currentSpot);
	static unsigned long Flip32Bit(unsigned long inLong);
	static unsigned short Flip16Bit(unsigned short ShortValue);
	static unsigned short CharArrayToShort(unsigned char* currentSpot);
	static float CharArrayToFloat(unsigned char* currentSpot);
	static void WriteLongToBuffer(unsigned char* Buffer, unsigned long address, unsigned long data);
	static void WriteShortToBuffer(unsigned char* Buffer, unsigned long address, unsigned short data);

	static void decode_8( unsigned char *in, signed short *out , int index, signed short * pred1, signed short lastsmp[8]);
	static int determineBestEncodeIndex(signed short* pred1, signed short* pred2, signed short* lastSampleSet, signed short* inPCMSamples, float& bestFitIndex);
	static signed short* determineBestPredictors(ALBank* alBank, unsigned long& npredictors, unsigned long& norder, signed short* inPCMSamples, int numberSamplesIn);

	static void selectionSortAscending(float *array, signed short** arrayCopy, int length);

	static float keyTable[0xFF];
	static void InitializeKeyTable();
};

