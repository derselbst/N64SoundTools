// GEDecompressorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GEDecompressor.h"
#include "GEDecompressorDlg.h"
#include ".\gedecompressordlg.h"

#include <vector>
#include <map>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PI           3.14159265358979323846  /* pi */

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CGEDecompressorDlg dialog



CGEDecompressorDlg::CGEDecompressorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGEDecompressorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGEDecompressorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_gameselection);
	DDX_Control(pDX, IDC_GENTXT, m_genTextFiles);
	DDX_Control(pDX, IDC_LISTDECOMPRESSEDFILES, m_list);
	DDX_Control(pDX, IDC_PROGRESS, m_progressBar);
	DDX_Control(pDX, IDC_BUTTONCANCELLOAD, m_cancelLoad);
	DDX_Control(pDX, IDC_BUTTON3, m_injectButton);
	DDX_Control(pDX, IDC_BUTTONSAVEROM, m_saveROMButton);
	DDX_Control(pDX, IDC_EDITSHOWHEX, m_hexView);
	DDX_Control(pDX, IDC_EDITSEARCH, mSearch);
	DDX_Control(pDX, IDC_EDIT3, mFile);
	DDX_Control(pDX, IDC_COMPRESSFILEBUTTONENCRYPTED, mCompressEncryptedButton);
	DDX_Control(pDX, IDC_EDITENCRYPTED, mEncryptedFileNumber);
	DDX_Control(pDX, IDC_FILENUMBERLABEL, mFileNumberStatic);
}

BEGIN_MESSAGE_MAP(CGEDecompressorDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_COMPRESSFILEBUTTON, OnBnClickedCompressfilebutton)
	ON_BN_CLICKED(IDC_DECOMPRESSGAME, OnBnClickedDecompressgame)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTDECOMPRESSEDFILES, &CGEDecompressorDlg::OnLvnItemchangedListdecompressedfiles)
	ON_MESSAGE(UPDATE_LIST, AddListEntry)
	ON_MESSAGE(UPDATE_PROGRESS_BAR, UpdateProgressBar)
	ON_BN_CLICKED(IDC_BUTTONCANCELLOAD, &CGEDecompressorDlg::OnBnClickedButtoncancelload)
	ON_BN_CLICKED(IDC_BUTTONSAVEROM, &CGEDecompressorDlg::OnBnClickedButtonsaverom)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTONSEARCH, &CGEDecompressorDlg::OnBnClickedButtonsearch)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CGEDecompressorDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_COMPRESSFILEBUTTONENCRYPTED, &CGEDecompressorDlg::OnBnClickedCompressfilebuttonencrypted)
END_MESSAGE_MAP()


// CGEDecompressorDlg message handlers

int GetSizeFile(CString filename)
{
	FILE* inFile = fopen(filename, "rb");
	
	if (inFile == 0)
		return 0;

	fseek(inFile, 0, SEEK_END);
	int fileSize = ftell(inFile);
	fclose(inFile);
	return fileSize;
}

void SaveBlastCorpsBitmap(int w, int h, int bits, unsigned char data[], CString filename, int type)
{
	int Width = w;
	int Height = h;
	int Bits = bits;

	FILE * Shiznit;
	CString Path = filename;
	Shiznit = fopen(Path, "wb");

	if (Shiznit != NULL)
	{
		if (bits == 0x20)
		{
			int sized = (Width*Height)*4;

			BITMAPFILEHEADER bmphdr;
			BITMAPINFOHEADER bmpihdr;

			bmphdr.bfType = 0x4D42;	//0x00 short
			bmphdr.bfSize = ( sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sized );	//0x02 long
			bmphdr.bfOffBits = ( sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) ); // 0x0a long
			bmphdr.bfReserved1 = 0;	//0x06 short
			bmphdr.bfReserved2 = 0;	//0x08 short
			bmpihdr.biBitCount = Bits;	//0x1c
			bmpihdr.biClrImportant = 0;
			bmpihdr.biClrUsed = 0;
			bmpihdr.biCompression = BI_RGB;
			bmpihdr.biHeight = Height;	//0x16
			bmpihdr.biPlanes = 1;	//0x1a
			bmpihdr.biSize = sizeof(BITMAPINFOHEADER);
			bmpihdr.biSizeImage = sized;
			
			bmpihdr.biXPelsPerMeter = 0x0b12;	// Taken from a bitmap created in Photoshop
			bmpihdr.biYPelsPerMeter = 0x0b12;; // Taken from a bitmap created in Photoshop

			int trueWidth = Width;
			bmpihdr.biWidth = Width;	// 0x12

			fwrite((void*)&bmphdr, 1, sizeof(BITMAPFILEHEADER), Shiznit);
			fwrite((void*)&bmpihdr, 1, sizeof(BITMAPINFOHEADER), Shiznit);

			if (type == 0x1)
			{
				unsigned char* outputDecompressed = new unsigned char[(Width * Height * 4)];
				int counter = 0;
				for (int x = 0; x < (Width * Height * 2); x+=2)
				{
					unsigned short value = ((data[x] << 8) | data[x+1]);
					unsigned char red = ((value >> 11) & 0x1F) << 3;
					unsigned char green = ((value >> 6) & 0x1F) << 3;
					unsigned char blue = ((value >> 1) & 0x1F) << 3;
					unsigned char alpha = (value & 0x1) << 7;

					outputDecompressed[counter++] = blue;
					outputDecompressed[counter++] = green;
					outputDecompressed[counter++] = red;
					outputDecompressed[counter++] = alpha;
				}

				for (int x = 0; x < (Width * Height * 4); x++)
				{
					fwrite(&outputDecompressed[x], 1, 1, Shiznit);
				}
				
				delete [] outputDecompressed;
			}
			else if (type == 0x3)
			{
				for (int x = 0; x < (Width * Height * 2); x+=2)
				{
					unsigned short value = (data[x] >> 8);
					unsigned char alpha = (data[x+1] & 0xFF);

					fwrite(&value, 1, 1, Shiznit);
					fwrite(&value, 1, 1, Shiznit);
					fwrite(&value, 1, 1, Shiznit);
					fwrite(&alpha, 1, 1, Shiznit);
				}
			}
			else if (type == 0x4)
			{
				for (int x = 0; x < (Width * Height * 2); x+=2)
				{
					unsigned short value = (data[x] >> 8);
					unsigned char alpha = (data[x+1] & 0xFF);

					fwrite(&value, 1, 1, Shiznit);
					fwrite(&value, 1, 1, Shiznit);
					fwrite(&value, 1, 1, Shiznit);
					fwrite(&alpha, 1, 1, Shiznit);
				}
			}
			else if (type == 0x2)
			{
				for (int x = 0; x < (Width * Height * 4); x+=4)
				{
					fwrite(&data[x+2], 1, 1, Shiznit);
					fwrite(&data[x+1], 1, 1, Shiznit);
					fwrite(&data[x+0], 1, 1, Shiznit);
					fwrite(&data[x+3], 1, 1, Shiznit);
				}
			}
			else if (type == 0x5)
			{
				for (int x = 0; x < (Width * Height * 4); x+=4)
				{
					fwrite(&data[x+2], 1, 1, Shiznit);
					fwrite(&data[x+1], 1, 1, Shiznit);
					fwrite(&data[x+0], 1, 1, Shiznit);
					fwrite(&data[x+3], 1, 1, Shiznit);
				}
			}

			/*if ((type & 0x7) == 0x6)
			{
				// 4-bit greyscale
				for (int x = 0; x < (Width * Height)/2; x++)
				{
					unsigned char alpha = 0xFF;
					unsigned char value1 = ((data[x] >> 4) & 0xF) << 4;
					unsigned char value2 = (data[x] & 0xF) << 4;
					fwrite(&value1, 1, 1, Shiznit);
					fwrite(&value1, 1, 1, Shiznit);
					fwrite(&value1, 1, 1, Shiznit);
					fwrite(&alpha, 1, 1, Shiznit);

					fwrite(&value2, 1, 1, Shiznit);
					fwrite(&value2, 1, 1, Shiznit);
					fwrite(&value2, 1, 1, Shiznit);
					fwrite(&alpha, 1, 1, Shiznit);
				}
			}
			else if ((type & 0x7) == 0x5)
			{
				// 4-bit alpha/4-bit value
				for (int x = 0; x < (Width * Height); x++)
				{
					unsigned char alpha = (data[x] & 0xF) << 4;
					unsigned char value = ((data[x] >> 4) & 0xF) << 4;

					unsigned char oldValue = value;
					value = alpha;
					alpha = oldValue;

					fwrite(&value, 1, 1, Shiznit);
					fwrite(&value, 1, 1, Shiznit);
					fwrite(&value, 1, 1, Shiznit);
					fwrite(&alpha, 1, 1, Shiznit);
				}
			}
			else if ((type & 0x7) == 0x4)
			{
				// 8-bit alpha/8-bit greyscale
				for (int x = 0; x < (Width * Height)*2; x+=2)
				{
					unsigned char alpha = data[x];
					unsigned char value = data[x+1];
					
					fwrite(&value, 1, 1, Shiznit);
					fwrite(&value, 1, 1, Shiznit);
					fwrite(&value, 1, 1, Shiznit);
					fwrite(&alpha, 1, 1, Shiznit);
				}
			}
			else if ((type & 0x7) == 0x3)
			{
				// 4-bit greyscale
				for (int x = 0; x < (Width * Height)/2; x++)
				{
					unsigned char alpha = 0xFF;
					unsigned char value1 = ((data[x] >> 4) & 0xF) << 4;
					unsigned char value2 = (data[x] & 0xF) << 4;
					fwrite(&value1, 1, 1, Shiznit);
					fwrite(&value1, 1, 1, Shiznit);
					fwrite(&value1, 1, 1, Shiznit);
					fwrite(&alpha, 1, 1, Shiznit);

					fwrite(&value2, 1, 1, Shiznit);
					fwrite(&value2, 1, 1, Shiznit);
					fwrite(&value2, 1, 1, Shiznit);
					fwrite(&alpha, 1, 1, Shiznit);
				}
			}
			else if ((type & 0x7) == 0x2)
			{
				// 8-bit greyscale
				for (int x = 0; x < (Width * Height); x++)
				{
					unsigned char alpha = 0xFF;
					fwrite(&data[x], 1, 1, Shiznit);
					fwrite(&data[x], 1, 1, Shiznit);
					fwrite(&data[x], 1, 1, Shiznit);
					fwrite(&alpha, 1, 1, Shiznit);
				}
			}
			else if ((type & 0x7) == 0x1)
			{
				// ARRRRRGGGGGBBBBBB
				for (int x = 0; x < (Width * Height)*2; x+=2)
				{
					unsigned short value = ((data[x] << 8) | data[x+1]);
					unsigned char red = ((value >> 11) & 0x1F) << 3;
					unsigned char green = ((value >> 6) & 0x1F) << 3;
					unsigned char blue = ((value >> 1) & 0x1F) << 3;

					unsigned char alpha = 0xFF * (value & 0x1);
					
					fwrite(&blue, 1, 1, Shiznit);
					fwrite(&green, 1, 1, Shiznit);
					fwrite(&red, 1, 1, Shiznit);
					fwrite(&alpha, 1, 1, Shiznit);

				}
			}
			else
			{
				// ABGR 32-bit
				for (int x = 0; x < (Width * Height)*4; x+=4)
				{
					fwrite(&data[x+2], 1, 1, Shiznit);
					fwrite(&data[x+1], 1, 1, Shiznit);
					fwrite(&data[x+0], 1, 1, Shiznit);
					fwrite(&data[x+3], 1, 1, Shiznit);
				}
			}*/
			fclose(Shiznit);
		}
	}
}

void SaveJFGBitmap(int w, int h, int bits, unsigned char data[], CString filename, int type)
{
	int Width = w;
	int Height = h;
	int Bits = bits;

	FILE * Shiznit;
	CString Path = filename;
	Shiznit = fopen(Path, "wb");

	if (Shiznit != NULL)
	{
		if (bits == 0x20)
		{
			int sized = (Width*Height)*4;

			BITMAPFILEHEADER bmphdr;
			BITMAPINFOHEADER bmpihdr;

			bmphdr.bfType = 0x4D42;	//0x00 short
			bmphdr.bfSize = ( sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sized );	//0x02 long
			bmphdr.bfOffBits = ( sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) ); // 0x0a long
			bmphdr.bfReserved1 = 0;	//0x06 short
			bmphdr.bfReserved2 = 0;	//0x08 short
			bmpihdr.biBitCount = Bits;	//0x1c
			bmpihdr.biClrImportant = 0;
			bmpihdr.biClrUsed = 0;
			bmpihdr.biCompression = BI_RGB;
			bmpihdr.biHeight = Height;	//0x16
			bmpihdr.biPlanes = 1;	//0x1a
			bmpihdr.biSize = sizeof(BITMAPINFOHEADER);
			bmpihdr.biSizeImage = sized;
			
			bmpihdr.biXPelsPerMeter = 0x0b12;	// Taken from a bitmap created in Photoshop
			bmpihdr.biYPelsPerMeter = 0x0b12;; // Taken from a bitmap created in Photoshop

			int trueWidth = Width;
			bmpihdr.biWidth = Width;	// 0x12

			fwrite((void*)&bmphdr, 1, sizeof(BITMAPFILEHEADER), Shiznit);
			fwrite((void*)&bmpihdr, 1, sizeof(BITMAPINFOHEADER), Shiznit);

			if ((type & 0x7) == 0x6)
			{
				// 4-bit greyscale
				for (int x = 0; x < (Width * Height)/2; x++)
				{
					unsigned char alpha = 0xFF;
					unsigned char value1 = ((data[x] >> 4) & 0xF) << 4;
					unsigned char value2 = (data[x] & 0xF) << 4;
					fwrite(&value1, 1, 1, Shiznit);
					fwrite(&value1, 1, 1, Shiznit);
					fwrite(&value1, 1, 1, Shiznit);
					fwrite(&alpha, 1, 1, Shiznit);

					fwrite(&value2, 1, 1, Shiznit);
					fwrite(&value2, 1, 1, Shiznit);
					fwrite(&value2, 1, 1, Shiznit);
					fwrite(&alpha, 1, 1, Shiznit);
				}
			}
			else if ((type & 0x7) == 0x5)
			{
				// 4-bit alpha/4-bit value
				for (int x = 0; x < (Width * Height); x++)
				{
					unsigned char alpha = (data[x] & 0xF) << 4;
					unsigned char value = ((data[x] >> 4) & 0xF) << 4;

					unsigned char oldValue = value;
					value = alpha;
					alpha = oldValue;

					fwrite(&value, 1, 1, Shiznit);
					fwrite(&value, 1, 1, Shiznit);
					fwrite(&value, 1, 1, Shiznit);
					fwrite(&alpha, 1, 1, Shiznit);
				}
			}
			else if ((type & 0x7) == 0x4)
			{
				// 8-bit alpha/8-bit greyscale
				for (int x = 0; x < (Width * Height)*2; x+=2)
				{
					unsigned char alpha = data[x];
					unsigned char value = data[x+1];
					
					fwrite(&value, 1, 1, Shiznit);
					fwrite(&value, 1, 1, Shiznit);
					fwrite(&value, 1, 1, Shiznit);
					fwrite(&alpha, 1, 1, Shiznit);
				}
			}
			else if ((type & 0x7) == 0x3)
			{
				// 4-bit greyscale
				for (int x = 0; x < (Width * Height)/2; x++)
				{
					unsigned char alpha = 0xFF;
					unsigned char value1 = ((data[x] >> 4) & 0xF) << 4;
					unsigned char value2 = (data[x] & 0xF) << 4;
					fwrite(&value1, 1, 1, Shiznit);
					fwrite(&value1, 1, 1, Shiznit);
					fwrite(&value1, 1, 1, Shiznit);
					fwrite(&alpha, 1, 1, Shiznit);

					fwrite(&value2, 1, 1, Shiznit);
					fwrite(&value2, 1, 1, Shiznit);
					fwrite(&value2, 1, 1, Shiznit);
					fwrite(&alpha, 1, 1, Shiznit);
				}
			}
			else if ((type & 0x7) == 0x2)
			{
				// 8-bit greyscale
				for (int x = 0; x < (Width * Height); x++)
				{
					unsigned char alpha = 0xFF;
					fwrite(&data[x], 1, 1, Shiznit);
					fwrite(&data[x], 1, 1, Shiznit);
					fwrite(&data[x], 1, 1, Shiznit);
					fwrite(&alpha, 1, 1, Shiznit);
				}
			}
			else if ((type & 0x7) == 0x1)
			{
				// ARRRRRGGGGGBBBBBB
				for (int x = 0; x < (Width * Height)*2; x+=2)
				{
					unsigned short value = ((data[x] << 8) | data[x+1]);
					unsigned char red = ((value >> 11) & 0x1F) << 3;
					unsigned char green = ((value >> 6) & 0x1F) << 3;
					unsigned char blue = ((value >> 1) & 0x1F) << 3;

					unsigned char alpha = 0xFF * (value & 0x1);
					
					fwrite(&blue, 1, 1, Shiznit);
					fwrite(&green, 1, 1, Shiznit);
					fwrite(&red, 1, 1, Shiznit);
					fwrite(&alpha, 1, 1, Shiznit);

				}
			}
			else
			{
				// ABGR 32-bit
				for (int x = 0; x < (Width * Height)*4; x+=4)
				{
					fwrite(&data[x+2], 1, 1, Shiznit);
					fwrite(&data[x+1], 1, 1, Shiznit);
					fwrite(&data[x+0], 1, 1, Shiznit);
					fwrite(&data[x+3], 1, 1, Shiznit);
				}
			}
			fclose(Shiznit);
		}
	}
}

bool JFGImageToBitmap(unsigned char* outputDecompressed, CString outputFilename, bool doSwap, bool isDkr, bool isMickey)
{
	int compressionedBufferOffset = 0x20;

	unsigned char* Buffer = outputDecompressed;

	int width = Buffer[0];
	int height = Buffer[1];

	if (isMickey)
	{
		width = Buffer[7];
		height = Buffer[9];
	}

	unsigned char type = Buffer[2];

	if (doSwap)
	{
		if ((type & 0x7) == 0x6)
		{
			for (int x = 1; x < height; x+=2)
			{
				unsigned char values[0x10];
				for (int y = 0; y < (width/2); y+=8)
				{
					for (int z = 0; z < 8; z++)
					{
						values[z*2] = (outputDecompressed[compressionedBufferOffset + x*(width/2) + y + z] & 0xF0);
						values[z*2+1] = (outputDecompressed[compressionedBufferOffset + x*(width/2) + y + z] & 0x0F) << 4;
					}

					outputDecompressed[compressionedBufferOffset + x*(width/2) + y + 0] = ((values[8] & 0xF0) | ((values[9]>>4) & 0xF));
					outputDecompressed[compressionedBufferOffset + x*(width/2) + y + 1] = ((values[10] & 0xF0) | ((values[11]>>4) & 0xF));
					outputDecompressed[compressionedBufferOffset + x*(width/2) + y + 2] = ((values[12] & 0xF0) | ((values[13]>>4) & 0xF));
					outputDecompressed[compressionedBufferOffset + x*(width/2) + y + 3] = ((values[14] & 0xF0) | ((values[15]>>4) & 0xF));
					outputDecompressed[compressionedBufferOffset + x*(width/2) + y + 4] = ((values[0] & 0xF0) | ((values[1]>>4) & 0xF));
					outputDecompressed[compressionedBufferOffset + x*(width/2) + y + 5] = ((values[2] & 0xF0) | ((values[3]>>4) & 0xF));
					outputDecompressed[compressionedBufferOffset + x*(width/2) + y + 6] = ((values[4] & 0xF0) | ((values[5]>>4) & 0xF));
					outputDecompressed[compressionedBufferOffset + x*(width/2) + y + 7] = ((values[6] & 0xF0) | ((values[7]>>4) & 0xF));
				}
			}
		}
		else if ((type & 0x7) == 0x5)
		{
			for (int x = 1; x < height; x+=2)
			{
				unsigned char values[8];
				for (int y = 0; y < width; y+=8)
				{
					for (int z = 0; z < 8; z++)
					{
						values[z] = outputDecompressed[compressionedBufferOffset + x*width + y + z];
					}

					outputDecompressed[compressionedBufferOffset + x*width + y + 0] = ((values[4]) & 0xFF);
					outputDecompressed[compressionedBufferOffset + x*width + y + 1] = ((values[5]) & 0xFF);
					outputDecompressed[compressionedBufferOffset + x*width + y + 2] = ((values[6]) & 0xFF);
					outputDecompressed[compressionedBufferOffset + x*width + y + 3] = ((values[7]) & 0xFF);
					outputDecompressed[compressionedBufferOffset + x*width + y + 4] = ((values[0]) & 0xFF);
					outputDecompressed[compressionedBufferOffset + x*width + y + 5] = ((values[1]) & 0xFF);
					outputDecompressed[compressionedBufferOffset + x*width + y + 6] = ((values[2]) & 0xFF);
					outputDecompressed[compressionedBufferOffset + x*width + y + 7] = ((values[3]) & 0xFF);
				}
			}
		}
		else if ((type & 0x7) == 0x4)
		{
			for (int x = 1; x < height; x+=2)
			{
				unsigned short values[4];
				for (int y = 0; y < width*2; y+=8)
				{
					for (int z = 0; z < 4; z++)
					{
						values[z] = ((outputDecompressed[compressionedBufferOffset + x*width*2 + y + z*2] << 8) | outputDecompressed[compressionedBufferOffset + x*width*2 + y + z*2 + 1]);
					}

					outputDecompressed[compressionedBufferOffset + x*width*2 + y + 0*2] = ((values[2] >> 8) & 0xFF);
					outputDecompressed[compressionedBufferOffset + x*width*2 + y + 0*2 + 1] = ((values[2]) & 0xFF);

					outputDecompressed[compressionedBufferOffset + x*width*2 + y + 1*2] = ((values[3] >> 8) & 0xFF);
					outputDecompressed[compressionedBufferOffset + x*width*2 + y + 1*2 + 1] = ((values[3]) & 0xFF);

					outputDecompressed[compressionedBufferOffset + x*width*2 + y + 2*2] = ((values[0] >> 8) & 0xFF);
					outputDecompressed[compressionedBufferOffset + x*width*2 + y + 2*2 + 1] = ((values[0]) & 0xFF);

					outputDecompressed[compressionedBufferOffset + x*width*2 + y + 3*2] = ((values[1] >> 8) & 0xFF);
					outputDecompressed[compressionedBufferOffset + x*width*2 + y + 3*2 + 1] = ((values[1]) & 0xFF);
				}
			}
		}
		else if ((type & 0x7) == 0x3)
		{
			for (int x = 1; x < height; x+=2)
			{
				unsigned char values[0x10];
				for (int y = 0; y < (width/2); y+=8)
				{
					for (int z = 0; z < 8; z++)
					{
						values[z*2] = (outputDecompressed[compressionedBufferOffset + x*(width/2) + y + z] & 0xF0);
						values[z*2+1] = (outputDecompressed[compressionedBufferOffset + x*(width/2) + y + z] & 0x0F) << 4;
					}

					outputDecompressed[compressionedBufferOffset + x*(width/2) + y + 0] = ((values[8] & 0xF0) | ((values[9]>>4) & 0xF));
					outputDecompressed[compressionedBufferOffset + x*(width/2) + y + 1] = ((values[10] & 0xF0) | ((values[11]>>4) & 0xF));
					outputDecompressed[compressionedBufferOffset + x*(width/2) + y + 2] = ((values[12] & 0xF0) | ((values[13]>>4) & 0xF));
					outputDecompressed[compressionedBufferOffset + x*(width/2) + y + 3] = ((values[14] & 0xF0) | ((values[15]>>4) & 0xF));
					outputDecompressed[compressionedBufferOffset + x*(width/2) + y + 4] = ((values[0] & 0xF0) | ((values[1]>>4) & 0xF));
					outputDecompressed[compressionedBufferOffset + x*(width/2) + y + 5] = ((values[2] & 0xF0) | ((values[3]>>4) & 0xF));
					outputDecompressed[compressionedBufferOffset + x*(width/2) + y + 6] = ((values[4] & 0xF0) | ((values[5]>>4) & 0xF));
					outputDecompressed[compressionedBufferOffset + x*(width/2) + y + 7] = ((values[6] & 0xF0) | ((values[7]>>4) & 0xF));
				}
			}
		}
		else if ((type & 0x7) == 0x2)
		{
			for (int x = 1; x < height; x+=2)
			{
				unsigned char values[8];
				for (int y = 0; y < width; y+=8)
				{
					for (int z = 0; z < 8; z++)
					{
						values[z] = outputDecompressed[compressionedBufferOffset + x*width + y + z];
					}

					outputDecompressed[compressionedBufferOffset + x*width + y + 0] = ((values[4]) & 0xFF);
					outputDecompressed[compressionedBufferOffset + x*width + y + 1] = ((values[5]) & 0xFF);
					outputDecompressed[compressionedBufferOffset + x*width + y + 2] = ((values[6]) & 0xFF);
					outputDecompressed[compressionedBufferOffset + x*width + y + 3] = ((values[7]) & 0xFF);
					outputDecompressed[compressionedBufferOffset + x*width + y + 4] = ((values[0]) & 0xFF);
					outputDecompressed[compressionedBufferOffset + x*width + y + 5] = ((values[1]) & 0xFF);
					outputDecompressed[compressionedBufferOffset + x*width + y + 6] = ((values[2]) & 0xFF);
					outputDecompressed[compressionedBufferOffset + x*width + y + 7] = ((values[3]) & 0xFF);
				}
			}
		}
		else if ((type & 0x7) == 0x1)
		{
			for (int x = 1; x < height; x+=2)
			{
				unsigned short values[4];
				for (int y = 0; y < width*2; y+=8)
				{
					for (int z = 0; z < 4; z++)
					{
						values[z] = ((outputDecompressed[compressionedBufferOffset + x*width*2 + y + z*2] << 8) | outputDecompressed[compressionedBufferOffset + x*width*2 + y + z*2 + 1]);
					}

					outputDecompressed[compressionedBufferOffset + x*width*2 + y + 0*2] = ((values[2] >> 8) & 0xFF);
					outputDecompressed[compressionedBufferOffset + x*width*2 + y + 0*2 + 1] = ((values[2]) & 0xFF);

					outputDecompressed[compressionedBufferOffset + x*width*2 + y + 1*2] = ((values[3] >> 8) & 0xFF);
					outputDecompressed[compressionedBufferOffset + x*width*2 + y + 1*2 + 1] = ((values[3]) & 0xFF);

					outputDecompressed[compressionedBufferOffset + x*width*2 + y + 2*2] = ((values[0] >> 8) & 0xFF);
					outputDecompressed[compressionedBufferOffset + x*width*2 + y + 2*2 + 1] = ((values[0]) & 0xFF);

					outputDecompressed[compressionedBufferOffset + x*width*2 + y + 3*2] = ((values[1] >> 8) & 0xFF);
					outputDecompressed[compressionedBufferOffset + x*width*2 + y + 3*2 + 1] = ((values[1]) & 0xFF);
				}
			}
		}
		else // type & 0x7 == 0
		{
			for (int x = 1; x < height; x+=2)
			{
				unsigned long values[4];
				for (int y = 0; y < width*4; y+=16)
				{
					for (int z = 0; z < 4; z++)
					{
						values[z] = ((((((outputDecompressed[compressionedBufferOffset + x*width*4 + y + z*4] << 8) | outputDecompressed[compressionedBufferOffset + x*width*4 + y + z*4 + 1]) << 8) | outputDecompressed[compressionedBufferOffset + x*width*4 + y + z*4 + 2]) << 8) | outputDecompressed[compressionedBufferOffset + x*width*4 + y + z*4 + 3]);
					}

					outputDecompressed[compressionedBufferOffset + x*width*4 + y + 0*4] = ((values[2] >> 24) & 0xFF);
					outputDecompressed[compressionedBufferOffset + x*width*4 + y + 0*4 + 1] = ((values[2] >> 16) & 0xFF);
					outputDecompressed[compressionedBufferOffset + x*width*4 + y + 0*4 + 2] = ((values[2] >> 8) & 0xFF);
					outputDecompressed[compressionedBufferOffset + x*width*4 + y + 0*4 + 3] = ((values[2] >> 0) & 0xFF);

					outputDecompressed[compressionedBufferOffset + x*width*4 + y + 1*4] = ((values[3] >> 24) & 0xFF);
					outputDecompressed[compressionedBufferOffset + x*width*4 + y + 1*4 + 1] = ((values[3] >> 16) & 0xFF);
					outputDecompressed[compressionedBufferOffset + x*width*4 + y + 1*4 + 2] = ((values[3] >> 8) & 0xFF);
					outputDecompressed[compressionedBufferOffset + x*width*4 + y + 1*4 + 3] = ((values[3] >> 0) & 0xFF);

					outputDecompressed[compressionedBufferOffset + x*width*4 + y + 2*4] = ((values[0] >> 24) & 0xFF);
					outputDecompressed[compressionedBufferOffset + x*width*4 + y + 2*4 + 1] = ((values[0] >> 16) & 0xFF);
					outputDecompressed[compressionedBufferOffset + x*width*4 + y + 2*4 + 2] = ((values[0] >> 8) & 0xFF);
					outputDecompressed[compressionedBufferOffset + x*width*4 + y + 2*4 + 3] = ((values[0] >> 0) & 0xFF);

					outputDecompressed[compressionedBufferOffset + x*width*4 + y + 3*4] = ((values[1] >> 24) & 0xFF);
					outputDecompressed[compressionedBufferOffset + x*width*4 + y + 3*4 + 1] = ((values[1] >> 16) & 0xFF);
					outputDecompressed[compressionedBufferOffset + x*width*4 + y + 3*4 + 2] = ((values[1] >> 8) & 0xFF);
					outputDecompressed[compressionedBufferOffset + x*width*4 + y + 3*4 + 3] = ((values[1] >> 0) & 0xFF);
				}
			}
		}
	}

	SaveJFGBitmap(width, height, 0x20, &outputDecompressed[0x20], outputFilename, type);

	delete [] outputDecompressed;

	return true;
}

struct FoundDLImageData
{
	bool found[7];
	int widths[7];
	int heights[7];
	int uls[7];
	int ult[7];
	int lrs[7];
	int lrt[7];
	CString files[7];
	unsigned long addresses[7];

	FoundDLImageData()
	{
		for (int x = 0; x < 7; x++)
			found[x] = false;
	}
};

BOOL CGEDecompressorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	fileCount = 0;
	killThread = false;
	lastSearchSpot = -1;

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	char tempFolder[8000];
	::GetCurrentDirectory(8000, tempFolder);
	directory.Format("%s\\", tempFolder);

	decompressGamethread = NULL;

	ROM = NULL;
	romName = "";
	ROMSize = 0;
	m_gameselection.SetCurSel(2);




	return TRUE;  // return TRUE  unless you set the focus to a control
}


CGEDecompressorDlg::~CGEDecompressorDlg(void)
{
	
}

LRESULT CGEDecompressorDlg::AddListEntry(WPARAM wp, LPARAM lp)
{    	
	ListUpdateStruct* listUpdate = ((ListUpdateStruct*)lp);
	CString addressStr;
	addressStr.Format("%08X", listUpdate->address);
    LVITEM lv;
    lv.iItem = fileCount;
    lv.iSubItem = 0;
	lv.pszText = (LPSTR) addressStr.GetBuffer();
    lv.mask = LVIF_TEXT;
    m_list.InsertItem(&lv);
	m_list.SetItemData(fileCount, (DWORD_PTR)listUpdate);
	
	CString fileSizeUncompressedStr;
	fileSizeUncompressedStr.Format("%06X", listUpdate->fileSizeUncompressed);

    LVITEM lv2;
    lv2.iItem = fileCount;
    lv2.iSubItem = 1;
    lv2.pszText = (LPSTR) fileSizeUncompressedStr.GetBuffer();
    lv2.mask = LVIF_TEXT;
	m_list.SetItem(&lv2);   

	CString fileSizeCompressedStr;
	fileSizeCompressedStr.Format("%06X", listUpdate->fileSizeCompressed);

	LVITEM lv3;
    lv3.iItem = fileCount;
    lv3.iSubItem = 2;
    lv3.pszText = (LPSTR) fileSizeCompressedStr.GetBuffer();
    lv3.mask = LVIF_TEXT;
	m_list.SetItem(&lv3);

	LVITEM lv4;
    lv4.iItem = fileCount;
    lv4.iSubItem = 3;
	lv4.pszText = (LPSTR) listUpdate->type.GetBuffer();
    lv4.mask = LVIF_TEXT;
	m_list.SetItem(&lv4);

	LVITEM lv5;
    lv5.iItem = fileCount;
    lv5.iSubItem = 4;
    lv5.pszText = (LPSTR) listUpdate->tempLocation.GetBuffer();
    lv5.mask = LVIF_TEXT;
	m_list.SetItem(&lv5);

	LVITEM lv6;
    lv6.iItem = fileCount;
    lv6.iSubItem = 5;
    lv6.pszText = (LPSTR) listUpdate->internalName.GetBuffer();
    lv6.mask = LVIF_TEXT;
	m_list.SetItem(&lv6);

	fileCount++;

	return 0;
}

LRESULT CGEDecompressorDlg::UpdateProgressBar(WPARAM wp, LPARAM lp)
{    	
	int progress = (int)lp;
	m_progressBar.SetPos(progress);

	if (progress == 100)
	{
		m_progressBar.ShowWindow(SW_HIDE);
		m_cancelLoad.ShowWindow(SW_HIDE);
	}
	return 0;
}

void CGEDecompressorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGEDecompressorDlg::OnPaint() 
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
HCURSOR CGEDecompressorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//Tables type 1
unsigned short bt1Table1;

//Tables type 1 and 2


void CGEDecompressorDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here

	
	CString fileOpen;
	CFileDialog m_ldFile(TRUE, "bin", "uploadfile.bin", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Bin File of Codes (*.bin)|*.bin|");
	int didRead = m_ldFile.DoModal();
	if ((didRead == IDCANCEL) || (m_ldFile.GetPathName() == ""))
		return;

	if (didRead == FALSE)
		return;

	CFileDialog m_svFile(FALSE, "bin", (m_ldFile.GetFileName() + ".bin"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Bin File of Codes (*.*)|*.*|");
	didRead = m_svFile.DoModal();
	
	if ((didRead == IDCANCEL) || (m_ldFile.GetPathName() == ""))
		return;

	if (didRead == FALSE)
		return;

	CString gameNameStr;
	m_gameselection.GetWindowText(gameNameStr);
	
	int size = GetSizeFile(m_ldFile.GetPathName());
	if (size > 0)
	{
		FILE* inFile = fopen(m_ldFile.GetPathName(),"rb");
		if (inFile != NULL)
		{
			unsigned char* Buffer;
			Buffer = new unsigned char[size];
			fread(Buffer, 1, size, inFile);
			

			unsigned char* outputDecompressed = NULL;
			int fileSize = 0;
			try
			{
				if (GetZLibGameName(gameNameStr) != -1)
				{
					GECompression compressed;
					compressed.SetPath(directory);
					compressed.SetGame(GetZLibGameName(gameNameStr));
					
					compressed.SetCompressedBuffer(Buffer, size);
					int compressedSize = 0;
					unsigned char* outputDecompressed = compressed.OutputDecompressedBuffer(fileSize, compressedSize);
				}
				else if ((gameNameStr == "Turok 1") || (gameNameStr == "Duke Nukem 64") || (gameNameStr == "Turok 2") || (gameNameStr == "Turok 3") || (gameNameStr == "Turok Rage Wars") || (gameNameStr == "Iggy's Reckin' Balls") || (gameNameStr == "Shadowman")  || (gameNameStr == "NHL Breakaway 98") || (gameNameStr == "NHL Breakaway 99") || (gameNameStr == "PGA European Tour"))
				{
					outputDecompressed = new unsigned char[0x100000];
					int compressedSize = -1;
					RncDecoder rnc;
					fileSize = rnc.unpackM1(Buffer, outputDecompressed, 0x0000, compressedSize);
				}
				else if (gameNameStr == "Forsaken")
				{
					outputDecompressed = new unsigned char[0x100000];
					int compressedSize = -1;
					RncDecoder rnc;
					fileSize = rnc.unpackM1(Buffer, outputDecompressed, 0x0000, compressedSize);
				}
				else if ((gameNameStr == "ECW Hardcore Revolution") || (gameNameStr == "NFL QB Club 2000") || (gameNameStr == "NFL QB Club 2001") || (gameNameStr == "NFL QB Club 98") || (gameNameStr == "NFL QB Club 99") || (gameNameStr == "WWF WarZone") || (gameNameStr == "WWF Attitude") || (gameNameStr == "All-Star Baseball 99") || (gameNameStr == "South Park Chefs Love Shack"))
				{
					outputDecompressed = new unsigned char[0x100000];
					// Also has RNC81 type and RNC01
					int compressedSize = -1;
					RncDecoder rnc;
					fileSize = rnc.unpackM1(Buffer, outputDecompressed, 0x0000, compressedSize);
				}
				else if (gameNameStr == "Command and Conquer")
				{
					outputDecompressed = new unsigned char[0x100000];
					int compressedSize = -1;
					CommandAndConquerDecoder commandConquerDec;
					fileSize = commandConquerDec.dec(Buffer, outputDecompressed, compressedSize, 2);
				}
				else if ((gameNameStr == "Armorines"))
				{
					outputDecompressed = new unsigned char[0x100000];
					//RNC2
					int compressedSize = -1;
					RncDecoder rnc;
					fileSize = rnc.unpackM1(Buffer, outputDecompressed, 0x0000, compressedSize);
				}
				else if ((gameNameStr == "South Park"))
				{
					outputDecompressed = new unsigned char[0x100000];
					//RNC1/2
					int compressedSize = -1;
					RncDecoder rnc;
					fileSize = rnc.unpackM1(Buffer, outputDecompressed, 0x0000, compressedSize);
				}
				else if ((gameNameStr == "AeroFighter") || (gameNameStr == "Beetle Adventure Racing") || (gameNameStr == "Body Harvest") || (gameNameStr == "Duck Dodgers") || (gameNameStr == "F-1 World Grand Prix")|| (gameNameStr == "F-1 World Grand Prix II (E)")|| (gameNameStr == "F-ZERO Expansion Kit 64DD")|| (gameNameStr == "F-ZERO X")|| (gameNameStr == "Indy Racing 2000")|| (gameNameStr == "Lt. Duck Dodgers (Prototype)")|| (gameNameStr == "MarioKart64")|| (gameNameStr == "Pilotwings 64")|| (gameNameStr == "StarFox64")|| (gameNameStr == "Super Mario 64")|| (gameNameStr == "WaveRace"))
				{
					outputDecompressed = new unsigned char[0x100000];
					int compressedSize = -1;
					MIO0 mio0;
					fileSize = mio0.mio0_decode(Buffer, outputDecompressed, compressedSize);
				}
				else if ((gameNameStr == "Starcraft") || (gameNameStr == "Bassmasters 2000") || (gameNameStr == "Ms PacMan") || (gameNameStr == "Power Rangers"))
				{
					outputDecompressed = new unsigned char[0x100000];
					int compressedSize = -1;
					CBoltDecoder bolt;
					fileSize = bolt.dec(Buffer, outputDecompressed, compressedSize, 0);
				}
				else if (gameNameStr == "Superman")
				{
					outputDecompressed = new unsigned char[0x100000];
					int compressedSize = -1;
					SupermanDecoder superman;
					int expectedSize = superman.header(Buffer, compressedSize);
					fileSize = superman.dec(&Buffer[0x11], compressedSize, expectedSize, outputDecompressed);
				}
				else if (gameNameStr == "Monaco Grand Prix")
				{
					outputDecompressed = new unsigned char[0x100000];
					int compressedSize = -1;
					MonacoDecoder monaco;
					int expectedSize = monaco.header(Buffer, compressedSize);
					fileSize = monaco.dec(&Buffer[0x8], compressedSize, expectedSize, outputDecompressed);
				}
				else if ((gameNameStr == "1080Snow") || (gameNameStr == "BakuBomberman2")|| (gameNameStr == "Battlezone Black Dogs")|| (gameNameStr == "Bomberman 64 Second Attack")|| (gameNameStr == "Disney's Donald Duck - Goin' Quackers")|| (gameNameStr == "Doraemon 2")|| (gameNameStr == "Fushigi no Dungeon")|| (gameNameStr == "Kira to Kaiketsu")|| (gameNameStr == "Neon Genesis Evangelion")|| (gameNameStr == "Nushi Tsuri 64")|| (gameNameStr == "Paper Mario")|| (gameNameStr == "Parlor Pro 64")|| (gameNameStr == "Pokemon Stadium")|| (gameNameStr == "Pokemon Stadium 2")|| (gameNameStr == "Rayman 2")|| (gameNameStr == "Scooby-Doo")|| (gameNameStr == "Shadowgate 64")|| (gameNameStr == "Tonic Trouble")|| (gameNameStr == "Tsumi to Batsu")|| (gameNameStr == "Zool"))
				{
					outputDecompressed = new unsigned char[0x100000];
					int compressedSize = -1;
					YAY0 yay0;
					fileSize = yay0.decodeAll(Buffer, outputDecompressed, compressedSize);
				}
				else if (gameNameStr == "SSB")
				{
					outputDecompressed = new unsigned char[0x100000];
					int compressedSize = size;
					CVPK0Decoder vpk;
					fileSize = vpk.dec(&Buffer[8], compressedSize, CharArrayToLong(&Buffer[4]), outputDecompressed);
				}
				else if (gameNameStr == "Pokemon Snap")
				{
					outputDecompressed = new unsigned char[0x100000];
					int compressedSize = size;
					CVPK0Decoder vpk;
					fileSize = vpk.dec(&Buffer[8], compressedSize, CharArrayToLong(&Buffer[4]), outputDecompressed);
				}
				else if (gameNameStr == "Shigesato Itoi Bass Fishing")
				{
					outputDecompressed = new unsigned char[0x100000];
					int compressedSize = size;
					CVPK0Decoder vpk;
					fileSize = vpk.dec(&Buffer[8], compressedSize, CharArrayToLong(&Buffer[4]), outputDecompressed);
				}
				else if ((gameNameStr == "Tony Hawk Pro Skater 1") || (gameNameStr == "Tony Hawk Pro Skater 2") || (gameNameStr == "Tony Hawk Pro Skater 3"))
				{
					outputDecompressed = new unsigned char[0x100000];
					int compressedSize = -1;
					ERZ erz;
					fileSize = erz.decode(Buffer, size, outputDecompressed, compressedSize);
				}
				else if ((gameNameStr == "Monster Truck Madness") || (gameNameStr == "Spiderman"))
				{
					outputDecompressed = new unsigned char[0x100000];
					int compressedSize = -1;
					ERZ erz;
					fileSize = erz.decode(Buffer, size, outputDecompressed, compressedSize);
				}
				else if ((gameNameStr == "Doubutsu no Mori") || (gameNameStr == "MajorasMask") || (gameNameStr == "OoT"))
				{
					outputDecompressed = new unsigned char[0x100000];
					int compressedSize = -1;
					YAZ0 yaz0;
					fileSize = yaz0.yaz0_decode(Buffer, outputDecompressed, compressedSize);
				}
				else if ((gameNameStr == "40 Winks")|| (gameNameStr == "Cruis'n World")|| (gameNameStr == "Duke Nukem - ZER0 H0UR")|| (gameNameStr == "Hydro Thunder")|| (gameNameStr == "Mortal Kombat 4")|| (gameNameStr == "NBA Showtime")|| (gameNameStr == "Tarzan")|| (gameNameStr == "TWINE")|| (gameNameStr == "War Gods"))
				{
					// written directly
					int compressedSize = -1;
					CString type = "";
					CEDLCompress edlcompress;
					edlcompress.decode(inFile, 0, m_svFile.GetPathName(), compressedSize, type);
				}
				else if ((gameNameStr == "Custom Robo") || (gameNameStr == "Custom Robo V2"))
				{
					// written directly
					int compressedSize = -1;
					CCustomRoboCompress customrobocompress;
					customrobocompress.decode(inFile, 0, m_svFile.GetPathName(), compressedSize);
				}
				else if (gameNameStr == "O.D.T.")
				{
					// written directly
					int compressedSize = -1;
					CODTCompress odtcompress;
					odtcompress.decode(inFile, 0, m_svFile.GetPathName(), compressedSize);
				}

				if ((outputDecompressed != NULL))
				{
					if (fileSize > 0)
					{
						FILE* outFile = fopen(m_svFile.GetPathName(), "wb");
						for (int x = 0; x < fileSize; x++)
						{
							fwrite(&outputDecompressed[x], 1, 1, outFile);
						}
						fclose(outFile);
					}

					delete [] outputDecompressed;
				}
			}
			catch (...)
			{
				MessageBox("Error decompressing");
			}

			delete [] Buffer;


			fclose(inFile);
		}
		else
		{
			MessageBox("Cannot open input file");
			return;
		}
	}
}

int CGEDecompressorDlg::HexToInt(char inChar)
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

unsigned long CGEDecompressorDlg::StringHexToLong(CString inString)
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

unsigned long CGEDecompressorDlg::GetSizeFile(CString filename)
{
	FILE* inFile = fopen(filename, "rb");
	
	if (inFile == 0)
		return 0;

	fseek(inFile, 0, SEEK_END);
	unsigned long fileSize = ftell(inFile);
	fclose(inFile);
	return fileSize;
}

void CGEDecompressorDlg::OnBnClickedCompressfilebutton()
{
	// TODO: Add your control notification handler code here
	CString fileOpen;
	CFileDialog m_ldFile(TRUE, NULL, "compressedfile.bin", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Bin File of Codes (*.*)|*.*|");
	int didRead = m_ldFile.DoModal();
	if ((didRead == IDCANCEL) || (m_ldFile.GetPathName() == ""))
		return;

	if (didRead == FALSE)
		return;

	CString gameNameStr;
	m_gameselection.GetWindowText(gameNameStr);

	if (GetZLibGameName(gameNameStr) != -1)
	{
		GECompression compressed;
		compressed.SetPath(directory);
		compressed.SetGame(GetZLibGameName(gameNameStr));

		CFileDialog m_svFile(FALSE, "bin", (m_ldFile.GetFileName() + ".bin"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Bin File of Codes (*.*)|*.*|");
		didRead = m_svFile.DoModal();
		if ((didRead == IDCANCEL) || (m_svFile.GetPathName() == ""))
			return;

		if (didRead == FALSE)
			return;

		compressed.CompressGZipFile(m_ldFile.GetPathName(), m_svFile.GetPathName(), false);
	}
	else if ((gameNameStr == "Turok 1")|| (gameNameStr == "Duke Nukem 64") || (gameNameStr == "Turok 2") || (gameNameStr == "Turok 3") || (gameNameStr == "Turok Rage Wars") || (gameNameStr == "Iggy's Reckin' Balls") || (gameNameStr == "Shadowman") || (gameNameStr == "NHL Breakaway 98") || (gameNameStr == "NHL Breakaway 99") || (gameNameStr == "PGA European Tour"))
	{
		RncDecoder rnccompression;

		CFileDialog m_svFile(FALSE, "bin", (m_ldFile.GetFileName() + ".bin"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Bin File of Codes (*.*)|*.*|");
		didRead = m_svFile.DoModal();
		if ((didRead == IDCANCEL) || (m_svFile.GetPathName() == ""))
			return;

		if (didRead == FALSE)
			return;


		rnccompression.CompressRNCFile(directory, m_ldFile.GetPathName(), m_svFile.GetPathName(), false);
	}
	else if (gameNameStr == "Forsaken")
	{
		RncDecoder rnccompression;

		CFileDialog m_svFile(FALSE, "bin", (m_ldFile.GetFileName() + ".bin"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Bin File of Codes (*.*)|*.*|");
		didRead = m_svFile.DoModal();
		if ((didRead == IDCANCEL) || (m_svFile.GetPathName() == ""))
			return;

		if (didRead == FALSE)
			return;


		rnccompression.CompressRNCFile(directory, m_ldFile.GetPathName(), m_svFile.GetPathName(), true);
	}
	else if ((gameNameStr == "ECW Hardcore Revolution") || (gameNameStr == "NFL QB Club 2000") || (gameNameStr == "NFL QB Club 2001") || (gameNameStr == "NFL QB Club 98") || (gameNameStr == "NFL QB Club 99") || (gameNameStr == "WWF WarZone") || (gameNameStr == "WWF Attitude") || (gameNameStr == "All-Star Baseball 99") || (gameNameStr == "South Park Chefs Love Shack"))
	{
		RncDecoder rnccompression;

		CFileDialog m_svFile(FALSE, "bin", (m_ldFile.GetFileName() + ".bin"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Bin File of Codes (*.*)|*.*|");
		didRead = m_svFile.DoModal();
		if ((didRead == IDCANCEL) || (m_svFile.GetPathName() == ""))
			return;

		if (didRead == FALSE)
			return;


		rnccompression.CompressRNCFile(directory, m_ldFile.GetPathName(), m_svFile.GetPathName(), false);
	}
	else if (gameNameStr == "Armorines")
	{
		RncDecoder rnccompression;

		CFileDialog m_svFile(FALSE, "bin", (m_ldFile.GetFileName() + ".bin"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Bin File of Codes (*.*)|*.*|");
		didRead = m_svFile.DoModal();
		if ((didRead == IDCANCEL) || (m_svFile.GetPathName() == ""))
			return;

		if (didRead == FALSE)
			return;


		rnccompression.CompressRNCFile(directory, m_ldFile.GetPathName(), m_svFile.GetPathName(), false);
	}
	else if (gameNameStr == "South Park")
	{
		RncDecoder rnccompression;

		CFileDialog m_svFile(FALSE, "bin", (m_ldFile.GetFileName() + ".bin"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Bin File of Codes (*.*)|*.*|");
		didRead = m_svFile.DoModal();
		if ((didRead == IDCANCEL) || (m_svFile.GetPathName() == ""))
			return;

		if (didRead == FALSE)
			return;


		rnccompression.CompressRNCFile(directory, m_ldFile.GetPathName(), m_svFile.GetPathName(), false);
	}
	else if ((gameNameStr == "AeroFighter") || (gameNameStr == "Beetle Adventure Racing") || (gameNameStr == "Body Harvest") || (gameNameStr == "Duck Dodgers") || (gameNameStr == "F-1 World Grand Prix")|| (gameNameStr == "F-1 World Grand Prix II (E)")|| (gameNameStr == "F-ZERO Expansion Kit 64DD")|| (gameNameStr == "F-ZERO X")|| (gameNameStr == "Indy Racing 2000")|| (gameNameStr == "Lt. Duck Dodgers (Prototype)")|| (gameNameStr == "MarioKart64")|| (gameNameStr == "Pilotwings 64")|| (gameNameStr == "StarFox64")|| (gameNameStr == "Super Mario 64")|| (gameNameStr == "WaveRace"))
	{
		MIO0 mio0compression;

		CFileDialog m_svFile(FALSE, "bin", (m_ldFile.GetFileName() + ".bin"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Bin File of Codes (*.*)|*.*|");
		didRead = m_svFile.DoModal();
		if ((didRead == IDCANCEL) || (m_svFile.GetPathName() == ""))
			return;

		if (didRead == FALSE)
			return;


		mio0compression.CompressMIO0File(directory, m_ldFile.GetPathName(), m_svFile.GetPathName());
	}
	else if ((gameNameStr == "Starcraft") || (gameNameStr == "Bassmasters 2000") || (gameNameStr == "Ms PacMan") || (gameNameStr == "Power Rangers"))
	{
		MessageBox("Unsupported game, cannot compress");
		return;
	}
	else if ((gameNameStr == "1080Snow") || (gameNameStr == "BakuBomberman2")|| (gameNameStr == "Battlezone Black Dogs")|| (gameNameStr == "Bomberman 64 Second Attack")|| (gameNameStr == "Disney's Donald Duck - Goin' Quackers")|| (gameNameStr == "Doraemon 2")|| (gameNameStr == "Fushigi no Dungeon")|| (gameNameStr == "Kira to Kaiketsu")|| (gameNameStr == "Neon Genesis Evangelion")|| (gameNameStr == "Nushi Tsuri 64")|| (gameNameStr == "Paper Mario")|| (gameNameStr == "Parlor Pro 64")|| (gameNameStr == "Pokemon Stadium")|| (gameNameStr == "Pokemon Stadium 2")|| (gameNameStr == "Rayman 2")|| (gameNameStr == "Scooby-Doo")|| (gameNameStr == "Shadowgate 64")|| (gameNameStr == "Tonic Trouble")|| (gameNameStr == "Tsumi to Batsu")|| (gameNameStr == "Zool"))
	{
		MessageBox("Unsupported game, cannot compress");
		return;
	}
	else if (gameNameStr == "SSB")
	{
		MessageBox("Unsupported game, cannot compress");
		return;
	}
	else if (gameNameStr == "Pokemon Snap")
	{
		MessageBox("Unsupported game, cannot compress");
		return;
	}
	else if (gameNameStr == "Shigesato Itoi Bass Fishing")
	{
		MessageBox("Unsupported game, cannot compress");
		return;
	}
	else if (gameNameStr == "Superman")
	{
		MessageBox("Unsupported game, cannot compress");
		return;
	}
	else if (gameNameStr == "Monaco Grand Prix")
	{
		MessageBox("Unsupported game, cannot compress");
		return;
	}
	else if ((gameNameStr == "Tony Hawk Pro Skater 1") || (gameNameStr == "Tony Hawk Pro Skater 2") || (gameNameStr == "Tony Hawk Pro Skater 3"))
	{
		MessageBox("Unsupported game, cannot compress");
		return;
	}
	else if ((gameNameStr == "Monster Truck Madness") || (gameNameStr == "Spiderman"))
	{
		MessageBox("Unsupported game, cannot compress");
		return;
	}
	else if ((gameNameStr == "Doubutsu no Mori") || (gameNameStr == "MajorasMask") || (gameNameStr == "OoT"))
	{
		YAZ0 yaz0;
		
		CFileDialog m_svFile(FALSE, "bin", (m_ldFile.GetFileName() + ".bin"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Bin File of Codes (*.*)|*.*|");
		didRead = m_svFile.DoModal();
		if ((didRead == IDCANCEL) || (m_svFile.GetPathName() == ""))
			return;

		if (didRead == FALSE)
			return;

		FILE* tempInputFile = fopen(m_ldFile.GetPathName(), "rb");
		fseek(tempInputFile, 0, SEEK_END);
		unsigned long size = ftell(tempInputFile);

		unsigned char* tempBuffer;
		tempBuffer = new unsigned char[size];
		
		fseek(tempInputFile, 0, SEEK_SET);
		fread(tempBuffer, 1, size, tempInputFile);
		fclose(tempInputFile);
		int compressedSize;

		unsigned char* outputBuffer = new unsigned char[0x200000];
		yaz0.yaz0_encode(tempBuffer, size, outputBuffer, &compressedSize);

		if (compressedSize > 0)
		{
			FILE* outputFileName = fopen(m_svFile.GetPathName(), "wb");
			if (outputFileName == NULL)
			{
				MessageBox("Error opening output file", "Error");
				return;
			}

			fwrite(outputBuffer, 1, (compressedSize), outputFileName);	

			fflush(outputFileName);
			fclose(outputFileName);
		}

		delete [] tempBuffer;
		delete [] outputBuffer;
	}
	else if ((gameNameStr == "40 Winks")|| (gameNameStr == "Cruis'n World")|| (gameNameStr == "Duke Nukem - ZER0 H0UR")|| (gameNameStr == "Hydro Thunder")|| (gameNameStr == "Mortal Kombat 4")|| (gameNameStr == "NBA Showtime")|| (gameNameStr == "Tarzan")|| (gameNameStr == "TWINE")|| (gameNameStr == "War Gods"))
	{
		CEDLCompress edlcompress;

		CFileDialog m_svFile(FALSE, "bin", (m_ldFile.GetFileName() + ".bin"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Bin File of Codes (*.*)|*.*|");
		didRead = m_svFile.DoModal();
		if ((didRead == IDCANCEL) || (m_svFile.GetPathName() == ""))
			return;

		if (didRead == FALSE)
			return;


		edlcompress.encode(m_ldFile.GetPathName(), m_svFile.GetPathName());
	}
	else if ((gameNameStr == "Custom Robo") || (gameNameStr == "Custom Robo V2"))
	{
		CCustomRoboCompress customrobocompress;

		CFileDialog m_svFile(FALSE, "bin", (m_ldFile.GetFileName() + ".bin"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Bin File of Codes (*.*)|*.*|");
		didRead = m_svFile.DoModal();
		if ((didRead == IDCANCEL) || (m_svFile.GetPathName() == ""))
			return;

		if (didRead == FALSE)
			return;


		customrobocompress.encode(m_ldFile.GetPathName(), m_svFile.GetPathName());
	}
	else if (gameNameStr == "O.D.T.")
	{
		CODTCompress odtcompress;

		CFileDialog m_svFile(FALSE, "bin", (m_ldFile.GetFileName() + ".bin"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Bin File of Codes (*.*)|*.*|");
		didRead = m_svFile.DoModal();
		if ((didRead == IDCANCEL) || (m_svFile.GetPathName() == ""))
			return;

		if (didRead == FALSE)
			return;


		odtcompress.encode(m_ldFile.GetPathName(), m_svFile.GetPathName());
	}
	else
	{
		MessageBox("Unsupported game, cannot compress");
		return;
	}
}

float CGEDecompressorDlg::CharArrayToFloat(unsigned char* currentSpot)
{
	unsigned long tempLong = (Flip32Bit(*reinterpret_cast<unsigned long*> (currentSpot)));
	return (*reinterpret_cast<float*> (&tempLong));
}

unsigned long CGEDecompressorDlg::CharArrayToLong(unsigned char* currentSpot)
{
	return Flip32Bit(*reinterpret_cast<unsigned long*> (currentSpot));
}

unsigned long CGEDecompressorDlg::Flip32Bit(unsigned long inLong)
{
	return (((inLong & 0xFF000000) >> 24) | ((inLong & 0x00FF0000) >> 8) | ((inLong & 0x0000FF00) << 8) | ((inLong & 0x000000FF) << 24));
}

unsigned short CGEDecompressorDlg::CharArrayToShort(unsigned char* currentSpot)
{
	return Flip16Bit(*reinterpret_cast<unsigned short*> (currentSpot));
}

unsigned short CGEDecompressorDlg::Flip16Bit(unsigned short ShortValue)
{
	return ((ShortValue >> 8) | ((ShortValue << 8)));
}

void CGEDecompressorDlg::ReceivedNewROM(CGEDecompressorDlg* dlg, CString fileLocation, unsigned char* GameBuffer, int romSize)
{
	if (romSize > 0)
	{
		dlg->romName = fileLocation;
		dlg->ROMSize = romSize;
		if (dlg->ROM != NULL)
		{
			delete [] dlg->ROM;
			dlg->ROM = NULL;
		}
		dlg->ROM = new unsigned char[romSize];
		memcpy(dlg->ROM, GameBuffer, romSize);
	}
}


void CGEDecompressorDlg::DecryptBTFile(int fileNumber, unsigned char* input, unsigned char* output, int size)
{
	char rsp[0x20]; 

	unsigned char inputKey[0x10];

	signed short key = (fileNumber - 0x955);

	signed long A2 = 0x10;
	for (signed long V1 = 0; V1 < 0xE; V1+=2)
	{

		signed long T6 = key >> V1;
		signed long T7 = A2 - V1;
		signed long T8 = key << T7;

		signed long T9 = T6 | T8;

		inputKey[V1] = ((T9) & 0xFF);
		inputKey[V1+1] = ((T9 & 0xFF00) & 0xFF);
	}

	inputKey[0x0E] = 0x00;
	inputKey[0x0F] = 0x02;

	char nibbleKeyVersion[0x20];
	for (int x = 0; x < 0x20; x++)
	{
		if ((x % 2) == 0)
			nibbleKeyVersion[x] = ((inputKey[x/2] >> 4) & 0xF);
		else
			nibbleKeyVersion[x] = ((inputKey[x/2]) & 0xF);
	}

	n64_cic_nus_6105(nibbleKeyVersion, rsp, CHL_LEN - 2);
	rsp[CHL_LEN - 2] = rsp[CHL_LEN - 1] = 0;

	unsigned char cicValue[0x10];
	for (int x = 0; x < 0x20; x+=2)
	{
		cicValue[x/2] = (rsp[x] << 4) | rsp[x+1];
	}

	for (int x = 0; x < size; x++)
	{
		unsigned char value = input[x];
		output[x] = value ^ cicValue[x % 0xE];
	}
}

void CGEDecompressorDlg::DecompressConkerFromTable(CGEDecompressorDlg* dlg, CString filein, unsigned char* input, int size, unsigned long start, unsigned long end, int GAME, bool writeFileNumberInstead, int bankNumber)
{
	int step = 8;
	CString folderPath = filein;
	folderPath = folderPath.Mid(0, (folderPath.ReverseFind('\\') + 1));

	GECompression compressed;
	compressed.SetPath(dlg->directory);
	compressed.SetGame(GAME);

	for (unsigned long x = start; x < end; x+=step)
	{
		if (dlg->killThread)
			break;
		unsigned long address;
		CString type = "";
		CString fileNameStr = "";
		
		address = CharArrayToLong(&input[x]) + start;
		
		CString tempLocation;
		int fileSizeCompressed = CharArrayToLong(&input[x+4]) & 0xFFFFFF;
		unsigned char isBit = input[x+4];

		if (fileSizeCompressed > 0)
		{
			if (isBit & 0x10) // compressed
			{
				int writeAddress = address;
				if (writeFileNumberInstead)
					writeAddress = bankNumber;

				int fileSizeUncompressed = DecompressZLibSpot(&compressed, dlg->genText, address, input, size, GAME, folderPath, fileNameStr, -1, tempLocation, fileSizeCompressed, type, writeAddress, writeFileNumberInstead, ((x - start) / 8));
				if (fileSizeUncompressed > 0)
				{
					AddRowData(dlg, address, fileSizeCompressed, fileSizeUncompressed, fileNameStr, tempLocation, type);
				}
			}
			else
			{
				if (writeFileNumberInstead)
					tempLocation.Format("%s%06X_%06X.bin", folderPath, bankNumber, ((x - start) / 8));
				else
					tempLocation.Format("%s%06X.bin", folderPath, address);

				WriteResult(dlg->genText, tempLocation, &input[address], fileSizeCompressed, false);

				AddRowData(dlg, address, fileSizeCompressed, fileSizeCompressed, fileNameStr, tempLocation, "Uncompressed");
			}
		}
	}
}

void CGEDecompressorDlg::DecompressZLibFromTable(CString gameNameStr, CGEDecompressorDlg* dlg, CString filein, unsigned long start, unsigned long end, int step, int GAME, unsigned long tblOffset, int shift, int multiplier, int offset)
{
	CString folderPath = filein;
	folderPath = folderPath.Mid(0, (folderPath.ReverseFind('\\') + 1));

	GECompression compressed;
	compressed.SetPath(dlg->directory);
	compressed.SetGame(GAME);

	unsigned char* input;
	int size;
	if (ReadROM(gameNameStr, filein, input, size, folderPath))
	{
		ReceivedNewROM(dlg, filein, input, size);
		for (unsigned long x = start; x < end; x+=step)
		{
			if (dlg->killThread)
				break;
			ToUpdateProgressBar(dlg, (x - start), (end - x));
			unsigned long address;
			CString type = "";
			CString fileNameStr = "";

			if (GAME == BANJOTOOIE)
			{
				address = ((CharArrayToLong(&input[x]) >> shift) * multiplier) + tblOffset + offset;
				unsigned char btType = input[x+3];
				if (btType == 0x04) // empty
					continue;

				type.Format("%02X", btType);
				if (btType == 0x1A)
					type += " Encrypted";
			}
			else 
				address = CharArrayToLong(&input[x]) + tblOffset;
			
			CString tempLocation;
			int fileSizeCompressed = -1;
			

			if ((GAME == BANJOTOOIE) && (input[x+3] == 0x1A)) // Encrypted
			{
				unsigned long nextFile = ((CharArrayToLong(&input[x+4]) >> shift) * multiplier) + tblOffset + offset;
				fileSizeCompressed = (nextFile - address);
				unsigned char* compressedCopy = new unsigned char[fileSizeCompressed];
				memcpy(compressedCopy, &input[address], fileSizeCompressed);
				int fileNumber = ((x - start) / 4);
				fileNameStr.Format("%04X", fileNumber);
				DecryptBTFile(fileNumber, compressedCopy, compressedCopy, fileSizeCompressed);

				int fileSizeUncompressed = DecompressZLibSpot(&compressed, dlg->genText, 0, compressedCopy, fileSizeCompressed, GAME, folderPath, fileNameStr, -1, tempLocation, fileSizeCompressed, type, address, false, 0);
				if (fileSizeUncompressed > 0)
				{
					fileSizeCompressed = (nextFile - address);
					AddRowData(dlg, x, fileSizeCompressed, fileSizeUncompressed, fileNameStr, tempLocation, type);
				}
			}
			else
			{
				int fileSizeUncompressed = DecompressZLibSpot(&compressed, dlg->genText, address, input, size, GAME, folderPath, fileNameStr, -1, tempLocation, fileSizeCompressed, type, address, false, 0);
				if (fileSizeUncompressed > 0)
				{
					AddRowData(dlg, x, fileSizeCompressed, fileSizeUncompressed, fileNameStr, tempLocation, type);
				}
			}
			/*CString name;
			unsigned long curSpot = 0;
			bool started = false;
			while (curSpot < numBytes)
			{
				if (started)
				{
					if (((output[curSpot] >= 0x41) && (output[curSpot] < 0x7B)))
					{
						CString tempAddStr;
						tempAddStr.Format("%c", output[curSpot]);
						name = name + tempAddStr;
						curSpot++;
					}
					else
						break;
				}
				else
				{
					if (((output[curSpot] >= 0x41) && (output[curSpot] < 0x7B))
						&& ((output[curSpot+1] >= 0x41) && (output[curSpot+1] < 0x7B))
						&& ((output[curSpot+2] >= 0x41) && (output[curSpot+2] < 0x7B))
						)
					{
						started = true;
						CString tempAddStr;
						tempAddStr.Format("%c", output[curSpot]);
						name = name + tempAddStr;
					}
					curSpot++;
				}
			}*/
		}

		delete [] input;
	}
}

bool CGEDecompressorDlg::ReadROM(CString gameNameStr, CString fileLocation, unsigned char*& GameBuffer, int& romSize, CString& folderPath)
{
	folderPath = fileLocation;
	folderPath = folderPath.Mid(0, (folderPath.ReverseFind('\\') + 1));

	
	romSize = GetSizeFile(fileLocation);
	FILE* inFile = fopen(fileLocation,"rb");
	if (inFile == NULL)
	{
		::MessageBox(NULL, "Invalid ROM", "Error", NULL);
		return false;
	}
	
	
	if (romSize == 0)
	{
		fclose(inFile);
		return false;
	}

	fseek(inFile,0,SEEK_END);
	romSize = ftell(inFile);
	rewind(inFile);

	GameBuffer = new unsigned char[romSize+1];
	int readAmt = fread(GameBuffer, 1, romSize, inFile);
	fclose(inFile);

	
	bool byteFlippedROM = false;
	if ((gameNameStr != "Star Fox Adventures (GC) ZLB"))
	{
		if ((GameBuffer[0] == 0x37) || (GameBuffer[0] == 0x08))
			byteFlippedROM = true;
		else if (((GameBuffer[0] == 0x80) || (GameBuffer[0] == 0x3C)) || (CharArrayToLong(&GameBuffer[0]) == 0xE848D316))
			byteFlippedROM = false;
		else
		{
			delete [] GameBuffer;
			::MessageBox(NULL, "Not a valid ROM", "Error", NULL);
			return false;
		}
	}

	if (byteFlippedROM == true)
	{
		if ((romSize % 2) == 1)
		{
			GameBuffer[romSize] = 0;
			romSize++;
		}

		for (int x = 0; x < romSize; x=x+2)
		{
			unsigned char tempSpot = GameBuffer[x];
			GameBuffer[x] = GameBuffer[x+1];
			GameBuffer[x+1] = tempSpot;
		}
	}	
	return true;
}

bool CGEDecompressorDlg::AllocateInput(int offset, unsigned char*& Buffer, unsigned char* GameBuffer, int& endSize, int romSize)
{
	endSize = maxByteSize;
	if ((offset + maxByteSize) > romSize)
	{
		endSize = (romSize - offset);
	}

	Buffer = new unsigned char[endSize];
	for (int y = 0; y < endSize; y++)
	{
		Buffer[y] = GameBuffer[offset+y];
	}
	return true;
}

int CGEDecompressorDlg::DecompressMonacoSpot(MonacoDecoder* compressed, bool genText, int offset, unsigned char* GameBuffer, int romSize, int GAME, CString folderPath, CString internalName, int expectedSize, CString& tempLocation, int& fileSizeCompressed, CString& type)
{
	int returnSize = 0;
	unsigned char* Buffer = NULL;
	unsigned char* outputDecompressed = NULL;

	try
	{
		int endSize = 0;
		AllocateInput(offset, Buffer, GameBuffer, endSize, romSize);
		
		outputDecompressed = new unsigned char[maxByteSize];
		int fileSize = compressed->dec(Buffer, fileSizeCompressed, expectedSize, outputDecompressed); 

		if ((outputDecompressed != NULL))
		{
			if (fileSize > 0)
			{
				if ((expectedSize == -1) || (fileSize == expectedSize))
				{
					if (internalName == "")
					{
						if ((outputDecompressed[0] == 0x42) && (outputDecompressed[1] == 0x31) && (outputDecompressed[2] == 0x00) && (outputDecompressed[3] == 0x01))
							tempLocation.Format("%s%06X.b1", folderPath, offset);
						else if ((outputDecompressed[0] == 0x4D) && (outputDecompressed[1] == 0x54) && (outputDecompressed[2] == 0x68) && (outputDecompressed[3] == 0x64))
							tempLocation.Format("%s%06X.mid", folderPath, offset);
						else
							tempLocation.Format("%s%06X.bin", folderPath, offset);
					}
					else
						tempLocation.Format("%s%06X_%s.bin", folderPath, offset, internalName);

					WriteResult(genText, tempLocation, outputDecompressed, fileSize, false);

					returnSize = fileSize;
				}
			}

			delete [] outputDecompressed;
		}

		delete [] Buffer;
	}
	catch (...)
	{
		if (Buffer != NULL)
		{
			delete [] Buffer;
			Buffer = NULL;
		}
		if (outputDecompressed != NULL)
		{
			delete [] outputDecompressed;
			outputDecompressed = NULL;
		}
	}
	return returnSize;
}

int CGEDecompressorDlg::DecompressSupermanSpot(SupermanDecoder* compressed, bool genText, int offset, unsigned char* GameBuffer, int romSize, int GAME, CString folderPath, CString internalName, int expectedSize, CString& tempLocation, int& fileSizeCompressed, CString& type)
{
	int returnSize = 0;
	unsigned char* Buffer = NULL;
	unsigned char* outputDecompressed = NULL;

	try
	{
		int endSize = 0;
		AllocateInput(offset, Buffer, GameBuffer, endSize, romSize);
		
		outputDecompressed = new unsigned char[maxByteSize];
		int fileSize = compressed->dec(Buffer, fileSizeCompressed, expectedSize, outputDecompressed); 

		if ((outputDecompressed != NULL))
		{
			if (fileSize > 0)
			{
				if ((expectedSize == -1) || (fileSize == expectedSize))
				{
					if (internalName == "")
					{
						if ((outputDecompressed[0] == 0x42) && (outputDecompressed[1] == 0x31) && (outputDecompressed[2] == 0x00) && (outputDecompressed[3] == 0x01))
							tempLocation.Format("%s%06X.b1", folderPath, offset);
						else if ((outputDecompressed[0] == 0x4D) && (outputDecompressed[1] == 0x54) && (outputDecompressed[2] == 0x68) && (outputDecompressed[3] == 0x64))
							tempLocation.Format("%s%06X.mid", folderPath, offset);
						else
							tempLocation.Format("%s%06X.bin", folderPath, offset);
					}
					else
						tempLocation.Format("%s%06X_%s.bin", folderPath, offset, internalName);

					WriteResult(genText, tempLocation, outputDecompressed, fileSize, false);

					returnSize = fileSize;
				}
			}

			delete [] outputDecompressed;
		}

		delete [] Buffer;
	}
	catch (...)
	{
		if (Buffer != NULL)
		{
			delete [] Buffer;
			Buffer = NULL;
		}
		if (outputDecompressed != NULL)
		{
			delete [] outputDecompressed;
			outputDecompressed = NULL;
		}
	}
	return returnSize;
}

int CGEDecompressorDlg::DecompressBOLTSpot(CBoltDecoder* compressed, bool genText, int offset, unsigned char* GameBuffer, int romSize, int GAME, CString folderPath, CString internalName, int expectedSize, CString& tempLocation, int& fileSizeCompressed, CString& type, int bank, int entry)
{
	fileSizeCompressed = -1;
	int returnSize = 0;
	unsigned char* Buffer = NULL;
	unsigned char* outputDecompressed = NULL;

	try
	{
		int endSize = 0;
		AllocateInput(offset, Buffer, GameBuffer, endSize, romSize);
		
		outputDecompressed = new unsigned char[maxByteSize];
		int fileSize = compressed->dec(Buffer, outputDecompressed, fileSizeCompressed, expectedSize); 

		if ((outputDecompressed != NULL))
		{
			if (fileSize > 0)
			{
				if ((expectedSize == -1) || (fileSize == expectedSize))
				{
					if (internalName == "")
						tempLocation.Format("%s%06X_B%02X_%02X.bin", folderPath, offset, bank, entry);
					else
						tempLocation.Format("%s%06X_%s.bin", folderPath, offset, internalName);

					WriteResult(genText, tempLocation, outputDecompressed, fileSize, false);

					returnSize = fileSize;
				}
			}

			delete [] outputDecompressed;
		}

		delete [] Buffer;
	}
	catch (...)
	{
		if (Buffer != NULL)
		{
			delete [] Buffer;
			Buffer = NULL;
		}
		if (outputDecompressed != NULL)
		{
			delete [] outputDecompressed;
			outputDecompressed = NULL;
		}
	}
	return returnSize;
}

int CGEDecompressorDlg::DecompressMIO0Spot(MIO0* compressed, bool genText, int offset, unsigned char* GameBuffer, int romSize, int GAME, CString folderPath, CString internalName, int expectedSize, CString& tempLocation, int& fileSizeCompressed, CString& type)
{
	fileSizeCompressed = -1;
	int returnSize = 0;
	unsigned char* Buffer = NULL;
	unsigned char* outputDecompressed = NULL;

	try
	{
		int endSize = 0;
		AllocateInput(offset, Buffer, GameBuffer, endSize, romSize);
		
		outputDecompressed = new unsigned char[maxByteSize];
		int fileSize = compressed->mio0_decode(Buffer, outputDecompressed, fileSizeCompressed); 

		if ((outputDecompressed != NULL))
		{
			if (fileSize > 0)
			{
				if ((expectedSize == -1) || (fileSize == expectedSize))
				{
					if (internalName == "")
						tempLocation.Format("%s%06X.bin", folderPath, offset);
					else
						tempLocation.Format("%s%06X_%s.bin", folderPath, offset, internalName);

					WriteResult(genText, tempLocation, outputDecompressed, fileSize, false);

					returnSize = fileSize;
				}
			}

			delete [] outputDecompressed;
		}

		delete [] Buffer;
	}
	catch (...)
	{
		if (Buffer != NULL)
		{
			delete [] Buffer;
			Buffer = NULL;
		}
		if (outputDecompressed != NULL)
		{
			delete [] outputDecompressed;
			outputDecompressed = NULL;
		}
	}
	return returnSize;
}

int CGEDecompressorDlg::DecompressERZSpot(ERZ* compressed, bool genText, int offset, unsigned char* GameBuffer, int romSize, int GAME, CString folderPath, CString internalName, int expectedSize, CString& tempLocation, int& fileSizeCompressed, CString& type)
{
	int returnSize = 0;
	unsigned char* Buffer = NULL;
	unsigned char* outputDecompressed = NULL;

	try
	{
		int endSize = 0;
		AllocateInput(offset, Buffer, GameBuffer, endSize, romSize);
		
		outputDecompressed = new unsigned char[maxByteSize];
		if (endSize > 0xFE000)
			endSize = 0xFE000;
		int fileSize = compressed->decode(Buffer, endSize, outputDecompressed, fileSizeCompressed); 

		if ((outputDecompressed != NULL))
		{
			if (fileSize > 0)
			{
				if (Buffer[3] == 0x01)
					type = "ERZ1";
				else if (Buffer[3] == 0x02)
					type = "ERZ2";
				if ((expectedSize == -1) || (fileSize == expectedSize))
				{
					if (internalName == "")
						tempLocation.Format("%s%06X.bin", folderPath, offset);
					else
						tempLocation.Format("%s%06X_%s.bin", folderPath, offset, internalName);

					WriteResult(genText, tempLocation, outputDecompressed, fileSize, false);

					returnSize = fileSize;
				}
			}

			delete [] outputDecompressed;
		}

		delete [] Buffer;
	}
	catch (...)
	{
		if (Buffer != NULL)
		{
			delete [] Buffer;
			Buffer = NULL;
		}
		if (outputDecompressed != NULL)
		{
			delete [] outputDecompressed;
			outputDecompressed = NULL;
		}
	}
	return returnSize;
}

int CGEDecompressorDlg::DecompressYAY0Spot(YAY0* compressed, bool genText, int offset, unsigned char* GameBuffer, int romSize, int GAME, CString folderPath, CString internalName, int expectedSize, CString& tempLocation, int& fileSizeCompressed, CString& type)
{
	fileSizeCompressed = -1;
	int returnSize = 0;
	unsigned char* Buffer = NULL;
	unsigned char* outputDecompressed = NULL;

	try
	{
		int endSize = 0;
		AllocateInput(offset, Buffer, GameBuffer, endSize, romSize);
		
		outputDecompressed = new unsigned char[maxByteSize];
		int fileSize = compressed->decodeAll(Buffer, outputDecompressed, fileSizeCompressed); 

		if ((outputDecompressed != NULL))
		{
			if (fileSize > 0)
			{
				if ((expectedSize == -1) || (fileSize == expectedSize))
				{
					if (internalName == "")
						tempLocation.Format("%s%06X.bin", folderPath, offset);
					else
						tempLocation.Format("%s%06X_%s.bin", folderPath, offset, internalName);

					WriteResult(genText, tempLocation, outputDecompressed, fileSize, false);

					returnSize = fileSize;
				}
			}

			delete [] outputDecompressed;
		}

		delete [] Buffer;
	}
	catch (...)
	{
		if (Buffer != NULL)
		{
			delete [] Buffer;
			Buffer = NULL;
		}
		if (outputDecompressed != NULL)
		{
			delete [] outputDecompressed;
			outputDecompressed = NULL;
		}
	}
	return returnSize;
}

int CGEDecompressorDlg::DecompressVPK0Spot(CVPK0Decoder* compressed, bool genText, int offset, unsigned char* GameBuffer, int romSize, int GAME, CString folderPath, CString internalName, int expectedSize, CString& tempLocation, int& fileSizeCompressed, CString& type)
{
	int returnSize = 0;
	unsigned char* Buffer = NULL;
	unsigned char* outputDecompressed = NULL;

	try
	{
		int endSize = 0;
		AllocateInput(offset, Buffer, GameBuffer, endSize, romSize);

		outputDecompressed = new unsigned char[maxByteSize];
		int fileSize = compressed->dec(Buffer, fileSizeCompressed, expectedSize, outputDecompressed); 

		if ((outputDecompressed != NULL))
		{
			if (fileSize > 0)
			{
				if ((expectedSize == -1) || (fileSize == expectedSize))
				{
					if (internalName == "")
						tempLocation.Format("%s%06X.bin", folderPath, offset);
					else
						tempLocation.Format("%s%06X_%s.bin", folderPath, offset, internalName);

					WriteResult(genText, tempLocation, outputDecompressed, fileSize, false);

					returnSize = fileSize;
				}
			}

			delete [] outputDecompressed;
		}

		delete [] Buffer;
	}
	catch (...)
	{
		if (Buffer != NULL)
		{
			delete [] Buffer;
			Buffer = NULL;
		}
		if (outputDecompressed != NULL)
		{
			delete [] outputDecompressed;
			outputDecompressed = NULL;
		}
	}
	return returnSize;
}

int CGEDecompressorDlg::DecompressBlitzSpot(CBlitzDecoder* compressed, bool genText, int offset, unsigned char* GameBuffer, int romSize, int GAME, CString folderPath, CString internalName, int expectedSize, CString& tempLocation, int& fileSizeCompressed, CString& type)
{
	fileSizeCompressed = -1;
	int returnSize = 0;
	unsigned char* Buffer = NULL;
	unsigned char* outputDecompressed = NULL;

	try
	{
		int endSize = 0;
		AllocateInput(offset, Buffer, GameBuffer, endSize, romSize);
		
		outputDecompressed = new unsigned char[maxByteSize];
		int fileSize = compressed->dec(Buffer, outputDecompressed, fileSizeCompressed); 

		if ((outputDecompressed != NULL))
		{
			if (fileSize > 0)
			{
				if ((expectedSize == -1) || (fileSize == expectedSize))
				{
					if (internalName == "")
						tempLocation.Format("%s%06X.bin", folderPath, offset);
					else
						tempLocation.Format("%s%06X_%s.bin", folderPath, offset, internalName);

					WriteResult(genText, tempLocation, outputDecompressed, fileSize, false);

					returnSize = fileSize;
				}
			}

			delete [] outputDecompressed;
		}

		delete [] Buffer;
	}
	catch (...)
	{
		if (Buffer != NULL)
		{
			delete [] Buffer;
			Buffer = NULL;
		}
		if (outputDecompressed != NULL)
		{
			delete [] outputDecompressed;
			outputDecompressed = NULL;
		}
	}
	return returnSize;
}

int CGEDecompressorDlg::DecompressFLA2Spot(CFLA2Decoder* compressed, bool genText, int offset, unsigned char* GameBuffer, int romSize, int GAME, CString folderPath, CString internalName, int expectedSize, CString& tempLocation, int& fileSizeCompressed, CString& type)
{
	fileSizeCompressed = -1;
	int returnSize = 0;
	unsigned char* Buffer = NULL;
	unsigned char* outputDecompressed = NULL;

	try
	{
		int endSize = 0;
		AllocateInput(offset, Buffer, GameBuffer, endSize, romSize);
		
		outputDecompressed = new unsigned char[maxByteSize];
		int fileSize = compressed->decFLA2(Buffer, fileSizeCompressed, expectedSize, outputDecompressed); 

		if ((outputDecompressed != NULL))
		{
			if (fileSize > 0)
			{
				if ((expectedSize == -1) || (fileSize == expectedSize))
				{
					if (internalName == "")
						tempLocation.Format("%s%06X.bin", folderPath, offset);
					else
						tempLocation.Format("%s%06X_%s.bin", folderPath, offset, internalName);

					WriteResult(genText, tempLocation, outputDecompressed, fileSize, false);

					returnSize = fileSize;
				}
			}

			delete [] outputDecompressed;
		}

		delete [] Buffer;
	}
	catch (...)
	{
		if (Buffer != NULL)
		{
			delete [] Buffer;
			Buffer = NULL;
		}
		if (outputDecompressed != NULL)
		{
			delete [] outputDecompressed;
			outputDecompressed = NULL;
		}
	}
	return returnSize;
}

int CGEDecompressorDlg::DecompressODTSpot(CODTCompress* compressed, bool genText, int offset, FILE* in, int romSize, int GAME, CString folderPath, CString internalName, int expectedSize, CString& tempLocation, int& fileSizeCompressed, CString& type)
{
	fileSizeCompressed = -1;
	int returnSize = 0;
	try
	{
		if (internalName == "")
			tempLocation.Format("%s%06X.bin", folderPath, offset);
		else
			tempLocation.Format("%s%06X_%s.bin", folderPath, offset, internalName);

		returnSize = compressed->decode(in, offset, tempLocation, fileSizeCompressed); 
	}
	catch (...)
	{
	
	}
	return returnSize;
}	

int CGEDecompressorDlg::DecompressCustomRoboSpot(CCustomRoboCompress* compressed, bool genText, int offset, FILE* in, int romSize, int GAME, CString folderPath, CString internalName, int expectedSize, CString& tempLocation, int& fileSizeCompressed, CString& type)
{
	fileSizeCompressed = -1;
	int returnSize = 0;
	try
	{
		if (internalName == "")
			tempLocation.Format("%s%06X.bin", folderPath, offset);
		else
			tempLocation.Format("%s%06X_%s.bin", folderPath, offset, internalName);

		returnSize = compressed->decode(in, offset, tempLocation, fileSizeCompressed); 
	}
	catch (...)
	{
	
	}
	return returnSize;
}

int CGEDecompressorDlg::DecompressEAGamesSpot(EASportsDecoder* compressed, bool genText, int offset, unsigned char* GameBuffer, int romSize, int GAME, CString folderPath, CString internalName, int expectedSize, CString& tempLocation, int& fileSizeCompressed, CString& type)
{
	fileSizeCompressed = -1;
	int returnSize = 0;
	unsigned char* Buffer = NULL;
	unsigned char* outputDecompressed = NULL;

	try
	{
		int endSize = 0;
		AllocateInput(offset, Buffer, GameBuffer, endSize, romSize);
		
		outputDecompressed = new unsigned char[maxByteSize];
		int fileSize = compressed->dec(Buffer, outputDecompressed, fileSizeCompressed); 

		if ((outputDecompressed != NULL))
		{
			if (fileSize > 0)
			{
				if ((expectedSize == -1) || (fileSize == expectedSize))
				{
					if (internalName == "")
						tempLocation.Format("%s%06X.bin", folderPath, offset);
					else
						tempLocation.Format("%s%06X_%s.bin", folderPath, offset, internalName);

					WriteResult(genText, tempLocation, outputDecompressed, fileSize, false);

					returnSize = fileSize;
				}
			}

			delete [] outputDecompressed;
		}

		delete [] Buffer;
	}
	catch (...)
	{
		if (Buffer != NULL)
		{
			delete [] Buffer;
			Buffer = NULL;
		}
		if (outputDecompressed != NULL)
		{
			delete [] outputDecompressed;
			outputDecompressed = NULL;
		}
	}
	return returnSize;
}

int CGEDecompressorDlg::DecompressEDLSpot(CEDLCompress* compressed, bool genText, int offset, FILE* in, int romSize, int GAME, CString folderPath, CString internalName, int expectedSize, CString& tempLocation, int& fileSizeCompressed, CString& type)
{
	fileSizeCompressed = -1;
	int returnSize = 0;
	try
	{
		if (internalName == "")
			tempLocation.Format("%s%06X.bin", folderPath, offset);
		else
			tempLocation.Format("%s%06X_%s.bin", folderPath, offset, internalName);

		returnSize = compressed->decode(in, offset, tempLocation, fileSizeCompressed, type); 
	}
	catch (...)
	{
	
	}
	return returnSize;
}

int CGEDecompressorDlg::DecompressYAZ0Spot(YAZ0* compressed, bool genText, int offset, unsigned char* GameBuffer, int romSize, int GAME, CString folderPath, CString internalName, int expectedSize, CString& tempLocation, int& fileSizeCompressed, CString& type)
{
	fileSizeCompressed = -1;
	int returnSize = 0;
	unsigned char* Buffer = NULL;
	unsigned char* outputDecompressed = NULL;

	try
	{
		int endSize = 0;
		AllocateInput(offset, Buffer, GameBuffer, endSize, romSize);
		
		outputDecompressed = new unsigned char[maxByteSize];
		int fileSize = compressed->yaz0_decode(Buffer, outputDecompressed, fileSizeCompressed); 

		if ((outputDecompressed != NULL))
		{
			if (fileSize > 0)
			{
				if ((expectedSize == -1) || (fileSize == expectedSize))
				{
					if (internalName == "")
						tempLocation.Format("%s%06X.bin", folderPath, offset);
					else
						tempLocation.Format("%s%06X_%s.bin", folderPath, offset, internalName);

					WriteResult(genText, tempLocation, outputDecompressed, fileSize, false);

					returnSize = fileSize;
				}
			}

			delete [] outputDecompressed;
		}

		delete [] Buffer;
	}
	catch (...)
	{
		if (Buffer != NULL)
		{
			delete [] Buffer;
			Buffer = NULL;
		}
		if (outputDecompressed != NULL)
		{
			delete [] outputDecompressed;
			outputDecompressed = NULL;
		}
	}
	return returnSize;
}

int CGEDecompressorDlg::DecompressRNCSpot(RncDecoder* compressed, bool genText, int offset, unsigned char* GameBuffer, int romSize, int GAME, CString folderPath, CString internalName, int expectedSize, CString& tempLocation, int& fileSizeCompressed, CString& type, bool appendFile, unsigned long appendROMLocation, CString appendInternalFileName)
{
	fileSizeCompressed = -1;
	int returnSize = 0;
	unsigned char* Buffer = NULL;
	unsigned char* outputDecompressed = NULL;

	try
	{
		int endSize = 0;
		AllocateInput(offset, Buffer, GameBuffer, endSize, romSize);
		
		outputDecompressed = new unsigned char[maxByteSize];
		int fileSize = compressed->unpackM1(Buffer, outputDecompressed, 0x0000, fileSizeCompressed); 

		if ((outputDecompressed != NULL))
		{
			if (fileSize > 0)
			{
				if ((expectedSize == -1) || (fileSize == expectedSize))
				{
					if (appendFile)
					{
						if (internalName == "")
							tempLocation.Format("%s%06X.bin", folderPath, appendROMLocation);
						else
							tempLocation.Format("%s%06X_%s.bin", folderPath, appendROMLocation, appendInternalFileName);
					}
					else
					{
						if (internalName == "")
							tempLocation.Format("%s%06X.bin", folderPath, offset);
						else
							tempLocation.Format("%s%06X_%s.bin", folderPath, offset, internalName);
					}

					WriteResult(genText, tempLocation, outputDecompressed, fileSize, appendFile);

					returnSize = fileSize;
				}
			}

			delete [] outputDecompressed;
		}

		delete [] Buffer;
	}
	catch (...)
	{
		if (Buffer != NULL)
		{
			delete [] Buffer;
			Buffer = NULL;
		}
		if (outputDecompressed != NULL)
		{
			delete [] outputDecompressed;
			outputDecompressed = NULL;
		}
	}
	return returnSize;
}

int CGEDecompressorDlg::DecompressCommandConquerSpot(CommandAndConquerDecoder* compressed, bool genText, int offset, unsigned char* GameBuffer, int romSize, int GAME, CString folderPath, CString internalName, int expectedSize, CString& tempLocation, int& fileSizeCompressed, CString& type, bool appendFile, unsigned long appendROMLocation, CString appendInternalFileName, int kind)
{
	int returnSize = 0;
	unsigned char* Buffer = NULL;
	unsigned char* outputDecompressed = NULL;

	try
	{
		int endSize = 0;
		AllocateInput(offset, Buffer, GameBuffer, endSize, romSize);
		
		outputDecompressed = new unsigned char[maxByteSize];
		int fileSize = compressed->dec(Buffer, outputDecompressed, fileSizeCompressed, kind);

		if ((outputDecompressed != NULL))
		{
			if (fileSize > 0)
			{
				if ((expectedSize == -1) || (fileSize == expectedSize))
				{
					if (appendFile)
					{
						if (internalName == "")
							tempLocation.Format("%s%06X", folderPath, appendROMLocation);
						else
							tempLocation.Format("%s%06X_%s", folderPath, appendROMLocation, appendInternalFileName);
					}
					else
					{
						if (internalName == "")
							tempLocation.Format("%s%06X", folderPath, offset);
						else
							tempLocation.Format("%s%06X_%s", folderPath, offset, internalName);
					}

					WriteResult(genText, tempLocation, outputDecompressed, fileSize, appendFile);

					returnSize = fileSize;
				}
			}

			delete [] outputDecompressed;
		}

		delete [] Buffer;
	}
	catch (...)
	{
		if (Buffer != NULL)
		{
			delete [] Buffer;
			Buffer = NULL;
		}
		if (outputDecompressed != NULL)
		{
			delete [] outputDecompressed;
			outputDecompressed = NULL;
		}
	}
	return returnSize;
}

int CGEDecompressorDlg::DecompressZLibSpot(GECompression* compressed, bool genText, int offset, unsigned char* GameBuffer, int romSize, int GAME, CString folderPath, CString internalName, int expectedSize, CString& tempLocation, int& fileSizeCompressed, CString& type, unsigned long printedAddress, bool printBank, unsigned printbankAddress)
{
	int returnSize = 0;
	unsigned char* Buffer = NULL;
	unsigned char* outputDecompressed = NULL;

	try
	{
		int endSize = 0;
		AllocateInput(offset, Buffer, GameBuffer, endSize, romSize);
		
		compressed->SetCompressedBuffer(Buffer, endSize);
		int fileSize = 0;
		outputDecompressed = compressed->OutputDecompressedBuffer(fileSize, fileSizeCompressed);

		if ((outputDecompressed != NULL))
		{
			if (fileSize > 0)
			{
				if ((expectedSize == -1) || (fileSize == expectedSize))
				{
					if (printBank)
						tempLocation.Format("%s%06X_%06X.bin", folderPath, printedAddress, printbankAddress);
					else if (internalName == "")
						tempLocation.Format("%s%06X.bin", folderPath, printedAddress);
					else
						tempLocation.Format("%s%06X_%s.bin", folderPath, printedAddress, internalName);

					WriteResult(genText, tempLocation, outputDecompressed, fileSize, false);

					returnSize = fileSize;
				}
			}

			delete [] outputDecompressed;
		}

		delete [] Buffer;
	}
	catch (...)
	{
		if (Buffer != NULL)
		{
			delete [] Buffer;
			Buffer = NULL;
		}
		if (outputDecompressed != NULL)
		{
			delete [] outputDecompressed;
			outputDecompressed = NULL;
		}
	}
	return returnSize;
}

int CGEDecompressorDlg::GetZLibGameName(CString gameNameStr)
{
	if (gameNameStr == "Goldeneye")
	{
		return GOLDENEYE;
	}
	else if (gameNameStr == "Perfect Dark")
	{
		return PD;
	}
	else if (gameNameStr == "Banjo Kazooie")
	{
		return BANJOKAZOOIE;
	}
	else if (gameNameStr == "Killer Instinct")
	{
		return KILLERINSTINCT;
	}
	else if (gameNameStr == "Donkey Kong 64")
	{
		return DONKEYKONG64;
	}
	else if (gameNameStr == "Blast Corps")
	{
		return BLASTCORPS;
	}
	else if (gameNameStr == "Banjo Tooie")
	{
		return BANJOTOOIE;
	}
	else if (gameNameStr == "Donkey Kong 64 Kiosk")
	{
		return DONKEYKONG64KIOSK;
	}
	else if (gameNameStr == "Conker")
	{
		return CONKER;
	}
	else if (gameNameStr == "Top Gear Rally")
	{
		return TOPGEARRALLY;
	}
	else if (gameNameStr == "Milo's Astro Lanes")
	{
		return MILO;
	}
	else if (gameNameStr == "Blues Brothers")
	{
		return MILO;
	}
	else if (gameNameStr == "Stunt Racer 64")
	{
		return STUNTRACER64;
	}
	else if (gameNameStr == "Star Fox Adventures (GC) ZLB")
	{
		return ZLB;
	}
	else if (gameNameStr == "Jet Force Gemini")
	{
		return JFG;
	}
	else if (gameNameStr == "Diddy Kong Racing")
	{
		return DKR;
	}
	else if (gameNameStr == "JFG Kiosk")
	{
		return JFGKIOSK;
	}
	else if (gameNameStr == "Mickey's Speedway Racing")
	{
		return MICKEYSPEEDWAY;
	}
	else if (gameNameStr == "Mission Impossible")
	{
		return MORTALKOMBAT;
	}
	else if (gameNameStr == "Resident Evil 2")
	{
		return RESIDENTEVIL2;
	}
	return -1;
}



void CGEDecompressorDlg::AddRowData(CGEDecompressorDlg* dlg, unsigned long address, int fileSizeCompressed,  int fileSizeUncompressed, CString internalName, CString tempLocation, CString type)
{
	ListUpdateStruct* listUpdate = new ListUpdateStruct();
	listUpdate->address = address;
	listUpdate->fileSizeUncompressed = fileSizeUncompressed;
	listUpdate->fileSizeCompressed = fileSizeCompressed;
	listUpdate->internalName = static_cast<LPCTSTR>(internalName);
	listUpdate->tempLocation = static_cast<LPCTSTR>(tempLocation);
	listUpdate->type = type;
	::SendMessage(dlg->m_hWnd, UPDATE_LIST, 0, (LPARAM)listUpdate);
}

void CGEDecompressorDlg::ToUpdateProgressBar(CGEDecompressorDlg* dlg, int curSpot,  int romSize)
{
	int progress = (((float)curSpot / (float)romSize) * 100);
	if ((curSpot % 0x10000) == 0)
	{
		if (progress != 100)
			::SendMessage(dlg->m_hWnd, UPDATE_PROGRESS_BAR, 0, (LPARAM)progress);
	}
	if ((progress == 100) && (romSize == curSpot))
		::SendMessage(dlg->m_hWnd, UPDATE_PROGRESS_BAR, 0, (LPARAM)progress);
}

UINT CGEDecompressorDlg::DecompressGameThread( LPVOID pParam )
{
	CGEDecompressorDlg* dlg = (CGEDecompressorDlg*)(pParam);
	CString gameNameStr = dlg->gameNameStr;
	CString strROMPath = dlg->strROMPath;
	bool genText = dlg->genText;

	GECompression compressed;
	compressed.SetPath(dlg->directory);

	RncDecoder rnccompressed;
	MIO0 mio0compressed;
	SupermanDecoder supermancompressed;
	MonacoDecoder monacocompressed;
	CVPK0Decoder vpk0compressed;
	YAY0 yay0compressed;
	YAZ0 yaz0compressed;
	ERZ erzcompressed;
	CEDLCompress edlcompressed;
	CCustomRoboCompress customrobocompressed;
	CODTCompress odtcompressed;
	CBlitzDecoder blitzcompressed;
	EASportsDecoder eagamescompressed;
	CFLA2Decoder fla2compressed;
	CBoltDecoder boltcompressed;
	CommandAndConquerDecoder commandconquercompressed;

	unsigned char* GameBuffer = NULL;
	try
	{
		if (gameNameStr == "Goldeneye")
		{
			int game = GetZLibGameName(gameNameStr);
			compressed.SetGame(game);
			int romSize = 0;
			CString folderPath;

			if (ReadROM(gameNameStr, strROMPath, GameBuffer, romSize, folderPath))
			{
				ReceivedNewROM(dlg, strROMPath, GameBuffer, romSize);
				for (unsigned long x = 0x0; x < (romSize - 0x100); x++)
				{	
					if (dlg->killThread)
						break;
					ToUpdateProgressBar(dlg, x, romSize);
					if ((GameBuffer[x] == 0x11) && (GameBuffer[x+1] == 0x72))
					{		
						CString tempLocation;
						int fileSizeCompressed = -1;
						CString type = "1172";
						int fileSizeUncompressed = 0;
						fileSizeUncompressed = DecompressZLibSpot(&compressed, genText, x, GameBuffer, romSize, game, folderPath, "", -1, tempLocation, fileSizeCompressed, type, x, false, 0);
						if (fileSizeUncompressed > 0)
						{
							AddRowData(dlg, x, fileSizeCompressed, fileSizeUncompressed, "", tempLocation, type);
						}
					}
				}
				
				delete [] GameBuffer;
			}
			
		}
		else if (gameNameStr == "Perfect Dark")
		{
			int game = GetZLibGameName(gameNameStr);
			compressed.SetGame(game);
			int romSize = 0;
			unsigned char* GameBuffer = NULL;
			CString folderPath;

			if (ReadROM(gameNameStr, strROMPath, GameBuffer, romSize, folderPath))
			{
				ReceivedNewROM(dlg, strROMPath, GameBuffer, romSize);
				for (unsigned long x = 0x0; x < (romSize - 0x100); x++)
				{	
					if (dlg->killThread)
						break;
					ToUpdateProgressBar(dlg, x, romSize);
					if ((GameBuffer[x] == 0x11) && (GameBuffer[x+1] == 0x73))
					{
						int trueFileSize = ((GameBuffer[x+2] << 16) | (GameBuffer[x+3] << 8) | GameBuffer[x+4]);
						if ((trueFileSize > 0xFFFFFF))
							continue;

						CString tempLocation;
						int fileSizeCompressed = -1;
						CString type = "1173";
						int fileSizeUncompressed = DecompressZLibSpot(&compressed, genText, x, GameBuffer, romSize, game, folderPath, "", trueFileSize, tempLocation, fileSizeCompressed, type, x, false, 0);
						if (fileSizeUncompressed > 0)
						{
							AddRowData(dlg, x, fileSizeCompressed, fileSizeUncompressed, "", tempLocation, type);
						}
					}
				}
				
				delete [] GameBuffer;
			}
			
		}
		else if (gameNameStr == "Banjo Kazooie")
		{
			int game = GetZLibGameName(gameNameStr);
			compressed.SetGame(game);
			int romSize = 0;
			unsigned char* GameBuffer = NULL;
			CString folderPath;

			if (ReadROM(gameNameStr, strROMPath, GameBuffer, romSize, folderPath))
			{
				ReceivedNewROM(dlg, strROMPath, GameBuffer, romSize);
				for (unsigned long x = 0x0; x < (romSize - 0x100); x++)
				{	
					if (dlg->killThread)
						break;
					ToUpdateProgressBar(dlg, x, romSize);
					if ((GameBuffer[x] == 0x11) && (GameBuffer[x+1] == 0x72)  && (GameBuffer[x+2] == 0x00))
					{
						int trueFileSize = trueFileSize = ((GameBuffer[x+2] << 24) | (GameBuffer[x+3] << 16) | (GameBuffer[x+4] << 8) | GameBuffer[x+5]);
						if ((trueFileSize > 0xFFFFFF))
							continue;

						CString tempLocation;
						int fileSizeCompressed = -1;
						CString type = "1172";
						int fileSizeUncompressed = DecompressZLibSpot(&compressed, genText, x, GameBuffer, romSize, game, folderPath, "", trueFileSize, tempLocation, fileSizeCompressed, type, x, false, 0);
						if (fileSizeUncompressed > 0)
						{
							AddRowData(dlg, x, fileSizeCompressed, fileSizeUncompressed, "", tempLocation, type);
						}
					}
				}
				
				delete [] GameBuffer;
			}
			
		}
		else if (gameNameStr == "Killer Instinct")
		{
			int game = GetZLibGameName(gameNameStr);
			compressed.SetGame(game);
			int romSize = 0;
			unsigned char* GameBuffer = NULL;
			CString folderPath;

			if (ReadROM(gameNameStr, strROMPath, GameBuffer, romSize, folderPath))
			{
				ReceivedNewROM(dlg, strROMPath, GameBuffer, romSize);
				for (unsigned long x = 0x0; x < (romSize - 0x100); x++)
				{	
					if (dlg->killThread)
						break;
					ToUpdateProgressBar(dlg, x, romSize);
					if ((GameBuffer[x] == 0x11) && (GameBuffer[x+1] == 0x72))
					{
						CString tempLocation;
						int fileSizeCompressed = -1;
						CString type = "1172";
						int fileSizeUncompressed = DecompressZLibSpot(&compressed, genText, x, GameBuffer, romSize, game, folderPath, "", -1, tempLocation, fileSizeCompressed, type, x, false, 0);
						if (fileSizeUncompressed > 0)
						{
							AddRowData(dlg, x, fileSizeCompressed, fileSizeUncompressed, "", tempLocation, type);
						}
					}
				}
				
				delete [] GameBuffer;
			}
			
		}
		else if (gameNameStr == "Resident Evil 2")
		{
			int game = GetZLibGameName(gameNameStr);
			compressed.SetGame(game);
			int romSize = 0;
			unsigned char* GameBuffer = NULL;
			CString folderPath;

			if (ReadROM(gameNameStr, strROMPath, GameBuffer, romSize, folderPath))
			{
				ReceivedNewROM(dlg, strROMPath, GameBuffer, romSize);
				for (unsigned long x = 0x0; x < (romSize - 0x100); x++)
				{	
					if (dlg->killThread)
						break;
					ToUpdateProgressBar(dlg, x, romSize);
					if ((GameBuffer[x] == 0x68) && (GameBuffer[x+1] == 0xDE))
					{
						CString tempLocation;
						int fileSizeCompressed = -1;
						CString type = "68DE";
						int fileSizeUncompressed = DecompressZLibSpot(&compressed, genText, x, GameBuffer, romSize, game, folderPath, "", -1, tempLocation, fileSizeCompressed, type, x, false, 0);
						if (fileSizeUncompressed > 0)
						{
							AddRowData(dlg, x, fileSizeCompressed, fileSizeUncompressed, "", tempLocation, type);
						}
					}
				}
				
				delete [] GameBuffer;
			}
			
		}
		else if ((gameNameStr == "Blast Corps") || (gameNameStr == "Donkey Kong 64 Kiosk") || (gameNameStr == "Donkey Kong 64"))
		{
			int game = GetZLibGameName(gameNameStr);
			compressed.SetGame(game);
			int romSize = 0;
			unsigned char* GameBuffer = NULL;
			CString folderPath;

			if (ReadROM(gameNameStr, strROMPath, GameBuffer, romSize, folderPath))
			{
				ReceivedNewROM(dlg, strROMPath, GameBuffer, romSize);
				for (unsigned long x = 0x0; x < (romSize - 0x100); x++)
				{	
					if (dlg->killThread)
						break;
					ToUpdateProgressBar(dlg, x, romSize);
					if (
						((GameBuffer[x] == 0x1F) && (GameBuffer[x+1] == 0x8B) && (GameBuffer[x+2] == 0x08) && (GameBuffer[x+3] == 0x0))
						|| 
						((GameBuffer[x] == 0x1F) && (GameBuffer[x+1] == 0x8B) && (GameBuffer[x+2] == 0x08) && (GameBuffer[x+3] == 0x08))
					)
					{
						CString internalName = "ZLib";
						
						if ((GameBuffer[x] == 0x1F) && (GameBuffer[x+1] == 0x8B) && (GameBuffer[x+2] == 0x08) && (GameBuffer[x+3] == 0x8))
						{
							int curSpot = x + 0xA;
							while (GameBuffer[curSpot] != 0x0)
							{
								CString tempAddStr;
								tempAddStr.Format("%c", GameBuffer[curSpot]);
								internalName = internalName + tempAddStr;
								curSpot++;
							}
						}
						CString tempLocation;
						int fileSizeCompressed = -1;
						CString type = "";
						int fileSizeUncompressed = DecompressZLibSpot(&compressed, genText, x, GameBuffer, romSize, game, folderPath, internalName, -1, tempLocation, fileSizeCompressed, type, x, false, 0);
						if (fileSizeUncompressed > 0)
						{
							AddRowData(dlg, x, fileSizeCompressed, fileSizeUncompressed, internalName, tempLocation, type);
						}
					}
				}
				
				delete [] GameBuffer;
			}		
			
		}
		else if (gameNameStr == "Banjo Tooie")
		{
			int game = GetZLibGameName(gameNameStr);
			compressed.SetGame(game);
			DecompressZLibFromTable(gameNameStr, dlg, strROMPath, 0x5188, 0x11A24 , 4, BANJOTOOIE, 0x12B24, 8, 4, 0);
			DecompressZLibFromTable(gameNameStr, dlg, strROMPath, 0x1E899B0, 0x1E8A77C , 4, BANJOTOOIE, 0x1E899B0, 0, 1, 0x10);
			
			return 0;
		}
		else if (gameNameStr == "Conker")
		{
			// start
			// brute force for now
			int game = GetZLibGameName(gameNameStr);
			compressed.SetGame(game);
			int romSize = 0;
			unsigned char* GameBuffer = NULL;
			CString folderPath;

			if (ReadROM(gameNameStr, strROMPath, GameBuffer, romSize, folderPath))
			{
				ReceivedNewROM(dlg, strROMPath, GameBuffer, romSize);

				if (GameBuffer[0x25] == 0x45) // ECTS
				{
					unsigned long start = 0x11B5858;
					unsigned long end = 0x11B5A78;

					for (int x = start; x < end; x += 8)
					{
						unsigned long offset = start + CharArrayToLong(&GameBuffer[x]);

						DecompressConkerFromTable(dlg, strROMPath, GameBuffer, romSize, offset, (offset + CharArrayToLong(&GameBuffer[offset])), CONKER, false, ((x - start) / 8));
					}

					start = 0xB802E0;
					end = 0xB803D8;

					for (int x = start; x < end; x += 8)
					{
						unsigned long offset = start + CharArrayToLong(&GameBuffer[x]);

						DecompressConkerFromTable(dlg, strROMPath, GameBuffer, romSize, offset, (offset + CharArrayToLong(&GameBuffer[offset])), CONKER, false, ((x - start) / 8));
					}
					
					for (unsigned long x = 0x138A9B0; x < 0x138B578; x+=8)
					{	
						if (dlg->killThread)
							break;

						unsigned long offset = (0x138A9B0 + CharArrayToLong(&GameBuffer[x]));
						CString mp3Name;
						mp3Name.Format("%s%06X.mp3", folderPath, offset);
						// write mp3s
						CString tempFullFileName;
						FILE* outFileTemp = fopen(mp3Name, "wb");

						unsigned long next;
						if (x == 0x138B570)
							next = 0x26335F0;
						else
							next = (0x138A9B0 + CharArrayToLong(&GameBuffer[x+0x8]));
						fwrite(&GameBuffer[offset], 1, (next - offset), outFileTemp);
						fclose(outFileTemp);

						AddRowData(dlg, offset, (next - offset), (next - offset), "", mp3Name, "MP3");
					}
				}
				else if ((GameBuffer[0x20] == 0x43) && (GameBuffer[0x2B] == 0x20)) // retail NTSC
				{
					int game = GetZLibGameName(gameNameStr);
					compressed.SetGame(game);
					DecompressConkerFromTable(dlg, strROMPath, GameBuffer, romSize, 0x029AE9E8, (0x029AE9E8 + 0x38), CONKER, false, 0x029AE9E8);

					unsigned long start = 0x117FE50;
					unsigned long end = 0x1180070;

					for (int x = start; x < end; x += 8)
					{
						unsigned long offset = start + CharArrayToLong(&GameBuffer[x]);

						DecompressConkerFromTable(dlg, strROMPath, GameBuffer, romSize, offset, (offset + 0x140), CONKER, false, ((x - start) / 8));
					}

					start = 0xAB1950;
					end = 0xAB1A38;

					for (int x = start; x < end; x += 8)
					{
						unsigned long offset = start + CharArrayToLong(&GameBuffer[x]);

						DecompressConkerFromTable(dlg, strROMPath, GameBuffer, romSize, offset, (offset + CharArrayToLong(&GameBuffer[offset])), CONKER, false, ((x - start) / 8));
					}
					
					

					for (unsigned long x = 0x1330478; x < 0x13312E8; x+=8)
					{	
						if (dlg->killThread)
							break;

						unsigned long offset = (0x1330478 + CharArrayToLong(&GameBuffer[x]));
						CString mp3Name;
						mp3Name.Format("%s%06X.mp3", folderPath, offset);
						// write mp3s
						CString tempFullFileName;
						FILE* outFileTemp = fopen(mp3Name, "wb");

						unsigned long next;
						if (x == 0x13312E0)
							next = 0x29AFD38;
						else
							next = (0x1330478 + CharArrayToLong(&GameBuffer[x+0x8]));
						fwrite(&GameBuffer[offset], 1, (next - offset), outFileTemp);
						fclose(outFileTemp);

						AddRowData(dlg, offset, (next - offset), (next - offset), "", mp3Name, "MP3");
					}
				}
				else if ((GameBuffer[0x20] == 0x43) && (GameBuffer[0x2B] == 0x44)) // Debug Conker
				{
					
				}
				
				ToUpdateProgressBar(dlg, 100, 100);
				delete [] GameBuffer;
			}
			
		}
		else if (gameNameStr == "Star Fox Adventures (GC) ZLB")
		{
			int game = GetZLibGameName(gameNameStr);
			compressed.SetGame(game);
			int romSize = 0;
			CString folderPath;

			if (ReadROM(gameNameStr, strROMPath, GameBuffer, romSize, folderPath))
			{
				ReceivedNewROM(dlg, strROMPath, GameBuffer, romSize);
				for (unsigned long x = 0x0; x < (romSize - 0x100); x++)
				{	
					if (dlg->killThread)
						break;
					ToUpdateProgressBar(dlg, x, romSize);
					if ((GameBuffer[x] == 0x5A) && (GameBuffer[x+1] == 0x4C) && (GameBuffer[x+2] == 0x42))
					{		
						CString tempLocation;
						int fileSizeCompressed = -1;
						CString type = "zlb";
						int fileSizeUncompressed = 0;
						fileSizeUncompressed = DecompressZLibSpot(&compressed, genText, x, GameBuffer, romSize, game, folderPath, "", -1, tempLocation, fileSizeCompressed, type, x, false, 0);
						if (fileSizeUncompressed > 0)
						{
							AddRowData(dlg, x, fileSizeCompressed, fileSizeUncompressed, "", tempLocation, type);
						}
					}
				}
				
				delete [] GameBuffer;
			}
			
		}
		else if (gameNameStr == "Top Gear Rally")
		{
			int game = GetZLibGameName(gameNameStr);
			compressed.SetGame(game);
			int romSize = 0;
			unsigned char* GameBuffer = NULL;
			CString folderPath;

			if (ReadROM(gameNameStr, strROMPath, GameBuffer, romSize, folderPath))
			{
				ReceivedNewROM(dlg, strROMPath, GameBuffer, romSize);
				
				int current = 0xAD400;
				int end = 0x1CB410;
			  int start = current;
				while(current<end)
				{	
					if (dlg->killThread)
						break;
					ToUpdateProgressBar(dlg, (current - start), (end - start));
					
					CString tempLocation;
					int fileSizeCompressed = CharArrayToLong(&GameBuffer[current]);
					int fileSizeCompressedOrig = fileSizeCompressed;
					CString type = "";
					unsigned long expectedFileSize = CharArrayToLong(&GameBuffer[current+4]);
					int fileSizeUncompressed = DecompressZLibSpot(&compressed, genText, current, GameBuffer, romSize, game, folderPath, "", expectedFileSize, tempLocation, fileSizeCompressed, type, current, false, 0);
					if (fileSizeUncompressed > 0)
					{
						AddRowData(dlg, current, fileSizeCompressed, fileSizeUncompressed, "", tempLocation, type);
					}

					unsigned long prevCurrent = current;
					current += fileSizeCompressedOrig;
					if ((current % 4) != 0)
						current = current + (4 - (current % 4));

					unsigned long tempAddress = CharArrayToLong(&GameBuffer[current]);
					while (tempAddress == 0x00000000)
					{
						current += 4;
						/*read in sample*/
						tempAddress = CharArrayToLong(&GameBuffer[current]);
					}

					if (current == 0xE0900) 
						current = 0x149C80;
				}
				delete [] GameBuffer;
			}	
		}
		else if ((gameNameStr == "Milo's Astro Lanes") || (gameNameStr == "Blues Brothers"))
		{
			int game = GetZLibGameName(gameNameStr);
			compressed.SetGame(game);
			int romSize = 0;
			unsigned char* GameBuffer = NULL;
			CString folderPath;

			if (ReadROM(gameNameStr, strROMPath, GameBuffer, romSize, folderPath))
			{
				ReceivedNewROM(dlg, strROMPath, GameBuffer, romSize);
				for (unsigned long x = 0x0; x < (romSize - 0x100); x++)
				{	
					if (dlg->killThread)
						break;
					ToUpdateProgressBar(dlg, x, romSize);
					if ((GameBuffer[x] == 0x78) && (GameBuffer[x+1] == 0x9C))
					{
						CString tempLocation;
						int fileSizeCompressed = -1;
						CString type = "789C";
						int fileSizeUncompressed = DecompressZLibSpot(&compressed, genText, x, GameBuffer, romSize, game, folderPath, "", -1, tempLocation, fileSizeCompressed, type, x, false, 0);
						if (fileSizeUncompressed > 0)
						{
							AddRowData(dlg, x, fileSizeCompressed, fileSizeUncompressed, "", tempLocation, type);
						}
					}
				}
				
				delete [] GameBuffer;
			}
			
		}
		else if ((gameNameStr == "Stunt Racer 64"))
		{
			int game = GetZLibGameName(gameNameStr);
			compressed.SetGame(game);
			int romSize = 0;
			unsigned char* GameBuffer = NULL;
			CString folderPath;

			if (ReadROM(gameNameStr, strROMPath, GameBuffer, romSize, folderPath))
			{
				ReceivedNewROM(dlg, strROMPath, GameBuffer, romSize);
				for (unsigned long x = 0x0; x < (romSize - 0x100); x++)
				{	
					if (dlg->killThread)
						break;
					ToUpdateProgressBar(dlg, x, romSize);
					if ((GameBuffer[x] == 0x78) && (GameBuffer[x+1] == 0xDA))
					{
						CString tempLocation;
						int fileSizeCompressed = -1;
						CString type = "78DA";
						int fileSizeUncompressed = DecompressZLibSpot(&compressed, genText, x, GameBuffer, romSize, game, folderPath, "", -1, tempLocation, fileSizeCompressed, type, x, false, 0);
						if (fileSizeUncompressed > 0)
						{
							AddRowData(dlg, x, fileSizeCompressed, fileSizeUncompressed, "", tempLocation, type);
						}
					}
				}
				
				delete [] GameBuffer;
			}
			
		}
		else if (gameNameStr == "Mission Impossible")
		{
			int game = GetZLibGameName(gameNameStr);
			compressed.SetGame(game);
			int romSize = 0;
			unsigned char* GameBuffer = NULL;
			CString folderPath;

			if (ReadROM(gameNameStr, strROMPath, GameBuffer, romSize, folderPath))
			{
				ReceivedNewROM(dlg, strROMPath, GameBuffer, romSize);
				for (unsigned long x = 0x0; x < (romSize - 0x100); x++)
				{	
					if (dlg->killThread)
						break;
					ToUpdateProgressBar(dlg, x, romSize);
					if ((GameBuffer[x] == 0x42) && (GameBuffer[x+1] == 0x75) && (GameBuffer[x+2] == 0x72) && (GameBuffer[x+3] == 0x70))
					{
						int numberBuffers = CharArrayToLong(&GameBuffer[x+4]);
						for (int r = 0; r < numberBuffers; r++)
						{
							if (GameBuffer[x + 0x8 + (r * 0x14) + 0x10] == 0x04)
							{
								CString tempLocation;
								int fileSizeCompressed = CharArrayToLong(&GameBuffer[x + 0x8 + (r * 0x14)]);
								unsigned long expectedSize = CharArrayToLong(&GameBuffer[x + 0x8 + 0x4 + (r * 0x14)]);
								CString type = "Burp";
								int fileSizeUncompressed = DecompressZLibSpot(&compressed, genText, CharArrayToLong(&GameBuffer[x + 0x8 + 0x4 + (r * 0x14)]), GameBuffer, romSize, game, folderPath, "", -1, tempLocation, fileSizeCompressed, type, CharArrayToLong(&GameBuffer[x + 0x8 + 0x4 + (r * 0x14)]), false, 0);
								if (fileSizeUncompressed > 0)
								{
									AddRowData(dlg, CharArrayToLong(&GameBuffer[x + 0x8 + 0x4 + (r * 0x14)]), fileSizeCompressed, fileSizeUncompressed, "", tempLocation, type);
								}
							}
						}
					}
				}
				
				delete [] GameBuffer;
			}
			
		}
		else if (gameNameStr == "Jet Force Gemini")
		{
			int game = GetZLibGameName(gameNameStr);
			compressed.SetGame(game);
			DecompressZLibFromTable(gameNameStr, dlg, strROMPath, 0xB79DC0, 0xB805E0 , 4, JFG, 0xB18A0, 0, 1, 0);
			DecompressZLibFromTable(gameNameStr, dlg, strROMPath, 0xC34310, 0xC34D58, 4, JFG, 0xB80610, 0, 1, 0);
			DecompressZLibFromTable(gameNameStr, dlg, strROMPath, 0xCD2CE0, 0xCD3198, 4, JFG, 0xCD31A0, 0, 1, 0);
			DecompressZLibFromTable(gameNameStr, dlg, strROMPath, 0x139B800, 0x139C624, 4, JFG, 0x139C630, 0, 1, 0);
			
			return 0;
		}
		else if (gameNameStr == "Diddy Kong Racing")
		{
			int game = GetZLibGameName(gameNameStr);
			compressed.SetGame(game);
			DecompressZLibFromTable(gameNameStr, dlg, strROMPath, 0x296CA0, 0x298288 , 4, DKR, 0xECD70, 0, 1, 0);
			DecompressZLibFromTable(gameNameStr, dlg, strROMPath, 0x383AA0, 0x383CCC , 4, DKR, 0x383CD0, 0, 1, 0); 
			DecompressZLibFromTable(gameNameStr, dlg, strROMPath, 0x3A5670, 0x3A5750 , 4, DKR, 0x3A5760, 0, 1, 0); 
			DecompressZLibFromTable(gameNameStr, dlg, strROMPath, 0x54F4B0, 0x54FACC , 4, DKR, 0x54FAD0, 0, 1, 0); 
			DecompressZLibFromTable(gameNameStr, dlg, strROMPath, 0x61FD80, 0x6204DC , 4, DKR, 0x6204E0, 0, 1, 0);
			
			return 0;
		}
		else if (gameNameStr == "JFG Kiosk")
		{
			int game = GetZLibGameName(gameNameStr);
			compressed.SetGame(game);
			DecompressZLibFromTable(gameNameStr, dlg, strROMPath, 0xB6A9F0, 0xB711D0 , 4, JFG, 0x2400, 0, 1, 0);
			DecompressZLibFromTable(gameNameStr, dlg, strROMPath, 0xC250E0, 0xC25B1C, 4, JFG, 0xB71200, 0, 1, 0);
			DecompressZLibFromTable(gameNameStr, dlg, strROMPath, 0xCC3390, 0xCC3848, 4, JFG, 0xCC3850, 0, 1, 0);
			DecompressZLibFromTable(gameNameStr, dlg, strROMPath, 0xFA65A0, 0xFA73B0, 4, JFG, 0xFA73C0, 0, 1, 0);
			
			return 0;
		}
		else if (gameNameStr == "Mickey's Speedway Racing")
		{
			int game = GetZLibGameName(gameNameStr);
			compressed.SetGame(game);
			DecompressZLibFromTable(gameNameStr, dlg, strROMPath, 0x58AFD0, 0x58DFDC , 4, MICKEYSPEEDWAY, 0x86780, 0, 1, 0);
			DecompressZLibFromTable(gameNameStr, dlg, strROMPath, 0x7925C0, 0x7942CC , 4, MICKEYSPEEDWAY, 0x58E000, 0, 1, 0); 
			DecompressZLibFromTable(gameNameStr, dlg, strROMPath, 0x7F6960, 0x7F6C08 , 4, MICKEYSPEEDWAY, 0x798530, 0, 1, 0); 
			DecompressZLibFromTable(gameNameStr, dlg, strROMPath, 0x8130F0, 0x8131A0 , 4, MICKEYSPEEDWAY, 0x8131B0, 0, 1, 0); 
			DecompressZLibFromTable(gameNameStr, dlg, strROMPath, 0x9B9AF0, 0x9B9E68 , 4, MICKEYSPEEDWAY, 0x9B9E70, 0, 1, 0); 
			
			return 0;
		}
		else if ((gameNameStr == "Turok 1")|| (gameNameStr == "Duke Nukem 64") || (gameNameStr == "Turok 2") || (gameNameStr == "Turok 3") || (gameNameStr == "Turok Rage Wars") || (gameNameStr == "Iggy's Reckin' Balls") || (gameNameStr == "Shadowman") || (gameNameStr == "NHL Breakaway 98") || (gameNameStr == "NHL Breakaway 99") || (gameNameStr == "PGA European Tour"))
		{
			//RNC1
			int game = 1; // placeholder
			int romSize = 0;
			unsigned char* GameBuffer = NULL;
			CString folderPath;

			if (ReadROM(gameNameStr, strROMPath, GameBuffer, romSize, folderPath))
			{
				ReceivedNewROM(dlg, strROMPath, GameBuffer, romSize);
				for (unsigned long x = 0x0; x < (romSize - 0x100); x++)
				{	
					if (dlg->killThread)
						break;
					ToUpdateProgressBar(dlg, x, romSize);
					if ((GameBuffer[x] == 0x52) && (GameBuffer[x+1] == 0x4E) && (GameBuffer[x+2] == 0x43) && (GameBuffer[x+3] == 0x01))
					{
						CString tempLocation;
						int fileSizeCompressed = -1;
						CString type = "";
						type.Format("RNC%X", GameBuffer[x+3]);
						int fileSizeUncompressed = DecompressRNCSpot(&rnccompressed, genText, x, GameBuffer, romSize, game, folderPath, "", -1, tempLocation, fileSizeCompressed, type, false, 0x0, "");
						if (fileSizeUncompressed > 0)
						{
							AddRowData(dlg, x, fileSizeCompressed, fileSizeUncompressed, "", tempLocation, type);
						}
					}
				}
				
				delete [] GameBuffer;
			}	
		}
		else if (gameNameStr == "Forsaken")
		{
			//RNX1
			int game = 1; // placeholder
			int romSize = 0;
			unsigned char* GameBuffer = NULL;
			CString folderPath;

			bool startedRNX = false;
			bool startedRNC = false;
			unsigned long rnxSpot = 0;

			int currentRNXCompressedSize = 0;
			int currentRNXUncompressedSize = 0;
			CString tempLocation;
			CString type = "";

			if (ReadROM(gameNameStr, strROMPath, GameBuffer, romSize, folderPath))
			{
				ReceivedNewROM(dlg, strROMPath, GameBuffer, romSize);
				int x = 0x0;
				while (x < (romSize - 0x100))
				{	
					if (dlg->killThread)
						break;
					ToUpdateProgressBar(dlg, x, romSize);
					if (
						((GameBuffer[x] == 0x52) && (GameBuffer[x+1] == 0x4E) && (GameBuffer[x+2] == 0x58) && (GameBuffer[x+3] == 0x01)))
					{
						if (startedRNX || startedRNC)
						{
							AddRowData(dlg, rnxSpot, currentRNXCompressedSize, currentRNXUncompressedSize, "", tempLocation, type);
							startedRNX = false;
							startedRNC = false;
						}

						int fileSizeCompressed = -1;
						
						type.Format("RNX%X", GameBuffer[x+3]);

						int fileSizeUncompressed = DecompressRNCSpot(&rnccompressed, genText, x, GameBuffer, romSize, game, folderPath, "", -1, tempLocation, fileSizeCompressed, type, startedRNX, x, "");

						startedRNX = true;
						rnxSpot = x;

						x += fileSizeCompressed;
						x += (8 - (fileSizeCompressed % 8));
						x += 0x10;

						currentRNXCompressedSize = fileSizeCompressed;
						currentRNXUncompressedSize = fileSizeUncompressed;
					}
					else if (
						((GameBuffer[x] == 0x52) && (GameBuffer[x+1] == 0x4E) && (GameBuffer[x+2] == 0x43) && (GameBuffer[x+3] == 0x01)))
					{
						int fileSizeCompressed = -1;

						int fileSizeUncompressed = 0;
						if (!startedRNX)
						{
							type.Format("RNC%X", GameBuffer[x+3]);
							currentRNXCompressedSize = 0;
							currentRNXUncompressedSize = 0;
							fileSizeUncompressed = DecompressRNCSpot(&rnccompressed, genText, x, GameBuffer, romSize, game, folderPath, "", -1, tempLocation, fileSizeCompressed, type, false, x, "");
							rnxSpot = x;
							startedRNC = true;
							currentRNXCompressedSize += fileSizeCompressed;
							currentRNXUncompressedSize += fileSizeUncompressed;
						}
						else
						{
							fileSizeUncompressed = DecompressRNCSpot(&rnccompressed, genText, x, GameBuffer, romSize, game, folderPath, "", -1, tempLocation, fileSizeCompressed, type, startedRNX, rnxSpot, "");
							currentRNXCompressedSize += fileSizeCompressed;
							currentRNXUncompressedSize += fileSizeUncompressed;
						}

						x += fileSizeCompressed;
						x += (8 - (fileSizeCompressed % 8));
						x += 0xC;	
					}
					else if ( // fill
						((GameBuffer[x] == 0x46) && (GameBuffer[x+1] == 0x49) && (GameBuffer[x+2] == 0x4C) && (GameBuffer[x+3] == 0x4C)))
					{
						unsigned short padAmount = CharArrayToShort(&GameBuffer[x+4]);
						unsigned char* outputDecompressed = new unsigned char[padAmount];
						for (int z = 0; z < padAmount; z++)
							outputDecompressed[z] = 0x00;

						CString internalName = "";
						
						WriteResult(genText, tempLocation, outputDecompressed, padAmount, true);

						x += 8;
						delete [] outputDecompressed;

						currentRNXCompressedSize += 8;
						currentRNXUncompressedSize += padAmount;

					}
					else if (
						((GameBuffer[x] == 0x00))
						)
						
					{
						if ((startedRNX) || (startedRNC))
						{
							AddRowData(dlg, rnxSpot, currentRNXCompressedSize, currentRNXUncompressedSize, "", tempLocation, type);
						}
						startedRNX = false;
						startedRNC = false;
						x++;
					}
					else if (
						((GameBuffer[x] == 0x45) && (GameBuffer[x+1] == 0x4E) && (GameBuffer[x+2] == 0x44) && (GameBuffer[x+3] == 0x21))
						)
					{
						if ((startedRNX) || startedRNC)
						{
							AddRowData(dlg, rnxSpot, currentRNXCompressedSize, currentRNXUncompressedSize, "", tempLocation, type);
						}

						startedRNX = false;
						startedRNC = false;
						x+=4;
						currentRNXCompressedSize += 4;
					}
					else
					{
						x++;
					}

				}

				if ((startedRNX) || (startedRNC))
				{
					AddRowData(dlg, rnxSpot, currentRNXCompressedSize, currentRNXUncompressedSize, "", tempLocation, type);
				}
				
				delete [] GameBuffer;
			}	
		}
		else if ((gameNameStr == "ECW Hardcore Revolution") || (gameNameStr == "NFL QB Club 2000") || (gameNameStr == "NFL QB Club 2001") || (gameNameStr == "NFL QB Club 98") || (gameNameStr == "NFL QB Club 99") || (gameNameStr == "WWF WarZone") || (gameNameStr == "WWF Attitude") || (gameNameStr == "All-Star Baseball 99") || (gameNameStr == "South Park Chefs Love Shack"))
		{
			//RNC1 + RNC81, 81 ignored for now
			int game = 1; // placeholder
			int romSize = 0;
			unsigned char* GameBuffer = NULL;
			CString folderPath;

			if (ReadROM(gameNameStr, strROMPath, GameBuffer, romSize, folderPath))
			{
				ReceivedNewROM(dlg, strROMPath, GameBuffer, romSize);
				for (unsigned long x = 0x0; x < (romSize - 0x100); x++)
				{	
					if (dlg->killThread)
						break;
					ToUpdateProgressBar(dlg, x, romSize);
					if ((GameBuffer[x] == 0x52) && (GameBuffer[x+1] == 0x4E) && (GameBuffer[x+2] == 0x43) && (GameBuffer[x+3] == 0x01))
					{
						CString tempLocation;
						int fileSizeCompressed = -1;
						CString type = "";
						type.Format("RNC%X", GameBuffer[x+3]);
						int fileSizeUncompressed = DecompressRNCSpot(&rnccompressed, genText, x, GameBuffer, romSize, game, folderPath, "", -1, tempLocation, fileSizeCompressed, type, false, 0x0, "");
						if (fileSizeUncompressed > 0)
						{
							AddRowData(dlg, x, fileSizeCompressed, fileSizeUncompressed, "", tempLocation, type);
						}
					}
				}
				
				delete [] GameBuffer;
			}
		}
		else if (gameNameStr == "Command and Conquer")
		{
			int romSize = 0;
			unsigned char* GameBuffer = NULL;
			CString folderPath;

			if (ReadROM(gameNameStr, strROMPath, GameBuffer, romSize, folderPath))
			{
				ReceivedNewROM(dlg, strROMPath, GameBuffer, romSize);
				for (unsigned long x = 0x0; x < (romSize - 0x100); x++)
				{	
					if (dlg->killThread)
						break;
					ToUpdateProgressBar(dlg, x, romSize);

					

					// SIGN.TXT file list
					if (
						(
						(GameBuffer[x] == 0x53) && (GameBuffer[x+1] == 0x49) && (GameBuffer[x+2] == 0x47) && (GameBuffer[x+3] == 0x4E)
						&& (GameBuffer[x+4] == 0x2E) && (GameBuffer[x+5] == 0x54) && (GameBuffer[x+6] == 0x58) && (GameBuffer[x+7] == 0x54)
						)
						||
						(
						(GameBuffer[x] == 0x57) && (GameBuffer[x+1] == 0x41) && (GameBuffer[x+2] == 0x56) && (GameBuffer[x+3] == 0x42)
						&& (GameBuffer[x+4] == 0x4F) && (GameBuffer[x+5] == 0x58) && (GameBuffer[x+6] == 0x2E) && (GameBuffer[x+7] == 0x49)
						)
					   )
					{
						while (GameBuffer[x] != 0x00)
						{
							if (dlg->killThread)
								break;

							int game = 1;

							unsigned long startData = x;

							while (GameBuffer[startData] != 0)
							{
								startData += 0x18;
							}

							if ((startData % 0x10) != 0)
								startData = startData + (0x10 - (startData % 0x10));

							int curSpot = x;

							CString internalName = "";

							while ((GameBuffer[curSpot] != 0x0) && (curSpot < (x + 12)))
							{
								CString tempAddStr;
								tempAddStr.Format("%c", GameBuffer[curSpot]);
								internalName = internalName + tempAddStr;
								curSpot++;
							}

							CString tempLocation;
							unsigned long dataOffset = CharArrayToLong(&GameBuffer[x + 0xC]);
							int fileSizeCompressed = CharArrayToLong(&GameBuffer[x + 0x14]) & 0xFFFFFF;
							int fileSizeDecompressed = CharArrayToLong(&GameBuffer[x + 0x10]);
							unsigned char compressionFormat = GameBuffer[x + 0x14];

							CString type = "";
							if (compressionFormat == 0)
								type = "Uncompressed";
							else if (compressionFormat == 0x11)
								type = "8Bit LZ";
							else if (compressionFormat == 0x22)
								type = "16Bit LZ";

							int fileSizeUncompressed = DecompressCommandConquerSpot(&commandconquercompressed, genText, startData + dataOffset, GameBuffer, romSize, game, folderPath, internalName, fileSizeDecompressed, tempLocation, fileSizeCompressed, type, false, 0x0, "", (compressionFormat & 0x3));
							if (fileSizeUncompressed > 0)
							{
								AddRowData(dlg, startData + dataOffset, fileSizeCompressed, fileSizeUncompressed, internalName, tempLocation, type);
							}

							x += 0x18;
						}
					}
				}
				
				delete [] GameBuffer;
			}
		}
		else if (gameNameStr == "Armorines")
		{
			//RNC2
			int game = 1; // placeholder
			int romSize = 0;
			unsigned char* GameBuffer = NULL;
			CString folderPath;

			if (ReadROM(gameNameStr, strROMPath, GameBuffer, romSize, folderPath))
			{
				ReceivedNewROM(dlg, strROMPath, GameBuffer, romSize);
				for (unsigned long x = 0x0; x < (romSize - 0x100); x++)
				{	
					if (dlg->killThread)
						break;
					ToUpdateProgressBar(dlg, x, romSize);
					if ((GameBuffer[x] == 0x52) && (GameBuffer[x+1] == 0x4E) && (GameBuffer[x+2] == 0x43) && (GameBuffer[x+3] == 0x02))
					{
						CString tempLocation;
						int fileSizeCompressed = -1;
						CString type = "";
						type.Format("RNC%X", GameBuffer[x+3]);
						int fileSizeUncompressed = DecompressRNCSpot(&rnccompressed, genText, x, GameBuffer, romSize, game, folderPath, "", -1, tempLocation, fileSizeCompressed, type, false, 0x0, "");
						if (fileSizeUncompressed > 0)
						{
							AddRowData(dlg, x, fileSizeCompressed, fileSizeUncompressed, "", tempLocation, type);
						}
					}
				}
				
				delete [] GameBuffer;
			}	
		}
		else if (gameNameStr == "South Park")
		{
			//RNC1/2
			int game = 1; // placeholder
			int romSize = 0;
			unsigned char* GameBuffer = NULL;
			CString folderPath;

			if (ReadROM(gameNameStr, strROMPath, GameBuffer, romSize, folderPath))
			{
				ReceivedNewROM(dlg, strROMPath, GameBuffer, romSize);
				for (unsigned long x = 0x0; x < (romSize - 0x100); x++)
				{	
					if (dlg->killThread)
						break;
					ToUpdateProgressBar(dlg, x, romSize);
					if (
						((GameBuffer[x] == 0x52) && (GameBuffer[x+1] == 0x4E) && (GameBuffer[x+2] == 0x43) && (GameBuffer[x+3] == 0x01))
						||
						((GameBuffer[x] == 0x52) && (GameBuffer[x+1] == 0x4E) && (GameBuffer[x+2] == 0x43) && (GameBuffer[x+3] == 0x02)))
					{
						CString tempLocation;
						int fileSizeCompressed = -1;
						CString type = "";
						type.Format("RNC%X", GameBuffer[x+3]);
						int fileSizeUncompressed = DecompressRNCSpot(&rnccompressed, genText, x, GameBuffer, romSize, game, folderPath, "", -1, tempLocation, fileSizeCompressed, type, false, 0x0, "");
						if (fileSizeUncompressed > 0)
						{
							AddRowData(dlg, x, fileSizeCompressed, fileSizeUncompressed, "", tempLocation, type);
						}
					}
				}
				
				delete [] GameBuffer;
			}	
		}
		else if ((gameNameStr == "AeroFighter") || (gameNameStr == "Beetle Adventure Racing") || (gameNameStr == "Body Harvest") || (gameNameStr == "Duck Dodgers") || (gameNameStr == "F-1 World Grand Prix")|| (gameNameStr == "F-1 World Grand Prix II (E)")|| (gameNameStr == "F-ZERO Expansion Kit 64DD")|| (gameNameStr == "F-ZERO X")|| (gameNameStr == "Indy Racing 2000")|| (gameNameStr == "Lt. Duck Dodgers (Prototype)")|| (gameNameStr == "MarioKart64")|| (gameNameStr == "Pilotwings 64")|| (gameNameStr == "StarFox64")|| (gameNameStr == "Super Mario 64")|| (gameNameStr == "WaveRace"))
		{
			int game = 1; // placeholder
			int romSize = 0;
			unsigned char* GameBuffer = NULL;
			CString folderPath;

			if (ReadROM(gameNameStr, strROMPath, GameBuffer, romSize, folderPath))
			{
				ReceivedNewROM(dlg, strROMPath, GameBuffer, romSize);
				for (unsigned long x = 0x0; x < (romSize - 0x100); x++)
				{	
					if (dlg->killThread)
						break;
					ToUpdateProgressBar(dlg, x, romSize);
					if ((GameBuffer[x] == 0x4D) && (GameBuffer[x+1] == 0x49) && (GameBuffer[x+2] == 0x4F) && (GameBuffer[x+3] == 0x30))
					{
						CString tempLocation;
						int fileSizeCompressed = -1;
						CString type = "MIO0";
						int fileSizeUncompressed = DecompressMIO0Spot(&mio0compressed, genText, x, GameBuffer, romSize, game, folderPath, "", -1, tempLocation, fileSizeCompressed, type);
						if (fileSizeUncompressed > 0)
						{
							AddRowData(dlg, x, fileSizeCompressed, fileSizeUncompressed, "", tempLocation, type);
						}
					}
				}
				
				delete [] GameBuffer;
			}	
		}
		else if ((gameNameStr == "Starcraft") || (gameNameStr == "Bassmasters 2000") || (gameNameStr == "Ms PacMan") || (gameNameStr == "Power Rangers"))
		{
			int game = 1; // placeholder
			int romSize = 0;
			unsigned char* GameBuffer = NULL;
			CString folderPath;

			if (ReadROM(gameNameStr, strROMPath, GameBuffer, romSize, folderPath))
			{
				ReceivedNewROM(dlg, strROMPath, GameBuffer, romSize);
				for (unsigned long x = 0x0; x < (romSize - 0x100); x++)
				{	
					if (dlg->killThread)
						break;
					ToUpdateProgressBar(dlg, x, romSize);
					if ((GameBuffer[x] == 0x42) && (GameBuffer[x+1] == 0x4F) && (GameBuffer[x+2] == 0x4C) && (GameBuffer[x+3] == 0x54))
					{
						int bank = 0;
						for (int currentSpot = (x+0x10); currentSpot < (x + CharArrayToLong(&GameBuffer[x+0x18])); currentSpot += 0x10)
						{
							unsigned char noneCompressed = (GameBuffer[currentSpot] & 0x08);
							int numEntries = CharArrayToLong(&GameBuffer[currentSpot]) & 0xFFFFFF;
							unsigned long subSize = CharArrayToLong(&GameBuffer[currentSpot+4]);
							unsigned long subAddress = CharArrayToLong(&GameBuffer[currentSpot+8]) + x;

							for (int entry = 0; entry < numEntries; entry++)
							{
								int subEntryAddress = subAddress + (entry * 0x10);
								unsigned long decompressedSize = CharArrayToLong(&GameBuffer[subEntryAddress+4]);
								unsigned long decompressedAddress = x + CharArrayToLong(&GameBuffer[subEntryAddress+8]);

								if (noneCompressed || (GameBuffer[subEntryAddress] == 0x08))
								{
									CString tempLocation;
									tempLocation.Format("%s%06X_B%02X_%02X.bin", folderPath, subEntryAddress, bank, entry);

									WriteResult(dlg->genText, tempLocation, &GameBuffer[subEntryAddress], decompressedSize, false);

									AddRowData(dlg, subEntryAddress, decompressedSize, decompressedSize, "", tempLocation, "Uncompressed");
								}
								else
								{
									CString tempLocation;
									int fileSizeCompressed = -1;
									CString type = "Bolt";
									int fileSizeUncompressed = DecompressBOLTSpot(&boltcompressed, genText, decompressedAddress, GameBuffer, romSize, game, folderPath, "", decompressedSize, tempLocation, fileSizeCompressed, type, bank, entry);
									if (fileSizeUncompressed > 0)
									{
										AddRowData(dlg, x, fileSizeCompressed, fileSizeUncompressed, "", tempLocation, type);
									}
								}
							}

							bank++;
						}
					}
				}
				
				delete [] GameBuffer;
			}	
		}
		else if (gameNameStr == "Superman")
		{
			int game = 1; // placeholder
			int romSize = 0;
			unsigned char* GameBuffer = NULL;
			CString folderPath;

			if (ReadROM(gameNameStr, strROMPath, GameBuffer, romSize, folderPath))
			{
				ReceivedNewROM(dlg, strROMPath, GameBuffer, romSize);
				for (unsigned long x = 0x0; x < (romSize - 0x100); x++)
				{	
					if (dlg->killThread)
						break;
					ToUpdateProgressBar(dlg, x, romSize);
					if ((GameBuffer[x] == 0x9D) && (GameBuffer[x+1] == 0x89) && (GameBuffer[x+2] == 0x64) && (GameBuffer[x+3] == 0x6C))
					{
						int fileSizeCompressed = -1;

						int expectedSizeUncompressed = supermancompressed.header(&GameBuffer[x-4], fileSizeCompressed);
						CString tempLocation;
						
						CString type = "dlz";
						int fileSizeUncompressed = DecompressSupermanSpot(&supermancompressed, genText, (x+0xD), GameBuffer, romSize, game, folderPath, "", expectedSizeUncompressed, tempLocation, fileSizeCompressed, type);
						if (fileSizeUncompressed > 0)
						{
							AddRowData(dlg, x, fileSizeCompressed, fileSizeUncompressed, "", tempLocation, type);
						}
					}
				}
				
				delete [] GameBuffer;
			}	
		}
		else if (gameNameStr == "Glover") // Frogger 2 also
		{
			int game = 1; // placeholder
			int romSize = 0;
			unsigned char* GameBuffer = NULL;
			CString folderPath;

			if (ReadROM(gameNameStr, strROMPath, GameBuffer, romSize, folderPath))
			{
				ReceivedNewROM(dlg, strROMPath, GameBuffer, romSize);
				for (unsigned long x = 0x0; x < (romSize - 0x100); x++)
				{	
					if (dlg->killThread)
						break;
					ToUpdateProgressBar(dlg, x, romSize);
					if ((GameBuffer[x] == 0x46) && (GameBuffer[x+1] == 0x4C) && (GameBuffer[x+2] == 0x41) && (GameBuffer[x+3] == 0x32))
					{
						int fileSizeCompressed = -1;

						int expectedSizeUncompressed = Flip32Bit(CharArrayToLong(&GameBuffer[x+4]));
						CString tempLocation;
						
						CString type = "fla2";
						int fileSizeUncompressed = DecompressFLA2Spot(&fla2compressed, genText, x+8, GameBuffer, romSize, game, folderPath, "", expectedSizeUncompressed, tempLocation, fileSizeCompressed, type);
						if (fileSizeUncompressed > 0)
						{
							AddRowData(dlg, x, fileSizeCompressed, fileSizeUncompressed, "", tempLocation, type);
						}
					}
				}
				
				delete [] GameBuffer;
			}	
		}
		else if (gameNameStr == "Monaco Grand Prix")
		{
			int game = 1; // placeholder
			int romSize = 0;
			unsigned char* GameBuffer = NULL;
			CString folderPath;

			if (ReadROM(gameNameStr, strROMPath, GameBuffer, romSize, folderPath))
			{
				ReceivedNewROM(dlg, strROMPath, GameBuffer, romSize);
				for (unsigned long x = 0x0; x < (romSize - 0x100); x++)
				{	
					if (dlg->killThread)
						break;
					ToUpdateProgressBar(dlg, x, romSize);
					if ((GameBuffer[x] == 0x53) && (GameBuffer[x+1] == 0x31) && (GameBuffer[x+2] == 0x00))
					{
						int fileSizeCompressed = -1;

						int expectedSizeUncompressed = monacocompressed.header(&GameBuffer[x-4], fileSizeCompressed);
						CString tempLocation;
						
						CString type = "TBR8";
						int fileSizeUncompressed = DecompressMonacoSpot(&monacocompressed, genText, (x+0xD), GameBuffer, romSize, game, folderPath, "", expectedSizeUncompressed, tempLocation, fileSizeCompressed, type);
						if (fileSizeUncompressed > 0)
						{
							AddRowData(dlg, x, fileSizeCompressed, fileSizeUncompressed, "", tempLocation, type);
						}
					}
				}
				
				delete [] GameBuffer;
			}	
		}
		else if ((gameNameStr == "1080Snow") || (gameNameStr == "BakuBomberman2")|| (gameNameStr == "Battlezone Black Dogs")|| (gameNameStr == "Bomberman 64 Second Attack")|| (gameNameStr == "Disney's Donald Duck - Goin' Quackers")|| (gameNameStr == "Doraemon 2")|| (gameNameStr == "Fushigi no Dungeon")|| (gameNameStr == "Kira to Kaiketsu")|| (gameNameStr == "Neon Genesis Evangelion")|| (gameNameStr == "Nushi Tsuri 64")|| (gameNameStr == "Paper Mario")|| (gameNameStr == "Parlor Pro 64")|| (gameNameStr == "Pokemon Stadium")|| (gameNameStr == "Pokemon Stadium 2")|| (gameNameStr == "Rayman 2")|| (gameNameStr == "Scooby-Doo")|| (gameNameStr == "Shadowgate 64")|| (gameNameStr == "Tonic Trouble")|| (gameNameStr == "Tsumi to Batsu")|| (gameNameStr == "Zool"))
		{
			int game = 1; // placeholder
			int romSize = 0;
			unsigned char* GameBuffer = NULL;
			CString folderPath;

			if (ReadROM(gameNameStr, strROMPath, GameBuffer, romSize, folderPath))
			{
				ReceivedNewROM(dlg, strROMPath, GameBuffer, romSize);
				for (unsigned long x = 0x00147620; x < (romSize - 0x100); x++)
				{	
					if (dlg->killThread)
						break;
					ToUpdateProgressBar(dlg, x, romSize);
					if ((GameBuffer[x] == 0x59) && (GameBuffer[x+1] == 0x61) && (GameBuffer[x+2] == 0x79) && 
						((GameBuffer[x+3] == 0x30) || (GameBuffer[x+3] == 0x31))
						)
					{
						CString tempLocation;
						int fileSizeCompressed = -1;
						CString type;
						type.Format("YAY%c", GameBuffer[x+3]);
						int fileSizeUncompressed = DecompressYAY0Spot(&yay0compressed, genText, x, GameBuffer, romSize, game, folderPath, "", -1, tempLocation, fileSizeCompressed, type);
						if (fileSizeUncompressed > 0)
						{
							AddRowData(dlg, x, fileSizeCompressed, fileSizeUncompressed, "", tempLocation, type);
						}
					}
				}
				
				delete [] GameBuffer;
			}	
		}
		else if (gameNameStr == "SSB")
		{
			int game = 1; // placeholder
			int romSize = 0;
			unsigned char* GameBuffer = NULL;
			CString folderPath;


			if (ReadROM(gameNameStr, strROMPath, GameBuffer, romSize, folderPath))
			{
				ReceivedNewROM(dlg, strROMPath, GameBuffer, romSize);

				unsigned long start = 0x1AC870;
				unsigned long end = 0x1B2C6C;

				if (GameBuffer[0x3E] == 0x45)
				{ 
					// NTSC default
				}
				else if (GameBuffer[0x3E] == 0x4A)
				{ 
					// JAP
					start = 0x1ACAF0;
					end = 0x1B2DC0;
				}
				else if (GameBuffer[0x3E] == 0x55)
				{ 
					// AUS
					start = 0x1AD4F0;
					end = 0x1B38F8;
				}
				else if (GameBuffer[0x3E] == 0x50)
				{ 
					// PAL
					start = 0x262E20;
					end = 0x2692E8;
				}

				for (int x = start; x < end; x+=0xC)
				{
					/*# Entry format:	0xC each 
					# 0x0	4	offset to data from end of table [1B2C6C]; compressed if negative 
					# 0x4	2	-1 if not present 
					#	 offset in words in decompressed file for filetable (*4 for actual) 
					# 0x6	2	compressed size in words (*4 for actual) 
					# 0x8	2	-1 if not present 
					#	 offset in words in decompressed file for resource data (*4 for actual) 
					# 0xA	2	decompressed size in words (*4 for actual) */

					signed long offset = CharArrayToLong(&GameBuffer[x]);

					int compressedSize = CharArrayToShort(&GameBuffer[x+6]) * 4;
					int decompressedSize = CharArrayToShort(&GameBuffer[x+0xA]) * 4;

					if (GameBuffer[x] & 0x80) // compressed
					{
						offset = (offset & (0xFFFFFFF)) + end;
						if (dlg->killThread)
							break;
						ToUpdateProgressBar(dlg, x / (end - start), x);
						
						CString tempLocation;
						CString type = "VPK0";

						unsigned long expectedSize = CharArrayToLong(&GameBuffer[offset+4]);

						int fileSizeUncompressed = DecompressVPK0Spot(&vpk0compressed, genText, offset+8, GameBuffer, romSize, game, folderPath, "", decompressedSize, tempLocation, compressedSize, type);
						if (fileSizeUncompressed > 0)
						{
							AddRowData(dlg, offset+8, compressedSize, decompressedSize, "", tempLocation, type);
						}
					}
					else
					{
						offset = offset + end;
						if (dlg->killThread)
							break;
						ToUpdateProgressBar(dlg, x / (end - start), x);
						
						CString tempLocation;
						CString type = "bin";

						tempLocation.Format("%s%06X.bin", folderPath, offset);

						WriteResult(genText, tempLocation, &GameBuffer[offset], decompressedSize, false);

						AddRowData(dlg, offset, compressedSize, decompressedSize, "", tempLocation, type);
					}
				}
				
				delete [] GameBuffer;
			}	
		}
		else if (gameNameStr == "Pokemon Snap")
		{
			int game = 1; // placeholder
			int romSize = 0;
			unsigned char* GameBuffer = NULL;
			CString folderPath;


			if (ReadROM(gameNameStr, strROMPath, GameBuffer, romSize, folderPath))
			{
				ReceivedNewROM(dlg, strROMPath, GameBuffer, romSize);

				unsigned long addressList[0x3];
				unsigned long stopList[0x3];


				if (GameBuffer[0x3E] == 0x45)
				{ 
					// NTSC default
					if (GameBuffer[0x3D] == 0x46) // Snap
					{
						addressList[0] = 0xA0F830;
						addressList[1] = 0xAA0B80;
						addressList[2] = 0xAAA610;
						stopList[0] = 0xA5CC50;
						stopList[1] = 0xAA18E0;
						stopList[2] = 0xAAA660;
					}
					else // Snap Station
					{
						addressList[0] = 0xA0FA60;
						addressList[1] = 0xAA1430;
						addressList[2] = 0xAAAEC0;
						stopList[0] = 0xA5D500;
						stopList[1] = 0xAA2190;
						stopList[2] = 0xAAAF10;
					}
				}
				else if (GameBuffer[0x3E] == 0x44) //G
				{ 
					addressList[0] = 0xA0A170;
					addressList[1] = 0xA9B970;
					addressList[2] = 0xAA5970;
					stopList[0] = 0xA58080;
					stopList[1] = 0xA9BEE0;
					stopList[2] = 0xAA59C0;
				}
				else if (GameBuffer[0x3E] == 0x46) //F
				{ 
					addressList[0] = 0xA09670;
					addressList[1] = 0xA9BEA0;
					addressList[2] = 0xAA5930;
					stopList[0] = 0xA58030;
					stopList[1] = 0xA9CC00;
					stopList[2] = 0xAA5980;
				}
				else if (GameBuffer[0x3E] == 0x46) //I
				{ 
					addressList[0] = 0xA09FB0;
					addressList[1] = 0xA9D140;
					addressList[2] = 0xAA6BD0;
					stopList[0] = 0xA592E0;
					stopList[1] = 0xA9DEA0;
					stopList[2] = 0xAA6C20;
				}
				else if (GameBuffer[0x3E] == 0x4A) //J
				{ 
					addressList[0] = 0xA32A20;
					addressList[1] = 0xAC8810;
					addressList[2] = 0xAD2220;
					stopList[0] = 0xA848C0;
					stopList[1] = 0xAC9570;
					stopList[2] = 0xAD2270;
				}
				else if (GameBuffer[0x3E] == 0x50) //P
				{ 
					addressList[0] = 0xA09770;
					addressList[1] = 0xA9B4A0;
					addressList[2] = 0xAA4F30;
					stopList[0] = 0xA576A0;
					stopList[1] = 0xA9C200;
					stopList[2] = 0xAA4F80;
				}
				else if (GameBuffer[0x3E] == 0x53) //S
				{ 
					addressList[0] = 0xA09600;
					addressList[1] = 0xA9CA40;
					addressList[2] = 0xAA64D0;
					stopList[0] = 0xA58BD0;
					stopList[1] = 0xA9D7A0;
					stopList[2] = 0xAA6520;
				}
				else if (GameBuffer[0x3E] == 0x55) //U (Aus)
				{ 
					addressList[0] = 0xA0F730;
					addressList[1] = 0xAA0BB0;
					addressList[2] = 0xAAA640;
					stopList[0] = 0xA5CB60;
					stopList[1] = 0xAA1910;
					stopList[2] = 0xAAA690;
				}

				for (int x = 0; x < 3; x++)
				{
					signed long offset = addressList[x];

					int compressedSize = stopList[x] - addressList[x];

					if (dlg->killThread)
						break;
					ToUpdateProgressBar(dlg, x / 3, x);
					
					CString tempLocation;
					CString type = "VPK0";

					unsigned long expectedSize = CharArrayToLong(&GameBuffer[offset+4]);

					int fileSizeUncompressed = DecompressVPK0Spot(&vpk0compressed, genText, offset+8, GameBuffer, romSize, game, folderPath, "", expectedSize, tempLocation, compressedSize, type);
					if (fileSizeUncompressed > 0)
					{
						AddRowData(dlg, offset, compressedSize, expectedSize, "", tempLocation, type);
					}

				}
				delete [] GameBuffer;
			}	
		}
		else if (gameNameStr == "Shigesato Itoi Bass Fishing")
		{
			int game = 1; // placeholder
			int romSize = 0;
			unsigned char* GameBuffer = NULL;
			CString folderPath;


			if (ReadROM(gameNameStr, strROMPath, GameBuffer, romSize, folderPath))
			{
				ReceivedNewROM(dlg, strROMPath, GameBuffer, romSize);

				unsigned long addressList[0x1];
				unsigned long stopList[0x1];

				addressList[0] = 0x4E8170;
				stopList[0] = 0x590A90;
					
				for (int x = 0; x < 1; x++)
				{
					signed long offset = addressList[x];

					int compressedSize = stopList[x] - addressList[x];

					if (dlg->killThread)
						break;
					ToUpdateProgressBar(dlg, x / 1, x);
					
					CString tempLocation;
					CString type = "VPK0";

					unsigned long expectedSize = CharArrayToLong(&GameBuffer[offset+4]);

					int fileSizeUncompressed = DecompressVPK0Spot(&vpk0compressed, genText, offset+8, GameBuffer, romSize, game, folderPath, "", expectedSize, tempLocation, compressedSize, type);
					if (fileSizeUncompressed > 0)
					{
						AddRowData(dlg, offset, compressedSize, expectedSize, "", tempLocation, type);
					}

				}
				delete [] GameBuffer;
			}	
		}
		else if ((gameNameStr == "Tony Hawk Pro Skater 1") || (gameNameStr == "Tony Hawk Pro Skater 2") || (gameNameStr == "Tony Hawk Pro Skater 3"))
		{
			// ERZ1/ERZ2
			int game = 1; // placeholder
			int romSize = 0;
			unsigned char* GameBuffer = NULL;
			CString folderPath;

			if (ReadROM(gameNameStr, strROMPath, GameBuffer, romSize, folderPath))
			{
				ReceivedNewROM(dlg, strROMPath, GameBuffer, romSize);
				for (unsigned long x = 0x0; x < (romSize - 0x100); x++)
				{	
					if (dlg->killThread)
						break;
					ToUpdateProgressBar(dlg, x, romSize);
					if (
						((GameBuffer[x] == 0x45) && (GameBuffer[x+1] == 0x52) && (GameBuffer[x+2] == 0x5A) && (GameBuffer[x+3] == 0x01))
						||
						((GameBuffer[x] == 0x45) && (GameBuffer[x+1] == 0x52) && (GameBuffer[x+2] == 0x5A) && (GameBuffer[x+3] == 0x02)))
					{
						CString tempLocation;
						int fileSizeCompressed = CharArrayToLong(&GameBuffer[x + 8]);
						int expectedSize = CharArrayToLong(&GameBuffer[x + 4]);
						CString type = "";
						int fileSizeUncompressed = DecompressERZSpot(&erzcompressed, genText, x, GameBuffer, romSize, game, folderPath, "", expectedSize, tempLocation, fileSizeCompressed, type);
						if (fileSizeUncompressed > 0)
						{
							AddRowData(dlg, x, fileSizeCompressed, fileSizeUncompressed, "", tempLocation, type);
						}
					}
				}
				
				delete [] GameBuffer;
			}
		}
		else if ((gameNameStr == "Monster Truck Madness") || (gameNameStr == "Spiderman"))
		{
			// ERZ2
			int game = 1; // placeholder
			int romSize = 0;
			unsigned char* GameBuffer = NULL;
			CString folderPath;

			if (ReadROM(gameNameStr, strROMPath, GameBuffer, romSize, folderPath))
			{
				ReceivedNewROM(dlg, strROMPath, GameBuffer, romSize);
				for (unsigned long x = 0x0; x < (romSize - 0x100); x++)
				{	
					if (dlg->killThread)
						break;
					ToUpdateProgressBar(dlg, x, romSize);
					if (
						((GameBuffer[x] == 0x45) && (GameBuffer[x+1] == 0x52) && (GameBuffer[x+2] == 0x5A) && (GameBuffer[x+3] == 0x02))
						)
					{
						CString tempLocation;
						int fileSizeCompressed = CharArrayToLong(&GameBuffer[x + 8]);
						int expectedSize = CharArrayToLong(&GameBuffer[x + 4]);
						CString type = "";
						int fileSizeUncompressed = DecompressERZSpot(&erzcompressed, genText, x, GameBuffer, romSize, game, folderPath, "", expectedSize, tempLocation, fileSizeCompressed, type);
						if (fileSizeUncompressed > 0)
						{
							AddRowData(dlg, x, fileSizeCompressed, fileSizeUncompressed, "", tempLocation, type);
						}
					}
				}
				
				delete [] GameBuffer;
			}
		}
		else if ((gameNameStr == "Doubutsu no Mori") || (gameNameStr == "MajorasMask") || (gameNameStr == "OoT"))
		{
			int game = 1; // placeholder
			int romSize = 0;
			unsigned char* GameBuffer = NULL;
			CString folderPath;

			if (ReadROM(gameNameStr, strROMPath, GameBuffer, romSize, folderPath))
			{
				ReceivedNewROM(dlg, strROMPath, GameBuffer, romSize);
				for (unsigned long x = 0x0; x < (romSize - 0x100); x++)
				{	
					if (dlg->killThread)
						break;
					ToUpdateProgressBar(dlg, x, romSize);
					if ((GameBuffer[x] == 0x59) && (GameBuffer[x+1] == 0x61) && (GameBuffer[x+2] == 0x7A) && (GameBuffer[x+3] == 0x30))
					{
						CString tempLocation;
						int fileSizeCompressed = -1;
						CString type = "YAZ0";
						int fileSizeUncompressed = DecompressYAZ0Spot(&yaz0compressed, genText, x, GameBuffer, romSize, game, folderPath, "", -1, tempLocation, fileSizeCompressed, type);
						if (fileSizeUncompressed > 0)
						{
							AddRowData(dlg, x, fileSizeCompressed, fileSizeUncompressed, "", tempLocation, type);
						}
					}
				}
				
				delete [] GameBuffer;
			}	
		}
		else if ((gameNameStr == "40 Winks")|| (gameNameStr == "Cruis'n World")|| (gameNameStr == "Duke Nukem - ZER0 H0UR")|| (gameNameStr == "Hydro Thunder")|| (gameNameStr == "Mortal Kombat 4")|| (gameNameStr == "NBA Showtime")|| (gameNameStr == "Tarzan")|| (gameNameStr == "TWINE")|| (gameNameStr == "War Gods"))
		{
			int game = 1; // placeholder
			int romSize = 0;
			unsigned char* GameBuffer = NULL;
			CString folderPath;

			if (ReadROM(gameNameStr, strROMPath, GameBuffer, romSize, folderPath))
			{
				ReceivedNewROM(dlg, strROMPath, GameBuffer, romSize);
				FILE* in = fopen(strROMPath, "rb");
				for (unsigned long x = 0x0; x < (romSize - 0x100); x++)
				{	
					if (dlg->killThread)
						break;
					ToUpdateProgressBar(dlg, x, romSize);
					if ((GameBuffer[x] == 0x45) && (GameBuffer[x+1] == 0x44) && (GameBuffer[x+2] == 0x4C))
					{
						CString tempLocation;
						int fileSizeCompressed = -1;
						CString type = "";
						int fileSizeUncompressed = DecompressEDLSpot(&edlcompressed, genText, x, in, romSize, game, folderPath, "", -1, tempLocation, fileSizeCompressed, type);
						if (fileSizeUncompressed > 0)
						{
							AddRowData(dlg, x, fileSizeCompressed, fileSizeUncompressed, "", tempLocation, type);
						}
					}
				}
				fclose(in);
				delete [] GameBuffer;
			}
		}
		else if (gameNameStr == "NHL 99")
		{
			int game = 1; // placeholder
			int romSize = 0;
			unsigned char* GameBuffer = NULL;
			CString folderPath;

			if (ReadROM(gameNameStr, strROMPath, GameBuffer, romSize, folderPath))
			{
				ReceivedNewROM(dlg, strROMPath, GameBuffer, romSize);
				for (unsigned long x = 0; x < (romSize - 0x100); x++)
				{	
					if (dlg->killThread)
						break;
					ToUpdateProgressBar(dlg, x, romSize);
					if ((GameBuffer[x] == 0x10) && (GameBuffer[x+1] == 0xFB))
					{
						CString tempLocation;
						int fileSizeCompressed = -1;
						CString type = "10FB";
						int fileSizeUncompressed = DecompressEAGamesSpot(&eagamescompressed, genText, x, GameBuffer, romSize, game, folderPath, "", -1, tempLocation, fileSizeCompressed, type);
						if (fileSizeUncompressed > 0)
						{
							AddRowData(dlg, x, fileSizeCompressed, fileSizeUncompressed, "", tempLocation, type);
						}
					}
				}
				
				delete [] GameBuffer;
			}
		}
		else if ((gameNameStr == "Custom Robo") || (gameNameStr == "Custom Robo V2"))
		{
			int game = 1; // placeholder
			int romSize = 0;
			unsigned char* GameBuffer = NULL;
			CString folderPath;

			if (ReadROM(gameNameStr, strROMPath, GameBuffer, romSize, folderPath))
			{
				ReceivedNewROM(dlg, strROMPath, GameBuffer, romSize);
				FILE* source = fopen(strROMPath, "rb");

				int current = 0;
				int end = romSize;

				fseek(source,0x3D,SEEK_SET);
			  switch(fgetc(source))
				{/*Custom Robo*/
				case 'X':
					current = 0x2E2540;
					end = 0x657F14;
					game == 0;
					break;
				/*Custom Robo Version 2
				also, but not incorporated: 30B480 - */
				case 'Z':
					current = 0x30B480;
					end = 0x980340;
					game == 1;
					break;
				}

			  int start = current;
				while(current<end)
				{	
					fseek(source,current,SEEK_SET);
					if (dlg->killThread)
						break;
					ToUpdateProgressBar(dlg, (current - start), (end - start));
					
					CString tempLocation;
					int fileSizeCompressed = -1;
					CString type = "";
					int fileSizeUncompressed = DecompressCustomRoboSpot(&customrobocompressed, genText, current, source, romSize, game, folderPath, "", -1, tempLocation, fileSizeCompressed, type);
					if (fileSizeUncompressed > 0)
					{
						AddRowData(dlg, current, fileSizeCompressed, fileSizeUncompressed, "", tempLocation, type);
					}

					current += fileSizeCompressed;

					
					if(current==0x3199f4) current=0x31c290;
				  /*this patch fixes continuity errors in CRV2.  images between compressed files*/
					  if(current==0x3fae64) current=0x41f7d0;
					  if(current==0x435FB4) current=0x44b8b0;
					  if(current==0x673ca0) current=0x84F930;
					  if(current==0x897538) current=0x8d7300;

					fseek(source,current,SEEK_SET);
					unsigned long tempAddress = 0x00000000;
					while ((((tempAddress) & 0xFF) != 0) || (tempAddress == 0x00000000))
					{
						/*read in sample*/
						fread(&tempAddress,4,1,source);
						current += 4;
					}
					current -= 4;
				}
				fclose(source);
				delete [] GameBuffer;
			}	
		}
		else if (gameNameStr == "O.D.T.")
		{
			int game = 1; // placeholder
			int romSize = 0;
			unsigned char* GameBuffer = NULL;
			CString folderPath;

			if (ReadROM(gameNameStr, strROMPath, GameBuffer, romSize, folderPath))
			{
				ReceivedNewROM(dlg, strROMPath, GameBuffer, romSize);
				FILE* source = fopen(strROMPath, "rb");

				int current = 0xFF230;
				int end = romSize + 4;

				int start = current;
				while(current<end)
				{	
					fseek(source,current,SEEK_SET);
					if (dlg->killThread)
						break;
					ToUpdateProgressBar(dlg, (current - start), (end - start));
					
					CString tempLocation;
					int fileSizeCompressed = -1;
					CString type = "";
					int fileSizeUncompressed = DecompressODTSpot(&odtcompressed, genText, current, source, romSize, game, folderPath, "", -1, tempLocation, fileSizeCompressed, type);
					if (fileSizeUncompressed > 0)
					{
						AddRowData(dlg, current, fileSizeCompressed, fileSizeUncompressed, "", tempLocation, type);
					}

					current += fileSizeCompressed;
				}
				fclose(source);
				delete [] GameBuffer;
			}	
		}
		else if (gameNameStr == "NFL Blitz 2000")
		{
			int game = 1; // placeholder
			int romSize = 0;
			unsigned char* GameBuffer = NULL;
			CString folderPath;

			if (ReadROM(gameNameStr, strROMPath, GameBuffer, romSize, folderPath))
			{
				ReceivedNewROM(dlg, strROMPath, GameBuffer, romSize);
				for (unsigned long x = 0x167708; x < 0x167709; x++)
				{	
					if (dlg->killThread)
						break;
					ToUpdateProgressBar(dlg, x, romSize);
					CString tempLocation;
					int fileSizeCompressed = -1;
					CString type = "YAZ0";
					int fileSizeUncompressed = DecompressBlitzSpot(&blitzcompressed, genText, x, GameBuffer, romSize, game, folderPath, "", -1, tempLocation, fileSizeCompressed, type);
					if (fileSizeUncompressed > 0)
					{
						AddRowData(dlg, x, fileSizeCompressed, fileSizeUncompressed, "", tempLocation, type);
					}
				}
				
				delete [] GameBuffer;
			}	
		}
	}
	catch (...)
	{
		if (GameBuffer != NULL)
		{
			delete [] GameBuffer;
			GameBuffer = NULL;
		}
	}
	ToUpdateProgressBar(dlg, 100, 100);
	dlg->decompressGamethread = NULL;
	return 0;
}

void CGEDecompressorDlg::OnBnClickedDecompressgame()
{
	KillDecompressGameThread();
	// TODO: Add your control notification handler code here
	CString fileOpen;
	CFileDialog m_ldFile(TRUE, NULL, "Game.rom", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "ROM (*.v64;*.z64;*.rom;*.bin;*.zlb)|*.v64;*.z64;*.rom;*.bin;*.zlb|", this);
	int didRead = m_ldFile.DoModal();
	if ((didRead == IDCANCEL) || (m_ldFile.GetPathName() == ""))
		return;

	if (didRead == FALSE)
		return;

	fileCount = 0;

	for (int x = 0; x < m_list.GetItemCount(); x++)
	{
		delete ((ListUpdateStruct*)m_list.GetItemData(x));
	}

	m_list.DeleteAllItems();
	for ( int i = 5; i >= 0; i--)
     m_list.DeleteColumn (i);
	m_list.InsertColumn(0, "Address",LVCFMT_LEFT, 200);
	m_list.InsertColumn(1, "Uncompressed Size",LVCFMT_LEFT, 100);
	m_list.InsertColumn(2, "Compressed Size",LVCFMT_LEFT, 100);
	m_list.InsertColumn(3, "Type",LVCFMT_LEFT, 60);
	m_list.InsertColumn(4, "Path",LVCFMT_LEFT, 300);
	m_list.InsertColumn(5, "Internal Name",LVCFMT_LEFT, 100);
	

	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	CString gameNameStr;
	m_gameselection.GetWindowText(gameNameStr);

	CString filePathString = m_ldFile.GetPathName();

	this->strROMPath = filePathString;
	this->gameNameStr = gameNameStr;
	this->genText = m_genTextFiles.GetCheck();

	int size = GetSizeFile(this->strROMPath);
	m_progressBar.SetRange(0, 100);
	m_progressBar.SetPos(0);
	m_progressBar.ShowWindow(SW_SHOW);
	killThread = false;
	m_cancelLoad.ShowWindow(SW_SHOW);
	lastSearchSpot = -1;
	decompressGamethread = AfxBeginThread(&CGEDecompressorDlg::DecompressGameThread, this);
 
	m_injectButton.ShowWindow(SW_SHOW);
	m_saveROMButton.ShowWindow(SW_SHOW);
	
	//dkr
	/*FILE* outA = fopen("C:\\temp\\dkrlist.txt", "w");
	for (int x = 0x3A1440; x < 0x3A1548 ; x+= 4)
	{
		unsigned long offset = CharArrayToLong(&GameBuffer[x]) + 0x3A1550;
		unsigned long curSpot = offset;
		CString name;
		while (GameBuffer[curSpot] != 0x0)
		{
			CString tempAddStr;
			tempAddStr.Format("%c", GameBuffer[curSpot]);
			name = name + tempAddStr;
			curSpot++;
		}
		unsigned short bgfile = CharArrayToShort(&GameBuffer[offset + 0x34]);
		unsigned short setupfile = CharArrayToShort(&GameBuffer[offset + 0x36]);
		fprintf(outA, "%s,%04X,%04X\n", name, bgfile, setupfile);
	}
	fclose(outA);*/

	//jfg

	/*FILE* outA = fopen("C:\\temp\\list.txt", "w");
	for (int x = 0xCB2A80; x < 0xCB30F0 ; x+= 4)
	{
		unsigned long offset = CharArrayToLong(&GameBuffer[x]) + 0xCB3100;
		unsigned long curSpot = offset;
		CString name;
		while (GameBuffer[curSpot] != 0x0)
		{
			CString tempAddStr;
			tempAddStr.Format("%c", GameBuffer[curSpot]);
			name = name + tempAddStr;
			curSpot++;
		}
		unsigned short bgfile = CharArrayToShort(&GameBuffer[offset + 0x54]);
		unsigned short setupfile = CharArrayToShort(&GameBuffer[offset + 0x56]);
		fprintf(outA, "%s,%04X,%04X\n", name, bgfile, setupfile);
	}
	fclose(outA);*/

	/*FILE* outA = fopen("C:\\temp\\list.txt", "w");
	for (int x = 0x1714AE0; x < 0x171579C ; x+= 4)
	{
		unsigned long offset = CharArrayToLong(&GameBuffer[x]) + 0x17157A0;
		unsigned long curSpot = offset + 4;
		CString name;
		while (GameBuffer[curSpot] != 0x0)
		{
			CString tempAddStr;
			tempAddStr.Format("%c", GameBuffer[curSpot]);
			name = name + tempAddStr;
			curSpot++;
		}
		unsigned short imageFile = CharArrayToShort(&GameBuffer[offset + 0xE2]);

		CString imageName = "";
		if (imageFile <= 0x387)
		{
			unsigned long offsetImageName = CharArrayToLong(&GameBuffer[0x139B800 + (imageFile << 2)]) + 0x139C630;
			GECompression geComp;
			geComp.SetGame(JFG);
			geComp.SetCompressedBuffer(&GameBuffer[offsetImageName], 0x100000);
			int numBytes;
			unsigned char* output = geComp.OutputDecompressedBuffer(numBytes);


			unsigned long curSpotSub = 0;
			
			
			while (output[curSpotSub] != 0x0)
			{
				CString tempAddStr;
				tempAddStr.Format("%c", output[curSpotSub]);
				imageName = imageName + tempAddStr;
				curSpotSub++;
			}

			delete [] output;
		}

		fprintf(outA, "%04X,%s,%04X,%s\n", ((x - 0x1714AE0) / 4), name, imageFile, imageName);
	}
	fclose(outA);*/

	//kiosk
	/*FILE* outA = fopen("C:\\temp\\jfgkiosklist.txt", "w");
	for (int x = 0xCA39E0; x < 0xCA4038 ; x+= 4)
	{
		unsigned long offset = CharArrayToLong(&GameBuffer[x]) + 0xCA4040;
		unsigned long curSpot = offset;
		CString name;
		while (GameBuffer[curSpot] != 0x0)
		{
			CString tempAddStr;
			tempAddStr.Format("%c", GameBuffer[curSpot]);
			name = name + tempAddStr;
			curSpot++;
		}
		unsigned short bgfile = CharArrayToShort(&GameBuffer[offset + 0x54]);
		unsigned short setupfile = CharArrayToShort(&GameBuffer[offset + 0x56]);
		//fprintf(outA, "%s,%04X,%04X\n", name, bgfile, setupfile);
		fprintf(outA, "%s\n", name, bgfile, setupfile);
	}
	fclose(outA);*/
	/*FILE* outA = fopen("C:\\temp\\jfgkiosklevelsize.txt", "w");
	for (int x = 0xCC3390; x < 0xCC3844 ; x+= 4)
	{
		unsigned short warp = 0xFFFF;
		CString warpStr = "";
		for (int y = 0xCA39E0; y < 0xCA4038 ; y+= 4)
		{
			unsigned long offset = CharArrayToLong(&GameBuffer[y]) + 0xCA4040;
			unsigned long curSpot = offset;
			CString name;
			while (GameBuffer[curSpot] != 0x0)
			{
				CString tempAddStr;
				tempAddStr.Format("%c", GameBuffer[curSpot]);
				name = name + tempAddStr;
				curSpot++;
			}
			unsigned short bgfile = CharArrayToShort(&GameBuffer[offset + 0x54]);
			unsigned short setupfile = CharArrayToShort(&GameBuffer[offset + 0x56]);
			if (bgfile == ((x - 0xCC3390) / 4))
			{
				warp = ((y - 0xCA39E0) / 4);
				warpStr = name;
				break;
			}
		}


		unsigned long size = (CharArrayToLong(&GameBuffer[x+4]) - CharArrayToLong(&GameBuffer[x]));
		fprintf(outA, "%04X,Size %08X,Warp %04X,%s\n", ((x - 0xCC3390) / 4), size, warp, warpStr);
	}
	fclose(outA);*/

	// mickey
	/*FILE* outA = fopen("C:\\temp\\micklist.txt", "w");
	for (int x = 0x80F760; x < 0x80F818 ; x+= 4)
	{
		unsigned long offset = CharArrayToLong(&GameBuffer[x]) + 0x80F820;
		unsigned long curSpot = offset;
		CString name;
		while (GameBuffer[curSpot] != 0x0)
		{
			CString tempAddStr;
			tempAddStr.Format("%c", GameBuffer[curSpot]);
			name = name + tempAddStr;
			curSpot++;
		}
		unsigned short bgfile = CharArrayToShort(&GameBuffer[offset + 0x4C]);
		unsigned short setupfile = CharArrayToShort(&GameBuffer[offset + 0x4E]);
		fprintf(outA, "%04X,%08X,%08X,%s,%04X,%04X\n", ((x - 0x80F760) / 4), x, offset, name, bgfile, setupfile);
	}
	fclose(outA);*/


	//for (int x = 0; x < (size-maxByteSize); x++)
	// start 0xB00000 BT, do earlier though
	// d00000 last
	/*for (unsigned long x = 0x0; x < (size - 0x50); x++)
	{
		try
		{

			if ((((compressed.game == GOLDENEYE) || (compressed.game == KILLERINSTINCT) ) && ((GameBuffer[x] == 0x11) && (GameBuffer[x+1] == 0x72))) 
				|| ((compressed.game == PD) && (GameBuffer[x] == 0x11) && (GameBuffer[x+1] == 0x73)) 
				|| ((compressed.game == MILO) && (GameBuffer[x] == 0x78) && (GameBuffer[x+1] == 0x9C))
				|| (((compressed.game == DONKEYKONG64KIOSK) || (compressed.game == DONKEYKONG64) || (compressed.game == BLASTCORPS)) && (GameBuffer[x] == 0x1F) && (GameBuffer[x+1] == 0x8B) && (GameBuffer[x+2] == 0x08) && (GameBuffer[x+3] == 0x0))
				|| (((compressed.game == DONKEYKONG64KIOSK) || (compressed.game == DONKEYKONG64) || (compressed.game == BLASTCORPS)) && (GameBuffer[x] == 0x1F) && (GameBuffer[x+1] == 0x8B) && (GameBuffer[x+2] == 0x08) && (GameBuffer[x+3] == 0x08))
				|| ((compressed.game == BANJOKAZOOIE) && (GameBuffer[x] == 0x11) && (GameBuffer[x+1] == 0x72) && (GameBuffer[x+2] == 0x0)) 
				|| ((compressed.game == CONKER) && (GameBuffer[x] == 0x0) && (GameBuffer[x+4] != 0x0)))
			{

				
				unsigned long trueFileSize = 0;

				if (compressed.game == PD)
				{
					trueFileSize = ((GameBuffer[x+2] << 16) | (GameBuffer[x+3] << 8) | GameBuffer[x+4]);
					if ((trueFileSize > 0xFFFFFF))
						continue;
				}
				else if (compressed.game == BANJOKAZOOIE)
				{
					trueFileSize = ((GameBuffer[x+2] << 24) | (GameBuffer[x+3] << 16) | (GameBuffer[x+4] << 8) | GameBuffer[x+5]);
					if ((trueFileSize > 0xFFFFFF))
						continue;
				}
				else if (compressed.game == CONKER)
				{
					trueFileSize = ((GameBuffer[x] << 24) | (GameBuffer[x+1] << 16) | (GameBuffer[x+2] << 8) | GameBuffer[x+3]);
					if (((trueFileSize > 0xFFFFF)) || (GameBuffer[x+4] == 0) || (trueFileSize == 0))
						continue;
				}

				unsigned char* Buffer;
				Buffer = new unsigned char[maxByteSize];
				unsigned long endSize = maxByteSize;
				if ((x + maxByteSize) > size)
				{
					endSize = (size - x);
				}
				for (int y = 0; y < endSize; y++)
				{
					Buffer[y] = GameBuffer[x+y];
				}
				compressed.SetCompressedBuffer(Buffer, maxByteSize);
				int fileSize = 0;
				unsigned char* outputDecompressed = compressed.OutputDecompressedBuffer(fileSize);

				if (outputDecompressed == NULL)
				{
					delete [] Buffer;
					continue;
				}

				CString folderPath = m_ldFile.GetPathName();
				folderPath = folderPath.Mid(0, (folderPath.ReverseFind('\\') + 1));


				CString tempLocation;
				if ((compressed.game == DONKEYKONG64) || (compressed.game == DONKEYKONG64KIOSK) || (compressed.game == BLASTCORPS))
				{
					tempLocation.Format("%07X", x);
					if ((GameBuffer[x] == 0x1F) && (GameBuffer[x+1] == 0x8B) && (GameBuffer[x+2] == 0x08) && (GameBuffer[x+3] == 0x8))
					{
						int curSpot = x + 0xA;
						while (GameBuffer[curSpot] != 0x0)
						{
							CString tempAddStr;
							tempAddStr.Format("%c", GameBuffer[curSpot]);
							tempLocation = tempLocation + tempAddStr;
							curSpot++;
						}
					}
					tempLocation = folderPath + tempLocation + ".bin";
				}
				else if ((compressed.game == CONKER) || (compressed.game == PD))
				{
					tempLocation.Format("%s%07X.bin", folderPath, x, fileSize);
				}
				else
				{
					tempLocation.Format("%s%06X.bin", folderPath, x, fileSize);
				}


				if (((compressed.game != PD) && (compressed.game != BANJOKAZOOIE) && (compressed.game != PD)) || (((compressed.game == BANJOKAZOOIE) || (compressed.game == CONKER) || (compressed.game == PD)) && (fileSize > 0) && (fileSize == trueFileSize)))
				{
					if (((compressed.game != CONKER) && (fileSize != 0)) || ((compressed.game == CONKER) && (fileSize == (trueFileSize))))
					{
						WriteBinaryFile(tempLocation, outputDecompressed, fileSize);
						if (m_genTextFiles.GetCheck())
						{
							WriteASCIIFile(tempLocation + ".txt", outputDecompressed, fileSize);
						}
					}
				}

				delete [] Buffer;

				if (outputDecompressed != NULL)
					delete [] outputDecompressed;
			}

			else if ((compressed.game == BANJOTOOIE) && (GameBuffer[x] == 0xAA) && (GameBuffer[x+1] == 0xAA))
			{
				while (GameBuffer[x] == 0xAA)
					x++;

				unsigned char* Buffer;
				Buffer = new unsigned char[maxByteSize];
				for (int y = 0; y < maxByteSize; y++)
				{
					Buffer[y] = GameBuffer[x+y];
				}
				compressed.SetCompressedBuffer(Buffer, maxByteSize);
				int fileSize = 0;
				unsigned char* outputDecompressed = compressed.OutputDecompressedBuffer(fileSize);

				CString tempLocation;
				tempLocation.Format("%07X.bin", x, fileSize);


				if (fileSize != 0)
				{
					WriteBinaryFile(tempLocation, outputDecompressed, fileSize);
					WriteASCIIFile(tempLocation + ".txt", outputDecompressed, fileSize);
				}
				delete [] Buffer;

				if (outputDecompressed != NULL)
					delete [] outputDecompressed;

				x = x + 0x10;
			}
		} catch (...)
		{
			continue;
		}
	}
	
	delete [] GameBuffer;*/

}

void CGEDecompressorDlg::WriteASCIIFile(CString filename, unsigned char* outputDecompressed, int fileSize, bool appendFile)
{
	FILE* outFile;
	if (!appendFile)
		outFile = fopen(filename, "w");
	else
		outFile = fopen(filename, "a");
	if (outFile != NULL)
	{
		for (int y = 0; y < fileSize; y++)
		{
			if (outputDecompressed != NULL)
				fprintf(outFile, "%02X", outputDecompressed[y]);
		}
		fprintf(outFile, "\n\n\n");
		for (int y = 0; y < fileSize; y++)
		{
			if (outputDecompressed != NULL)
				fprintf(outFile, "%c", outputDecompressed[y]);
		}
		fprintf(outFile, "\n\n\n");
	}
	fflush(outFile);
	fclose(outFile);
}

void CGEDecompressorDlg::WriteBinaryFile(CString filename, unsigned char* outputDecompressed, int fileSize, bool appendFile)
{
	FILE* outFile;
	if (!appendFile)
		outFile = fopen(filename, "wb");
	else
		outFile = fopen(filename, "ab");
	if (outFile != NULL)
	{
		for (int y = 0; y < fileSize; y++)
		{
			if (outputDecompressed != NULL)
				fwrite(&outputDecompressed[y], 1, 1, outFile);
		}
		fflush(outFile);
		fclose(outFile);
	}
}

void CGEDecompressorDlg::WriteResult(bool genTextFile, CString filename, unsigned char* outputDecompressed, int fileSize, bool appendFile)
{
	if (fileSize > 0)
	{
		WriteBinaryFile(filename, outputDecompressed, fileSize, appendFile);
		if (genTextFile)
		{
			WriteASCIIFile(filename + ".txt", outputDecompressed, fileSize, appendFile);
		}
	}
}

void CGEDecompressorDlg::OnBnClickedButton3()
{
	int rowSel = m_list.GetSelectionMark();
	if (rowSel == -1)
		return;

	ListUpdateStruct* listUpdateStruct = ((ListUpdateStruct*)m_list.GetItemData(rowSel));

	unsigned long diff = 0xF000000;

	if (listUpdateStruct->fileSizeCompressed > 0)
		diff = listUpdateStruct->fileSizeCompressed;
	else if (rowSel < (m_list.GetItemCount() - 1))
	{
		ListUpdateStruct* listUpdateStructNext = ((ListUpdateStruct*)m_list.GetItemData(rowSel+1));
		diff = listUpdateStructNext->address - listUpdateStruct->address;
	}

	unsigned long address = listUpdateStruct->address;

	CString fileOpen;
	CFileDialog m_ldFile(TRUE, NULL, "binary.bin", NULL, "IN Binary to Inject(*.bin)|*.*|", this);
	int didRead = m_ldFile.DoModal();
	if ((didRead == IDCANCEL) || (m_ldFile.GetPathName() == ""))
		return;

	if (didRead == FALSE)
		return;

	CString gameNameStr;
	m_gameselection.GetWindowText(gameNameStr);

	CString tempAddrStr;
	tempAddrStr.Format("%08X_MODIFIED.bin", address);

	int zlibGame = GetZLibGameName(gameNameStr);
	if (zlibGame != -1)
	{
		if (listUpdateStruct->type == "MP3")
		{
			unsigned long outSize = GetSizeFile(m_ldFile.GetPathName());
			if (outSize > 0)
			{
				if (outSize > diff)
				{
					CString sizeTempStr;
					sizeTempStr.Format("%08X is larger than %08X (next item), are you sure you want to replace?", outSize, diff);
					int iResults = MessageBox(sizeTempStr, "Are you sure?", MB_YESNO | MB_ICONINFORMATION);
					if (iResults == IDNO)
						return;
				}

				FILE* inNew = fopen(m_ldFile.GetPathName(), "rb");
				if (inNew == NULL)
				{
					MessageBox("Error opening temporary file");
					return;
				}

				fread(&ROM[address], 1, outSize, inNew);
				
				fclose(inNew);
				
				listUpdateStruct->fileSizeCompressed = outSize;
				listUpdateStruct->tempLocation = static_cast<LPCTSTR>(directory + tempAddrStr);
			}
		}
		else
		{
			GECompression compressed;
			compressed.SetPath(directory);
			compressed.SetGame(zlibGame);

			CString zlibInput = m_ldFile.GetPathName();
			compressed.CompressGZipFile(zlibInput, (directory + tempAddrStr), false);

			unsigned long outSize = GetSizeFile((directory + tempAddrStr));
			if (outSize > 0)
			{
				if (outSize > diff)
				{
					CString sizeTempStr;
					sizeTempStr.Format("%08X is larger than %08X (next item), are you sure you want to replace?", outSize, diff);
					int iResults = MessageBox(sizeTempStr, "Are you sure?", MB_YESNO | MB_ICONINFORMATION);
					if (iResults == IDNO)
						return;
				}

				if (zlibGame == BANJOTOOIE)
				{
					if (listUpdateStruct->type.Find("Encrypted") != -1)
					{
						FILE* inFile = fopen((directory + tempAddrStr), "rb");
						if (inFile == NULL)
						{
							MessageBox("Cannot open BT file", "Error");
							return;
						}

						unsigned char* tempInputBuffer = new unsigned char[outSize];
						fread(tempInputBuffer, 1, outSize, inFile);
						fclose(inFile);

						DecryptBTFile(((listUpdateStruct->address - 0x5188) / 4), tempInputBuffer, tempInputBuffer, outSize);

						FILE* outFile = fopen((directory + tempAddrStr), "wb");
						if (outFile == NULL)
						{
							MessageBox("Cannot open encrypted BT file", "Error");
							return;
						}
						fwrite(tempInputBuffer, 1, outSize, outFile);
						fclose(outFile);

						delete [] tempInputBuffer;
					}

					int extraAAs = 0;
					if (outSize < diff)
					{
						extraAAs = (diff - outSize);
					}

					FILE* appendFile = fopen((directory + tempAddrStr), "ab");
					if (appendFile == NULL)
					{
						MessageBox("Cannot open BT file", "Error");
						return;
					}

					unsigned char aa = 0xAA;
					for (int x = outSize; x < diff; x++)
					{
						fwrite(&aa, 1, 1, appendFile);
					}
					fclose(appendFile);
				}

				FILE* inNew = fopen((directory + tempAddrStr), "rb");
				if (inNew == NULL)
				{
					MessageBox("Error opening temporary file");
					return;
				}

				if ((zlibGame == BANJOTOOIE) && (address >= 0x5188)  && (address < 0x11A24))
				{
					address = ((CharArrayToLong(&ROM[address]) >> 8) * 4) + 0x12B24;
					fread(&ROM[address], 1, diff, inNew);
				}
				else if ((zlibGame == BANJOTOOIE) && (address >= 0x1E8A77C)  && (address < 0x1E899B0))
				{
					address = ((CharArrayToLong(&ROM[address]))) + 0x10;
					fread(&ROM[address], 1, diff, inNew);
				}
				else
				{
					fread(&ROM[address], 1, outSize, inNew);
				}
				fclose(inNew);
				
				listUpdateStruct->fileSizeCompressed = outSize;
				listUpdateStruct->tempLocation = static_cast<LPCTSTR>(directory + tempAddrStr);
			}
		}
	}
	else if ((gameNameStr == "Turok 1") || (gameNameStr == "Duke Nukem 64")|| (gameNameStr == "Turok 2") || (gameNameStr == "Turok 3") || (gameNameStr == "Turok Rage Wars") || (gameNameStr == "Iggy's Reckin' Balls") || (gameNameStr == "Shadowman") || (gameNameStr == "NHL Breakaway 98") || (gameNameStr == "NHL Breakaway 99") || (gameNameStr == "PGA European Tour"))
	{
		RncDecoder rnccompression;

		rnccompression.CompressRNCFile(directory, m_ldFile.GetPathName(), (directory + tempAddrStr), false);
		unsigned long outSize = GetSizeFile((directory + tempAddrStr));
		if (outSize > 0)
		{
			if (outSize > diff)
			{
				CString sizeTempStr;
				sizeTempStr.Format("%08X is larger than %08X (next item), are you sure you want to replace?", outSize, diff);
				int iResults = MessageBox(sizeTempStr, "Are you sure?", MB_YESNO | MB_ICONINFORMATION);
				if (iResults == IDNO)
					return;
			}
			FILE* inNew = fopen((directory + tempAddrStr), "rb");
			if (inNew == NULL)
			{
				MessageBox("Error opening temporary file");
				return;
			}
			fread(&ROM[address], 1, outSize, inNew);
			fclose(inNew);
			
			listUpdateStruct->fileSizeCompressed = outSize;
			listUpdateStruct->tempLocation = static_cast<LPCTSTR>(directory + tempAddrStr);
		}
	}
	else if (gameNameStr == "Forsaken")
	{
		RncDecoder rnccompression;

		rnccompression.CompressRNCFile(directory, m_ldFile.GetPathName(), (directory + tempAddrStr), true);
		unsigned long outSize = GetSizeFile((directory + tempAddrStr));
		if (outSize > 0)
		{
			if (outSize > diff)
			{
				CString sizeTempStr;
				sizeTempStr.Format("%08X is larger than %08X (next item), are you sure you want to replace?", outSize, diff);
				int iResults = MessageBox(sizeTempStr, "Are you sure?", MB_YESNO | MB_ICONINFORMATION);
				if (iResults == IDNO)
					return;
			}
			FILE* inNew = fopen((directory + tempAddrStr), "rb");
			if (inNew == NULL)
			{
				MessageBox("Error opening temporary file");
				return;
			}
			fread(&ROM[address], 1, outSize, inNew);
			fclose(inNew);
			
			listUpdateStruct->fileSizeCompressed = outSize;
			listUpdateStruct->tempLocation = static_cast<LPCTSTR>(directory + tempAddrStr);
		}
	}
	else if ((gameNameStr == "ECW Hardcore Revolution") || (gameNameStr == "NFL QB Club 2000") || (gameNameStr == "NFL QB Club 2001") || (gameNameStr == "NFL QB Club 98") || (gameNameStr == "NFL QB Club 99") || (gameNameStr == "WWF WarZone") || (gameNameStr == "WWF Attitude") || (gameNameStr == "All-Star Baseball 99") || (gameNameStr == "South Park Chefs Love Shack"))
	{
		RncDecoder rnccompression;

		rnccompression.CompressRNCFile(directory, m_ldFile.GetPathName(), (directory + tempAddrStr), false);
		unsigned long outSize = GetSizeFile((directory + tempAddrStr));
		if (outSize > 0)
		{
			if (outSize > diff)
			{
				CString sizeTempStr;
				sizeTempStr.Format("%08X is larger than %08X (next item), are you sure you want to replace?", outSize, diff);
				int iResults = MessageBox(sizeTempStr, "Are you sure?", MB_YESNO | MB_ICONINFORMATION);
				if (iResults == IDNO)
					return;
			}
			FILE* inNew = fopen((directory + tempAddrStr), "rb");
			if (inNew == NULL)
			{
				MessageBox("Error opening temporary file");
				return;
			}
			fread(&ROM[address], 1, outSize, inNew);
			fclose(inNew);
			
			listUpdateStruct->fileSizeCompressed = outSize;
			listUpdateStruct->tempLocation = static_cast<LPCTSTR>(directory + tempAddrStr);
		}
	}
	else if (gameNameStr == "Armorines")
	{
		RncDecoder rnccompression;

		rnccompression.CompressRNCFile(directory, m_ldFile.GetPathName(), (directory + tempAddrStr), false);
		unsigned long outSize = GetSizeFile((directory + tempAddrStr));
		if (outSize > 0)
		{
			if (outSize > diff)
			{
				CString sizeTempStr;
				sizeTempStr.Format("%08X is larger than %08X (next item), are you sure you want to replace?", outSize, diff);
				int iResults = MessageBox(sizeTempStr, "Are you sure?", MB_YESNO | MB_ICONINFORMATION);
				if (iResults == IDNO)
					return;
			}
			FILE* inNew = fopen((directory + tempAddrStr), "rb");
			if (inNew == NULL)
			{
				MessageBox("Error opening temporary file");
				return;
			}
			fread(&ROM[address], 1, outSize, inNew);
			fclose(inNew);
			
			listUpdateStruct->fileSizeCompressed = outSize;
			listUpdateStruct->tempLocation = static_cast<LPCTSTR>(directory + tempAddrStr);
		}
	}
	else if (gameNameStr == "South Park")
	{
		RncDecoder rnccompression;

		rnccompression.CompressRNCFile(directory, m_ldFile.GetPathName(), (directory + tempAddrStr), false);
		unsigned long outSize = GetSizeFile((directory + tempAddrStr));
		if (outSize > 0)
		{
			if (outSize > diff)
			{
				CString sizeTempStr;
				sizeTempStr.Format("%08X is larger than %08X (next item), are you sure you want to replace?", outSize, diff);
				int iResults = MessageBox(sizeTempStr, "Are you sure?", MB_YESNO | MB_ICONINFORMATION);
				if (iResults == IDNO)
					return;
			}
			FILE* inNew = fopen((directory + tempAddrStr), "rb");
			if (inNew == NULL)
			{
				MessageBox("Error opening temporary file");
				return;
			}
			fread(&ROM[address], 1, outSize, inNew);
			fclose(inNew);
			
			listUpdateStruct->fileSizeCompressed = outSize;
			listUpdateStruct->tempLocation = static_cast<LPCTSTR>(directory + tempAddrStr);
		}
	}
	else if ((gameNameStr == "AeroFighter") || (gameNameStr == "Beetle Adventure Racing") || (gameNameStr == "Body Harvest") || (gameNameStr == "Duck Dodgers") || (gameNameStr == "F-1 World Grand Prix")|| (gameNameStr == "F-1 World Grand Prix II (E)")|| (gameNameStr == "F-ZERO Expansion Kit 64DD")|| (gameNameStr == "F-ZERO X")|| (gameNameStr == "Indy Racing 2000")|| (gameNameStr == "Lt. Duck Dodgers (Prototype)")|| (gameNameStr == "MarioKart64")|| (gameNameStr == "Pilotwings 64")|| (gameNameStr == "StarFox64")|| (gameNameStr == "Super Mario 64")|| (gameNameStr == "WaveRace"))
	{
		MIO0 mio0compression;
		mio0compression.CompressMIO0File(directory, m_ldFile.GetPathName(), (directory + tempAddrStr));
		unsigned long outSize = GetSizeFile((directory + tempAddrStr));
		if (outSize > 0)
		{
			if (outSize > diff)
			{
				CString sizeTempStr;
				sizeTempStr.Format("%08X is larger than %08X (next item), are you sure you want to replace?", outSize, diff);
				int iResults = MessageBox(sizeTempStr, "Are you sure?", MB_YESNO | MB_ICONINFORMATION);
				if (iResults == IDNO)
					return;
			}
			FILE* inNew = fopen((directory + tempAddrStr), "rb");
			if (inNew == NULL)
			{
				MessageBox("Error opening temporary file");
				return;
			}
			fread(&ROM[address], 1, outSize, inNew);
			fclose(inNew);
			
			listUpdateStruct->fileSizeCompressed = outSize;
			listUpdateStruct->tempLocation = static_cast<LPCTSTR>(directory + tempAddrStr);
		}
	}
	else if (gameNameStr == "Superman")
	{
		MessageBox("Unsupported game, cannot compress");
		return;
	}
	else if (gameNameStr == "Monaco Grand Prix")
	{
		MessageBox("Unsupported game, cannot compress");
		return;
	}
	else if ((gameNameStr == "1080Snow") || (gameNameStr == "BakuBomberman2")|| (gameNameStr == "Battlezone Black Dogs")|| (gameNameStr == "Bomberman 64 Second Attack")|| (gameNameStr == "Disney's Donald Duck - Goin' Quackers")|| (gameNameStr == "Doraemon 2")|| (gameNameStr == "Fushigi no Dungeon")|| (gameNameStr == "Kira to Kaiketsu")|| (gameNameStr == "Neon Genesis Evangelion")|| (gameNameStr == "Nushi Tsuri 64")|| (gameNameStr == "Paper Mario")|| (gameNameStr == "Parlor Pro 64")|| (gameNameStr == "Pokemon Stadium")|| (gameNameStr == "Pokemon Stadium 2")|| (gameNameStr == "Rayman 2")|| (gameNameStr == "Scooby-Doo")|| (gameNameStr == "Shadowgate 64")|| (gameNameStr == "Tonic Trouble")|| (gameNameStr == "Tsumi to Batsu")|| (gameNameStr == "Zool"))
	{
		MessageBox("Unsupported game, cannot compress");
		return;
	}
	else if (gameNameStr == "SSB")
	{
		MessageBox("Unsupported game, cannot compress");
		return;
	}
	else if (gameNameStr == "Pokemon Snap")
	{
		MessageBox("Unsupported game, cannot compress");
		return;
	}
	else if (gameNameStr == "Shigesato Itoi Bass Fishing")
	{
		MessageBox("Unsupported game, cannot compress");
		return;
	}
	else if ((gameNameStr == "Tony Hawk Pro Skater 1") || (gameNameStr == "Tony Hawk Pro Skater 2") || (gameNameStr == "Tony Hawk Pro Skater 3"))
	{
		MessageBox("Unsupported game, cannot compress");
		return;
	}
	else if ((gameNameStr == "Monster Truck Madness") || (gameNameStr == "Spiderman"))
	{
		MessageBox("Unsupported game, cannot compress");
		return;
	}
	else if ((gameNameStr == "Doubutsu no Mori") || (gameNameStr == "MajorasMask") || (gameNameStr == "OoT"))
	{
		YAZ0 yaz0;
		
		FILE* tempInputFile = fopen(m_ldFile.GetPathName(), "rb");
		fseek(tempInputFile, 0, SEEK_END);
		unsigned long size = ftell(tempInputFile);

		unsigned char* tempBuffer;
		tempBuffer = new unsigned char[size];
		
		fseek(tempInputFile, 0, SEEK_SET);
		fread(tempBuffer, 1, size, tempInputFile);
		fclose(tempInputFile);
		int compressedSize;

		// will flip to -1 if invalid
		compressedSize = 0x200000;
		unsigned char* outputBuffer = new unsigned char[0x200000];
		yaz0.yaz0_encode(tempBuffer, size, outputBuffer, &compressedSize);

		if (compressedSize > 0)
		{
			if (compressedSize > diff)
			{
				CString sizeTempStr;
				sizeTempStr.Format("%08X is larger than %08X (next item), are you sure you want to replace?", compressedSize, diff);
				int iResults = MessageBox(sizeTempStr, "Are you sure?", MB_YESNO | MB_ICONINFORMATION);
				if (iResults == IDNO)
				{
					delete [] tempBuffer;
					delete [] outputBuffer;
					return;
				}
			}

			FILE* outFile = fopen((directory + tempAddrStr), "wb");
			if (outFile == NULL)
			{
				MessageBox("Cannot open file");
				return;
			}
			fwrite(outputBuffer, 1, compressedSize, outFile);
			fclose(outFile);
			listUpdateStruct->fileSizeCompressed = compressedSize;
			listUpdateStruct->tempLocation = static_cast<LPCTSTR>(directory + tempAddrStr);

			memcpy(&ROM[address], outputBuffer, compressedSize);
		}

		delete [] tempBuffer;
		delete [] outputBuffer;
	}
	else if ((gameNameStr == "40 Winks")|| (gameNameStr == "Cruis'n World")|| (gameNameStr == "Duke Nukem - ZER0 H0UR")|| (gameNameStr == "Hydro Thunder")|| (gameNameStr == "Mortal Kombat 4")|| (gameNameStr == "NBA Showtime")|| (gameNameStr == "Tarzan")|| (gameNameStr == "TWINE")|| (gameNameStr == "War Gods"))
	{
		CEDLCompress edlcompress;
		edlcompress.encode(m_ldFile.GetPathName(), (directory + tempAddrStr));

		int inputSize = GetSizeFile(m_ldFile.GetPathName());
		unsigned long outSize = GetSizeFile((directory + tempAddrStr));
		if (outSize > 0)
		{
			if (outSize > diff)
			{
				CString sizeTempStr;
				sizeTempStr.Format("%08X is larger than %08X (next item), are you sure you want to replace?", outSize, diff);
				int iResults = MessageBox(sizeTempStr, "Are you sure?", MB_YESNO | MB_ICONINFORMATION);
				if (iResults == IDNO)
					return;
			}
			FILE* inNew = fopen((directory + tempAddrStr), "rb");
			if (inNew == NULL)
			{
				MessageBox("Error opening temporary file");
				return;
			}
			fread(&ROM[address], 1, outSize, inNew);
			fclose(inNew);
			
			listUpdateStruct->fileSizeUncompressed = inputSize;
			listUpdateStruct->fileSizeCompressed = outSize;
			listUpdateStruct->tempLocation = static_cast<LPCTSTR>(directory + tempAddrStr);
		}
	}
	else if ((gameNameStr == "Custom Robo") || (gameNameStr == "Custom Robo V2"))
	{
		CCustomRoboCompress customrobocompress;
		customrobocompress.encode(m_ldFile.GetPathName(), (directory + tempAddrStr));

		int inputSize = GetSizeFile(m_ldFile.GetPathName());
		unsigned long outSize = GetSizeFile((directory + tempAddrStr));
		if (outSize > 0)
		{
			if (outSize > diff)
			{
				CString sizeTempStr;
				sizeTempStr.Format("%08X is larger than %08X (next item), are you sure you want to replace?", outSize, diff);
				int iResults = MessageBox(sizeTempStr, "Are you sure?", MB_YESNO | MB_ICONINFORMATION);
				if (iResults == IDNO)
					return;
			}
			FILE* inNew = fopen((directory + tempAddrStr), "rb");
			if (inNew == NULL)
			{
				MessageBox("Error opening temporary file");
				return;
			}
			fread(&ROM[address], 1, outSize, inNew);
			fclose(inNew);
			
			listUpdateStruct->fileSizeUncompressed = inputSize;
			listUpdateStruct->fileSizeCompressed = outSize;
			listUpdateStruct->tempLocation = static_cast<LPCTSTR>(directory + tempAddrStr);
		}
	}
	else if (gameNameStr == "O.D.T.")
	{
		CODTCompress odtcompress;
		odtcompress.encode(m_ldFile.GetPathName(), (directory + tempAddrStr));

		int inputSize = GetSizeFile(m_ldFile.GetPathName());
		unsigned long outSize = GetSizeFile((directory + tempAddrStr));
		if (outSize > 0)
		{
			if (outSize > diff)
			{
				CString sizeTempStr;
				sizeTempStr.Format("%08X is larger than %08X (next item), are you sure you want to replace?", outSize, diff);
				int iResults = MessageBox(sizeTempStr, "Are you sure?", MB_YESNO | MB_ICONINFORMATION);
				if (iResults == IDNO)
					return;
			}
			FILE* inNew = fopen((directory + tempAddrStr), "rb");
			if (inNew == NULL)
			{
				MessageBox("Error opening temporary file");
				return;
			}
			fread(&ROM[address], 1, outSize, inNew);
			fclose(inNew);
			
			listUpdateStruct->fileSizeUncompressed = inputSize;
			listUpdateStruct->fileSizeCompressed = outSize;
			listUpdateStruct->tempLocation = static_cast<LPCTSTR>(directory + tempAddrStr);
		}
	}
	else
	{
		MessageBox("Unsupported game, cannot compress");
		return;
	}

	
	CString tempNewUncompressedSize;
	tempNewUncompressedSize.Format("%06X", listUpdateStruct->fileSizeUncompressed);
	LVITEM lv2;
	lv2.iItem = rowSel;
	lv2.iSubItem = 1;
	lv2.pszText = (LPSTR) tempNewUncompressedSize.GetBuffer();
	lv2.mask = LVIF_TEXT;
	m_list.SetItem(&lv2);   

	CString tempNewCompressedSize;
	tempNewCompressedSize.Format("%06X", listUpdateStruct->fileSizeCompressed);
	LVITEM lv3;
	lv3.iItem = rowSel;
	lv3.iSubItem = 2;
	lv3.pszText = (LPSTR) tempNewCompressedSize.GetBuffer();
	lv3.mask = LVIF_TEXT;
	m_list.SetItem(&lv3);

	LVITEM lv5;
	lv5.iItem = rowSel;
	lv5.iSubItem = 3;
	lv5.pszText = (LPSTR) listUpdateStruct->tempLocation.GetBuffer();
	lv5.mask = LVIF_TEXT;
	m_list.SetItem(&lv5);
}

unsigned char CGEDecompressorDlg::ReturnASCIIChar(unsigned char asciiByte)
{
	if ((asciiByte >= 0x21) && (asciiByte < 0x7F))
		return asciiByte;
	else
		return 0x20;
}

void CGEDecompressorDlg::OnLvnItemchangedListdecompressedfiles(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	// only when done
	if (decompressGamethread == NULL)
	{
		LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
		int item = pNMLV->iItem;
		ListUpdateStruct* listUpdateStruct = ((ListUpdateStruct*)m_list.GetItemData(item));

		int size = GetSizeFile(listUpdateStruct->tempLocation);
		CString locationAddressStr;
		mFile.SetWindowText(listUpdateStruct->tempLocation);
		FILE* inFile = fopen(listUpdateStruct->tempLocation, "rb");
		
		if (inFile == NULL)
		{
			m_hexView.SetWindowText("!!!GEDECOMPRESSOR ERROR:File not found or error opening!!!");
			return;
		}
		unsigned char* buffer = new unsigned char[size];
		fread(buffer, 1, size, inFile);
		fclose(inFile);

		CString totalStr = "";
		if (size >= 0x10)
		{
			for (int x = 0; x < size; x+=0x10)
			{
				CString tempAddStr;
				tempAddStr.Format("%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\r\n", buffer[x], buffer[x+1], buffer[x+2], buffer[x+3], buffer[x+4], buffer[x+5], buffer[x+6], buffer[x+7], buffer[x+8], buffer[x+9], buffer[x+0xA], buffer[x+0xB], buffer[x+0xC], buffer[x+0xD], buffer[x+0xE], buffer[x+0xF], ReturnASCIIChar(buffer[x]), ReturnASCIIChar(buffer[x+1]), ReturnASCIIChar(buffer[x+2]), ReturnASCIIChar(buffer[x+3]), ReturnASCIIChar(buffer[x+4]), ReturnASCIIChar(buffer[x+5]), ReturnASCIIChar(buffer[x+6]), ReturnASCIIChar(buffer[x+7]), ReturnASCIIChar(buffer[x+8]), ReturnASCIIChar(buffer[x+9]), ReturnASCIIChar(buffer[x+0xA]), ReturnASCIIChar(buffer[x+0xB]), ReturnASCIIChar(buffer[x+0xC]), ReturnASCIIChar(buffer[x+0xD]), ReturnASCIIChar(buffer[x+0xE]), ReturnASCIIChar(buffer[x+0xF]));
				totalStr = totalStr + tempAddStr;
			}
		}

		int strL = totalStr.GetLength();
		if ((size % 0x10) != 0)
		{
			for (int x = (size - (size % 0x10)); x < size; x++)
			{
				CString tempAddStr;
				tempAddStr.Format("%02X", buffer[x]);
				totalStr = totalStr + tempAddStr;
			}
			totalStr += " ";
			for (int x = (size - (size % 0x10)); x < size; x++)
			{
				CString tempAddStr;
				tempAddStr.Format("%c", buffer[x]);
				totalStr = totalStr + tempAddStr;
			}
		}

		m_hexView.SetWindowText(totalStr);

		delete [] buffer;
	}
}



void CGEDecompressorDlg::KillDecompressGameThread()
{

	if (decompressGamethread != NULL)
	{
		try
		{
			killThread = true;
			/*Sleep(1000);
			if (decompressGamethread != NULL)
			{
				TerminateThread(decompressGamethread, 0);
			}*/
		}
		catch (...)
		{

		}
		m_progressBar.ShowWindow(SW_HIDE);
		decompressGamethread = NULL;
		m_cancelLoad.ShowWindow(SW_HIDE);
	}
}

void CGEDecompressorDlg::OnBnClickedButtoncancelload()
{
	KillDecompressGameThread();
}

void CGEDecompressorDlg::OnBnClickedButtonsaverom()
{
	if ((ROMSize > 0) && (ROM != NULL))
	{
		CFileDialog m_svFile(FALSE, NULL, (romName + ".rom"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "OUT ROM(*.v64;*.z64;*.rom;*.bin)|*.v64;*.z64;*.rom;*.bin|", this);

		int isFileOpened2 = m_svFile.DoModal();

		if (isFileOpened2 == IDCANCEL)
			return;

		if (m_svFile.GetFileName() == "")
			return;

		FILE* outFile = fopen(m_svFile.GetPathName(), "wb");
		if (outFile == NULL)
		{
			MessageBox("Cannot open output file");
			return;
		}
		fwrite(ROM, 1, ROMSize, outFile);

		fclose(outFile);

		CString RN64dir = m_svFile.GetPathName().Mid(0, (m_svFile.GetPathName().ReverseFind('\\')+1));
		CString RN64name = m_svFile.GetPathName().Mid((m_svFile.GetPathName().ReverseFind('\\')+1), (m_svFile.GetPathName().GetLength() - (m_svFile.GetPathName().ReverseFind('\\')+1)));

		if (RN64dir != directory)
			::CopyFile((directory + "rn64crc.exe"), (RN64dir + "rn64crc.exe"), false);

		::SetCurrentDirectory(RN64dir);

		CString tempStr;
		tempStr.Format("rn64crc.exe -u \"%s\"", RN64name);
		hiddenExec(_T(tempStr.GetBuffer()), RN64dir);
	}
}

BOOL CGEDecompressorDlg::hiddenExec (PTSTR pCmdLine, CString currentDirectory)
{
	int errorCode;
	return hiddenExec(pCmdLine, currentDirectory, errorCode);
};

BOOL CGEDecompressorDlg::hiddenExec (PTSTR pCmdLine, CString currentDirectory, int& errorCode)
{
	return hiddenExec(pCmdLine, currentDirectory, errorCode, 20000);
}

BOOL CGEDecompressorDlg::hiddenExec (PTSTR pCmdLine, CString currentDirectory, int& errorCode, int waitTime)
{
	errorCode = 0;
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
   WaitForSingleObject(processInfo.hProcess, waitTime);
   DWORD exitCode;
   if (GetExitCodeProcess(processInfo.hProcess, &exitCode))
   {
	   errorCode = exitCode;
		if (exitCode == STILL_ACTIVE)
		{
			MessageBox("For some reason Process Failed", "Error");
			TerminateProcess(processInfo.hProcess, exitCode);
			return false;
		}
   }   
   return true;
}

void CGEDecompressorDlg::OnClose()
{
	if (ROM != NULL)
	{
		delete [] ROM;
		ROM = NULL;
	}

	for (int x = 0; x < m_list.GetItemCount(); x++)
	{
		delete ((ListUpdateStruct*)m_list.GetItemData(x));
	}

	CDialog::OnClose();
}

unsigned char CGEDecompressorDlg::StringToUnsignedChar(CString inString)
{
	int tempA = inString.GetLength();
	if (inString.GetLength() < 2)
	{
		CString tempStr = inString;
		inString = "";
		for (int x = 0; x < (2-tempStr.GetLength()); x++)
		{
			inString = inString + "0";
		}
		inString = inString + tempStr;
	}
	char* b;
	b = inString.GetBuffer(0);
	unsigned long tempLong = 0;
	for (int x = 0;x < 2; x++)
	{
		char tempChar = b[x];
		int hexInt = HexToInt(tempChar);
		tempLong = tempLong | hexInt<<((1-x)*4);
	}
	return (unsigned char) tempLong;
}

void CGEDecompressorDlg::OnBnClickedButtonsearch()
{
	int start = m_list.GetSelectionMark();
	if (m_list.GetSelectionMark() == -1)
	{
		if (m_list.GetItemCount() > 0)
			start = 0;
		else
			return;
	}

	if (lastSearchSpot == start)
	{
		start = lastSearchSpot + 1;
		if (lastSearchSpot >= m_list.GetItemCount())
			start = 0;
	}

	CString searchStr;
	mSearch.GetWindowText(searchStr);

	int searchLength = searchStr.GetLength();
	if (((searchLength % 2) != 0) && (searchLength > 0))
	{
		MessageBox("Must be multiple of 2 and > 0");
		return;
	}

	searchLength = searchLength / 2;

	unsigned char* search = new unsigned char[searchLength];
	for (int x = 0; x < searchLength; x++)
	{
		search[x] = StringToUnsignedChar(searchStr.Mid((x * 2), 2));
	}

	
	for (int x = start; x < m_list.GetItemCount(); x++)
	{
		ListUpdateStruct* list = ((ListUpdateStruct*)m_list.GetItemData(x));
		int size = GetSizeFile(list->tempLocation);
		if (searchLength >= size)
			continue;

		FILE* inFile = fopen(list->tempLocation, "rb");
		unsigned char* buffer = new unsigned char[size];
		fread(buffer, 1, size, inFile);
		fclose(inFile);

		for (int y = 0; y < (size - searchLength); y++)
		{
			bool match = true;
			for (int z = y; z < (y + searchLength); z++)
			{
				if (search[z - y] != buffer[z])
				{
					match = false;
					break;
				}

			}
			if (match)
			{
				m_list.SetItemState(x, LVIS_SELECTED, LVIS_SELECTED);
				m_list.SetItemState(x, LVIS_FOCUSED, LVIS_FOCUSED);
				CString tempStr;
				mFile.SetWindowText(list->tempLocation);
				delete [] buffer;
				delete [] search;
				lastSearchSpot = x;
				return;
			}
		}
		delete [] buffer;
	}

	delete [] search;
}

void CGEDecompressorDlg::OnCbnSelchangeCombo1()
{
	CString tempStr;
	m_gameselection.GetWindowText(tempStr);

	if (tempStr.Find("Banjo Tooie") != -1)
	{
		mCompressEncryptedButton.ShowWindow(SW_SHOW);
		mEncryptedFileNumber.ShowWindow(SW_SHOW);
		mFileNumberStatic.ShowWindow(SW_SHOW);
	}
	else
	{
		mCompressEncryptedButton.ShowWindow(SW_HIDE);
		mEncryptedFileNumber.ShowWindow(SW_HIDE);
		mFileNumberStatic.ShowWindow(SW_HIDE);
	}
}

unsigned short CGEDecompressorDlg::StringToUnsignedShort(CString inString)
{
	int tempA = inString.GetLength();
	if (inString.GetLength() < 4)
	{
		CString tempStr = inString;
		inString = "";
		for (int x = 0; x < (4-tempStr.GetLength()); x++)
		{
			inString = inString + "0";
		}
		inString = inString + tempStr;
	}
	char* b;
	b = inString.GetBuffer(0);
	unsigned long tempLong = 0;
	for (int x = 0;x < 4; x++)
	{
		char tempChar = b[x];
		int hexInt = HexToInt(tempChar);
		tempLong = tempLong | hexInt<<((3-x)*4);
	}
	return tempLong;
}


void CGEDecompressorDlg::OnBnClickedCompressfilebuttonencrypted()
{
	// TODO: Add your control notification handler code here
	CString fileOpen;
	CFileDialog m_ldFile(TRUE, NULL, "compressedfile.bin", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Bin File of Codes (*.*)|*.*|");
	int didRead = m_ldFile.DoModal();
	if ((didRead == IDCANCEL) || (m_ldFile.GetPathName() == ""))
		return;

	if (didRead == FALSE)
		return;

	CString gameNameStr;
	m_gameselection.GetWindowText(gameNameStr);

	if (GetZLibGameName(gameNameStr) != -1)
	{
		GECompression compressed;
		compressed.SetPath(directory);
		compressed.SetGame(GetZLibGameName(gameNameStr));

		CFileDialog m_svFile(FALSE, "bin", (m_ldFile.GetFileName() + ".bin"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Bin File of Codes (*.*)|*.*|");
		didRead = m_svFile.DoModal();
		if ((didRead == IDCANCEL) || (m_svFile.GetPathName() == ""))
			return;

		if (didRead == FALSE)
			return;

		compressed.CompressGZipFile(m_ldFile.GetPathName(), m_svFile.GetPathName(), false);

		FILE* inFile = fopen(m_svFile.GetPathName(), "rb");
		if (inFile == NULL)
		{
			MessageBox("Cannot open BT file", "Error");
			return;
		}

		unsigned long outSize = GetSizeFile(m_svFile.GetPathName());
		if (outSize > 0)
		{
			unsigned char* tempInputBuffer = new unsigned char[outSize];
			fread(tempInputBuffer, 1, outSize, inFile);

			fclose(inFile);

			CString tempStr;
			mEncryptedFileNumber.GetWindowText(tempStr);
			int fileNumber = StringToUnsignedShort(tempStr);

			DecryptBTFile(fileNumber, tempInputBuffer, tempInputBuffer, outSize);

			FILE* outFile = fopen(m_svFile.GetPathName(), "wb");
			if (outFile == NULL)
			{
				MessageBox("Cannot open encrypted BT file", "Error");
				return;
			}
			fwrite(tempInputBuffer, 1, outSize, outFile);
			fclose(outFile);

			delete [] tempInputBuffer;
		}
	}	
}
