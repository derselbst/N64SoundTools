// N64MidiToolDlg.h : header file
//

#pragma once

#include "..\N64MidiLibrary\MidiParse.h"
#include "GECompression.h"
#include "..\..\GEDecompressor\SupermanDecoder.h"
#include "rnc_deco.h"

#include "H20Decoder.h"
#include "TetrisphereDecoder.h"

#include "afxwin.h"

struct MidiConfig
{
	unsigned long start;
	unsigned long end;
};

struct GameConfig
{
	int numberMidis;
	CString gameType;
	CString gameName;
	MidiConfig* midiBanks;
};


// CN64MidiToolDlg dialog
class CN64MidiToolDlg : public CDialog
{
// Construction
public:
	CN64MidiToolDlg(CWnd* pParent = NULL);	// standard constructor
	~CN64MidiToolDlg();
// Dialog Data
	enum { IDD = IDD_N64MIDITOOL_DIALOG };

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
private:
	CMidiParse midiParse;
	GECompression compress;
public:
	CComboBox m_game;
	CComboBox m_spot;
	CButton m_loop;
	afx_msg void OnBnClickedButtonloadrom();
private:
	unsigned long CharArrayToLong(unsigned char* currentSpot);
	unsigned long Flip32Bit(unsigned long inLong);
	int GetSizeFile(CString filename);
	unsigned char* buffer;
	unsigned short CharArrayToShort(unsigned char* currentSpot);
	unsigned short Flip16Bit(unsigned short ShortValue);
	bool compressed;
	byte* Decompress(unsigned char* Buffer, unsigned long size, int& fileSize, int& compressedSize);
	bool DecompressToFile(unsigned char* Buffer, unsigned long size, CString outputFile);
	unsigned long startSpot;
	unsigned long endSpot;
	unsigned long ReadAddiuAddress(unsigned char* GEROM, unsigned long upperLocation, unsigned long lowerLocation);
	CString gameName;
	bool CompressGZipFile(CString inputFile, CString outputFile, bool byteFlipCompressed, CString gameType);
	BOOL IsFileExist(LPSTR lpszFilename);
	BOOL hiddenExec (PTSTR pCmdLine, CString currentDirectory);
	BOOL normalExec (PTSTR pCmdLine, CString currentDirectory);
	CString mainFolder;
	void InitializeSpecificGames();
	int countGames;
public:
	afx_msg void OnCbnSelchangeCombomidi();
	afx_msg void OnBnClickedButtonexportbin();
	afx_msg void OnBnClickedButtonexportmidi();
	afx_msg void OnBnClickedButtonimportbin();
	afx_msg void OnBnClickedButtonimportmidi();
	void ConvertIntoSpot(CString inputFile);
	afx_msg void OnBnClickedButtonexportalltomidi();
	void ExportToBin(unsigned long address, unsigned long size, CString fileName);
	void ExportToMidi(unsigned char* gamebuffer, unsigned long address, unsigned long size, CString fileName, CString gameType, int& numberInstruments, unsigned long division);
	void KillWMPlayer();
	afx_msg void OnBnClickedButtonexportalltorawbin();
	void CN64MidiToolDlg::ParseUncompressedType(unsigned char* gamebuffer, unsigned long start, unsigned long endSpot, CString addMidiStrings[1000], int& numberMidiStrings, int& numberInstruments);
	afx_msg void OnBnClickedButtonwriterom();
	CString lastRomName;
	unsigned long romSize;
	afx_msg void OnBnClickedButtonplaymidi();
	afx_msg void OnBnClickedButtonimportinstruments();
	afx_msg void OnBnClickedButtonscandirformidis();
	int WorkOnFilesIntoADirectory(char* pszFiles, CString fileNames[0x1000]);
	GameConfig* gameConfig;
	afx_msg void OnBnClickedButtonriproms();
	void ProcessMidis(GameConfig* gameConfig, int gameNumber, byte* gameBuffer, CString addMidiStrings[1000], int& numberMidiStrings, int& numberInstruments);
	CEdit m_numberInstruments;
	CEdit mLoopPoint;
	CButton mNoRepeaters;
	afx_msg void OnBnClickedButton4();
};
