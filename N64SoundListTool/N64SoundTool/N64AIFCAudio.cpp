#pragma once

#include "StdAfx.h"
#include "N64AIFCAudio.h"
#include <math.h>

CN64AIFCAudio::CN64AIFCAudio(void)
{
}

CN64AIFCAudio::~CN64AIFCAudio(void)
{
}

CString CN64AIFCAudio::CompareALBanks(ALBank* alBank1, ALBank* alBank2)
{
	CString errors = "";
	if (alBank1->count != alBank2->count)
		errors += "Differing instr counts\r\n";
	if (alBank1->flags != alBank2->flags)
		errors += "Differing bank flags counts\r\n";
	if (alBank1->samplerate != alBank2->samplerate)
		errors += "Differing bank samplerate counts\r\n";
	for (int x = 0; x < alBank1->count; x++)
	{
		if (alBank1->inst[x]->volume != alBank2->inst[x]->volume)
			errors += "Differing instr volume counts\r\n";
		if (alBank1->inst[x]->pan != alBank2->inst[x]->pan)
			errors += "Differing instr pan counts\r\n";
		if (alBank1->inst[x]->priority != alBank2->inst[x]->priority)
			errors += "Differing instr priority counts\r\n";
		if (alBank1->inst[x]->flags != alBank2->inst[x]->flags)
			errors += "Differing instr flags counts\r\n";
		if (alBank1->inst[x]->tremType != alBank2->inst[x]->tremType)
			errors += "Differing instr tremType counts\r\n";
		if (alBank1->inst[x]->tremRate != alBank2->inst[x]->tremRate)
			errors += "Differing instr tremRate counts\r\n";
		if (alBank1->inst[x]->tremDepth != alBank2->inst[x]->tremDepth)
			errors += "Differing instr tremDepth counts\r\n";
		if (alBank1->inst[x]->tremDelay != alBank2->inst[x]->tremDelay)
			errors += "Differing instr tremDelay counts\r\n";
		if (alBank1->inst[x]->vibType != alBank2->inst[x]->vibType)
			errors += "Differing instr vibType counts\r\n";
		if (alBank1->inst[x]->vibRate != alBank2->inst[x]->vibRate)
			errors += "Differing instr vibRate counts\r\n";
		if (alBank1->inst[x]->vibDelay != alBank2->inst[x]->vibDelay)
			errors += "Differing instr vibDelay counts\r\n";
		if (alBank1->inst[x]->bendRange != alBank2->inst[x]->bendRange)
			errors += "Differing instr bendRange counts\r\n";
		if (alBank1->inst[x]->soundCount != alBank2->inst[x]->soundCount)
		{
			errors += "Differing instr soundCount counts\r\n";
			continue;
		}

		for (int y = 0; y < alBank1->inst[x]->soundCount; y++)
		{
			if (alBank1->inst[x]->sounds[y]->samplePan != alBank2->inst[x]->sounds[y]->samplePan)
				errors += "Differing instr samplePan counts\r\n";
			if (alBank1->inst[x]->sounds[y]->sampleVolume != alBank2->inst[x]->sounds[y]->sampleVolume)
				errors += "Differing instr sampleVolume counts\r\n";
			if (alBank1->inst[x]->sounds[y]->flags != alBank2->inst[x]->sounds[y]->flags)
				errors += "Differing instr flags counts\r\n";

			if (alBank1->inst[x]->sounds[y]->env.attackTime != alBank2->inst[x]->sounds[y]->env.attackTime)
				errors += "Differing sound attackTime counts\r\n";
			if (alBank1->inst[x]->sounds[y]->env.decayTime != alBank2->inst[x]->sounds[y]->env.decayTime)
				errors += "Differing sound decayTime counts\r\n";
			if (alBank1->inst[x]->sounds[y]->env.releaseTime != alBank2->inst[x]->sounds[y]->env.releaseTime)
				errors += "Differing sound releaseTime counts\r\n";
			if (alBank1->inst[x]->sounds[y]->env.attackVolume != alBank2->inst[x]->sounds[y]->env.attackVolume)
				errors += "Differing sound attackVolume counts\r\n";
			if (alBank1->inst[x]->sounds[y]->env.decayVolume != alBank2->inst[x]->sounds[y]->env.decayVolume)
				errors += "Differing sound decayVolume counts\r\n";

			if (alBank1->inst[x]->sounds[y]->key.velocitymin != alBank2->inst[x]->sounds[y]->key.velocitymin)
				errors += "Differing sound velocitymin counts\r\n";
			if (alBank1->inst[x]->sounds[y]->key.velocitymax != alBank2->inst[x]->sounds[y]->key.velocitymax)
				errors += "Differing sound velocitymax counts\r\n";
			if (alBank1->inst[x]->sounds[y]->key.keymin != alBank2->inst[x]->sounds[y]->key.keymin)
				errors += "Differing sound keymin counts\r\n";
			if (alBank1->inst[x]->sounds[y]->key.keymax != alBank2->inst[x]->sounds[y]->key.keymax)
				errors += "Differing sound keymax counts\r\n";
			if (alBank1->inst[x]->sounds[y]->key.keybase != alBank2->inst[x]->sounds[y]->key.keybase)
				errors += "Differing sound keybase counts\r\n";
			if (alBank1->inst[x]->sounds[y]->key.detune != alBank2->inst[x]->sounds[y]->key.detune)
				errors += "Differing sound detune counts\r\n";

			if (alBank1->inst[x]->sounds[y]->wav.len != alBank2->inst[x]->sounds[y]->wav.len)
				errors += "Differing sound len\r\n";
			if (alBank1->inst[x]->sounds[y]->wav.type != alBank2->inst[x]->sounds[y]->wav.type)
				errors += "Differing sound len\r\n";
			if (alBank1->inst[x]->sounds[y]->wav.flags != alBank2->inst[x]->sounds[y]->wav.flags)
				errors += "Differing sound flags\r\n";

			if ((alBank1->inst[x]->sounds[y]->wav.type == 0) && (alBank2->inst[x]->sounds[y]->wav.type == 0))
			{
				if ((alBank1->inst[x]->sounds[y]->wav.adpcmWave->loop != NULL) && (alBank2->inst[x]->sounds[y]->wav.adpcmWave->loop != NULL))
				{
					if (alBank1->inst[x]->sounds[y]->wav.adpcmWave->loop->start != alBank2->inst[x]->sounds[y]->wav.adpcmWave->loop->start)
						errors += "Differing loop start flags\r\n";
					if (alBank1->inst[x]->sounds[y]->wav.adpcmWave->loop->end != alBank2->inst[x]->sounds[y]->wav.adpcmWave->loop->end)
						errors += "Differing loop end flags\r\n";
					if (alBank1->inst[x]->sounds[y]->wav.adpcmWave->loop->count != alBank2->inst[x]->sounds[y]->wav.adpcmWave->loop->count)
						errors += "Differing loop count flags\r\n";

					for (int z = 0; z < 0x10; z++)
					{
						if (alBank1->inst[x]->sounds[y]->wav.adpcmWave->loop->state[z] != alBank2->inst[x]->sounds[y]->wav.adpcmWave->loop->state[z])
							errors += "Differing loop state flags\r\n";
					}
				}
				else if (((alBank1->inst[x]->sounds[y]->wav.adpcmWave->loop != NULL) && (alBank2->inst[x]->sounds[y]->wav.adpcmWave->loop == NULL)) || 
					((alBank1->inst[x]->sounds[y]->wav.adpcmWave->loop == NULL) && (alBank2->inst[x]->sounds[y]->wav.adpcmWave->loop != NULL)))
				{
					errors += "Differing loop null\r\n";
				}

				if (alBank1->inst[x]->sounds[y]->wav.adpcmWave->book->order != alBank2->inst[x]->sounds[y]->wav.adpcmWave->book->order)
				{
						errors += "Differing book->order\r\n";
						continue;
				}
				if (alBank1->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors != alBank2->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors)
				{
						errors += "Differing book->npredictors\r\n";
						continue;
				}

				for (int z = 0; z < (alBank1->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank1->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8); z++)
				{
					if (alBank1->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[z] != alBank2->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[z])
						errors += "Differing predictors \r\n";
				}
			}
			else if ((alBank1->inst[x]->sounds[y]->wav.type == 1) && (alBank2->inst[x]->sounds[y]->wav.type == 1))
			{
				if ((alBank1->inst[x]->sounds[y]->wav.rawWave->loop != NULL) && (alBank2->inst[x]->sounds[y]->wav.rawWave->loop != NULL))
				{
					if (alBank1->inst[x]->sounds[y]->wav.rawWave->loop->start != alBank2->inst[x]->sounds[y]->wav.rawWave->loop->start)
						errors += "Differing loop start flags\r\n";
					if (alBank1->inst[x]->sounds[y]->wav.rawWave->loop->end != alBank2->inst[x]->sounds[y]->wav.rawWave->loop->end)
						errors += "Differing loop end flags\r\n";
					if (alBank1->inst[x]->sounds[y]->wav.rawWave->loop->count != alBank2->inst[x]->sounds[y]->wav.rawWave->loop->count)
						errors += "Differing loop count flags\r\n";
				}
				else if (((alBank1->inst[x]->sounds[y]->wav.rawWave->loop != NULL) && (alBank2->inst[x]->sounds[y]->wav.rawWave->loop == NULL)) || 
					((alBank1->inst[x]->sounds[y]->wav.rawWave->loop == NULL) && (alBank2->inst[x]->sounds[y]->wav.rawWave->loop != NULL)))
				{
					errors += "Differing loop null flags\r\n";
				}
			}

			if (alBank1->inst[x]->sounds[y]->wav.len == alBank2->inst[x]->sounds[y]->wav.len)
			{
				for (int z = 0; z < alBank1->inst[x]->sounds[y]->wav.len; z++)
				{
					if (alBank1->inst[x]->sounds[y]->wav.wavData[z] != alBank2->inst[x]->sounds[y]->wav.wavData[z])
					{
						errors += "tbl mismatch\r\n";
						break;
					}
				}
			}
		}
	}

	return errors;
}

bool CN64AIFCAudio::ExtractRawSound(ALBank* alBank, int instrument, int sound, CString outputFile, unsigned long samplingRate)
{
	if (alBank->inst[instrument]->sounds[sound]->wav.type == AL_RAW16_WAVE)
	{
		FILE* outFileTempRaw = fopen(outputFile, "wb");
		if (outFileTempRaw == NULL)
		{
			MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
			return false;
		}

		unsigned char* wavHeader = new unsigned char[0x28];


		wavHeader[0x0] = 0x52;
		wavHeader[0x1] = 0x49;
		wavHeader[0x2] = 0x46;
		wavHeader[0x3] = 0x46;
		wavHeader[0x4] = 0x14;
		wavHeader[0x5] = 0x7E;
		wavHeader[0x6] = 0x00;
		wavHeader[0x7] = 0x00;
		wavHeader[0x8] = 0x57;
		wavHeader[0x9] = 0x41;
		wavHeader[0xA] = 0x56;
		wavHeader[0xB] = 0x45;
		wavHeader[0xC] = 0x66;
		wavHeader[0xD] = 0x6D;
		wavHeader[0xE] = 0x74;
		wavHeader[0xF] = 0x20;
		wavHeader[0x10] = 0x10;
		wavHeader[0x11] = 0x00;
		wavHeader[0x12] = 0x00;
		wavHeader[0x13] = 0x00;
		wavHeader[0x14] = 0x01;
		wavHeader[0x15] = 0x00;
		wavHeader[0x16] = 0x01;
		wavHeader[0x17] = 0x00;
		wavHeader[0x18] = ((samplingRate >> 0) & 0xFF);
		wavHeader[0x19] = ((samplingRate >> 8) & 0xFF);
		wavHeader[0x1A] = ((samplingRate >> 16) & 0xFF);
		wavHeader[0x1B] = ((samplingRate >> 24) & 0xFF);
		wavHeader[0x1C] = (((samplingRate * 2) >> 0) & 0xFF);
		wavHeader[0x1D] = (((samplingRate * 2) >> 8) & 0xFF);
		wavHeader[0x1E] = (((samplingRate * 2) >> 16) & 0xFF);
		wavHeader[0x1F] = (((samplingRate * 2) >> 24) & 0xFF);
		wavHeader[0x20] = 0x02;
		wavHeader[0x21] = 0x00;
		wavHeader[0x22] = 0x10;
		wavHeader[0x23] = 0x00;
		wavHeader[0x24] = 0x64;
		wavHeader[0x25] = 0x61;
		wavHeader[0x26] = 0x74;
		wavHeader[0x27] = 0x61;

		fwrite(wavHeader, 1, 0x28, outFileTempRaw );

		delete [] wavHeader;

		unsigned long length = (alBank->inst[instrument]->sounds[sound]->wav.len-2);
		fwrite(&length, 1, 4, outFileTempRaw);

		fwrite(&alBank->inst[instrument]->sounds[sound]->wav.wavData[1], 1, (alBank->inst[instrument]->sounds[sound]->wav.len-2), outFileTempRaw);

		fclose(outFileTempRaw);

		return true;
	}
	else
	{
		signed short* outRawData = new signed short[alBank->inst[instrument]->sounds[sound]->wav.len * 4];
		int nSamples = decode(alBank->inst[instrument]->sounds[sound]->wav.wavData, outRawData, alBank->inst[instrument]->sounds[sound]->wav.len, alBank->inst[instrument]->sounds[sound]->wav.adpcmWave->book);
		
		FILE* outFileTempRaw = fopen(outputFile, "wb");
		if (outFileTempRaw == NULL)
		{
			MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
			return false;
		}

		unsigned char* wavHeader = new unsigned char[0x28];


		wavHeader[0x0] = 0x52;
		wavHeader[0x1] = 0x49;
		wavHeader[0x2] = 0x46;
		wavHeader[0x3] = 0x46;
		wavHeader[0x4] = 0x14;
		wavHeader[0x5] = 0x7E;
		wavHeader[0x6] = 0x00;
		wavHeader[0x7] = 0x00;
		wavHeader[0x8] = 0x57;
		wavHeader[0x9] = 0x41;
		wavHeader[0xA] = 0x56;
		wavHeader[0xB] = 0x45;
		wavHeader[0xC] = 0x66;
		wavHeader[0xD] = 0x6D;
		wavHeader[0xE] = 0x74;
		wavHeader[0xF] = 0x20;
		wavHeader[0x10] = 0x10;
		wavHeader[0x11] = 0x00;
		wavHeader[0x12] = 0x00;
		wavHeader[0x13] = 0x00;
		wavHeader[0x14] = 0x01;
		wavHeader[0x15] = 0x00;
		wavHeader[0x16] = 0x01;
		wavHeader[0x17] = 0x00;
		wavHeader[0x18] = ((samplingRate >> 0) & 0xFF);
		wavHeader[0x19] = ((samplingRate >> 8) & 0xFF);
		wavHeader[0x1A] = ((samplingRate >> 16) & 0xFF);
		wavHeader[0x1B] = ((samplingRate >> 24) & 0xFF);
		wavHeader[0x1C] = (((samplingRate * 2) >> 0) & 0xFF);
		wavHeader[0x1D] = (((samplingRate * 2) >> 8) & 0xFF);
		wavHeader[0x1E] = (((samplingRate * 2) >> 16) & 0xFF);
		wavHeader[0x1F] = (((samplingRate * 2) >> 24) & 0xFF);
		wavHeader[0x20] = 0x02;
		wavHeader[0x21] = 0x00;
		wavHeader[0x22] = 0x10;
		wavHeader[0x23] = 0x00;
		wavHeader[0x24] = 0x64;
		wavHeader[0x25] = 0x61;
		wavHeader[0x26] = 0x74;
		wavHeader[0x27] = 0x61;

		fwrite(wavHeader, 1, 0x28, outFileTempRaw );

		delete [] wavHeader;

		unsigned long length = (nSamples * 2);
		fwrite(&length, 1, 4, outFileTempRaw);

		for (int x = 0; x < nSamples; x++)
		{
			fwrite(&outRawData[x], 1,2, outFileTempRaw);
		}

		fclose(outFileTempRaw);

		delete [] outRawData;
		return true;
	}
}

bool CN64AIFCAudio::AddSound(ALBank*& alBank, int instrument, CString rawWavFileName, unsigned long& samplingRate, bool type)
{
	AddSound(alBank, instrument);

	return ReplaceSoundWithWavData(alBank, instrument, (alBank->inst[instrument]->soundCount-1), rawWavFileName, samplingRate, type);
}
bool CN64AIFCAudio::AddSound(ALBank*& alBank, int instrument)
{
	ALSound** newSounds = new ALSound*[alBank->inst[instrument]->soundCount + 1];
	for (int x = 0; x < alBank->inst[instrument]->soundCount; x++)
	{
		newSounds[x] = alBank->inst[instrument]->sounds[x];
	}

	newSounds[alBank->inst[instrument]->soundCount] = new ALSound();
	newSounds[alBank->inst[instrument]->soundCount]->env.attackTime = 0;
	newSounds[alBank->inst[instrument]->soundCount]->env.attackVolume = 0x7F7F;
	newSounds[alBank->inst[instrument]->soundCount]->env.decayTime = 0xFFFFFFFF;
	newSounds[alBank->inst[instrument]->soundCount]->env.decayVolume = 0;
	newSounds[alBank->inst[instrument]->soundCount]->env.releaseTime = 0;
	newSounds[alBank->inst[instrument]->soundCount]->flags = 0;
	newSounds[alBank->inst[instrument]->soundCount]->key.detune = 0;
	newSounds[alBank->inst[instrument]->soundCount]->key.keybase = 0x3C;
	newSounds[alBank->inst[instrument]->soundCount]->key.keymax = 0x7F;
	newSounds[alBank->inst[instrument]->soundCount]->key.keymin = 0;
	newSounds[alBank->inst[instrument]->soundCount]->key.velocitymax = 0x7F;
	newSounds[alBank->inst[instrument]->soundCount]->key.velocitymin = 0;
	newSounds[alBank->inst[instrument]->soundCount]->samplePan = 0x40;
	newSounds[alBank->inst[instrument]->soundCount]->sampleVolume = 0x7F;
	newSounds[alBank->inst[instrument]->soundCount]->wav.base = 0;
	newSounds[alBank->inst[instrument]->soundCount]->wav.len = 4;
	newSounds[alBank->inst[instrument]->soundCount]->wav.type = AL_RAW16_WAVE;
	newSounds[alBank->inst[instrument]->soundCount]->wav.flags = 0;
	newSounds[alBank->inst[instrument]->soundCount]->wav.wavData = new unsigned char[4];
	newSounds[alBank->inst[instrument]->soundCount]->wav.wavData[0] = 0x3;
	newSounds[alBank->inst[instrument]->soundCount]->wav.wavData[1] = 0x0;
	newSounds[alBank->inst[instrument]->soundCount]->wav.wavData[2] = 0x0;
	newSounds[alBank->inst[instrument]->soundCount]->wav.wavData[3] = 0x0;
	newSounds[alBank->inst[instrument]->soundCount]->wav.rawWave = new ALRAWWaveInfo();
	newSounds[alBank->inst[instrument]->soundCount]->wav.rawWave->loop = NULL;

	delete [] alBank->inst[instrument]->sounds;
	alBank->inst[instrument]->sounds = newSounds;

	alBank->inst[instrument]->soundCount++;

	return true;
}
void CN64AIFCAudio::DeleteSound(ALBank*& alBank, int instrument, int sound)
{
	ALSound** newSounds = new ALSound*[alBank->inst[instrument]->soundCount - 1];
	for (int x = 0; x < sound; x++)
	{
		newSounds[x] = alBank->inst[instrument]->sounds[x];
	}

	for (int x = (sound+1); x < alBank->inst[instrument]->soundCount; x++)
	{
		newSounds[x-1] = alBank->inst[instrument]->sounds[x];
	}

	delete [] alBank->inst[instrument]->sounds;
	alBank->inst[instrument]->sounds = newSounds;

	alBank->inst[instrument]->soundCount--;
}
void CN64AIFCAudio::MoveUpSound(ALBank*& alBank, int instrument, int sound)
{
	if (sound == 0)
		return;

	ALSound* soundAl = alBank->inst[instrument]->sounds[sound-1];
	alBank->inst[instrument]->sounds[sound-1] = alBank->inst[instrument]->sounds[sound];
	alBank->inst[instrument]->sounds[sound] = soundAl;
}

void CN64AIFCAudio::ExportPredictors(ALBank*& alBank, int instrument, int sound, CString fileName)
{
	FILE* outFile = fopen(fileName, "wb");
	if (outFile == NULL)
	{
		MessageBox(NULL, "Error opening file", "Error", NULL);
		return;
	}
	unsigned long tempLong = Flip32Bit(alBank->inst[instrument]->sounds[sound]->wav.adpcmWave->book->order);
	fwrite(&tempLong, 1, 4, outFile);

	tempLong = Flip32Bit(alBank->inst[instrument]->sounds[sound]->wav.adpcmWave->book->npredictors);
	fwrite(&tempLong, 1, 4, outFile);

	for (int z = 0; z < (alBank->inst[instrument]->sounds[sound]->wav.adpcmWave->book->order * alBank->inst[instrument]->sounds[sound]->wav.adpcmWave->book->npredictors * 8); z++)
	{
		unsigned short tempShort = Flip16Bit(alBank->inst[instrument]->sounds[sound]->wav.adpcmWave->book->predictors[z]);
		fwrite(&tempShort, 1, 2, outFile);
	}

	fclose(outFile);
}

void CN64AIFCAudio::ImportPredictors(ALBank*& alBank, int instrument, int sound, CString fileName)
{
	int fileSize = GetSizeFile(fileName);
	FILE* inFile = fopen(fileName, "rb");
	if (inFile == NULL)
	{
		MessageBox(NULL, "Error opening", "Error", NULL);
		return;
	}

	if (fileSize < 0x8)
	{
		MessageBox(NULL, "Error too small file", "Error", NULL);
		return;
	}


	unsigned char* buffer = new unsigned char[fileSize];
	fread(buffer, 1, fileSize, inFile);
	fclose(inFile);
	

	alBank->inst[instrument]->sounds[sound]->wav.adpcmWave->book->order = CharArrayToLong(&buffer[0]);
	alBank->inst[instrument]->sounds[sound]->wav.adpcmWave->book->npredictors = CharArrayToLong(&buffer[4]);

	if (fileSize != (8 + ((alBank->inst[instrument]->sounds[sound]->wav.adpcmWave->book->order * alBank->inst[instrument]->sounds[sound]->wav.adpcmWave->book->npredictors * 8) * 2)))
	{
		delete [] buffer;
		MessageBox(NULL, "Error invalid file size", "Error", NULL);
		return;
	}

	delete [] alBank->inst[instrument]->sounds[sound]->wav.adpcmWave->book->predictors;
	alBank->inst[instrument]->sounds[sound]->wav.adpcmWave->book->predictors = new signed short[(alBank->inst[instrument]->sounds[sound]->wav.adpcmWave->book->order * alBank->inst[instrument]->sounds[sound]->wav.adpcmWave->book->npredictors * 8)];
	
	

	for (int z = 0; z < (alBank->inst[instrument]->sounds[sound]->wav.adpcmWave->book->order * alBank->inst[instrument]->sounds[sound]->wav.adpcmWave->book->npredictors * 8); z++)
	{
		alBank->inst[instrument]->sounds[sound]->wav.adpcmWave->book->predictors[z] = CharArrayToShort(&buffer[0x8 + (z * 2)]);
	}

	delete [] buffer;

	unsigned char* ctl;
	unsigned char* tbl;
	int ctlCounter, tblCounter;
	// update base offsets
	WriteAudio(alBank, ctl, ctlCounter, tbl, tblCounter);
	delete [] ctl;
	delete [] tbl;
}

void CN64AIFCAudio::ExportRawData(ALBank*& alBank, int instrument, int sound, CString fileName)
{
	FILE* outFile = fopen(fileName, "wb");
	if (outFile == NULL)
	{
		MessageBox(NULL, "Error opening file", "Error", NULL);
		return;
	}

	fwrite(alBank->inst[instrument]->sounds[sound]->wav.wavData, 1, alBank->inst[instrument]->sounds[sound]->wav.len, outFile);

	fclose(outFile);
}

void CN64AIFCAudio::ImportRawData(ALBank*& alBank, int instrument, int sound, CString fileName)
{
	int fileSize = GetSizeFile(fileName);
	FILE* inFile = fopen(fileName, "rb");
	if (inFile == NULL)
	{
		MessageBox(NULL, "Error opening", "Error", NULL);
		return;
	}


	alBank->inst[instrument]->sounds[sound]->wav.len = fileSize;
	delete [] alBank->inst[instrument]->sounds[sound]->wav.wavData;

	alBank->inst[instrument]->sounds[sound]->wav.wavData = new unsigned char[fileSize];
	fread(alBank->inst[instrument]->sounds[sound]->wav.wavData, 1, fileSize, inFile);
	fclose(inFile);

	unsigned char* ctl;
	unsigned char* tbl;
	int ctlCounter, tblCounter;
	// update base offsets
	WriteAudio(alBank, ctl, ctlCounter, tbl, tblCounter);
	delete [] ctl;
	delete [] tbl;
}

void CN64AIFCAudio::MoveDownSound(ALBank*& alBank, int instrument, int sound)
{
	if (sound == (alBank->inst[instrument]->soundCount-1))
		return;

	ALSound* soundAl = alBank->inst[instrument]->sounds[sound+1];
	alBank->inst[instrument]->sounds[sound+1] = alBank->inst[instrument]->sounds[sound];
	alBank->inst[instrument]->sounds[sound] = soundAl;
}

bool CN64AIFCAudio::ReadWavData(CString rawWavFileName, unsigned char*& rawData, int& rawLength, unsigned long& samplingRate)
{
	FILE* inWavFile = fopen(rawWavFileName, "rb");
	if (inWavFile == NULL)
	{
		MessageBox(NULL, "Error cannot read wav file", "Error", NULL);
		return false;
	}

	fseek(inWavFile, 0, SEEK_END);
	int fileSize = ftell(inWavFile);
	rewind(inWavFile);

	unsigned char* wavData = new unsigned char[fileSize];
	fread(wavData, 1, fileSize, inWavFile);
	fclose(inWavFile);

	if (((((((wavData[0] << 8) | wavData[1]) << 8) | wavData[2]) << 8) | wavData[3]) != 0x52494646)
	{
		MessageBox(NULL, "Error not RIFF wav", "Error", NULL);
		return false;
	}

	if (((((((wavData[0x8] << 8) | wavData[0x9]) << 8) | wavData[0xA]) << 8) | wavData[0xB]) != 0x57415645)
	{
		MessageBox(NULL, "Error not WAVE wav", "Error", NULL);
		return false;
	}

	bool endFlag = false;

	unsigned long currentOffset = 0xC;

	unsigned short channels = 0;
	samplingRate = 0;
	unsigned short bitRate = 0;

	while (!endFlag)
	{
		unsigned long sectionType = ((((((wavData[currentOffset] << 8) | wavData[currentOffset + 1]) << 8) | wavData[currentOffset + 2]) << 8) | wavData[currentOffset + 3]);

		if (sectionType == 0x666D7420) // fmt
		{
			unsigned long chunkSize = ((((((wavData[currentOffset + 0x7] << 8) | wavData[currentOffset + 0x6]) << 8) | wavData[currentOffset + 0x5]) << 8) | wavData[currentOffset + 0x4]);

			channels = ((wavData[currentOffset + 0xB] << 8) | wavData[currentOffset + 0xA]);

			if (channels != 0x0001)
			{
				MessageBox(NULL, "Warning: Only mono wav supported", "Error", NULL);
				return false;
			}

			samplingRate = ((((((wavData[currentOffset + 0xF] << 8) | wavData[currentOffset + 0xE]) << 8) | wavData[currentOffset + 0xD]) << 8) | wavData[currentOffset + 0xC]);
			
			bitRate = ((wavData[currentOffset + 0x17] << 8) | wavData[currentOffset + 0x16]);

			currentOffset += chunkSize + 8;
		}
		else if (sectionType == 0x64617461) // data
		{
			rawLength = ((((((wavData[currentOffset + 0x7] << 8) | wavData[currentOffset + 0x6]) << 8) | wavData[currentOffset + 0x5]) << 8) | wavData[currentOffset + 0x4]);

			if ((channels == 0) || (samplingRate == 0) || (bitRate == 0))
			{
				MessageBox(NULL, "Incorrect section type (missing fmt)", "Error unknown wav format", NULL);
				delete [] wavData;
				return false;
			}

			if (bitRate == 0x0010)
			{
				rawData = new unsigned char[rawLength];
				for (int x = 0; x < rawLength; x++)
				{
					rawData[x] = wavData[currentOffset + 0x8 + x];
				}
			
				delete [] wavData;
				return true;
			}
			else
			{
				delete [] wavData;
				MessageBox(NULL, "Error only 16-bit PCM wav supported", "Error", NULL);
				return false;
			}

			currentOffset += rawLength + 4;
			endFlag = true;
		}
		else
		{
			unsigned long chunkSize = ((((((wavData[currentOffset + 0x7] << 8) | wavData[currentOffset + 0x6]) << 8) | wavData[currentOffset + 0x5]) << 8) | wavData[currentOffset + 0x4]);

			currentOffset += 8 + chunkSize;
		}
	}

	delete [] wavData;
}

bool CN64AIFCAudio::ReplaceSoundWithWavData(ALBank*& alBank, int instrument, int sound, CString rawWavFileName, unsigned long& samplingRate, bool newType)
{
	unsigned char* wavPCMData;
	int rawLength;

	if (!ReadWavData(rawWavFileName, wavPCMData, rawLength, samplingRate))
	{
		return false;
	}

	if (alBank->inst[instrument]->sounds[sound]->wav.type == AL_ADPCM_WAVE)
	{
		if (alBank->inst[instrument]->sounds[sound]->wav.adpcmWave->loop != NULL)
		{
			delete alBank->inst[instrument]->sounds[sound]->wav.adpcmWave->loop;
			alBank->inst[instrument]->sounds[sound]->wav.adpcmWave->loop = NULL;
		}

		if (alBank->inst[instrument]->sounds[sound]->wav.adpcmWave->book != NULL)
		{
			delete alBank->inst[instrument]->sounds[sound]->wav.adpcmWave->book;
			alBank->inst[instrument]->sounds[sound]->wav.adpcmWave->book = NULL;
		}

		delete alBank->inst[instrument]->sounds[sound]->wav.adpcmWave->book;
		delete alBank->inst[instrument]->sounds[sound]->wav.adpcmWave;
		alBank->inst[instrument]->sounds[sound]->wav.adpcmWave = NULL;
		alBank->inst[instrument]->sounds[sound]->wav.rawWave = NULL;
	}
	else
	{
		if (alBank->inst[instrument]->sounds[sound]->wav.rawWave->loop != NULL)
		{
			delete alBank->inst[instrument]->sounds[sound]->wav.rawWave->loop;
			alBank->inst[instrument]->sounds[sound]->wav.rawWave->loop = NULL;
		}
		delete alBank->inst[instrument]->sounds[sound]->wav.rawWave;
		alBank->inst[instrument]->sounds[sound]->wav.rawWave = NULL;
		alBank->inst[instrument]->sounds[sound]->wav.adpcmWave = NULL;
	}

	if (newType == AL_RAW16_WAVE)
	{
		alBank->inst[instrument]->sounds[sound]->wav.type = AL_RAW16_WAVE;
		alBank->inst[instrument]->sounds[sound]->wav.rawWave = new ALRAWWaveInfo();
		alBank->inst[instrument]->sounds[sound]->wav.rawWave->loop = NULL;/*new ALRawLoop();
		alBank->inst[instrument]->sounds[sound]->wav.rawWave->loop->start = 0;
		alBank->inst[instrument]->sounds[sound]->wav.rawWave->loop->end = (rawLength-2);
		alBank->inst[instrument]->sounds[sound]->wav.rawWave->loop->count = 0;*/

		delete [] alBank->inst[instrument]->sounds[sound]->wav.wavData;
		alBank->inst[instrument]->sounds[sound]->wav.wavData = new unsigned char[rawLength + 2];
		alBank->inst[instrument]->sounds[sound]->wav.wavData[0] = 0x03;
		alBank->inst[instrument]->sounds[sound]->wav.wavData[rawLength + 1] = 0x00;
		memcpy(&alBank->inst[instrument]->sounds[sound]->wav.wavData[1], wavPCMData, rawLength);
		alBank->inst[instrument]->sounds[sound]->wav.len = rawLength + 2;

		alBank->inst[instrument]->sounds[sound]->env.attackTime = 0;
		alBank->inst[instrument]->sounds[sound]->env.attackVolume = 0x7F7F;
		alBank->inst[instrument]->sounds[sound]->env.decayTime = 0xFFFFFFFF;
		alBank->inst[instrument]->sounds[sound]->env.decayVolume = 0;
		alBank->inst[instrument]->sounds[sound]->env.releaseTime = 0;
		alBank->inst[instrument]->sounds[sound]->flags = 0;
		alBank->inst[instrument]->sounds[sound]->key.detune = 0;
		alBank->inst[instrument]->sounds[sound]->key.keybase = 0x3C;
		alBank->inst[instrument]->sounds[sound]->key.keymax = 0x7F;
		alBank->inst[instrument]->sounds[sound]->key.keymin = 0;
		alBank->inst[instrument]->sounds[sound]->key.velocitymax = 0x7F;
		alBank->inst[instrument]->sounds[sound]->key.velocitymin = 0;
		alBank->inst[instrument]->sounds[sound]->samplePan = 0x40;
		alBank->inst[instrument]->sounds[sound]->sampleVolume = 0x7F;
		alBank->inst[instrument]->sounds[sound]->wav.flags = 0;
	}
	else
	{
		alBank->inst[instrument]->sounds[sound]->wav.type = AL_ADPCM_WAVE;
		alBank->inst[instrument]->sounds[sound]->wav.adpcmWave = new ALADPCMWaveInfo();
		alBank->inst[instrument]->sounds[sound]->wav.adpcmWave->loop = NULL;/*new ALRawLoop();
		alBank->inst[instrument]->sounds[sound]->wav.rawWave->loop->start = 0;
		alBank->inst[instrument]->sounds[sound]->wav.rawWave->loop->end = (rawLength-2);
		alBank->inst[instrument]->sounds[sound]->wav.rawWave->loop->count = 0;*/

		alBank->inst[instrument]->sounds[sound]->wav.adpcmWave->book = new ALADPCMBook();
		alBank->inst[instrument]->sounds[sound]->wav.adpcmWave->book->npredictors = 1;
		alBank->inst[instrument]->sounds[sound]->wav.adpcmWave->book->order = 2;

		int numberSamples = (rawLength / 2);
		signed short* pcmSamples = new signed short[numberSamples];

		for (int x = 0; x < numberSamples; x++)
		{
			pcmSamples[x] = (signed short)(((wavPCMData[x*2+1] << 8)) | wavPCMData[x*2]);
		}

		alBank->inst[instrument]->sounds[sound]->wav.adpcmWave->book->predictors = determineBestPredictors(pcmSamples, numberSamples);

		delete [] alBank->inst[instrument]->sounds[sound]->wav.wavData;

		unsigned long vadpcmOutputLength;
		unsigned char* vadpcmData = new unsigned char[numberSamples];

		encode(pcmSamples, numberSamples, vadpcmData, vadpcmOutputLength, alBank->inst[instrument]->sounds[sound]->wav.adpcmWave->book);

		alBank->inst[instrument]->sounds[sound]->wav.wavData = new unsigned char[vadpcmOutputLength];
		memcpy(alBank->inst[instrument]->sounds[sound]->wav.wavData, vadpcmData, vadpcmOutputLength);
		alBank->inst[instrument]->sounds[sound]->wav.len = vadpcmOutputLength;

		alBank->inst[instrument]->sounds[sound]->env.attackTime = 0;
		alBank->inst[instrument]->sounds[sound]->env.attackVolume = 0x7F7F;
		alBank->inst[instrument]->sounds[sound]->env.decayTime = 0xFFFFFFFF;
		alBank->inst[instrument]->sounds[sound]->env.decayVolume = 0;
		alBank->inst[instrument]->sounds[sound]->env.releaseTime = 0;
		alBank->inst[instrument]->sounds[sound]->flags = 0;
		alBank->inst[instrument]->sounds[sound]->key.detune = 0;
		alBank->inst[instrument]->sounds[sound]->key.keybase = 0x3C;
		alBank->inst[instrument]->sounds[sound]->key.keymax = 0x7F;
		alBank->inst[instrument]->sounds[sound]->key.keymin = 0;
		alBank->inst[instrument]->sounds[sound]->key.velocitymax = 0x7F;
		alBank->inst[instrument]->sounds[sound]->key.velocitymin = 0;
		alBank->inst[instrument]->sounds[sound]->samplePan = 0x40;
		alBank->inst[instrument]->sounds[sound]->sampleVolume = 0x7F;
		alBank->inst[instrument]->sounds[sound]->wav.flags = 0;

		delete [] pcmSamples;
		delete [] vadpcmData;
	}

	delete [] wavPCMData;

	unsigned char* ctl;
	unsigned char* tbl;
	int ctlCounter, tblCounter;
	// update base offsets
	WriteAudio(alBank, ctl, ctlCounter, tbl, tblCounter);
	delete [] ctl;
	delete [] tbl;

	return true;
}

void CN64AIFCAudio::WriteAudioToFile(ALBank*& alBank, CString outFileNameCtl, CString outFileNameTbl)
{
	unsigned char* ctl;
	unsigned char *tbl;
	int ctlSize, tblSize;
	WriteAudio(alBank, ctl, ctlSize, tbl, tblSize);

	FILE* outFileCtl = fopen(outFileNameCtl, "wb");
	if (outFileCtl == NULL)
	{
		MessageBox(NULL, "Cannot open ctl file", "Error", NULL);
		return;
	}

	FILE* outFileTbl = fopen(outFileNameTbl, "wb");
	if (outFileTbl == NULL)
	{
		MessageBox(NULL, "Cannot open tbl file", "Error", NULL);
		return;
	}

	fwrite(ctl, 1, ctlSize, outFileCtl);
	fwrite(tbl, 1, tblSize, outFileTbl);

	fclose(outFileCtl);
	fclose(outFileTbl);
}

void CN64AIFCAudio::WriteAudio(ALBank*& alBank, unsigned char*& ctl, int& ctlSize, unsigned char*& tbl, int& tblSize)
{
	unsigned char* temporaryCtlBuffer = new unsigned char[0x1000000];
	unsigned char* temporaryTblBuffer = new unsigned char[0x1000000];
	unsigned long outputCtlCounter = 0;
	
	// magic
	WriteShortToBuffer(temporaryCtlBuffer, outputCtlCounter, 0x4231);
	outputCtlCounter += 2;

	// bank #
	int bankCount = 1;
	WriteShortToBuffer(temporaryCtlBuffer, outputCtlCounter, bankCount);
	outputCtlCounter += 2;

	for (int x = 0; x < bankCount; x++)
	{
		// offset to bank later in file
		outputCtlCounter += 4;
	}

	unsigned long outputTblCounter = 0;

	unsigned long* instrumentALLookup = new unsigned long[alBank->count];
	unsigned long** instrumentSoundStartLookup = new unsigned long*[alBank->count];
	unsigned long** offsetsEnv = new unsigned long*[alBank->count];
	unsigned long** offsetsKey = new unsigned long*[alBank->count];
	unsigned long** offsetsWav = new unsigned long*[alBank->count];
	unsigned long** bookOffsetsWav = new unsigned long*[alBank->count];
	unsigned long** adpcmRawLoopOffsetsWav = new unsigned long*[alBank->count];
	unsigned long** rawLoopOffsetsWav = new unsigned long*[alBank->count];
	unsigned long** tblOffsets = new unsigned long*[alBank->count];

	for (int x = 0; x < alBank->count; x++)
	{
		instrumentSoundStartLookup[x] = new unsigned long[alBank->inst[x]->soundCount];
		offsetsEnv[x] = new unsigned long[alBank->inst[x]->soundCount];
		offsetsKey[x] = new unsigned long[alBank->inst[x]->soundCount];
		offsetsWav[x] = new unsigned long[alBank->inst[x]->soundCount];
		bookOffsetsWav[x] = new unsigned long[alBank->inst[x]->soundCount];
		adpcmRawLoopOffsetsWav[x] = new unsigned long[alBank->inst[x]->soundCount];
		rawLoopOffsetsWav[x] = new unsigned long[alBank->inst[x]->soundCount];
		tblOffsets[x] = new unsigned long[alBank->inst[x]->soundCount];
	}

	for (int x = 0; x < alBank->count; x++)
	{

		

		unsigned long envStart = outputCtlCounter;
		for (int y = 0; y < alBank->inst[x]->soundCount; y++)
		{
			bool foundPast = false;

			for (int r = 0; r < (x + 1); r++)
			{
				int loopEnd = y;
				if (r != x)
				{
					loopEnd = alBank->inst[r]->soundCount;
				}
				for (int z = 0; z < loopEnd; z++)
				{
					if (
						(alBank->inst[x]->sounds[y]->env.attackTime == alBank->inst[r]->sounds[z]->env.attackTime)
						&& (alBank->inst[x]->sounds[y]->env.decayTime == alBank->inst[r]->sounds[z]->env.decayTime)
						&& (alBank->inst[x]->sounds[y]->env.releaseTime == alBank->inst[r]->sounds[z]->env.releaseTime)
						&& (alBank->inst[x]->sounds[y]->env.attackVolume == alBank->inst[r]->sounds[z]->env.attackVolume)
						&& (alBank->inst[x]->sounds[y]->env.decayVolume == alBank->inst[r]->sounds[z]->env.decayVolume)
						)
					{
						foundPast = true;
						offsetsEnv[x][y] = offsetsEnv[r][z];
						break;
					}
				}
			}

			/*if (y != 0)
			{
				//hack
				offsetsEnv[x][y] = offsetsEnv[0][0];
				foundPast = true;
			}*/


			if (foundPast)
				continue;

			offsetsEnv[x][y] = outputCtlCounter;

			WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->env.attackTime);
			outputCtlCounter += 4;
			WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->env.decayTime);
			outputCtlCounter += 4;
			WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->env.releaseTime);
			outputCtlCounter += 4;
			WriteShortToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->env.attackVolume);
			outputCtlCounter += 2;
			WriteShortToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->env.decayVolume);
			outputCtlCounter += 2;
		}
	}

	for (int x = 0; x < alBank->count; x++)
	{


		
		if ((outputCtlCounter % 8) != 0)
		{
			int pad = 8 - (outputCtlCounter % 8);
			for (int z = 0; z < pad; z++)
			{
				temporaryCtlBuffer[outputCtlCounter++] = 0;
			}
		}

		unsigned long keyStart = outputCtlCounter;
		for (int y = 0; y < alBank->inst[x]->soundCount; y++)
		{
			bool foundPast = false;

			for (int r = 0; r < (x + 1); r++)
			{
				int loopEnd = y;
				if (r != x)
				{
					loopEnd = alBank->inst[r]->soundCount;
				}
				for (int z = 0; z < loopEnd; z++)
				{
					if (
						(alBank->inst[x]->sounds[y]->key.velocitymin == alBank->inst[r]->sounds[z]->key.velocitymin)
						&& (alBank->inst[x]->sounds[y]->key.velocitymax == alBank->inst[r]->sounds[z]->key.velocitymax)
						&& (alBank->inst[x]->sounds[y]->key.keymin == alBank->inst[r]->sounds[z]->key.keymin)
						&& (alBank->inst[x]->sounds[y]->key.keymax == alBank->inst[r]->sounds[z]->key.keymax)
						&& (alBank->inst[x]->sounds[y]->key.keybase == alBank->inst[r]->sounds[z]->key.keybase)
						&& (alBank->inst[x]->sounds[y]->key.detune == alBank->inst[r]->sounds[z]->key.detune)
						)
					{
						foundPast = true;
						offsetsKey[x][y] = offsetsKey[r][z];
						break;
					}
				}
			}

			if (foundPast)
				continue;


			offsetsKey[x][y] = outputCtlCounter;

			temporaryCtlBuffer[outputCtlCounter++] = alBank->inst[x]->sounds[y]->key.velocitymin;
			temporaryCtlBuffer[outputCtlCounter++] = alBank->inst[x]->sounds[y]->key.velocitymax;
			temporaryCtlBuffer[outputCtlCounter++] = alBank->inst[x]->sounds[y]->key.keymin;
			temporaryCtlBuffer[outputCtlCounter++] = alBank->inst[x]->sounds[y]->key.keymax;
			temporaryCtlBuffer[outputCtlCounter++] = alBank->inst[x]->sounds[y]->key.keybase;
			temporaryCtlBuffer[outputCtlCounter++] = alBank->inst[x]->sounds[y]->key.detune;

			if ((outputCtlCounter % 8) != 0)
			{
				int pad = 8 - (outputCtlCounter % 8);
				for (int z = 0; z < pad; z++)
				{
					temporaryCtlBuffer[outputCtlCounter++] = 0;
				}
			}
		}


		if ((outputCtlCounter % 8) != 0)
		{
			int pad = 8 - (outputCtlCounter % 8);
			for (int z = 0; z < pad; z++)
			{
				temporaryCtlBuffer[outputCtlCounter++] = 0;
			}
		}
	}

	for (int x = 0; x < alBank->count; x++)
	{


		unsigned long wavStart = outputCtlCounter;

		for (int y = 0; y < alBank->inst[x]->soundCount; y++)
		{
			instrumentSoundStartLookup[x][y] = outputCtlCounter;

			offsetsWav[x][y] = outputCtlCounter + 0x10;

			bool entireSoundMatch = false;

			int entireMatchBank = -1;
			int entireMatch = -1;
			for (int r = 0; r < (x + 1); r++)
			{
				int loopEnd = y;
				if (r != x)
				{
					loopEnd = alBank->inst[r]->soundCount;
				}

				for (int w = 0; w < loopEnd; w++)
				{
					if (
						(alBank->inst[x]->sounds[y]->wav.base == alBank->inst[r]->sounds[w]->wav.base)
						&& (alBank->inst[x]->sounds[y]->wav.len == alBank->inst[r]->sounds[w]->wav.len)
						&& (alBank->inst[x]->sounds[y]->wav.flags == alBank->inst[r]->sounds[w]->wav.flags)
						&& (alBank->inst[x]->sounds[y]->wav.type == alBank->inst[r]->sounds[w]->wav.type)
						)
					{
						if (alBank->inst[x]->sounds[y]->wav.type == 0)
						{
							if (((alBank->inst[x]->sounds[y]->wav.adpcmWave->loop == NULL) && (alBank->inst[r]->sounds[w]->wav.adpcmWave->loop != NULL))
								|| ((alBank->inst[x]->sounds[y]->wav.adpcmWave->loop != NULL) && (alBank->inst[r]->sounds[w]->wav.adpcmWave->loop == NULL)))
							{
								continue;
							}

							bool sameLoop = false;
							if ((alBank->inst[x]->sounds[y]->wav.adpcmWave->loop == NULL) && (alBank->inst[r]->sounds[w]->wav.adpcmWave->loop == NULL))
							{
								sameLoop = true;
							}
							else
							{
								if (
									(alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->start == alBank->inst[r]->sounds[w]->wav.adpcmWave->loop->start)
									&& (alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->end == alBank->inst[r]->sounds[w]->wav.adpcmWave->loop->end)
									&& (alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->count == alBank->inst[r]->sounds[w]->wav.adpcmWave->loop->count))
								{
									bool abort = false;
									for (int z = 0; z < 0x10; z++)
									{
										if (alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->state[z] != alBank->inst[r]->sounds[w]->wav.adpcmWave->loop->state[z])
										{
											abort = true;
											break;
										}
									}

									if (abort)
										continue;
									else

										sameLoop = true;
								}
								else
								{
									continue;
								}
							}

							if (sameLoop)
							{
								if ((alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors == alBank->inst[r]->sounds[w]->wav.adpcmWave->book->npredictors)
									&& (alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order == alBank->inst[r]->sounds[w]->wav.adpcmWave->book->order))
								{
									bool abort = false;
									for (int z = 0; z < (alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * 8); z++)
									{
										if (alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[z] != alBank->inst[r]->sounds[w]->wav.adpcmWave->book->predictors[z])
										{
											abort = true;
											break;
										}
									}

									if (abort)
										continue;
									else
									{
										entireMatchBank = r;
										entireMatch = w;
										offsetsWav[x][y] = offsetsWav[entireMatchBank][entireMatch];
										bookOffsetsWav[x][y] = bookOffsetsWav[entireMatchBank][entireMatch];
										tblOffsets[x][y] = tblOffsets[entireMatchBank][entireMatch];


										if (
											(alBank->inst[x]->sounds[y]->env.attackTime == alBank->inst[entireMatchBank]->sounds[entireMatch]->env.attackTime)
											&& (alBank->inst[x]->sounds[y]->env.attackVolume == alBank->inst[entireMatchBank]->sounds[entireMatch]->env.attackVolume)
											&& (alBank->inst[x]->sounds[y]->env.decayTime == alBank->inst[entireMatchBank]->sounds[entireMatch]->env.decayTime)
											&& (alBank->inst[x]->sounds[y]->env.decayVolume == alBank->inst[entireMatchBank]->sounds[entireMatch]->env.decayVolume)
											&& (alBank->inst[x]->sounds[y]->env.releaseTime == alBank->inst[entireMatchBank]->sounds[entireMatch]->env.releaseTime)
											&& (alBank->inst[x]->sounds[y]->samplePan == alBank->inst[entireMatchBank]->sounds[entireMatch]->samplePan)
											&& (alBank->inst[x]->sounds[y]->sampleVolume == alBank->inst[entireMatchBank]->sounds[entireMatch]->sampleVolume)
											&& (alBank->inst[x]->sounds[y]->flags == alBank->inst[entireMatchBank]->sounds[entireMatch]->flags)
											&& (alBank->inst[x]->sounds[y]->key.detune == alBank->inst[entireMatchBank]->sounds[entireMatch]->key.detune)
											&& (alBank->inst[x]->sounds[y]->key.keybase == alBank->inst[entireMatchBank]->sounds[entireMatch]->key.keybase)
											&& (alBank->inst[x]->sounds[y]->key.keymax == alBank->inst[entireMatchBank]->sounds[entireMatch]->key.keymax)
											&& (alBank->inst[x]->sounds[y]->key.keymin == alBank->inst[entireMatchBank]->sounds[entireMatch]->key.keymin)
											&& (alBank->inst[x]->sounds[y]->key.velocitymax == alBank->inst[entireMatchBank]->sounds[entireMatch]->key.velocitymax)
											&& (alBank->inst[x]->sounds[y]->key.velocitymin == alBank->inst[entireMatchBank]->sounds[entireMatch]->key.velocitymin)
											)
										{
											instrumentSoundStartLookup[x][y] = instrumentSoundStartLookup[entireMatchBank][entireMatch];
											entireSoundMatch = true;
											break;
										}
										else
										{
											continue;
										}
										
									}
								}
								else
								{
									continue;
								}
							}
						}
						else
						{
							if (((alBank->inst[x]->sounds[y]->wav.rawWave->loop == NULL) && (alBank->inst[r]->sounds[w]->wav.rawWave->loop != NULL))
								|| ((alBank->inst[x]->sounds[y]->wav.rawWave->loop != NULL) && (alBank->inst[r]->sounds[w]->wav.rawWave->loop == NULL)))
							{
								continue;
							}

							bool sameLoop = false;
							if ((alBank->inst[x]->sounds[y]->wav.rawWave->loop == NULL) && (alBank->inst[r]->sounds[w]->wav.rawWave->loop == NULL))
							{
								sameLoop = true;
							}
							else
							{
								if (
									(alBank->inst[x]->sounds[y]->wav.rawWave->loop->start == alBank->inst[r]->sounds[w]->wav.rawWave->loop->start)
									&& (alBank->inst[x]->sounds[y]->wav.rawWave->loop->end == alBank->inst[r]->sounds[w]->wav.rawWave->loop->end)
									&& (alBank->inst[x]->sounds[y]->wav.rawWave->loop->count == alBank->inst[r]->sounds[w]->wav.rawWave->loop->count))
								{
									entireMatchBank = r;
									entireMatch = w;
									offsetsWav[x][y] = offsetsWav[entireMatchBank][entireMatch];
									bookOffsetsWav[x][y] = bookOffsetsWav[entireMatchBank][entireMatch];
									tblOffsets[x][y] = tblOffsets[entireMatchBank][entireMatch];
									
									if (
											(alBank->inst[x]->sounds[y]->env.attackTime == alBank->inst[entireMatchBank]->sounds[entireMatch]->env.attackTime)
											&& (alBank->inst[x]->sounds[y]->env.attackVolume == alBank->inst[entireMatchBank]->sounds[entireMatch]->env.attackVolume)
											&& (alBank->inst[x]->sounds[y]->env.decayTime == alBank->inst[entireMatchBank]->sounds[entireMatch]->env.decayTime)
											&& (alBank->inst[x]->sounds[y]->env.decayVolume == alBank->inst[entireMatchBank]->sounds[entireMatch]->env.decayVolume)
											&& (alBank->inst[x]->sounds[y]->env.releaseTime == alBank->inst[entireMatchBank]->sounds[entireMatch]->env.releaseTime)
											&& (alBank->inst[x]->sounds[y]->samplePan == alBank->inst[entireMatchBank]->sounds[entireMatch]->samplePan)
											&& (alBank->inst[x]->sounds[y]->sampleVolume == alBank->inst[entireMatchBank]->sounds[entireMatch]->sampleVolume)
											&& (alBank->inst[x]->sounds[y]->flags == alBank->inst[entireMatchBank]->sounds[entireMatch]->flags)
											&& (alBank->inst[x]->sounds[y]->key.detune == alBank->inst[entireMatchBank]->sounds[entireMatch]->key.detune)
											&& (alBank->inst[x]->sounds[y]->key.keybase == alBank->inst[entireMatchBank]->sounds[entireMatch]->key.keybase)
											&& (alBank->inst[x]->sounds[y]->key.keymax == alBank->inst[entireMatchBank]->sounds[entireMatch]->key.keymax)
											&& (alBank->inst[x]->sounds[y]->key.keymin == alBank->inst[entireMatchBank]->sounds[entireMatch]->key.keymin)
											&& (alBank->inst[x]->sounds[y]->key.velocitymax == alBank->inst[entireMatchBank]->sounds[entireMatch]->key.velocitymax)
											&& (alBank->inst[x]->sounds[y]->key.velocitymin == alBank->inst[entireMatchBank]->sounds[entireMatch]->key.velocitymin)
											)
										{
											instrumentSoundStartLookup[x][y] = instrumentSoundStartLookup[entireMatchBank][entireMatch];
											entireSoundMatch = true;
											break;
										}
										else
										{
											continue;
										}
								}
								else
								{
									continue;
								}
							}
						}
					}
				}
			}

			

			if (!entireSoundMatch)
			{
				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, offsetsEnv[x][y]);
				outputCtlCounter += 4;
				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, offsetsKey[x][y]);
				outputCtlCounter += 4;

				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, offsetsWav[x][y]);
				outputCtlCounter += 4;

				temporaryCtlBuffer[outputCtlCounter++] = alBank->inst[x]->sounds[y]->samplePan;
				temporaryCtlBuffer[outputCtlCounter++] = alBank->inst[x]->sounds[y]->sampleVolume;
				WriteShortToBuffer(temporaryCtlBuffer, outputCtlCounter, 0);
				outputCtlCounter += 2;
			}

			if (entireMatch == -1)
			{
			
				tblOffsets[x][y] = outputTblCounter;

				int foundSameTbl = false;

				for (int r = 0; r < (x + 1); r++)
				{
					int loopEnd = y;
					if (r != x)
					{
						loopEnd = alBank->inst[r]->soundCount;
					}
					for (int z = 0; z < loopEnd; z++)
					{
						if (
							(alBank->inst[x]->sounds[y]->wav.base == alBank->inst[r]->sounds[z]->wav.base)
							&& (alBank->inst[x]->sounds[y]->wav.len == alBank->inst[r]->sounds[z]->wav.len)
							)
						{
							bool mismatchValues = false;
							for (int rr = 0; rr < alBank->inst[x]->sounds[y]->wav.len; rr++)
							{
								if (alBank->inst[x]->sounds[y]->wav.wavData[rr] != alBank->inst[r]->sounds[z]->wav.wavData[rr])
								{
									mismatchValues = true;
									break;
								}
							}
							if (!mismatchValues)
							{
								foundSameTbl = true;
								tblOffsets[x][y] = tblOffsets[r][z];
								break;
							}
							else
							{
								continue;
							}
						}
					}
				}

				// update base
				//alBank->inst[x]->sounds[y]->wav.base = tblOffsets[x][y];

				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, tblOffsets[x][y]);
				outputCtlCounter += 4;
				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wav.len);
				outputCtlCounter += 4;

				if (!foundSameTbl)
				{
					for (int z = 0; z < alBank->inst[x]->sounds[y]->wav.len; z++)
					{
						temporaryTblBuffer[outputTblCounter + z] = alBank->inst[x]->sounds[y]->wav.wavData[z];
					}
					outputTblCounter += alBank->inst[x]->sounds[y]->wav.len;

					if ((outputTblCounter % 8) != 0)
					{
						int pad = 8 - (outputTblCounter % 8);
						for (int z = 0; z < pad; z++)
						{
							temporaryTblBuffer[outputTblCounter++] = 0;
						}
					}
				}

				temporaryCtlBuffer[outputCtlCounter++] = alBank->inst[x]->sounds[y]->wav.type;
				temporaryCtlBuffer[outputCtlCounter++] = 0; // offset type
				temporaryCtlBuffer[outputCtlCounter++] = 0; // pad
				temporaryCtlBuffer[outputCtlCounter++] = 0; // pad

				if (alBank->inst[x]->sounds[y]->wav.type == AL_ADPCM_WAVE)
				{
					int sameLoop = -1;
					int sameLoopBank = -1;


					if (alBank->inst[x]->sounds[y]->wav.adpcmWave->loop != NULL)
					{
						for (int r = 0; r < (x + 1); r++)
						{
							if (sameLoop != -1)
								break;

							int loopEnd = y;
							if (r != x)
							{
								loopEnd = alBank->inst[r]->soundCount;
							}
							for (int w = 0; w < loopEnd; w++)
							{
								if ((alBank->inst[r]->sounds[w]->wav.adpcmWave != NULL) && (alBank->inst[r]->sounds[w]->wav.adpcmWave->loop != NULL))
								{
									if ((alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->start == alBank->inst[r]->sounds[w]->wav.adpcmWave->loop->start) 
										&& (alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->end == alBank->inst[r]->sounds[w]->wav.adpcmWave->loop->end)
										&& (alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->count == alBank->inst[r]->sounds[w]->wav.adpcmWave->loop->count))
									{
										bool goOn = false;
										for (int ww = 0; ww < 0x10; ww++)
										{
											if (alBank->inst[r]->sounds[w]->wav.adpcmWave->loop->state[ww] != alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->state[ww])
											{
												goOn = true;
												break;
											}
										}

										if (goOn)
											continue;

										adpcmRawLoopOffsetsWav[x][y] = adpcmRawLoopOffsetsWav[r][w];
										sameLoopBank = r;
										sameLoop = w;
										break;
									}
								}
							}
						}
					}

					unsigned long loopWriteSpot = outputCtlCounter;
					unsigned long bookWriteSpot = outputCtlCounter + 4;
					 
					int same = -1;
					int sameBank = -1;

					for (int r = 0; r < (x + 1); r++)
					{
						if (same != -1)
							break;

						int loopEnd = y;
						if (r != x)
						{
							loopEnd = alBank->inst[r]->soundCount;
						}
						for (int w = 0; w < loopEnd; w++)
						{
							if (alBank->inst[r]->sounds[w]->wav.adpcmWave != NULL)
							{
								if ((alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order == alBank->inst[r]->sounds[w]->wav.adpcmWave->book->order) 
									&& (alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors == alBank->inst[r]->sounds[w]->wav.adpcmWave->book->npredictors))
								{
									bool goOn = false;
									for (int ww = 0; ww < (alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8); ww++)
									{
										if (alBank->inst[r]->sounds[w]->wav.adpcmWave->book->predictors[ww] != alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[ww])
										{
											goOn = true;
											break;
										}
									}

									if (goOn)
										continue;

									sameBank = r;
									same = w;
									break;
								}
							}
						}
					}

					// write later loop/predictor offsets
					outputCtlCounter += 8;


					if ((outputCtlCounter % 8) != 0)
					{
						int pad = 8 - (outputCtlCounter % 8);
						for (int z = 0; z < pad; z++)
						{
							temporaryCtlBuffer[outputCtlCounter++] = 0;
						}
					}

					if (alBank->inst[x]->sounds[y]->wav.adpcmWave->loop != NULL)
					{
						if (sameLoop == -1)
						{
							adpcmRawLoopOffsetsWav[x][y] = outputCtlCounter;
							WriteLongToBuffer(temporaryCtlBuffer, loopWriteSpot, outputCtlCounter);

							WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->start);
							outputCtlCounter += 4;
							WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->end);
							outputCtlCounter += 4;
							WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->count);
							outputCtlCounter += 4;
							for (int z = 0; z < 0x10; z++)
							{
								temporaryCtlBuffer[outputCtlCounter++] = ((alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->state[z] >> 8) & 0xFF);
								temporaryCtlBuffer[outputCtlCounter++] = ((alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->state[z]) & 0xFF);
							}
						}
						else
						{
							WriteLongToBuffer(temporaryCtlBuffer, loopWriteSpot, adpcmRawLoopOffsetsWav[sameLoopBank][sameLoop]);
						}
					}
					else
					{
						// null loop
						WriteLongToBuffer(temporaryCtlBuffer, loopWriteSpot, 0x0);
					}

					if ((outputCtlCounter % 8) != 0)
					{
						int pad = 8 - (outputCtlCounter % 8);
						for (int z = 0; z < pad; z++)
						{
							temporaryCtlBuffer[outputCtlCounter++] = 0;
						}
					}

					if (same == -1)
					{
						bookOffsetsWav[x][y] = outputCtlCounter;
						WriteLongToBuffer(temporaryCtlBuffer, bookWriteSpot, outputCtlCounter);

						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order);
						outputCtlCounter += 4;
						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors);
						outputCtlCounter += 4;

						for (int z = 0; z < (alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8); z++)
						{
							temporaryCtlBuffer[outputCtlCounter++] = ((alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[z] >> 8) & 0xFF);
							temporaryCtlBuffer[outputCtlCounter++] = ((alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[z]) & 0xFF);
						}

						
						// game does this not sure why
						/*int pad = 8;
						for (int z = 0; z < pad; z++)
						{
							temporaryCtlBuffer[outputCtlCounter++] = 0;
						}*/
					}
					else
					{
						WriteLongToBuffer(temporaryCtlBuffer, bookWriteSpot, bookOffsetsWav[sameBank][same]);
					}
				}
				else
				{

					if (alBank->inst[x]->sounds[y]->wav.rawWave->loop != NULL)
					{
						int sameBank = -1;
						int same = -1;

						for (int r = 0; r < (x + 1); r++)
						{
							if (same != -1)
								break;

							int loopEnd = y;
							if (r != x)
							{
								loopEnd = alBank->inst[r]->soundCount;
							}
							for (int w = 0; w < loopEnd; w++)
							{
								if ((alBank->inst[r]->sounds[w]->wav.rawWave != NULL) && (alBank->inst[r]->sounds[w]->wav.rawWave->loop != NULL))
								{
									if ((alBank->inst[x]->sounds[y]->wav.rawWave->loop->start == alBank->inst[r]->sounds[w]->wav.rawWave->loop->start) 
										&& (alBank->inst[x]->sounds[y]->wav.rawWave->loop->end == alBank->inst[r]->sounds[w]->wav.rawWave->loop->end) 
										&& (alBank->inst[x]->sounds[y]->wav.rawWave->loop->count == alBank->inst[r]->sounds[w]->wav.rawWave->loop->count) 
										)
									{
										sameBank = r;
										rawLoopOffsetsWav[x][y] = rawLoopOffsetsWav[r][w];
										same = w;
										break;
									}
								}
							}
						}

						if (same == -1)
						{
							unsigned long loopWriteSpot = outputCtlCounter;

							outputCtlCounter += 4;

							if ((outputCtlCounter % 8) != 0)
							{
								int pad = 8 - (outputCtlCounter % 8);
								for (int z = 0; z < pad; z++)
								{
									temporaryCtlBuffer[outputCtlCounter++] = 0;
								}
							}

							rawLoopOffsetsWav[x][y] = (outputCtlCounter);
							WriteLongToBuffer(temporaryCtlBuffer, loopWriteSpot, outputCtlCounter);

							WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wav.rawWave->loop->start);
							outputCtlCounter += 4;
							WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wav.rawWave->loop->end);
							outputCtlCounter += 4;
							WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wav.rawWave->loop->count);
							outputCtlCounter += 4;
						}
						else
						{
							WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, rawLoopOffsetsWav[sameBank][same]);
							outputCtlCounter += 4;
						}
					}
					else
					{
						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, 0);
						outputCtlCounter += 4;
					}

					if ((outputCtlCounter % 8) != 0)
					{
						int pad = 8 - (outputCtlCounter % 8);
						for (int z = 0; z < pad; z++)
						{
							temporaryCtlBuffer[outputCtlCounter++] = 0;
						}
					}
				}
			}
		}

		if ((outputCtlCounter % 8) != 0)
		{
			int pad = 8 - (outputCtlCounter % 8);
			for (int z = 0; z < pad; z++)
			{
				temporaryCtlBuffer[outputCtlCounter++] = 0;
			}
		}
	}

	unsigned long startALSound = outputCtlCounter;


	for (int y = 0; y < alBank->count; y++)
	{
		instrumentALLookup[y] = outputCtlCounter;
		temporaryCtlBuffer[outputCtlCounter++] = alBank->inst[y]->volume;
		temporaryCtlBuffer[outputCtlCounter++] = alBank->inst[y]->pan;
		temporaryCtlBuffer[outputCtlCounter++] = alBank->inst[y]->priority;
		temporaryCtlBuffer[outputCtlCounter++] = 0; //offset type
		temporaryCtlBuffer[outputCtlCounter++] = alBank->inst[y]->tremType;
		temporaryCtlBuffer[outputCtlCounter++] = alBank->inst[y]->tremRate;
		temporaryCtlBuffer[outputCtlCounter++] = alBank->inst[y]->tremDepth;
		temporaryCtlBuffer[outputCtlCounter++] = alBank->inst[y]->tremDelay;
		temporaryCtlBuffer[outputCtlCounter++] = alBank->inst[y]->vibType;
		temporaryCtlBuffer[outputCtlCounter++] = alBank->inst[y]->vibRate;
		temporaryCtlBuffer[outputCtlCounter++] = alBank->inst[y]->vibDepth;
		temporaryCtlBuffer[outputCtlCounter++] = alBank->inst[y]->vibDelay;
		WriteShortToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[y]->bendRange);
		outputCtlCounter += 2;
		WriteShortToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[y]->soundCount);
		outputCtlCounter += 2;
		
		for (int z = 0; z < alBank->inst[y]->soundCount; z++)
		{
			WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, (instrumentSoundStartLookup[y][z]));
			outputCtlCounter += 4;
		}

		if ((outputCtlCounter % 8) != 0)
		{
			int pad = 8 - (outputCtlCounter % 8);
			for (int z = 0; z < pad; z++)
			{
				temporaryCtlBuffer[outputCtlCounter++] = 0;
			}
		}
	}

	if ((outputCtlCounter % 8) != 0)
	{
		int pad = 8 - (outputCtlCounter % 8);
		for (int z = 0; z < pad; z++)
		{
			temporaryCtlBuffer[outputCtlCounter++] = 0;
		}
	}

	unsigned long startBank = outputCtlCounter;
	WriteLongToBuffer(temporaryCtlBuffer, 0x4, startBank);

	WriteShortToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->count);
	outputCtlCounter += 2;
	WriteShortToBuffer(temporaryCtlBuffer, outputCtlCounter, 0); // offset type
	outputCtlCounter += 2;
	WriteShortToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->pad);
	outputCtlCounter += 2;
	WriteShortToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->samplerate);
	outputCtlCounter += 2;
	WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->percussion);
	outputCtlCounter += 4;

	for (int y = 0; y < alBank->count; y++)
	{
		WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, instrumentALLookup[y]);
		outputCtlCounter += 4;
	}
	outputCtlCounter += 4;

	if ((outputCtlCounter % 0x10) != 0)
	{
		int pad = 0x10 - (outputCtlCounter % 0x10);
		for (int z = 0; z < pad; z++)
		{
			temporaryCtlBuffer[outputCtlCounter++] = 0;
		}
	}

	for (int x = 0; x < alBank->count; x++)
	{
		delete [] instrumentSoundStartLookup[x];
		delete [] offsetsEnv[x];;
		delete [] offsetsKey[x];;
		delete [] offsetsWav[x];;
		delete [] bookOffsetsWav[x];;
		delete [] adpcmRawLoopOffsetsWav[x];;
		delete [] rawLoopOffsetsWav[x];;
		delete [] tblOffsets[x];;
	}

	delete [] instrumentSoundStartLookup;
	delete [] instrumentALLookup;
	delete [] offsetsEnv;
	delete [] offsetsKey;
	delete [] offsetsWav;
	delete [] bookOffsetsWav;
	delete [] adpcmRawLoopOffsetsWav;
	delete [] rawLoopOffsetsWav;
	delete [] tblOffsets;
	
	ctl = temporaryCtlBuffer;
	tbl = temporaryTblBuffer;

	ctlSize = outputCtlCounter;
	tblSize = outputTblCounter;
}

bool CN64AIFCAudio::InjectCtlTblIntoROMArrayInPlace(unsigned char* ROM, unsigned char* ctl, int ctlSize, unsigned char* tbl, int tblSize, unsigned long ctlOffset, unsigned long tblOffset, unsigned long maxCtl, unsigned long maxTbl)
{
	if (ctlSize > maxCtl)
	{
		MessageBox(NULL, "Error ctl too big", "Error", NULL);
		return false;
	}
	if (tblSize > maxTbl)
	{
		MessageBox(NULL, "Error tbl too big", "Error", NULL);
		return false;
	}
	for (int x = ctlOffset; x < (ctlOffset + ctlSize); x++)
	{
		ROM[x] = ctl[x - ctlOffset];
	}
	for (int x = tblOffset; x < (tblOffset + tblSize); x++)
	{
		ROM[x] = tbl[x - tblOffset];
	}
	return true;
}

void CN64AIFCAudio::DisposeALBank(ALBank*& alBank)
{
	if (alBank != NULL)
	{
		if (alBank->inst != NULL)
		{
			for (int x = 0; x < alBank->count; x++)
			{
				if (alBank->inst[x]->sounds != NULL)
				{
					for (int y = 0; y < alBank->inst[x]->soundCount; y++)
					{
						if (alBank->inst[x]->sounds[y]->wav.type == AL_ADPCM_WAVE)
						{
							if (alBank->inst[x]->sounds[y]->wav.adpcmWave->loop != NULL)
							{
								delete alBank->inst[x]->sounds[y]->wav.adpcmWave->loop;
							}
							delete [] alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors;
							delete alBank->inst[x]->sounds[y]->wav.adpcmWave->book;
							delete alBank->inst[x]->sounds[y]->wav.adpcmWave;
						}
						else
						{
							if (alBank->inst[x]->sounds[y]->wav.rawWave->loop != NULL)
							{
								delete alBank->inst[x]->sounds[y]->wav.rawWave->loop;
							}
							delete alBank->inst[x]->sounds[y]->wav.rawWave;
						}

						if (alBank->inst[x]->sounds[y]->wav.wavData != NULL)
							delete [] alBank->inst[x]->sounds[y]->wav.wavData;

						delete alBank->inst[x]->sounds[y];
					}

					delete alBank->inst[x]->sounds;
				}

				delete alBank->inst[x];
			}

			delete alBank->inst;
		}

		delete alBank;
	}
}

ALBank* CN64AIFCAudio::ReadAudio(unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl)
{
	unsigned short numberBands = CharArrayToShort(&ctl[ctlOffset+0x2]);
	unsigned long bankOffset = CharArrayToLong(&ctl[ctlOffset+0x4]);
	if ((bankOffset >> 24) == 0x80)
		bankOffset = bankOffset & 0xFFFFFF;
	else
		bankOffset = ctlOffset + bankOffset;

	ALBank* alBank = new ALBank();
	alBank->count = CharArrayToShort(&ctl[bankOffset]);
	alBank->flags = CharArrayToShort(&ctl[bankOffset+2]);
	alBank->pad = CharArrayToShort(&ctl[bankOffset+4]);
	alBank->samplerate = CharArrayToShort(&ctl[bankOffset+6]);
	alBank->percussion = CharArrayToLong(&ctl[bankOffset+8]);
	
	alBank->inst = NULL;

	if ((alBank->flags == 0x0000) || (alBank->flags == 0x0100)) // offset
	{
		alBank->inst = new ALInst*[alBank->count];

		for (int x = 0; x < alBank->count; x++)
		{
			alBank->inst[x] = new ALInst();
			alBank->inst[x]->sounds = NULL;
		}

		for (int x = 0; x < alBank->count; x++)
		{
			unsigned long offsetInstrument = CharArrayToLong(&ctl[bankOffset+ 0xC + x*4]);
			if (alBank->flags == 0x0100)
				offsetInstrument = offsetInstrument & 0xFFFFFF;
			else
				offsetInstrument = ctlOffset + offsetInstrument;

			alBank->inst[x]->volume = ctl[offsetInstrument];
			alBank->inst[x]->pan = ctl[offsetInstrument+1];
			alBank->inst[x]->priority = ctl[offsetInstrument+2];
			alBank->inst[x]->flags = ctl[offsetInstrument+3];
			alBank->inst[x]->tremType = ctl[offsetInstrument+4];
			alBank->inst[x]->tremRate = ctl[offsetInstrument+5];
			alBank->inst[x]->tremDepth = ctl[offsetInstrument+6];
			alBank->inst[x]->tremDelay = ctl[offsetInstrument+7];
			alBank->inst[x]->vibType = ctl[offsetInstrument+8];
			alBank->inst[x]->vibRate = ctl[offsetInstrument+9];
			alBank->inst[x]->vibDepth = ctl[offsetInstrument+0xA];
			alBank->inst[x]->vibDelay = ctl[offsetInstrument+0xB];
			alBank->inst[x]->bendRange = CharArrayToShort(&ctl[offsetInstrument+0xC]);
			alBank->inst[x]->soundCount = CharArrayToShort(&ctl[offsetInstrument+0xE]);

			if ((alBank->inst[x]->flags == 0x0) || (alBank->inst[x]->flags == 0x1))
			{	
				alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

				for (int y = 0; y < alBank->inst[x]->soundCount; y++)
				{
					alBank->inst[x]->sounds[y] = new ALSound();
					alBank->inst[x]->sounds[y]->wav.wavData = NULL;
				}

				for (int y = 0; y < alBank->inst[x]->soundCount; y++)
				{
					unsigned long offsetSound = CharArrayToLong(&ctl[offsetInstrument+ 0x10 + y*0x4]);
					if (alBank->inst[x]->flags == 0x1)
						offsetSound = offsetSound & 0xFFFFFF;
					else
						offsetSound = ctlOffset + offsetSound;

					alBank->inst[x]->sounds[y]->samplePan = ctl[offsetSound+0xC];
					alBank->inst[x]->sounds[y]->sampleVolume = ctl[offsetSound+0xD];
					alBank->inst[x]->sounds[y]->flags = ctl[offsetSound+0xE];

					
					if ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1))
					{
						unsigned long offsetEnv = CharArrayToLong(&ctl[offsetSound+ 0x0]);
						if (alBank->inst[x]->sounds[y]->flags == 0x1)
							offsetEnv = offsetEnv & 0xFFFFFF;
						else
							offsetEnv = ctlOffset + offsetEnv;

						unsigned long offsetKeyMap = CharArrayToLong(&ctl[offsetSound+ 0x4]);
						if (alBank->inst[x]->sounds[y]->flags == 0x1)
							offsetKeyMap = offsetKeyMap & 0xFFFFFF;
						else
							offsetKeyMap = ctlOffset + offsetKeyMap;

						unsigned long offsetWaveTable = CharArrayToLong(&ctl[offsetSound+ 0x8]);
						if (alBank->inst[x]->sounds[y]->flags == 0x1)
							offsetWaveTable = offsetWaveTable & 0xFFFFFF;
						else
							offsetWaveTable = ctlOffset + offsetWaveTable;

						alBank->inst[x]->sounds[y]->env.attackTime = CharArrayToLong(&ctl[offsetEnv + 0x0]);
						alBank->inst[x]->sounds[y]->env.decayTime = CharArrayToLong(&ctl[offsetEnv + 0x4]);
						alBank->inst[x]->sounds[y]->env.releaseTime = CharArrayToLong(&ctl[offsetEnv + 0x8]);
						alBank->inst[x]->sounds[y]->env.attackVolume = CharArrayToShort(&ctl[offsetEnv + 0xC]);
						alBank->inst[x]->sounds[y]->env.decayVolume = CharArrayToShort(&ctl[offsetEnv + 0xE]);

						alBank->inst[x]->sounds[y]->key.velocitymin = ctl[offsetKeyMap];
						alBank->inst[x]->sounds[y]->key.velocitymax = ctl[offsetKeyMap+0x1];
						alBank->inst[x]->sounds[y]->key.keymin = ctl[offsetKeyMap+0x2];
						alBank->inst[x]->sounds[y]->key.keymax = ctl[offsetKeyMap+0x3];
						alBank->inst[x]->sounds[y]->key.keybase = ctl[offsetKeyMap+0x4];
						alBank->inst[x]->sounds[y]->key.detune = ctl[offsetKeyMap+0x5];

						alBank->inst[x]->sounds[y]->wav.base = CharArrayToLong(&ctl[offsetWaveTable + 0x0]);
						alBank->inst[x]->sounds[y]->wav.len = CharArrayToLong(&ctl[offsetWaveTable + 0x4]);
						alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];

						for (int  r = 0; r < alBank->inst[x]->sounds[y]->wav.len; r++)
						{
							alBank->inst[x]->sounds[y]->wav.wavData[r] = tbl[alBank->inst[x]->sounds[y]->wav.base + r];
						}
						

						alBank->inst[x]->sounds[y]->wav.type = ctl[offsetWaveTable + 0x8];
						alBank->inst[x]->sounds[y]->wav.flags = ctl[offsetWaveTable + 0x9];
						// MUST PAD to 4s

						if ((alBank->inst[x]->sounds[y]->wav.type == AL_ADPCM_WAVE) && ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1)))
						{
							alBank->inst[x]->sounds[y]->wav.adpcmWave = new ALADPCMWaveInfo();
							unsigned long loopOffset = CharArrayToLong(&ctl[offsetWaveTable + 0xC]);
							unsigned long predictorOffset = CharArrayToLong(&ctl[offsetWaveTable + 0x10]);

							if (loopOffset != 0)
							{
								if (alBank->inst[x]->sounds[y]->flags == 0x1)
									loopOffset = loopOffset & 0xFFFFFF;
								else
									loopOffset = ctlOffset + loopOffset;

								alBank->inst[x]->sounds[y]->wav.adpcmWave->loop = new ALADPCMloop();
								alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->start = CharArrayToLong(&ctl[loopOffset + 0x0]);
								alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->end = CharArrayToLong(&ctl[loopOffset + 0x4]);
								alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->count = CharArrayToLong(&ctl[loopOffset + 0x8]);
								for (int z = 0; z < 0x10; z++)
								{
									alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->state[z] = CharArrayToShort(&ctl[loopOffset + 0xC + z * 2]);
								}
							}
							else
							{
								alBank->inst[x]->sounds[y]->wav.adpcmWave->loop = NULL;
							}

							if (predictorOffset != 0)
							{
								if (alBank->inst[x]->sounds[y]->flags == 0x1)
									predictorOffset = predictorOffset & 0xFFFFFF;
								else
									predictorOffset = ctlOffset + predictorOffset;

								alBank->inst[x]->sounds[y]->wav.adpcmWave->book = new ALADPCMBook();
								alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order = CharArrayToLong(&ctl[predictorOffset + 0x0]);
								alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors = CharArrayToLong(&ctl[predictorOffset + 0x4]);
								alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors = new signed short[alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8];

								for (int z = 0; z < alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8; z++)
								{
									alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[z] = (signed short)CharArrayToShort(&ctl[predictorOffset + 0x8 + z * 2]);
								}
							}
						}
						else if ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1))
						{
							alBank->inst[x]->sounds[y]->wav.rawWave = new ALRAWWaveInfo();
							unsigned long loopOffset = CharArrayToLong(&ctl[offsetWaveTable + 0xC]);

							if (loopOffset != 0)
							{
								if (alBank->inst[x]->sounds[y]->flags == 0x1)
									loopOffset = loopOffset & 0xFFFFFF;
								else
									loopOffset = ctlOffset + loopOffset;

								alBank->inst[x]->sounds[y]->wav.rawWave->loop = new ALRawLoop();
								alBank->inst[x]->sounds[y]->wav.rawWave->loop->start = CharArrayToLong(&ctl[loopOffset + 0x0]);
								alBank->inst[x]->sounds[y]->wav.rawWave->loop->end = CharArrayToLong(&ctl[loopOffset + 0x4]);
								alBank->inst[x]->sounds[y]->wav.rawWave->loop->count = CharArrayToLong(&ctl[loopOffset + 0x8]);	
							}
							else
							{
								alBank->inst[x]->sounds[y]->wav.rawWave->loop = NULL;
							}
						}
						else
						{
							DisposeALBank(alBank);
							MessageBox(NULL, "Error", "Unsupported type in ALSound wav", NULL);
							return NULL;
						}
					}
					else
					{
						DisposeALBank(alBank);
						MessageBox(NULL, "Error", "Unsupported type in ALSound", NULL);
						return NULL;
					}
				}
			}
			else
			{
				DisposeALBank(alBank);
				MessageBox(NULL, "Error", "Unsupported inst type in ALInst", NULL);
				return NULL;
			}
		}
	}
	else
	{
		DisposeALBank(alBank);
		MessageBox(NULL, "Error", "Unsupported type in ALBank", NULL);
		return NULL;
	}

	return alBank;
}

int CN64AIFCAudio::GetSizeFile(CString filename)
{
	FILE* inFile = fopen(filename, "rb");
	
	if (inFile == 0)
		return 0;

	fseek(inFile, 0, SEEK_END);
	int fileSize = ftell(inFile);
	fclose(inFile);
	return fileSize;
}


unsigned short CN64AIFCAudio::Flip16Bit(unsigned short ShortValue)
{
	return ((ShortValue >> 8) | ((ShortValue << 8)));
}

unsigned long CN64AIFCAudio::Flip32Bit(unsigned long inLong)
{
	return (((inLong & 0xFF000000) >> 24) | ((inLong & 0x00FF0000) >> 8) | ((inLong & 0x0000FF00) << 8) | ((inLong & 0x000000FF) << 24));
}

unsigned short CN64AIFCAudio::CharArrayToShort(unsigned char* currentSpot)
{
	return Flip16Bit(*reinterpret_cast<unsigned short*> (currentSpot));
}

float CN64AIFCAudio::CharArrayToFloat(unsigned char* currentSpot)
{
	unsigned long tempLong = (Flip32Bit(*reinterpret_cast<unsigned long*> (currentSpot)));
	return (*reinterpret_cast<float*> (&tempLong));
}

unsigned long CN64AIFCAudio::CharArrayToLong(unsigned char* currentSpot)
{
	return Flip32Bit(*reinterpret_cast<unsigned long*> (currentSpot));
}

void CN64AIFCAudio::WriteLongToBuffer(unsigned char* Buffer, unsigned long address, unsigned long data)
{
	Buffer[address] = ((data >> 24) & 0xFF);
	Buffer[address+1] = ((data >> 16) & 0xFF);
	Buffer[address+2] = ((data >> 8) & 0xFF);
	Buffer[address+3] = ((data) & 0xFF);
}

void CN64AIFCAudio::WriteShortToBuffer(unsigned char* Buffer, unsigned long address, unsigned short data)
{
	Buffer[address] = ((data >> 8) & 0xFF);
	Buffer[address+1] = ((data) & 0xFF);	
}

int CN64AIFCAudio::ReadCtlTblLocations(unsigned char* ROM, int romSize, ctlTblResult*& results)
{
	int numResults = 0;
	for (int x = 0x0; x < (romSize - 4); x+=4)
	{
		unsigned long value = CharArrayToLong(&ROM[x]);

 		if (value == 0x42310001)
		{
			// possible ctl
			unsigned long bankOffset = CharArrayToLong(&ROM[x+4]);
			if ((bankOffset >> 24) != 0x80) // not pointer
				bankOffset = (x + bankOffset);
			else // absolute pointer, mask
				bankOffset = (bankOffset & 0xFFFFFF);

			if (bankOffset > romSize)
				continue;

			unsigned short numberInstruments = CharArrayToShort(&ROM[bankOffset]);

			unsigned long endInstrumentOffsets = (bankOffset + 0xC + (4 * numberInstruments));

			if (endInstrumentOffsets > romSize)
				continue;

			unsigned long tbl = endInstrumentOffsets;
			if ((endInstrumentOffsets % 0x10) != 0)
				tbl += (0x10 - (endInstrumentOffsets % 0x10));
			else
				tbl += 0x10;

			while (CharArrayToLong(&ROM[tbl]) != 0)
			{
				tbl -= 4;
			}

			tbl += 4;

			results[numResults].ctlOffset = x;
			results[numResults].tblOffset = tbl;
			results[numResults].ctlSize = (results[numResults].tblOffset - results[numResults].ctlOffset);
			if (results[numResults].tblOffset > results[numResults].ctlOffset)
				results[numResults].tblSize = (romSize - results[numResults].tblOffset);
			else
				results[numResults].tblSize = (results[numResults].ctlOffset - results[numResults].tblOffset);

			results[numResults].bank = ReadAudio(&ROM[0], results[numResults].ctlSize, results[numResults].ctlOffset, &ROM[results[numResults].tblOffset]);

			if (results[numResults].bank != NULL)
			{
				unsigned long tblHighestSpot = 0;


				
				for (int y = 0; y < results[numResults].bank->count; y++)
				{
					for (int z = 0; z < results[numResults].bank->inst[y]->soundCount; z++)
					{
						if ((results[numResults].bank->inst[y]->sounds[z]->wav.base + results[numResults].bank->inst[y]->sounds[z]->wav.len) > tblHighestSpot)
							tblHighestSpot = (results[numResults].bank->inst[y]->sounds[z]->wav.base + results[numResults].bank->inst[y]->sounds[z]->wav.len);
					}
				}

				if ((tblHighestSpot % 0x10) != 0)
					tblHighestSpot += (0x10 - (tblHighestSpot % 0x10));

				results[numResults].tblSize = tblHighestSpot;

				numResults++;

				if (numResults == 10)
					return numResults;
			}
		}
	}
	return numResults;
}

// ***************	//
// VADPCM Decoding	//
// By Ice Mario!	//
// ***************	//

static const short itable[16] =
{
	0,1,2,3,4,5,6,7,
	-8,-7,-6,-5,-4,-3,-2,-1,
};

//#define ENCODE


void CN64AIFCAudio::decode_8( unsigned char *in, signed short *out , int index, signed short * pred1, signed short lastsmp[8])
{
	int i;
	signed short tmp[8];
	signed long total = 0;
	signed short sample =0;
	memset(out, 0, sizeof(signed short)*8);

	signed short *pred2 = (pred1 + 8);

	//printf("pred2[] = %x\n" , pred2[0]);
	for(i=0; i<8; i++)
	{
		tmp[i] = itable[(i&1) ? (*in++ & 0xf) : ((*in >> 4) & 0xf)] << index;
	}

	for(i=0; i<8; i++)
	{
		total = (pred1[i] * lastsmp[6]);
		total+= (pred2[i] * lastsmp[7]);

		if (i>0)
		{
			for(int x=i-1; x>-1; x--)
			{
				total += ( tmp[((i-1)-x)] * pred2[x] );
				//printf("sample: %x - pred: %x - _smp: %x\n" , ((i-1)-x) , pred2[x] , tmp[((i-1)-x)]);
			}
		}

		//printf("pred = %x | total = %x\n" , pred2[0] , total);
		float result = ((tmp[i] << 0xb) + total) >> 0xb;
		if (result > 32767)
			sample = 32767;
		else if (result < -32768)
			sample = -32768;
		else
			sample = (signed short)result;

		// TODO: still need to clamp the final sample
		out[i] = sample;
	}
	// update the last sample set for subsequent iterations
	memcpy(lastsmp, out, sizeof(signed short)*8);
}
//static signed short preds[16] = { 0,0,0,0,0,0,0,0, 0x1234, 0x5678, 0x9abc, 0,0,0,0,0,};

unsigned long CN64AIFCAudio::decode( unsigned char *in, signed short *out, unsigned long len, ALADPCMBook *book )
{
	signed short lastsmp[8];

	for (int x = 0; x < 8; x++)
		lastsmp[x] = 0;

	int index;
	int pred;
	unsigned char cmd;
	unsigned char *pin = in;
	signed short *pout = out;
	int j;
	unsigned char n1,n2;
	int total = 0;
	int _tmp;

	int _len = (len / 9) * 9;	//make sure length was actually a multiple of 9
	int samples = 0;

	// flip the predictors
	signed short *preds = (signed short*)malloc( 32 * book->npredictors );
	for(int p=0; p<(16*book->npredictors); p++)
	{
		preds[p] = book->predictors[p];
	}

	while(_len)
	{
		index = (*in >> 4) & 0xf;
		pred = (*in & 0xf);
		_len--;

		signed short * pred1 = &preds[ pred * 16] ;

		decode_8(++in, out, index, pred1, lastsmp);
		in+=4;	_len-=4;	out+=8;

		decode_8(in, out, index, pred1, lastsmp);
		in+=4;	_len-=4;	out+=8;

		samples += 16;
	}

	free(preds);

	return samples;
}

signed short* CN64AIFCAudio::determineBestPredictors(signed short* inPCMSamples, int numberSamplesIn)
{
	// todo find some way to calculate this
	signed short* predictors = new signed short[16];
	predictors[0] = 0xFAE2;
	predictors[1] = 0xFAD0;
	predictors[2] = 0xFE04;
	predictors[3] = 0x014F;
	predictors[4] = 0x0298;
	predictors[5] = 0x01CB;
	predictors[6] = 0x0029;
	predictors[7] = 0xFF03;
	predictors[8] = 0x081C;
	predictors[9] = 0x031A;
	predictors[10] = 0xFDF5;
	predictors[11] = 0xFBF2;
	predictors[12] = 0xFD32;
	predictors[13] = 0xFFC1;
	predictors[14] = 0x018B;
	predictors[15] = 0x01B9;

	return predictors;
}

int CN64AIFCAudio::determineBestEncodeIndex(signed short* pred1, signed short* pred2, signed short* lastSampleSet, signed short* inPCMSamples)
{
	signed short* tempSampleSet = new signed short[8];
	signed short* tmp = new signed short[8];

	int index = 0;
	float bestFitIndex = 99999999999.0f;

	for (int testIndex = 0; testIndex < 16; testIndex++)
	{
		for (int x = 0; x < 8; x++)
		{
			tempSampleSet[x] = lastSampleSet[x];
		}

		float tempFitIndex = 0;
		for (int r = 0; r < 2; r++)
		{
			for (int i = 0; i < 8; i++)
			{
				signed short sample = inPCMSamples[(r*8)+i];

				signed long total = pred1[i] * tempSampleSet[6];
				total += (pred2[i] * tempSampleSet[7]);

				if (i>0)
				{
					for(int x=i-1; x>-1; x--)
					{
						total += ( tmp[((i-1)-x)] * pred2[x] );
					}
				}

				float bestFit = 9999999999;
				int bestMatch = 0;

				for (int x = 0; x < 16; x++)
				{
					signed short newValue = ((((itable[x] << testIndex) << 0xB) + total) >> 0xB);
					if ((fabs((float)(sample - newValue))) < bestFit)
					{
						bestFit = (fabs((float)(sample - newValue)));
						bestMatch = x;
					}
				}

				tmp[i] = (bestMatch << testIndex);
				tempFitIndex += bestFit;
			}

			
			for (int x = 0; x < 8; x++)
			{
				tempSampleSet[x] = inPCMSamples[(r*8)+x];
			}
		}

		if (tempFitIndex < bestFitIndex)
		{
			bestFitIndex = tempFitIndex;
			index = testIndex;
		}
	}

	delete [] tmp;
	delete [] tempSampleSet;

	return index;
}

void CN64AIFCAudio::encode(signed short* inPCMSamples, int numberSamplesIn, unsigned char* outVADPCM, unsigned long& lenOut, ALADPCMBook *book)
{
	signed short* lastSampleSet = new signed short[8];
	for (int x = 0; x < 8; x++)
		lastSampleSet[x] = 0x0;

	signed short* tmp = new signed short[8];

	signed short* pred1 = &book->predictors[0];
	signed short* pred2 = &book->predictors[8];

	lenOut = 0;

	for (int y = 0; y < numberSamplesIn; y += 16)
	{
		int index = determineBestEncodeIndex(pred1, pred2, lastSampleSet, &inPCMSamples[y]);
		int predictor = 0;

		outVADPCM[lenOut++] = ((index << 4) | predictor);

		for (int r = 0; r < 2; r++)
		{
			for (int i = 0; i < 8; i++)
			{
				signed short sample = inPCMSamples[y+(r*8)+i];

				signed long total = pred1[i] * lastSampleSet[6];
				total += (pred2[i] * lastSampleSet[7]);

				if (i>0)
				{
					for(int x=i-1; x>-1; x--)
					{
						total += ( tmp[((i-1)-x)] * pred2[x] );
					}
				}

				float bestFit = 9999999999;
				int bestMatch = 0;

				for (int x = 0; x < 16; x++)
				{
					signed short newValue = ((((itable[x] << index) << 0xB) + total) >> 0xB);
					if ((fabs((float)(sample - newValue))) < bestFit)
					{
						bestFit = (fabs((float)(sample - newValue)));
						bestMatch = x;
					}
				}

				tmp[i] = (bestMatch << index);

				if ((i % 2) == 0)
					outVADPCM[lenOut] = (bestMatch << 4);
				else
				{
					outVADPCM[lenOut] = (outVADPCM[lenOut] | bestMatch);
					lenOut++;
				}
			}

			for (int x = 0; x < 8; x++)
			{
				lastSampleSet[x] = inPCMSamples[y+(r*8)+x];
			}
		}
	}

	if ((numberSamplesIn % 16) != 0)
	{
		// just cut it off for now
	}

	delete [] lastSampleSet;
	delete [] tmp;
}