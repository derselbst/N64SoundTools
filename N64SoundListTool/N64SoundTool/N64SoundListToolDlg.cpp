b// N64SoundListToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "N64SoundListTool.h"
#include "N64SoundListToolDlg.h"
#include "mmsystem.h"
#include "OpenSpecificCtlTbl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MAXRESULTS 10

// CN64SoundListToolDlg dialog


CN64SoundListToolDlg::CN64SoundListToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CN64SoundListToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CN64SoundListToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBOSOUNDBANK, mSoundBankIndex);
	DDX_Control(pDX, IDC_COMBOSOUND, mInstrumentChoice);
	DDX_Control(pDX, IDC_EDITSAMPLINGRATE, mSamplingRate);
	DDX_Control(pDX, IDC_EDITVOL, mVolume);
	DDX_Control(pDX, IDC_EDITPAN, mPan);
	DDX_Control(pDX, IDC_EDITPRIORITY, mPriority);
	DDX_Control(pDX, IDC_EDITTREMTYPE, mTremType);
	DDX_Control(pDX, IDC_EDITTREMRATE, mTremRate);
	DDX_Control(pDX, IDC_EDITTREMDEPTH, mTremDepth);
	DDX_Control(pDX, IDC_EDITTREMDELAY, mTremDelay);
	DDX_Control(pDX, IDC_EDITTVIBTYPE, mVibType);
	DDX_Control(pDX, IDC_EDITVIBRATE, mVibRate);
	DDX_Control(pDX, IDC_EDITVIBDEPTH, mVibDepth);
	DDX_Control(pDX, IDC_EDITVIBDELAY, mVibDelay);
	DDX_Control(pDX, IDC_COMBOSOUND2, mSoundChoice);
	DDX_Control(pDX, IDC_EDITBASE, mBase);
	DDX_Control(pDX, IDC_EDITLENGTH, mLength);
	DDX_Control(pDX, IDC_CHECKRAW, mCheckRaw);
	DDX_Control(pDX, IDC_EDITSAMPLEVOL, mSampleVol);
	DDX_Control(pDX, IDC_EDITSAMPLEPAN, mSamplePan);
	DDX_Control(pDX, IDC_EDITATTACKTIME, mAttackTime);
	DDX_Control(pDX, IDC_EDITDECAYTIME, mDecayTime);
	DDX_Control(pDX, IDC_EDITRELEASETIME, mReleaseTime);
	DDX_Control(pDX, IDC_EDITATTACKVOLUME, mAttackVolume);
	DDX_Control(pDX, IDC_EDITDECAYVOLUME, mDecayVolume);
	DDX_Control(pDX, IDC_EDITVELMIN, mVelMin);
	DDX_Control(pDX, IDC_EDITVELMAX, mVelMax);
	DDX_Control(pDX, IDC_EDITKEYMIN, mKeyMin);
	DDX_Control(pDX, IDC_EDITKEYMAX, mKeyMax);
	DDX_Control(pDX, IDC_EDITKEYBASE, mKeyBase);
	DDX_Control(pDX, IDC_EDITDETUNE, mDetune);
	DDX_Control(pDX, IDC_EDITLOOP, mLoopStart);
	DDX_Control(pDX, IDC_EDITEND, mLoopEnd);
	DDX_Control(pDX, IDC_EDITCOUNT, mLoopCount);
	DDX_Control(pDX, IDC_CHECKLOOPENABLED, mLoopingEnabled);
	DDX_Control(pDX, IDC_STATICL1, mL1Text);
	DDX_Control(pDX, IDC_STATICL2, mL2Text);
	DDX_Control(pDX, IDC_STATICL3, mL3Text);
	DDX_Control(pDX, IDC_BUTTONIMPORTPREDICTORS, m_importPredictors);
	DDX_Control(pDX, IDC_BUTTONEXPORTPREDITORS, m_exportPredictors);
}

BEGIN_MESSAGE_MAP(CN64SoundListToolDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_OPENROM, &CN64SoundListToolDlg::OnFileOpenrom)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_COMBOSOUNDBANK, &CN64SoundListToolDlg::OnCbnSelchangeCombosoundbank)
	ON_CBN_SELCHANGE(IDC_COMBOSOUND, &CN64SoundListToolDlg::OnCbnSelchangeCombosound)
	ON_CBN_SELCHANGE(IDC_COMBOSOUND2, &CN64SoundListToolDlg::OnCbnSelchangeCombosound2)
	ON_EN_CHANGE(IDC_EDITSAMPLINGRATE, &CN64SoundListToolDlg::OnEnChangeEditsamplingrate)
	ON_EN_CHANGE(IDC_EDITVOL, &CN64SoundListToolDlg::OnEnChangeEditvol)
	ON_EN_CHANGE(IDC_EDITPAN, &CN64SoundListToolDlg::OnEnChangeEditpan)
	ON_EN_CHANGE(IDC_EDITPRIORITY, &CN64SoundListToolDlg::OnEnChangeEditpriority)
	ON_EN_CHANGE(IDC_EDITTREMTYPE, &CN64SoundListToolDlg::OnEnChangeEdittremtype)
	ON_EN_CHANGE(IDC_EDITTREMRATE, &CN64SoundListToolDlg::OnEnChangeEdittremrate)
	ON_EN_CHANGE(IDC_EDITTREMDEPTH, &CN64SoundListToolDlg::OnEnChangeEdittremdepth)
	ON_EN_CHANGE(IDC_EDITTREMDELAY, &CN64SoundListToolDlg::OnEnChangeEdittremdelay)
	ON_EN_CHANGE(IDC_EDITTVIBTYPE, &CN64SoundListToolDlg::OnEnChangeEdittvibtype)
	ON_EN_CHANGE(IDC_EDITVIBRATE, &CN64SoundListToolDlg::OnEnChangeEditvibrate)
	ON_EN_CHANGE(IDC_EDITVIBDEPTH, &CN64SoundListToolDlg::OnEnChangeEditvibdepth)
	ON_EN_CHANGE(IDC_EDITVIBDELAY, &CN64SoundListToolDlg::OnEnChangeEditvibdelay)
	ON_EN_CHANGE(IDC_EDITSAMPLEPAN, &CN64SoundListToolDlg::OnEnChangeEditsamplepan)
	ON_EN_CHANGE(IDC_EDITSAMPLEVOL, &CN64SoundListToolDlg::OnEnChangeEditsamplevol)
	ON_EN_CHANGE(IDC_EDITATTACKTIME, &CN64SoundListToolDlg::OnEnChangeEditattacktime)
	ON_EN_CHANGE(IDC_EDITDECAYTIME, &CN64SoundListToolDlg::OnEnChangeEditdecaytime)
	ON_EN_CHANGE(IDC_EDITRELEASETIME, &CN64SoundListToolDlg::OnEnChangeEditreleasetime)
	ON_EN_CHANGE(IDC_EDITATTACKVOLUME, &CN64SoundListToolDlg::OnEnChangeEditattackvolume)
	ON_EN_CHANGE(IDC_EDITDECAYVOLUME, &CN64SoundListToolDlg::OnEnChangeEditdecayvolume)
	ON_EN_CHANGE(IDC_EDITVELMIN, &CN64SoundListToolDlg::OnEnChangeEditvelmin)
	ON_EN_CHANGE(IDC_EDITVELMAX, &CN64SoundListToolDlg::OnEnChangeEditvelmax)
	ON_EN_CHANGE(IDC_EDITKEYMIN, &CN64SoundListToolDlg::OnEnChangeEditkeymin)
	ON_EN_CHANGE(IDC_EDITKEYMAX, &CN64SoundListToolDlg::OnEnChangeEditkeymax)
	ON_EN_CHANGE(IDC_EDITKEYBASE, &CN64SoundListToolDlg::OnEnChangeEditkeybase)
	ON_EN_CHANGE(IDC_EDITDETUNE, &CN64SoundListToolDlg::OnEnChangeEditdetune)
	ON_EN_CHANGE(IDC_EDITLOOP, &CN64SoundListToolDlg::OnEnChangeEditloop)
	ON_EN_CHANGE(IDC_EDITEND, &CN64SoundListToolDlg::OnEnChangeEditend)
	ON_EN_CHANGE(IDC_EDITCOUNT, &CN64SoundListToolDlg::OnEnChangeEditcount)
	ON_BN_CLICKED(IDC_CHECKLOOPENABLED, &CN64SoundListToolDlg::OnBnClickedCheckloopenabled)
	ON_BN_CLICKED(IDC_BUTTONUP, &CN64SoundListToolDlg::OnBnClickedButtonup)
	ON_BN_CLICKED(IDC_BUTTONDOWN, &CN64SoundListToolDlg::OnBnClickedButtondown)
	ON_BN_CLICKED(IDC_BUTTON1, &CN64SoundListToolDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTONVADPCMIMPORT, &CN64SoundListToolDlg::OnBnClickedButtonvadpcmimport)
	ON_BN_CLICKED(IDC_BUTTONADDSOUND, &CN64SoundListToolDlg::OnBnClickedButtonaddsound)
	ON_BN_CLICKED(IDC_BUTTONADDSOUND2, &CN64SoundListToolDlg::OnBnClickedButtonaddsound2)
	ON_BN_CLICKED(IDC_BUTTONDELETESOUND, &CN64SoundListToolDlg::OnBnClickedButtondeletesound)
	ON_BN_CLICKED(IDC_BUTTONDELETESOUND2, &CN64SoundListToolDlg::OnBnClickedButtondeletesound2)
	ON_COMMAND(ID_FILE_SAVE, &CN64SoundListToolDlg::OnFileSave)
	ON_BN_CLICKED(IDC_EXPORTCTLTBL, &CN64SoundListToolDlg::OnBnClickedExportctltbl)
	ON_BN_CLICKED(IDC_INJECTPLACE, &CN64SoundListToolDlg::OnBnClickedInjectplace)
	ON_BN_CLICKED(IDC_BUTTONRIP, &CN64SoundListToolDlg::OnBnClickedButtonrip)
	ON_BN_CLICKED(IDC_BUTTONPLAYSND, &CN64SoundListToolDlg::OnBnClickedButtonplayoriginalsound)
	ON_BN_CLICKED(IDC_BUTTONTEST, &CN64SoundListToolDlg::OnBnClickedButtontest)
	ON_COMMAND(ID_FILE_QUIT, &CN64SoundListToolDlg::OnFileQuit)
	ON_COMMAND(ID_FILE_OPENROMEXPLICIT, &CN64SoundListToolDlg::OnFileOpenromexplicit)
	ON_BN_CLICKED(IDC_BUTTONIMPORTPREDICTORS, &CN64SoundListToolDlg::OnBnClickedButtonimportpredictors)
	ON_BN_CLICKED(IDC_BUTTONEXPORTPREDITORS, &CN64SoundListToolDlg::OnBnClickedButtonexportpreditors)
	ON_BN_CLICKED(IDC_BUTTONIMPORT16BITRAW, &CN64SoundListToolDlg::OnBnClickedButtonimport16bitraw)
	ON_BN_CLICKED(IDC_BUTTONEXPORTRAW, &CN64SoundListToolDlg::OnBnClickedButtonexportraw)
END_MESSAGE_MAP()


// CN64SoundListToolDlg message handlers

BOOL CN64SoundListToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	dontupdateitall = true;
	romName = "";

	ROM = NULL;
	romSize = 0;
	results = new ctlTblResult[MAXRESULTS];
	numberResults = 0;
	alBankCurrent = NULL;

	char tempFolder[8000];
	::GetCurrentDirectory(8000, tempFolder);
	mainFolder.Format("%s\\", tempFolder);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CN64SoundListToolDlg::OnPaint()
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
HCURSOR CN64SoundListToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


bool CN64SoundListToolDlg::OpenROMPlusDlg()
{
	CFileDialog m_ldFile(TRUE, NULL, "Goldeneye.rom", OFN_HIDEREADONLY, "N64 ROM(*.v64;*.z64;*.rom;*.n64)|*.v64;*.z64;*.rom;*.n64|", this);

	int statusFileOpen = (int) m_ldFile.DoModal();

	if ((statusFileOpen == IDCANCEL) || (m_ldFile.GetPathName() == ""))
		return false;

	if (statusFileOpen == FALSE)
		return false;

	if (statusFileOpen != IDOK)
	{
		return false;
	}

	if (ROM != NULL)
	{
		delete [] ROM;
		ROM = NULL;
	}

	romSize = CSharedFunctions::GetSizeFile(m_ldFile.GetPathName());
	romName = m_ldFile.GetPathName();

	if (romSize == 0)
	{
		MessageBox("Error reading ROM");
		return false;
	}

	FILE* inFile = fopen(m_ldFile.GetPathName(), "rb");
	if (inFile == NULL)
	{
		romSize = 0;
		MessageBox("Error reading ROM");
		return false;
	}

	ROM = new unsigned char[romSize];
	fread(ROM, 1, romSize, inFile);

	if (CharArrayToLong(&ROM[0]) == 0x37804012)
	{
		for (int x = 0; x < romSize; x=x+2)
		{
			unsigned char tempSpot = ROM[x];
			ROM[x] = ROM[x+1];
			ROM[x+1] = tempSpot;
		}
	}

	fclose(inFile);

	return true;
}

void CN64SoundListToolDlg::OnFileOpenrom()
{

	/*unsigned char* geTempRom = new unsigned char[0xC00000];
	FILE * inGE = fopen("C:\\GoldeneyeStuff\\GE Editor Source\\Goldeneye.rom", "rb");
	fread(geTempRom, 1, 0xC00000, inGE);
	fclose(inGE);
	ALBank * GEBank = n64AudioLibrary.ReadAudio(&geTempRom[0x3B4450], 0x43A0, 0x0, &geTempRom[0x3B87F0]);*/

	/*unsigned char* geTempRom = new unsigned char[0x2000000];
	FILE * inGE = fopen("C:\\GoldeneyeStuff\\GE Editor Source\\Perfect_dark.v64", "rb");
	fread(geTempRom, 1, 0x2000000, inGE);
	fclose(inGE);
	ALBank * GEBank = n64AudioLibrary.ReadAudio(&geTempRom[0xCFBF30], 0xA060, 0x0, &geTempRom[0xD05F90]);*/


	if (!OpenROMPlusDlg())
		return;


	mSoundBankIndex.ResetContent();

	try
	{
		for (int x = 0; x < numberResults; x++)
		{
			n64AudioLibrary.DisposeALBank(results[x].bank);
		}

		alBankCurrent = NULL;

		numberResults = n64AudioLibrary.ReadCtlTblLocations(ROM, romSize, results);

		for (int x = 0; x < numberResults; x++)
		{
			CString tempStr;
			tempStr.Format("%02X - Ctl %08X (%08X) Tbl %08X (%08X)", x, results[x].ctlOffset, results[x].ctlSize, results[x].tblOffset, results[x].tblSize);
			mSoundBankIndex.AddString(tempStr);
		}




		/*for (int x = 0; x < GEBank->count; x++)
		{
			results[1].bank->inst[x] = GEBank->inst[x];
		}*/






		mSoundBankIndex.SetCurSel(0);

		OnCbnSelchangeCombosoundbank();
	}
	catch (char * str)
	{
		CString tempErrStr;
		tempErrStr.Format("%s", str);
		MessageBox(tempErrStr, "Error processing ");

		numberResults = 0;
	}
}
void CN64SoundListToolDlg::OnClose()
{
	CDialog::OnClose();
}

void CN64SoundListToolDlg::UpdateTextBoxes()
{
	if (alBankCurrent != NULL)
	{
		if (dontupdateitall)
		{
			dontupdateitall = false;

			CString tempStr;
			mSamplingRate.GetWindowText(tempStr);
			alBankCurrent->samplerate = atoi(tempStr);

			int instrumentSel = mInstrumentChoice.GetCurSel();

			mVolume.GetWindowText(tempStr);
			alBankCurrent->inst[instrumentSel]->volume = CSharedFunctions::StringToSignedChar(tempStr);
			
			mPan.GetWindowText(tempStr);
			alBankCurrent->inst[instrumentSel]->pan = CSharedFunctions::StringToSignedChar(tempStr);

			mPriority.GetWindowText(tempStr);
			alBankCurrent->inst[instrumentSel]->priority = CSharedFunctions::StringToSignedChar(tempStr);

			mTremType.GetWindowText(tempStr);
			alBankCurrent->inst[instrumentSel]->tremType = CSharedFunctions::StringToSignedChar(tempStr);

			mTremRate.GetWindowText(tempStr);
			alBankCurrent->inst[instrumentSel]->tremRate = CSharedFunctions::StringToSignedChar(tempStr);

			mTremDepth.GetWindowText(tempStr);
			alBankCurrent->inst[instrumentSel]->tremDepth = CSharedFunctions::StringToSignedChar(tempStr);

			mTremDelay.GetWindowText(tempStr);
			alBankCurrent->inst[instrumentSel]->tremDelay = CSharedFunctions::StringToSignedChar(tempStr);

			mVibType.GetWindowText(tempStr);
			alBankCurrent->inst[instrumentSel]->vibType = CSharedFunctions::StringToSignedChar(tempStr);

			mVibRate.GetWindowText(tempStr);
			alBankCurrent->inst[instrumentSel]->vibRate = CSharedFunctions::StringToSignedChar(tempStr);

			mVibDepth.GetWindowText(tempStr);
			alBankCurrent->inst[instrumentSel]->vibDepth = CSharedFunctions::StringToSignedChar(tempStr);

			mVibDelay.GetWindowText(tempStr);
			alBankCurrent->inst[instrumentSel]->vibDelay = CSharedFunctions::StringToSignedChar(tempStr);

			int soundSel = mSoundChoice.GetCurSel();

			mSamplePan.GetWindowText(tempStr);
			alBankCurrent->inst[instrumentSel]->sounds[soundSel]->samplePan = CSharedFunctions::StringToSignedChar(tempStr);

			mSampleVol.GetWindowText(tempStr);
			alBankCurrent->inst[instrumentSel]->sounds[soundSel]->sampleVolume = CSharedFunctions::StringToSignedChar(tempStr);
			
			mAttackTime.GetWindowText(tempStr);
			alBankCurrent->inst[instrumentSel]->sounds[soundSel]->env.attackTime =CSharedFunctions::StringHexToLong(tempStr);

			mDecayTime.GetWindowText(tempStr);
			alBankCurrent->inst[instrumentSel]->sounds[soundSel]->env.decayTime =CSharedFunctions::StringHexToLong(tempStr);

			mReleaseTime.GetWindowText(tempStr);
			alBankCurrent->inst[instrumentSel]->sounds[soundSel]->env.releaseTime =CSharedFunctions::StringHexToLong(tempStr);

			mAttackVolume.GetWindowText(tempStr);
			alBankCurrent->inst[instrumentSel]->sounds[soundSel]->env.attackVolume =CSharedFunctions::StringToUnsignedShort(tempStr);

			mDecayVolume.GetWindowText(tempStr);
			alBankCurrent->inst[instrumentSel]->sounds[soundSel]->env.decayVolume =CSharedFunctions::StringToUnsignedShort(tempStr);
			
			mVelMin.GetWindowText(tempStr);
			alBankCurrent->inst[instrumentSel]->sounds[soundSel]->key.velocitymin = CSharedFunctions::StringToSignedChar(tempStr);

			mVelMin.GetWindowText(tempStr);
			alBankCurrent->inst[instrumentSel]->sounds[soundSel]->key.velocitymin = CSharedFunctions::StringToSignedChar(tempStr);

			mVelMax.GetWindowText(tempStr);
			alBankCurrent->inst[instrumentSel]->sounds[soundSel]->key.velocitymax = CSharedFunctions::StringToSignedChar(tempStr);

			mKeyMin.GetWindowText(tempStr);
			alBankCurrent->inst[instrumentSel]->sounds[soundSel]->key.keymin = CSharedFunctions::StringToSignedChar(tempStr);

			mKeyMax.GetWindowText(tempStr);
			alBankCurrent->inst[instrumentSel]->sounds[soundSel]->key.keymax = CSharedFunctions::StringToSignedChar(tempStr);

			mKeyBase.GetWindowText(tempStr);
			alBankCurrent->inst[instrumentSel]->sounds[soundSel]->key.keybase = CSharedFunctions::StringToSignedChar(tempStr);

			mDetune.GetWindowText(tempStr);
			alBankCurrent->inst[instrumentSel]->sounds[soundSel]->key.detune = CSharedFunctions::StringToSignedChar(tempStr);

			if (alBankCurrent->inst[instrumentSel]->sounds[soundSel]->wav.type == AL_ADPCM_WAVE)
			{
				bool loopEnabled = mLoopingEnabled.GetCheck();

				if (loopEnabled)
				{
					if (alBankCurrent->inst[instrumentSel]->sounds[soundSel]->wav.adpcmWave->loop == NULL)
					{
						alBankCurrent->inst[instrumentSel]->sounds[soundSel]->wav.adpcmWave->loop = new ALADPCMloop();

						// TODO make real, better predictors, from looping point
						for (int x = 0; x < 16; x++)
						{
							alBankCurrent->inst[instrumentSel]->sounds[soundSel]->wav.adpcmWave->loop->state[x] = alBankCurrent->inst[instrumentSel]->sounds[soundSel]->wav.adpcmWave->book->predictors[x];
						}
					}
				}
				else
				{
					if (alBankCurrent->inst[instrumentSel]->sounds[soundSel]->wav.adpcmWave->loop != NULL)
					{
						delete alBankCurrent->inst[instrumentSel]->sounds[soundSel]->wav.adpcmWave->loop;
						alBankCurrent->inst[instrumentSel]->sounds[soundSel]->wav.adpcmWave->loop = NULL;
					}
				}

				if (alBankCurrent->inst[instrumentSel]->sounds[soundSel]->wav.adpcmWave->loop != NULL)
				{
					mLoopStart.GetWindowText(tempStr);
					alBankCurrent->inst[instrumentSel]->sounds[soundSel]->wav.adpcmWave->loop->start =CSharedFunctions::StringHexToLong(tempStr);

					mLoopEnd.GetWindowText(tempStr);
					alBankCurrent->inst[instrumentSel]->sounds[soundSel]->wav.adpcmWave->loop->end =CSharedFunctions::StringHexToLong(tempStr);

					mLoopCount.GetWindowText(tempStr);
					alBankCurrent->inst[instrumentSel]->sounds[soundSel]->wav.adpcmWave->loop->count =CSharedFunctions::StringHexToLong(tempStr);

					mLoopCount.ShowWindow(SW_SHOW);
					mLoopEnd.ShowWindow(SW_SHOW);
					mLoopStart.ShowWindow(SW_SHOW);

					mL1Text.ShowWindow(SW_SHOW);
					mL2Text.ShowWindow(SW_SHOW);
					mL3Text.ShowWindow(SW_SHOW);
				}
				else
				{
					mLoopCount.ShowWindow(SW_HIDE);
					mLoopEnd.ShowWindow(SW_HIDE);
					mLoopStart.ShowWindow(SW_HIDE);
					mL1Text.ShowWindow(SW_HIDE);
					mL2Text.ShowWindow(SW_HIDE);
					mL3Text.ShowWindow(SW_HIDE);
				}
			}
			else
			{
				bool loopEnabled = mLoopingEnabled.GetCheck();

				if (loopEnabled)
				{
					if (alBankCurrent->inst[instrumentSel]->sounds[soundSel]->wav.rawWave->loop == NULL)
						alBankCurrent->inst[instrumentSel]->sounds[soundSel]->wav.rawWave->loop = new ALRawLoop();
				}
				else
				{
					if (alBankCurrent->inst[instrumentSel]->sounds[soundSel]->wav.rawWave->loop != NULL)
					{
						delete alBankCurrent->inst[instrumentSel]->sounds[soundSel]->wav.rawWave->loop;
						alBankCurrent->inst[instrumentSel]->sounds[soundSel]->wav.rawWave->loop = NULL;
					}
				}

				if (alBankCurrent->inst[instrumentSel]->sounds[soundSel]->wav.rawWave->loop != NULL)
				{
					mLoopStart.GetWindowText(tempStr);
					alBankCurrent->inst[instrumentSel]->sounds[soundSel]->wav.rawWave->loop->start =CSharedFunctions::StringHexToLong(tempStr);

					mLoopEnd.GetWindowText(tempStr);
					alBankCurrent->inst[instrumentSel]->sounds[soundSel]->wav.rawWave->loop->end =CSharedFunctions::StringHexToLong(tempStr);

					mLoopCount.GetWindowText(tempStr);
					alBankCurrent->inst[instrumentSel]->sounds[soundSel]->wav.rawWave->loop->count =CSharedFunctions::StringHexToLong(tempStr);
					
					mLoopCount.ShowWindow(SW_SHOW);
					mLoopEnd.ShowWindow(SW_SHOW);
					mLoopStart.ShowWindow(SW_SHOW);

					mL1Text.ShowWindow(SW_SHOW);
					mL2Text.ShowWindow(SW_SHOW);
					mL3Text.ShowWindow(SW_SHOW);
				}
				else
				{
					mLoopCount.ShowWindow(SW_HIDE);
					mLoopEnd.ShowWindow(SW_HIDE);
					mLoopStart.ShowWindow(SW_HIDE);
					mL1Text.ShowWindow(SW_HIDE);
					mL2Text.ShowWindow(SW_HIDE);
					mL3Text.ShowWindow(SW_HIDE);
				}
			}
			
			dontupdateitall = true;
		}
	}
}

void CN64SoundListToolDlg::OnCbnSelchangeCombosoundbank()
{
	int bankIndex = mSoundBankIndex.GetCurSel();
	if (bankIndex == -1)
		return;

	if (dontupdateitall)
	{
		dontupdateitall = false;

		alBankCurrent = results[bankIndex].bank;

		mInstrumentChoice.ResetContent();

		for (int x = 0; x < alBankCurrent->count; x++)
		{
			CString tempStr;
			tempStr.Format("%04X", x);
			mInstrumentChoice.AddString(tempStr);
		}

		mInstrumentChoice.SetCurSel(0);

		CString tempSampleStr;
		tempSampleStr.Format("%d", alBankCurrent->samplerate);
		mSamplingRate.SetWindowText(tempSampleStr);

		dontupdateitall = true;
	}

	OnCbnSelchangeCombosound();
}

void CN64SoundListToolDlg::OnCbnSelchangeCombosound()
{
	if (dontupdateitall)
	{
		dontupdateitall = false;

		int instrSel = mInstrumentChoice.GetCurSel();

		CString tempStr;
		tempStr.Format("%02X", alBankCurrent->inst[instrSel]->volume);
		mVolume.SetWindowText(tempStr);

		tempStr.Format("%02X", alBankCurrent->inst[instrSel]->pan);
		mPan.SetWindowText(tempStr);

		tempStr.Format("%02X", alBankCurrent->inst[instrSel]->priority);
		mPriority.SetWindowText(tempStr);

		tempStr.Format("%02X", alBankCurrent->inst[instrSel]->tremType);
		mTremType.SetWindowText(tempStr);

		tempStr.Format("%02X", alBankCurrent->inst[instrSel]->tremRate);
		mTremRate.SetWindowText(tempStr);

		tempStr.Format("%02X", alBankCurrent->inst[instrSel]->tremDepth);
		mTremDepth.SetWindowText(tempStr);

		tempStr.Format("%02X", alBankCurrent->inst[instrSel]->tremDelay);
		mTremDelay.SetWindowText(tempStr);

		tempStr.Format("%02X", alBankCurrent->inst[instrSel]->vibType);
		mVibType.SetWindowText(tempStr);

		tempStr.Format("%02X", alBankCurrent->inst[instrSel]->vibRate);
		mVibRate.SetWindowText(tempStr);

		tempStr.Format("%02X", alBankCurrent->inst[instrSel]->vibDepth);
		mVibDepth.SetWindowText(tempStr);

		tempStr.Format("%02X", alBankCurrent->inst[instrSel]->vibDelay);
		mVibDelay.SetWindowText(tempStr);

		mSoundChoice.ResetContent();

		for (int x = 0; x < alBankCurrent->inst[instrSel]->soundCount; x++)
		{
			CString tempStr;
			tempStr.Format("%04X", x);
			mSoundChoice.AddString(tempStr);
		}

		mSoundChoice.SetCurSel(0);

		dontupdateitall = true;
	}

	OnCbnSelchangeCombosound2();
}

void CN64SoundListToolDlg::OnCbnSelchangeCombosound2()
{
	if (dontupdateitall)
	{
		dontupdateitall = false;

		int instrSel = mInstrumentChoice.GetCurSel();
		int soundChoice = mSoundChoice.GetCurSel();

		mCheckRaw.SetCheck(alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.type);

		CString tempStr;
		tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.base);
		mBase.SetWindowText(tempStr);

		tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.len);
		mLength.SetWindowText(tempStr);

		tempStr.Format("%02X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->samplePan);
		mSamplePan.SetWindowText(tempStr);

		tempStr.Format("%02X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->sampleVolume);
		mSampleVol.SetWindowText(tempStr);

		tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->env.attackTime);
		mAttackTime.SetWindowText(tempStr);

		tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->env.decayTime);
		mDecayTime.SetWindowText(tempStr);

		tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->env.releaseTime);
		mReleaseTime.SetWindowText(tempStr);

		tempStr.Format("%04X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->env.attackVolume);
		mAttackVolume.SetWindowText(tempStr);

		tempStr.Format("%04X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->env.decayVolume);
		mDecayVolume.SetWindowText(tempStr);

		tempStr.Format("%02X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->key.velocitymin);
		mVelMin.SetWindowText(tempStr);

		tempStr.Format("%02X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->key.velocitymax);
		mVelMax.SetWindowText(tempStr);

		tempStr.Format("%02X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->key.keymin);
		mKeyMin.SetWindowText(tempStr);

		tempStr.Format("%02X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->key.keymax);
		mKeyMax.SetWindowText(tempStr);

		tempStr.Format("%02X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->key.keybase);
		mKeyBase.SetWindowText(tempStr);

		tempStr.Format("%02X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->key.detune);
		mDetune.SetWindowText(tempStr);

		if ( alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.type == AL_ADPCM_WAVE)
		{
			mLoopingEnabled.SetCheck(alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.adpcmWave->loop != NULL);
			mLoopingEnabled.EnableWindow(true);

			if (alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.adpcmWave->loop != NULL)
			{
				tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.adpcmWave->loop->start);
				mLoopStart.SetWindowText(tempStr);

				tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.adpcmWave->loop->end);
				mLoopEnd.SetWindowText(tempStr);

				tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.adpcmWave->loop->count);
				mLoopCount.SetWindowText(tempStr);

				mLoopCount.ShowWindow(SW_SHOW);
				mLoopEnd.ShowWindow(SW_SHOW);
				mLoopStart.ShowWindow(SW_SHOW);
				mL1Text.ShowWindow(SW_SHOW);
				mL2Text.ShowWindow(SW_SHOW);
				mL3Text.ShowWindow(SW_SHOW);
			}
			else
			{
				mLoopCount.ShowWindow(SW_HIDE);
				mLoopEnd.ShowWindow(SW_HIDE);
				mLoopStart.ShowWindow(SW_HIDE);
				mL1Text.ShowWindow(SW_HIDE);
				mL2Text.ShowWindow(SW_HIDE);
				mL3Text.ShowWindow(SW_HIDE);
			}

			m_exportPredictors.EnableWindow(true);
			m_importPredictors.EnableWindow(true);
		}
		else
		{
			mLoopingEnabled.SetCheck(alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.rawWave->loop != NULL);

			mLoopingEnabled.EnableWindow(true);
			if (alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.rawWave->loop != NULL)
			{
				tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.rawWave->loop->start);
				mLoopStart.SetWindowText(tempStr);

				tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.rawWave->loop->end);
				mLoopEnd.SetWindowText(tempStr);

				tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.rawWave->loop->count);
				mLoopCount.SetWindowText(tempStr);

				mLoopCount.ShowWindow(SW_SHOW);
				mLoopEnd.ShowWindow(SW_SHOW);
				mLoopStart.ShowWindow(SW_SHOW);
				mL1Text.ShowWindow(SW_SHOW);
				mL2Text.ShowWindow(SW_SHOW);
				mL3Text.ShowWindow(SW_SHOW);
			}
			else
			{
				mLoopCount.ShowWindow(SW_HIDE);
				mLoopEnd.ShowWindow(SW_HIDE);
				mLoopStart.ShowWindow(SW_HIDE);
				mL1Text.ShowWindow(SW_HIDE);
				mL2Text.ShowWindow(SW_HIDE);
				mL3Text.ShowWindow(SW_HIDE);
			}

			m_exportPredictors.EnableWindow(false);
			m_importPredictors.EnableWindow(false);
		}

		dontupdateitall = true;
	}
}

void CN64SoundListToolDlg::OnEnChangeEditsamplingrate()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEditvol()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEditpan()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEditpriority()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEdittremtype()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEdittremrate()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEdittremdepth()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEdittremdelay()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEdittvibtype()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEditvibrate()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEditvibdepth()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEditvibdelay()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEditsamplepan()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEditsamplevol()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEditattacktime()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEditdecaytime()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEditreleasetime()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEditattackvolume()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEditdecayvolume()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEditvelmin()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEditvelmax()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEditkeymin()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEditkeymax()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEditkeybase()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEditdetune()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEditloop()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEditend()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEditcount()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnBnClickedCheckloopenabled()
{
	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnBnClickedButtonup()
{
	if (alBankCurrent != NULL)
	{
		int instrSel = mInstrumentChoice.GetCurSel();
		int soundChoice = mSoundChoice.GetCurSel();

		n64AudioLibrary.MoveUpSound(alBankCurrent, instrSel, soundChoice);

		if (dontupdateitall)
		{
			dontupdateitall = false;
			if (soundChoice != 0)
				mSoundChoice.SetCurSel(soundChoice-1);
			dontupdateitall = true;
		}
		OnCbnSelchangeCombosound2();
	}
}

void CN64SoundListToolDlg::OnBnClickedButtondown()
{
	if (alBankCurrent != NULL)
	{
		int instrSel = mInstrumentChoice.GetCurSel();
		int soundChoice = mSoundChoice.GetCurSel();

		n64AudioLibrary.MoveDownSound(alBankCurrent, instrSel, soundChoice);

		if (dontupdateitall)
		{
			dontupdateitall = false;
			if (soundChoice != (alBankCurrent->inst[instrSel]->soundCount-1))
				mSoundChoice.SetCurSel(soundChoice+1);
			dontupdateitall = true;
		}
		OnCbnSelchangeCombosound2();
	}
}

void CN64SoundListToolDlg::OnBnClickedButtonplayoriginalsound()
{
	if (alBankCurrent != NULL)
	{
		int instrSel = mInstrumentChoice.GetCurSel();
		int soundSel = mSoundChoice.GetCurSel();

		KillWMPlayer();

		if (n64AudioLibrary.ExtractRawSound(alBankCurrent, instrSel, soundSel, (mainFolder + "tempWav231A24r.wav"), alBankCurrent->samplerate))
		{
			PlayWMPlayerSound((mainFolder + "tempWav231A24r.wav"));
		}
	}
}

void CN64SoundListToolDlg::OnBnClickedButton1()
{
	if (alBankCurrent != NULL)
	{
		CFileDialog m_ldFile(TRUE, "wav", "raw16.wav", OFN_HIDEREADONLY, "16-bit Raw Wave file (*.wav)|*.wav|", this);

		int statusFileOpen = (int) m_ldFile.DoModal();
		CString fileName = m_ldFile.GetPathName();
		if (statusFileOpen == IDOK)
		{
			FILE* inFile = fopen(m_ldFile.GetPathName(), "r");
			if (inFile == NULL)
			{
				MessageBox("Error opening", "Error");
				return;
			}
			fclose(inFile);

			int instrSel = mInstrumentChoice.GetCurSel();
			int soundChoice = mSoundChoice.GetCurSel();

			unsigned long samplingRate;
			if (!n64AudioLibrary.ReplaceSoundWithWavData(alBankCurrent, instrSel, soundChoice, m_ldFile.GetPathName(), samplingRate, AL_RAW16_WAVE))
				return;

			if (alBankCurrent->samplerate != samplingRate)
			{
				CString tempStr;
				tempStr.Format("Warning sampling rate of wav %d is not equal to sampling rate of %d", samplingRate, alBankCurrent->samplerate);
				MessageBox(tempStr, "Warning");
			}

			OnCbnSelchangeCombosound2();
		}
	}
}

void CN64SoundListToolDlg::OnBnClickedButtonvadpcmimport()
{
	if (alBankCurrent != NULL)
	{
		CFileDialog m_ldFile(TRUE, "wav", "raw16.wav", OFN_HIDEREADONLY, "16-bit Raw Wave file (*.wav)|*.wav|", this);

		int statusFileOpen = (int) m_ldFile.DoModal();
		CString fileName = m_ldFile.GetPathName();
		if (statusFileOpen == IDOK)
		{
			FILE* inFile = fopen(m_ldFile.GetPathName(), "r");
			if (inFile == NULL)
			{
				MessageBox("Error opening", "Error");
				return;
			}
			fclose(inFile);

			int instrSel = mInstrumentChoice.GetCurSel();
			int soundChoice = mSoundChoice.GetCurSel();

			unsigned long samplingRate;
			if (!n64AudioLibrary.ReplaceSoundWithWavData(alBankCurrent, instrSel, soundChoice, m_ldFile.GetPathName(), samplingRate, AL_ADPCM_WAVE))
				return;

			if (alBankCurrent->samplerate != samplingRate)
			{
				CString tempStr;
				tempStr.Format("Warning sampling rate of wav %d is not equal to sampling rate of %d", samplingRate, alBankCurrent->samplerate);
				MessageBox(tempStr, "Warning");
			}

			OnCbnSelchangeCombosound2();
		}
	}
}

void CN64SoundListToolDlg::OnBnClickedButtonaddsound()
{
	if (alBankCurrent != NULL)
	{
		CFileDialog m_ldFile(TRUE, "wav", "raw16.wav", OFN_HIDEREADONLY, "16-bit Raw Wave file (*.wav)|*.wav|", this);

		int statusFileOpen = (int) m_ldFile.DoModal();
		CString fileName = m_ldFile.GetPathName();
		if (statusFileOpen == IDOK)
		{
			FILE* inFile = fopen(m_ldFile.GetPathName(), "r");
			if (inFile == NULL)
			{
				MessageBox("Error opening", "Error");
				return;
			}
			fclose(inFile);

			int instrSel = mInstrumentChoice.GetCurSel();
			int soundChoice = mSoundChoice.GetCurSel();

			unsigned long samplingRate;
			
			n64AudioLibrary.AddSound(alBankCurrent, instrSel,  m_ldFile.GetPathName(), samplingRate, AL_RAW16_WAVE);
			
			if (alBankCurrent->samplerate != samplingRate)
			{
				CString tempStr;
				tempStr.Format("Warning sampling rate of wav %d is not equal to sampling rate of %d", samplingRate, alBankCurrent->samplerate);
				MessageBox(tempStr, "Warning");
			}

			OnCbnSelchangeCombosound();

			if (dontupdateitall)
			{
				dontupdateitall = false;
				mSoundChoice.SetCurSel(alBankCurrent->inst[instrSel]->soundCount-1);
				dontupdateitall = true;
			}
			OnCbnSelchangeCombosound2();
		}
	}
}

void CN64SoundListToolDlg::OnBnClickedButtonaddsound2()
{
	if (alBankCurrent != NULL)
	{
		CFileDialog m_ldFile(TRUE, "wav", "raw16.wav", OFN_HIDEREADONLY, "16-bit Raw Wave file (*.wav)|*.wav|", this);

		int statusFileOpen = (int) m_ldFile.DoModal();
		CString fileName = m_ldFile.GetPathName();
		if (statusFileOpen == IDOK)
		{
			FILE* inFile = fopen(m_ldFile.GetPathName(), "r");
			if (inFile == NULL)
			{
				MessageBox("Error opening", "Error");
				return;
			}
			fclose(inFile);

			int instrSel = mInstrumentChoice.GetCurSel();
			int soundChoice = mSoundChoice.GetCurSel();

			unsigned long samplingRate;
			
			n64AudioLibrary.AddSound(alBankCurrent, instrSel,  m_ldFile.GetPathName(), samplingRate, AL_ADPCM_WAVE);
			
			if (alBankCurrent->samplerate != samplingRate)
			{
				CString tempStr;
				tempStr.Format("Warning sampling rate of wav %d is not equal to sampling rate of %d", samplingRate, alBankCurrent->samplerate);
				MessageBox(tempStr, "Warning");
			}

			OnCbnSelchangeCombosound();

			if (dontupdateitall)
			{
				dontupdateitall = false;
				mSoundChoice.SetCurSel(alBankCurrent->inst[instrSel]->soundCount-1);
				dontupdateitall = true;
			}
			OnCbnSelchangeCombosound2();
		}
	}
}

void CN64SoundListToolDlg::OnBnClickedButtondeletesound()
{
	if (alBankCurrent != NULL)
	{
		int instrSel = mInstrumentChoice.GetCurSel();
		int soundChoice = mSoundChoice.GetCurSel();

		n64AudioLibrary.DeleteSound(alBankCurrent, instrSel, soundChoice);

		OnCbnSelchangeCombosound();

		if (dontupdateitall)
		{
			dontupdateitall = false;
			if (soundChoice != 0)
				mSoundChoice.SetCurSel(soundChoice-1);
			dontupdateitall = true;
		}
		OnCbnSelchangeCombosound2();
	}
}

void CN64SoundListToolDlg::OnBnClickedButtondeletesound2()
{
	if (alBankCurrent != NULL)
	{
		CFileDialog m_svFile(FALSE, "wav", "ExtractedSound.wav", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Sound (*.wav)|*.wav|");

		int statusFileOpen = (int) m_svFile.DoModal();
		CString fileName = m_svFile.GetFileName();
		if ((statusFileOpen == IDOK) && (fileName != ""))
		{
			int instrSel = mInstrumentChoice.GetCurSel();
			int soundChoice = mSoundChoice.GetCurSel();

			if (n64AudioLibrary.ExtractRawSound(alBankCurrent, instrSel, soundChoice, m_svFile.GetPathName(), alBankCurrent->samplerate))
			{

			}
		}
	}
}

void CN64SoundListToolDlg::OnFileSave()
{
	if (alBankCurrent != NULL)
	{
		CFileDialog m_svFileOutROM(FALSE, "rom", (romName + ".rom"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "N64 ROM(*.v64;*.z64;*.rom;*.n64)|*.v64;*.z64;*.rom;*.n64|", this);

		int isFileOpened2 = m_svFileOutROM.DoModal();

		if ((isFileOpened2 != IDCANCEL) && (m_svFileOutROM.GetFileName() != ""))
		{
			CString romOutName = m_svFileOutROM.GetPathName();

			FILE* outFile = fopen(m_svFileOutROM.GetPathName(), "wb");
			if (outFile == NULL)
			{
				MessageBox("Error opening ROM output");
				return;
			}
			fwrite(ROM, 1, romSize, outFile);
			fclose(outFile);

			/*if (GetSizeFile((mainFolder + "rn64crc.exe")) == 0)
			{
				MessageBox("Warning, rn64crc.exe not found in main folder, could not re-crc ROM, if needed");
				return;
			}

			CString RN64dir = romOutName.Mid(0, (romOutName.ReverseFind('\\')+1));
			CString RN64name = romOutName.Mid((romOutName.ReverseFind('\\')+1), (romOutName.GetLength() - (romOutName.ReverseFind('\\')+1)));

			if (RN64dir != mainFolder)
				::CopyFile((mainFolder + "rn64crc.exe"), (RN64dir + "rn64crc.exe"), false);

			::SetCurrentDirectory(RN64dir);

			CString tempStr;
			tempStr.Format("rn64crc.exe -u \"%s\"", RN64name);
			hiddenExec(_T(tempStr.GetBuffer()), RN64dir);*/
		}
	}
}

void CN64SoundListToolDlg::KillWMPlayer()
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

void CN64SoundListToolDlg::PlayWMPlayerSound(CString wavFileSound)
{
	if (CSharedFunctions::GetSizeFile(wavFileSound) > 0)
	{
		STARTUPINFO si;
		::ZeroMemory(&si,sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);

		PROCESS_INFORMATION pi;  
		::ZeroMemory(&si,sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);
		CString tempwmpStr;
		tempwmpStr.Format("C:\\Program Files\\Windows Media Player\\wmplayer.exe \"%s\"", wavFileSound);
		LPSTR pwsz = tempwmpStr.GetBuffer(0);
		CString strFilePath = tempwmpStr ;
			
		CFileFind finder;
		//Find the Default Drives 
		if(TRUE == finder.FindFile("C:\\Program Files\\Windows Media Player\\wmplayer.exe",0))
		{
			finder.FindNextFile() ;
				if(CreateProcess(NULL, pwsz, NULL,
			NULL,FALSE,NORMAL_PRIORITY_CLASS,NULL,NULL,&si,&pi))
			{
						::CloseHandle(pi.hThread);
						::CloseHandle(pi.hProcess);
			}
			
		}

		return;
	}
}

BOOL CN64SoundListToolDlg::hiddenExec (PTSTR pCmdLine, CString currentDirectory)
{
	::SetCurrentDirectory(currentDirectory);
   STARTUPINFO si;
   PROCESS_INFORMATION processInfo;
   ZeroMemory(&si, sizeof(si));
   si.cb           = sizeof(si);
   si.dwFlags      = STARTF_USESHOWWINDOW;
   si.wShowWindow  = SW_HIDE;
   ZeroMemory(&processInfo, sizeof(processInfo));


   if (currentDirectory.ReverseFind('\\') == (currentDirectory.GetLength()-1))
   {
		currentDirectory = currentDirectory.Mid(0, (currentDirectory.GetLength()-1));
   }

   /*return */CreateProcess(0, pCmdLine, 0, 0, FALSE, 0, 0, currentDirectory, &si, &processInfo);
   WaitForSingleObject(processInfo.hProcess, 20000);
   DWORD exitCode;
   if (GetExitCodeProcess(processInfo.hProcess, &exitCode))
   {
        if (exitCode == STILL_ACTIVE)
		{
			MessageBox("For some reason Process Failed", "Error");
			TerminateProcess(processInfo.hProcess, exitCode);
			return false;
		}
   }   
   return true;
};

void CN64SoundListToolDlg::OnBnClickedExportctltbl()
{
	if (alBankCurrent != NULL)
	{
		CFileDialog m_svFileCtl(FALSE, "ctl", "SoundBank.ctl", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Ctl File (*.ctl)|*.ctl|", this);

		int statusFileOpen2 = (int) m_svFileCtl.DoModal();

		if ((statusFileOpen2 == IDCANCEL) || (m_svFileCtl.GetPathName() == ""))
			return;

		if (statusFileOpen2 == FALSE)
			return;

		CFileDialog m_svFileTbl(FALSE, "tbl", "SoundBank.tbl", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Tbl File (*.tbl)|*.tbl|", this);

		statusFileOpen2 = (int) m_svFileTbl.DoModal();

		if ((statusFileOpen2 == IDCANCEL) || (m_svFileTbl.GetPathName() == ""))
			return;

		if (statusFileOpen2 == FALSE)
			return;

		n64AudioLibrary.WriteAudioToFile(alBankCurrent, m_svFileCtl.GetPathName(), m_svFileTbl.GetPathName());
	}
}

void CN64SoundListToolDlg::OnBnClickedInjectplace()
{
	if ((alBankCurrent != NULL) && (romSize > 0))
	{
		int bank = mSoundBankIndex.GetCurSel();
		if (bank == -1)
			return;

		unsigned char* ctl;
		unsigned char* tbl;
		int ctlSize, tblSize;
		n64AudioLibrary.WriteAudio(alBankCurrent, ctl, ctlSize, tbl, tblSize);
		if (n64AudioLibrary.InjectCtlTblIntoROMArrayInPlace(ROM, ctl, ctlSize, tbl, tblSize, results[bank].ctlOffset, results[bank].tblOffset, results[bank].ctlSize, results[bank].tblSize))
		{
			for (int x = (results[bank].ctlOffset + ctlSize); x < (results[bank].ctlOffset + results[bank].ctlSize); x++)
			{
				ROM[x] = 0xFF;
			}
		}
	}
}


bool CN64SoundListToolDlg::GetFolder(CString& folderpath, const char* szCaption, HWND hOwner)
{
	bool retVal = false;

	// The BROWSEINFO struct tells the shell 
	// how it should display the dialog.
	BROWSEINFO bi;
	memset(&bi, 0, sizeof(bi));

	bi.ulFlags   = BIF_USENEWUI;
	bi.hwndOwner = hOwner;
	bi.lpszTitle = szCaption;

	// must call this if using BIF_USENEWUI
	::OleInitialize(NULL);

	// Show the dialog and get the itemIDList for the selected folder.
	LPITEMIDLIST pIDL = ::SHBrowseForFolder(&bi);

	if(pIDL != NULL)
	{
		// Create a buffer to store the path, then get the path.
		char buffer[_MAX_PATH] = {'\0'};
		if(::SHGetPathFromIDList(pIDL, buffer) != 0)
		{
			// Set the string value.
			folderpath = buffer;
			retVal = true;
		}		

		// free the item id list
		CoTaskMemFree(pIDL);
	}

	::OleUninitialize();

	return retVal;
}

void CN64SoundListToolDlg::OnBnClickedButtonrip()
{
	if (alBankCurrent != NULL)
	{
		CString outputFolder;
		if (GetFolder(outputFolder, "Folder To Save To", NULL))
		{

			for (int x = 0; x < alBankCurrent->count; x++)
			{
				for (int y = 0; y < alBankCurrent->inst[x]->soundCount; y++)
				{
					CString tempExportNameStr;
					tempExportNameStr.Format("%s\\INSTR_%04X_SND_%04X.wav", outputFolder, x, y);
					n64AudioLibrary.ExtractRawSound(alBankCurrent, x, y, tempExportNameStr, alBankCurrent->samplerate);
				}
			}
		}
	}
}
void CN64SoundListToolDlg::OnBnClickedButtontest()
{
	if (alBankCurrent != NULL)
	{
		CFileDialog m_ldFile(TRUE, "wav", "raw16.wav", OFN_HIDEREADONLY, "16-bit Raw Wave file (*.wav)|*.wav|", this);

		int statusFileOpen = (int) m_ldFile.DoModal();
		CString fileName = m_ldFile.GetPathName();
		if (statusFileOpen == IDOK)
		{
			FILE* inFile = fopen(m_ldFile.GetPathName(), "r");
			if (inFile == NULL)
			{
				MessageBox("Error opening", "Error");
				return;
			}
			fclose(inFile);

			int instrSel = 0x0;
			int soundChoice = 0x50;

			unsigned long samplingRate;
			if (!n64AudioLibrary.ReplaceSoundWithWavData(alBankCurrent, instrSel, soundChoice, m_ldFile.GetPathName(), samplingRate, AL_ADPCM_WAVE))
				return;

			if (alBankCurrent->samplerate != samplingRate)
			{
				CString tempStr;
				tempStr.Format("Warning sampling rate of wav %d is not equal to sampling rate of %d", samplingRate, alBankCurrent->samplerate);
				MessageBox(tempStr, "Warning");
			}

			for (int x = 0x51; x < 0x70; x++)
			{
				if (x != soundChoice)
					alBankCurrent->inst[instrSel]->sounds[x] = alBankCurrent->inst[instrSel]->sounds[soundChoice];
					
			}

			OnCbnSelchangeCombosound2();
		}
	}
}

unsigned long CN64SoundListToolDlg::CharArrayToLong(unsigned char* currentSpot)
{
	return Flip32Bit(*reinterpret_cast<unsigned long*> (currentSpot));
}

unsigned long CN64SoundListToolDlg::Flip32Bit(unsigned long inLong)
{
	return (((inLong & 0xFF000000) >> 24) | ((inLong & 0x00FF0000) >> 8) | ((inLong & 0x0000FF00) << 8) | ((inLong & 0x000000FF) << 24));
}
void CN64SoundListToolDlg::OnFileQuit()
{
	OnOK();
}

BOOL CN64SoundListToolDlg::DestroyWindow()
{
	for (int x = 0; x < numberResults; x++)
	{
		n64AudioLibrary.DisposeALBank(results[x].bank);
	}
	delete [] results;

	if (ROM != NULL)
	{
		delete [] ROM;
		ROM = NULL;
	}

	return CDialog::DestroyWindow();
}

void CN64SoundListToolDlg::OnFileOpenromexplicit()
{
	if (!OpenROMPlusDlg())
		return;


	mSoundBankIndex.ResetContent();

	try
	{
		for (int x = 0; x < numberResults; x++)
		{
			n64AudioLibrary.DisposeALBank(results[x].bank);
		}

		alBankCurrent = NULL;

		COpenSpecificCtlTbl tempDlg = new COpenSpecificCtlTbl();
		if (tempDlg.DoModal() != IDCANCEL)
		{
			unsigned long ctl = tempDlg.GetCtl();
			unsigned long tbl = tempDlg.GetTbl();

			numberResults = 0;

			results[numberResults].ctlOffset = ctl;
			results[numberResults].tblOffset = tbl;
			if (tbl > ctl)
			{
				results[numberResults].ctlSize = (results[numberResults].tblOffset - results[numberResults].ctlOffset);
				results[numberResults].tblSize = romSize - results[numberResults].tblOffset;
			}
			else
			{
				results[numberResults].ctlSize = (romSize - results[numberResults].ctlOffset);
				results[numberResults].tblSize = (results[numberResults].ctlOffset - results[numberResults].tblOffset);
			}
			results[numberResults].bank = n64AudioLibrary.ReadAudio(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);

			numberResults++;

			for (int x = 0; x < numberResults; x++)
			{
				CString tempStr;
				tempStr.Format("%02X - Ctl %08X (%08X) Tbl %08X (%08X)", x, results[x].ctlOffset, results[x].ctlSize, results[x].tblOffset, results[x].tblSize);
				mSoundBankIndex.AddString(tempStr);
			}

			mSoundBankIndex.SetCurSel(0);

			OnCbnSelchangeCombosoundbank();
		}
	}
	catch (char * str)
	{
		CString tempErrStr;
		tempErrStr.Format("%s", str);
		MessageBox(tempErrStr, "Error processing ");

		numberResults = 0;
	}
}

void CN64SoundListToolDlg::OnBnClickedButtonimportpredictors()
{
	if (alBankCurrent != NULL)
	{

		int instrumentSel = mInstrumentChoice.GetCurSel();
		int soundSel = mSoundChoice.GetCurSel();

		CFileDialog m_ldFile(TRUE, "bin", "Predictors.bin", OFN_HIDEREADONLY, "Predictors (*.bin)|*.bin|", this);

		int statusFileOpen = (int) m_ldFile.DoModal();
		CString fileName = m_ldFile.GetPathName();
		if (statusFileOpen == IDOK)
		{
			n64AudioLibrary.ImportPredictors(alBankCurrent, instrumentSel, soundSel, fileName);
		}
	}
}

void CN64SoundListToolDlg::OnBnClickedButtonexportpreditors()
{
	if (alBankCurrent != NULL)
	{

		int instrumentSel = mInstrumentChoice.GetCurSel();
		int soundSel = mSoundChoice.GetCurSel();

		CFileDialog m_svFile(FALSE, "bin", "Predictors.bin", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "PRedictors (*.bin)|*.bin|");

		int statusFileOpen = (int) m_svFile.DoModal();
		CString fileName = m_svFile.GetFileName();
		if ((statusFileOpen == IDOK) && (fileName != ""))
		{
			n64AudioLibrary.ExportPredictors(alBankCurrent, instrumentSel, soundSel, fileName);
		}
	}
}

void CN64SoundListToolDlg::OnBnClickedButtonimport16bitraw()
{
	if (alBankCurrent != NULL)
	{

		int instrumentSel = mInstrumentChoice.GetCurSel();
		int soundSel = mSoundChoice.GetCurSel();

		CFileDialog m_ldFile(TRUE, "bin", "RawSound.bin", OFN_HIDEREADONLY, "Raw Bin Sound (*.bin)|*.bin|", this);

		int statusFileOpen = (int) m_ldFile.DoModal();
		CString fileName = m_ldFile.GetPathName();
		if (statusFileOpen == IDOK)
		{
			n64AudioLibrary.ImportRawData(alBankCurrent, instrumentSel, soundSel, fileName);
		}
	}
}

void CN64SoundListToolDlg::OnBnClickedButtonexportraw()
{
	if (alBankCurrent != NULL)
	{

		int instrumentSel = mInstrumentChoice.GetCurSel();
		int soundSel = mSoundChoice.GetCurSel();

		CFileDialog m_svFile(FALSE, "bin", "RawSound.bin", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Raw Bin Sound (*.bin)|*.bin|");

		int statusFileOpen = (int) m_svFile.DoModal();
		CString fileName = m_svFile.GetFileName();

		if ((statusFileOpen == IDOK) && (fileName != ""))
		{
			n64AudioLibrary.ExportRawData(alBankCurrent, instrumentSel, soundSel, fileName);

		}
	}
}
