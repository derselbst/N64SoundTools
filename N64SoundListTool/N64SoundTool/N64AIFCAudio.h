#pragma once
#include "StdAfx.h"
#include "afxwin.h"
#include "afxcmn.h"

#pragma once

enum    {AL_ADPCM_WAVE = 0, AL_RAW16_WAVE = 1};

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
};


struct ALEnv
{
	unsigned long	attackTime;
	unsigned long	decayTime;
	unsigned long	releaseTime;
	unsigned short attackVolume;
	unsigned short decayVolume;
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
	ALSound	**sounds;
};

struct ALBank
{
	unsigned short	count;
	unsigned short	flags;
	unsigned short	pad;
	unsigned short	samplerate;
	unsigned long	percussion;
	ALInst**			inst;
};

struct ctlTblResult
{
	unsigned long ctlOffset;
	unsigned long ctlSize;
	unsigned long tblOffset;
	unsigned long tblSize;
	ALBank* bank;
};

class CN64AIFCAudio
{
public:
	CN64AIFCAudio(void);
	~CN64AIFCAudio(void);

	static bool InjectCtlTblIntoROMArrayInPlace(unsigned char* ROM, unsigned char* ctl, int ctlSize, unsigned char* tbl, int tblSize, unsigned long ctlOffset, unsigned long tblOffset, unsigned long maxCtl, unsigned long maxTbl);
	static void DisposeALBank(ALBank*& alBank);
	static ALBank* ReadAudio(unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl);
	static void WriteAudio(ALBank*& alBank, unsigned char*& ctl, int& ctlSize, unsigned char*& tbl, int& tblSize);
	static void WriteAudioToFile(ALBank*& alBank, CString outFileNameCtl, CString outFileNameTbl);
	static bool ReadWavData(CString rawWavFileName, unsigned char*& rawData, int& rawLength, unsigned long& samplingRate);
	static bool ReplaceSoundWithWavData(ALBank*& alBank, int instrument, int sound, CString rawWavFileName, unsigned long& samplingRate, bool newType);
	static CString CompareALBanks(ALBank* alBank1, ALBank* alBank2);
	static bool ExtractRawSound(ALBank* alBank, int instrument, int sound, CString outputFile, unsigned long samplingRate);
	static bool AddSound(ALBank*& alBank, int instrument, CString rawWavFileName, unsigned long& samplingRate, bool type);
	static bool AddSound(ALBank*& alBank, int instrument);
	static void DeleteSound(ALBank*& alBank, int instrument, int sound);
	static void MoveUpSound(ALBank*& alBank, int instrument, int sound);
	static void MoveDownSound(ALBank*& alBank, int instrument, int sound);
	static void ExportPredictors(ALBank*& alBank, int instrument, int sound, CString fileName);
	static void ImportPredictors(ALBank*& alBank, int instrument, int sound, CString fileName);
	static void ExportRawData(ALBank*& alBank, int instrument, int sound, CString fileName);
	static void ImportRawData(ALBank*& alBank, int instrument, int sound, CString fileName);
	static int ReadCtlTblLocations(unsigned char* ROM, int romSize, ctlTblResult*& results);

	static unsigned long decode( unsigned char *in, signed short *out, unsigned long len, ALADPCMBook *book );
	static void encode(signed short* inPCMSamples, int numberSamplesIn, unsigned char* outVADPCM, unsigned long& lenOut, ALADPCMBook *book);

private:
	static unsigned long CharArrayToLong(unsigned char* currentSpot);
	static unsigned long Flip32Bit(unsigned long inLong);
	static unsigned short Flip16Bit(unsigned short ShortValue);
	static unsigned short CharArrayToShort(unsigned char* currentSpot);
	static float CharArrayToFloat(unsigned char* currentSpot);
	static void WriteLongToBuffer(unsigned char* Buffer, unsigned long address, unsigned long data);
	static void WriteShortToBuffer(unsigned char* Buffer, unsigned long address, unsigned short data);
	static int GetSizeFile(CString filename);

	static void decode_8( unsigned char *in, signed short *out , int index, signed short * pred1, signed short lastsmp[8]);
	static int determineBestEncodeIndex(signed short* pred1, signed short* pred2, signed short* lastSampleSet, signed short* inPCMSamples);
	static signed short* determineBestPredictors(signed short* inPCMSamples, int numberSamplesIn);
};

