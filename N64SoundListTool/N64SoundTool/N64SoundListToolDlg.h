// N64SoundListToolDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "N64AIFCAudio.h"
#include "shlobj.h"
#include <string>
#include "SharedFunctions.h"

// CN64SoundListToolDlg dialog
class CN64SoundListToolDlg : public CDialog
{
// Construction
public:
	CN64SoundListToolDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_N64SOUNDLISTTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox mSoundBankIndex;
	CComboBox mInstrumentChoice;
	CEdit mSamplingRate;
	CEdit mVolume;
	CEdit mPan;
	CEdit mPriority;
	CEdit mTremType;
	CEdit mTremRate;
	CEdit mTremDepth;
	CEdit mTremDelay;
	CEdit mVibType;
	CEdit mVibRate;
	CEdit mVibDepth;
	CEdit mVibDelay;
	CComboBox mSoundChoice;
	CEdit mBase;
	CEdit mLength;
	CButton mCheckRaw;
	CEdit mSampleVol;
	CEdit mSamplePan;
	CEdit mAttackTime;
	CEdit mDecayTime;
	CEdit mReleaseTime;
	CEdit mAttackVolume;
	CEdit mDecayVolume;
	CEdit mVelMin;
	CEdit mVelMax;
	CEdit mKeyMin;
	CEdit mKeyMax;
	CEdit mKeyBase;
	CEdit mDetune;
	CEdit mLoopStart;
	CEdit mLoopEnd;
	CEdit mLoopCount;
	CButton mLoopingEnabled;
	afx_msg void OnFileOpenrom();
	bool OpenROMPlusDlg();
	CN64AIFCAudio n64AudioLibrary;
	unsigned char* ROM;
	int romSize;
	ctlTblResult* results;
	int numberResults;
	afx_msg void OnClose();
	afx_msg void OnCbnSelchangeCombosoundbank();
	bool dontupdateitall;
	ALBank* alBankCurrent;
	afx_msg void OnCbnSelchangeCombosound();
	afx_msg void OnCbnSelchangeCombosound2();
	CStatic mL1Text;
	CStatic mL2Text;
	CStatic mL3Text;
	void UpdateTextBoxes();
	afx_msg void OnEnChangeEditsamplingrate();
	afx_msg void OnEnChangeEditvol();
	afx_msg void OnEnChangeEditpan();
	afx_msg void OnEnChangeEditpriority();
	afx_msg void OnEnChangeEdittremtype();
	afx_msg void OnEnChangeEdittremrate();
	afx_msg void OnEnChangeEdittremdepth();
	afx_msg void OnEnChangeEdittremdelay();
	afx_msg void OnEnChangeEdittvibtype();
	afx_msg void OnEnChangeEditvibrate();
	afx_msg void OnEnChangeEditvibdepth();
	afx_msg void OnEnChangeEditvibdelay();
	afx_msg void OnEnChangeEditsamplepan();
	afx_msg void OnEnChangeEditsamplevol();
	afx_msg void OnEnChangeEditattacktime();
	afx_msg void OnEnChangeEditdecaytime();
	afx_msg void OnEnChangeEditreleasetime();
	afx_msg void OnEnChangeEditattackvolume();
	afx_msg void OnEnChangeEditdecayvolume();
	afx_msg void OnEnChangeEditvelmin();
	afx_msg void OnEnChangeEditvelmax();
	afx_msg void OnEnChangeEditkeymin();
	afx_msg void OnEnChangeEditkeymax();
	afx_msg void OnEnChangeEditkeybase();
	afx_msg void OnEnChangeEditdetune();
	afx_msg void OnEnChangeEditloop();
	afx_msg void OnEnChangeEditend();
	afx_msg void OnEnChangeEditcount();
	afx_msg void OnBnClickedCheckloopenabled();
	afx_msg void OnBnClickedButtonup();
	afx_msg void OnBnClickedButtondown();
	afx_msg void OnBnClickedButtonplayoriginalsound();
	CString mainFolder;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonvadpcmimport();
	afx_msg void OnBnClickedButtonaddsound();
	afx_msg void OnBnClickedButtonaddsound2();
	afx_msg void OnBnClickedButtondeletesound();
	afx_msg void OnBnClickedButtondeletesound2();
	afx_msg void OnFileSave();
	afx_msg void OnBnClickedExportctltbl();
	afx_msg void OnBnClickedInjectplace();
	BOOL hiddenExec (PTSTR pCmdLine, CString currentDirectory);
	void PlayWMPlayerSound(CString wavFileSound);
	void KillWMPlayer();
	afx_msg void OnBnClickedButtonrip();
	bool GetFolder(CString& folderpath, const char* szCaption = NULL, HWND hOwner = NULL);
	CString romName;
	afx_msg void OnBnClickedButtontest();
	unsigned long Flip32Bit(unsigned long inLong);
	unsigned long CharArrayToLong(unsigned char* currentSpot);
	afx_msg void OnFileQuit();
	virtual BOOL DestroyWindow();
	afx_msg void OnFileOpenromexplicit();
	afx_msg void OnBnClickedButtonimportpredictors();
	afx_msg void OnBnClickedButtonexportpreditors();
	afx_msg void OnBnClickedButtonimport16bitraw();
	afx_msg void OnBnClickedButtonexportraw();
	CButton m_importPredictors;
	CButton m_exportPredictors;
};
