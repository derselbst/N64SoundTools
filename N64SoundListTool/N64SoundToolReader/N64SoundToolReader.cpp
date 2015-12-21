#include "StdAfx.h"
#include "N64SoundToolReader.h"
#include "..\N64SoundLibrary\TigDecoder.h"

CN64SoundToolReader::CN64SoundToolReader(void)
{
}

CN64SoundToolReader::~CN64SoundToolReader(void)
{
}

int CN64SoundToolReader::HexToInt(char inChar)
{
	switch(inChar)
	{
	case '0':
		return 0;
	case '1':
		return 1;
	case '2':
		return 2;
	case '3':
		return 3;
	case '4':
		return 4;
	case '5':
		return 5;
	case '6':
		return 6;
	case '7':
		return 7;
	case '8':
		return 8;
	case '9':
		return 9;
	case 'A':
		return 10;
	case 'a':
		return 10;
	case 'B':
		return 11;
	case 'b':
		return 11;
	case 'C':
		return 12;
	case 'c':
		return 12;
	case 'D':
		return 13;
	case 'd':
		return 13;
	case 'E':
		return 14;
	case 'e':
		return 14;
	case 'F':
		return 15;
	case 'f':
		return 15;
	default:
		return 0;
	}
}

unsigned long CN64SoundToolReader::StringHexToLong(CString inString)
{
	int tempA = inString.GetLength();
	if (inString.GetLength() < 8)
	{
		CString tempStr = inString;
		inString = "";
		for (int x = 0; x < (8-tempStr.GetLength()); x++)
		{
			inString = inString + "0";
		}
		inString = inString + tempStr;
	}
	char* b;
	b = inString.GetBuffer(0);
	unsigned long tempLong = 0;
	for (int x = 0;x < 8; x++)
	{
		char tempChar = b[x];
		int hexInt = HexToInt(tempChar);
		tempLong = tempLong | hexInt<<((7-x)*4);
	}
	return tempLong;
}

void CN64SoundToolReader::InitializeSpecificGames(CString iniPath, int& countGames, SoundGameConfig*& gameConfig)
{
	FILE* inIni = fopen(iniPath, "r");
	if (inIni == NULL)
	{
		MessageBox(NULL, "Error reading gameconfigsound.ini, file must exist", "Error", NULL);
		return;
	}

	CString lastGame = "";

	countGames = 0;

	while (!feof(inIni))
	{
		char currentLine[1000];
		fgets(currentLine, 1000, inIni);

		CString lastGameNew = lastGame;

		if (currentLine[0] == '[')
		{
			int spot = 0;
			while (spot < 1000)
			{
				if (currentLine[spot] == ']')
				{
					currentLine[spot] = 0x0;
					break;
				}
				spot++;
			}

			lastGameNew.Format("%s", &currentLine[1]);
		}

		if (lastGameNew != lastGame)
		{
			countGames++;
			lastGame = lastGameNew;
		}
	}

	if (lastGame != "")
	{
		countGames++;
	}

	rewind(inIni);

	gameConfig = new SoundGameConfig[countGames];
	countGames = 0;
	lastGame = "";
	
	int soundBankCount = 0;
	int nameCount = 0;


	CString gameType = "Uncompressed";
	while (!feof(inIni))
	{
		
		char currentLine[1000];
		fgets(currentLine, 1000, inIni);

		CString lastGameNew = lastGame;
		
		CString curLineStr;
		curLineStr.Format("%s", currentLine);

		if (currentLine[0] == '/')
			continue;
		else if (currentLine[0] == '[')
		{
			int spot = 0;
			while (spot < 1000)
			{
				if (currentLine[spot] == ']')
				{
					currentLine[spot] = 0x0;
					break;
				}
				spot++;
			}

			lastGameNew.Format("%s", curLineStr.Mid(1, (spot - 1)));

			gameType = "Uncompressed";
			if (curLineStr.Find(":type=") != -1)
			{
				gameType = curLineStr.Mid((curLineStr.Find(":type=") + 6), (curLineStr.GetLength() - curLineStr.Find(":type=") - 6));
			}
		}
		else if (currentLine[0] == '|')
		{
			nameCount++;
		}
		else if (currentLine[0] == '{')
		{

		}
		else
		{
			soundBankCount++;
		}

		if (lastGameNew != lastGame)
		{
			if (lastGame != "")
			{
				gameConfig[countGames].soundBanks = new CtlTblConfig[soundBankCount];
				countGames++;
			}
			else
			{
				gameConfig[countGames].gameName = lastGameNew;
			}

			soundBankCount = 0;
			nameCount = 0;
			lastGame = lastGameNew;
			gameConfig[countGames].gameName = lastGameNew;
			gameConfig[countGames].gameType = gameType;
			gameConfig[countGames].numberSoundBanks = 0;
		}
	}

	if (lastGame != "")
	{
		gameConfig[countGames].soundBanks = new CtlTblConfig[soundBankCount];
		gameConfig[countGames].gameType = gameType;
		gameConfig[countGames].numberSoundBanks = 0;
		countGames++;
	}


	rewind(inIni);

	countGames = 0;
	lastGame = "";

	bool overrideSamplingRate = false;
	int samplingRate = 22050;

	while (!feof(inIni))
	{
		char currentLine[1000];
		fgets(currentLine, 1000, inIni);

		CString lastGameNew = lastGame;

		if (currentLine[0] == '/')
			continue;
		else if (currentLine[0] == '[')
		{
			overrideSamplingRate = false;

			int spot = 0;
			while (spot < 1000)
			{
				if (currentLine[spot] == ']')
				{
					currentLine[spot] = 0x0;
					break;
				}
				spot++;
			}

			lastGameNew.Format("%s", &currentLine[1]);
		}
		else if (currentLine[0] == '|')
		{
			gameConfig[countGames].soundBanks[gameConfig[countGames].numberSoundBanks-1].instrumentNames.push_back(&currentLine[1]);
		}
		else if (currentLine[0] == '{')
		{
			CString line;
			line.Format("%s", currentLine);

			line.Replace("{", "");
			line.Replace("}", "");
			line.Replace(" ", "");

			if ((line.Find("HalfSampling")) != -1)
			{
				gameConfig[countGames].soundBanks[gameConfig[countGames].numberSoundBanks-1].halfSamplingRate = true;
			}
			else if (line.Find("AllSamplingRate") != -1)
			{
				line.Replace("AllSamplingRate", "");
				line.Replace(":", "");
				overrideSamplingRate = true;
				samplingRate = atoi(line);
			}
			else if (line.Find("SamplingRate") != -1)
			{
				line.Replace("SamplingRate", "");
				line.Replace(":", "");
				gameConfig[countGames].soundBanks[gameConfig[countGames].numberSoundBanks-1].overrideSamplingRate = true;
				gameConfig[countGames].soundBanks[gameConfig[countGames].numberSoundBanks-1].samplingRate = atoi(line);
			}
		}
		else
		{
			CString line;
			line.Format("%s", currentLine);
			if (line.Find(",") != -1)
			{
				int commaSpot = line.Find(",");
				CString first = line.Mid(0,commaSpot);
				CString second = line.Mid((commaSpot + 1), (line.GetLength() - commaSpot - 1));
				CString third = "";
				CString fourth = "";

				int commaSpot2 = second.Find(",");
				if (commaSpot2 != -1)
				{
					third = second.Mid((commaSpot2 + 1), (second.GetLength() - commaSpot2 - 1));
					second = second.Mid(0,commaSpot2);


					int commaSpot3 = third.Find(",");
					if (commaSpot3 != -1)
					{
						fourth = third.Mid((commaSpot3 + 1), (third.GetLength() - commaSpot3 - 1));
						third = third.Mid(0,commaSpot3);
					}
				}

				second.Replace("\r", "");
				second.Replace("\n", "");
				third.Replace("\r", "");
				third.Replace("\n", "");
				fourth.Replace("\r", "");
				fourth.Replace("\n", "");

				if (overrideSamplingRate)
				{
					if (!gameConfig[countGames].soundBanks[gameConfig[countGames].numberSoundBanks].overrideSamplingRate)
					{
						gameConfig[countGames].soundBanks[gameConfig[countGames].numberSoundBanks].overrideSamplingRate = true;
						gameConfig[countGames].soundBanks[gameConfig[countGames].numberSoundBanks].samplingRate = samplingRate;
					}
				}

				gameConfig[countGames].soundBanks[gameConfig[countGames].numberSoundBanks].ctl = StringHexToLong(first);
				gameConfig[countGames].soundBanks[gameConfig[countGames].numberSoundBanks].tbl = StringHexToLong(second);
				if (third.GetLength() > 0)
				{
					if (gameConfig[countGames].gameType.Find("vox") == 0)
						gameConfig[countGames].soundBanks[gameConfig[countGames].numberSoundBanks].numberInstruments = atoi(third);
					else
						gameConfig[countGames].soundBanks[gameConfig[countGames].numberSoundBanks].numberInstruments = StringHexToLong(third);
				}
				else
					gameConfig[countGames].soundBanks[gameConfig[countGames].numberSoundBanks].numberInstruments = 0;
				if (fourth.GetLength() > 0)
					gameConfig[countGames].soundBanks[gameConfig[countGames].numberSoundBanks].mask = StringHexToLong(fourth);
				else
					gameConfig[countGames].soundBanks[gameConfig[countGames].numberSoundBanks].mask = -1;
				gameConfig[countGames].numberSoundBanks++;
				soundBankCount++;
			}
		}

		if (lastGameNew != lastGame)
		{
			if (lastGame != "")
			{
				countGames++;
			}

			lastGame = lastGameNew;

			soundBankCount = 0;
			gameConfig[countGames].numberSoundBanks = 0;
		}
	}

	if (lastGame != "")
	{
		countGames++;
	}

	fclose(inIni);
}

unsigned long CN64SoundToolReader::GetRegionSize(unsigned long currentSpot, SoundGameConfig gameConfig, unsigned long romSize)
{
	unsigned long endSpot = romSize;
	for (int x = 0; x < gameConfig.numberSoundBanks; x++)
	{
		if (gameConfig.soundBanks[x].ctl > currentSpot)
		{
			if (gameConfig.soundBanks[x].ctl < endSpot)
				endSpot = gameConfig.soundBanks[x].ctl;
		}

		if (gameConfig.soundBanks[x].tbl > currentSpot)
		{
			if (gameConfig.soundBanks[x].tbl < endSpot)
				endSpot = gameConfig.soundBanks[x].tbl;
		}
	}

	return endSpot - currentSpot;
}

void CN64SoundToolReader::ReadSoundbanks(unsigned char* ROM, int romSize, SoundGameConfig gameConfig, int& numberResults, std::vector<ctlTblResult>& results)
{
	numberResults = 0;
	if (gameConfig.gameType.Find("MultiPartERZN64WavePtrV2") == 0)
	{
		unsigned char* ctlBinaryPre = new unsigned char[0x1000000];
		unsigned long  ctlSize = 0;

		for (int x = 0; x < gameConfig.numberSoundBanks; x++)
		{
			RncDecoder rnc;
			int fileSizeCompressed = romSize - gameConfig.soundBanks[x].ctl;
			if (fileSizeCompressed > 0xFE000)
				fileSizeCompressed = 0xFE000;
			unsigned char* outputDecompressed = new unsigned char[0x1000000];
			ROM[gameConfig.soundBanks[x].ctl] = 0x52;
			ROM[gameConfig.soundBanks[x].ctl+1] = 0x4E;
			ROM[gameConfig.soundBanks[x].ctl+2] = 0x43;
			int fileSize = rnc.unpackM1(&ROM[gameConfig.soundBanks[x].ctl], outputDecompressed, 0, fileSizeCompressed);
			memcpy(&ctlBinaryPre[ctlSize], outputDecompressed, fileSize);
			ctlSize += fileSize;
			delete [] outputDecompressed;
		}

		unsigned char* ctlBinary = new unsigned char[ctlSize - gameConfig.soundBanks[0].mask];
		memcpy(ctlBinary, &ctlBinaryPre[gameConfig.soundBanks[0].mask], (ctlSize - gameConfig.soundBanks[0].mask));
		delete [] ctlBinaryPre;
		ctlSize = ctlSize - gameConfig.soundBanks[0].mask;

		
		results.resize(numberResults+1);

		results[numberResults].ctlOffset = gameConfig.soundBanks[0].ctl;
		results[numberResults].ctlSize = ctlSize;
		results[numberResults].tblOffset = gameConfig.soundBanks[0].tbl;
		results[numberResults].tblSize = romSize - results[0].tblOffset;
		results[numberResults].bank = n64AudioLibrary.ReadAudioN64PtrWavetableV2(ctlBinary, ctlSize, 0, &ROM[results[0].tblOffset]);
		delete [] ctlBinary;
		numberResults = 1;
	}
	else if (gameConfig.gameType.Find("MultiPartTigWavePtrV2") == 0)
	{
		unsigned char* ctlBinaryPre = new unsigned char[0x1000000];
		unsigned long  ctlSize = 0;

		int currentIndex = -1;

		int start = 0;

		for (int x = 0; x < gameConfig.numberSoundBanks; x++)
		{
			if ((int)gameConfig.soundBanks[x].numberInstruments < currentIndex)
			{
				results.resize(numberResults+1);

				results[numberResults].ctlOffset = gameConfig.soundBanks[start].ctl;
				results[numberResults].ctlSize = ctlSize;
				results[numberResults].tblOffset = gameConfig.soundBanks[start].tbl;
				results[numberResults].tblSize = romSize - results[numberResults].tblOffset;
				results[numberResults].bank = n64AudioLibrary.ReadAudioN64PtrWavetableV2(ctlBinaryPre, ctlSize, 0, &ROM[results[numberResults].tblOffset]);
				delete [] ctlBinaryPre;
				numberResults++;

				start = x;
				currentIndex = 0;
				// start a new one

				ctlBinaryPre = new unsigned char[0x1000000];
				ctlSize = 0;
			}

			TigDecoder tig;
			int fileSizeCompressed = romSize - gameConfig.soundBanks[x].ctl;
			if (fileSizeCompressed > 0xFE000)
				fileSizeCompressed = 0xFE000;
			unsigned char* outputDecompressed = new unsigned char[0x1000000];
			
			unsigned long address = gameConfig.soundBanks[x].ctl;
			unsigned long compressedsize = ((((((ROM[address+3] << 8) | ROM[address+2]) << 8) | ROM[address+1]) << 8) | ROM[address+0]);
			unsigned char type = ROM[address+4];

			if (type == 1)  // no compression
			{
				memcpy(&ctlBinaryPre[ctlSize], &ROM[address+8], compressedsize-8);
				ctlSize += compressedsize-8;
			}
			else if (type == 0)
			{
				int fileSize = tig.dec(&ROM[address+8], compressedsize, outputDecompressed);
				if (fileSize > 0x1000)
					fileSize = 0x1008;
				memcpy(&ctlBinaryPre[ctlSize], outputDecompressed, fileSize-8);
				ctlSize += fileSize-8;
			}

			delete [] outputDecompressed;

			currentIndex++;
		}

		results.resize(numberResults + 1);

		results[numberResults].ctlOffset = gameConfig.soundBanks[start].ctl;
		results[numberResults].ctlSize = ctlSize;
		results[numberResults].tblOffset = gameConfig.soundBanks[start].tbl;
		results[numberResults].tblSize = romSize - results[numberResults].tblOffset;
		results[numberResults].bank = n64AudioLibrary.ReadAudioN64PtrWavetableV2(ctlBinaryPre, ctlSize, 0, &ROM[results[numberResults].tblOffset]);
		delete [] ctlBinaryPre;
		
		numberResults++;
	}
	else
	{
		for (int x = 0; x < gameConfig.numberSoundBanks; x++)
		{
			results.resize(numberResults+1);

			results[numberResults].ctlOffset = gameConfig.soundBanks[x].ctl;
			results[numberResults].tblOffset = gameConfig.soundBanks[x].tbl;

			results[numberResults].ctlSize = GetRegionSize(results[numberResults].ctlOffset, gameConfig, romSize);
			results[numberResults].tblSize = GetRegionSize(results[numberResults].tblOffset, gameConfig, romSize);
			
			if (gameConfig.gameType.Find("RawTest") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioRawTest(&ROM[0], results[numberResults].ctlSize);
			}
			else if (gameConfig.gameType.Find("SF64Uncompressed") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioStarFox(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments, gameConfig.soundBanks[x].mask);
			}
			else if (gameConfig.gameType.Find("ZeldaUncompressed") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioZelda(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments, gameConfig.soundBanks[x].mask, &ROM[0]);
			}
			else if (gameConfig.gameType.Find("TurokUncompressed") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioTurok(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments, gameConfig.soundBanks[x].mask, &ROM[0], 0);
			}
			else if (gameConfig.gameType.Find("ArmyMenCompressed") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioArmyMenCompressed(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], true, false);
			}
			else if (gameConfig.gameType.Find("SargeArmyMenCompressed") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioArmyMenCompressed(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], true, true);
			}
			else if (gameConfig.gameType.Find("ArmyMenAirCombatCompressed") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioArmyMenCompressed(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], false, false);
			}
			else if (gameConfig.gameType.Find("SnoWave") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioSno(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments);
			}
			else if (gameConfig.gameType.Find("RNCCompressed") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadRNCAudio(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments);
			}
			else if (gameConfig.gameType.Find("RNXCompressed") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadRNXAudio(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments);
			}
			else if (gameConfig.gameType.Find("FZeroUncompressed") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioFZero(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments, &ROM[0]);
			}
			else if (gameConfig.gameType.Find("MegaManN64WavePtrTableV2") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioMegamanN64PtrWavetableV2(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
			}
			else if (gameConfig.gameType.Find("N64WavePtrTableV2") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioN64PtrWavetableV2(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
			}
			else if (gameConfig.gameType.Find("PaperMario") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioPaperMario(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
			}
			else if (gameConfig.gameType.Find("B0") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioB0(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
			}
			else if (gameConfig.gameType.Find("DuckDodgers") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioDuckDodgers(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
			}
			else if (gameConfig.gameType.Find("SydneyWavePtr") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioSydney(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments);
			}
			else if (gameConfig.gameType.Find("NinDec") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioNinDec(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments);
			}
			else if (gameConfig.gameType.Find("TazHuffmanWavePtr") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioTazN64PtrWavetableV2(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
			}
			else if (gameConfig.gameType.Find("RNCN64Wave") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioRNCN64Ptr(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
			}
			else if (gameConfig.gameType.Find("RNCN64OffsetWave") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioRNCN64PtrOffset(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments);
			}
			else if (gameConfig.gameType.Find("BnkB") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioBnkB(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
			}
			else if (gameConfig.gameType.Find("N64WavePtrTableV1") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioN64PtrWavetableV1(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
			}
			else if (gameConfig.gameType.Find("H20Raw816") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioH20Raw816(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments);
			}
			else if (gameConfig.gameType.Find("TetrisphereRaw816") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioTetrisphereRaw816(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments);
			}
			else if (gameConfig.gameType.Find("YAY0N64WavePtrTableV2") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioN64PtrWavetableV2YAY0(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
			}
			else if (gameConfig.gameType.Find("Titus") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioN64Titus(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
			}
			else if (gameConfig.gameType.Find("MKMythologies") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioN64MKMythologies(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments);
			}
			else if (gameConfig.gameType.Find("BlitzN64WavePtrTableV2") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioN64PtrWavetableBlitz(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
			}	
			else if (gameConfig.gameType.Find("ZLibN64WavePtrTableV2") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioN64PtrWavetableV2ZLIB(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
			}
			else if (gameConfig.gameType.Find("ZLIBSN64") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioN64SN64Zlib(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments, gameConfig.soundBanks[x].mask);
			}
			else if (gameConfig.gameType.Find("SN64") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioN64SN64(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments, gameConfig.soundBanks[x].mask);
			}
			else if (gameConfig.gameType.Find("SuperMario64") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioMario(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], results[numberResults].tblSize, gameConfig.soundBanks[x].numberInstruments);
			}
			else if (gameConfig.gameType.Find("MarioKart64") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioMario(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], results[numberResults].tblSize, true, gameConfig.soundBanks[x].numberInstruments);
			}
			else if (gameConfig.gameType.Find("StandardRawAllowed") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioRawAllowed(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments, gameConfig.soundBanks[x].mask, 0);
			}
			else if (gameConfig.gameType.Find("64dd") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudio64dd(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
			}
			else if (gameConfig.gameType.Find("vox") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioVox(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments);
			}
			else if (gameConfig.gameType.Find("BanjoTooie") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioBanjoTooie(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], 0, 0xFFFFFFFF, gameConfig.soundBanks[x].numberInstruments);
			}
			else if (gameConfig.gameType.Find("Conker") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioConker(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, gameConfig.soundBanks[x].numberInstruments, &ROM[results[numberResults].tblOffset]);
			}
			else if (gameConfig.gameType.Find("LzMusyx") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioLzMusyx(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments, gameConfig.soundBanks[x].mask);
			}
			else if (gameConfig.gameType.Find("ZLib78DAMusyx") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioZLibMusyx78DA(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments, gameConfig.soundBanks[x].mask);
			}
			else if (gameConfig.gameType.Find("ZLibMusyx") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioZLibMusyx(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments, gameConfig.soundBanks[x].mask, GAUNTLETLEGENDS);
			}
			else if (gameConfig.gameType.Find("MusyxSmallZlib") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioZLibMusyxSmall(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments, gameConfig.soundBanks[x].mask);
			}
			else if (gameConfig.gameType.Find("MusyxREZLib") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioZLibMusyx(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments, gameConfig.soundBanks[x].mask, RESIDENTEVIL2);
			}
			else if (gameConfig.gameType.Find("Musyx") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioMusyx(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments, gameConfig.soundBanks[x].mask);
			}
			else if (gameConfig.gameType.Find("Musyx") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioMusyx(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments, gameConfig.soundBanks[x].mask);
			}
			else if (gameConfig.gameType.Find("BlastCorps") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioBlastCorps(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
			}
			else if (gameConfig.gameType.Find("MarioParty2E") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioMarioParty2E(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], 0, 0);
			}
			else
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudio(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], 0, 0xFFFFFFFF, gameConfig.soundBanks[x].numberInstruments);
			}

			if (results[numberResults].bank != NULL)
			{
				results[numberResults].halfSamplingRate = gameConfig.soundBanks[x].halfSamplingRate;
				results[numberResults].overrideSamplingRate = gameConfig.soundBanks[x].overrideSamplingRate;
				results[numberResults].samplingRate = gameConfig.soundBanks[x].samplingRate;
				numberResults++;
			}
			else
				results.resize(numberResults);
		}
	}
}