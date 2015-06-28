// N64MidiToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "N64MidiTool.h"
#include "N64MidiToolDlg.h"
#include "FolderDialog.h"
#include <sys/stat.h>
#include "SharedFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CN64MidiToolDlg dialog




CN64MidiToolDlg::CN64MidiToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CN64MidiToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CN64MidiToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBOGAME, m_game);
	DDX_Control(pDX, IDC_COMBOMIDI, m_spot);
	DDX_Control(pDX, IDC_CHECKLOOP, m_loop);
	DDX_Control(pDX, IDC_EDITNUMINSTRUMENTS, m_numberInstruments);
	DDX_Control(pDX, IDC_EDITLOOPPOINT, mLoopPoint);
	DDX_Control(pDX, IDC_CHECKLOOP2, mNoRepeaters);
}

BEGIN_MESSAGE_MAP(CN64MidiToolDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTONLOADROM, &CN64MidiToolDlg::OnBnClickedButtonloadrom)
	ON_CBN_SELCHANGE(IDC_COMBOMIDI, &CN64MidiToolDlg::OnCbnSelchangeCombomidi)
	ON_BN_CLICKED(IDC_BUTTONExportBin, &CN64MidiToolDlg::OnBnClickedButtonexportbin)
	ON_BN_CLICKED(IDC_BUTTONExportMidi, &CN64MidiToolDlg::OnBnClickedButtonexportmidi)
	ON_BN_CLICKED(IDC_BUTTONImportBin, &CN64MidiToolDlg::OnBnClickedButtonimportbin)
	ON_BN_CLICKED(IDC_BUTTONImportMidi, &CN64MidiToolDlg::OnBnClickedButtonimportmidi)
	ON_BN_CLICKED(IDC_BUTTONEXPORTALLTOMIDI, &CN64MidiToolDlg::OnBnClickedButtonexportalltomidi)
	ON_BN_CLICKED(IDC_BUTTONEXPORTALLTORAWBIN, &CN64MidiToolDlg::OnBnClickedButtonexportalltorawbin)
	ON_BN_CLICKED(IDC_BUTTONWriteROM, &CN64MidiToolDlg::OnBnClickedButtonwriterom)
	ON_BN_CLICKED(IDC_BUTTONPLAYMIDI, &CN64MidiToolDlg::OnBnClickedButtonplaymidi)
	ON_BN_CLICKED(IDC_BUTTONIMPORTINSTRUMENTS, &CN64MidiToolDlg::OnBnClickedButtonimportinstruments)
	ON_BN_CLICKED(IDC_BUTTONSCANDIRFORMIDIS, &CN64MidiToolDlg::OnBnClickedButtonscandirformidis)
	ON_BN_CLICKED(IDC_BUTTONRIPROMS, &CN64MidiToolDlg::OnBnClickedButtonriproms)
	ON_BN_CLICKED(IDC_BUTTON4, &CN64MidiToolDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CN64MidiToolDlg message handlers

BOOL CN64MidiToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	mLoopPoint.SetWindowText("0");
	int iniOption;
	char tempFolder[8000];
	::GetCurrentDirectory(8000, tempFolder);
	mainFolder.Format("%s\\", tempFolder);

	buffer = NULL;
	compressed = false;
	startSpot = 0;
	endSpot = 0;
	lastRomName = "";
	romSize = 0;

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_game.ResetContent();

	InitializeSpecificGames();
	
	m_game.SetCurSel(0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CN64MidiToolDlg::InitializeSpecificGames()	
{
	char folder[8000];
	strcpy(folder, mainFolder);
	strcat(folder, "gameconfig.ini");

	FILE* inIni = fopen(folder, "r");
	if (inIni == NULL)
	{
		MessageBox("Error reading gameconfig.ini, file must exist", "Error");
		OnCancel();
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

	gameConfig = new GameConfig[countGames];
	countGames = 0;
	lastGame = "";
	
	int midiCount = 0;


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
		else
		{
			midiCount++;
		}

		if (lastGameNew != lastGame)
		{
			if (lastGame != "")
			{
				m_game.AddString(gameConfig[countGames].gameName);
				gameConfig[countGames].midiBanks = new MidiConfig[midiCount];
				countGames++;
			}
			else
			{
				gameConfig[countGames].gameName = lastGameNew;
				gameConfig[countGames].gameName.Replace("\r", "");
				gameConfig[countGames].gameName.Replace("\n", "");
				m_game.AddString(gameConfig[countGames].gameName);
			}


			midiCount = 0;
			lastGame = lastGameNew;
			gameConfig[countGames].gameName = lastGameNew;
			gameConfig[countGames].gameName.Replace("\r", "");
			gameConfig[countGames].gameName.Replace("\n", "");
			gameConfig[countGames].gameType = gameType;
		}
	}

	if (lastGame != "")
	{
		gameConfig[countGames].midiBanks = new MidiConfig[midiCount];
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

				gameConfig[countGames].midiBanks[gameConfig[countGames].numberMidis].start = CSharedFunctions::StringHexToLong(first);
				gameConfig[countGames].midiBanks[gameConfig[countGames].numberMidis].end = CSharedFunctions::StringHexToLong(second);
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
			gameConfig[countGames].numberMidis = 0;
		}
	}

	if (lastGame != "")
	{
		countGames++;
	}

	fclose(inIni);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CN64MidiToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CN64MidiToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


unsigned long CN64MidiToolDlg::CharArrayToLong(unsigned char* currentSpot)
{
	return Flip32Bit(*reinterpret_cast<unsigned long*> (currentSpot));
}

unsigned long CN64MidiToolDlg::Flip32Bit(unsigned long inLong)
{
	return (((inLong & 0xFF000000) >> 24) | ((inLong & 0x00FF0000) >> 8) | ((inLong & 0x0000FF00) << 8) | ((inLong & 0x000000FF) << 24));
}

unsigned short CN64MidiToolDlg::CharArrayToShort(unsigned char* currentSpot)
{
	return Flip16Bit(*reinterpret_cast<unsigned short*> (currentSpot));
}

unsigned short CN64MidiToolDlg::Flip16Bit(unsigned short ShortValue)
{
	return ((ShortValue >> 8) | ((ShortValue << 8)));
}

unsigned long CN64MidiToolDlg::ReadAddiuAddress(unsigned char* GEROM, unsigned long upperLocation, unsigned long lowerLocation)
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

int CN64MidiToolDlg::GetSizeFile(CString filename)
{
	FILE* inFile = fopen(filename, "rb");
	
	if (inFile == 0)
		return 0;

	fseek(inFile, 0, SEEK_END);
	int fileSize = ftell(inFile);
	fclose(inFile);
	return fileSize;
}

void CN64MidiToolDlg::OnBnClickedButtonloadrom()
{
	CString tempStr;
	m_game.GetWindowText(tempStr);
	CFileDialog m_ldFile(TRUE, NULL, tempStr + " [!].z64", OFN_HIDEREADONLY, "GE ROM(*.v64;*.z64;*.rom;*.n64)|*.v64;*.z64;*.rom;*.n64|", this);

	int statusFileOpen = (int) m_ldFile.DoModal();

	if ((statusFileOpen == IDCANCEL) || (m_ldFile.GetPathName() == ""))
		return;

	if (statusFileOpen == FALSE)
		return;	


	romSize = GetSizeFile(m_ldFile.GetPathName());

	if ((statusFileOpen == IDOK) && (romSize > 0))
	{
		

		m_game.GetWindowText(gameName);


		FILE* inROM = fopen(m_ldFile.GetPathName(), "rb");

		if (inROM == NULL)
		{
			MessageBox("Error opening ROM");
			return;
		}
		if (buffer != NULL)
		{
			delete [] buffer;
			buffer = NULL;
		}

		lastRomName = m_ldFile.GetPathName();

		buffer = new unsigned char[romSize];
		fread(buffer, 1, romSize, inROM);
		fclose(inROM);

		bool byteFlippedROM = false;
		if (buffer[0] == 0x37)
			byteFlippedROM = true;
		else if (buffer[0] == 0x80)
			byteFlippedROM = false;
		else
		{
			//MessageBox("Not a valid ROM", "Error");
			//return;
		}

		if (byteFlippedROM == true)
		{
			for (int x = 0; x < romSize; x=x+2)
			{
				unsigned char tempSpot = buffer[x];
				buffer[x] = buffer[x+1];
				buffer[x+1] = tempSpot;
			}
		}	
			

		m_spot.ResetContent();

		int gameNumber = 0;
		for (int x = 0; x < countGames; x++)
		{
			if (gameConfig[x].gameName.Find(gameName) == 0)
			{
				gameNumber = x;
				break;
			}
		}

		CString addMidiStrings[1000];
		int numberMidiStrings = 0;
		int numberInstruments = 0;
		ProcessMidis(gameConfig, gameNumber, buffer, addMidiStrings, numberMidiStrings, numberInstruments);

		for (int x = 0; x < numberMidiStrings; x++)
			m_spot.AddString(addMidiStrings[x]);

		CString tempInstrStr;
		tempInstrStr.Format("%02X", numberInstruments);
		m_numberInstruments.SetWindowText(tempInstrStr);

		
		

		if (m_spot.GetCount() > 0)
		{
			m_spot.SetCurSel(0);
			OnCbnSelchangeCombomidi();
		}
	}
}

void CN64MidiToolDlg::ProcessMidis(GameConfig* gameConfig, int gameNumber, byte* gameBuffer, CString addMidiStrings[1000], int& numberMidiStrings, int& numberInstruments)
{
	if (gameConfig[gameNumber].gameType.Find("GoldenEye") == 0)
	{
		// TODO
		//midiParse.ImportMidiConfig("gemidiconfig.txt");
		compress.SetGame(GOLDENEYE);
		compressed = true;
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			unsigned long start = ReadAddiuAddress(gameBuffer, gameConfig[gameNumber].midiBanks[x].start, gameConfig[gameNumber].midiBanks[x].end) + 4;

			int numberMusicData = CharArrayToShort(&gameBuffer[start-4]);
			
			unsigned long currentSpot = start;
			
			int currentBinaryNumber = 0;
			while (currentBinaryNumber < numberMusicData)
			{
				unsigned long musicOffset = (CharArrayToLong(&gameBuffer[currentSpot]) + (start - 4));
				unsigned long compressedSize = CharArrayToShort(&gameBuffer[currentSpot+6]);

				CString tempSpotStr;
				tempSpotStr.Format("%08X:%08X", musicOffset, compressedSize);
				addMidiStrings[numberMidiStrings++] = tempSpotStr;

				int numberInstTemp = 0;
				ExportToMidi(gameBuffer, musicOffset, compressedSize, "asdasdaw43.mid", gameConfig[gameNumber].gameType, numberInstTemp, 0);
				if (numberInstTemp > numberInstruments)
					numberInstruments = numberInstTemp;
				::DeleteFile("asdasdaw43.mid");

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
			
			unsigned long countMidi = CharArrayToLong(&gameBuffer[start]);
			unsigned long endMidi = CharArrayToLong(&gameBuffer[start + 0x14]);
			for (int y = 0; y < countMidi; y++)
			{
				unsigned long midiLocation = CharArrayToLong(&gameBuffer[start + 0x18 + (y * 8)]);
				unsigned long midiDivison = CharArrayToLong(&gameBuffer[start + 0x18 + (y * 8) + 4]);
				unsigned long midiEnd;
				if (y == (countMidi - 1))
					midiEnd = endMidi;
				else
					midiEnd = CharArrayToLong(&gameBuffer[start + 0x18 + (y * 8) + 8]);

				CString tempSpotStr;
				tempSpotStr.Format("%08X:%08X", midiLocation, (midiEnd - midiLocation));
				addMidiStrings[numberMidiStrings++] = tempSpotStr;

				int numberInstTemp = 0;
				ExportToMidi(gameBuffer, (start + midiLocation), (midiEnd - midiLocation), "asdasdaw43.mid", gameConfig[gameNumber].gameType, numberInstTemp, midiDivison);
				if (numberInstTemp > numberInstruments)
					numberInstruments = numberInstTemp;
				::DeleteFile("asdasdaw43.mid");
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
			
			startSpot = (CharArrayToLong(&gameBuffer[currentSpot]) + (start - 4));
			
			int currentBinaryNumber = 0;
			while (currentBinaryNumber <  0x3F)
			{
				unsigned long musicOffset = (CharArrayToLong(&gameBuffer[currentSpot]) + gameConfig[gameNumber].midiBanks[0].end);
				unsigned long compressedSize = CharArrayToShort(&gameBuffer[currentSpot+6]);

				CString tempSpotStr;
				tempSpotStr.Format("%08X:%08X", musicOffset, compressedSize);
				addMidiStrings[numberMidiStrings++] = tempSpotStr;

				int numberInstTemp = 0;
				ExportToMidi(gameBuffer, musicOffset, compressedSize, "asdasdaw43.mid", gameConfig[gameNumber].gameType, numberInstTemp, 0);
				if (numberInstTemp > numberInstruments)
					numberInstruments = numberInstTemp;
				::DeleteFile("asdasdaw43.mid");

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
			addMidiStrings[numberMidiStrings++] = tempSpotStr;

			int numberInstTemp = 0;
			ExportToMidi(gameBuffer, gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start), "asdasdaw43.mid", gameConfig[gameNumber].gameType, numberInstTemp, 0);
			if (numberInstTemp > numberInstruments)
				numberInstruments = numberInstTemp;
			::DeleteFile("asdasdaw43.mid");
		}
	}
	else if (gameConfig[gameNumber].gameType.Find("MIDx") == 0)
	{
		compressed = false;
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings[numberMidiStrings++] = tempSpotStr;

			int numberInstTemp = 0;
			ExportToMidi(gameBuffer, gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start), "asdasdaw43.mid", gameConfig[gameNumber].gameType, numberInstTemp, 0);
			if (numberInstTemp > numberInstruments)
				numberInstruments = numberInstTemp;
			::DeleteFile("asdasdaw43.mid");
		}
	}
	else if (gameConfig[gameNumber].gameType.Find("MML") == 0)
	{
		compressed = false;
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings[numberMidiStrings++] = tempSpotStr;

			int numberInstTemp = 0;
			ExportToMidi(gameBuffer, gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start), "asdasdaw43.mid", gameConfig[gameNumber].gameType, numberInstTemp, 0);
			if (numberInstTemp > numberInstruments)
				numberInstruments = numberInstTemp;
			::DeleteFile("asdasdaw43.mid");
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
			addMidiStrings[numberMidiStrings++] = tempSpotStr;

			int numberInstTemp = 0;
			ExportToMidi(gameBuffer, gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start), "asdasdaw43.mid", gameConfig[gameNumber].gameType, numberInstTemp, 0);
			if (numberInstTemp > numberInstruments)
				numberInstruments = numberInstTemp;
			::DeleteFile("asdasdaw43.mid");
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
				while ((gameBuffer[currentSpot + byteCheck] != 0x1F) || (gameBuffer[currentSpot + byteCheck + 1] != 0x8B))
				{
					byteCheck++;
				}

				// move ahead 1
				currentSpot++;

				while ((currentSpot <  gameConfig[gameNumber].midiBanks[x].end))
				{
					if ((gameBuffer[currentSpot] != 0x1F) || (gameBuffer[currentSpot + 1] != 0x8B) || (gameBuffer[currentSpot + 2] != 0x08))
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
				addMidiStrings[numberMidiStrings++] = tempSpotStr;

				int numberInstTemp = 0;
				ExportToMidi(gameBuffer, startCurrentSpot, (currentSpot - startCurrentSpot), "asdasdaw43.mid", gameConfig[gameNumber].gameType, numberInstTemp, 0);
				if (numberInstTemp > numberInstruments)
					numberInstruments = numberInstTemp;
				::DeleteFile("asdasdaw43.mid");
				
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
			addMidiStrings[numberMidiStrings++] = tempSpotStr;
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
			addMidiStrings[numberMidiStrings++] = tempSpotStr;
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
			addMidiStrings[numberMidiStrings++] = tempSpotStr;
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
			addMidiStrings[numberMidiStrings++] = tempSpotStr;
		}
	}
	else if (gameConfig[gameNumber].gameType.Find("TitusMidi") == 0)
	{
		compressed = true;
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings[numberMidiStrings++] = tempSpotStr;
		}
	}
	else if (gameConfig[gameNumber].gameType.Find("RNCMidi") == 0)
	{
		compressed = true;
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings[numberMidiStrings++] = tempSpotStr;
		}
	}
	else
	{
		compressed = false;
		//midiParse.ImportMidiConfig("aerofightersassault.txt");
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			ParseUncompressedType(gameBuffer, gameConfig[gameNumber].midiBanks[x].start, gameConfig[gameNumber].midiBanks[x].end, addMidiStrings, numberMidiStrings, numberInstruments);
		}
	}
}

void CN64MidiToolDlg::ParseUncompressedType(unsigned char* gamebuffer, unsigned long start, unsigned long endSpot, CString addMidiStrings[1000], int& numberMidiStrings, int& numberInstruments)
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
			addMidiStrings[numberMidiStrings++] = tempSpotStr;

			int numberInstTemp = 0;
			ExportToMidi(gamebuffer, startCurrentSpot, (currentSpot - startCurrentSpot), "asdasdaw43.mid", "Uncompressed", numberInstTemp, 0);
			if (numberInstTemp > numberInstruments)
				numberInstruments = numberInstTemp;
			::DeleteFile("asdasdaw43.mid");
		}
		
		if (currentSpot >= endSpot)
			endFlag = true;
	}
	
	compressed  = false;
}

CN64MidiToolDlg::~CN64MidiToolDlg()
{
	if (gameConfig != NULL)
	{
		for (int x = 0; x < countGames; x++)
		{
			delete [] gameConfig[x].midiBanks;
		}

		delete [] gameConfig;
	}

	if (buffer != NULL)
	{
		delete [] buffer;
		buffer = NULL;
	}
}
void CN64MidiToolDlg::OnCbnSelchangeCombomidi()
{

	
}

byte* CN64MidiToolDlg::Decompress(unsigned char* Buffer, unsigned long size, int& fileSize, int& compressedSize)
{
	compress.SetCompressedBuffer(Buffer, size);
	
	fileSize = 0; // is by reference, overwritten
	byte* outputDecompressed = compress.OutputDecompressedBuffer(fileSize, compressedSize);

	if (outputDecompressed == NULL)
	{
		MessageBox("Error Decompressing", "Error");
		return NULL;
	}

	return outputDecompressed;
}

bool CN64MidiToolDlg::DecompressToFile(unsigned char* Buffer, unsigned long size, CString outputFile)
{
	int fileSize = 0; // is by reference, overwritten
	int compressedSize = -1;
	byte* outputDecompressed = Decompress(Buffer, size, fileSize, compressedSize);

	if (outputDecompressed == NULL)
	{
		MessageBox("Error Decompressing", "Error");
		return false;
	}

	FILE* outFile = fopen(outputFile, "wb");
	if (outFile == NULL)
	{
		MessageBox("Cannot Write File", "Error");
		return false;
	}
	for (int x = 0; x < fileSize; x++)
	{
		fwrite(&outputDecompressed[x], 1, 1, outFile);
	}
	fclose(outFile);
	
	delete [] outputDecompressed;
	return false;
}

void CN64MidiToolDlg::OnBnClickedButtonexportbin()
{
	if (buffer == NULL)
		return;

	if (m_spot.GetCount() == 0)
		return;

	CString spotStr;
	m_spot.GetWindowText(spotStr);

	unsigned long address = 0;
	unsigned long size = 0;
	sscanf(spotStr, "%08X:%08X", &address, &size);

	CString gameName;
	m_game.GetWindowText(gameName);

	int gameNumber = 0;
	for (int x = 0; x < countGames; x++)
	{
		if (gameConfig[x].gameName.Find(gameName) == 0)
		{
			gameNumber = x;
			break;
		}
	}

	if (gameConfig[gameNumber].gameType.Find("MultipartZLibXMFastTracker2") == 0)
	{
		CFileDialog m_svFile(FALSE, "xm", "FastTracker2.xm", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Xm File (*.xm)|*.xm|", this);

		int statusFileOpen = (int) m_svFile.DoModal();

		if ((statusFileOpen == IDCANCEL) || (m_svFile.GetFileName() == ""))
			return;

		if (statusFileOpen == FALSE)
			return;

		CString fileName;
		if (statusFileOpen == IDOK)
		{
			fileName = m_svFile.GetPathName();
		}
		else
		{
			return;
		}

		int numberInstruments;
		ExportToMidi(buffer, address, size, fileName, gameConfig[gameNumber].gameType, numberInstruments, 0);
	}
	else if (gameConfig[gameNumber].gameType.Find("DCM") == 0)
	{
		CFileDialog m_svFile(FALSE, "dcm", "song.dcm", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Dcm File (*.dcm)|*.dcm|", this);

		int statusFileOpen = (int) m_svFile.DoModal();

		if ((statusFileOpen == IDCANCEL) || (m_svFile.GetFileName() == ""))
			return;

		if (statusFileOpen == FALSE)
			return;

		CString fileName;
		if (statusFileOpen == IDOK)
		{
			fileName = m_svFile.GetPathName();
		}
		else
		{
			return;
		}

		int numberInstruments;
		ExportToMidi(buffer, address, size, fileName, gameConfig[gameNumber].gameType, numberInstruments, 0);
	}
	else if (gameConfig[gameNumber].gameType.Find("LZSamplesDCM") == 0)
	{
		CFileDialog m_svFile(FALSE, "dcm", "song.dcm", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Dcm File (*.dcm)|*.dcm|", this);

		int statusFileOpen = (int) m_svFile.DoModal();

		if ((statusFileOpen == IDCANCEL) || (m_svFile.GetFileName() == ""))
			return;

		if (statusFileOpen == FALSE)
			return;

		CString fileName;
		if (statusFileOpen == IDOK)
		{
			fileName = m_svFile.GetPathName();
		}
		else
		{
			return;
		}

		int numberInstruments;
		ExportToMidi(buffer, address, size, fileName, gameConfig[gameNumber].gameType, numberInstruments, 0);
	}
	else
	{
		CFileDialog m_svFile(FALSE, "bin", "Midi.bin", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Bin File (*.bin)|*.bin|", this);

		int statusFileOpen = (int) m_svFile.DoModal();

		if ((statusFileOpen == IDCANCEL) || (m_svFile.GetFileName() == ""))
			return;

		if (statusFileOpen == FALSE)
			return;

		CString fileName;
		if (statusFileOpen == IDOK)
		{
			fileName = m_svFile.GetPathName();
		}
		else
		{
			return;
		}

		ExportToBin(address, size, fileName);
	}
}

void CN64MidiToolDlg::ExportToBin(unsigned long address, unsigned long size, CString fileName)
{
	if (compressed)
	{
		DecompressToFile(&buffer[address], size, fileName);
	}
	else
	{
		FILE* outFile = fopen(fileName, "wb");
		if (outFile == NULL)
		{
			MessageBox("Cannot Write File", "Error");
			return;
		}
		for (int x = 0; x < size; x++)
		{
			fwrite(&buffer[address+x], 1, 1, outFile);
		}
		fclose(outFile);
	}
}

void CN64MidiToolDlg::OnBnClickedButtonexportmidi()
{
	if (buffer == NULL)
		return;

	if (m_spot.GetCount() == 0)
		return;

	CString spotStr;
	m_spot.GetWindowText(spotStr);

	unsigned long address = 0;
	unsigned long size = 0;
	sscanf(spotStr, "%08X:%08X", &address, &size);

	CString gameName;
	m_game.GetWindowText(gameName);

	int gameNumber = 0;
	for (int x = 0; x < countGames; x++)
	{
		if (gameConfig[x].gameName.Find(gameName) == 0)
		{
			gameNumber = x;
			break;
		}
	}

	if (gameConfig[gameNumber].gameType.Find("MultipartZLibXMFastTracker2") == 0)
	{
		CFileDialog m_svFile(FALSE, "xm", "FastTracker2.xm", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Xm File (*.xm)|*.xm|", this);

		int statusFileOpen = (int) m_svFile.DoModal();

		if ((statusFileOpen == IDCANCEL) || (m_svFile.GetFileName() == ""))
			return;

		if (statusFileOpen == FALSE)
			return;

		CString fileName;
		if (statusFileOpen == IDOK)
		{
			fileName = m_svFile.GetPathName();
		}
		else
		{
			return;
		}

		int numberInstruments;
		ExportToMidi(buffer, address, size, fileName, gameConfig[gameNumber].gameType, numberInstruments, 0);
	}
	else if (gameConfig[gameNumber].gameType.Find("DCM") == 0)
	{
		CFileDialog m_svFile(FALSE, "dcm", "song.dcm", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Dcm File (*.dcm)|*.dcm|", this);

		int statusFileOpen = (int) m_svFile.DoModal();

		if ((statusFileOpen == IDCANCEL) || (m_svFile.GetFileName() == ""))
			return;

		if (statusFileOpen == FALSE)
			return;

		CString fileName;
		if (statusFileOpen == IDOK)
		{
			fileName = m_svFile.GetPathName();
		}
		else
		{
			return;
		}

		int numberInstruments;
		ExportToMidi(buffer, address, size, fileName, gameConfig[gameNumber].gameType, numberInstruments, 0);
	}
	else
	{
		if (buffer == NULL)
			return;

		if (m_spot.GetCount() == 0)
			return;

		CFileDialog m_svFile(FALSE, "mid", "Midi.mid", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Midi File (*.mid)|*.mid|", this);

		int statusFileOpen = (int) m_svFile.DoModal();

		if ((statusFileOpen == IDCANCEL) || (m_svFile.GetFileName() == ""))
			return;

		if (statusFileOpen == FALSE)
			return;

		CString fileName;
		if (statusFileOpen == IDOK)
		{
			fileName = m_svFile.GetPathName();
		}
		else
		{
			return;
		}

		CString spotStr;
		m_spot.GetWindowText(spotStr);

		unsigned long address = 0;
		unsigned long size = 0;
		sscanf(spotStr, "%08X:%08X", &address, &size);

		CString gameName;
		m_game.GetWindowText(gameName);

		int gameNumber = 0;
		for (int x = 0; x < countGames; x++)
		{
			if (gameConfig[x].gameName.Find(gameName) == 0)
			{
				gameNumber = x;
				break;
			}
		}

		int numberInstruments;
		ExportToMidi(buffer, address, size, fileName, gameConfig[gameNumber].gameType, numberInstruments, 0);
	}


}

void CN64MidiToolDlg::KillWMPlayer()
{
	STARTUPINFO si;
	::ZeroMemory(&si,sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	PROCESS_INFORMATION pi;    
   
	::ZeroMemory(&si,sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	if(CreateProcess(NULL, "taskkill /im wmplayer.exe", NULL,
	NULL,FALSE,NORMAL_PRIORITY_CLASS,NULL,NULL,&si,&pi))
	{
		WaitForSingleObject(pi.hProcess, 1000);
		::CloseHandle(pi.hThread);
		::CloseHandle(pi.hProcess);
	}

	Sleep(1000);
}

void CN64MidiToolDlg::ExportToMidi(unsigned char* gamebuffer, unsigned long address, unsigned long size, CString fileName, CString gameType, int& numberInstruments, unsigned long division)
{
	if (gameType.Find("BanjoTooie") == 0)
	{
		if (compressed)
		{
			int decompressedSize = 0;
			int compressedSize = -1;
			byte* outputBuffer = Decompress(&gamebuffer[address], size, decompressedSize, compressedSize);
			midiParse.BTMidiToMidi(outputBuffer, decompressedSize, fileName, numberInstruments);
			delete [] outputBuffer;
		}
		else
		{
			midiParse.BTMidiToMidi(&gamebuffer[address], size, fileName, numberInstruments);
		}
	}
	else if (gameType.Find("Glover") == 0)
	{
		midiParse.GloverMidiToMidi(&gamebuffer[address], size, fileName, numberInstruments, division);
	}
	else if (gameType.Find("MIDx") == 0)
	{
		if (compressed)
		{
			
		}
		else
		{
			midiParse.MIDxMidiToMidi(&gamebuffer[address], size, fileName, numberInstruments);
		}
	}
	else if (gameType.Find("MML") == 0)
	{
		if (compressed)
		{
			
		}
		else
		{
			midiParse.MMLToMidi(&gamebuffer[address], size, fileName, numberInstruments);
		}
	}
	else if (gameType.Find("Midi") == 0)
	{
		if (compressed)
		{
			
		}
		else
		{
			FILE* outFile = fopen(fileName, "wb");
			if (outFile == NULL)
			{
				MessageBox("Cannot Write File", "Error");
				return;
			}
			for (int x = 0; x < size; x++)
			{
				fwrite(&buffer[address+x], 1, 1, outFile);
			}
			fclose(outFile);
		}
	}
	else if (gameType.Find("MultipartZLibXMFastTracker2") == 0)
	{
		if (compressed)
		{
			compress.SetGame(STUNTRACER64);

			FILE* outFile = fopen(fileName, "wb");
			if (outFile == NULL)
			{
				MessageBox("Cannot Write File", "Error");
				return;
			}

			unsigned long compressedMainSize = CharArrayToLong(&gamebuffer[address]);
			unsigned long uncompressedMainSize = CharArrayToLong(&gamebuffer[address+4]);
			unsigned long step = CharArrayToLong(&gamebuffer[address+0x8]);

			int readSize = 0;
			while ((readSize < compressedMainSize) && (CharArrayToLong(&gamebuffer[address+0xC+readSize]) != 0))
			{
				int decompressedSize = 0;
				int compressedSize = -1;
				byte* outputBuffer = Decompress(&gamebuffer[address+0xC+readSize], size, decompressedSize, compressedSize);

				for (int x = 0; x < decompressedSize; x++)
				{
					fwrite(&outputBuffer[x], 1, 1, outFile);
				}

				readSize += step;

				if (((address+0xC+readSize) % 2) != 0)
				{
					readSize += (2-((address+0xC+readSize) % 2));
				}

				step = CharArrayToLong(&gamebuffer[address+0xC+readSize]);

				readSize += 4;

				delete [] outputBuffer;
			}
			fclose(outFile);

		}
		else
		{
			
		}
	}
	else if (gameType.Find("DCM") == 0)
	{
		if (compressed)
		{
			FILE* outFile = fopen(fileName, "wb");
			if (outFile == NULL)
			{
				MessageBox("Cannot Write File", "Error");
				return;
			}

			CH20Decoder h20dec;
			int compressedSize = -1;
			unsigned char* outputDecompressed = new unsigned char[0x100000];
			unsigned char* cleanDecompressed = new unsigned char[0x100000];

			int decSize = h20dec.decPolaris(&gamebuffer[address], compressedSize, outputDecompressed);
			decSize = h20dec.decPolaris(&gamebuffer[address], compressedSize, cleanDecompressed);

			for (int x = 14; x < (14 + (outputDecompressed[5] * 0x10)); x+=0x10)
			{
				unsigned char tempValue1 = outputDecompressed[x];
				unsigned char tempValue2 = outputDecompressed[x+1];

				outputDecompressed[x] = outputDecompressed[x+3];
				outputDecompressed[x+1] = outputDecompressed[x+2];
				outputDecompressed[x+2] = tempValue2;
				outputDecompressed[x+3] = tempValue1;

				tempValue1 = outputDecompressed[x+4];
				tempValue2 = outputDecompressed[x+5];

				outputDecompressed[x+4] = outputDecompressed[x+7];
				outputDecompressed[x+5] = outputDecompressed[x+6];
				outputDecompressed[x+6] = tempValue2;
				outputDecompressed[x+7] = tempValue1;

				tempValue1 = outputDecompressed[x+8];
				tempValue2 = outputDecompressed[x+9];

				outputDecompressed[x+8] = outputDecompressed[x+11];
				outputDecompressed[x+9] = outputDecompressed[x+10];
				outputDecompressed[x+10] = tempValue2;
				outputDecompressed[x+11] = tempValue1;

				unsigned char tempValue = outputDecompressed[x+12];
				outputDecompressed[x+12] = outputDecompressed[x+13];
				outputDecompressed[x+13] = tempValue;

				tempValue = outputDecompressed[x+14];
				outputDecompressed[x+14] = outputDecompressed[x+15];
				outputDecompressed[x+15] = tempValue;
			}

			int position = (14 + (outputDecompressed[5] * 0x10));

			if (cleanDecompressed[position] == 0x00)
			{
				unsigned long length = CharArrayToLong(&cleanDecompressed[position]);
				if (length > decSize)
				{
					cleanDecompressed[6] = (length & 0xFF);
					cleanDecompressed[7] = ((length >> 8) & 0xFF);
					cleanDecompressed[8] = ((length >> 16) & 0xFF);
					cleanDecompressed[9] = ((length >> 24) & 0xFF);

					fwrite(cleanDecompressed, 1, position, outFile);

					CTetrisphereDecoder tetDec;
					unsigned char* outputLz = new unsigned char[0x1000000];
					int returnSize = tetDec.decompressLZ(&cleanDecompressed[position+4], length, outputLz, true);
					returnSize = returnSize;

					fwrite(outputLz, 1, length, outFile);

					delete [] outputLz;
				}
				else
				{
					cleanDecompressed[6] = (length & 0xFF);
					cleanDecompressed[7] = ((length >> 8) & 0xFF);
					cleanDecompressed[8] = ((length >> 16) & 0xFF);
					cleanDecompressed[9] = ((length >> 24) & 0xFF);

					fwrite(cleanDecompressed, 1, position, outFile);

					length = (decSize - (position + 4));
					fwrite(&cleanDecompressed[position+4], 1, length, outFile);
				}
			}
			else
			{
				fwrite(cleanDecompressed, 1, position, outFile);

				int length = (decSize - (position));
				fwrite(&cleanDecompressed[position], 1, length, outFile);
			}

			for (int x = 0; x < outputDecompressed[5]; x++)
			{
				unsigned short instrumentNumber = CharArrayToShort(&outputDecompressed[14+(16*(x))+14]);

				unsigned long sampleSize = CharArrayToLong(&outputDecompressed[14+(16*(x))]);
				unsigned short flags = CharArrayToShort(&outputDecompressed[14+(16*(x))+12]);

				if (flags & 1)
				{
					sampleSize = sampleSize * 2;
				}
				unsigned long location = address + size + CharArrayToLong(&gamebuffer[address + size + (instrumentNumber * 4) + 2]);

				unsigned char* outputDecompressedInstrument = new unsigned char[0x100000];

				int decSizeInstrument = h20dec.decPolaris(&gamebuffer[location], compressedSize, outputDecompressedInstrument);
				fwrite(outputDecompressedInstrument, 1, sampleSize, outFile);

				delete [] outputDecompressedInstrument;
			}

			delete [] outputDecompressed;
			delete [] cleanDecompressed;

			fclose(outFile);
		}
		else
		{

		}
	}
	else if (gameType.Find("LZSamplesDCM") == 0)
	{
		if (compressed)
		{
			FILE* outFile = fopen(fileName, "wb");
			if (outFile == NULL)
			{
				MessageBox("Cannot Write File", "Error");
				return;
			}

			unsigned long decSize = ((((((gamebuffer[address+0x9] << 8) | gamebuffer[address+0x8]) << 8) | gamebuffer[address+0x7]) << 8) | gamebuffer[address+0x6]);
			
			unsigned char* outputDecompressed = new unsigned char[0x100000];
			unsigned char* cleanDecompressed = new unsigned char[0x100000];
			memcpy(outputDecompressed, &gamebuffer[address], decSize + 4 + (14 + (outputDecompressed[5] * 0x10)));
			memcpy(cleanDecompressed, &gamebuffer[address], decSize + 4 + (14 + (outputDecompressed[5] * 0x10)));

			for (int x = 14; x < (14 + (outputDecompressed[5] * 0x10)); x+=0x10)
			{
				unsigned char tempValue1 = outputDecompressed[x];
				unsigned char tempValue2 = outputDecompressed[x+1];

				outputDecompressed[x] = outputDecompressed[x+3];
				outputDecompressed[x+1] = outputDecompressed[x+2];
				outputDecompressed[x+2] = tempValue2;
				outputDecompressed[x+3] = tempValue1;

				tempValue1 = outputDecompressed[x+4];
				tempValue2 = outputDecompressed[x+5];

				outputDecompressed[x+4] = outputDecompressed[x+7];
				outputDecompressed[x+5] = outputDecompressed[x+6];
				outputDecompressed[x+6] = tempValue2;
				outputDecompressed[x+7] = tempValue1;

				tempValue1 = outputDecompressed[x+8];
				tempValue2 = outputDecompressed[x+9];

				outputDecompressed[x+8] = outputDecompressed[x+11];
				outputDecompressed[x+9] = outputDecompressed[x+10];
				outputDecompressed[x+10] = tempValue2;
				outputDecompressed[x+11] = tempValue1;

				unsigned char tempValue = outputDecompressed[x+12];
				outputDecompressed[x+12] = outputDecompressed[x+13];
				outputDecompressed[x+13] = tempValue;

				tempValue = outputDecompressed[x+14];
				outputDecompressed[x+14] = outputDecompressed[x+15];
				outputDecompressed[x+15] = tempValue;
			}

			int position = (14 + (outputDecompressed[5] * 0x10));

			if (cleanDecompressed[position] == 0x00)
			{
				unsigned long length = CharArrayToLong(&cleanDecompressed[position]);
				if (length > decSize)
				{
					cleanDecompressed[6] = (length & 0xFF);
					cleanDecompressed[7] = ((length >> 8) & 0xFF);
					cleanDecompressed[8] = ((length >> 16) & 0xFF);
					cleanDecompressed[9] = ((length >> 24) & 0xFF);

					fwrite(cleanDecompressed, 1, position, outFile);

					CTetrisphereDecoder tetDec;
					unsigned char* outputLz = new unsigned char[0x1000000];
					int returnSize = tetDec.decompressLZ(&cleanDecompressed[position+4], (decSize - 4), outputLz, true);
					returnSize = returnSize;

					fwrite(outputLz, 1, length, outFile);

					delete [] outputLz;

					position += returnSize;
				}
				else
				{
					cleanDecompressed[6] = (length & 0xFF);
					cleanDecompressed[7] = ((length >> 8) & 0xFF);
					cleanDecompressed[8] = ((length >> 16) & 0xFF);
					cleanDecompressed[9] = ((length >> 24) & 0xFF);

					fwrite(cleanDecompressed, 1, position, outFile);

					length = (decSize - (position + 4));
					fwrite(&cleanDecompressed[position+4], 1, length, outFile);

					position += length;
				}
			}
			else
			{
				fwrite(cleanDecompressed, 1, position, outFile);

				int length = decSize;
				fwrite(&cleanDecompressed[position], 1, length, outFile);

				position += length;
			}

			for (int x = 0; x < outputDecompressed[5]; x++)
			{
				unsigned short instrumentNumber = CharArrayToShort(&outputDecompressed[14+(16*(x))+14]);

				unsigned long sampleSize = CharArrayToLong(&outputDecompressed[14+(16*(x))]);
				unsigned short flags = CharArrayToShort(&outputDecompressed[14+(16*(x))+12]);

				if (flags & 1)
				{
					sampleSize = sampleSize * 2;
				}
				unsigned long location = address + size + CharArrayToLong(&gamebuffer[address + size + (instrumentNumber * 4) + 2]);

				unsigned char* outputDecompressedInstrument = new unsigned char[0x100000];

				CTetrisphereDecoder tetDec;
				tetDec.sphereDecompress(&gamebuffer[location], outputDecompressedInstrument);
				fwrite(outputDecompressedInstrument, 1, sampleSize, outFile);

				position += sampleSize;

				delete [] outputDecompressedInstrument;
			}

			delete [] outputDecompressed;
			delete [] cleanDecompressed;

			fclose(outFile);
		}
		else
		{

		}
	}
	else if (gameType.Find("TitusMidi") == 0)
	{
		if (compressed)
		{
			int fileSizeCompressed = -1;
			SupermanDecoder decode;
			int expectedSize = decode.header(&gamebuffer[address], fileSizeCompressed);
			unsigned char* outputDecompressed = new unsigned char[expectedSize];
			
			decode.dec(&gamebuffer[address+0x11], fileSizeCompressed, expectedSize, outputDecompressed);
			
			FILE* outFile = fopen(fileName, "wb");
			if (outFile == NULL)
			{
				MessageBox("Cannot Write File", "Error");
				return;
			}
			for (int x = 0; x < expectedSize; x++)
			{
				fwrite(&outputDecompressed[x], 1, 1, outFile);
			}
			fclose(outFile);

			delete [] outputDecompressed;
		}
		else
		{
			
		}
	}
	else if (gameType.Find("RNCMidi") == 0)
	{
		if (compressed)
		{
			int fileSizeCompressed = -1;
			RncDecoder decode;
			unsigned char* outputDecompressed = new unsigned char[0x100000];
			int expectedSize = decode.unpackM1(&gamebuffer[address], outputDecompressed, 0x0000, fileSizeCompressed);
			
			FILE* outFile = fopen(fileName, "wb");
			if (outFile == NULL)
			{
				MessageBox("Cannot Write File", "Error");
				return;
			}
			for (int x = 0; x < expectedSize; x++)
			{
				fwrite(&outputDecompressed[x], 1, 1, outFile);
			}
			fclose(outFile);

			midiParse.GEMidiToMidi(outputDecompressed, expectedSize, fileName, numberInstruments);

			delete [] outputDecompressed;
		}
		else
		{
			
		}
	}
	else
	{
		if (compressed)
		{
			int decompressedSize = 0;
			int compressedSize = -1;
			byte* outputBuffer = Decompress(&gamebuffer[address], (size + 20000), decompressedSize, compressedSize);
			if (outputBuffer != NULL)
			{
				if ((outputBuffer[0] == 0x0) && (outputBuffer[1] == 0x0)
					&& (outputBuffer[2] == 0x0) && (outputBuffer[3] == 0x44))
				{
					midiParse.GEMidiToMidi(outputBuffer, decompressedSize, fileName, numberInstruments);
				}
				delete [] outputBuffer;
			}
		}
		else
		{
			midiParse.GEMidiToMidi(&gamebuffer[address], size, fileName, numberInstruments);
		}
	}
}

void CN64MidiToolDlg::ConvertIntoSpot(CString inputFile)
{
	if (buffer == NULL)
		return;

	if (m_spot.GetCount() == 0)
		return;

	CString spotStr;
	m_spot.GetWindowText(spotStr);

	unsigned long address = 0;
	unsigned long size = 0;
	sscanf(spotStr, "%08X:%08X", &address, &size);

	int fileSize = GetSizeFile(inputFile);

	CString gameName;
	m_game.GetWindowText(gameName);

	int gameNumber = 0;
	for (int x = 0; x < countGames; x++)
	{
		if (gameConfig[x].gameName.Find(gameName) == 0)
		{
			gameNumber = x;
			break;
		}
	}

	if (compressed)
	{
		CompressGZipFile(inputFile, mainFolder + "tempAS734esa.gz", false, (gameConfig[gameNumber].gameType));

		int sizeCompressed = GetSizeFile(mainFolder + "tempAS734esa.gz");

		if (sizeCompressed > size)
		{
			CString errorStr;
			errorStr.Format("Cannot import, %08X is larger than %08X", sizeCompressed, size);
			MessageBox(errorStr);
			return;
		}

		FILE* inFile = fopen((mainFolder + "tempAS734esa.gz"), "rb");
		if (inFile == NULL)
		{
			MessageBox("Error reading file");
			return;
		}
		fread(&buffer[address], 1, sizeCompressed, inFile);
		fclose(inFile);
		::DeleteFile(mainFolder + "tempAS734esa.gz");
	}
	else
	{
		int sizeOnDisk = GetSizeFile(inputFile);

		if (sizeOnDisk > size)
		{
			CString errorStr;
			errorStr.Format("Cannot import, %08X is larger than %08X", sizeOnDisk, size);
			MessageBox(errorStr);
			return;
		}

		FILE* inFile = fopen(inputFile, "rb");
		if (inFile == NULL)
		{
			MessageBox("Error reading file");
			return;
		}
		fread(&buffer[address], 1, sizeOnDisk, inFile);
		fclose(inFile);

	}
}

void CN64MidiToolDlg::OnBnClickedButtonimportbin()
{
	if (buffer == NULL)
		return;

	if (m_spot.GetCount() == 0)
		return;

	CFileDialog m_ldFile(TRUE, NULL, "Midi.bin", OFN_HIDEREADONLY, "Bin (*.bin)|*.bin|", this);

	int isFileOpened = m_ldFile.DoModal();

	if (isFileOpened == IDCANCEL)
		return;
	
	
	CString spotStr;
	m_spot.GetWindowText(spotStr);

	unsigned long address = 0;
	unsigned long size = 0;
	sscanf(spotStr, "%08X:%08X", &address, &size);


	ConvertIntoSpot(m_ldFile.GetPathName());
}

BOOL CN64MidiToolDlg::IsFileExist(LPSTR lpszFilename)
{
	DWORD dwAttr = GetFileAttributes(lpszFilename);
	if (dwAttr == 0xffffffff)
		return FALSE;
	else 
		return TRUE;
}


BOOL CN64MidiToolDlg::normalExec (PTSTR pCmdLine, CString currentDirectory)
{
	STARTUPINFO si;
   PROCESS_INFORMATION processInfo;
   ZeroMemory(&si, sizeof(si));
   si.cb           = sizeof(si);
   si.wShowWindow  = SW_SHOW;
   ZeroMemory(&processInfo, sizeof(processInfo));
   /*return */CreateProcess(0, pCmdLine, 0, 0, FALSE, NORMAL_PRIORITY_CLASS, 0, currentDirectory, &si, &processInfo);
   WaitForSingleObject(processInfo.hProcess, 20000);

   DWORD exitCode;
   if (GetExitCodeProcess(processInfo.hProcess, &exitCode))
   {
        if (exitCode == STILL_ACTIVE)
		{
			MessageBox("For some reason GZip Failed", "Error");
			TerminateProcess(processInfo.hProcess, exitCode);
			return false;
		}
   }   
   return true;
}

BOOL CN64MidiToolDlg::hiddenExec (PTSTR pCmdLine, CString currentDirectory)
{
   STARTUPINFO si;
   PROCESS_INFORMATION processInfo;
   ZeroMemory(&si, sizeof(si));
   si.cb           = sizeof(si);
   si.dwFlags      = STARTF_USESHOWWINDOW;
   si.wShowWindow  = SW_HIDE;
   ZeroMemory(&processInfo, sizeof(processInfo));
   /*return */CreateProcess(0, pCmdLine, 0, 0, FALSE, 0, 0, currentDirectory, &si, &processInfo);
   WaitForSingleObject(processInfo.hProcess, 20000);
   DWORD exitCode;
   if (GetExitCodeProcess(processInfo.hProcess, &exitCode))
   {
        if (exitCode == STILL_ACTIVE)
		{
			MessageBox("For some reason GZip Failed", "Error");
			TerminateProcess(processInfo.hProcess, exitCode);
			return false;
		}
   }   
   return true;
};

bool CN64MidiToolDlg::CompressGZipFile(CString inputFile, CString outputFile, bool byteFlipCompressed, CString gameType)
{
	CString gzipFileName = (mainFolder + "gzip.exe");
	char tempFileExistName[1000];
	strcpy(tempFileExistName, (mainFolder + "gzip.exe"));
	if (IsFileExist(tempFileExistName) == false)
	{
		MessageBox("gzip.exe not found!", "Error");
		return false;
	}

	strcpy(tempFileExistName, inputFile);
	if (IsFileExist(tempFileExistName))
	{
		FILE* tempInputFile = fopen(inputFile, "rb");
		fseek(tempInputFile, 0, SEEK_END);
		unsigned long size = ftell(tempInputFile);

		unsigned char* tempBuffer;
		tempBuffer = new unsigned char[size + 0x10];
		for (int x = 0; x < (size + 0x10); x++)
			tempBuffer[x] = 0;

		fseek(tempInputFile, 0, SEEK_SET);


		fread(tempBuffer, 1, size, tempInputFile);


		FILE* tempOutputFile = fopen((mainFolder+"tempgh9.bin"), "wb");
		if (tempOutputFile == 0)
		{
			MessageBox("Cannot Write Temporary File", "Error");
			return false;
		}

		unsigned long compressedSize = size;

		if (((compressedSize % 0x10) != 0))
			compressedSize = ((compressedSize - (compressedSize % 0x10)) + 0x10);

		fwrite(tempBuffer, 1, compressedSize, tempOutputFile);	

		fclose(tempInputFile);
		fclose(tempOutputFile);

		delete [] tempBuffer;

		::SetCurrentDirectory(mainFolder);
		CString tempStr = ("gzip.exe -f -q -9 tempgh9.bin");
		hiddenExec(_T(tempStr.GetBuffer()), (mainFolder));
		CString outputGZippedName = (mainFolder+"TEMPGH9.BIZ");

		strcpy(tempFileExistName, outputGZippedName);
		if (!IsFileExist(tempFileExistName))
		{
			outputGZippedName = (mainFolder+"tempgh9.bin.gz");;
		}

		strcpy(tempFileExistName, outputGZippedName);
		if (IsFileExist(tempFileExistName))
		{
			FILE* inputFileName = fopen(outputGZippedName, "rb");
			int sizeNew = 0;
			fseek(inputFileName, 0, SEEK_END);
			sizeNew = ftell(inputFileName);
			fseek(inputFileName, 0, SEEK_SET);

			unsigned char* tempBufferNew;
			tempBufferNew = new unsigned char[sizeNew];
			fread(tempBufferNew, 1, sizeNew, inputFileName);

			fclose(inputFileName);
			DeleteFile((mainFolder+"TEMPGH9.BIZ"));
			FILE* outputFileName = fopen(outputFile, "wb");
			if (outputFileName == NULL)
			{
				MessageBox("Error opening temp output file", "Error");
				return false;
			}

			unsigned long start = 0x16;
			if ((gameType.Find("GoldenEye") == 0) || (gameType.Find("Killer Instinct") == 0))
			{
				start = start - 2;
				tempBufferNew[start] = 0x11;
				tempBufferNew[start+1] = 0x72;
			}
			else if (gameType.Find("PerfectDark") == 0)
			{
				start = start - 5;
				tempBufferNew[start] = 0x11;
				tempBufferNew[start+1] = 0x73;
				tempBufferNew[start+2] = ((size >> 16) & 0xFF);
				tempBufferNew[start+3] = ((size >> 8) & 0xFF);
				tempBufferNew[start+4] = ((size) & 0xFF);
			}
			else if (gameType.Find("BanjoKazooie") == 0)
			{
				start = start - 6;
				tempBufferNew[start] = 0x11;
				tempBufferNew[start+1] = 0x72;
				tempBufferNew[start+2] = ((size >> 24) & 0xFF);
				tempBufferNew[start+3] = ((size >> 16) & 0xFF);
				tempBufferNew[start+4] = ((size >> 8) & 0xFF);
				tempBufferNew[start+5] = ((size) & 0xFF);
			}
			else if (gameType.Find("DonkeyKong") == 0)
			{
				start = start - 0xA;
				tempBufferNew[start] = 0x1F;
				tempBufferNew[start+1] = 0x8B;
				tempBufferNew[start+2] = 0x08;
				tempBufferNew[start+3] = 0x00;
				tempBufferNew[start+4] = 0x00;
				tempBufferNew[start+5] = 0x00;
				tempBufferNew[start+6] = 0x00;
				tempBufferNew[start+7] = 0x00;
				tempBufferNew[start+8] = 0x02;
				tempBufferNew[start+9] = 0x03;
			}
			else if (gameType.Find("BlastCorps") == 0)
			{
				start = start;
			}
			else if (gameType.Find("Donkey Kong Demo") == 0)
			{
				start = start - 0x12;
				tempBufferNew[start] = 0x1F;
				tempBufferNew[start+1] = 0x8B;
				tempBufferNew[start+2] = 0x08;
				tempBufferNew[start+3] = 0x08;
				tempBufferNew[start+4] = 0x03;
				tempBufferNew[start+5] = 0x45;
				tempBufferNew[start+6] = 0x75;
				tempBufferNew[start+7] = 0x37;
				tempBufferNew[start+8] = 0x00;
				tempBufferNew[start+9] = 0x03;
				tempBufferNew[start+0xA] = 0x74;
				tempBufferNew[start+0xB] = 0x6D;
				tempBufferNew[start+0xC] = 0x70;
				tempBufferNew[start+0xD] = 0x2E;
				tempBufferNew[start+0xE] = 0x62;
				tempBufferNew[start+0xF] = 0x69;
				tempBufferNew[start+0x10] = 0x6E;
				tempBufferNew[start+0x11] = 0x00;
			}
			else if (gameType.Find("BanjoTooie") == 0)
			{

				start = start - 2;
				// is this checksum, not sure?
				tempBufferNew[start] = 0x0;
				tempBufferNew[start+1] = 0x15;
			}
			else if (gameType.Find("Conker") == 0)
			{
				start = start - 4;
				tempBufferNew[start] = ((size >> 24) & 0xFF);
				tempBufferNew[start+1] = ((size >> 16) & 0xFF);
				tempBufferNew[start+2] = ((size >> 8) & 0xFF);
				tempBufferNew[start+3] = ((size) & 0xFF);
			}



			if (byteFlipCompressed == true)
			{
				if ((sizeNew%2) == 1)
				{
					tempBufferNew[sizeNew-0x8] = 0;
					sizeNew++;
				}

				for (int x = 0; x < sizeNew; x=x+2)
				{
					unsigned char tempSpot = tempBufferNew[x];
					tempBufferNew[x] = tempBufferNew[x+1];
					tempBufferNew[x+1] = tempSpot;
				}
			}



			fwrite(&tempBufferNew[start], 1, (sizeNew-(start + 8)), outputFileName);	

			unsigned long fileSizeNew = (sizeNew-(start + 8));

			// don't pad
			/*if ((fileSizeNew % 0x8) != 0)
			{
				for (int x = 0; x < (0x8-(fileSizeNew % 0x8)); x++)
				{
					if ((gameName == "BanjoKazooie") || (gameName == "BanjoTooie"))
					{
						unsigned char tempZero = 0xAA;
						fwrite(&tempZero, 1, 1, outputFileName);
					}
					else
					{
						unsigned char tempZero = 0;
						fwrite(&tempZero, 1, 1, outputFileName);
					}
				}
			}*/

			delete [] tempBufferNew;
			fflush(outputFileName);
			fclose(outputFileName);
			return true;
		}
		else
		{
			MessageBox("Error Compressing - GZip didn't spit out a file", "Error");
			return false;
		}
	}
}
void CN64MidiToolDlg::OnBnClickedButtonimportmidi()
{
	if (buffer == NULL)
		return;

	if (m_spot.GetCount() == 0)
		return;

	CFileDialog m_ldFile(TRUE, NULL, "Midi.mid", OFN_HIDEREADONLY, "Midi (*.mid)|*.mid|", this);

	int isFileOpened = m_ldFile.DoModal();

	if (isFileOpened == IDCANCEL)
		return;
	
	CString gameName;
	m_game.GetWindowText(gameName);

	int gameNumber = 0;
	for (int x = 0; x < countGames; x++)
	{
		if (gameConfig[x].gameName.Find(gameName) == 0)
		{
			gameNumber = x;
			break;
		}
	}

	CString outputFilename = (mainFolder + "ASDxs34sMIDI.bin");
	if (gameConfig[gameNumber].gameType.Find("BanjoTooie") == 0)
	{
		if (!midiParse.MidiToBTFormat(m_ldFile.GetPathName(), outputFilename, m_loop.GetCheck(), 0, mNoRepeaters.GetCheck()))
			return;
	}
	else if (gameConfig[gameNumber].gameType.Find("MIDx") == 0)
	{
		MessageBox("Unsupported MIDx import");
		return;
	}
	else if (gameConfig[gameNumber].gameType.Find("MML") == 0)
	{
		MessageBox("Unsupported MML import");
		return;
	}
	else if (gameConfig[gameNumber].gameType.Find("MultipartZLibXMFastTracker2") == 0)
	{
		MessageBox("Unsupported Fast Tracker import");
		return;
	}
	else if (gameConfig[gameNumber].gameType.Find("DCM") == 0)
	{
		MessageBox("Unsupported DCM import");
		return;
	}
	else if (gameConfig[gameNumber].gameType.Find("LZSamplesDCM") == 0)
	{
		MessageBox("Unsupported LZSamplesDCM import");
		return;
	}
	else if (gameConfig[gameNumber].gameType.Find("Midi") == 0)
	{
		outputFilename = m_ldFile.GetPathName();
	}
	else if (gameConfig[gameNumber].gameType.Find("TitusMidi") == 0)
	{
		MessageBox("Unsupported Titus Midi Compressed import");
		return;
	}

	else if (gameConfig[gameNumber].gameType.Find("RNCMidi") == 0)
	{
		MessageBox("Unsupported Titus Midi Compressed import");
		return;
	}else
	{
		CString tempStr;
		mLoopPoint.GetWindowText(tempStr);
		int loop = atoi(tempStr);

		if (!midiParse.MidiToGEFormat(m_ldFile.GetPathName(), outputFilename, m_loop.GetCheck(), loop, mNoRepeaters.GetCheck()))
			return;
	}
	

	
	ConvertIntoSpot(outputFilename);
	::DeleteFile((mainFolder+"ASDxs34sMIDI.bin"));
}

void CN64MidiToolDlg::OnBnClickedButtonexportalltomidi()
{
	if (buffer == NULL)
		return;

	if (m_spot.GetCount() == 0)
		return;

	CFolderDialog tempDlg;
	int dialogbox = tempDlg.DoModal();
	if ((dialogbox == IDCANCEL) || (tempDlg.GetPathName()==""))
		return;

	CString tempPath = tempDlg.GetPathName();
	tempPath = (tempPath + "\\");

	for (int x = 0; x < m_spot.GetCount(); x++)
	{
		CString spotStr;
		m_spot.GetLBText(x, spotStr);

		unsigned long address = 0;
		unsigned long size = 0;
		sscanf(spotStr, "%08X:%08X", &address, &size);

		CString gameName;
		m_game.GetWindowText(gameName);

		int gameNumber = 0;
		for (int x = 0; x < countGames; x++)
		{
			if (gameConfig[x].gameName.Find(gameName) == 0)
			{
				gameNumber = x;
				break;
			}
		}

		if (size > 0)
		{
			CString outputName;
			if (gameConfig[gameNumber].gameType.Find("MultipartZLibXMFastTracker2") == 0)
			{
				outputName.Format("%s%s %08X %08X.xm", tempPath, gameName, x, address);
			}
			else if (gameConfig[gameNumber].gameType.Find("DCM") == 0)
			{
				outputName.Format("%s%s %08X %08X.dcm", tempPath, gameName, x, address);
			}
			else if (gameConfig[gameNumber].gameType.Find("LZSamplesDCM") == 0)
			{
				outputName.Format("%s%s %08X %08X.dcm", tempPath, gameName, x, address);
			}
			else
			{
				outputName.Format("%s%s %08X %08X.mid", tempPath, gameName, x, address);
			}
			try
			{
				CString gameName;
				m_game.GetWindowText(gameName);

				int gameNumber = 0;
				for (int x = 0; x < countGames; x++)
				{
					if (gameConfig[x].gameName.Find(gameName) == 0)
					{
						gameNumber = x;
						break;
					}
				}

				int numberInstruments;
				ExportToMidi(buffer, address, size, outputName, gameConfig[gameNumber].gameType, numberInstruments, 0);
			}
			catch (...)
			{
				MessageBox("Exception processing midi, maybe bad auto-read");
			}

		}
		else
		{
			int a =1;
		}
	}
}

void CN64MidiToolDlg::OnBnClickedButtonexportalltorawbin()
{
	if (buffer == NULL)
		return;

	if (m_spot.GetCount() == 0)
		return;

	CFolderDialog tempDlg;
	int dialogbox = tempDlg.DoModal();
	if ((dialogbox == IDCANCEL) || (tempDlg.GetPathName()==""))
		return;

	CString tempPath = tempDlg.GetPathName();
	tempPath = (tempPath + "\\");

	for (int x = 0; x < m_spot.GetCount(); x++)
	{
		CString spotStr;
		m_spot.GetLBText(x, spotStr);

		unsigned long address = 0;
		unsigned long size = 0;
		sscanf(spotStr, "%08X:%08X", &address, &size);

		CString gameName;
		m_game.GetWindowText(gameName);

		int gameNumber = 0;
		for (int x = 0; x < countGames; x++)
		{
			if (gameConfig[x].gameName.Find(gameName) == 0)
			{
				gameNumber = x;
				break;
			}
		}

		CString outputName;
		if (gameConfig[gameNumber].gameType.Find("MultipartZLibXMFastTracker2") == 0)
		{
			outputName.Format("%s%s %08X %08X.xm", tempPath, gameName, x, address);
			int numberInstruments;
			ExportToMidi(buffer, address, size, outputName, gameConfig[gameNumber].gameType, numberInstruments, 0);
		}
		else if (gameConfig[gameNumber].gameType.Find("DCM") == 0)
		{
			outputName.Format("%s%s %08X %08X.dcm", tempPath, gameName, x, address);
			int numberInstruments;
			ExportToMidi(buffer, address, size, outputName, gameConfig[gameNumber].gameType, numberInstruments, 0);
		}
		else if (gameConfig[gameNumber].gameType.Find("LZSamplesDCM") == 0)
		{
			outputName.Format("%s%s %08X %08X.dcm", tempPath, gameName, x, address);
			int numberInstruments;
			ExportToMidi(buffer, address, size, outputName, gameConfig[gameNumber].gameType, numberInstruments, 0);
		}
		else
		{
			outputName.Format("%s%s %08X %08X.bin", tempPath, gameName, x, address);
			ExportToBin(address, size, outputName);
		}
	}
}

void CN64MidiToolDlg::OnBnClickedButtonwriterom()
{
	CFileDialog m_svFileOutROM(FALSE, NULL, lastRomName + ".rom", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "ROM(*.v64;*.z64;*.rom;*.n64)|*.v64;*.z64;*.rom;*.n64|", this);

	int isFileOpened2 = m_svFileOutROM.DoModal();

	if ((isFileOpened2 != IDCANCEL) && (m_svFileOutROM.GetFileName() != ""))
	{
		FILE* outFile = fopen(m_svFileOutROM.GetPathName(), "wb");
		if (outFile == NULL)
		{
			MessageBox("Bad output file");
			return;
		}

		fwrite(buffer, 1, romSize, outFile);
		fclose(outFile);
	}
}

void CN64MidiToolDlg::OnBnClickedButtonplaymidi()
{
	KillWMPlayer();

	if (buffer == NULL)
		return;

	if (m_spot.GetCount() == 0)
		return;

	CString spotStr;
	m_spot.GetWindowText(spotStr);

	unsigned long address = 0;
	unsigned long size = 0;
	sscanf(spotStr, "%08X:%08X", &address, &size);

	CString gameName;
	m_game.GetWindowText(gameName);

	int gameNumber = 0;
	for (int x = 0; x < countGames; x++)
	{
		if (gameConfig[x].gameName.Find(gameName) == 0)
		{
			gameNumber = x;
			break;
		}
	}
	int numberInstruments;

	if (gameConfig[gameNumber].gameType.Find("MultipartZLibXMFastTracker2") == 0)
	{
		ExportToMidi(buffer, address, size, mainFolder + "tempAS123123as.xm", gameConfig[gameNumber].gameType, numberInstruments, 0);
		CString sParameter;
		ShellExecute( GetSafeHwnd(), "open", "explorer.exe", (mainFolder + "tempAS123123as.xm"), mainFolder, SW_SHOWNORMAL );
	}
	else if (gameConfig[gameNumber].gameType.Find("DCM") == 0)
	{
		ExportToMidi(buffer, address, size, mainFolder + "tempAS123123as.dcm", gameConfig[gameNumber].gameType, numberInstruments, 0);
		CString sParameter;
		normalExec((mainFolder + "MikIT.exe " + (mainFolder + "tempAS123123as.dcm ") + (mainFolder + "tempAS123123as.wav ") + "300").GetBuffer(), mainFolder);
		ShellExecute( GetSafeHwnd(), "open", "explorer.exe", (mainFolder + "tempAS123123as.wav"), mainFolder, SW_SHOWNORMAL );
	}
	else if (gameConfig[gameNumber].gameType.Find("LZSamplesDCM") == 0)
	{
		ExportToMidi(buffer, address, size, mainFolder + "tempAS123123as.dcm", gameConfig[gameNumber].gameType, numberInstruments, 0);
		CString sParameter;
		normalExec((mainFolder + "MikIT.exe " + (mainFolder + "tempAS123123as.dcm ") + (mainFolder + "tempAS123123as.wav ") + "300").GetBuffer(), mainFolder);
		ShellExecute( GetSafeHwnd(), "open", "explorer.exe", (mainFolder + "tempAS123123as.wav"), mainFolder, SW_SHOWNORMAL );
	}
	else
	{
		ExportToMidi(buffer, address, size, mainFolder + "tempAS123123as.mid", gameConfig[gameNumber].gameType, numberInstruments, 0);
		CString sParameter;
		ShellExecute( GetSafeHwnd(), "open", "explorer.exe", (mainFolder + "tempAS123123as.mid"), mainFolder, SW_SHOWNORMAL );
	}


}

void CN64MidiToolDlg::OnBnClickedButtonimportinstruments()
{
	CFileDialog m_ldFile(TRUE, NULL, "midilookup.txt", OFN_HIDEREADONLY, "Instrument Lookup Txt (*.txt)|*.txt|", this);

	int statusFileOpen = (int) m_ldFile.DoModal();

	if ((statusFileOpen == IDCANCEL) || (m_ldFile.GetPathName() == ""))
		return;

	if (statusFileOpen == FALSE)
		return;	

	midiParse.ImportMidiConfig(m_ldFile.GetPathName());
}

int CN64MidiToolDlg::WorkOnFilesIntoADirectory(char* pszFiles, CString fileNames[0x1000])
{
	int numberFiles = 0;

	WIN32_FIND_DATA files;
	HANDLE handle = FindFirstFile(pszFiles, &files);

	if(handle != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (files.dwFileAttributes & 0x10)
			{
				
			}
			else
			{
				fileNames[numberFiles] = files.cFileName;
				numberFiles++;
			}
		} while(FindNextFile(handle, &files));
	}

	return numberFiles;
}

void CN64MidiToolDlg::OnBnClickedButtonscandirformidis()
{
	CFolderDialog tempDlg;
	int dialogbox = tempDlg.DoModal();
	if ((dialogbox == IDCANCEL) || (tempDlg.GetPathName()==""))
		return;

	CString tempPath = tempDlg.GetPathName();
	tempPath = (tempPath + "\\");

	char tempPathChar[1000];
	strcpy(tempPathChar, tempPath);
	strcat(tempPathChar, "*.*");

	CFileDialog m_svFile(FALSE, "txt", "midiReport.txt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Txt File (*.txt)|*.txt|", this);

	int statusFileOpen = (int) m_svFile.DoModal();

	if ((statusFileOpen == IDCANCEL) || (m_svFile.GetFileName() == ""))
		return;

	if (statusFileOpen == FALSE)
		return;

	CString fileName;
	if (statusFileOpen == IDOK)
	{

		FILE* outFile = fopen(m_svFile.GetPathName(), "w");

		CString fileNames[0x10000];
		int count = WorkOnFilesIntoADirectory(tempPathChar, fileNames);

		for (int y = 0; y < count; y++)
		{
			
			struct stat results;
			stat((tempPath + fileNames[y]), &results);	

			if (results.st_size == 0)
				continue;

			FILE* inFile = fopen((tempPath + fileNames[y]), "rb");
			if (inFile == NULL)
				continue;
			
			unsigned char* binary = new unsigned char[results.st_size];
			fread(binary, 1, results.st_size, inFile);

			if (CharArrayToLong(&binary[0]) == 0x37804012)
			{
				for (int r = 0; r < results.st_size; r+=2)
				{
					unsigned char spot = binary[r];
					binary[r] = binary[r+1];
					binary[r+1] = spot;
				}
			}

			CString prev = "";

			if (results.st_size > 10000)
			{
				bool wroteNameHeader = false;
				for (unsigned long x = 0; x < (results.st_size - 0x44); x++)
				{
					//if ((CharArrayToLong(&binary[x]) == 0x00000044) && (CharArrayToLong(&binary[x + 0x40]) == 0x00000180))
					//if (CharArrayToLong(&binary[x]) == 0x42310001)
					//if (CharArrayToLong(&binary[x]) == 0x59617930) // yay0				
					//if (CharArrayToLong(&binary[x]) == 0x59617A30) // yaz0				
					//if (CharArrayToLong(&binary[x]) == 0x4D494F30) // mio0
					//if ((CharArrayToLong(&binary[x]) == 0x20696E66) && (CharArrayToLong(&binary[x+4]) == 0x6C617465)) // inflate
					//if (CharArrayToLong(&binary[x]) == 0xFFF330C0) // mp3
					//if (CharArrayToLong(&binary[x]) == 0x534E3634) // SN64
					//if (((CharArrayToLong(&binary[x]) & 0xFFFFFF00) == 0x42310000) && (binary[x+4] == 0) && (binary[x+3] > 0x01)) // Ctl multiple banks
					//if (CharArrayToLong(&binary[x]) == 0x4D546864) // MIDx
					//if (CharArrayToLong(&binary[x]) == 0x424F4653) // BOFS
					//if (CharArrayToLong(&binary[x]) == 0x464C4132) //FLA2
					/*if (
						((CharArrayToLong(&binary[x]) == 0x4E363420) && (CharArrayToLong(&binary[x+4]) == 0x50545220))
						|| 
						((CharArrayToLong(&binary[x]) == 0x6279206D) && (CharArrayToLong(&binary[x+4]) == 0x61747375))
						)*/
					//if (CharArrayToLong(&binary[x]) == 0x76706B30) //VPK0
					//if (
						//(CharArrayToLong(&binary[x]) == 0x36346464)
						//&& (CharArrayToLong(&binary[x+4]) == 0x2D4D756C)
						//&& (CharArrayToShort(&binary[x+8]) == 0x7469)
						//)//64dd-Multi
					//if (CharArrayToLong(&binary[x]) == 0x36344444) //64dd
					/*if ((
						(CharArrayToLong(&binary[x]) == 0x2D6C7A73) ||

						(CharArrayToLong(&binary[x]) == 0x2D6C7A34) ||

						(CharArrayToLong(&binary[x]) == 0x2D6C7A35) ||

						(CharArrayToLong(&binary[x]) == 0x2D6C6830) ||

						(CharArrayToLong(&binary[x]) == 0x2D6C6831) ||

						(CharArrayToLong(&binary[x]) == 0x2D6C6832) ||

						(CharArrayToLong(&binary[x]) == 0x2D6C6833) ||

						(CharArrayToLong(&binary[x]) == 0x2D6C6834) ||

						(CharArrayToLong(&binary[x]) == 0x2D6C6835))
						&& (binary[x+4] == 0x2D)) // lz/lh*/
					//if (CharArrayToLong(&binary[x]) == 0x4D4F5254) //MORT

					if (
						(CharArrayToLong(&binary[x]) == 0x46617374)
						&& (CharArrayToLong(&binary[x+4]) == 0x54726163)
						&& (CharArrayToLong(&binary[x+8]) == 0x6B657220)
						) // FastTracker
					
					{
						if (!wroteNameHeader)
						{
							wroteNameHeader = true;
							fprintf(outFile, "[%s]\n", fileNames[y]);
						}
						fprintf(outFile, "%08X,\n", x);
					}
					/*if ((CharArrayToLong(&binary[x]) == 0x4E363420) && (CharArrayToLong(&binary[x+4]) == 0x57617665)) // n64 wave
					{
						//fprintf(outFile, "%08X\n", x);
						//continue;
						if (prev == "ctl")
							fprintf(outFile, "%08X\n", x);
						else if (prev == "tbl")
							fprintf(outFile, "DTBL%08X\n", x);
						else
							fprintf(outFile, "FIRSTTBL\n%08X\n", x);
						prev = "tbl";
					}
					else if ((CharArrayToLong(&binary[x]) == 0x4E363420) && (CharArrayToLong(&binary[x+4]) == 0x50747254)) // n64 Ptr
					{
						//fprintf(outFile, "%08X,\n", x);
						//continue;
 						if (prev == "ctl")
							fprintf(outFile, "DCTL%08X,\n", x);
						else if (prev == "tbl")
							fprintf(outFile, "%08X,", x);
						else
							fprintf(outFile, "%08X,", x);
						prev = "ctl";
					}*/
					/*if ((CharArrayToLong(&binary[x]) & 0xFFFF0000) == 0x53310000) // S1 Midi Format
					{
						unsigned short numberMidis = CharArrayToShort(&binary[x+2]);
						unsigned long firstOffset = x + CharArrayToLong(&binary[x+4]);
						if (firstOffset < (results.st_size - 0x44))
						{
							if (CharArrayToLong(&binary[firstOffset]) == 0x00000044)
							{
								unsigned long lastOffset = x + CharArrayToLong(&binary[x+(8 * (numberMidis-1))]);
								unsigned long lastSize = CharArrayToLong(&binary[x+(8 * (numberMidis-1)) + 4]);
								fprintf(outFile, "[%s]\n", fileNames[y]);
								fprintf(outFile, "%08X,%08X\n", firstOffset, (lastOffset+lastSize));
								fflush(outFile);
							}
						}
					}*/
				}
			}

			delete [] binary;
			

			fclose(inFile);
		}

		fclose(outFile);
	}
}

void CN64MidiToolDlg::OnBnClickedButtonriproms()
{
	CFolderDialog tempDlg;
	int dialogbox = tempDlg.DoModal();
	if ((dialogbox == IDCANCEL) || (tempDlg.GetPathName()==""))
		return;

	CString tempPath = tempDlg.GetPathName();
	tempPath = (tempPath + "\\");

	

	for (int y = 0x0; y < countGames; y++)
	{
		CString gameName = (tempPath + gameConfig[y].gameName + " [!].z64");
		int gameROMSize = GetSizeFile(gameName);

		if (gameROMSize <= 0)
			continue;

		CString outputFolder = (tempPath + gameConfig[y].gameName + "\\");
		
		::CreateDirectory(outputFolder, NULL);

		try
		{
			
			FILE* inROM = fopen(gameName, "rb");

			if (inROM == NULL)
			{
				continue;
			}
			
			unsigned char* gamebuffer = new unsigned char[gameROMSize];
			fread(gamebuffer, 1, gameROMSize, inROM);
			fclose(inROM);

			bool byteFlippedROM = false;
			if (gamebuffer[0] == 0x37)
				byteFlippedROM = true;
			else if (gamebuffer[0] == 0x80)
				byteFlippedROM = false;
			else
			{
				//MessageBox("Not a valid ROM", "Error");
				//return;
			}

			if (byteFlippedROM == true)
			{
				for (int x = 0; x < gameROMSize; x=x+2)
				{
					unsigned char tempSpot = gamebuffer[x];
					gamebuffer[x] = gamebuffer[x+1];
					gamebuffer[x+1] = tempSpot;
				}
			}

			CString addMidiStrings[1000];
			int numberMidiStrings = 0;
			int numberInstruments = 0;
			ProcessMidis(gameConfig, y, gamebuffer, addMidiStrings, numberMidiStrings, numberInstruments);

			for (int x = 0; x < numberMidiStrings; x++)
			{
				unsigned long address;
				int size;
				sscanf(addMidiStrings[x], "%08X:%08X", &address, &size);

				CString tempOutFileNameStr;
				tempOutFileNameStr.Format("%s%02X_%08X_%08X.mid",outputFolder,x,address,size);
				int numberInstruments;
				ExportToMidi(gamebuffer, address, size, tempOutFileNameStr, gameConfig[y].gameType, numberInstruments, 0);
			}
			

			delete [] gamebuffer;

		}
		catch (char * str)
		{
			CString tempErrStr;
			tempErrStr.Format("%s", str);
			MessageBox(tempErrStr, "Error processing ");
		}	
	}
}

void CN64MidiToolDlg::OnBnClickedButton4()
{
	CFileDialog m_ldFile(TRUE, NULL, "GEMidi.bin", OFN_HIDEREADONLY, "Bin (*.bin)|*.bin|", this);

	int isFileOpened = m_ldFile.DoModal();

	if (isFileOpened == IDCANCEL)
		return;

	CFileDialog m_svFile(FALSE, "bin", "Midi.bin", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Bin File (*.bin)|*.bin|", this);

	int statusFileOpen = (int) m_svFile.DoModal();

	if ((statusFileOpen == IDCANCEL) || (m_svFile.GetFileName() == ""))
		return;

	if (statusFileOpen == FALSE)
		return;

	CString fileName;
	if (statusFileOpen == IDOK)
	{
		fileName = m_svFile.GetPathName();
	}
	else
	{
		return;
	}

	struct stat results;
	stat(m_ldFile.GetPathName(), &results);		

	FILE* inFile1 = fopen(m_ldFile.GetPathName(), "rb");
	if (inFile1 == NULL)
	{
		MessageBox("Error reading file", "Error");
		return;
	}	

	byte* inputMID = new byte[results.st_size];
	fread(inputMID, 1, results.st_size, inFile1);
	fclose(inFile1);

	midiParse.AddLoopGEFormat(inputMID, fileName, results.st_size, true, 0, true);

	delete [] inputMID;
}
