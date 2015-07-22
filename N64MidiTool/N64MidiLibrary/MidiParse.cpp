#include "StdAfx.h"
#include "MidiParse.h"
#include <sys/stat.h>
#include "SupermanDecoder.h"
#include "rnc_deco.h"
#include "H20Decoder.h"
#include "TetrisphereDecoder.h"
#include "GECompression.h"

unsigned long CMidiParse::GetVLBytes(byte* vlByteArray, int& offset, unsigned long& original, byte*& altPattern, byte& altOffset, byte& altLength, bool includeFERepeats)
{
    unsigned long VLVal = 0; //Vlength Value.
    byte TempByte; //Byte value read.

    for (; ; )
    {
		if (altPattern != NULL)
		{
			TempByte = altPattern[altOffset];
			altOffset++;

			if ((altOffset == altLength) && (altPattern != NULL))
			{
				delete [] altPattern;
				altPattern = NULL;
				altOffset = 0;
				altLength = 0;
			}
		}
		else
		{
			TempByte = vlByteArray[offset];
			offset++;

			if ((TempByte == 0xFE) && (vlByteArray[offset] != 0xFE) && includeFERepeats)
			{
				byte repeatFirstByte = vlByteArray[offset];
				offset++;

				unsigned short repeatDistanceFromBeginningMarker = ((repeatFirstByte << 8) | vlByteArray[offset]);
				offset++;
				byte repeatCount = vlByteArray[offset];
				offset++;

				altPattern = new byte[repeatCount];
				for (int copy = ((offset - 4) - repeatDistanceFromBeginningMarker); copy < (((offset - 4) - repeatDistanceFromBeginningMarker) + repeatCount); copy++)
				{
					altPattern[copy - ((offset - 4) - repeatDistanceFromBeginningMarker)] = vlByteArray[copy];
				}
				altOffset = 0;
				altLength = repeatCount;

				TempByte = altPattern[altOffset];
				altOffset++;
			}
			else if ((TempByte == 0xFE) && (vlByteArray[offset] == 0xFE) && includeFERepeats)
			{
				// skip duplicate FEs
				offset++;
			}

			if ((altOffset == altLength) && (altPattern != NULL))
			{
				delete [] altPattern;
				altPattern = NULL;
				altOffset = 0;
				altLength = 0;
			}
		}
        if ((TempByte >> 7) == 0x1)
        {
            VLVal += TempByte;
            VLVal = VLVal << 8; //Shift to next byte in VLVal.
        }
        else
        {
            VLVal += TempByte;
            break;
        } 
    }
    
	original = VLVal;

	unsigned long Vlength = 0;

	for (int c = 0, a = 0; ;c += 8, a+= 7)
    {
       
        Vlength += (((VLVal >> c) & 0x7F) << a);
		if (c == 24)
			break;
    }
    return Vlength;
}




void CMidiParse::WriteVLBytes(FILE* outFile, unsigned long value, unsigned long length, bool includeFERepeats)
{
	byte tempByte;
	if (length == 1)
	{
		tempByte = value & 0xFF;
		fwrite(&tempByte, 1, 1, outFile);
	}
	else if (length == 2)
	{
		tempByte = (value >> 8) & 0xFF;
		fwrite(&tempByte, 1, 1, outFile);
		tempByte = value & 0xFF;
		fwrite(&tempByte, 1, 1, outFile);
	}
	else if (length == 3)
	{
		tempByte = (value >> 16) & 0xFF;
		fwrite(&tempByte, 1, 1, outFile);
		tempByte = (value >> 8) & 0xFF;
		fwrite(&tempByte, 1, 1, outFile);
		tempByte = value & 0xFF;
		fwrite(&tempByte, 1, 1, outFile);
	}
	else
	{
		tempByte = (value >> 24) & 0xFF;
		fwrite(&tempByte, 1, 1, outFile);
		tempByte = (value >> 8) & 0xFF;
		fwrite(&tempByte, 1, 1, outFile);
		tempByte = value & 0xFF;
		fwrite(&tempByte, 1, 1, outFile);
	}
}

unsigned long ReturnVLBytes(unsigned long value, unsigned long& length)
{
	byte subValue1 = (value >> 21) & 0x7F;
	byte subValue2 = (value >> 14) & 0x7F;
	byte subValue3 = (value >> 7) & 0x7F;
	byte subValue4 = (value >> 0) & 0x7F;

	if (subValue1 > 0)
	{
		unsigned long newValue = 0x80808000;
		newValue |= (subValue1 << 24);
		newValue |= (subValue2 << 16);
		newValue |= (subValue3 << 8);
		newValue |= subValue4;
		length = 4;
		return newValue;
	}
	else if (subValue2 > 0)
	{
		unsigned long newValue = 0x00808000;
		newValue |= (subValue2 << 16);
		newValue |= (subValue3 << 8);
		newValue |= subValue4;
		length = 3;
		return newValue;
	}
	else if (subValue3 > 0)
	{
		unsigned long newValue = 0x00008000;
		newValue |= (subValue3 << 8);
		newValue |= subValue4;
		length = 2;
		return newValue;
	}
	else
	{
		length = 1;
		return value;
	}
}

byte CMidiParse::ReadMidiByte(byte* vlByteArray, int& offset, byte*& altPattern, byte& altOffset, byte& altLength, bool includeFERepeats)
{
	byte returnByte;
	if (altPattern != NULL)
	{
		returnByte = altPattern[altOffset];
		altOffset++;
	}
	else
	{
		returnByte = vlByteArray[offset];
		offset++;

		if ((returnByte == 0xFE) && (vlByteArray[offset] != 0xFE) && includeFERepeats)
		{
			byte repeatFirstByte = vlByteArray[offset];
			offset++;

			unsigned long repeatDistanceFromBeginningMarker = ((repeatFirstByte << 8) | vlByteArray[offset]);
			offset++;
			byte repeatCount = vlByteArray[offset];
			offset++;

			altPattern = new byte[repeatCount];
			for (int copy = ((offset - 4) - repeatDistanceFromBeginningMarker); copy < (((offset - 4) - repeatDistanceFromBeginningMarker) + repeatCount); copy++)
			{
				altPattern[copy - ((offset - 4) - repeatDistanceFromBeginningMarker)] = vlByteArray[copy];
			}
			altOffset = 0;
			altLength = repeatCount;

			returnByte = altPattern[altOffset];
			altOffset++;
		}
		else if ((returnByte == 0xFE) && (vlByteArray[offset] == 0xFE) && includeFERepeats)
		{
			// skip duplicate FEs
			offset++;
		}
	}

	if ((altOffset == altLength) && (altPattern != NULL))
	{
		delete [] altPattern;
		altPattern = NULL;
		altOffset = 0;
		altLength = 0;
	}
	
	return returnByte;
}

CMidiParse::CMidiParse(void)
{
	char tempFolder[8000];
	::GetCurrentDirectory(8000, tempFolder);
	mainFolder.Format("%s\\", tempFolder);

	compress = new GECompression();

	for (int i = 0; i < 32; i++)
		trackEventCount[i] = 0;
	trackEvents = new TrackEvent *[0x20];
	for (unsigned int x = 0; x < 0x20; x++ )
		trackEvents[x] = new TrackEvent[0x10000];

	/* GE Mapping
	instrumentMidiToGEMapping[0x00] = 0x12;// 0x00
		instrumentMidiToGEMapping[0x01] = 0x12;// 0x01
		instrumentMidiToGEMapping[0x02] = 0x12;// 0x02
		instrumentMidiToGEMapping[0x03] = 0x12;// 0x03
		instrumentMidiToGEMapping[0x04] = 0x12;// 0x04
		instrumentMidiToGEMapping[0x05] = 0x12;// 0x05
		instrumentMidiToGEMapping[0x06] = 0x12;// 0x06
		instrumentMidiToGEMapping[0x07] = 0x12;// 0x07
		instrumentMidiToGEMapping[0x08] = 0x12;// 0x08
		instrumentMidiToGEMapping[0x09] = 0x0C;// 0x09
		instrumentMidiToGEMapping[0x0A] = 0x20;// 0x0A
		instrumentMidiToGEMapping[0x0B] = 0x20;// 0x0B
		instrumentMidiToGEMapping[0x0C] = 0x20;// 0x0C
		instrumentMidiToGEMapping[0x0D] = 0x16;// 0x0D
		instrumentMidiToGEMapping[0x0E] = 0x10;// 0x0E
		instrumentMidiToGEMapping[0x0F] = 0x20;// 0x0F
		instrumentMidiToGEMapping[0x10] = 0x12;// 0x10
		instrumentMidiToGEMapping[0x11] = 0x12;// 0x11
		instrumentMidiToGEMapping[0x12] = 0x12;// 0x12
		instrumentMidiToGEMapping[0x13] = 0x12;// 0x13
		instrumentMidiToGEMapping[0x14] = 0x12;// 0x14
		instrumentMidiToGEMapping[0x15] = 0x12;// 0x15
		instrumentMidiToGEMapping[0x16] = 0x12;// 0x16
		instrumentMidiToGEMapping[0x17] = 0x12;// 0x17
		instrumentMidiToGEMapping[0x18] = 0x12;// 0x18
		instrumentMidiToGEMapping[0x19] = 0x0B;// 0x19
		instrumentMidiToGEMapping[0x1A] = 0x0B;// 0x1A
		instrumentMidiToGEMapping[0x1B] = 0x0B;// 0x1B
		instrumentMidiToGEMapping[0x1C] = 0x18;// 0x1C
		instrumentMidiToGEMapping[0x1D] = 0x0B;// 0x1D
		instrumentMidiToGEMapping[0x1E] = 0x0B;// 0x1E
		instrumentMidiToGEMapping[0x1F] = 0x0B;// 0x1F
		instrumentMidiToGEMapping[0x20] = 0x0B;// 0x20
		instrumentMidiToGEMapping[0x21] = 0x04;// 0x21
		instrumentMidiToGEMapping[0x22] = 0x04;// 0x22
		instrumentMidiToGEMapping[0x23] = 0x04;// 0x23
		instrumentMidiToGEMapping[0x24] = 0x00;// 0x24
		instrumentMidiToGEMapping[0x25] = 0x04;// 0x25
		instrumentMidiToGEMapping[0x26] = 0x04;// 0x26
		instrumentMidiToGEMapping[0x27] = 0x04;// 0x27
		instrumentMidiToGEMapping[0x28] = 0x04;// 0x28
		instrumentMidiToGEMapping[0x29] = 0x04;// 0x29
		instrumentMidiToGEMapping[0x2A] = 0x04;// 0x2A
		instrumentMidiToGEMapping[0x2B] = 0x04;// 0x2B
		instrumentMidiToGEMapping[0x2C] = 0x12;// 0x2C
		instrumentMidiToGEMapping[0x2D] = 0x04;// 0x2D
		instrumentMidiToGEMapping[0x2E] = 0x03;// 0x2E
		instrumentMidiToGEMapping[0x2F] = 0x07;// 0x2F
		instrumentMidiToGEMapping[0x30] = 0x07;// 0x30
		instrumentMidiToGEMapping[0x31] = 0x09;// 0x31
		instrumentMidiToGEMapping[0x32] = 0x09;// 0x32
		instrumentMidiToGEMapping[0x33] = 0x09;// 0x33
		instrumentMidiToGEMapping[0x34] = 0x09;// 0x34
		instrumentMidiToGEMapping[0x35] = 0x09;// 0x35
		instrumentMidiToGEMapping[0x36] = 0x25;// 0x36
		instrumentMidiToGEMapping[0x37] = 0x25;// 0x37
		instrumentMidiToGEMapping[0x38] = 0x07;// 0x38
		instrumentMidiToGEMapping[0x39] = 0x07;// 0x39
		instrumentMidiToGEMapping[0x3A] = 0x06;// 0x3A
		instrumentMidiToGEMapping[0x3B] = 0x05;// 0x3B
		instrumentMidiToGEMapping[0x3C] = 0x05;// 0x3C
		instrumentMidiToGEMapping[0x3D] = 0x11;// 0x3D
		instrumentMidiToGEMapping[0x3E] = 0x07;// 0x3E
		instrumentMidiToGEMapping[0x3F] = 0x05;// 0x3F
		instrumentMidiToGEMapping[0x40] = 0x05;// 0x40
		instrumentMidiToGEMapping[0x41] = 0x1E;// 0x41
		instrumentMidiToGEMapping[0x42] = 0x1E;// 0x42
		instrumentMidiToGEMapping[0x43] = 0x1E;// 0x43
		instrumentMidiToGEMapping[0x44] = 0x1E;// 0x44
		instrumentMidiToGEMapping[0x45] = 0x28;// 0x45
		instrumentMidiToGEMapping[0x46] = 0x14;// 0x46
		instrumentMidiToGEMapping[0x47] = 0x1E;// 0x47
		instrumentMidiToGEMapping[0x48] = 0x1E;// 0x48
		instrumentMidiToGEMapping[0x49] = 0x24;// 0x49
		instrumentMidiToGEMapping[0x4A] = 0x24;// 0x4A
		instrumentMidiToGEMapping[0x4B] = 0x24;// 0x4B
		instrumentMidiToGEMapping[0x4C] = 0x24;// 0x4C
		instrumentMidiToGEMapping[0x4D] = 0x0A;// 0x4D
		instrumentMidiToGEMapping[0x4E] = 0x24;// 0x4E
		instrumentMidiToGEMapping[0x4F] = 0x24;// 0x4F
		instrumentMidiToGEMapping[0x50] = 0x24;// 0x50
		instrumentMidiToGEMapping[0x51] = 0x29;// 0x51
		instrumentMidiToGEMapping[0x52] = 0x29;// 0x52
		instrumentMidiToGEMapping[0x53] = 0x29;// 0x53
		instrumentMidiToGEMapping[0x54] = 0x29;// 0x54
		instrumentMidiToGEMapping[0x55] = 0x29;// 0x55
		instrumentMidiToGEMapping[0x56] = 0x29;// 0x56
		instrumentMidiToGEMapping[0x57] = 0x29;// 0x57
		instrumentMidiToGEMapping[0x58] = 0x29;// 0x58
		instrumentMidiToGEMapping[0x59] = 0x08;// 0x59
		instrumentMidiToGEMapping[0x5A] = 0x08;// 0x5A
		instrumentMidiToGEMapping[0x5B] = 0x08;// 0x5B
		instrumentMidiToGEMapping[0x5C] = 0x1F;// 0x5C
		instrumentMidiToGEMapping[0x5D] = 0x1F;// 0x5D
		instrumentMidiToGEMapping[0x5E] = 0x1F;// 0x5E
		instrumentMidiToGEMapping[0x5F] = 0x19;// 0x5F
		instrumentMidiToGEMapping[0x60] = 0x0D;// 0x60
		instrumentMidiToGEMapping[0x61] = 0x0D;// 0x61
		instrumentMidiToGEMapping[0x62] = 0x0D;// 0x62
		instrumentMidiToGEMapping[0x63] = 0x0D;// 0x63
		instrumentMidiToGEMapping[0x64] = 0x17;// 0x64
		instrumentMidiToGEMapping[0x65] = 0x17;// 0x65
		instrumentMidiToGEMapping[0x66] = 0x17;// 0x66
		instrumentMidiToGEMapping[0x67] = 0x17;// 0x67
		instrumentMidiToGEMapping[0x68] = 0x23;// 0x68
		instrumentMidiToGEMapping[0x69] = 0x23;// 0x69
		instrumentMidiToGEMapping[0x6A] = 0x23;// 0x6A
		instrumentMidiToGEMapping[0x6B] = 0x23;// 0x6B
		instrumentMidiToGEMapping[0x6C] = 0x23;// 0x6C
		instrumentMidiToGEMapping[0x6D] = 0x0F;// 0x6D
		instrumentMidiToGEMapping[0x6E] = 0x1E;// 0x6E
		instrumentMidiToGEMapping[0x6F] = 0x1E;// 0x6F
		instrumentMidiToGEMapping[0x70] = 0x1E;// 0x70
		instrumentMidiToGEMapping[0x71] = 0x2B;// 0x71
		instrumentMidiToGEMapping[0x72] = 0x1A;// 0x72
		instrumentMidiToGEMapping[0x73] = 0x1A;// 0x73
		instrumentMidiToGEMapping[0x74] = 0x1B;// 0x74
		instrumentMidiToGEMapping[0x75] = 0x15;// 0x75
		instrumentMidiToGEMapping[0x76] = 0x1C;// 0x76
		instrumentMidiToGEMapping[0x77] = 0x13;// 0x77
		instrumentMidiToGEMapping[0x78] = 0x24;// 0x78
		instrumentMidiToGEMapping[0x79] = 0x24;// 0x79
		instrumentMidiToGEMapping[0x7A] = 0x24;// 0x7A
		instrumentMidiToGEMapping[0x7B] = 0x24;// 0x7B
		instrumentMidiToGEMapping[0x7C] = 0x24;// 0x7C
		instrumentMidiToGEMapping[0x7D] = 0x24;// 0x7D
		instrumentMidiToGEMapping[0x7E] = 0x24;// 0x7E
		instrumentMidiToGEMapping[0x7F] = 0x1;// 0x7F
		*/
}

CMidiParse::~CMidiParse(void)
{
	for (unsigned int x = 0; x < 0x20; x++ )
		delete [] trackEvents[x];
	delete [] trackEvents;

	delete compress;
}


void CMidiParse::GloverMidiToMidi(byte* inputMID, int inputSize, CString outFileName, int& numberInstruments, unsigned long division)
{
	// UNFINISHED possibly wrong
	numberInstruments = 0;
	try
	{
		FILE* outFile = fopen(outFileName, "wb");
		if (outFile == NULL)
		{
			MessageBox(NULL, "Error outputting file", "Error", NULL);
			return;
		}

		unsigned long trackSize = 1;
		

		unsigned long tempLong = Flip32Bit(0x4D546864);
		fwrite(&tempLong, 1 ,4 , outFile);
		tempLong = Flip32Bit(0x00000006);
		fwrite(&tempLong, 1 ,4 , outFile);
		tempLong = Flip32Bit(0x00010000 | trackSize);
		fwrite(&tempLong, 1 ,4 , outFile);

		unsigned short tempShort = division;
		tempShort = Flip16Bit(tempShort);
		fwrite(&tempShort, 1 ,2 , outFile);

		int counterTrack = 0;



		unsigned long absoluteTime = 0;

		int trackEventCount = 0;
		TrackEvent* trackEvents = new TrackEvent[0x100000];

		for (int j = 0; j < 0x100000; j++)
		{
			trackEvents[j].contents = NULL;
			trackEvents[j].obsoleteEvent = false;
			trackEvents[j].deltaTime = 0;
			trackEvents[j].absoluteTime = 0;

		}

		int position = 0;	

		tempLong = Flip32Bit(0x4D54726B);
		fwrite(&tempLong, 1 ,4 , outFile);

		int previousEventValue = 0;

		byte* repeatPattern = NULL;
		byte altOffset = 0;
		byte altLength = 0;

		bool endFlag = false;
		while ((position < inputSize) && !endFlag)
		{
			if (trackEventCount > 0x90000)
				return;

			int timePosition = position;

			byte eventVal = 0x90;
			byte statusBit = false;

			unsigned long original;
			// trackEvents[trackEventCount].deltaTime is for loops
			unsigned long timeTag = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, true);

			trackEvents[trackEventCount].deltaTime += timeTag;

			absoluteTime += timeTag;
			trackEvents[trackEventCount].absoluteTime = absoluteTime;

			byte curEventVal = 0x90;

			byte noteNumber;
			if (statusBit)
			{
				trackEvents[trackEventCount].type = previousEventValue;
				noteNumber = eventVal;
				curEventVal = previousEventValue;
			}
			else
			{
				trackEvents[trackEventCount].type = eventVal;
				noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
				curEventVal = eventVal;
			}
			byte velocity = 0x7F; // TODO

			unsigned long timeDuration = 0x10; // TODO

			

			trackEvents[trackEventCount].durationTime = timeDuration; // to be filled in
			trackEvents[trackEventCount].contentSize = 2;
			trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
			trackEvents[trackEventCount].contents[0] = noteNumber;
			trackEvents[trackEventCount].contents[1] = velocity;

			trackEventCount++;

			if (!statusBit)
				previousEventValue = eventVal;
		}

		for (int eventCount = 0; eventCount < trackEventCount; eventCount++)
		{
			if (trackEventCount > 0x90000)
				return;

			TrackEvent trackEvent = trackEvents[eventCount];
			if ((trackEvent.type >= 0x90) && (trackEvent.type < 0xA0))
			{
				// need to split out
				if (trackEvent.durationTime > 0)
				{

					unsigned long shutoffTime = (trackEvent.absoluteTime + trackEvent.durationTime);

					if (eventCount != (trackEventCount - 1))
					{

						for (int i = (eventCount+1); i < trackEventCount; i++)
						{
							if ((trackEvents[i].absoluteTime > shutoffTime) && (i != (trackEventCount - 1)))
							{
								for (int j = (trackEventCount - 1); j >= i; j--)
								{
									trackEvents[j+1].absoluteTime = trackEvents[j].absoluteTime;
									trackEvents[j+1].contentSize = trackEvents[j].contentSize;
									if (trackEvents[j+1].contents != NULL)
									{
										delete [] trackEvents[j+1].contents;
										trackEvents[j+1].contents = NULL;
									}
									trackEvents[j+1].contents = new byte[trackEvents[j].contentSize];
									for (int r = 0; r < trackEvents[j].contentSize; r++)
									{
										trackEvents[j+1].contents[r] = trackEvents[j].contents[r];
									}
									trackEvents[j+1].deltaTime = trackEvents[j].deltaTime;
									trackEvents[j+1].durationTime = trackEvents[j].durationTime;
									trackEvents[j+1].obsoleteEvent = trackEvents[j].obsoleteEvent;
									trackEvents[j+1].type = trackEvents[j].type;
								}

								trackEvents[i].type = trackEvents[eventCount].type;
								trackEvents[i].absoluteTime = shutoffTime;
								trackEvents[i].deltaTime = (trackEvents[i].absoluteTime - trackEvents[i-1].absoluteTime);
								trackEvents[i].contentSize = trackEvents[eventCount].contentSize;
								trackEvents[i].durationTime = 0;

								if (trackEvents[i].contents != NULL)
								{
									delete [] trackEvents[i].contents;
								}

								trackEvents[i].contents = new byte[trackEvents[i].contentSize];
								trackEvents[i].contents[0] = trackEvents[eventCount].contents[0];
								trackEvents[i].contents[1] = 0;

								trackEvents[i+1].deltaTime = (trackEvents[i+1].absoluteTime - trackEvents[i].absoluteTime);

								if (trackEvents[i].deltaTime > 0xFF000000)
								{
									int a =1;
								}

								trackEventCount++;
								break;
							}
							else if (i == (trackEventCount - 1))
							{
								trackEvents[i+1].absoluteTime = shutoffTime; // move end to end
								trackEvents[i+1].contentSize = trackEvents[i].contentSize;
								if (trackEvents[i+1].contents != NULL)
								{
									delete [] trackEvents[i+1].contents;
									trackEvents[i+1].contents = NULL;
								}
								trackEvents[i+1].contents = new byte[trackEvents[i].contentSize];
								for (int r = 0; r < trackEvents[i].contentSize; r++)
								{
									trackEvents[i+1].contents[r] = trackEvents[i].contents[r];
								}
								trackEvents[i+1].deltaTime = trackEvents[i].deltaTime;
								trackEvents[i+1].durationTime = trackEvents[i].durationTime;
								trackEvents[i+1].obsoleteEvent = trackEvents[i].obsoleteEvent;
								trackEvents[i+1].type = trackEvents[i].type;


								trackEvents[i].type = trackEvents[eventCount].type;
								trackEvents[i].absoluteTime = shutoffTime;
								trackEvents[i].deltaTime = (trackEvents[i].absoluteTime - trackEvents[i - 1].absoluteTime);
								trackEvents[i].contentSize = trackEvents[eventCount].contentSize;
								trackEvents[i].durationTime = 0;
								trackEvents[i].contents = new byte[trackEvents[i].contentSize];
								trackEvents[i].contents[0] = trackEvents[eventCount].contents[0];
								trackEvents[i].contents[1] = 0;

								trackEvents[i+1].deltaTime = (trackEvents[i+1].absoluteTime - trackEvents[i].absoluteTime);

								trackEventCount++;
								break;
							}
						}
					}
					else
					{
						trackEvents[eventCount+1].absoluteTime = shutoffTime; // move end to end
						trackEvents[eventCount+1].contentSize = trackEvents[eventCount].contentSize;
						if (trackEvents[eventCount+1].contents != NULL)
						{
							delete [] trackEvents[eventCount+1].contents;
							trackEvents[eventCount+1].contents = NULL;
						}
						trackEvents[eventCount+1].contents = new byte[trackEvents[eventCount].contentSize];
						for (int r = 0; r < trackEvents[eventCount].contentSize; r++)
						{
							trackEvents[eventCount+1].contents[r] = trackEvents[eventCount].contents[r];
						}
						trackEvents[eventCount+1].deltaTime = trackEvents[eventCount].deltaTime;
						trackEvents[eventCount+1].durationTime = trackEvents[eventCount].durationTime;
						trackEvents[eventCount+1].obsoleteEvent = trackEvents[eventCount].obsoleteEvent;
						trackEvents[eventCount+1].type = trackEvents[eventCount].type;


						trackEvents[eventCount].type = trackEvents[eventCount].type;
						trackEvents[eventCount].absoluteTime = shutoffTime;
						if ((trackEvents[eventCount].absoluteTime - trackEvents[eventCount - 1].absoluteTime) > 0xFF000000)
						{
							int a =1;
						}
						trackEvents[eventCount].deltaTime = (trackEvents[eventCount].absoluteTime - trackEvents[eventCount - 1].absoluteTime);
						trackEvents[eventCount].contentSize = trackEvents[eventCount].contentSize;
						trackEvents[eventCount].durationTime = 0;
						trackEvents[eventCount].contents = new byte[trackEvents[eventCount].contentSize];
						trackEvents[eventCount].contents[0] = trackEvents[eventCount].contents[0];
						trackEvents[eventCount].contents[1] = 0;

						trackEvents[eventCount+1].deltaTime = (trackEvents[eventCount+1].absoluteTime - trackEvents[eventCount].absoluteTime);
						if (trackEvents[eventCount].deltaTime > 0xFF000000)
						{
							int a =1;
						}
						trackEventCount++;
					}
				}
			}
		}

		
		unsigned long timeOffset = 0;

		unsigned long sizeData = 0;
		byte previousTrackEvent = 0x0;

		
		for (int j = 0; j < trackEventCount; j++)
		{
			TrackEvent trackEvent =  trackEvents[j];
			if (trackEvent.obsoleteEvent)
			{
				timeOffset += trackEvent.deltaTime;
			}
			else
			{
				unsigned long lengthTimeDelta = 0;
				unsigned long timeDelta = ReturnVLBytes((trackEvent.deltaTime + timeOffset), lengthTimeDelta);
				timeOffset = 0;

				sizeData += lengthTimeDelta;


				if ((trackEvent.type != previousTrackEvent) || (trackEvent.type == 0xFF))
				{
					sizeData += 1;
				}

				sizeData += trackEvent.contentSize;

				previousTrackEvent = trackEvent.type;
			}
		}

		tempLong = Flip32Bit(sizeData);
		fwrite(&tempLong,1, 4, outFile);

		timeOffset = 0;
		previousTrackEvent = 0x0;
		for (int eventCount = 0; eventCount < trackEventCount; eventCount++)
		{
			TrackEvent trackEvent = trackEvents[eventCount];

			if (trackEvent.obsoleteEvent)
			{
				timeOffset += trackEvent.deltaTime;
			}
			else
			{
				unsigned long lengthTimeDelta = 0;
				unsigned long timeDelta = ReturnVLBytes((trackEvent.deltaTime + timeOffset), lengthTimeDelta);
				timeOffset = 0;
				WriteVLBytes(outFile, timeDelta, lengthTimeDelta, true);

				if ((trackEvent.type != previousTrackEvent) || (trackEvent.type == 0xFF))
				{
					fwrite(&trackEvent.type, 1, 1, outFile);
				}

				fwrite(trackEvent.contents, 1, trackEvent.contentSize, outFile);

				previousTrackEvent = trackEvent.type;
			}
		}

		for (int eventCount = 0; eventCount < trackEventCount; eventCount++)
		{
			if (trackEvents[eventCount].contents != NULL)
			{
				delete [] trackEvents[eventCount].contents;
				trackEvents[eventCount].contents = NULL;
			}
		}

		counterTrack++;

		delete [] trackEvents;


	

		fflush(outFile);	
		fclose(outFile);
	}
	catch (...)
	{
		MessageBox(NULL, "Error exporting", "Error", NULL);
	}
}


void CMidiParse::GEMidiToMidi(byte* inputMID, int inputSize, CString outFileName, int& numberInstruments, bool& hasLoopPoint, int& loopStart, int& loopEnd)
{
	numberInstruments = 0;
	try
	{
		FILE* outFile = fopen(outFileName, "wb");
		if (outFile == NULL)
		{
			MessageBox(NULL, "Error outputting file", "Error", NULL);
			return;
		}

		unsigned long lengthHeader = 0x44;

		// parse midi
		
		int trackSize = 0;
		for (int i = 0; i < (lengthHeader - 4); i+=4) // ignore last 00000180
		{
			unsigned long offset = CharArrayToLong(&inputMID[i]);
			if (offset != 0)
				trackSize++;
		}

		unsigned long tempLong = Flip32Bit(0x4D546864);
		fwrite(&tempLong, 1 ,4 , outFile);
		tempLong = Flip32Bit(0x00000006);
		fwrite(&tempLong, 1 ,4 , outFile);
		tempLong = Flip32Bit(0x00010000 | trackSize);
		fwrite(&tempLong, 1 ,4 , outFile);

		unsigned long division = CharArrayToLong(&inputMID[0x40]);

		unsigned short tempShort = division;
		tempShort = Flip16Bit(tempShort);
		fwrite(&tempShort, 1 ,2 , outFile);

		int counterTrack = 0;

		for (int iii = 0; iii < (lengthHeader - 4); iii+=4) // ignore last 00000180
		{
			unsigned long absoluteTime = 0;

			int trackEventCount = 0;
			TrackEvent* trackEvents = new TrackEvent[0x100000];

			for (int j = 0; j < 0x100000; j++)
			{
				trackEvents[j].contents = NULL;
				trackEvents[j].obsoleteEvent = false;
				trackEvents[j].deltaTime = 0;
				trackEvents[j].absoluteTime = 0;

			}

			unsigned long offset = CharArrayToLong(&inputMID[iii]);
			
			int position = offset;	

			if (position != 0)
			{
				tempLong = Flip32Bit(0x4D54726B);
				fwrite(&tempLong, 1 ,4 , outFile);
	
				int previousEventValue = 0;

				byte* repeatPattern = NULL;
				byte altOffset = 0;
				byte altLength = 0;

				bool endFlag = false;
				while ((position < inputSize) && !endFlag)
				{
					if (trackEventCount > 0x90000)
						return;

					int timePosition = position;

					unsigned long original;
					// trackEvents[trackEventCount].deltaTime is for loops
					unsigned long timeTag = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, true);

					trackEvents[trackEventCount].deltaTime += timeTag;

					absoluteTime += timeTag;
					trackEvents[trackEventCount].absoluteTime = absoluteTime;

					int vlLength = 0;
					byte eventVal = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

					bool statusBit = false;

					if (eventVal < 0x80)
					{
						// continuation
						statusBit = true;
					}
					else
					{
						statusBit = false;
					}

					if ((eventVal == 0xFF) || (statusBit && (previousEventValue == 0xFF))) // meta event
					{
						byte subType;
						if (statusBit)
							subType = eventVal;
						else
							subType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						if (subType == 0x51) // tempo
						{
							int microsecondsSinceQuarterNote = ((((ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true));

							trackEvents[trackEventCount].type = 0xFF;
							trackEvents[trackEventCount].contentSize = 5;
							trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
							
							trackEvents[trackEventCount].contents[0] = 0x51;
							trackEvents[trackEventCount].contents[1] = 0x3;
							trackEvents[trackEventCount].contents[2] = ((microsecondsSinceQuarterNote >> 16) & 0xFF);
							trackEvents[trackEventCount].contents[3] = ((microsecondsSinceQuarterNote >> 8) & 0xFF);
							trackEvents[trackEventCount].contents[4] = ((microsecondsSinceQuarterNote >> 0) & 0xFF);

							trackEventCount++;

							
							int MICROSECONDS_PER_MINUTE = 60000000;
							float beatsPerMinute = (float)MICROSECONDS_PER_MINUTE / (float)microsecondsSinceQuarterNote;
						}
						else if (subType == 0x2D) // end loop
						{
							byte loopCount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							byte currentLoopCount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							unsigned long offsetToBeginningLoop = ((((((ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false));
							
							hasLoopPoint = true;
							loopEnd = absoluteTime;
						}
						else if (subType == 0x2E) // start loop
						{
							byte loopNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							byte endLoop = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							if (endLoop != 0xFF)
							{
								// is this used?
							}
							hasLoopPoint = true;
							loopStart = absoluteTime;
						}
						else if (subType == 0x2F)
						{
							trackEvents[trackEventCount].type = 0xFF;
							trackEvents[trackEventCount].contentSize = 2;
							trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
							
							trackEvents[trackEventCount].contents[0] = 0x2F;
							trackEvents[trackEventCount].contents[1] = 0x0;

							trackEventCount++;

							endFlag = true;
						}

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if ((eventVal >= 0x90 && eventVal < 0xA0) || (statusBit && (previousEventValue >= 0x90) && (previousEventValue < 0xA0)))
					{
						byte curEventVal;

						byte noteNumber;
						if (statusBit)
						{
							trackEvents[trackEventCount].type = previousEventValue;
							noteNumber = eventVal;
							curEventVal = previousEventValue;
						}
						else
						{
							trackEvents[trackEventCount].type = eventVal;
							noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							curEventVal = eventVal;
						}
						byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						unsigned long timeDuration = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, true);

						

						trackEvents[trackEventCount].durationTime = timeDuration; // to be filled in
						trackEvents[trackEventCount].contentSize = 2;
						trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
						trackEvents[trackEventCount].contents[0] = noteNumber;
						trackEvents[trackEventCount].contents[1] = velocity;

						trackEventCount++;

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xB0) && (eventVal < 0xC0)) || (statusBit && (previousEventValue >= 0xB0) && (previousEventValue < 0xC0))) // controller change
					{
						CString controllerTypeText = "";
						byte controllerType;
						
						if (statusBit)
						{
							controllerType = eventVal;
							trackEvents[trackEventCount].type = previousEventValue;
						}
						else
						{
							controllerType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							trackEvents[trackEventCount].type = eventVal;
						}
						byte controllerValue = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						trackEvents[trackEventCount].contentSize = 2;
						trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
						trackEvents[trackEventCount].contents[0] = controllerType;
						trackEvents[trackEventCount].contents[1] = controllerValue;

						trackEventCount++;

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xC0) && (eventVal < 0xD0)) || (statusBit && (previousEventValue >= 0xC0) && (previousEventValue < 0xD0))) // change instrument
					{
						byte instrument;
						if (statusBit)
						{
							instrument = eventVal;
							trackEvents[trackEventCount].type = previousEventValue;
						}
						else
						{
							instrument = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							trackEvents[trackEventCount].type = eventVal;
						}

						trackEvents[trackEventCount].contentSize = 1;
						trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
						trackEvents[trackEventCount].contents[0] = instrument;
						if (instrument >= numberInstruments)
							numberInstruments = (instrument + 1);

						trackEventCount++;

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xD0) && (eventVal < 0xE0)) || (statusBit && (previousEventValue >= 0xD0) && (previousEventValue < 0xE0))) // channel aftertouch
					{
						byte amount;
						if (statusBit)
						{
							amount = eventVal;
							trackEvents[trackEventCount].type = previousEventValue;
						}
						else
						{
							amount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							trackEvents[trackEventCount].type = eventVal;
						}

						trackEvents[trackEventCount].contentSize = 1;
						trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
						trackEvents[trackEventCount].contents[0] = amount;

						trackEventCount++;

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xE0) && (eventVal < 0xF0)) || (statusBit && (previousEventValue >= 0xE0) && (previousEventValue < 0xF0))) // pitch bend
					{
						byte valueLSB;
						if (statusBit)
						{
							valueLSB = eventVal;
							trackEvents[trackEventCount].type = previousEventValue;
						}
						else
						{
							valueLSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							trackEvents[trackEventCount].type = eventVal;
						}
						
						byte valueMSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						trackEvents[trackEventCount].contentSize = 2;
						trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
						trackEvents[trackEventCount].contents[0] = valueLSB;
						trackEvents[trackEventCount].contents[1] = valueMSB;

						trackEventCount++;

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (eventVal == 0xFE) // repeat operation
					{
						// should not be here...

						// no prev event set
					}
					else
					{
						fprintf(outFile, "%02X ERROR MISSING PARSE OF TYPE\n", eventVal);
					}

				}

				for (int eventCount = 0; eventCount < trackEventCount; eventCount++)
				{
					if (trackEventCount > 0x90000)
						return;

					TrackEvent trackEvent = trackEvents[eventCount];
					if ((trackEvent.type >= 0x90) && (trackEvent.type < 0xA0))
					{
						// need to split out
						if (trackEvent.durationTime > 0)
						{

							unsigned long shutoffTime = (trackEvent.absoluteTime + trackEvent.durationTime);

							if (eventCount != (trackEventCount - 1))
							{

								for (int i = (eventCount+1); i < trackEventCount; i++)
								{
									if ((trackEvents[i].absoluteTime > shutoffTime) && (i != (trackEventCount - 1)))
									{
										for (int j = (trackEventCount - 1); j >= i; j--)
										{
											trackEvents[j+1].absoluteTime = trackEvents[j].absoluteTime;
											trackEvents[j+1].contentSize = trackEvents[j].contentSize;
											if (trackEvents[j+1].contents != NULL)
											{
												delete [] trackEvents[j+1].contents;
												trackEvents[j+1].contents = NULL;
											}
											trackEvents[j+1].contents = new byte[trackEvents[j].contentSize];
											for (int r = 0; r < trackEvents[j].contentSize; r++)
											{
												trackEvents[j+1].contents[r] = trackEvents[j].contents[r];
											}
											trackEvents[j+1].deltaTime = trackEvents[j].deltaTime;
											trackEvents[j+1].durationTime = trackEvents[j].durationTime;
											trackEvents[j+1].obsoleteEvent = trackEvents[j].obsoleteEvent;
											trackEvents[j+1].type = trackEvents[j].type;
										}

										trackEvents[i].type = trackEvents[eventCount].type;
										trackEvents[i].absoluteTime = shutoffTime;
										trackEvents[i].deltaTime = (trackEvents[i].absoluteTime - trackEvents[i-1].absoluteTime);
										trackEvents[i].contentSize = trackEvents[eventCount].contentSize;
										trackEvents[i].durationTime = 0;


										if (trackEvents[i].contents != NULL)
										{
											delete [] trackEvents[i].contents;
										}

										trackEvents[i].contents = new byte[trackEvents[i].contentSize];
										trackEvents[i].contents[0] = trackEvents[eventCount].contents[0];
										trackEvents[i].contents[1] = 0;

										trackEvents[i+1].deltaTime = (trackEvents[i+1].absoluteTime - trackEvents[i].absoluteTime);

										if (trackEvents[i].deltaTime > 0xFF000000)
										{
											int a =1;
										}

										trackEventCount++;
										break;
									}
									else if (i == (trackEventCount - 1))
									{
										trackEvents[i+1].absoluteTime = shutoffTime; // move end to end
										trackEvents[i+1].contentSize = trackEvents[i].contentSize;
										if (trackEvents[i+1].contents != NULL)
										{
											delete [] trackEvents[i+1].contents;
											trackEvents[i+1].contents = NULL;
										}
										trackEvents[i+1].contents = new byte[trackEvents[i].contentSize];
										for (int r = 0; r < trackEvents[i].contentSize; r++)
										{
											trackEvents[i+1].contents[r] = trackEvents[i].contents[r];
										}
										trackEvents[i+1].deltaTime = trackEvents[i].deltaTime;
										trackEvents[i+1].durationTime = trackEvents[i].durationTime;
										trackEvents[i+1].obsoleteEvent = trackEvents[i].obsoleteEvent;
										trackEvents[i+1].type = trackEvents[i].type;


										trackEvents[i].type = trackEvents[eventCount].type;
										trackEvents[i].absoluteTime = shutoffTime;
										trackEvents[i].deltaTime = (trackEvents[i].absoluteTime - trackEvents[i - 1].absoluteTime);
										trackEvents[i].contentSize = trackEvents[eventCount].contentSize;
										trackEvents[i].durationTime = 0;

										if (trackEvents[i].contents != NULL)
										{
											delete [] trackEvents[i].contents;
										}

										trackEvents[i].contents = new byte[trackEvents[i].contentSize];
										trackEvents[i].contents[0] = trackEvents[eventCount].contents[0];
										trackEvents[i].contents[1] = 0;

										trackEvents[i+1].deltaTime = (trackEvents[i+1].absoluteTime - trackEvents[i].absoluteTime);

										trackEventCount++;
										break;
									}
								}
							}
							else
							{
								trackEvents[eventCount+1].absoluteTime = shutoffTime; // move end to end
								trackEvents[eventCount+1].contentSize = trackEvents[eventCount].contentSize;
								if (trackEvents[eventCount+1].contents != NULL)
								{
									delete [] trackEvents[eventCount+1].contents;
									trackEvents[eventCount+1].contents = NULL;
								}
								trackEvents[eventCount+1].contents = new byte[trackEvents[eventCount].contentSize];
								for (int r = 0; r < trackEvents[eventCount].contentSize; r++)
								{
									trackEvents[eventCount+1].contents[r] = trackEvents[eventCount].contents[r];
								}
								trackEvents[eventCount+1].deltaTime = trackEvents[eventCount].deltaTime;
								trackEvents[eventCount+1].durationTime = trackEvents[eventCount].durationTime;
								trackEvents[eventCount+1].obsoleteEvent = trackEvents[eventCount].obsoleteEvent;
								trackEvents[eventCount+1].type = trackEvents[eventCount].type;


								trackEvents[eventCount].type = trackEvents[eventCount].type;
								trackEvents[eventCount].absoluteTime = shutoffTime;
								if ((trackEvents[eventCount].absoluteTime - trackEvents[eventCount - 1].absoluteTime) > 0xFF000000)
								{
									int a =1;
								}
								trackEvents[eventCount].deltaTime = (trackEvents[eventCount].absoluteTime - trackEvents[eventCount - 1].absoluteTime);
								trackEvents[eventCount].contentSize = trackEvents[eventCount].contentSize;
								trackEvents[eventCount].durationTime = 0;
								trackEvents[eventCount].contents = new byte[trackEvents[eventCount].contentSize];
								trackEvents[eventCount].contents[0] = trackEvents[eventCount].contents[0];
								trackEvents[eventCount].contents[1] = 0;

								trackEvents[eventCount+1].deltaTime = (trackEvents[eventCount+1].absoluteTime - trackEvents[eventCount].absoluteTime);
								if (trackEvents[eventCount].deltaTime > 0xFF000000)
								{
									int a =1;
								}
								trackEventCount++;
							}
						}
					}
				}

				
				unsigned long timeOffset = 0;

				unsigned long sizeData = 0;
				byte previousTrackEvent = 0x0;

				
				for (int j = 0; j < trackEventCount; j++)
				{
					TrackEvent trackEvent =  trackEvents[j];
					if (trackEvent.obsoleteEvent)
					{
						timeOffset += trackEvent.deltaTime;
					}
					else
					{
						unsigned long lengthTimeDelta = 0;
						unsigned long timeDelta = ReturnVLBytes((trackEvent.deltaTime + timeOffset), lengthTimeDelta);
						timeOffset = 0;

						sizeData += lengthTimeDelta;


						if ((trackEvent.type != previousTrackEvent) || (trackEvent.type == 0xFF))
						{
							sizeData += 1;
						}

						sizeData += trackEvent.contentSize;

						previousTrackEvent = trackEvent.type;
					}
				}

				tempLong = Flip32Bit(sizeData);
				fwrite(&tempLong,1, 4, outFile);

				timeOffset = 0;
				previousTrackEvent = 0x0;
				for (int eventCount = 0; eventCount < trackEventCount; eventCount++)
				{
					TrackEvent trackEvent = trackEvents[eventCount];

					if (trackEvent.obsoleteEvent)
					{
						timeOffset += trackEvent.deltaTime;
					}
					else
					{
						unsigned long lengthTimeDelta = 0;
						unsigned long timeDelta = ReturnVLBytes((trackEvent.deltaTime + timeOffset), lengthTimeDelta);
						timeOffset = 0;
						WriteVLBytes(outFile, timeDelta, lengthTimeDelta, true);

						if ((trackEvent.type != previousTrackEvent) || (trackEvent.type == 0xFF))
						{
							fwrite(&trackEvent.type, 1, 1, outFile);
						}

						fwrite(trackEvent.contents, 1, trackEvent.contentSize, outFile);

						previousTrackEvent = trackEvent.type;
					}
				}
			}
			else
			{
				
			}

			for (int eventCount = 0; eventCount < trackEventCount; eventCount++)
			{
				if (trackEvents[eventCount].contents != NULL)
				{
					delete [] trackEvents[eventCount].contents;
					trackEvents[eventCount].contents = NULL;
				}
			}

			counterTrack++;

			delete [] trackEvents;
		}



	

		fflush(outFile);	
		fclose(outFile);
	}
	catch (...)
	{
		MessageBox(NULL, "Error exporting", "Error", NULL);
	}
}

void CMidiParse::MMLToMidi(byte* inputMID, int inputSize, CString outFileName, int& numberInstruments)
{
	/*numberInstruments = 1;
	try
	{
		FILE* outFile = fopen(outFileName, "wb");
		if (outFile == NULL)
		{
			MessageBox(NULL, "Error outputting file", "Error", NULL);
			return;
		}

		int currentSpot = 0;

		int trackSize = 1;  // add 1 for main one
		// count tracks first
		while (currentSpot < inputSize)
		{
			if ((command >= 0x90) && (command < 0xA0)) // load track
			{
				trackSize++;
			}
			else if (command == 0xFF)
			{
				break;
			}
		}

		currentSpot = 0;

		unsigned long tempLong = Flip32Bit(0x4D546864);
		fwrite(&tempLong, 1 ,4 , outFile);
		tempLong = Flip32Bit(0x00000006);
		fwrite(&tempLong, 1 ,4 , outFile);
		tempLong = Flip32Bit(0x00010000 | trackSize);
		fwrite(&tempLong, 1 ,4 , outFile);

		unsigned short tempShort = 0x400; // 1024 delta per quarter note
		tempShort = Flip16Bit(tempShort);
		fwrite(&tempShort, 1 ,2 , outFile);


		while (currentSpot < inputSize)
		{
			unsigned char command = inputMID[currentSpot];
			if (command == 0xD3) // Header/type
			{
				unsigned char headerBit = inputMID[currentSpot+1];
				currentSpot += 2;
			}
			else if ((command >= 0x90) && (command < 0xA0)) // load track
			{
				unsigned short offset = (inputMID[currentSpot+1] << 8) | inputMID[currentSpot+2];
				currentSpot += 3;
			}
			else if (command == 0xD6) // Disable channels
			{
				unsigned short channelsBitflags = (inputMID[currentSpot+1] << 8) | inputMID[currentSpot+2];
				currentSpot += 3;
			}
			else if (command == 0xDB) // Master Volume
			{
				unsigned short volume = (inputMID[currentSpot+1]);
				currentSpot += 2;
			}
			else if (command == 0xDD) // Tempo BPM
			{
				unsigned short bpm = (inputMID[currentSpot+1]);
				currentSpot += 2;
			}
			else if (command == 0xD5) // Unknown
			{
				currentSpot += 3;
			}
			else if (command == 0xD7) // Enable channels
			{
				unsigned short channelsBitflags = (inputMID[currentSpot+1] << 8) | inputMID[currentSpot+2];
				currentSpot += 3;
			}
			else if (command == 0xFD) // Timestamp
			{
				
			}
			else if (command == 0xFB) // Loop
			{
				
			}
			else if (command == 0xFF) // End
			{
				break;
			}
		}

		while ((

		fflush(outFile);	
		fclose(outFile);
	}
	catch (...)
	{
		MessageBox(NULL, "Error exporting", "Error", NULL);
	}*/
}

void CMidiParse::MIDxMidiToMidi(byte* inputMID, int inputSize, CString outFileName, int& numberInstruments)
{
	numberInstruments = 1;
	try
	{
		FILE* outFile = fopen(outFileName, "wb");
		if (outFile == NULL)
		{
			MessageBox(NULL, "Error outputting file", "Error", NULL);
			return;
		}

		// parse midi
		
		int trackSize = inputSize;

		unsigned long tempLong = Flip32Bit(0x4D546864);
		fwrite(&tempLong, 1 ,4 , outFile);
		tempLong = Flip32Bit(0x00000006);
		fwrite(&tempLong, 1 ,4 , outFile);
		tempLong = Flip32Bit(0x00010000 | 0x0001); // num tracks
		fwrite(&tempLong, 1 ,4 , outFile);

		unsigned long division = 0x01E0;

		unsigned short tempShort = division;
		tempShort = Flip16Bit(tempShort);
		fwrite(&tempShort, 1 ,2 , outFile);

		unsigned long absoluteTime = 0;

		int trackEventCount = 0;
		TrackEvent* trackEvents = new TrackEvent[0x90000];

		for (int j = 0; j < 0x90000; j++)
			trackEvents[j].contents = NULL;

		int position = 4;	

		tempLong = Flip32Bit(0x4D54726B);
		fwrite(&tempLong, 1 ,4 , outFile);

		byte* repeatPattern = NULL;
		byte altOffset = 0;
		byte altLength = 0;

		bool endFlag = false;
		while ((position < inputSize) && !endFlag)
		{
			int timePosition = position;

			unsigned long original;
			unsigned long timeTag;
			timeTag = inputMID[position];
			position++;
			if (timeTag > 0x7F)
				timeTag = (timeTag & 0x7F);
			
			trackEvents[trackEventCount].deltaTime = timeTag;
			trackEvents[trackEventCount].obsoleteEvent = false;

			absoluteTime += timeTag;
			trackEvents[trackEventCount].absoluteTime = absoluteTime;

			int vlLength = 0;
			byte eventVal = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

			if ((eventVal >= 0xF0) && (eventVal <= 0xFF)) // meta event
			{
				byte  subType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				if (subType == 0x51) // tempo
				{
					int microsecondsSinceQuarterNote = ((((ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false));

					trackEvents[trackEventCount].type = 0xFF;
					trackEvents[trackEventCount].contentSize = 5;
					trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
					
					trackEvents[trackEventCount].contents[0] = 0x51;
					trackEvents[trackEventCount].contents[1] = 0x3;
					trackEvents[trackEventCount].contents[2] = ((microsecondsSinceQuarterNote >> 16) & 0xFF);
					trackEvents[trackEventCount].contents[3] = ((microsecondsSinceQuarterNote >> 8) & 0xFF);
					trackEvents[trackEventCount].contents[4] = ((microsecondsSinceQuarterNote >> 0) & 0xFF);

					trackEventCount++;

					
					int MICROSECONDS_PER_MINUTE = 60000000;
					float beatsPerMinute = (float)MICROSECONDS_PER_MINUTE / (float)microsecondsSinceQuarterNote;
				}
				else if (subType == 0x2D) // end loop
				{
					byte loopCount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					byte currentLoopCount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					unsigned long offsetToBeginningLoop = ((((((ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false));
				}
				else if (subType == 0x2E) // start loop
				{
					byte loopNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					byte endLoop = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					if (endLoop != 0xFF)
					{
						// is this used?
					}
				}
				else if (subType == 0x2F)
				{
					trackEvents[trackEventCount].type = 0xFF;
					trackEvents[trackEventCount].contentSize = 2;
					trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
					
					trackEvents[trackEventCount].contents[0] = 0x2F;
					trackEvents[trackEventCount].contents[1] = 0x0;

					trackEventCount++;

					endFlag = true;
				}
				else if (subType == 0xF8)
				{
					// ?
				}
			}
			else if ((eventVal >= 0x80 && eventVal < 0x90))
			{
				byte curEventVal;

				byte noteNumber;
				
				trackEvents[trackEventCount].type = eventVal;
				noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				curEventVal = eventVal;

				byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);


				trackEvents[trackEventCount].durationTime = 0; // to be filled in
				trackEvents[trackEventCount].contentSize = 2;
				trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
				trackEvents[trackEventCount].contents[0] = noteNumber;
				trackEvents[trackEventCount].contents[1] = velocity;

				trackEventCount++;
			}
			else if ((eventVal >= 0x90 && eventVal < 0xA0))
			{
				byte curEventVal;

				byte noteNumber;
				
				trackEvents[trackEventCount].type = eventVal;
				noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				curEventVal = eventVal;

				byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);


				trackEvents[trackEventCount].durationTime = 0; // to be filled in
				trackEvents[trackEventCount].contentSize = 2;
				trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
				trackEvents[trackEventCount].contents[0] = noteNumber;
				trackEvents[trackEventCount].contents[1] = velocity;

				trackEventCount++;
			}
			else if (((eventVal >= 0xB0) && (eventVal < 0xC0))) // controller change
			{
				CString controllerTypeText = "";
				byte controllerType;
				
				controllerType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				trackEvents[trackEventCount].type = eventVal;

				byte controllerValue = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				trackEvents[trackEventCount].contentSize = 2;
				trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
				trackEvents[trackEventCount].contents[0] = controllerType;
				trackEvents[trackEventCount].contents[1] = controllerValue;

				trackEventCount++;

			}
			else if (((eventVal >= 0xC0) && (eventVal < 0xD0))) // change instrument
			{
				byte instrument;
				
				instrument = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				trackEvents[trackEventCount].type = eventVal;

				trackEvents[trackEventCount].contentSize = 1;
				trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
				trackEvents[trackEventCount].contents[0] = instrument;
				if (instrument >= numberInstruments)
					numberInstruments = (instrument + 1);

				trackEventCount++;
			}
			else if (((eventVal >= 0xD0) && (eventVal < 0xE0))) // channel aftertouch
			{
				byte amount;
				
				amount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				trackEvents[trackEventCount].type = eventVal;

				trackEvents[trackEventCount].contentSize = 1;
				trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
				trackEvents[trackEventCount].contents[0] = amount;

				trackEventCount++;
			}
			else if (((eventVal >= 0xE0) && (eventVal < 0xF0))) // pitch bend
			{
				byte valueLSB;
				
				valueLSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				trackEvents[trackEventCount].type = eventVal;
				
				byte valueMSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				trackEvents[trackEventCount].contentSize = 2;
				trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
				trackEvents[trackEventCount].contents[0] = valueLSB;
				trackEvents[trackEventCount].contents[1] = valueMSB;

				trackEventCount++;
			}
			else if (eventVal == 0xFE) // repeat operation
			{
				// should not be here...

				// no prev event set
			}
			else
			{
				fprintf(outFile, "%02X ERROR MISSING PARSE OF TYPE\n", eventVal);
			}
		}

		
		unsigned long timeOffset = 0;

		unsigned long sizeData = 0;
		byte previousTrackEvent = 0x0;

		
		for (int j = 0; j < trackEventCount; j++)
		{
			TrackEvent trackEvent =  trackEvents[j];
			if (trackEvent.obsoleteEvent)
			{
				timeOffset += trackEvent.deltaTime;
			}
			else
			{
				unsigned long lengthTimeDelta = 0;
				unsigned long timeDelta = ReturnVLBytes((trackEvent.deltaTime + timeOffset), lengthTimeDelta);
				timeOffset = 0;

				sizeData += lengthTimeDelta;


				if ((trackEvent.type != previousTrackEvent) || (trackEvent.type >= 0xF0))
				{
					sizeData += 1;
				}

				sizeData += trackEvent.contentSize;

				previousTrackEvent = trackEvent.type;
			}
		}

		tempLong = Flip32Bit(sizeData);
		fwrite(&tempLong,1, 4, outFile);

		timeOffset = 0;
		previousTrackEvent = 0x0;
		for (int eventCount = 0; eventCount < trackEventCount; eventCount++)
		{
			TrackEvent trackEvent = trackEvents[eventCount];

			if (trackEvent.obsoleteEvent)
			{
				timeOffset += trackEvent.deltaTime;
			}
			else
			{
				unsigned long lengthTimeDelta = 0;
				unsigned long timeDelta = ReturnVLBytes((trackEvent.deltaTime + timeOffset), lengthTimeDelta);
				timeOffset = 0;
				WriteVLBytes(outFile, timeDelta, lengthTimeDelta, false);

				if ((trackEvent.type != previousTrackEvent) || (trackEvent.type >= 0xF0))
				{
					fwrite(&trackEvent.type, 1, 1, outFile);
				}

				fwrite(trackEvent.contents, 1, trackEvent.contentSize, outFile);

				previousTrackEvent = trackEvent.type;
			}
		}

		for (int eventCount = 0; eventCount < trackEventCount; eventCount++)
		{
			if (trackEvents[eventCount].contents != NULL)
			{
				delete [] trackEvents[eventCount].contents;
				trackEvents[eventCount].contents = NULL;
			}
		}

		delete [] trackEvents;

		// just one track
		
		fflush(outFile);	
		fclose(outFile);
	}
	catch (...)
	{
		MessageBox(NULL, "Error exporting", "Error", NULL);
	}
}

void CMidiParse::BTMidiToMidi(byte* inputMID, int inputSize, CString outFileName, int& numberInstruments, bool& hasLoopPoint, int& loopStart, int& loopEnd)
{
	numberInstruments = 0;
	try
	{
		FILE* outFile = fopen(outFileName, "wb");
		if (outFile == NULL)
		{
			MessageBox(NULL, "Error outputting file", "Error", NULL);
			return;
		}

		// parse midi
		
		int trackSize = CharArrayToLong(&inputMID[0x4]);

		unsigned long tempLong = Flip32Bit(0x4D546864);
		fwrite(&tempLong, 1 ,4 , outFile);
		tempLong = Flip32Bit(0x00000006);
		fwrite(&tempLong, 1 ,4 , outFile);
		tempLong = Flip32Bit(0x00010000 | trackSize);
		fwrite(&tempLong, 1 ,4 , outFile);

		unsigned long division = CharArrayToLong(&inputMID[0x0]);

		unsigned short tempShort = division;
		tempShort = Flip16Bit(tempShort);
		fwrite(&tempShort, 1 ,2 , outFile);

		int counterTrack = 0;

		for (int iii = 0; iii < trackSize; iii++)
		{
			unsigned long absoluteTime = 0;

			int trackEventCount = 0;
			TrackEvent* trackEvents = new TrackEvent[0x90000];

			for (int j = 0; j < 0x90000; j++)
			{
				trackEvents[j].contents = NULL;
				trackEvents[j].contentSize = 0;
				trackEvents[j].obsoleteEvent = false;
				trackEvents[j].deltaTime = 0;
				trackEvents[j].absoluteTime = 0;
			}

			unsigned long offset = CharArrayToLong(&inputMID[(iii * 4) + 0x8]);
			
			int position = offset;	

			if (position != 0)
			{
				tempLong = Flip32Bit(0x4D54726B);
				fwrite(&tempLong, 1 ,4 , outFile);
	
				int previousEventValue = 0;

				byte* repeatPattern = NULL;
				byte altOffset = 0;
				byte altLength = 0;

				bool endFlag = false;
				while ((position < inputSize) && !endFlag)
				{
					int timePosition = position;

					unsigned long original;
					unsigned long timeTag = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, true);

					trackEvents[trackEventCount].deltaTime += timeTag;

					absoluteTime += timeTag;
					trackEvents[trackEventCount].absoluteTime = absoluteTime;

					int vlLength = 0;
					byte eventVal = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

					bool statusBit = false;

					if (eventVal < 0x80)
					{
						// continuation
						statusBit = true;
					}
					else
					{
						statusBit = false;
					}

					if ((eventVal == 0xFF) || (statusBit && (previousEventValue == 0xFF))) // meta event
					{
						byte subType;
						if (statusBit)
							subType = eventVal;
						else
							subType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						if (subType == 0x51) // tempo
						{
							int microsecondsSinceQuarterNote = ((((ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true));

							trackEvents[trackEventCount].type = 0xFF;
							trackEvents[trackEventCount].contentSize = 5;
							trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
							
							trackEvents[trackEventCount].contents[0] = 0x51;
							trackEvents[trackEventCount].contents[1] = 0x3;
							trackEvents[trackEventCount].contents[2] = ((microsecondsSinceQuarterNote >> 16) & 0xFF);
							trackEvents[trackEventCount].contents[3] = ((microsecondsSinceQuarterNote >> 8) & 0xFF);
							trackEvents[trackEventCount].contents[4] = ((microsecondsSinceQuarterNote >> 0) & 0xFF);

							trackEventCount++;

							
							int MICROSECONDS_PER_MINUTE = 60000000;
							float beatsPerMinute = (float)MICROSECONDS_PER_MINUTE / (float)microsecondsSinceQuarterNote;
						}
						else if (subType == 0x2D) // end loop
						{
							byte loopCount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
							byte currentLoopCount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
							unsigned long offsetToBeginningLoop = ((((((ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false));

							hasLoopPoint = true;
							loopEnd = absoluteTime;
						}
						else if (subType == 0x2E) // start loop
						{
							byte loopNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
							byte endLoop = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
							if (endLoop != 0xFF)
							{
								// is this used?
							}
							hasLoopPoint = true;
							loopStart = absoluteTime;
						}
						else if (subType == 0x2F)
						{
							trackEvents[trackEventCount].type = 0xFF;
							trackEvents[trackEventCount].contentSize = 2;
							trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
							
							trackEvents[trackEventCount].contents[0] = 0x2F;
							trackEvents[trackEventCount].contents[1] = 0x0;

							trackEventCount++;

							endFlag = true;
						}

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if ((eventVal >= 0x90 && eventVal < 0xA0) || (statusBit && (previousEventValue >= 0x90) && (previousEventValue < 0xA0)))
					{
						byte curEventVal;

						byte noteNumber;
						if (statusBit)
						{
							trackEvents[trackEventCount].type = previousEventValue;
							noteNumber = eventVal;
							curEventVal = previousEventValue;
						}
						else
						{
							trackEvents[trackEventCount].type = eventVal;
							noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							curEventVal = eventVal;
						}
						byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						unsigned long timeDuration = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, true);

						

						trackEvents[trackEventCount].durationTime = timeDuration; // to be filled in
						trackEvents[trackEventCount].contentSize = 2;
						trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
						trackEvents[trackEventCount].contents[0] = noteNumber;
						trackEvents[trackEventCount].contents[1] = velocity;

						trackEventCount++;

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xB0) && (eventVal < 0xC0)) || (statusBit && (previousEventValue >= 0xB0) && (previousEventValue < 0xC0))) // controller change
					{
						CString controllerTypeText = "";
						byte controllerType;
						
						if (statusBit)
						{
							controllerType = eventVal;
							trackEvents[trackEventCount].type = previousEventValue;
						}
						else
						{
							controllerType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							trackEvents[trackEventCount].type = eventVal;
						}
						byte controllerValue = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						trackEvents[trackEventCount].contentSize = 2;
						trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
						trackEvents[trackEventCount].contents[0] = controllerType;
						trackEvents[trackEventCount].contents[1] = controllerValue;

						trackEventCount++;

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xC0) && (eventVal < 0xD0)) || (statusBit && (previousEventValue >= 0xC0) && (previousEventValue < 0xD0))) // change instrument
					{
						byte instrument;
						if (statusBit)
						{
							instrument = eventVal;
							trackEvents[trackEventCount].type = previousEventValue;
						}
						else
						{
							instrument = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							trackEvents[trackEventCount].type = eventVal;
						}

						trackEvents[trackEventCount].contentSize = 1;
						trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
						trackEvents[trackEventCount].contents[0] = instrument;
						if (instrument >= numberInstruments)
							numberInstruments = (instrument + 1);

						trackEventCount++;

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xD0) && (eventVal < 0xE0)) || (statusBit && (previousEventValue >= 0xD0) && (previousEventValue < 0xE0))) // channel aftertouch
					{
						byte amount;
						if (statusBit)
						{
							amount = eventVal;
							trackEvents[trackEventCount].type = previousEventValue;
						}
						else
						{
							amount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							trackEvents[trackEventCount].type = eventVal;
						}

						trackEvents[trackEventCount].contentSize = 1;
						trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
						trackEvents[trackEventCount].contents[0] = amount;

						trackEventCount++;

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xE0) && (eventVal < 0xF0)) || (statusBit && (previousEventValue >= 0xE0) && (previousEventValue < 0xF0))) // pitch bend
					{
						byte valueLSB;
						if (statusBit)
						{
							valueLSB = eventVal;
							trackEvents[trackEventCount].type = previousEventValue;
						}
						else
						{
							valueLSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							trackEvents[trackEventCount].type = eventVal;
						}
						
						byte valueMSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						trackEvents[trackEventCount].contentSize = 2;
						trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
						trackEvents[trackEventCount].contents[0] = valueLSB;
						trackEvents[trackEventCount].contents[1] = valueMSB;

						trackEventCount++;

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (eventVal == 0xFE) // repeat operation
					{
						// should not be here...

						// no prev event set
					}
					else
					{
						fprintf(outFile, "%02X ERROR MISSING PARSE OF TYPE\n", eventVal);
					}

				}

				for (int eventCount = 0; eventCount < trackEventCount; eventCount++)
				{
					TrackEvent trackEvent = trackEvents[eventCount];
					if ((trackEvent.type >= 0x90) && (trackEvent.type < 0xA0))
					{
						// need to split out
						if (trackEvent.durationTime > 0)
						{

							unsigned long shutoffTime = (trackEvent.absoluteTime + trackEvent.durationTime);

							if (eventCount != (trackEventCount - 1))
							{

								for (int i = (eventCount+1); i < trackEventCount; i++)
								{
									if ((trackEvents[i].absoluteTime > shutoffTime) && (i != (trackEventCount - 1)))
									{
										for (int j = (trackEventCount - 1); j >= i; j--)
										{
											trackEvents[j+1].absoluteTime = trackEvents[j].absoluteTime;
											trackEvents[j+1].contentSize = trackEvents[j].contentSize;
 											if (trackEvents[j+1].contents != NULL)
											{
												delete [] trackEvents[j+1].contents;
												trackEvents[j+1].contents = NULL;
											}
											trackEvents[j+1].contents = new byte[trackEvents[j].contentSize];
											for (int r = 0; r < trackEvents[j].contentSize; r++)
											{
												trackEvents[j+1].contents[r] = trackEvents[j].contents[r];
											}
											trackEvents[j+1].deltaTime = trackEvents[j].deltaTime;
											trackEvents[j+1].durationTime = trackEvents[j].durationTime;
											trackEvents[j+1].obsoleteEvent = trackEvents[j].obsoleteEvent;
											trackEvents[j+1].type = trackEvents[j].type;
										}

										trackEvents[i].type = trackEvents[eventCount].type;
										trackEvents[i].absoluteTime = shutoffTime;
										trackEvents[i].deltaTime = (trackEvents[i].absoluteTime - trackEvents[i-1].absoluteTime);
										trackEvents[i].contentSize = trackEvents[eventCount].contentSize;
										trackEvents[i].durationTime = 0;
										trackEvents[i].contents = new byte[trackEvents[i].contentSize];
										trackEvents[i].contents[0] = trackEvents[eventCount].contents[0];
										trackEvents[i].contents[1] = 0;

										trackEvents[i+1].deltaTime = (trackEvents[i+1].absoluteTime - trackEvents[i].absoluteTime);

										if (trackEvents[i].deltaTime > 0xFF000000)
										{
											int a =1;
										}

										trackEventCount++;
										break;
									}
									else if (i == (trackEventCount - 1))
									{
										trackEvents[i+1].absoluteTime = shutoffTime; // move end to end
										trackEvents[i+1].contentSize = trackEvents[i].contentSize;
										if (trackEvents[i+1].contents != NULL)
										{
											delete [] trackEvents[i+1].contents;
											trackEvents[i+1].contents = NULL;
										}
										trackEvents[i+1].contents = new byte[trackEvents[i].contentSize];
										for (int r = 0; r < trackEvents[i].contentSize; r++)
										{
											trackEvents[i+1].contents[r] = trackEvents[i].contents[r];
										}
										trackEvents[i+1].deltaTime = trackEvents[i].deltaTime;
										trackEvents[i+1].durationTime = trackEvents[i].durationTime;
										trackEvents[i+1].obsoleteEvent = trackEvents[i].obsoleteEvent;
										trackEvents[i+1].type = trackEvents[i].type;


										trackEvents[i].type = trackEvents[eventCount].type;
										trackEvents[i].absoluteTime = shutoffTime;
										trackEvents[i].deltaTime = (trackEvents[i].absoluteTime - trackEvents[i - 1].absoluteTime);
										trackEvents[i].contentSize = trackEvents[eventCount].contentSize;
										trackEvents[i].durationTime = 0;
										trackEvents[i].contents = new byte[trackEvents[i].contentSize];
										trackEvents[i].contents[0] = trackEvents[eventCount].contents[0];
										trackEvents[i].contents[1] = 0;

										trackEvents[i+1].deltaTime = (trackEvents[i+1].absoluteTime - trackEvents[i].absoluteTime);
										if (trackEvents[i].deltaTime > 0xFF000000)
										{
											int a =1;
										}
										trackEventCount++;
										break;
									}
								}
							}
							else
							{
								trackEvents[eventCount+1].absoluteTime = shutoffTime; // move end to end
								trackEvents[eventCount+1].contentSize = trackEvents[eventCount].contentSize;
								if (trackEvents[eventCount+1].contents != NULL)
								{
									delete [] trackEvents[eventCount+1].contents;
									trackEvents[eventCount+1].contents = NULL;
								}
								trackEvents[eventCount+1].contents = new byte[trackEvents[eventCount].contentSize];
								for (int r = 0; r < trackEvents[eventCount].contentSize; r++)
								{
									trackEvents[eventCount+1].contents[r] = trackEvents[eventCount].contents[r];
								}
								trackEvents[eventCount+1].deltaTime = trackEvents[eventCount].deltaTime;
								trackEvents[eventCount+1].durationTime = trackEvents[eventCount].durationTime;
								trackEvents[eventCount+1].obsoleteEvent = trackEvents[eventCount].obsoleteEvent;
								trackEvents[eventCount+1].type = trackEvents[eventCount].type;


								trackEvents[eventCount].type = trackEvents[eventCount].type;
								trackEvents[eventCount].absoluteTime = shutoffTime;
								if ((trackEvents[eventCount].absoluteTime - trackEvents[eventCount - 1].absoluteTime) > 0xFF000000)
								{
									int a =1;
								}
								trackEvents[eventCount].deltaTime = (trackEvents[eventCount].absoluteTime - trackEvents[eventCount - 1].absoluteTime);
								trackEvents[eventCount].contentSize = trackEvents[eventCount].contentSize;
								trackEvents[eventCount].durationTime = 0;
								trackEvents[eventCount].contents = new byte[trackEvents[eventCount].contentSize];
								trackEvents[eventCount].contents[0] = trackEvents[eventCount].contents[0];
								trackEvents[eventCount].contents[1] = 0;

								trackEvents[eventCount+1].deltaTime = (trackEvents[eventCount+1].absoluteTime - trackEvents[eventCount].absoluteTime);
								if (trackEvents[eventCount].deltaTime > 0xFF000000)
								{
									int a =1;
								}
								trackEventCount++;
							}
						}
					}
				}

				
				unsigned long timeOffset = 0;

				unsigned long sizeData = 0;
				byte previousTrackEvent = 0x0;

				
				for (int j = 0; j < trackEventCount; j++)
				{
					TrackEvent trackEvent =  trackEvents[j];
					if (trackEvent.obsoleteEvent)
					{
						timeOffset += trackEvent.deltaTime;
					}
					else
					{
						unsigned long lengthTimeDelta = 0;
						unsigned long timeDelta = ReturnVLBytes((trackEvent.deltaTime + timeOffset), lengthTimeDelta);
						timeOffset = 0;

						sizeData += lengthTimeDelta;


						if ((trackEvent.type != previousTrackEvent) || (trackEvent.type == 0xFF))
						{
							sizeData += 1;
						}

						sizeData += trackEvent.contentSize;

						previousTrackEvent = trackEvent.type;
					}
				}

				tempLong = Flip32Bit(sizeData);
				fwrite(&tempLong,1, 4, outFile);

				timeOffset = 0;
				previousTrackEvent = 0x0;
				for (int eventCount = 0; eventCount < trackEventCount; eventCount++)
				{
					TrackEvent trackEvent = trackEvents[eventCount];

					if (trackEvent.obsoleteEvent)
					{
						timeOffset += trackEvent.deltaTime;
					}
					else
					{
						unsigned long lengthTimeDelta = 0;
						unsigned long timeDelta = ReturnVLBytes((trackEvent.deltaTime + timeOffset), lengthTimeDelta);
						timeOffset = 0;
						WriteVLBytes(outFile, timeDelta, lengthTimeDelta, true);

						if ((trackEvent.type != previousTrackEvent) || (trackEvent.type == 0xFF))
						{
							fwrite(&trackEvent.type, 1, 1, outFile);
						}

						fwrite(trackEvent.contents, 1, trackEvent.contentSize, outFile);

						previousTrackEvent = trackEvent.type;
					}
				}

				for (int eventCount = 0; eventCount < trackEventCount; eventCount++)
				{
					if (trackEvents[eventCount].contents != NULL)
					{
						delete [] trackEvents[eventCount].contents;
						trackEvents[eventCount].contents = NULL;
					}
				}
			}
			else
			{
				
			}

			counterTrack++;
			delete [] trackEvents;
		}



	

		fflush(outFile);	
		fclose(outFile);
	}
	catch (...)
	{
		MessageBox(NULL, "Error exporting", "Error", NULL);
	}
}


bool CMidiParse::MidiToBTFormat(CString input, CString output, bool loop, unsigned long loopPoint, bool useRepeaters)
{
	CString tempOutput = (output + "temp.bin");

	unsigned short numberTracks = 0x20;

	if (MidiToBTFormatStageOne(input, tempOutput, loop, loopPoint, useRepeaters, numberTracks))
	{
		FILE* inFile = fopen(tempOutput, "rb");
		if (inFile == NULL)
		{
			MessageBox(NULL, "Error opening temp file", "Error", NULL);
			return false;
		}

		fseek(inFile, 0, SEEK_END);
		int sizeOut = ftell(inFile);
		rewind(inFile);
		byte* inArray = new byte[sizeOut];
		fread(inArray, 1, sizeOut, inFile);
		fclose(inFile);

		::DeleteFile(tempOutput);

		FILE* outFile = fopen(output, "wb");
		if (outFile == NULL)
		{
			MessageBox(NULL, "Error opening output", "Error", NULL);
			return false;
		}

		unsigned long speed = CharArrayToLong(&inArray[0x80]);

		

		unsigned long offsetheader[0x20];
		
		for (int x = 0; x < 0x80; x+=4)
		{
			offsetheader[x/4] = ((((((inArray[x] << 8) | inArray[x+1]) << 8) | inArray[x+2]) << 8) | inArray[x+3]);
			if (offsetheader[x/4] == 0)
			{
				break;
			}
		}

		unsigned long lessOffset = 0x84 - ((numberTracks * 0x4) + 0x8);

		unsigned long tempLong = Flip32Bit(speed);
		fwrite(&tempLong, 1, 4, outFile);

		tempLong = Flip32Bit((unsigned long)numberTracks);
		fwrite(&tempLong, 1, 4, outFile);

		for (int x = 0; x < numberTracks; x++)
		{
			tempLong = Flip32Bit(((unsigned long)offsetheader[x] - lessOffset));
			fwrite(&tempLong, 1, 4, outFile);
		}

		for (int x = 0x84; x < sizeOut; x++)
		{
			fwrite(&inArray[x], 1, 1, outFile);
		}

		fclose(outFile);
	}
	else
	{
		return false;
	}

	return true;
}

bool CMidiParse::AddLoopGEFormat(byte* inputMID, CString output, int inputSize, bool loop, unsigned long loopPoint, bool useRepeaters)
{
	unsigned long highestAbsoluteTime = 0;
	unsigned long highestAbsoluteTimeByTrack[0x10];
	for (int x = 0; x < 0x10; x++)
		highestAbsoluteTimeByTrack[x] = 0;

	unsigned long division = CharArrayToLong(&inputMID[0x40]);

	int numberInstruments = 0;
	numberTracks = 0x10;

	try
	{
		unsigned long lengthHeader = 0x44;

		// parse midi
		
		int trackSize = 0;
		for (int i = 0; i < (lengthHeader - 4); i+=4) // ignore last 00000180
		{
			unsigned long offset = CharArrayToLong(&inputMID[i]);
			if (offset != 0)
				trackSize++;
		}




		for (int iii = 0; iii < (lengthHeader - 4); iii+=4) // ignore last 00000180
		{
			int trackNum = iii / 4;

			unsigned long absoluteTime = 0;

			unsigned long offset = CharArrayToLong(&inputMID[iii]);
			
			int position = offset;	

			if (position != 0)
			{
				int previousEventValue = 0;

				byte* repeatPattern = NULL;
				byte altOffset = 0;
				byte altLength = 0;

				bool endFlag = false;
				while ((position < inputSize) && !endFlag)
				{
					int timePosition = position;

					unsigned long original;
					// trackEvents[trackEventCount].deltaTime is for loops
					unsigned long timeTag = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, true);

					absoluteTime += timeTag;

					int vlLength = 0;
					byte eventVal = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

					bool statusBit = false;

					if (eventVal < 0x80)
					{
						// continuation
						statusBit = true;
					}
					else
					{
						statusBit = false;
					}

					if ((eventVal == 0xFF) || (statusBit && (previousEventValue == 0xFF))) // meta event
					{
						byte subType;
						if (statusBit)
							subType = eventVal;
						else
							subType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						if (subType == 0x51) // tempo
						{
							int microsecondsSinceQuarterNote = ((((ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true));
							
							int MICROSECONDS_PER_MINUTE = 60000000;
							float beatsPerMinute = (float)MICROSECONDS_PER_MINUTE / (float)microsecondsSinceQuarterNote;
						}
						else if (subType == 0x2D) // end loop
						{
							byte loopCount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							byte currentLoopCount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							unsigned long offsetToBeginningLoop = ((((((ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true));
							
						}
						else if (subType == 0x2E) // start loop
						{
							byte loopNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							byte endLoop = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							if (endLoop != 0xFF)
							{
								// is this used?
							}
						}
						else if (subType == 0x2F)
						{
							endFlag = true;
						}

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if ((eventVal >= 0x90 && eventVal < 0xA0) || (statusBit && (previousEventValue >= 0x90) && (previousEventValue < 0xA0)))
					{
						byte curEventVal;

						byte noteNumber;
						if (statusBit)
						{
							noteNumber = eventVal;
							curEventVal = previousEventValue;
						}
						else
						{
							noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
						}
						byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						unsigned long timeDuration = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, true);

					

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xB0) && (eventVal < 0xC0)) || (statusBit && (previousEventValue >= 0xB0) && (previousEventValue < 0xC0))) // controller change
					{
						CString controllerTypeText = "";
						byte controllerType;
						
						if (statusBit)
						{
							controllerType = eventVal;
						}
						else
						{
							controllerType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
						}
						byte controllerValue = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xC0) && (eventVal < 0xD0)) || (statusBit && (previousEventValue >= 0xC0) && (previousEventValue < 0xD0))) // change instrument
					{
						byte instrument;
						if (statusBit)
						{
							instrument = eventVal;
						}
						else
						{
							instrument = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
						}
						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xD0) && (eventVal < 0xE0)) || (statusBit && (previousEventValue >= 0xD0) && (previousEventValue < 0xE0))) // channel aftertouch
					{
						byte amount;
						if (statusBit)
						{
							amount = eventVal;
						}
						else
						{
							amount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
						}

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xE0) && (eventVal < 0xF0)) || (statusBit && (previousEventValue >= 0xE0) && (previousEventValue < 0xF0))) // pitch bend
					{
						byte valueLSB;
						if (statusBit)
						{
							valueLSB = eventVal;
						}
						else
						{
							valueLSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
						}
						
						byte valueMSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (eventVal == 0xFE) // repeat operation
					{
						// should not be here...

						// no prev event set
					}
					else
					{
						
					}
				}
			}

			if (absoluteTime > highestAbsoluteTime)
			{
				highestAbsoluteTime = absoluteTime;
			}

			if (absoluteTime > highestAbsoluteTimeByTrack[trackNum])
			{
				highestAbsoluteTimeByTrack[trackNum] = absoluteTime;
			}
		}



		trackEvents = new TrackEvent*[0x10];


		int counterTrack = 0;

		for (int iii = 0; iii < (lengthHeader - 4); iii+=4) // ignore last 00000180
		{
			counterTrack = iii / 4;
			unsigned long absoluteTime = 0;

			trackEventCount[counterTrack] = 0;
			trackEvents[counterTrack] = new TrackEvent[0x100000];

			for (int j = 0; j < 0x100000; j++)
			{
				trackEvents[counterTrack][j].contents = NULL;
				trackEvents[counterTrack][j].obsoleteEvent = false;
				trackEvents[counterTrack][j].deltaTime = 0;
				trackEvents[counterTrack][j].absoluteTime = 0;

			}

			unsigned long offset = CharArrayToLong(&inputMID[iii]);
			
			int position = offset;	

			if (position != 0)
			{
				bool didLoop = false;

				if ((loop) && (loopPoint == 0) && (highestAbsoluteTimeByTrack[counterTrack] > 0))
				{
					TrackEvent* newTrackEvent = (TrackEvent*)&(trackEvents[counterTrack][trackEventCount[counterTrack]]);
					newTrackEvent->type = 0xFF;
					newTrackEvent->absoluteTime = 0;
					newTrackEvent->contentSize = 3;
					newTrackEvent->contents = new byte[newTrackEvent->contentSize];
					newTrackEvent->contents[0] = 0x2E;
					newTrackEvent->contents[1] = 0x00;
					newTrackEvent->contents[2] = 0xFF;
					newTrackEvent->deltaTime = 0;
					newTrackEvent->obsoleteEvent = false;
					
					trackEventCount[counterTrack]++;
					didLoop = true;
				}

				int previousEventValue = 0;

				byte* repeatPattern = NULL;
				byte altOffset = 0;
				byte altLength = 0;

				bool endFlag = false;
				while ((position < inputSize) && !endFlag)
				{
					if (trackEventCount[counterTrack] > 0x90000)
						return false;

					int timePosition = position;

					unsigned long original;
					// trackEvents[counterTrack][trackEventCount[counterTrack]].deltaTime is for loops
					unsigned long timeTag = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, true);

					trackEvents[counterTrack][trackEventCount[counterTrack]].deltaTime += timeTag;

					absoluteTime += timeTag;
					trackEvents[counterTrack][trackEventCount[counterTrack]].absoluteTime = absoluteTime;

					int vlLength = 0;
					byte eventVal = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

					bool statusBit = false;

					if (eventVal < 0x80)
					{
						// continuation
						statusBit = true;
					}
					else
					{
						statusBit = false;
					}

					if ((eventVal == 0xFF) || (statusBit && (previousEventValue == 0xFF))) // meta event
					{
						byte subType;
						if (statusBit)
							subType = eventVal;
						else
							subType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						if (subType == 0x51) // tempo
						{
							int microsecondsSinceQuarterNote = ((((ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true));

							trackEvents[counterTrack][trackEventCount[counterTrack]].type = 0xFF;
							trackEvents[counterTrack][trackEventCount[counterTrack]].contentSize = 4;
							trackEvents[counterTrack][trackEventCount[counterTrack]].contents = new byte[trackEvents[counterTrack][trackEventCount[counterTrack]].contentSize];
							
							trackEvents[counterTrack][trackEventCount[counterTrack]].contents[0] = 0x51;
							trackEvents[counterTrack][trackEventCount[counterTrack]].contents[1] = ((microsecondsSinceQuarterNote >> 16) & 0xFF);
							trackEvents[counterTrack][trackEventCount[counterTrack]].contents[2] = ((microsecondsSinceQuarterNote >> 8) & 0xFF);
							trackEvents[counterTrack][trackEventCount[counterTrack]].contents[3] = ((microsecondsSinceQuarterNote >> 0) & 0xFF);

							trackEventCount[counterTrack]++;

							
							int MICROSECONDS_PER_MINUTE = 60000000;
							float beatsPerMinute = (float)MICROSECONDS_PER_MINUTE / (float)microsecondsSinceQuarterNote;
						}
						else if (subType == 0x2D) // end loop
						{
							byte loopCount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							byte currentLoopCount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							unsigned long offsetToBeginningLoop = ((((((ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true));
							
						}
						else if (subType == 0x2E) // start loop
						{
							byte loopNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							byte endLoop = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							if (endLoop != 0xFF)
							{
								// is this used?
							}
						}
						else if (subType == 0x2F)
						{
							TrackEvent* newTrackEvent = ((TrackEvent*)&(trackEvents[counterTrack][trackEventCount[counterTrack]]));

							endFlag = true;

							if (trackEventCount[counterTrack] > 0)
							{
								if (loop)
								{

									TrackEvent* prevEvent = ((TrackEvent*)&(trackEvents[counterTrack][trackEventCount[counterTrack]-1]));

									newTrackEvent->type = 0xFF;
									if (highestAbsoluteTime > (prevEvent->absoluteTime))
									{
										newTrackEvent->deltaTime = (highestAbsoluteTime - (prevEvent->absoluteTime));
										newTrackEvent->absoluteTime = highestAbsoluteTime;
									}
									else
									{
										newTrackEvent->deltaTime = 0;
										newTrackEvent->absoluteTime = prevEvent->absoluteTime;
									}

									newTrackEvent->contentSize = 7;
									newTrackEvent->contents = new byte[newTrackEvent->contentSize];
									newTrackEvent->contents[0] = 0x2D;
									newTrackEvent->contents[1] = 0xFF;
									newTrackEvent->contents[2] = 0xFF;
									newTrackEvent->contents[3] = 0x0; // todo write location
									newTrackEvent->contents[4] = 0x0;
									newTrackEvent->contents[5] = 0x0;
									newTrackEvent->contents[6] = 0x0;
									newTrackEvent->obsoleteEvent = false;
									
									trackEventCount[counterTrack]++;
								
									endFlag = true;
								}
								else
								{
									newTrackEvent->contentSize = 7;
									newTrackEvent->contents = new byte[newTrackEvent->contentSize];
									newTrackEvent->contents[0] = 0x2D;
									newTrackEvent->contents[1] = 0xFF;
									newTrackEvent->contents[2] = 0xFF;
									newTrackEvent->contents[3] = 0x0; // todo write location
									newTrackEvent->contents[4] = 0x0;
									newTrackEvent->contents[5] = 0x0;
									newTrackEvent->contents[6] = 0x0;
									newTrackEvent->obsoleteEvent = false;
									
									trackEventCount[counterTrack]++;
								}
							}

							trackEvents[counterTrack][trackEventCount[counterTrack]].type = 0xFF;
							trackEvents[counterTrack][trackEventCount[counterTrack]].contentSize = 2;
							trackEvents[counterTrack][trackEventCount[counterTrack]].contents = new byte[trackEvents[counterTrack][trackEventCount[counterTrack]].contentSize];
							
							trackEvents[counterTrack][trackEventCount[counterTrack]].contents[0] = 0x2F;
							trackEvents[counterTrack][trackEventCount[counterTrack]].contents[1] = 0x0;
							trackEventCount[counterTrack]++;
						}

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if ((eventVal >= 0x90 && eventVal < 0xA0) || (statusBit && (previousEventValue >= 0x90) && (previousEventValue < 0xA0)))
					{
						byte curEventVal;

						byte noteNumber;
						if (statusBit)
						{
							trackEvents[counterTrack][trackEventCount[counterTrack]].type = previousEventValue;
							noteNumber = eventVal;
							curEventVal = previousEventValue;
						}
						else
						{
							trackEvents[counterTrack][trackEventCount[counterTrack]].type = eventVal;
							noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							curEventVal = eventVal;
						}
						byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						unsigned long timeDuration = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, true);

						

						trackEvents[counterTrack][trackEventCount[counterTrack]].durationTime = timeDuration; // to be filled in
						trackEvents[counterTrack][trackEventCount[counterTrack]].contentSize = 2;
						trackEvents[counterTrack][trackEventCount[counterTrack]].contents = new byte[trackEvents[counterTrack][trackEventCount[counterTrack]].contentSize];
						trackEvents[counterTrack][trackEventCount[counterTrack]].contents[0] = noteNumber;
						trackEvents[counterTrack][trackEventCount[counterTrack]].contents[1] = velocity;

						trackEventCount[counterTrack]++;

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xB0) && (eventVal < 0xC0)) || (statusBit && (previousEventValue >= 0xB0) && (previousEventValue < 0xC0))) // controller change
					{
						CString controllerTypeText = "";
						byte controllerType;
						
						if (statusBit)
						{
							controllerType = eventVal;
							trackEvents[counterTrack][trackEventCount[counterTrack]].type = previousEventValue;
						}
						else
						{
							controllerType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							trackEvents[counterTrack][trackEventCount[counterTrack]].type = eventVal;
						}
						byte controllerValue = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						trackEvents[counterTrack][trackEventCount[counterTrack]].contentSize = 2;
						trackEvents[counterTrack][trackEventCount[counterTrack]].contents = new byte[trackEvents[counterTrack][trackEventCount[counterTrack]].contentSize];
						trackEvents[counterTrack][trackEventCount[counterTrack]].contents[0] = controllerType;
						trackEvents[counterTrack][trackEventCount[counterTrack]].contents[1] = controllerValue;

						trackEventCount[counterTrack]++;

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xC0) && (eventVal < 0xD0)) || (statusBit && (previousEventValue >= 0xC0) && (previousEventValue < 0xD0))) // change instrument
					{
						byte instrument;
						if (statusBit)
						{
							instrument = eventVal;
							trackEvents[counterTrack][trackEventCount[counterTrack]].type = previousEventValue;
						}
						else
						{
							instrument = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							trackEvents[counterTrack][trackEventCount[counterTrack]].type = eventVal;
						}

						trackEvents[counterTrack][trackEventCount[counterTrack]].contentSize = 1;
						trackEvents[counterTrack][trackEventCount[counterTrack]].contents = new byte[trackEvents[counterTrack][trackEventCount[counterTrack]].contentSize];
						trackEvents[counterTrack][trackEventCount[counterTrack]].contents[0] = instrument;
						if (instrument >= numberInstruments)
							numberInstruments = (instrument + 1);

						trackEventCount[counterTrack]++;

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xD0) && (eventVal < 0xE0)) || (statusBit && (previousEventValue >= 0xD0) && (previousEventValue < 0xE0))) // channel aftertouch
					{
						byte amount;
						if (statusBit)
						{
							amount = eventVal;
							trackEvents[counterTrack][trackEventCount[counterTrack]].type = previousEventValue;
						}
						else
						{
							amount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							trackEvents[counterTrack][trackEventCount[counterTrack]].type = eventVal;
						}

						trackEvents[counterTrack][trackEventCount[counterTrack]].contentSize = 1;
						trackEvents[counterTrack][trackEventCount[counterTrack]].contents = new byte[trackEvents[counterTrack][trackEventCount[counterTrack]].contentSize];
						trackEvents[counterTrack][trackEventCount[counterTrack]].contents[0] = amount;

						trackEventCount[counterTrack]++;

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xE0) && (eventVal < 0xF0)) || (statusBit && (previousEventValue >= 0xE0) && (previousEventValue < 0xF0))) // pitch bend
					{
						byte valueLSB;
						if (statusBit)
						{
							valueLSB = eventVal;
							trackEvents[counterTrack][trackEventCount[counterTrack]].type = previousEventValue;
						}
						else
						{
							valueLSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							trackEvents[counterTrack][trackEventCount[counterTrack]].type = eventVal;
						}
						
						byte valueMSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						trackEvents[counterTrack][trackEventCount[counterTrack]].contentSize = 2;
						trackEvents[counterTrack][trackEventCount[counterTrack]].contents = new byte[trackEvents[counterTrack][trackEventCount[counterTrack]].contentSize];
						trackEvents[counterTrack][trackEventCount[counterTrack]].contents[0] = valueLSB;
						trackEvents[counterTrack][trackEventCount[counterTrack]].contents[1] = valueMSB;

						trackEventCount[counterTrack]++;

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (eventVal == 0xFE) // repeat operation
					{
						// should not be here...

						// no prev event set
					}
					else
					{
						
					}
				}
			}
		}

	}
	catch(...)
	{
		MessageBox(NULL, "Error processing inpt file", "Error", NULL);
		return false;
	}


		FILE* outFile = fopen(output, "wb");
		if (outFile == NULL)
		{
			MessageBox(NULL, "Error outputting file", "Error", NULL);
			return false;
		}


		unsigned long timeOffset = 0;

	unsigned long startPosition = 0x44;

	// get offsets
	for (int i = 0; i < numberTracks; i++)
	{
		unsigned long sizeData = 0;
		int loopStartPosition = 0;
		bool foundLoopStart = false;
		byte previousTrackEvent = 0x0;

		if (trackEventCount[i] > 0)
		{
			unsigned long tempLong = Flip32Bit(startPosition);
			fwrite(&tempLong, 1, 4, outFile);

			for (int j = 0; j < trackEventCount[i]; j++)
			{
				TrackEvent* trackEvent =  &(trackEvents[i][j]);
				unsigned long lengthTimeDelta = 0;
				unsigned long timeDelta = ReturnVLBytes((trackEvent->deltaTime + timeOffset), lengthTimeDelta);

				if (trackEvent->obsoleteEvent)
				{
					timeOffset += trackEvent->deltaTime;
				}
				else
				{
					if ((trackEvent->type == 0xFF) && (trackEvent->contents[0] == 0x2E))
					{
						foundLoopStart = true;
						loopStartPosition = (startPosition + sizeData + 1 + trackEvent->contentSize + lengthTimeDelta);
					}
					
					timeOffset = 0;

					sizeData += lengthTimeDelta;

					if ((trackEvent->type == 0xFF) && (trackEvent->contents[0] == 0x2D))
					{
						unsigned long offsetBack = ((startPosition + sizeData) - loopStartPosition + 8);
						trackEvent->contents[3] = ((offsetBack >> 24) & 0xFF);
						trackEvent->contents[4] = ((offsetBack >> 16) & 0xFF);
						trackEvent->contents[5] = ((offsetBack >> 8) & 0xFF);
						trackEvent->contents[6] = ((offsetBack >> 0) & 0xFF);
					}

					if ((trackEvent->type != previousTrackEvent) || (trackEvent->type == 0xFF))
					{
						sizeData += 1;
					}

					sizeData += trackEvent->contentSize;

					if ((trackEvent->type >= 0x90) && (trackEvent->type < 0xA0))
					{
						unsigned long lengthDurationBytes = 0;
						unsigned long duration = ReturnVLBytes(trackEvent->durationTime, lengthDurationBytes);

						sizeData += lengthDurationBytes;
					}

					previousTrackEvent = trackEvent->type;
				}
			}
			startPosition += sizeData;
		}
		else
		{
			unsigned long zero = 0;
			fwrite(&zero, 1, 4, outFile);
		}
	}

	for (int i = numberTracks; i < 16; i++)
	{
		unsigned long zero = 0;
		fwrite(&zero, 1, 4, outFile);
	}

	unsigned long divisionFlipped = Flip32Bit((unsigned long)division);
	fwrite(&divisionFlipped, 1, 4, outFile);

	//FILE* outDebug = fopen("C:\\GoldeneyeStuff\\GE Editor Source\\debug.txt", "w");
	FILE* outDebug = NULL;

	for (int i = 0; i < numberTracks; i++)
	{
		if (trackEventCount[i] > 0)
		{
			if (outDebug != NULL) fprintf(outDebug, "Track %X\n", i);
			byte previousTrackEvent = 0x0;
			for (int j = 0; j < trackEventCount[i]; j++)
			{
				TrackEvent* trackEvent =  &(trackEvents[i][j]);
				if (trackEvent->obsoleteEvent)
				{
					timeOffset += trackEvent->deltaTime;
				}
				else
				{
					unsigned long lengthTimeDelta = 0;
					unsigned long timeDelta = ReturnVLBytes((trackEvent->deltaTime + timeOffset), lengthTimeDelta);
					timeOffset = 0;
					WriteVLBytes(outFile, timeDelta, lengthTimeDelta, false);

					for (int ii = 0; ii < lengthTimeDelta; ii++)
						if (outDebug != NULL) fprintf(outDebug, "%02X", ((timeDelta >> ((lengthTimeDelta * 8) - 8 - (ii * 8))) & 0xFF));

					if ((trackEvent->type != previousTrackEvent) || (trackEvent->type == 0xFF))
					{
						fwrite(&trackEvent->type, 1, 1, outFile);
						if (outDebug != NULL) fprintf(outDebug, "%02X", trackEvent->type);
					}

					fwrite(trackEvent->contents, 1, trackEvent->contentSize, outFile);
					for (int ii = 0; ii < trackEvent->contentSize; ii++)
					{
						if (outDebug != NULL) fprintf(outDebug, "%02X", trackEvent->contents[ii]);
					}

					if ((trackEvent->type >= 0x90) && (trackEvent->type < 0xA0))
					{
						unsigned long lengthDurationBytes = 0;
						unsigned long duration = ReturnVLBytes(trackEvent->durationTime, lengthDurationBytes); // todo real trackevent
						WriteVLBytes(outFile, duration, lengthDurationBytes, false);
						for (int ii = 0; ii < lengthDurationBytes; ii++)
							if (outDebug != NULL) fprintf(outDebug, "%02X", ((duration >> ((lengthDurationBytes * 8) - 8 - (ii * 8))) & 0xFF));
					}

					if (outDebug != NULL) fprintf(outDebug, "\n");
					previousTrackEvent = trackEvent->type;
				}
				
			}
		}

		for (int j = 0; j < trackEventCount[i]; j++)
		{
			if (trackEvents[i][j].contents != NULL)
			{
				delete [] trackEvents[i][j].contents;
				trackEvents[i][j].contents = NULL;
			}
		}
	}

	fflush(outDebug);

	fclose(outFile);

	// write FEFE in case see FE
	outFile = fopen(output, "rb");
	if (outFile == NULL)
	{
		MessageBox(NULL, "Error reading file", "Error", NULL);
		return false;
	}	
	fseek(outFile, 0, SEEK_END);
	int sizeOut = ftell(outFile);
	rewind(outFile);
	byte* inArray = new byte[sizeOut];
	fread(inArray, 1, sizeOut, outFile);
	fclose(outFile);

	unsigned long offsetheader[0x10];
	int extraOffsets[0x10];
	for (int x = 0; x < 0x40; x+=4)
	{
		offsetheader[x/4] = ((((((inArray[x] << 8) | inArray[x+1]) << 8) | inArray[x+2]) << 8) | inArray[x+3]);
		extraOffsets[x/4] = 0x00000000;
	}
	
	for (int x = 0; x < sizeOut; x++)
	{
		if (x > 0x44)
		{
			if (inArray[x] == 0xFE) // need to write twice
			{
				for (int y = 0; y < numberTracks; y++)
				{
					if (offsetheader[y] > x)
					{
						extraOffsets[y]++;
					}
				}
			}
		}
	}

	outFile = fopen(output, "wb");
	if (outFile == NULL)
	{
		MessageBox(NULL, "Error reading file", "Error", NULL);
		return false;
	}

	for (int x = 0; x < 0x10; x++)
	{
		WriteLongToBuffer(inArray, x*4, offsetheader[x] + extraOffsets[x]);
	}
	for (int x = 0; x < sizeOut; x++)
	{
		fwrite(&inArray[x], 1, 1, outFile);
		if (x > 0x44)
		{
			if (inArray[x] == 0xFE) // need to write twice
				fwrite(&inArray[x], 1, 1, outFile);
		}
	}
	fclose(outFile);
	delete [] inArray;

	//return true;

	if (useRepeaters)
	{
		// "compress"
		outFile = fopen(output, "rb");
		if (outFile == NULL)
		{
			MessageBox(NULL, "Error reading file", "Error", NULL);
			return false;
		}	
		fseek(outFile, 0, SEEK_END);
		int sizeIn = ftell(outFile);
		rewind(outFile);
		inArray = new byte[sizeIn];
		fread(inArray, 1, sizeIn, outFile);
		fclose(outFile);

		unsigned long offset[0x10];
		for (int x = 0; x < 0x40; x+=4)
		{
			offset[x/4] = ((((((inArray[x] << 8) | inArray[x+1]) << 8) | inArray[x+2]) << 8) | inArray[x+3]);
		}
		unsigned quarternote = ((((((inArray[0x40] << 8) | inArray[0x41]) << 8) | inArray[0x42]) << 8) | inArray[0x43]);

		unsigned char* outArray = new unsigned char[sizeIn * 4];
		for (int x = 0x0; x < (sizeIn * 4); x++)
		{
			outArray[x] = 0x0;
		}

		unsigned long offsetNew[0x10];
		for (int x = 0; x < 0x10; x++)
		{
			offsetNew[x] = 0;
		}


		int outputSpot = 0x44;
		

		for (int x = 0x0; x < 0x10; x++)
		{
			if (offset[x] != 0)
			{
				offsetNew[x] = outputSpot;

				int outputStart = outputSpot;

				int endSpot = sizeIn;

				if (x < 0xF)
				{
					if (offset[x+1] != 0)
					{
						endSpot = offset[x+1];
					}
				}

				int y = offset[x];
				// loop till end of file
				while (y < endSpot)
				{
					int bestMatchOffset = -1;
					int bestMatchLoopCount = -1;


					// check from past to now for results
					for (int z = outputStart; z < outputSpot; z++)
					{
						int match = 0;
						int matchOffset = 0;

						// one spot match start here
						while  ((outArray[z+matchOffset] == inArray[y+matchOffset]) && ((y+matchOffset) < endSpot) && (outArray[z+matchOffset] != 0xFE) && (outArray[z+matchOffset] != 0xFF) && ((z+matchOffset) < outputSpot))
						{
							bool seeAnFF = false;
							for (int checkFF = y+matchOffset; ((checkFF < endSpot) && (checkFF < (y+matchOffset + 5))); checkFF++)
							{
								if (inArray[checkFF] == 0xFF)
									seeAnFF = true;
							}

							if (seeAnFF)
							{
								break;
							}

							matchOffset++;
						}

						if ((matchOffset > bestMatchLoopCount) && (matchOffset > 6))
						{
							bestMatchLoopCount = matchOffset;
							bestMatchOffset = z;
						}
					}

					int loopCheck = ((y - offset[x]) / 2);
					if (loopCheck > 0xFD)
						loopCheck = 0xFD;

					if (bestMatchLoopCount > 6)
					{
						if (bestMatchLoopCount > 0xFD)
                            bestMatchLoopCount = 0xFD;

						outArray[outputSpot++] = 0xFE;
						int distBack = ((outputSpot - bestMatchOffset) - 1);

						outArray[outputSpot++] = ((distBack >> 8) & 0xFF);
						outArray[outputSpot++] = (distBack & 0xFF);
						outArray[outputSpot++] = bestMatchLoopCount;

						y += bestMatchLoopCount;
					}
					else
					{
						// write one
						outArray[outputSpot++] = inArray[y];

						y++;
					}
				}
			}
			else
				break;

			if ((outputSpot % 4) != 0)
				outputSpot += (4 - (outputSpot % 4));
		}

		// correct loops
		for (int x = 0x0; x < 0x10; x++)
		{
			if (offsetNew[x] != 0)
			{
				int outputStart = offsetNew[x];

				int endSpot = outputSpot;

				if (x < 0xF)
				{
					if (offsetNew[x+1] != 0)
					{
						endSpot = offsetNew[x+1];
					}
				}

				int y = offsetNew[x];
				// loop till end of file

				bool foundStart = false;
				int startPos = 0;
				while (y < endSpot)
				{
					if ((outArray[y] == 0xFF) && (outArray[y+1] == 0x2E) && (outArray[y+2] == 0x00) && (outArray[y+3] == 0xFF)) 
					{
						foundStart = true;
						startPos = y + 4;
						y+=4;
					}
					else if ((outArray[y] == 0xFF) && (outArray[y+1] == 0x2D) && (outArray[y+2] == 0xFF) && (outArray[y+3] == 0xFF)) 
					{
						if (foundStart)
						{
							int distance = ((y + 8) - startPos);

							WriteLongToBuffer(outArray, y+4, distance);
							foundStart = false;
						}

						y+=8;
					}
					else
					{
						y++;
					}

					
				}
			}
		}

		for (int x = 0x0; x < 0x10; x++)
		{
			WriteLongToBuffer(outArray, x*4, offsetNew[x]);
		}
		WriteLongToBuffer(outArray, 0x40, quarternote);


		outFile = fopen(output, "wb");
		if (outFile == NULL)
		{
			MessageBox(NULL, "Error reading file", "Error", NULL);
			return false;
		}

		for (int x = 0; x < outputSpot; x++)
		{
			fwrite(&outArray[x], 1, 1, outFile);
		}
		fclose(outFile);

		delete [] inArray;
		delete [] outArray;






	}
}

bool CMidiParse::MidiToBTFormatStageOne(CString input, CString output, bool loop, unsigned long loopPoint, bool useRepeaters, unsigned short& numTracks)
{
	try
	{

	CString tempFileName = input;

	struct stat results;
	stat(tempFileName, &results);		

	FILE* inFile1 = fopen(input, "rb");
	if (inFile1 == NULL)
	{
		MessageBox(NULL, "Error reading file", "Error", NULL);
		return false;
	}	

	byte* inputMID = new byte[results.st_size];
	fread(inputMID, 1, results.st_size, inFile1);
	fclose(inFile1);

	unsigned long header = CharArrayToLong(&inputMID[0]);

	if (header != 0x4D546864)
	{
		MessageBox(NULL, "Invalid midi hdr", "Error", NULL);
		return false;
	}

	unsigned long headerLength = CharArrayToLong(&inputMID[4]);

	unsigned short type = CharArrayToShort(&inputMID[8]);
	numTracks = CharArrayToShort(&inputMID[0xA]);
	unsigned short tempo = CharArrayToShort(&inputMID[0xC]);

	if (numTracks > 32)
	{
		MessageBox(NULL, "Too many tracks, truncated to 32", "Warning", NULL);
		numTracks = 32;
	}

	unsigned long lessOffset = 0x84 - ((numTracks * 0x4) + 0x8);

	

	FILE* outFile = fopen(output, "wb");
	if (outFile == NULL)
	{
		MessageBox(NULL, "Error outputting file", "Error", NULL);
		return false;
	}


	CMidiParse midiParse;

	numberTracks = numTracks;

	if (type == 0)
	{
		
	}
	else if (type == 1)
	{

	}
	else
	{
		MessageBox(NULL, "Invalid midi type", "Error", NULL);
		return false;
	}



	int position = 0xE;

	byte* repeatPattern = NULL;
	byte altOffset = 0;
	byte altLength = 0;

	bool unknownsHit = false;

	unsigned long highestAbsoluteTime = 0;
	unsigned long highestAbsoluteTimeByTrack[0x20];
	for (int x = 0; x < 0x20; x++)
		highestAbsoluteTimeByTrack[x] = 0;

	for (int trackNum = 0; trackNum < numberTracks; trackNum++)
	{
		unsigned long absoluteTime = 0;

		unsigned long trackHeader = ((((((inputMID[position] << 8) | inputMID[position+1]) << 8) | inputMID[position+2]) << 8) | inputMID[position+3]);
		if (trackHeader != 0x4D54726B)
		{
			MessageBox(NULL, "Invalid track midi hdr", "Error", NULL);
			return false;
		}
		
		unsigned long trackLength = ((((((inputMID[position+4] << 8) | inputMID[position+5]) << 8) | inputMID[position+6]) << 8) | inputMID[position+7]);

		position += 8;

		byte previousEventValue = 0xFF;

		bool endFlag = false;

	

		while (!endFlag && (position < results.st_size))
		{
			unsigned long original;
			unsigned long timeTag = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, false);
			absoluteTime += timeTag;

			byte eventVal = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

			bool statusBit = false;

			if (eventVal <= 0x7F)
			{
				// continuation
				statusBit = true;
			}
			else
			{
				statusBit = false;
			}

			if (eventVal == 0xFF) // meta event
			{
				byte subType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				if (subType == 0x2F) //End of Track Event.
				{
					// remove time till end
					absoluteTime -= timeTag;

					endFlag = true;
					unsigned long length = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);  // end 00 in real mid
				}
				else if (subType == 0x51) //Set Tempo Event.
				{
					unsigned long length = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false); 
										
					ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				}
				//Various Unused Meta Events.
				else if ((subType < 0x7F) && !(subType == 0x51 || subType == 0x2F))
				{
					unsigned long length = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false); 

					for (int i = 0; i < length; i++)
						ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				}
				else if (subType == 0x7F) //Unused Sequencer Specific Event.
				{
					int length = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, false);
					// subtract length
					for (int i = 0; i < length; i++)
					{
						ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					}
				}

				previousEventValue = eventVal;
			}
			else if ((eventVal >= 0x80 && eventVal < 0x90) || (statusBit && (previousEventValue >= 0x80 && previousEventValue < 0x90)))
			{
				byte curEventVal;

				byte noteNumber;
				if (statusBit)
				{
					noteNumber = eventVal;
					curEventVal = previousEventValue;
				}
				else
				{
					noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					curEventVal = eventVal;
				}
				byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if ((eventVal >= 0x90 && eventVal < 0xA0) || (statusBit && (previousEventValue >= 0x90 && previousEventValue < 0xA0)))
			{
				byte curEventVal;

				byte noteNumber;
				if (statusBit)
				{
					noteNumber = eventVal;
					curEventVal = previousEventValue;
				}
				else
				{
					noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					curEventVal = eventVal;
				}
				byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (((eventVal >= 0xB0) && (eventVal < 0xC0))  || (statusBit && (previousEventValue >= 0xB0 && previousEventValue < 0xC0))) // controller change
			{
				byte controllerType;
				if (statusBit)
				{
					controllerType = eventVal;
				}
				else
				{
					controllerType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				}

				byte controllerValue = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (((eventVal >= 0xC0) && (eventVal < 0xD0)) || (statusBit && (previousEventValue >= 0xC0 && previousEventValue < 0xD0))) // change instrument
			{
				byte instrument;
				if (statusBit)
				{
					instrument = eventVal;
				}
				else
				{
					instrument = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				}

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (((eventVal >= 0xD0) && (eventVal < 0xE0))  || (statusBit && (previousEventValue >= 0xD0 && previousEventValue < 0xE0))) // channel aftertouch
			{
				byte amount;
				if (statusBit)
				{
					amount = eventVal;
				}
				else
				{
					amount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				}

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (((eventVal >= 0xE0) && (eventVal < 0xF0))  || (statusBit && (previousEventValue >= 0xE0 && previousEventValue < 0xF0))) // pitch bend
			{
				byte valueLSB;
				if (statusBit)
				{
					valueLSB = eventVal;
				}
				else
				{
					valueLSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				}

				byte valueMSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (eventVal == 0xF0 || eventVal == 0xF7)
            {
				int length = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, false);
				// subtract length
				for (int i = 0; i < length; i++)
				{
					ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				}
            }
			else
			{
				if (!unknownsHit)
				{
					MessageBox(NULL, "Invalid midi character found", "Error", NULL);
					unknownsHit = true;
				}
			}
		}

		if (absoluteTime > highestAbsoluteTime)
		{
			highestAbsoluteTime = absoluteTime;
		}

		if (absoluteTime > highestAbsoluteTimeByTrack[trackNum])
		{
			highestAbsoluteTimeByTrack[trackNum] = absoluteTime;
		}
	}


	position = 0xE;

	repeatPattern = NULL;
	altOffset = 0;
	altLength = 0;

	for (int trackNum = 0; trackNum < numberTracks; trackNum++)
	{
		unsigned long absoluteTime = 0;

		unsigned long trackHeader = ((((((inputMID[position] << 8) | inputMID[position+1]) << 8) | inputMID[position+2]) << 8) | inputMID[position+3]);
		if (trackHeader != 0x4D54726B)
		{
			MessageBox(NULL, "Invalid track midi hdr", "Error", NULL);
			return false;
		}
		
		unsigned long trackLength = ((((((inputMID[position+4] << 8) | inputMID[position+5]) << 8) | inputMID[position+6]) << 8) | inputMID[position+7]);

		position += 8;

		byte previousEventValue = 0xFF;

		bool endFlag = false;

		bool didLoop = false;

		if ((loop) && (loopPoint == 0) && (highestAbsoluteTimeByTrack[trackNum] > 0))
		{
			TrackEvent* newTrackEvent = (TrackEvent*)&(trackEvents[trackNum][trackEventCount[trackNum]]);
			newTrackEvent->type = 0xFF;
			newTrackEvent->absoluteTime = 0;
			newTrackEvent->contentSize = 3;
			newTrackEvent->contents = new byte[newTrackEvent->contentSize];
			newTrackEvent->contents[0] = 0x2E;
			newTrackEvent->contents[1] = 0x00;
			newTrackEvent->contents[2] = 0xFF;
			newTrackEvent->deltaTime = 0;
			newTrackEvent->obsoleteEvent = false;
			
			trackEventCount[trackNum]++;
			didLoop = true;
		}

		
		while (!endFlag && (position < results.st_size))
		{
			unsigned long original;
			unsigned long timeTag = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, false);
			absoluteTime += timeTag;

			TrackEvent* newTrackEvent = (TrackEvent*)&(trackEvents[trackNum][trackEventCount[trackNum]]);
			newTrackEvent->deltaTime = timeTag;
			newTrackEvent->obsoleteEvent = false;
			newTrackEvent->contents = NULL;
			newTrackEvent->absoluteTime = absoluteTime;

			if ((loop) && (!didLoop) && (highestAbsoluteTimeByTrack[trackNum] > loopPoint))
			{
				if (absoluteTime == loopPoint)
				{
					TrackEvent* newTrackEventLoop = (TrackEvent*)&(trackEvents[trackNum][trackEventCount[trackNum]]);
					newTrackEventLoop->type = 0xFF;
					newTrackEventLoop->absoluteTime = absoluteTime;
					newTrackEventLoop->contentSize = 3;
					newTrackEventLoop->contents = new byte[newTrackEventLoop->contentSize];
					newTrackEventLoop->contents[0] = 0x2E;
					newTrackEventLoop->contents[1] = 0x00;
					newTrackEventLoop->contents[2] = 0xFF;
					newTrackEventLoop->deltaTime = timeTag;
					newTrackEventLoop->obsoleteEvent = false;
					
					trackEventCount[trackNum]++;

					newTrackEvent = (TrackEvent*)&(trackEvents[trackNum][trackEventCount[trackNum]]);
					newTrackEvent->deltaTime = 0;
					newTrackEvent->obsoleteEvent = false;
					newTrackEvent->contents = NULL;
					newTrackEvent->absoluteTime = absoluteTime;

					didLoop = true;
				}
				else if (absoluteTime > loopPoint)
				{
					TrackEvent* newTrackEventLoop = (TrackEvent*)&(trackEvents[trackNum][trackEventCount[trackNum]]);
					newTrackEventLoop->type = 0xFF;
					newTrackEventLoop->absoluteTime = loopPoint;
					newTrackEventLoop->contentSize = 3;
					newTrackEventLoop->contents = new byte[newTrackEventLoop->contentSize];
					newTrackEventLoop->contents[0] = 0x2E;
					newTrackEventLoop->contents[1] = 0x00;
					newTrackEventLoop->contents[2] = 0xFF;
					if (trackEventCount[trackNum] > 0)
						newTrackEventLoop->deltaTime = (loopPoint - ((TrackEvent*)&(trackEvents[trackNum][trackEventCount[trackNum]-1]))->absoluteTime);
					else
						newTrackEventLoop->deltaTime = loopPoint;
					newTrackEventLoop->obsoleteEvent = false;
					
					trackEventCount[trackNum]++;

					newTrackEvent = (TrackEvent*)&(trackEvents[trackNum][trackEventCount[trackNum]]);
					newTrackEvent->deltaTime = (absoluteTime - loopPoint);
					newTrackEvent->obsoleteEvent = false;
					newTrackEvent->contents = NULL;
					newTrackEvent->absoluteTime = absoluteTime;

					didLoop = true;
				}
			}

			byte eventVal = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

			bool statusBit = false;

			if (eventVal <= 0x7F)
			{
				// continuation
				statusBit = true;
			}
			else
			{
				statusBit = false;
			}

			if (eventVal == 0xFF) // meta event
			{
				byte subType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				if (subType == 0x2F) //End of Track Event.
				{
					endFlag = true;

					if (loop && (highestAbsoluteTimeByTrack[trackNum] > loopPoint))
					{

						TrackEvent* prevEvent = ((TrackEvent*)&(trackEvents[trackNum][trackEventCount[trackNum]-1]));
						if (
							(prevEvent->type == 0xFF)
							&& (prevEvent->contentSize > 0)
							&& (prevEvent->contents[0] == 0x2E)
							)
						{
							newTrackEvent = prevEvent;

							delete [] newTrackEvent->contents;
							newTrackEvent->type = 0xFF;
							newTrackEvent->contentSize = 1;
							newTrackEvent->contents = new byte[newTrackEvent->contentSize];
							newTrackEvent->contents[0] = 0x2F;
						}
						else
						{
							TrackEvent* newTrackEventLast = (TrackEvent*)&(trackEvents[trackNum][trackEventCount[trackNum]+1]);
							newTrackEventLast->absoluteTime = highestAbsoluteTime;
							newTrackEventLast->deltaTime = 0;
							newTrackEventLast->durationTime = newTrackEvent->durationTime;
							newTrackEventLast->obsoleteEvent = newTrackEvent->obsoleteEvent;

							newTrackEventLast->type = 0xFF;
							newTrackEventLast->contentSize = 1;
							newTrackEventLast->contents = new byte[newTrackEventLast->contentSize];
							newTrackEventLast->contents[0] = 0x2F;

							newTrackEvent->type = 0xFF;
							if (highestAbsoluteTime > (prevEvent->absoluteTime + prevEvent->durationTime))
							{
								newTrackEvent->deltaTime = (highestAbsoluteTime - (prevEvent->absoluteTime + prevEvent->durationTime));
								newTrackEvent->absoluteTime = highestAbsoluteTime;
							}
							else
							{
								newTrackEvent->deltaTime = 0;
								newTrackEvent->absoluteTime = prevEvent->absoluteTime;
							}

							newTrackEvent->contentSize = 7;
							newTrackEvent->contents = new byte[newTrackEvent->contentSize];
							newTrackEvent->contents[0] = 0x2D;
							newTrackEvent->contents[1] = 0xFF;
							newTrackEvent->contents[2] = 0xFF;
							newTrackEvent->contents[3] = 0x0; // todo write location
							newTrackEvent->contents[4] = 0x0;
							newTrackEvent->contents[5] = 0x0;
							newTrackEvent->contents[6] = 0x0;
							newTrackEvent->obsoleteEvent = false;
							
							trackEventCount[trackNum]++;
						}
					}
					else
					{
						newTrackEvent->type = 0xFF;
						newTrackEvent->contentSize = 1;
						newTrackEvent->contents = new byte[newTrackEvent->contentSize];
						newTrackEvent->contents[0] = 0x2F;
					}

					unsigned long length = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);  // end 00 in real mid
				}
				else if (subType == 0x51) //Set Tempo Event.
				{
					newTrackEvent->type = 0xFF;
					newTrackEvent->contentSize = 4;
					newTrackEvent->contents = new byte[newTrackEvent->contentSize];
					newTrackEvent->contents[0] = 0x51;

					unsigned long length = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false); 
										
					newTrackEvent->contents[1] = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					newTrackEvent->contents[2] = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					newTrackEvent->contents[3] = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				}
				//Various Unused Meta Events.
				else if ((subType < 0x7F) && !(subType == 0x51 || subType == 0x2F))
				{
					newTrackEvent->type = 0xFF;
					unsigned long length = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false); 

					for (int i = 0; i < length; i++)
						ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

					newTrackEvent->obsoleteEvent = true;
				}
				else if (subType == 0x7F) //Unused Sequencer Specific Event.
				{
					newTrackEvent->type = 0xFF;
					int length = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, false);
					// subtract length
					for (int i = 0; i < length; i++)
					{
						ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					}
					
					newTrackEvent->obsoleteEvent = true;
				}

				previousEventValue = eventVal;
			}
			else if ((eventVal >= 0x80 && eventVal < 0x90) || (statusBit && (previousEventValue >= 0x80 && previousEventValue < 0x90)))
			{
				byte curEventVal;

				byte noteNumber;
				if (statusBit)
				{
					newTrackEvent->type = previousEventValue;
					noteNumber = eventVal;
					curEventVal = previousEventValue;
				}
				else
				{
					newTrackEvent->type = eventVal;
					noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					curEventVal = eventVal;
				}
				byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				for (int testBackwards = (trackEventCount[trackNum] - 1); testBackwards >= 0; testBackwards--)
				{
					if ((trackEvents[trackNum][testBackwards].type == (0x90 | (curEventVal & 0xF))) && !(trackEvents[trackNum][testBackwards].obsoleteEvent))
					{
						if (trackEvents[trackNum][testBackwards].contents[0] == noteNumber)
						{
							trackEvents[trackNum][testBackwards].durationTime = (absoluteTime - trackEvents[trackNum][testBackwards].absoluteTime);
							break;
						}
					}
				}


				newTrackEvent->durationTime = 0; 
				newTrackEvent->contentSize = 2;
				newTrackEvent->contents = new byte[newTrackEvent->contentSize];
				newTrackEvent->contents[0] = noteNumber;
				newTrackEvent->contents[1] = velocity;
				newTrackEvent->obsoleteEvent = true;

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if ((eventVal >= 0x90 && eventVal < 0xA0) || (statusBit && (previousEventValue >= 0x90 && previousEventValue < 0xA0)))
			{
				byte curEventVal;

				byte noteNumber;
				if (statusBit)
				{
					newTrackEvent->type = previousEventValue;
					noteNumber = eventVal;
					curEventVal = previousEventValue;
				}
				else
				{
					newTrackEvent->type = eventVal;
					noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					curEventVal = eventVal;
				}
				byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				if (velocity == 0)
				{
					// simulate note off
					for (int testBackwards = (trackEventCount[trackNum] - 1); testBackwards >= 0; testBackwards--)
					{
						if (((trackEvents[trackNum][testBackwards].type == curEventVal)) && !(trackEvents[trackNum][testBackwards].obsoleteEvent))
						{
							if (trackEvents[trackNum][testBackwards].contents[0] == noteNumber)
							{
								trackEvents[trackNum][testBackwards].durationTime = (absoluteTime - trackEvents[trackNum][testBackwards].absoluteTime);
								break;
							}
						}
					}

					newTrackEvent->durationTime = 0;
					newTrackEvent->contentSize = 2;
					newTrackEvent->contents = new byte[newTrackEvent->contentSize];
					newTrackEvent->contents[0] = noteNumber;
					newTrackEvent->contents[1] = velocity;
					newTrackEvent->obsoleteEvent = true;
				}
				else
				{
					// check if no note off received, if so, turn it off and restart note
					for (int testBackwards = (trackEventCount[trackNum] - 1); testBackwards >= 0; testBackwards--)
					{
						if (((trackEvents[trackNum][testBackwards].type == curEventVal)) && !(trackEvents[trackNum][testBackwards].obsoleteEvent))
						{
							if (trackEvents[trackNum][testBackwards].contents[0] == noteNumber)
							{
								if (trackEvents[trackNum][testBackwards].durationTime == 0) // means unfinished note
									trackEvents[trackNum][testBackwards].durationTime = (absoluteTime - trackEvents[trackNum][testBackwards].absoluteTime);
								break;
							}
						}
					}

					newTrackEvent->durationTime = 0; // to be filled in
					newTrackEvent->contentSize = 2;
					newTrackEvent->contents = new byte[newTrackEvent->contentSize];
					newTrackEvent->contents[0] = noteNumber;
					newTrackEvent->contents[1] = velocity;
				}

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (((eventVal >= 0xB0) && (eventVal < 0xC0))  || (statusBit && (previousEventValue >= 0xB0 && previousEventValue < 0xC0))) // controller change
			{
				byte controllerType;
				if (statusBit)
				{
					controllerType = eventVal;
					newTrackEvent->type = previousEventValue;
				}
				else
				{
					controllerType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					newTrackEvent->type = eventVal;
				}

				byte controllerValue = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				newTrackEvent->contentSize = 2;
				newTrackEvent->contents = new byte[newTrackEvent->contentSize];
				newTrackEvent->contents[0] = controllerType;
				newTrackEvent->contents[1] = controllerValue;
				
				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (((eventVal >= 0xC0) && (eventVal < 0xD0)) || (statusBit && (previousEventValue >= 0xC0 && previousEventValue < 0xD0))) // change instrument
			{
				byte instrument;
				if (statusBit)
				{
					instrument = eventVal;
					newTrackEvent->type = previousEventValue;
				}
				else
				{
					instrument = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					newTrackEvent->type = eventVal;
				}

				if ((eventVal & 0xF) == 9) // drums in GM
					instrument = instrument;
				else
					instrument = instrument;

				newTrackEvent->contentSize = 1;
				newTrackEvent->contents = new byte[newTrackEvent->contentSize];
				newTrackEvent->contents[0] = instrument;
				
				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (((eventVal >= 0xD0) && (eventVal < 0xE0))  || (statusBit && (previousEventValue >= 0xD0 && previousEventValue < 0xE0))) // channel aftertouch
			{
				newTrackEvent->type = eventVal;
				byte amount;
				if (statusBit)
				{
					amount = eventVal;
					newTrackEvent->type = previousEventValue;
				}
				else
				{
					amount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					newTrackEvent->type = eventVal;
				}

				newTrackEvent->contentSize = 1;
				newTrackEvent->contents = new byte[newTrackEvent->contentSize];
				newTrackEvent->contents[0] = amount;
				//newTrackEvent->obsoleteEvent = true; // temporary?

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (((eventVal >= 0xE0) && (eventVal < 0xF0))  || (statusBit && (previousEventValue >= 0xE0 && previousEventValue < 0xF0))) // pitch bend
			{
				newTrackEvent->type = eventVal;
				byte valueLSB;
				if (statusBit)
				{
					valueLSB = eventVal;
					newTrackEvent->type = previousEventValue;
				}
				else
				{
					valueLSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					newTrackEvent->type = eventVal;
				}

				byte valueMSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				newTrackEvent->contentSize = 2;
				newTrackEvent->contents = new byte[newTrackEvent->contentSize];
				newTrackEvent->contents[0] = valueLSB;
				newTrackEvent->contents[1] = valueMSB;
				//newTrackEvent->obsoleteEvent = true; // temporary?

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (eventVal == 0xF0 || eventVal == 0xF7)
            {
                newTrackEvent->type = eventVal;
				int length = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, false);
				// subtract length
				for (int i = 0; i < length; i++)
				{
					ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				}

				newTrackEvent->obsoleteEvent = true;
            }
			else
			{
				if (!unknownsHit)
				{
					MessageBox(NULL, "Invalid midi character found", "Error", NULL);
					unknownsHit = true;
				}
			}

			trackEventCount[trackNum]++;
		}
	}

	unsigned long timeOffset = 0;

	unsigned long startPosition = 0x84;

	// get offsets
	for (int i = 0; i < numberTracks; i++)
	{
		unsigned long sizeData = 0;
		int loopStartPosition = 0;
		bool foundLoopStart = false;
		byte previousTrackEvent = 0x0;

		if (trackEventCount[i] > 0)
		{
			unsigned long tempLong = Flip32Bit(startPosition);
			fwrite(&tempLong, 1, 4, outFile);

			for (int j = 0; j < trackEventCount[i]; j++)
			{
				TrackEvent* trackEvent =  &(trackEvents[i][j]);
				unsigned long lengthTimeDelta = 0;
				unsigned long timeDelta = ReturnVLBytes((trackEvent->deltaTime + timeOffset), lengthTimeDelta);

				if (trackEvent->obsoleteEvent)
				{
					timeOffset += trackEvent->deltaTime;
				}
				else
				{
					if ((trackEvent->type == 0xFF) && (trackEvent->contents[0] == 0x2E))
					{
						foundLoopStart = true;
						loopStartPosition = (startPosition + sizeData + 1 + trackEvent->contentSize + lengthTimeDelta);
					}
					
					timeOffset = 0;

					sizeData += lengthTimeDelta;

					if ((trackEvent->type == 0xFF) && (trackEvent->contents[0] == 0x2D))
					{
						unsigned long offsetBack = ((startPosition + sizeData) - loopStartPosition + 8);
						trackEvent->contents[3] = ((offsetBack >> 24) & 0xFF);
						trackEvent->contents[4] = ((offsetBack >> 16) & 0xFF);
						trackEvent->contents[5] = ((offsetBack >> 8) & 0xFF);
						trackEvent->contents[6] = ((offsetBack >> 0) & 0xFF);
					}

					if ((trackEvent->type != previousTrackEvent) || (trackEvent->type == 0xFF))
					{
						sizeData += 1;
					}

					sizeData += trackEvent->contentSize;

					if ((trackEvent->type >= 0x90) && (trackEvent->type < 0xA0))
					{
						unsigned long lengthDurationBytes = 0;
						unsigned long duration = ReturnVLBytes(trackEvent->durationTime, lengthDurationBytes);

						sizeData += lengthDurationBytes;
					}

					previousTrackEvent = trackEvent->type;
				}
			}
			startPosition += sizeData;
		}
		else
		{
			unsigned long zero = 0;
			fwrite(&zero, 1, 4, outFile);
		}
	}

	for (int i = numberTracks; i < 32; i++)
	{
		unsigned long zero = 0;
		fwrite(&zero, 1, 4, outFile);
	}

	unsigned long divisionFlipped = Flip32Bit((unsigned long)tempo);
	fwrite(&divisionFlipped, 1, 4, outFile);

	//FILE* outDebug = fopen("C:\\GoldeneyeStuff\\GE Editor Source\\debug.txt", "w");
	FILE* outDebug = NULL;

	for (int i = 0; i < numberTracks; i++)
	{
		if (trackEventCount[i] > 0)
		{
			if (outDebug != NULL) fprintf(outDebug, "Track %X\n", i);
			byte previousTrackEvent = 0x0;
			for (int j = 0; j < trackEventCount[i]; j++)
			{
				TrackEvent* trackEvent =  &(trackEvents[i][j]);
				if (trackEvent->obsoleteEvent)
				{
					timeOffset += trackEvent->deltaTime;
				}
				else
				{
					unsigned long lengthTimeDelta = 0;
					unsigned long timeDelta = ReturnVLBytes((trackEvent->deltaTime + timeOffset), lengthTimeDelta);
					timeOffset = 0;
					WriteVLBytes(outFile, timeDelta, lengthTimeDelta, false);

					for (int ii = 0; ii < lengthTimeDelta; ii++)
						if (outDebug != NULL) fprintf(outDebug, "%02X", ((timeDelta >> ((lengthTimeDelta * 8) - 8 - (ii * 8))) & 0xFF));

					if ((trackEvent->type != previousTrackEvent) || (trackEvent->type == 0xFF))
					{
						fwrite(&trackEvent->type, 1, 1, outFile);
						if (outDebug != NULL) fprintf(outDebug, "%02X", trackEvent->type);
					}

					fwrite(trackEvent->contents, 1, trackEvent->contentSize, outFile);
					for (int ii = 0; ii < trackEvent->contentSize; ii++)
					{
						if (outDebug != NULL) fprintf(outDebug, "%02X", trackEvent->contents[ii]);
					}

					if ((trackEvent->type >= 0x90) && (trackEvent->type < 0xA0))
					{
						unsigned long lengthDurationBytes = 0;
						unsigned long duration = ReturnVLBytes(trackEvent->durationTime, lengthDurationBytes); // todo real trackevent
						WriteVLBytes(outFile, duration, lengthDurationBytes, false);
						for (int ii = 0; ii < lengthDurationBytes; ii++)
							if (outDebug != NULL) fprintf(outDebug, "%02X", ((duration >> ((lengthDurationBytes * 8) - 8 - (ii * 8))) & 0xFF));
					}

					if (outDebug != NULL) fprintf(outDebug, "\n");
					previousTrackEvent = trackEvent->type;
				}
				
			}
		}

		for (int j = 0; j < trackEventCount[i]; j++)
		{
			if (trackEvents[i][j].contents != NULL)
			{
				delete [] trackEvents[i][j].contents;
				trackEvents[i][j].contents = NULL;
			}
		}
	}

	fflush(outDebug);

	fclose(outFile);

	// write FEFE in case see FE
	outFile = fopen(output, "rb");
	if (outFile == NULL)
	{
		MessageBox(NULL, "Error reading file", "Error", NULL);
		return false;
	}	
	fseek(outFile, 0, SEEK_END);
	int sizeOut = ftell(outFile);
	rewind(outFile);
	byte* inArray = new byte[sizeOut];
	fread(inArray, 1, sizeOut, outFile);
	fclose(outFile);

	unsigned long offsetheader[0x20];
	int extraOffsets[0x20];
	for (int x = 0; x < 0x80; x+=4)
	{
		offsetheader[x/4] = ((((((inArray[x] << 8) | inArray[x+1]) << 8) | inArray[x+2]) << 8) | inArray[x+3]);
		extraOffsets[x/4] = 0x00000000;
	}
	
	for (int x = 0; x < sizeOut; x++)
	{
		if (x > 0x84)
		{
			if (inArray[x] == 0xFE) // need to write twice
			{
				for (int y = 0; y < numberTracks; y++)
				{
					if (offsetheader[y] > x)
					{
						extraOffsets[y]++;
					}
				}
			}
		}
	}

	outFile = fopen(output, "wb");
	if (outFile == NULL)
	{
		MessageBox(NULL, "Error reading file", "Error", NULL);
		return false;
	}

	for (int x = 0; x < 0x20; x++)
	{
		WriteLongToBuffer(inArray, x*4, offsetheader[x] + extraOffsets[x]);
	}
	for (int x = 0; x < sizeOut; x++)
	{
		fwrite(&inArray[x], 1, 1, outFile);
		if (x > 0x84)
		{
			if (inArray[x] == 0xFE) // need to write twice
				fwrite(&inArray[x], 1, 1, outFile);
		}
	}
	fclose(outFile);
	delete [] inArray;

	//return true;

	if (useRepeaters)
	{
		// "compress"
		outFile = fopen(output, "rb");
		if (outFile == NULL)
		{
			MessageBox(NULL, "Error reading file", "Error", NULL);
			return false;
		}	
		fseek(outFile, 0, SEEK_END);
		int sizeIn = ftell(outFile);
		rewind(outFile);
		inArray = new byte[sizeIn];
		fread(inArray, 1, sizeIn, outFile);
		fclose(outFile);

		unsigned long offset[0x20];
		for (int x = 0; x < 0x80; x+=4)
		{
			offset[x/4] = ((((((inArray[x] << 8) | inArray[x+1]) << 8) | inArray[x+2]) << 8) | inArray[x+3]);
		}
		unsigned quarternote = ((((((inArray[0x80] << 8) | inArray[0x81]) << 8) | inArray[0x82]) << 8) | inArray[0x83]);

		unsigned char* outArray = new unsigned char[sizeIn * 4];
		for (int x = 0x0; x < (sizeIn * 4); x++)
		{
			outArray[x] = 0x0;
		}

		unsigned long offsetNew[0x20];
		for (int x = 0; x < 0x20; x++)
		{
			offsetNew[x] = 0;
		}


		int outputSpot = 0x84;
		

		for (int x = 0x0; x < 0x20; x++)
		{
			if (offset[x] != 0)
			{
				offsetNew[x] = outputSpot;

				int outputStart = outputSpot;

				int endSpot = sizeIn;

				if (x < 0xF)
				{
					if (offset[x+1] != 0)
					{
						endSpot = offset[x+1];
					}
				}

				int y = offset[x];
				// loop till end of file
				while (y < endSpot)
				{
					int bestMatchOffset = -1;
					int bestMatchLoopCount = -1;


					// check from past to now for results
					for (int z = outputStart; z < outputSpot; z++)
					{
						int match = 0;
						int matchOffset = 0;

						// one spot match start here
						while  ((outArray[z+matchOffset] == inArray[y+matchOffset]) && ((y+matchOffset) < endSpot) && (outArray[z+matchOffset] != 0xFE) && (outArray[z+matchOffset] != 0xFF) && ((z+matchOffset) < outputSpot))
						{
							bool seeAnFF = false;
							for (int checkFF = y+matchOffset; ((checkFF < endSpot) && (checkFF < (y+matchOffset + 5))); checkFF++)
							{
								if (inArray[checkFF] == 0xFF)
									seeAnFF = true;
							}

							if (seeAnFF)
							{
								break;
							}

							matchOffset++;
						}

						if ((matchOffset > bestMatchLoopCount) && (matchOffset > 6))
						{
							bestMatchLoopCount = matchOffset;
							bestMatchOffset = z;
						}
					}

					int loopCheck = ((y - offset[x]) / 2);
					if (loopCheck > 0xFD)
						loopCheck = 0xFD;

					if (bestMatchLoopCount > 6)
					{
						if (bestMatchLoopCount > 0xFD)
                            bestMatchLoopCount = 0xFD;

						outArray[outputSpot++] = 0xFE;
						int distBack = ((outputSpot - bestMatchOffset) - 1);

						outArray[outputSpot++] = ((distBack >> 8) & 0xFF);
						outArray[outputSpot++] = (distBack & 0xFF);
						outArray[outputSpot++] = bestMatchLoopCount;

						y += bestMatchLoopCount;
					}
					else
					{
						// write one
						outArray[outputSpot++] = inArray[y];

						y++;
					}
				}
			}
			else
				break;

			if ((outputSpot % 4) != 0)
				outputSpot += (4 - (outputSpot % 4));
		}

		// correct loops
		for (int x = 0x0; x < 0x20; x++)
		{
			if (offsetNew[x] != 0)
			{
				int outputStart = offsetNew[x];

				int endSpot = outputSpot;

				if (x < 0xF)
				{
					if (offsetNew[x+1] != 0)
					{
						endSpot = offsetNew[x+1];
					}
				}

				int y = offsetNew[x];
				// loop till end of file

				bool foundStart = false;
				int startPos = 0;
				while (y < endSpot)
				{
					if ((outArray[y] == 0xFF) && (outArray[y+1] == 0x2E) && (outArray[y+2] == 0x00) && (outArray[y+3] == 0xFF)) 
					{
						foundStart = true;
						startPos = y + 4;
						y+=4;
					}
					else if ((outArray[y] == 0xFF) && (outArray[y+1] == 0x2D) && (outArray[y+2] == 0xFF) && (outArray[y+3] == 0xFF)) 
					{
						if (foundStart)
						{
							int distance = ((y + 8) - startPos);

							WriteLongToBuffer(outArray, y+4, distance);
							foundStart = false;
						}

						y+=8;
					}
					else
					{
						y++;
					}

					
				}
			}
		}

		for (int x = 0x0; x < 0x20; x++)
		{
			WriteLongToBuffer(outArray, x*4, offsetNew[x]);
		}
		WriteLongToBuffer(outArray, 0x80, quarternote);


		outFile = fopen(output, "wb");
		if (outFile == NULL)
		{
			MessageBox(NULL, "Error reading file", "Error", NULL);
			return false;
		}

		for (int x = 0; x < outputSpot; x++)
		{
			fwrite(&outArray[x], 1, 1, outFile);
		}
		fclose(outFile);

		delete [] inArray;
		delete [] outArray;
	}
}
catch (...)
{
	MessageBox(NULL, "Error converting", "Error", NULL);
	return false;
}
	return true;
}

bool CMidiParse::MidiToGEFormat(CString input, CString output, bool loop, unsigned long loopPoint, bool useRepeaters)
{
	try
	{

	CString tempFileName = input;

	struct stat results;
	stat(tempFileName, &results);		

	FILE* inFile1 = fopen(input, "rb");
	if (inFile1 == NULL)
	{
		MessageBox(NULL, "Error reading file", "Error", NULL);
		return false;
	}	

	byte* inputMID = new byte[results.st_size];
	fread(inputMID, 1, results.st_size, inFile1);
	fclose(inFile1);

	unsigned long header = CharArrayToLong(&inputMID[0]);

	if (header != 0x4D546864)
	{
		MessageBox(NULL, "Invalid midi hdr", "Error", NULL);
		return false;
	}

	unsigned long headerLength = CharArrayToLong(&inputMID[4]);

	unsigned short type = CharArrayToShort(&inputMID[8]);
	unsigned short numTracks = CharArrayToShort(&inputMID[0xA]);
	unsigned short tempo = CharArrayToShort(&inputMID[0xC]);


	if (numTracks > 16)
	{
		MessageBox(NULL, "Too many tracks, truncated to 16", "Warning", NULL);
		numTracks = 16;
	}

	

	FILE* outFile = fopen(output, "wb");
	if (outFile == NULL)
	{
		MessageBox(NULL, "Error outputting file", "Error", NULL);
		return false;
	}


	CMidiParse midiParse;

	numberTracks = numTracks;

	if (type == 0)
	{
		
	}
	else if (type == 1)
	{

	}
	else
	{
		MessageBox(NULL, "Invalid midi type", "Error", NULL);
		return false;
	}



	int position = 0xE;

	byte* repeatPattern = NULL;
	byte altOffset = 0;
	byte altLength = 0;

	bool unknownsHit = false;

	unsigned long highestAbsoluteTime = 0;
	unsigned long highestAbsoluteTimeByTrack[0x10];
	for (int x = 0; x < 0x10; x++)
		highestAbsoluteTimeByTrack[x] = 0;

	for (int trackNum = 0; trackNum < numberTracks; trackNum++)
	{
		unsigned long absoluteTime = 0;

		unsigned long trackHeader = ((((((inputMID[position] << 8) | inputMID[position+1]) << 8) | inputMID[position+2]) << 8) | inputMID[position+3]);
		if (trackHeader != 0x4D54726B)
		{
			MessageBox(NULL, "Invalid track midi hdr", "Error", NULL);
			return false;
		}
		
		unsigned long trackLength = ((((((inputMID[position+4] << 8) | inputMID[position+5]) << 8) | inputMID[position+6]) << 8) | inputMID[position+7]);

		position += 8;

		byte previousEventValue = 0xFF;

		bool endFlag = false;

	

		while (!endFlag && (position < results.st_size))
		{
			unsigned long original;
			unsigned long timeTag = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, false);
			absoluteTime += timeTag;

			byte eventVal = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

			bool statusBit = false;

			if (eventVal <= 0x7F)
			{
				// continuation
				statusBit = true;
			}
			else
			{
				statusBit = false;
			}

			if (eventVal == 0xFF) // meta event
			{
				byte subType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				if (subType == 0x2F) //End of Track Event.
				{
					// remove time till end
					absoluteTime -= timeTag;

					endFlag = true;
					unsigned long length = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);  // end 00 in real mid
				}
				else if (subType == 0x51) //Set Tempo Event.
				{
					unsigned long length = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false); 
										
					ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				}
				//Various Unused Meta Events.
				else if ((subType < 0x7F) && !(subType == 0x51 || subType == 0x2F))
				{
					unsigned long length = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false); 

					for (int i = 0; i < length; i++)
						ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				}
				else if (subType == 0x7F) //Unused Sequencer Specific Event.
				{
					int length = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, false);
					// subtract length
					for (int i = 0; i < length; i++)
					{
						ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					}
				}

				previousEventValue = eventVal;
			}
			else if ((eventVal >= 0x80 && eventVal < 0x90) || (statusBit && (previousEventValue >= 0x80 && previousEventValue < 0x90)))
			{
				byte curEventVal;

				byte noteNumber;
				if (statusBit)
				{
					noteNumber = eventVal;
					curEventVal = previousEventValue;
				}
				else
				{
					noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					curEventVal = eventVal;
				}
				byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if ((eventVal >= 0x90 && eventVal < 0xA0) || (statusBit && (previousEventValue >= 0x90 && previousEventValue < 0xA0)))
			{
				byte curEventVal;

				byte noteNumber;
				if (statusBit)
				{
					noteNumber = eventVal;
					curEventVal = previousEventValue;
				}
				else
				{
					noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					curEventVal = eventVal;
				}
				byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (((eventVal >= 0xB0) && (eventVal < 0xC0))  || (statusBit && (previousEventValue >= 0xB0 && previousEventValue < 0xC0))) // controller change
			{
				byte controllerType;
				if (statusBit)
				{
					controllerType = eventVal;
				}
				else
				{
					controllerType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				}

				byte controllerValue = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (((eventVal >= 0xC0) && (eventVal < 0xD0)) || (statusBit && (previousEventValue >= 0xC0 && previousEventValue < 0xD0))) // change instrument
			{
				byte instrument;
				if (statusBit)
				{
					instrument = eventVal;
				}
				else
				{
					instrument = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				}

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (((eventVal >= 0xD0) && (eventVal < 0xE0))  || (statusBit && (previousEventValue >= 0xD0 && previousEventValue < 0xE0))) // channel aftertouch
			{
				byte amount;
				if (statusBit)
				{
					amount = eventVal;
				}
				else
				{
					amount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				}

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (((eventVal >= 0xE0) && (eventVal < 0xF0))  || (statusBit && (previousEventValue >= 0xE0 && previousEventValue < 0xF0))) // pitch bend
			{
				byte valueLSB;
				if (statusBit)
				{
					valueLSB = eventVal;
				}
				else
				{
					valueLSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				}

				byte valueMSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (eventVal == 0xF0 || eventVal == 0xF7)
            {
				int length = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, false);
				// subtract length
				for (int i = 0; i < length; i++)
				{
					ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				}
            }
			else
			{
				if (!unknownsHit)
				{
					MessageBox(NULL, "Invalid midi character found", "Error", NULL);
					unknownsHit = true;
				}
			}
		}

		if (absoluteTime > highestAbsoluteTime)
		{
			highestAbsoluteTime = absoluteTime;
		}

		if (absoluteTime > highestAbsoluteTimeByTrack[trackNum])
		{
			highestAbsoluteTimeByTrack[trackNum] = absoluteTime;
		}
	}


	position = 0xE;

	repeatPattern = NULL;
	altOffset = 0;
	altLength = 0;

	for (int trackNum = 0; trackNum < numberTracks; trackNum++)
	{
		unsigned long absoluteTime = 0;

		unsigned long trackHeader = ((((((inputMID[position] << 8) | inputMID[position+1]) << 8) | inputMID[position+2]) << 8) | inputMID[position+3]);
		if (trackHeader != 0x4D54726B)
		{
			MessageBox(NULL, "Invalid track midi hdr", "Error", NULL);
			return false;
		}
		
		unsigned long trackLength = ((((((inputMID[position+4] << 8) | inputMID[position+5]) << 8) | inputMID[position+6]) << 8) | inputMID[position+7]);

		position += 8;

		byte previousEventValue = 0xFF;

		bool endFlag = false;

		bool didLoop = false;

		if ((loop) && (loopPoint == 0) && (highestAbsoluteTimeByTrack[trackNum] > 0))
		{
			TrackEvent* newTrackEvent = (TrackEvent*)&(trackEvents[trackNum][trackEventCount[trackNum]]);
			newTrackEvent->type = 0xFF;
			newTrackEvent->absoluteTime = 0;
			newTrackEvent->contentSize = 3;
			newTrackEvent->contents = new byte[newTrackEvent->contentSize];
			newTrackEvent->contents[0] = 0x2E;
			newTrackEvent->contents[1] = 0x00;
			newTrackEvent->contents[2] = 0xFF;
			newTrackEvent->deltaTime = 0;
			newTrackEvent->obsoleteEvent = false;
			
			trackEventCount[trackNum]++;
			didLoop = true;
		}

		
		while (!endFlag && (position < results.st_size))
		{
			unsigned long original;
			unsigned long timeTag = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, false);
			absoluteTime += timeTag;

			TrackEvent* newTrackEvent = (TrackEvent*)&(trackEvents[trackNum][trackEventCount[trackNum]]);
			newTrackEvent->deltaTime = timeTag;
			newTrackEvent->obsoleteEvent = false;
			newTrackEvent->contents = NULL;
			newTrackEvent->absoluteTime = absoluteTime;

			if ((loop) && (!didLoop) && (highestAbsoluteTimeByTrack[trackNum] > loopPoint))
			{
				if (absoluteTime == loopPoint)
				{
					TrackEvent* newTrackEventLoop = (TrackEvent*)&(trackEvents[trackNum][trackEventCount[trackNum]]);
					newTrackEventLoop->type = 0xFF;
					newTrackEventLoop->absoluteTime = absoluteTime;
					newTrackEventLoop->contentSize = 3;
					newTrackEventLoop->contents = new byte[newTrackEventLoop->contentSize];
					newTrackEventLoop->contents[0] = 0x2E;
					newTrackEventLoop->contents[1] = 0x00;
					newTrackEventLoop->contents[2] = 0xFF;
					newTrackEventLoop->deltaTime = timeTag;
					newTrackEventLoop->obsoleteEvent = false;
					
					trackEventCount[trackNum]++;

					newTrackEvent = (TrackEvent*)&(trackEvents[trackNum][trackEventCount[trackNum]]);
					newTrackEvent->deltaTime = 0;
					newTrackEvent->obsoleteEvent = false;
					newTrackEvent->contents = NULL;
					newTrackEvent->absoluteTime = absoluteTime;

					didLoop = true;
				}
				else if (absoluteTime > loopPoint)
				{
					TrackEvent* newTrackEventLoop = (TrackEvent*)&(trackEvents[trackNum][trackEventCount[trackNum]]);
					newTrackEventLoop->type = 0xFF;
					newTrackEventLoop->absoluteTime = loopPoint;
					newTrackEventLoop->contentSize = 3;
					newTrackEventLoop->contents = new byte[newTrackEventLoop->contentSize];
					newTrackEventLoop->contents[0] = 0x2E;
					newTrackEventLoop->contents[1] = 0x00;
					newTrackEventLoop->contents[2] = 0xFF;
					if (trackEventCount[trackNum] > 0)
						newTrackEventLoop->deltaTime = (loopPoint - ((TrackEvent*)&(trackEvents[trackNum][trackEventCount[trackNum]-1]))->absoluteTime);
					else
						newTrackEventLoop->deltaTime = loopPoint;
					newTrackEventLoop->obsoleteEvent = false;
					
					trackEventCount[trackNum]++;

					newTrackEvent = (TrackEvent*)&(trackEvents[trackNum][trackEventCount[trackNum]]);
					newTrackEvent->deltaTime = (absoluteTime - loopPoint);
					newTrackEvent->obsoleteEvent = false;
					newTrackEvent->contents = NULL;
					newTrackEvent->absoluteTime = absoluteTime;

					didLoop = true;
				}
			}

			byte eventVal = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

			bool statusBit = false;

			if (eventVal <= 0x7F)
			{
				// continuation
				statusBit = true;
			}
			else
			{
				statusBit = false;
			}

			if (eventVal == 0xFF) // meta event
			{
				byte subType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				if (subType == 0x2F) //End of Track Event.
				{
					endFlag = true;

					if (loop && (highestAbsoluteTimeByTrack[trackNum] > loopPoint))
					{

						TrackEvent* prevEvent = ((TrackEvent*)&(trackEvents[trackNum][trackEventCount[trackNum]-1]));
						if (
							(prevEvent->type == 0xFF)
							&& (prevEvent->contentSize > 0)
							&& (prevEvent->contents[0] == 0x2E)
							)
						{
							newTrackEvent = prevEvent;

							delete [] newTrackEvent->contents;
							newTrackEvent->type = 0xFF;
							newTrackEvent->contentSize = 1;
							newTrackEvent->contents = new byte[newTrackEvent->contentSize];
							newTrackEvent->contents[0] = 0x2F;
						}
						else
						{
							TrackEvent* newTrackEventLast = (TrackEvent*)&(trackEvents[trackNum][trackEventCount[trackNum]+1]);
							newTrackEventLast->absoluteTime = highestAbsoluteTime;
							newTrackEventLast->deltaTime = 0;
							newTrackEventLast->durationTime = newTrackEvent->durationTime;
							newTrackEventLast->obsoleteEvent = newTrackEvent->obsoleteEvent;

							newTrackEventLast->type = 0xFF;
							newTrackEventLast->contentSize = 1;
							newTrackEventLast->contents = new byte[newTrackEventLast->contentSize];
							newTrackEventLast->contents[0] = 0x2F;

							newTrackEvent->type = 0xFF;
							if (highestAbsoluteTime > (prevEvent->absoluteTime + prevEvent->durationTime))
							{
								newTrackEvent->deltaTime = (highestAbsoluteTime - (prevEvent->absoluteTime + prevEvent->durationTime));
								newTrackEvent->absoluteTime = highestAbsoluteTime;
							}
							else
							{
								newTrackEvent->deltaTime = 0;
								newTrackEvent->absoluteTime = prevEvent->absoluteTime;
							}

							newTrackEvent->contentSize = 7;
							newTrackEvent->contents = new byte[newTrackEvent->contentSize];
							newTrackEvent->contents[0] = 0x2D;
							newTrackEvent->contents[1] = 0xFF;
							newTrackEvent->contents[2] = 0xFF;
							newTrackEvent->contents[3] = 0x0; // todo write location
							newTrackEvent->contents[4] = 0x0;
							newTrackEvent->contents[5] = 0x0;
							newTrackEvent->contents[6] = 0x0;
							newTrackEvent->obsoleteEvent = false;
							
							trackEventCount[trackNum]++;
						}
					}
					else
					{
						newTrackEvent->type = 0xFF;
						newTrackEvent->contentSize = 1;
						newTrackEvent->contents = new byte[newTrackEvent->contentSize];
						newTrackEvent->contents[0] = 0x2F;
					}

					unsigned long length = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);  // end 00 in real mid
				}
				else if (subType == 0x51) //Set Tempo Event.
				{
					newTrackEvent->type = 0xFF;
					newTrackEvent->contentSize = 4;
					newTrackEvent->contents = new byte[newTrackEvent->contentSize];
					newTrackEvent->contents[0] = 0x51;

					unsigned long length = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false); 
										
					newTrackEvent->contents[1] = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					newTrackEvent->contents[2] = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					newTrackEvent->contents[3] = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				}
				//Various Unused Meta Events.
				else if ((subType < 0x7F) && !(subType == 0x51 || subType == 0x2F))
				{
					newTrackEvent->type = 0xFF;
					unsigned long length = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false); 

					for (int i = 0; i < length; i++)
						ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

					newTrackEvent->obsoleteEvent = true;
				}
				else if (subType == 0x7F) //Unused Sequencer Specific Event.
				{
					newTrackEvent->type = 0xFF;
					int length = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, false);
					// subtract length
					for (int i = 0; i < length; i++)
					{
						ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					}
					
					newTrackEvent->obsoleteEvent = true;
				}

				previousEventValue = eventVal;
			}
			else if ((eventVal >= 0x80 && eventVal < 0x90) || (statusBit && (previousEventValue >= 0x80 && previousEventValue < 0x90)))
			{
				byte curEventVal;

				byte noteNumber;
				if (statusBit)
				{
					newTrackEvent->type = previousEventValue;
					noteNumber = eventVal;
					curEventVal = previousEventValue;
				}
				else
				{
					newTrackEvent->type = eventVal;
					noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					curEventVal = eventVal;
				}
				byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				for (int testBackwards = (trackEventCount[trackNum] - 1); testBackwards >= 0; testBackwards--)
				{
					if ((trackEvents[trackNum][testBackwards].type == (0x90 | (curEventVal & 0xF))) && !(trackEvents[trackNum][testBackwards].obsoleteEvent))
					{
						if (trackEvents[trackNum][testBackwards].contents[0] == noteNumber)
						{
							trackEvents[trackNum][testBackwards].durationTime = (absoluteTime - trackEvents[trackNum][testBackwards].absoluteTime);
							break;
						}
					}
				}


				newTrackEvent->durationTime = 0; 
				newTrackEvent->contentSize = 2;
				newTrackEvent->contents = new byte[newTrackEvent->contentSize];
				newTrackEvent->contents[0] = noteNumber;
				newTrackEvent->contents[1] = velocity;
				newTrackEvent->obsoleteEvent = true;

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if ((eventVal >= 0x90 && eventVal < 0xA0) || (statusBit && (previousEventValue >= 0x90 && previousEventValue < 0xA0)))
			{
				byte curEventVal;

				byte noteNumber;
				if (statusBit)
				{
					newTrackEvent->type = previousEventValue;
					noteNumber = eventVal;
					curEventVal = previousEventValue;
				}
				else
				{
					newTrackEvent->type = eventVal;
					noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					curEventVal = eventVal;
				}
				byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				if (velocity == 0)
				{
					// simulate note off
					for (int testBackwards = (trackEventCount[trackNum] - 1); testBackwards >= 0; testBackwards--)
					{
						if (((trackEvents[trackNum][testBackwards].type == curEventVal)) && !(trackEvents[trackNum][testBackwards].obsoleteEvent))
						{
							if (trackEvents[trackNum][testBackwards].contents[0] == noteNumber)
							{
								trackEvents[trackNum][testBackwards].durationTime = (absoluteTime - trackEvents[trackNum][testBackwards].absoluteTime);
								break;
							}
						}
					}

					newTrackEvent->durationTime = 0;
					newTrackEvent->contentSize = 2;
					newTrackEvent->contents = new byte[newTrackEvent->contentSize];
					newTrackEvent->contents[0] = noteNumber;
					newTrackEvent->contents[1] = velocity;
					newTrackEvent->obsoleteEvent = true;
				}
				else
				{
					// check if no note off received, if so, turn it off and restart note
					for (int testBackwards = (trackEventCount[trackNum] - 1); testBackwards >= 0; testBackwards--)
					{
						if (((trackEvents[trackNum][testBackwards].type == curEventVal)) && !(trackEvents[trackNum][testBackwards].obsoleteEvent))
						{
							if (trackEvents[trackNum][testBackwards].contents[0] == noteNumber)
							{
								if (trackEvents[trackNum][testBackwards].durationTime == 0) // means unfinished note
									trackEvents[trackNum][testBackwards].durationTime = (absoluteTime - trackEvents[trackNum][testBackwards].absoluteTime);
								break;
							}
						}
					}

					newTrackEvent->durationTime = 0; // to be filled in
					newTrackEvent->contentSize = 2;
					newTrackEvent->contents = new byte[newTrackEvent->contentSize];
					newTrackEvent->contents[0] = noteNumber;
					newTrackEvent->contents[1] = velocity;
				}

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (((eventVal >= 0xB0) && (eventVal < 0xC0))  || (statusBit && (previousEventValue >= 0xB0 && previousEventValue < 0xC0))) // controller change
			{
				byte controllerType;
				if (statusBit)
				{
					controllerType = eventVal;
					newTrackEvent->type = previousEventValue;
				}
				else
				{
					controllerType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					newTrackEvent->type = eventVal;
				}

				byte controllerValue = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				newTrackEvent->contentSize = 2;
				newTrackEvent->contents = new byte[newTrackEvent->contentSize];
				newTrackEvent->contents[0] = controllerType;
				newTrackEvent->contents[1] = controllerValue;
				
				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (((eventVal >= 0xC0) && (eventVal < 0xD0)) || (statusBit && (previousEventValue >= 0xC0 && previousEventValue < 0xD0))) // change instrument
			{
				byte instrument;
				if (statusBit)
				{
					instrument = eventVal;
					newTrackEvent->type = previousEventValue;
				}
				else
				{
					instrument = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					newTrackEvent->type = eventVal;
				}

				if ((eventVal & 0xF) == 9) // Drums in GM
					instrument = instrument;
				else
					instrument = instrument;

				newTrackEvent->contentSize = 1;
				newTrackEvent->contents = new byte[newTrackEvent->contentSize];
				newTrackEvent->contents[0] = instrument;
				
				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (((eventVal >= 0xD0) && (eventVal < 0xE0))  || (statusBit && (previousEventValue >= 0xD0 && previousEventValue < 0xE0))) // channel aftertouch
			{
				newTrackEvent->type = eventVal;
				byte amount;
				if (statusBit)
				{
					amount = eventVal;
					newTrackEvent->type = previousEventValue;
				}
				else
				{
					amount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					newTrackEvent->type = eventVal;
				}

				newTrackEvent->contentSize = 1;
				newTrackEvent->contents = new byte[newTrackEvent->contentSize];
				newTrackEvent->contents[0] = amount;
				//newTrackEvent->obsoleteEvent = true; // temporary?

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (((eventVal >= 0xE0) && (eventVal < 0xF0))  || (statusBit && (previousEventValue >= 0xE0 && previousEventValue < 0xF0))) // pitch bend
			{
				newTrackEvent->type = eventVal;
				byte valueLSB;
				if (statusBit)
				{
					valueLSB = eventVal;
					newTrackEvent->type = previousEventValue;
				}
				else
				{
					valueLSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					newTrackEvent->type = eventVal;
				}

				byte valueMSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				newTrackEvent->contentSize = 2;
				newTrackEvent->contents = new byte[newTrackEvent->contentSize];
				newTrackEvent->contents[0] = valueLSB;
				newTrackEvent->contents[1] = valueMSB;
				//newTrackEvent->obsoleteEvent = true; // temporary?

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (eventVal == 0xF0 || eventVal == 0xF7)
            {
                newTrackEvent->type = eventVal;
				int length = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, false);
				// subtract length
				for (int i = 0; i < length; i++)
				{
					ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				}

				newTrackEvent->obsoleteEvent = true;
            }
			else
			{
				if (!unknownsHit)
				{
					MessageBox(NULL, "Invalid midi character found", "Error", NULL);
					unknownsHit = true;
				}
			}

			trackEventCount[trackNum]++;
		}
	}

	unsigned long timeOffset = 0;

	unsigned long startPosition = 0x44;

	// get offsets
	for (int i = 0; i < numberTracks; i++)
	{
		unsigned long sizeData = 0;
		int loopStartPosition = 0;
		bool foundLoopStart = false;
		byte previousTrackEvent = 0x0;

		if (trackEventCount[i] > 0)
		{
			unsigned long tempLong = Flip32Bit(startPosition);
			fwrite(&tempLong, 1, 4, outFile);

			for (int j = 0; j < trackEventCount[i]; j++)
			{
				TrackEvent* trackEvent =  &(trackEvents[i][j]);
				unsigned long lengthTimeDelta = 0;
				unsigned long timeDelta = ReturnVLBytes((trackEvent->deltaTime + timeOffset), lengthTimeDelta);

				if (trackEvent->obsoleteEvent)
				{
					timeOffset += trackEvent->deltaTime;
				}
				else
				{
					if ((trackEvent->type == 0xFF) && (trackEvent->contents[0] == 0x2E))
					{
						foundLoopStart = true;
						loopStartPosition = (startPosition + sizeData + 1 + trackEvent->contentSize + lengthTimeDelta);
					}
					
					timeOffset = 0;

					sizeData += lengthTimeDelta;

					if ((trackEvent->type == 0xFF) && (trackEvent->contents[0] == 0x2D))
					{
						unsigned long offsetBack = ((startPosition + sizeData) - loopStartPosition + 8);
						trackEvent->contents[3] = ((offsetBack >> 24) & 0xFF);
						trackEvent->contents[4] = ((offsetBack >> 16) & 0xFF);
						trackEvent->contents[5] = ((offsetBack >> 8) & 0xFF);
						trackEvent->contents[6] = ((offsetBack >> 0) & 0xFF);
					}

					if ((trackEvent->type != previousTrackEvent) || (trackEvent->type == 0xFF))
					{
						sizeData += 1;
					}

					sizeData += trackEvent->contentSize;

					if ((trackEvent->type >= 0x90) && (trackEvent->type < 0xA0))
					{
						unsigned long lengthDurationBytes = 0;
						unsigned long duration = ReturnVLBytes(trackEvent->durationTime, lengthDurationBytes);

						sizeData += lengthDurationBytes;
					}

					previousTrackEvent = trackEvent->type;
				}
			}
			startPosition += sizeData;
		}
		else
		{
			unsigned long zero = 0;
			fwrite(&zero, 1, 4, outFile);
		}
	}

	for (int i = numberTracks; i < 16; i++)
	{
		unsigned long zero = 0;
		fwrite(&zero, 1, 4, outFile);
	}

	unsigned long divisionFlipped = Flip32Bit((unsigned long)tempo);
	fwrite(&divisionFlipped, 1, 4, outFile);

	//FILE* outDebug = fopen("C:\\GoldeneyeStuff\\GE Editor Source\\debug.txt", "w");
	FILE* outDebug = NULL;

	for (int i = 0; i < numberTracks; i++)
	{
		if (trackEventCount[i] > 0)
		{
			if (outDebug != NULL) fprintf(outDebug, "Track %X\n", i);
			byte previousTrackEvent = 0x0;
			for (int j = 0; j < trackEventCount[i]; j++)
			{
				TrackEvent* trackEvent =  &(trackEvents[i][j]);
				if (trackEvent->obsoleteEvent)
				{
					timeOffset += trackEvent->deltaTime;
				}
				else
				{
					unsigned long lengthTimeDelta = 0;
					unsigned long timeDelta = ReturnVLBytes((trackEvent->deltaTime + timeOffset), lengthTimeDelta);
					timeOffset = 0;
					WriteVLBytes(outFile, timeDelta, lengthTimeDelta, false);

					for (int ii = 0; ii < lengthTimeDelta; ii++)
						if (outDebug != NULL) fprintf(outDebug, "%02X", ((timeDelta >> ((lengthTimeDelta * 8) - 8 - (ii * 8))) & 0xFF));

					if ((trackEvent->type != previousTrackEvent) || (trackEvent->type == 0xFF))
					{
						fwrite(&trackEvent->type, 1, 1, outFile);
						if (outDebug != NULL) fprintf(outDebug, "%02X", trackEvent->type);
					}

					fwrite(trackEvent->contents, 1, trackEvent->contentSize, outFile);
					for (int ii = 0; ii < trackEvent->contentSize; ii++)
					{
						if (outDebug != NULL) fprintf(outDebug, "%02X", trackEvent->contents[ii]);
					}

					if ((trackEvent->type >= 0x90) && (trackEvent->type < 0xA0))
					{
						unsigned long lengthDurationBytes = 0;
						unsigned long duration = ReturnVLBytes(trackEvent->durationTime, lengthDurationBytes); // todo real trackevent
						WriteVLBytes(outFile, duration, lengthDurationBytes, false);
						for (int ii = 0; ii < lengthDurationBytes; ii++)
							if (outDebug != NULL) fprintf(outDebug, "%02X", ((duration >> ((lengthDurationBytes * 8) - 8 - (ii * 8))) & 0xFF));
					}

					if (outDebug != NULL) fprintf(outDebug, "\n");
					previousTrackEvent = trackEvent->type;
				}
				
			}
		}

		for (int j = 0; j < trackEventCount[i]; j++)
		{
			if (trackEvents[i][j].contents != NULL)
			{
				delete [] trackEvents[i][j].contents;
				trackEvents[i][j].contents = NULL;
			}
		}
	}

	fflush(outDebug);

	fclose(outFile);

	// write FEFE in case see FE
	outFile = fopen(output, "rb");
	if (outFile == NULL)
	{
		MessageBox(NULL, "Error reading file", "Error", NULL);
		return false;
	}	
	fseek(outFile, 0, SEEK_END);
	int sizeOut = ftell(outFile);
	rewind(outFile);
	byte* inArray = new byte[sizeOut];
	fread(inArray, 1, sizeOut, outFile);
	fclose(outFile);

	unsigned long offsetheader[0x10];
	int extraOffsets[0x10];
	for (int x = 0; x < 0x40; x+=4)
	{
		offsetheader[x/4] = ((((((inArray[x] << 8) | inArray[x+1]) << 8) | inArray[x+2]) << 8) | inArray[x+3]);
		extraOffsets[x/4] = 0x00000000;
	}
	
	for (int x = 0; x < sizeOut; x++)
	{
		if (x > 0x44)
		{
			if (inArray[x] == 0xFE) // need to write twice
			{
				for (int y = 0; y < numberTracks; y++)
				{
					if (offsetheader[y] > x)
					{
						extraOffsets[y]++;
					}
				}
			}
		}
	}

	outFile = fopen(output, "wb");
	if (outFile == NULL)
	{
		MessageBox(NULL, "Error reading file", "Error", NULL);
		return false;
	}

	for (int x = 0; x < 0x10; x++)
	{
		WriteLongToBuffer(inArray, x*4, offsetheader[x] + extraOffsets[x]);
	}
	for (int x = 0; x < sizeOut; x++)
	{
		fwrite(&inArray[x], 1, 1, outFile);
		if (x > 0x44)
		{
			if (inArray[x] == 0xFE) // need to write twice
				fwrite(&inArray[x], 1, 1, outFile);
		}
	}
	fclose(outFile);
	delete [] inArray;

	//return true;

	if (useRepeaters)
	{
		// "compress"
		outFile = fopen(output, "rb");
		if (outFile == NULL)
		{
			MessageBox(NULL, "Error reading file", "Error", NULL);
			return false;
		}	
		fseek(outFile, 0, SEEK_END);
		int sizeIn = ftell(outFile);
		rewind(outFile);
		inArray = new byte[sizeIn];
		fread(inArray, 1, sizeIn, outFile);
		fclose(outFile);

		unsigned long offset[0x10];
		for (int x = 0; x < 0x40; x+=4)
		{
			offset[x/4] = ((((((inArray[x] << 8) | inArray[x+1]) << 8) | inArray[x+2]) << 8) | inArray[x+3]);
		}
		unsigned quarternote = ((((((inArray[0x40] << 8) | inArray[0x41]) << 8) | inArray[0x42]) << 8) | inArray[0x43]);

		unsigned char* outArray = new unsigned char[sizeIn * 4];
		for (int x = 0x0; x < (sizeIn * 4); x++)
		{
			outArray[x] = 0x0;
		}

		unsigned long offsetNew[0x10];
		for (int x = 0; x < 0x10; x++)
		{
			offsetNew[x] = 0;
		}


		int outputSpot = 0x44;
		

		for (int x = 0x0; x < 0x10; x++)
		{
			if (offset[x] != 0)
			{
				offsetNew[x] = outputSpot;

				int outputStart = outputSpot;

				int endSpot = sizeIn;

				if (x < 0xF)
				{
					if (offset[x+1] != 0)
					{
						endSpot = offset[x+1];
					}
				}

				int y = offset[x];
				// loop till end of file
				while (y < endSpot)
				{
					int bestMatchOffset = -1;
					int bestMatchLoopCount = -1;


					// check from past to now for results
					for (int z = outputStart; z < outputSpot; z++)
					{
						int match = 0;
						int matchOffset = 0;

						// one spot match start here
						while  ((outArray[z+matchOffset] == inArray[y+matchOffset]) && ((y+matchOffset) < endSpot) && (outArray[z+matchOffset] != 0xFE) && (outArray[z+matchOffset] != 0xFF) && ((z+matchOffset) < outputSpot))
						{
							bool seeAnFF = false;
							for (int checkFF = y+matchOffset; ((checkFF < endSpot) && (checkFF < (y+matchOffset + 5))); checkFF++)
							{
								if (inArray[checkFF] == 0xFF)
									seeAnFF = true;
							}

							if (seeAnFF)
							{
								break;
							}

							matchOffset++;
						}

						if ((matchOffset > bestMatchLoopCount) && (matchOffset > 6))
						{
							bestMatchLoopCount = matchOffset;
							bestMatchOffset = z;
						}
					}

					int loopCheck = ((y - offset[x]) / 2);
					if (loopCheck > 0xFD)
						loopCheck = 0xFD;

					if (bestMatchLoopCount > 6)
					{
						if (bestMatchLoopCount > 0xFD)
							bestMatchLoopCount = 0xFD;

						outArray[outputSpot++] = 0xFE;
						int distBack = ((outputSpot - bestMatchOffset) - 1);

						outArray[outputSpot++] = ((distBack >> 8) & 0xFF);
						outArray[outputSpot++] = (distBack & 0xFF);
						outArray[outputSpot++] = bestMatchLoopCount;

						y += bestMatchLoopCount;
					}
					else
					{
						// write one
						outArray[outputSpot++] = inArray[y];

						y++;
					}
				}
			}
			else
				break;

			if ((outputSpot % 4) != 0)
				outputSpot += (4 - (outputSpot % 4));
		}

		// correct loops
		for (int x = 0x0; x < 0x10; x++)
		{
			if (offsetNew[x] != 0)
			{
				int outputStart = offsetNew[x];

				int endSpot = outputSpot;

				if (x < 0xF)
				{
					if (offsetNew[x+1] != 0)
					{
						endSpot = offsetNew[x+1];
					}
				}

				int y = offsetNew[x];
				// loop till end of file

				bool foundStart = false;
				int startPos = 0;
				while (y < endSpot)
				{
					if ((outArray[y] == 0xFF) && (outArray[y+1] == 0x2E) && (outArray[y+2] == 0x00) && (outArray[y+3] == 0xFF)) 
					{
						foundStart = true;
						startPos = y + 4;
						y+=4;
					}
					else if ((outArray[y] == 0xFF) && (outArray[y+1] == 0x2D) && (outArray[y+2] == 0xFF) && (outArray[y+3] == 0xFF)) 
					{
						if (foundStart)
						{
							int distance = ((y + 8) - startPos);

							WriteLongToBuffer(outArray, y+4, distance);
							foundStart = false;
						}

						y+=8;
					}
					else
					{
						y++;
					}

					
				}
			}
		}

		for (int x = 0x0; x < 0x10; x++)
		{
			WriteLongToBuffer(outArray, x*4, offsetNew[x]);
		}
		WriteLongToBuffer(outArray, 0x40, quarternote);


		outFile = fopen(output, "wb");
		if (outFile == NULL)
		{
			MessageBox(NULL, "Error reading file", "Error", NULL);
			return false;
		}

		for (int x = 0; x < outputSpot; x++)
		{
			fwrite(&outArray[x], 1, 1, outFile);
		}
		fclose(outFile);

		delete [] inArray;
		delete [] outArray;
	}
}
catch (...)
{
	MessageBox(NULL, "Error converting", "Error", NULL);
	return false;
}
	return true;
}


unsigned long CMidiParse::Flip32Bit(unsigned long inLong)
{
	return (((inLong & 0xFF000000) >> 24) | ((inLong & 0x00FF0000) >> 8) | ((inLong & 0x0000FF00) << 8) | ((inLong & 0x000000FF) << 24));
}


void CMidiParse::WriteLongToBuffer(unsigned char* Buffer, unsigned long address, unsigned long data)
{
	Buffer[address] = ((data >> 24) & 0xFF);
	Buffer[address+1] = ((data >> 16) & 0xFF);
	Buffer[address+2] = ((data >> 8) & 0xFF);
	Buffer[address+3] = ((data) & 0xFF);
}

void CMidiParse::WriteShortToBuffer(unsigned char* Buffer, unsigned long address, unsigned short data)
{
	Buffer[address] = ((data >> 8) & 0xFF);
	Buffer[address+1] = ((data) & 0xFF);	
}



unsigned short CMidiParse::CharArrayToShort(unsigned char* currentSpot)
{
	return Flip16Bit(*reinterpret_cast<unsigned short*> (currentSpot));
}

unsigned short CMidiParse::Flip16bit(unsigned short tempShort)
{
	return (((tempShort & 0xFF00) >> 8) | ((tempShort & 0x00FF) << 8));
}


unsigned long CMidiParse::CharArrayToLong(unsigned char* currentSpot)
{
	return Flip32Bit(*reinterpret_cast<unsigned long*> (currentSpot));
}



unsigned char CMidiParse::StringToUnsignedChar(CString inString)
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

unsigned long CMidiParse::StringHexToLong(CString inString)
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



void CMidiParse::MidiToDebugTextFile(CString midiFile, CString textFileOut)
{
	CString tempFileName = midiFile;

		struct stat results;
		stat(tempFileName, &results);		


		
		FILE* inFile1 = fopen(midiFile, "rb");
		if (inFile1 == NULL)
		{
			MessageBox(NULL, "Error reading file", "Error", NULL);
			return;
		}

		byte* inputMID = new byte[results.st_size];
		fread(inputMID, 1, results.st_size, inFile1);
		fclose(inFile1);


		FILE* outFile = fopen(textFileOut, "w");
		if (outFile == NULL)
		{
			MessageBox(NULL, "Error outputting file", "Error", NULL);
			return;
		}

		unsigned long lengthHeader = 0x44;

		fprintf(outFile, "Offset Start Midi Events: %08X \n", lengthHeader);

		fprintf(outFile, "Tracks\n");

		// parse midi
		
		int counterTrack = 0;

		for (int i = 0; i < (lengthHeader - 4); i+=4) // ignore last 00000180
		{
			unsigned long offset = CharArrayToLong(&inputMID[i]);
			fprintf(outFile, "Track %X Offset %X: Track Offset %08X\n", counterTrack, i, offset);

			int position = offset;	

			

			if (position != 0)
			{
				int previousEventValue = 0;

				byte* repeatPattern = NULL;
				byte altOffset = 0;
				byte altLength = 0;

				int timeAbsolute = 0;
				bool endFlag = false;
				while ((position < results.st_size) && !endFlag)
				{
					int timePosition = position;

					unsigned long original;
					unsigned long timeTag = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, true);
					timeAbsolute += timeTag;

					byte eventVal = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

					bool statusBit = false;

					if (eventVal < 0x80)
					{
						// continuation
						statusBit = true;
						fprintf(outFile, "Offset: %08X - Event Delta Time: %d -Abs %d (%08X)  -   ", timePosition, timeTag, timeAbsolute, original);
					}
					else
					{
						statusBit = false;
						fprintf(outFile, "Offset: %08X - Event Delta Time: %d -Abs %d (%08X) -   ", timePosition, timeTag, timeAbsolute, original);
					}

					if ((eventVal == 0xFF) || (statusBit && (previousEventValue == 0xFF))) // meta event
					{
						byte subType;
						if (statusBit)
							subType = eventVal;
						else
							subType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						if (subType == 0x51) // tempo
						{
							int microsecondsSinceQuarterNote = ((((ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true));
							int MICROSECONDS_PER_MINUTE = 60000000;
							float beatsPerMinute = (float)MICROSECONDS_PER_MINUTE / (float)microsecondsSinceQuarterNote;

							if (statusBit)
								fprintf(outFile, "!%02X%06X - MicroSecondSinceQuarterNote %d (BPM: %f)\n", subType, microsecondsSinceQuarterNote, microsecondsSinceQuarterNote, beatsPerMinute);
							else
								fprintf(outFile, "%02X%02X%06X - MicroSecondSinceQuarterNote %d (BPM: %f)\n", eventVal, subType, microsecondsSinceQuarterNote, microsecondsSinceQuarterNote, beatsPerMinute);
						}
						else if (subType == 0x2D) // end loop
						{
							
							byte loopCount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							byte currentLoopCount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							unsigned long offsetToBeginningLoop = ((((((ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true));

							if (statusBit)
								fprintf(outFile, "!%02X%02X%02X%08X Count %u LoopCount %u OffsetBeginning %u (%04X)\n", subType, loopCount, currentLoopCount, offsetToBeginningLoop, loopCount, currentLoopCount, offsetToBeginningLoop, (position - offsetToBeginningLoop));
							else
								fprintf(outFile, "%02X%02X%02X%02X%08X Count %u LoopCount %u OffsetBeginning %u (%04X)\n", eventVal, subType, loopCount, currentLoopCount, offsetToBeginningLoop, loopCount, currentLoopCount, offsetToBeginningLoop, (position - offsetToBeginningLoop));
							 //meta status byte (0xFF), a loop end subtype byte (0x2D), a loop count byte (0-255), a current loop count (should be the same as the loop count byte), and four bytes that specify the number of bytes difference between the end of the loop end event, and the begining of the loop start event. (note that if this value is calculated before the pattern matching compression takes place, this value will have to be adjusted to compensate for any compression of data that takes place between the loop end and the loop start.) The loop count value should be a zero to loop forever, otherwise it should be set to one less than the number of times the section should repeat. (i.e. to hear a section eight times, you would set the loop count to seven.)

						}
						else if (subType == 0x2E) // start loop
						{
							byte loopNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							byte endLoop = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							if (statusBit)
								fprintf(outFile, "!%02X%02X%02X Loop #%u\n", subType, loopNumber, endLoop, loopNumber);
							else
								fprintf(outFile, "%02X%02X%02X%02X Loop #%u\n", eventVal, subType, loopNumber, endLoop, loopNumber);
						}
						else if (subType == 0x2F)
						{
							if (statusBit)
								fprintf(outFile, "!%02X End of Track\n", subType);
							else
								fprintf(outFile, "%02X%02X End of Track\n", eventVal, subType);
							endFlag = true;
						}

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if ((eventVal >= 0x90 && eventVal < 0xA0) || (statusBit && (previousEventValue >= 0x90) && (previousEventValue < 0xA0)))
					{
						byte noteNumber;
						if (statusBit)
							noteNumber = eventVal;
						else
							noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
						byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						unsigned long timeDuration = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, true);

						if (statusBit)
							fprintf(outFile, "!%02X%02X Duration (%X) - Midi Channel %d NoteNumber %d Velocity %d Duration %d\n", noteNumber, velocity, timeDuration, (eventVal&0xF), noteNumber, velocity, timeDuration);
						else
							fprintf(outFile, "%02X%02X%02X Duration (%X) - Midi Channel %d NoteNumber %d Velocity %d Duration %d\n", eventVal, noteNumber, velocity, timeDuration, (eventVal&0xF), noteNumber, velocity, timeDuration);

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xB0) && (eventVal < 0xC0)) || (statusBit && (previousEventValue >= 0xB0) && (previousEventValue < 0xC0))) // controller change
					{
						CString controllerTypeText = "";
						byte controllerType;
						if (statusBit)
							controllerType = eventVal;
						else
							controllerType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
						byte controllerValue = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						if (controllerType == 0x00) controllerTypeText = "BankSelect";
						else if (controllerType == 0x01) controllerTypeText = "Modulation";
						else if (controllerType == 0x02) controllerTypeText = "BreathController";
						else if (controllerType == 0x04) controllerTypeText = "FootController";
						else if (controllerType == 0x05) controllerTypeText = "PortamentoTime";
						else if (controllerType == 0x06) controllerTypeText = "DataEntryMSB";
						else if (controllerType == 0x07) controllerTypeText = "MainVolume";
						else if (controllerType == 0x08) controllerTypeText = "Balance";
						else if (controllerType == 0x0A) controllerTypeText = "Pan";
						else if (controllerType == 0x0B) controllerTypeText = "ExpressionController";
						else if (controllerType == 0x0C) controllerTypeText = "EffectControl1";
						else if (controllerType == 0x0D) controllerTypeText = "EffectControl2";
						else if ((controllerType >= 0x10) && (controllerType <= 0x13)) controllerTypeText = "General-PurposeControllers01/04/09";
						else if ((controllerType >= 0x20) && (controllerType <= 0x3F)) controllerTypeText = "LSBforcontrollers0-31";
						else if (controllerType == 0x40) controllerTypeText = "Damperpedalsustain";
						else if (controllerType == 0x41) controllerTypeText = "Portamento";
						else if (controllerType == 0x42) controllerTypeText = "Sostenuto";
						else if (controllerType == 0x43) controllerTypeText = "SoftPedal";
						else if (controllerType == 0x44) controllerTypeText = "LegatoFootswitch";
						else if (controllerType == 0x45) controllerTypeText = "Hold2";
						else if (controllerType == 0x46) controllerTypeText = "SoundController1default:TimberVariation";
						else if (controllerType == 0x47) controllerTypeText = "SoundController2default:Timber/HarmonicContent";
						else if (controllerType == 0x48) controllerTypeText = "SoundController3default:ReleaseTime";
						else if (controllerType == 0x49) controllerTypeText = "SoundController4default:AttackTime";
						else if ((controllerType >= 0x4A) && (controllerType <= 0x4F)) controllerTypeText = "SoundController06/10/09";
						else if ((controllerType >= 0x50) && (controllerType <= 0x53)) controllerTypeText = "General-PurposeControllers05/08/09";
						else if (controllerType == 0x54) controllerTypeText = "PortamentoControl";
						else if (controllerType == 0x5B) controllerTypeText = "Effects1DepthformerlyExternalEffectsDepth";
						else if (controllerType == 0x5C) controllerTypeText = "Effects2DepthformerlyTremoloDepth";
						else if (controllerType == 0x5D) controllerTypeText = "Effects3DepthformerlyChorusDepth";
						else if (controllerType == 0x5E) controllerTypeText = "Effects4DepthformerlyCelesteDetune";
						else if (controllerType == 0x5F) controllerTypeText = "Effects5DepthformerlyPhaserDepth";
						else if (controllerType == 0x60) controllerTypeText = "DataIncrement";
						else if (controllerType == 0x61) controllerTypeText = "DataDecrement";
						else if (controllerType == 0x62) controllerTypeText = "Non-RegisteredParameterNumberLSB";
						else if (controllerType == 0x63) controllerTypeText = "Non-RegisteredParameterNumberMSB";
						else if (controllerType == 0x64) controllerTypeText = "RegisteredParameterNumberLSB";
						else if (controllerType == 0x65) controllerTypeText = "RegisteredParameterNumberMSB";
						else if ((controllerType >= 0x79) && (controllerType <= 0x7F)) controllerTypeText = "ModeMessages";

						if (statusBit)
							fprintf(outFile, "!%02X%02X - Midi Channel %d ControllerType %d (%s) Value %d\n", controllerType, controllerValue, (eventVal&0xF), controllerType, controllerTypeText, controllerValue);
						else
							fprintf(outFile, "%02X%02X%02X - Midi Channel %d ControllerType %d (%s) Value %d\n", eventVal, controllerType, controllerValue, (eventVal&0xF), controllerType, controllerTypeText, controllerValue);

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xC0) && (eventVal < 0xD0)) || (statusBit && (previousEventValue >= 0xC0) && (previousEventValue < 0xD0))) // change instrument
					{
						byte instrument;
						if (statusBit)
							instrument = eventVal;
						else
							instrument = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						if (statusBit)
						{
							if ((eventVal & 0xF) == 9)
								fprintf(outFile, "!%02X - Midi Channel %d Instrument %d\n", instrument, (eventVal&0xF), instrument);
							else
								fprintf(outFile, "!%02X - Midi Channel %d Instrument %d\n", instrument, (eventVal&0xF), instrument);
						}
						else
						{
							if ((eventVal & 0xF) == 9)
								fprintf(outFile, "%02X%02X - Midi Channel %d Instrument %d\n", eventVal, instrument, (eventVal&0xF), instrument);
							else
								fprintf(outFile, "%02X%02X - Midi Channel %d Instrument %d\n", eventVal, instrument, (eventVal&0xF), instrument);
						}

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xD0) && (eventVal < 0xE0)) || (statusBit && (previousEventValue >= 0xD0) && (previousEventValue < 0xE0))) // channel aftertouch
					{
						byte amount;
						if (statusBit)
							amount = eventVal;
						else
							amount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						if (statusBit)
						{
							if ((eventVal & 0xF) == 9)
								fprintf(outFile, "!%02X - Midi Channel %d Amount %d\n", amount, (eventVal&0xF), amount);
							else
								fprintf(outFile, "!%02X - Midi Channel %d Amount %d\n", amount, (eventVal&0xF), amount);
						}
						else
						{
							if ((eventVal & 0xF) == 9)
								fprintf(outFile, "%02X%02X - Midi Channel %d Amount %d\n", eventVal, amount, (eventVal&0xF), amount);
							else
								fprintf(outFile, "%02X%02X - Midi Channel %d Amount %d\n", eventVal, amount, (eventVal&0xF), amount);
						}

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xE0) && (eventVal < 0xF0)) || (statusBit && (previousEventValue >= 0xE0) && (previousEventValue < 0xF0))) // pitch bend
					{
						byte lsb;
						if (statusBit)
							lsb = eventVal;
						else
							lsb = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						byte msb = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						if (statusBit)
						{
							if ((eventVal & 0xF) == 9)
								fprintf(outFile, "!%02X%02X - Midi Channel %d lsb %d msb %d\n", lsb, msb, (eventVal&0xF), lsb, msb);
							else
								fprintf(outFile, "!%02X%02X - Midi Channel %d lsb %d msb %d\n", lsb, msb, (eventVal&0xF), lsb, msb);
						}
						else
						{
							if ((eventVal & 0xF) == 9)
								fprintf(outFile, "%02X%02X%02X - Midi Channel %d lsb %d msb %d\n", eventVal, lsb, msb, (eventVal&0xF), lsb, msb);
							else
								fprintf(outFile, "%02X%02X%02X - Midi Channel %d lsb %d msb %d\n", eventVal, lsb, msb, (eventVal&0xF), lsb, msb);
						}

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (eventVal == 0xFE) // repeat operation
					{
						// should not be here...

						// no prev event set
					}
					else
					{
						fprintf(outFile, "%02X ERROR MISSING PARSE OF TYPE\n", eventVal);
					}
				}
			}
			else
			{
				fprintf(outFile, "No Track Data\n");
			}

			fprintf(outFile, "\n");
			counterTrack++;
		}


		delete [] inputMID;
	

		
		fclose(outFile);
}

int CMidiParse::HexToInt(char inChar)
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

unsigned short CMidiParse::Flip16Bit(unsigned short ShortValue)
{
	return ((ShortValue >> 8) | ((ShortValue << 8)));
}

void CMidiParse::ExportToBin(unsigned char* buffer, unsigned long address, unsigned long size, CString fileName, bool& compressed)
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
			MessageBox(NULL, "Cannot Write File", "Error", NULL);
			return;
		}
		for (int x = 0; x < size; x++)
		{
			fwrite(&buffer[address+x], 1, 1, outFile);
		}
		fclose(outFile);
	}
}

void CMidiParse::ExportToMidi(CString gameName, unsigned char* gamebuffer, int gamebufferSize, unsigned long address, unsigned long size, CString fileName, CString gameType, int& numberInstruments, unsigned long division, bool& compressed, bool& hasLoopPoint, int& loopStart, int& loopEnd)
{
	if (gameType.Find("BanjoTooie") == 0)
	{
		if (compressed)
		{
			compress->SetGame(BANJOTOOIE);
			int decompressedSize = 0;	
			int compressedSize = -1;
			byte* outputBuffer = Decompress(&gamebuffer[address], size, decompressedSize, compressedSize);
			BTMidiToMidi(outputBuffer, decompressedSize, fileName, numberInstruments, hasLoopPoint, loopStart, loopEnd);
			delete [] outputBuffer;
		}
		else
		{
			BTMidiToMidi(&gamebuffer[address], size, fileName, numberInstruments, hasLoopPoint, loopStart, loopEnd);
		}
	}
	else if (gameType.Find("Glover") == 0)
	{
		GloverMidiToMidi(&gamebuffer[address], size, fileName, numberInstruments, division);
	}
	else if (gameType.Find("MIDx") == 0)
	{
		if (compressed)
		{
			
		}
		else
		{
			MIDxMidiToMidi(&gamebuffer[address], size, fileName, numberInstruments);
		}
	}
	else if (gameType.Find("MML") == 0)
	{
		if (compressed)
		{
			
		}
		else
		{
			MMLToMidi(&gamebuffer[address], size, fileName, numberInstruments);
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
				MessageBox(NULL, "Cannot Write File", "Error", NULL);
				return;
			}
			for (int x = 0; x < size; x++)
			{
				fwrite(&gamebuffer[address+x], 1, 1, outFile);
			}
			fclose(outFile);
		}
	}
	else if (gameType.Find("Seq64") == 0)
	{
		CString tempROMStr = mainFolder + "TempASEQ64ROM.rom";
		FILE* outTempROM = fopen(tempROMStr, "wb");
		if (outTempROM == NULL)
		{
			MessageBox(NULL, "Cannot Write temp ROM File", "Error", NULL);
			return;
		}

		fwrite(gamebuffer, 1, gamebufferSize, outTempROM);
		fclose(outTempROM);

		CString tempStr;
		tempStr.Format("seq64.exe -rom=\"%s\" -descriptor=\"%s\" -midi=%d -output=\"%s\"", tempROMStr, mainFolder + "romdesc\\" + gameName + ".xml", address, fileName);
		hiddenExec(_T(tempStr.GetBuffer()), (mainFolder));

		::DeleteFile(tempROMStr);
	}
	else if (gameType.Find("MultipartZLibXMFastTracker2") == 0)
	{
		if (compressed)
		{
			compress->SetGame(STUNTRACER64);

			FILE* outFile = fopen(fileName, "wb");
			if (outFile == NULL)
			{
				MessageBox(NULL, "Cannot Write File", "Error", NULL);
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
				MessageBox(NULL, "Cannot Write File", "Error", NULL);
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
				MessageBox(NULL, "Cannot Write File", "Error", NULL);
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
				MessageBox(NULL, "Cannot Write File", "Error", NULL);
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
				MessageBox(NULL, "Cannot Write File", "Error", NULL);
				return;
			}
			for (int x = 0; x < expectedSize; x++)
			{
				fwrite(&outputDecompressed[x], 1, 1, outFile);
			}
			fclose(outFile);

			GEMidiToMidi(outputDecompressed, expectedSize, fileName, numberInstruments, hasLoopPoint, loopStart, loopEnd);

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
			if (gameType.Find("GoldenEye") == 0)
			{
				compress->SetGame(GOLDENEYE);
			}
			else if (gameType.Find("PerfectDark") == 0)
			{
				compress->SetGame(PD);
			}
			else if (gameType.Find("BanjoKazooie") == 0)
			{
				compress->SetGame(BANJOKAZOOIE);
			}
			else if (gameType.Find("BanjoTooie") == 0)
			{
				compress->SetGame(BANJOTOOIE);
			}
			else if (gameType.Find("DonkeyKong") == 0)
			{
				compress->SetGame(DONKEYKONG64);
			}

			int decompressedSize = 0;
			int compressedSize = -1;
			byte* outputBuffer = Decompress(&gamebuffer[address], (size + 20000), decompressedSize, compressedSize);
			if (outputBuffer != NULL)
			{
				if ((outputBuffer[0] == 0x0) && (outputBuffer[1] == 0x0)
					&& (outputBuffer[2] == 0x0) && (outputBuffer[3] == 0x44))
				{
					GEMidiToMidi(outputBuffer, decompressedSize, fileName, numberInstruments, hasLoopPoint, loopStart, loopEnd);
				}
				delete [] outputBuffer;
			}
		}
		else
		{
			GEMidiToMidi(&gamebuffer[address], size, fileName, numberInstruments, hasLoopPoint, loopStart, loopEnd);
		}
	}
}

byte* CMidiParse::Decompress(unsigned char* Buffer, unsigned long size, int& fileSize, int& compressedSize)
{
	compress->SetCompressedBuffer(Buffer, size);
	
	fileSize = 0; // is by reference, overwritten
	byte* outputDecompressed = compress->OutputDecompressedBuffer(fileSize, compressedSize);

	if (outputDecompressed == NULL)
	{
		MessageBox(NULL, "Error Decompressing", "Error", NULL);
		return NULL;
	}

	return outputDecompressed;
}

bool CMidiParse::DecompressToFile(unsigned char* Buffer, unsigned long size, CString outputFile)
{
	int fileSize = 0; // is by reference, overwritten
	int compressedSize = -1;
	byte* outputDecompressed = Decompress(Buffer, size, fileSize, compressedSize);

	if (outputDecompressed == NULL)
	{
		MessageBox(NULL, "Error Decompressing", "Error", NULL);
		return false;
	}

	FILE* outFile = fopen(outputFile, "wb");
	if (outFile == NULL)
	{
		MessageBox(NULL, "Cannot Write File", "Error", NULL);
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

BOOL CMidiParse::hiddenExec (PTSTR pCmdLine, CString currentDirectory)
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
			MessageBox(NULL, "For some reason GZip Failed", "Error", NULL);
			TerminateProcess(processInfo.hProcess, exitCode);
			return false;
		}
   }   
   return true;
}