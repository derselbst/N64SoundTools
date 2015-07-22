#include "StdAfx.h"
#include "N64MidiToolReader.h"
#include "..\N64MidiLibrary\SupermanDecoder.h"
#include "..\N64MidiLibrary\rnc_deco.h"
#include "..\N64MidiLibrary\H20Decoder.h"
#include "..\N64MidiLibrary\TetrisphereDecoder.h"
#include "..\N64MidiLibrary\GECompression.h"


CMidiParse CN64MidiToolReader::midiParse;
GECompression CN64MidiToolReader::compress;

CN64MidiToolReader::CN64MidiToolReader(void)
{
}

CN64MidiToolReader::~CN64MidiToolReader(void)
{
}

int CN64MidiToolReader::HexToInt(char inChar)
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

unsigned long CN64MidiToolReader::CharArrayToLong(unsigned char* currentSpot)
{
	return Flip32Bit(*reinterpret_cast<unsigned long*> (currentSpot));
}

unsigned long CN64MidiToolReader::Flip32Bit(unsigned long inLong)
{
	return (((inLong & 0xFF000000) >> 24) | ((inLong & 0x00FF0000) >> 8) | ((inLong & 0x0000FF00) << 8) | ((inLong & 0x000000FF) << 24));
}

unsigned short CN64MidiToolReader::CharArrayToShort(unsigned char* currentSpot)
{
	return Flip16Bit(*reinterpret_cast<unsigned short*> (currentSpot));
}

unsigned short CN64MidiToolReader::Flip16Bit(unsigned short ShortValue)
{
	return ((ShortValue >> 8) | ((ShortValue << 8)));
}

unsigned long CN64MidiToolReader::StringHexToLong(CString inString)
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

void CN64MidiToolReader::InitializeSpecificGames(CString iniPath, int& countGames, MidiGameConfig*& gameConfig)
{
	FILE* inIni = fopen(iniPath, "r");
	if (inIni == NULL)
	{
		MessageBox(NULL, "Error reading gameconfigmidi.ini, file must exist", "Error", NULL);
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

	gameConfig = new MidiGameConfig[countGames];
	countGames = 0;
	lastGame = "";
	
	int midiCount = 0;
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
		else
		{
			midiCount++;
		}

		if (lastGameNew != lastGame)
		{
			if (lastGame != "")
			{
				if (midiCount > 0)
					gameConfig[countGames].midiBanks = new MidiConfig[midiCount];
				if (nameCount > 0)
					gameConfig[countGames].midiNames = new CString[nameCount];
				countGames++;
			}
			else
			{
				gameConfig[countGames].gameName = lastGameNew;
				gameConfig[countGames].gameName.Replace("\r", "");
				gameConfig[countGames].gameName.Replace("\n", "");
			}


			midiCount = 0;
			nameCount = 0;
			lastGame = lastGameNew;
			gameConfig[countGames].gameName = lastGameNew;
			gameConfig[countGames].gameName.Replace("\r", "");
			gameConfig[countGames].gameName.Replace("\n", "");
			gameConfig[countGames].gameType = gameType;
		}
	}

	if (lastGame != "")
	{
		if (midiCount > 0)
			gameConfig[countGames].midiBanks = new MidiConfig[midiCount];
		if (nameCount > 0)
			gameConfig[countGames].midiNames = new CString[nameCount];
		gameConfig[countGames].gameType = gameType;
		countGames++;
	}

	rewind(inIni);

	countGames = 0;
	lastGame = "";

	while (!feof(inIni))
	{
		char currentLine[1000];
		fgets(currentLine, 1000, inIni);

		CString lastGameNew = lastGame;

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

			lastGameNew.Format("%s", &currentLine[1]);
		}
		else if (currentLine[0] == '|')
		{
			gameConfig[countGames].midiNames[gameConfig[countGames].numberMidiNames].Format("%s", &currentLine[1]);
			gameConfig[countGames].numberMidiNames++;
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

				gameConfig[countGames].midiBanks[gameConfig[countGames].numberMidis].start = StringHexToLong(first);
				gameConfig[countGames].midiBanks[gameConfig[countGames].numberMidis].end = StringHexToLong(second);
				/*if (third.GetLength() > 0)
					gameConfig[countGames].soundBanks[gameConfig[countGames].numberSoundBanks].numberInstruments = CSharedFunctions::StringHexToLong(third);
				else
					gameConfig[countGames].soundBanks[gameConfig[countGames].numberSoundBanks].numberInstruments = -1;
				if (fourth.GetLength() > 0)
					gameConfig[countGames].soundBanks[gameConfig[countGames].numberSoundBanks].mask = CSharedFunctions::StringHexToLong(fourth);
				else
					gameConfig[countGames].soundBanks[gameConfig[countGames].numberSoundBanks].mask = -1;*/
				gameConfig[countGames].numberMidis++;
				midiCount++;
			}
		}

		if (lastGameNew != lastGame)
		{
			if (lastGame != "")
			{
				countGames++;
			}

			lastGame = lastGameNew;

			midiCount = 0;
			nameCount = 0;
			gameConfig[countGames].numberMidis = 0;
			gameConfig[countGames].numberMidiNames = 0;
		}
	}

	if (lastGame != "")
	{
		countGames++;
	}

	fclose(inIni);
}

void CN64MidiToolReader::ProcessMidis(MidiGameConfig* gameConfig, int gameNumber, std::vector<CString>& addMidiStrings, int& numberMidiStrings, int& numberInstruments, bool& compressed, unsigned char* buffer, int bufferSize, unsigned long& startSpot, unsigned long& endSpot, bool calculateInstrumentCount)
{
	numberInstruments = 0;

	if (gameConfig[gameNumber].gameType.Find("GoldenEye") == 0)
	{
		// TODO
		//midiParse.ImportMidiConfig("gemidiconfig.txt");
		compress.SetGame(GOLDENEYE);
		compressed = true;
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			unsigned long start = ReadAddiuAddress(buffer, gameConfig[gameNumber].midiBanks[x].start, gameConfig[gameNumber].midiBanks[x].end) + 4;

			int numberMusicData = CharArrayToShort(&buffer[start-4]);
			
			unsigned long currentSpot = start;
			
			int currentBinaryNumber = 0;
			while (currentBinaryNumber < numberMusicData)
			{
				unsigned long musicOffset = (CharArrayToLong(&buffer[currentSpot]) + (start - 4));
				unsigned long compressedSize = CharArrayToShort(&buffer[currentSpot+6]);

				CString tempSpotStr;
				tempSpotStr.Format("%08X:%08X", musicOffset, compressedSize);
				addMidiStrings.push_back(tempSpotStr);
				numberMidiStrings++;

				if (calculateInstrumentCount)
				{
					int numberInstTemp = 0;
					bool hasLoopPoint = false;
					int loopStart = 0;
					int loopEnd = 0;
					midiParse.ExportToMidi(gameConfig[gameNumber].gameName, buffer, bufferSize, musicOffset, compressedSize, "asdasdaw43.mid", gameConfig[gameNumber].gameType, numberInstTemp, 0, compressed, hasLoopPoint, loopStart, loopEnd);
					if (numberInstTemp > numberInstruments)
						numberInstruments = numberInstTemp;
					::DeleteFile("asdasdaw43.mid");
				}

				currentBinaryNumber++;
				currentSpot += 8;
			}		
		}

		
	}
	else if (gameConfig[gameNumber].gameType.Find("Glover") == 0)
	{
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			unsigned long start = gameConfig[gameNumber].midiBanks[x].start;
			unsigned long end = gameConfig[gameNumber].midiBanks[x].end;
			
			unsigned long countMidi = CharArrayToLong(&buffer[start]);
			unsigned long endMidi = CharArrayToLong(&buffer[start + 0x14]);
			for (int y = 0; y < countMidi; y++)
			{
				unsigned long midiLocation = CharArrayToLong(&buffer[start + 0x18 + (y * 8)]);
				unsigned long midiDivison = CharArrayToLong(&buffer[start + 0x18 + (y * 8) + 4]);
				unsigned long midiEnd;
				if (y == (countMidi - 1))
					midiEnd = endMidi;
				else
					midiEnd = CharArrayToLong(&buffer[start + 0x18 + (y * 8) + 8]);

				CString tempSpotStr;
				tempSpotStr.Format("%08X:%08X", midiLocation, (midiEnd - midiLocation));
				addMidiStrings.push_back(tempSpotStr);
				numberMidiStrings++;

				if (calculateInstrumentCount)
				{
					int numberInstTemp = 0;
					bool hasLoopPoint = false;
					int loopStart = 0;
					int loopEnd = 0;
					midiParse.ExportToMidi(gameConfig[gameNumber].gameName, buffer, bufferSize, (start + midiLocation), (midiEnd - midiLocation), "asdasdaw43.mid", gameConfig[gameNumber].gameType, numberInstTemp, midiDivison, compressed, hasLoopPoint, loopStart, loopEnd);
					if (numberInstTemp > numberInstruments)
						numberInstruments = numberInstTemp;
					::DeleteFile("asdasdaw43.mid");
				}
			}
		}
	}
	else if (gameConfig[gameNumber].gameType.Find("PerfectDark") == 0)
	{
		compress.SetGame(PD);
		compressed = true;

		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			unsigned long start = gameConfig[gameNumber].midiBanks[x].start;
			unsigned long currentSpot = start;
			
			startSpot = (CharArrayToLong(&buffer[currentSpot]) + (start - 4));
			
			int currentBinaryNumber = 0;
			while (currentBinaryNumber <  0x3F)
			{
				unsigned long musicOffset = (CharArrayToLong(&buffer[currentSpot]) + gameConfig[gameNumber].midiBanks[0].end);
				unsigned long compressedSize = CharArrayToShort(&buffer[currentSpot+6]);

				CString tempSpotStr;
				tempSpotStr.Format("%08X:%08X", musicOffset, compressedSize);
				addMidiStrings.push_back(tempSpotStr);
				numberMidiStrings++;

				if (calculateInstrumentCount)
				{
					int numberInstTemp = 0;
					bool hasLoopPoint = false;
					int loopStart = 0;
					int loopEnd = 0;
					midiParse.ExportToMidi(gameConfig[gameNumber].gameName, buffer, bufferSize, musicOffset, compressedSize, "asdasdaw43.mid", gameConfig[gameNumber].gameType, numberInstTemp, 0, compressed, hasLoopPoint, loopStart, loopEnd);
					if (numberInstTemp > numberInstruments)
						numberInstruments = numberInstTemp;
					::DeleteFile("asdasdaw43.mid");
				}

				endSpot = musicOffset + compressedSize;

				currentBinaryNumber++;
				currentSpot += 8;
			}
		}
	}
	else if (gameConfig[gameNumber].gameType.Find("BanjoKazooie") == 0)
	{
		compress.SetGame(BANJOKAZOOIE);
		compressed = true;
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;

			if (calculateInstrumentCount)
				{
				int numberInstTemp = 0;
				
				bool hasLoopPoint = false;
				int loopStart = 0;
				int loopEnd = 0;
				midiParse.ExportToMidi(gameConfig[gameNumber].gameName, buffer, bufferSize, gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start), "asdasdaw43.mid", gameConfig[gameNumber].gameType, numberInstTemp, 0, compressed, hasLoopPoint, loopStart, loopEnd);
				if (numberInstTemp > numberInstruments)
					numberInstruments = numberInstTemp;
				::DeleteFile("asdasdaw43.mid");
			}
		}
	}
	else if (gameConfig[gameNumber].gameType.Find("MIDx") == 0)
	{
		compressed = false;
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;

			if (calculateInstrumentCount)
			{
				int numberInstTemp = 0;
				
				bool hasLoopPoint = false;
				int loopStart = 0;
				int loopEnd = 0;
				midiParse.ExportToMidi(gameConfig[gameNumber].gameName, buffer, bufferSize, gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start), "asdasdaw43.mid", gameConfig[gameNumber].gameType, numberInstTemp, 0, compressed, hasLoopPoint, loopStart, loopEnd);
				if (numberInstTemp > numberInstruments)
					numberInstruments = numberInstTemp;
				::DeleteFile("asdasdaw43.mid");
			}
		}
	}
	else if (gameConfig[gameNumber].gameType.Find("MML") == 0)
	{
		compressed = false;
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;

			if (calculateInstrumentCount)
			{
				int numberInstTemp = 0;
				bool hasLoopPoint = false;
				int loopStart = 0;
				int loopEnd = 0;
				midiParse.ExportToMidi(gameConfig[gameNumber].gameName, buffer, bufferSize, gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start), "asdasdaw43.mid", gameConfig[gameNumber].gameType, numberInstTemp, 0, compressed, hasLoopPoint, loopStart, loopEnd);
				if (numberInstTemp > numberInstruments)
					numberInstruments = numberInstTemp;
				::DeleteFile("asdasdaw43.mid");
			}
		}
	}
	else if (gameConfig[gameNumber].gameType.Find("BanjoTooie") == 0)
	{
		compress.SetGame(BANJOTOOIE);
		compressed = true;

		// TODO
		//BT 13393bc
		//BT 138f044

		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;

			if (calculateInstrumentCount)
			{
				int numberInstTemp = 0;
				bool hasLoopPoint = false;
				int loopStart = 0;
				int loopEnd = 0;
				midiParse.ExportToMidi(gameConfig[gameNumber].gameName, buffer, bufferSize, gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start), "asdasdaw43.mid", gameConfig[gameNumber].gameType, numberInstTemp, 0, compressed, hasLoopPoint, loopStart, loopEnd);
				if (numberInstTemp > numberInstruments)
					numberInstruments = numberInstTemp;
				::DeleteFile("asdasdaw43.mid");
			}
		}

		
	}
	else if (gameConfig[gameNumber].gameType.Find("DonkeyKong") == 0)
	{
		compress.SetGame(DONKEYKONG64);
		compressed  = true;
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			unsigned long start = gameConfig[gameNumber].midiBanks[x].start;
			unsigned long  currentSpot = start;

			bool endFlag = false;

			while (!endFlag)
			{
				// algorithm to find each length, check latest offset, then find end from meta code

				unsigned long startCurrentSpot = currentSpot;
				unsigned long byteCheck = 0;

				// 00FF2F ends
				while ((buffer[currentSpot + byteCheck] != 0x1F) || (buffer[currentSpot + byteCheck + 1] != 0x8B))
				{
					byteCheck++;
				}

				// move ahead 1
				currentSpot++;

				while ((currentSpot <  gameConfig[gameNumber].midiBanks[x].end))
				{
					if ((buffer[currentSpot] != 0x1F) || (buffer[currentSpot + 1] != 0x8B) || (buffer[currentSpot + 2] != 0x08))
					{
						currentSpot+=1;
					}
					else
					{
						break;
					}
				}

				CString tempSpotStr;
				tempSpotStr.Format("%08X:%08X", startCurrentSpot, (currentSpot - startCurrentSpot));
				addMidiStrings.push_back(tempSpotStr);
				numberMidiStrings++;

				if (calculateInstrumentCount)
				{
					int numberInstTemp = 0;
					bool hasLoopPoint = false;
					int loopStart = 0;
					int loopEnd = 0;
					midiParse.ExportToMidi(gameConfig[gameNumber].gameName, buffer, bufferSize, startCurrentSpot, (currentSpot - startCurrentSpot), "asdasdaw43.mid", gameConfig[gameNumber].gameType, numberInstTemp, 0, compressed, hasLoopPoint, loopStart, loopEnd);
					if (numberInstTemp > numberInstruments)
						numberInstruments = numberInstTemp;
					::DeleteFile("asdasdaw43.mid");
				}
				
				if (currentSpot >= gameConfig[gameNumber].midiBanks[x].end)
					endFlag = true;
			}
		}	
	}
	else if (gameConfig[gameNumber].gameType.Find("Midi") == 0)
	{
		compressed = false;
		//midiParse.ImportMidiConfig("aerofightersassault.txt");
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;
		}
	}
	else if (gameConfig[gameNumber].gameType.Find("Seq64") == 0)
	{
		compressed = false;
		//midiParse.ImportMidiConfig("aerofightersassault.txt");
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;
		}
	}
	else if (gameConfig[gameNumber].gameType.Find("MultipartZLibXMFastTracker2") == 0)
	{
		compressed = true;
		//midiParse.ImportMidiConfig("aerofightersassault.txt");
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;
		}
	}
	else if (gameConfig[gameNumber].gameType.Find("DCM") == 0)
	{
		compressed = true;
		//midiParse.ImportMidiConfig("aerofightersassault.txt");
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;
		}
	}
	else if (gameConfig[gameNumber].gameType.Find("LZSamplesDCM") == 0)
	{
		compressed = true;
		//midiParse.ImportMidiConfig("aerofightersassault.txt");
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;
		}
	}
	else if (gameConfig[gameNumber].gameType.Find("TitusMidi") == 0)
	{
		compressed = true;
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;
		}
	}
	else if (gameConfig[gameNumber].gameType.Find("RNCMidi") == 0)
	{
		compressed = true;
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;
		}
	}
	else
	{
		compressed = false;
		//midiParse.ImportMidiConfig("aerofightersassault.txt");
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			ParseUncompressedType(buffer, bufferSize, gameConfig[gameNumber].midiBanks[x].start, gameConfig[gameNumber].midiBanks[x].end, addMidiStrings, numberMidiStrings, numberInstruments, compressed, buffer, calculateInstrumentCount);
		}
	}

	for (int x = 0; x < addMidiStrings.size(); x++)
	{
		if (x < gameConfig[gameNumber].numberMidiNames)
		{
			addMidiStrings[x] += " " + gameConfig[gameNumber].midiNames[x];
		}
		else
		{
			break;
		}
	}
}

unsigned long CN64MidiToolReader::ReadAddiuAddress(unsigned char* GEROM, unsigned long upperLocation, unsigned long lowerLocation)
{
	unsigned short upper = CharArrayToShort(&GEROM[upperLocation+2]);
	unsigned short lower = CharArrayToShort(&GEROM[lowerLocation+2]);
	if (lower > 0x7FFF)
	{
		return ((upper - 1) << 16) | lower;
	}
	else
	{
		return ((upper) << 16) | lower;
	}
}

void CN64MidiToolReader::ParseUncompressedType(unsigned char* gamebuffer, int gamebufferSize, unsigned long start, unsigned long endSpot, std::vector<CString>& addMidiStrings, int& numberMidiStrings, int& numberInstruments, bool& compressed, unsigned char* buffer, bool calculateInstrumentCount)
{
	unsigned long  currentSpot = start;

	bool endFlag = false;

	while (!endFlag)
	{
		// algorithm to find each length, check latest offset, then find end from meta code

		unsigned long startCurrentSpot = currentSpot;

		bool invalidMidi = false;
		unsigned long highestEnd = 0x00000044;
		for (unsigned long test = currentSpot; test < currentSpot + 0x40; test += 4)
		{
			unsigned long offsetToMidiStart = CharArrayToLong(&gamebuffer[test]);
			if (((offsetToMidiStart >> 8) & 0xFFFFFFF) == 0x00FF2F)
			{
				// blast corps 47D014 no headers, just data
				highestEnd = offsetToMidiStart + 4;
				invalidMidi = true;
				break;
			}

			unsigned long byteCheck = 0;

			if (offsetToMidiStart == 0x00000000)
				continue;

			bool endedWrongly = false;
			// FF2F ends
			while (true)
			{
				if ((gamebuffer[currentSpot + offsetToMidiStart + byteCheck] == 0xFF) && (gamebuffer[currentSpot + offsetToMidiStart + byteCheck + 1] == 0x2F))
				{
					byteCheck += 2;
					break;
				}
				if ((gamebuffer[currentSpot + offsetToMidiStart + byteCheck] == 0x00) && (gamebuffer[currentSpot + offsetToMidiStart + byteCheck + 1] == 0x00) && (gamebuffer[currentSpot + offsetToMidiStart + byteCheck + 2] == 0x00)  && (gamebuffer[currentSpot + offsetToMidiStart + byteCheck + 3] == 0x44))
				{
					endedWrongly = true;
					break;
				}
				if ((gamebuffer[currentSpot + offsetToMidiStart + byteCheck] == 0x42) && (gamebuffer[currentSpot + offsetToMidiStart + byteCheck + 1] == 0x31) && (gamebuffer[currentSpot + offsetToMidiStart + byteCheck + 2] == 0x00)  && (gamebuffer[currentSpot + offsetToMidiStart + byteCheck + 3] == 0x01))
				{
					// Toon Panic Hack
					endedWrongly = true;
					break;
				}
				byteCheck++;
				if (byteCheck > endSpot)
				{
					break;
				}
			}

			if ((offsetToMidiStart + byteCheck) > highestEnd)
				highestEnd = (offsetToMidiStart + byteCheck);
		}

		if ((highestEnd % 4) != 0)
			highestEnd += (4 - (highestEnd % 4));

		currentSpot += highestEnd;

		while ((currentSpot < endSpot))
		{
			if ((gamebuffer[currentSpot] != 0x00) || (gamebuffer[currentSpot + 1] != 0x00) || (gamebuffer[currentSpot + 2] != 0x0)  || (gamebuffer[currentSpot + 3] != 0x44) || (gamebuffer[currentSpot + 4] != 0x00))
			{
				currentSpot+=4;
			}
			else
			{
				break;
			}
		}

		if (!invalidMidi)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", startCurrentSpot, (currentSpot - startCurrentSpot));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;

			if (calculateInstrumentCount)
			{
				int numberInstTemp = 0;
				bool hasLoopPoint = false;
				int loopStart = 0;
				int loopEnd = 0;
				midiParse.ExportToMidi("Unknown", gamebuffer, gamebufferSize, startCurrentSpot, (currentSpot - startCurrentSpot), "asdasdaw43.mid", "Uncompressed", numberInstTemp, 0, compressed, hasLoopPoint, loopStart, loopEnd);
				if (numberInstTemp > numberInstruments)
					numberInstruments = numberInstTemp;
				::DeleteFile("asdasdaw43.mid");
			}
		}
		
		if (currentSpot >= endSpot)
			endFlag = true;
	}
	
	compressed  = false;
}