#pragma once

#ifdef DLL_CONFIG
	#ifdef SOUND_READER_EXPORTS
		#define SOUND_READER_API __declspec(dllexport)
	#else
		#define SOUND_READER_API __declspec(dllimport)
	#endif
#else
	#define SOUND_READER_API
#endif

#define SOUND_READER_API

#include <vector>
#include "..\N64SoundLibrary\N64AIFCAudio.h"

struct CtlTblConfig
{
	unsigned long ctl;
	unsigned long tbl;
	unsigned long numberInstruments;
	unsigned long mask;
	unsigned long extra;
	unsigned long extra2;
	std::vector<CString> instrumentNames;
	bool halfSamplingRate;
	int samplingRate;
	bool overrideSamplingRate;

	CtlTblConfig()
	{
		halfSamplingRate = false;

		overrideSamplingRate = false;
		samplingRate = 22050;

		numberInstruments = 0;
		mask = 0xFFFFFFFF;
		extra = 0;
		extra2 = 0;
	}
};

struct SoundGameConfig
{
	int numberSoundBanks;
	CString gameType;
	CString gameName;
	CtlTblConfig* soundBanks;

	SoundGameConfig()
	{
		soundBanks = NULL;

		numberSoundBanks = 0;
	}
};

class SOUND_READER_API CN64SoundToolReader
{
public:
	CN64SoundToolReader(void);
	~CN64SoundToolReader(void);

	static int HexToInt(char inChar);
	static unsigned long StringHexToLong(CString inString);
	static void InitializeSpecificGames(CString iniPath, int& countGames, SoundGameConfig*& gameConfig);

	static unsigned long GetRegionSize(unsigned long currentSpot, SoundGameConfig gameConfig, unsigned long romSize);

	static void ReadSoundbanks(unsigned char* ROM, int romSize, SoundGameConfig gameConfig, int& numberResults, std::vector<ctlTblResult>& results);
	static CN64AIFCAudio n64AudioLibrary;
};
