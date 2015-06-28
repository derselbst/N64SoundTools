	#pragma once

#include "StdAfx.h"
#include "N64AIFCAudio.h"
#include <math.h>

float CN64AIFCAudio::keyTable[0xFF];

CN64AIFCAudio::CN64AIFCAudio(void)
{
	InitializeKeyTable();
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

bool CN64AIFCAudio::ExtractPercussion(ALBank* alBank, int instrument, int sound, CString outputFile, unsigned long samplingRate, bool ignoreKeyBase, bool halfSamplingRate)
{
	if (alBank->inst[instrument]->countPercussion > 0)
	{
		float samplingRateFloat = (float)samplingRate;

		if (!ignoreKeyBase)
		{
			if ((alBank->soundBankFormat == SUPERMARIO64FORMAT)
							|| (alBank->soundBankFormat == MARIOKART64FORMAT) 
							|| (alBank->soundBankFormat == ZELDAFORMAT)
							|| (alBank->soundBankFormat == STARFOX64FORMAT))
			{
				//samplingRateFloat = samplingRateFloat / (*reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->unknown3 == 0) ? 1 : *reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->unknown3);
			}
		}

		if (halfSamplingRate)
		{
			samplingRateFloat = samplingRateFloat / 2;
		}


		if (alBank->inst[instrument]->alPercussion[sound].wav.type == AL_RAW16_WAVE)
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
			unsigned long chunkSize = 0x28 + (alBank->inst[instrument]->alPercussion[sound].wav.len-2) + 0x44 - 0x8;
			wavHeader[0x4] = ((chunkSize >> 0) & 0xFF);
			wavHeader[0x5] = ((chunkSize >> 8) & 0xFF);
			wavHeader[0x6] = ((chunkSize >> 16) & 0xFF);
			wavHeader[0x7] = ((chunkSize >> 24) & 0xFF);
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
			wavHeader[0x18] = (((unsigned long)samplingRateFloat >> 0) & 0xFF);
			wavHeader[0x19] = (((unsigned long)samplingRateFloat >> 8) & 0xFF);
			wavHeader[0x1A] = (((unsigned long)samplingRateFloat >> 16) & 0xFF);
			wavHeader[0x1B] = (((unsigned long)samplingRateFloat >> 24) & 0xFF);
			wavHeader[0x1C] = ((((unsigned long)samplingRateFloat * 2) >> 0) & 0xFF);
			wavHeader[0x1D] = ((((unsigned long)samplingRateFloat * 2) >> 8) & 0xFF);
			wavHeader[0x1E] = ((((unsigned long)samplingRateFloat * 2) >> 16) & 0xFF);
			wavHeader[0x1F] = ((((unsigned long)samplingRateFloat * 2) >> 24) & 0xFF);
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

			unsigned long length = (alBank->inst[instrument]->alPercussion[sound].wav.len-2);
			fwrite(&length, 1, 4, outFileTempRaw);

			fwrite(&alBank->inst[instrument]->alPercussion[sound].wav.wavData[1], 1, (alBank->inst[instrument]->alPercussion[sound].wav.len-2), outFileTempRaw);

			
			if (alBank->inst[instrument]->alPercussion[sound].wav.rawWave->loop != NULL)
			{
				unsigned char* wavHeader = new unsigned char[0x44];
				for (int x = 0; x < 0x44; x++)
					wavHeader[x] = 0x00;

				wavHeader[0x0] = 0x73;
				wavHeader[0x1] = 0x6D;
				wavHeader[0x2] = 0x70;
				wavHeader[0x3] = 0x6C;
				wavHeader[0x4] = 0x3C;
				wavHeader[0x5] = 0x00;
				wavHeader[0x6] = 0x00;
				wavHeader[0x7] = 0x00;
				
				if (alBank->inst[instrument]->sounds[sound]->key.keybase != 0)
					wavHeader[0x14] = alBank->inst[instrument]->sounds[sound]->key.keybase;//0x3C;
				else
					wavHeader[0x14] = 0x3C;
				wavHeader[0x15] = 0x00;
				wavHeader[0x16] = 0x00;
				wavHeader[0x17] = 0x00;
				
				wavHeader[0x24] = 0x01;
				wavHeader[0x25] = 0x00;
				wavHeader[0x26] = 0x00;
				wavHeader[0x27] = 0x00;

				wavHeader[0x34] = ((alBank->inst[instrument]->alPercussion[sound].wav.rawWave->loop->start >> 0) & 0xFF);
				wavHeader[0x35] = ((alBank->inst[instrument]->alPercussion[sound].wav.rawWave->loop->start >> 8) & 0xFF);
				wavHeader[0x36] = ((alBank->inst[instrument]->alPercussion[sound].wav.rawWave->loop->start >> 16) & 0xFF);
				wavHeader[0x37] = ((alBank->inst[instrument]->alPercussion[sound].wav.rawWave->loop->start >> 24) & 0xFF);
				wavHeader[0x38] = (((alBank->inst[instrument]->alPercussion[sound].wav.rawWave->loop->end) >> 0) & 0xFF);
				wavHeader[0x39] = (((alBank->inst[instrument]->alPercussion[sound].wav.rawWave->loop->end) >> 8) & 0xFF);
				wavHeader[0x3A] = (((alBank->inst[instrument]->alPercussion[sound].wav.rawWave->loop->end) >> 16) & 0xFF);
				wavHeader[0x3B] = (((alBank->inst[instrument]->alPercussion[sound].wav.rawWave->loop->end) >> 24) & 0xFF);

				fwrite(wavHeader, 1, 0x44, outFileTempRaw );

				delete [] wavHeader;
			}
			else
			{
				unsigned char* wavHeader = new unsigned char[0x44];
				for (int x = 0; x < 0x44; x++)
					wavHeader[x] = 0x00;

				wavHeader[0x0] = 0x73;
				wavHeader[0x1] = 0x6D;
				wavHeader[0x2] = 0x70;
				wavHeader[0x3] = 0x6C;
				wavHeader[0x4] = 0x3C;
				wavHeader[0x5] = 0x00;
				wavHeader[0x6] = 0x00;
				wavHeader[0x7] = 0x00;
				
				if (alBank->inst[instrument]->sounds[sound]->key.keybase != 0)
					wavHeader[0x14] = alBank->inst[instrument]->sounds[sound]->key.keybase;//0x3C;
				else
					wavHeader[0x14] = 0x3C;
				wavHeader[0x15] = 0x00;
				wavHeader[0x16] = 0x00;
				wavHeader[0x17] = 0x00;

				fwrite(wavHeader, 1, 0x44, outFileTempRaw );

				delete [] wavHeader;
			}


			fclose(outFileTempRaw);

			return true;
		}
		else
		{
			// hack for mario kart
			if (alBank->inst[instrument]->alPercussion[sound].wav.adpcmWave->book == NULL)
				return false;
			signed short* outRawData = new signed short[alBank->inst[instrument]->alPercussion[sound].wav.len * 4];
			int nSamples = decode(alBank->inst[instrument]->alPercussion[sound].wav.wavData, outRawData, alBank->inst[instrument]->alPercussion[sound].wav.len, alBank->inst[instrument]->alPercussion[sound].wav.adpcmWave->book);
			
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
			unsigned long chunkSize = 0x28 + (nSamples * 2) + 0x44 - 0x8;
			wavHeader[0x4] = ((chunkSize >> 0) & 0xFF);
			wavHeader[0x5] = ((chunkSize >> 8) & 0xFF);
			wavHeader[0x6] = ((chunkSize >> 16) & 0xFF);
			wavHeader[0x7] = ((chunkSize >> 24) & 0xFF);
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
			wavHeader[0x1C] = ((((unsigned long)samplingRateFloat * 2) >> 0) & 0xFF);
			wavHeader[0x1D] = ((((unsigned long)samplingRateFloat * 2) >> 8) & 0xFF);
			wavHeader[0x1E] = ((((unsigned long)samplingRateFloat * 2) >> 16) & 0xFF);
			wavHeader[0x1F] = ((((unsigned long)samplingRateFloat * 2) >> 24) & 0xFF);
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

			if (alBank->inst[instrument]->alPercussion[sound].wav.adpcmWave->loop != NULL)
			{
				unsigned char* wavHeader = new unsigned char[0x44];
				for (int x = 0; x < 0x44; x++)
					wavHeader[x] = 0x00;

				wavHeader[0x0] = 0x73;
				wavHeader[0x1] = 0x6D;
				wavHeader[0x2] = 0x70;
				wavHeader[0x3] = 0x6C;
				wavHeader[0x4] = 0x3C;
				wavHeader[0x5] = 0x00;
				wavHeader[0x6] = 0x00;
				wavHeader[0x7] = 0x00;
				
				wavHeader[0x14] = 0x3C;
				wavHeader[0x15] = 0x00;
				wavHeader[0x16] = 0x00;
				wavHeader[0x17] = 0x00;
				
				wavHeader[0x24] = 0x01;
				wavHeader[0x25] = 0x00;
				wavHeader[0x26] = 0x00;
				wavHeader[0x27] = 0x00;

				wavHeader[0x34] = ((alBank->inst[instrument]->alPercussion[sound].wav.adpcmWave->loop->start >> 0) & 0xFF);
				wavHeader[0x35] = ((alBank->inst[instrument]->alPercussion[sound].wav.adpcmWave->loop->start >> 8) & 0xFF);
				wavHeader[0x36] = ((alBank->inst[instrument]->alPercussion[sound].wav.adpcmWave->loop->start >> 16) & 0xFF);
				wavHeader[0x37] = ((alBank->inst[instrument]->alPercussion[sound].wav.adpcmWave->loop->start >> 24) & 0xFF);
				wavHeader[0x38] = (((alBank->inst[instrument]->alPercussion[sound].wav.adpcmWave->loop->end) >> 0) & 0xFF);
				wavHeader[0x39] = (((alBank->inst[instrument]->alPercussion[sound].wav.adpcmWave->loop->end) >> 8) & 0xFF);
				wavHeader[0x3A] = (((alBank->inst[instrument]->alPercussion[sound].wav.adpcmWave->loop->end) >> 16) & 0xFF);
				wavHeader[0x3B] = (((alBank->inst[instrument]->alPercussion[sound].wav.adpcmWave->loop->end) >> 24) & 0xFF);

				fwrite(wavHeader, 1, 0x44, outFileTempRaw );

				delete [] wavHeader;
			}
			else
			{
				unsigned char* wavHeader = new unsigned char[0x44];
				for (int x = 0; x < 0x44; x++)
					wavHeader[x] = 0x00;

				wavHeader[0x0] = 0x73;
				wavHeader[0x1] = 0x6D;
				wavHeader[0x2] = 0x70;
				wavHeader[0x3] = 0x6C;
				wavHeader[0x4] = 0x3C;
				wavHeader[0x5] = 0x00;
				wavHeader[0x6] = 0x00;
				wavHeader[0x7] = 0x00;
				
				wavHeader[0x14] = 0x3C;
				wavHeader[0x15] = 0x00;
				wavHeader[0x16] = 0x00;
				wavHeader[0x17] = 0x00;

				fwrite(wavHeader, 1, 0x44, outFileTempRaw );

				delete [] wavHeader;
			}

			fclose(outFileTempRaw);

			delete [] outRawData;
			return true;
		}
	}
	return false;
}

bool CN64AIFCAudio::ExtractRawSound(ALBank* alBank, int instrument, int sound, CString outputFile, unsigned long samplingRate, byte primSel, bool ignoreKeyBase, bool halfSamplingRate)
{

	float samplingRateFloat = (float)samplingRate;

	if (alBank->inst[instrument]->soundCount > 0)
	{
		if (alBank->inst[instrument]->sounds[sound] != NULL)
		{
			ALWave* alWave = NULL;
			if (primSel == PRIMARY)
			{
				alWave = &alBank->inst[instrument]->sounds[sound]->wav;
			}
			else if (primSel == PREVIOUS)
			{
				alWave = &alBank->inst[instrument]->sounds[sound]->wavPrevious;
			}
			else if (primSel == SECONDARY)
			{
				alWave = &alBank->inst[instrument]->sounds[sound]->wavSecondary;
			}

			if (!ignoreKeyBase)
			{
				if (
					(alBank->soundBankFormat == STANDARDFORMAT)
					|| (alBank->soundBankFormat == STANDARDRNCCOMPRESSED)
					|| (alBank->soundBankFormat == STANDARDRNXCOMPRESSED))
				{
					samplingRateFloat = samplingRateFloat / CN64AIFCAudio::keyTable[alBank->inst[instrument]->sounds[sound]->key.keybase];
				}
				else if ((alBank->soundBankFormat == SUPERMARIO64FORMAT)
					|| (alBank->soundBankFormat == MARIOKART64FORMAT) 
					|| (alBank->soundBankFormat == ZELDAFORMAT)
					|| (alBank->soundBankFormat == STARFOX64FORMAT))
				{
					samplingRateFloat = samplingRateFloat / (((*reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->unknown3) - 0.0) < 0.00001) ? 1.0f : *reinterpret_cast<float*> (&alBank->inst[instrument]->sounds[sound]->unknown3));
				}
			}

			if (halfSamplingRate)
			{
				samplingRateFloat = samplingRateFloat / 2;
			}

			if (alWave->type == AL_RAW16_WAVE)
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
				unsigned long chunkSize = 0x28 + (alWave->len-2) + 0x44 - 0x8;
				wavHeader[0x4] = ((chunkSize >> 0) & 0xFF);
				wavHeader[0x5] = ((chunkSize >> 8) & 0xFF);
				wavHeader[0x6] = ((chunkSize >> 16) & 0xFF);
				wavHeader[0x7] = ((chunkSize >> 24) & 0xFF);
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
				wavHeader[0x18] = (((unsigned long)samplingRateFloat >> 0) & 0xFF);
				wavHeader[0x19] = (((unsigned long)samplingRateFloat >> 8) & 0xFF);
				wavHeader[0x1A] = (((unsigned long)samplingRateFloat >> 16) & 0xFF);
				wavHeader[0x1B] = (((unsigned long)samplingRateFloat >> 24) & 0xFF);
				wavHeader[0x1C] = ((((unsigned long)samplingRateFloat * 2) >> 0) & 0xFF);
				wavHeader[0x1D] = ((((unsigned long)samplingRateFloat * 2) >> 8) & 0xFF);
				wavHeader[0x1E] = ((((unsigned long)samplingRateFloat * 2) >> 16) & 0xFF);
				wavHeader[0x1F] = ((((unsigned long)samplingRateFloat * 2) >> 24) & 0xFF);
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

				unsigned long length = (alWave->len-2);
				fwrite(&length, 1, 4, outFileTempRaw);

				fwrite(&alWave->wavData[1], 1, (alWave->len-2), outFileTempRaw);

				
				if (alWave->rawWave->loop != NULL)
				{
					unsigned char* wavHeader = new unsigned char[0x44];
					for (int x = 0; x < 0x44; x++)
						wavHeader[x] = 0x00;

					wavHeader[0x0] = 0x73;
					wavHeader[0x1] = 0x6D;
					wavHeader[0x2] = 0x70;
					wavHeader[0x3] = 0x6C;
					wavHeader[0x4] = 0x3C;
					wavHeader[0x5] = 0x00;
					wavHeader[0x6] = 0x00;
					wavHeader[0x7] = 0x00;
					
					if (alBank->inst[instrument]->sounds[sound]->key.keybase != 0)
						wavHeader[0x14] = alBank->inst[instrument]->sounds[sound]->key.keybase;//0x3C;
					else
						wavHeader[0x14] = 0x3C;
					wavHeader[0x15] = 0x00;
					wavHeader[0x16] = 0x00;
					wavHeader[0x17] = 0x00;
					
					wavHeader[0x24] = 0x01;
					wavHeader[0x25] = 0x00;
					wavHeader[0x26] = 0x00;
					wavHeader[0x27] = 0x00;

					wavHeader[0x34] = ((alWave->rawWave->loop->start >> 0) & 0xFF);
					wavHeader[0x35] = ((alWave->rawWave->loop->start >> 8) & 0xFF);
					wavHeader[0x36] = ((alWave->rawWave->loop->start >> 16) & 0xFF);
					wavHeader[0x37] = ((alWave->rawWave->loop->start >> 24) & 0xFF);
					wavHeader[0x38] = (((alWave->rawWave->loop->end) >> 0) & 0xFF);
					wavHeader[0x39] = (((alWave->rawWave->loop->end) >> 8) & 0xFF);
					wavHeader[0x3A] = (((alWave->rawWave->loop->end) >> 16) & 0xFF);
					wavHeader[0x3B] = (((alWave->rawWave->loop->end) >> 24) & 0xFF);

					if (alWave->rawWave->loop->count == 0xFFFFFFFF)
					{
						wavHeader[0x40] = 0x00;
						wavHeader[0x41] = 0x00;
						wavHeader[0x42] = 0x00;
						wavHeader[0x43] = 0x00;
					}
					else
					{
						wavHeader[0x40] = (((alWave->rawWave->loop->count) >> 0) & 0xFF);
						wavHeader[0x41] = (((alWave->rawWave->loop->count) >> 8) & 0xFF);
						wavHeader[0x42] = (((alWave->rawWave->loop->count) >> 16) & 0xFF);
						wavHeader[0x43] = (((alWave->rawWave->loop->count) >> 24) & 0xFF);
					}

					fwrite(wavHeader, 1, 0x44, outFileTempRaw );

					delete [] wavHeader;
				}
				else
				{
					unsigned char* wavHeader = new unsigned char[0x44];
					for (int x = 0; x < 0x44; x++)
						wavHeader[x] = 0x00;

					wavHeader[0x0] = 0x73;
					wavHeader[0x1] = 0x6D;
					wavHeader[0x2] = 0x70;
					wavHeader[0x3] = 0x6C;
					wavHeader[0x4] = 0x3C;
					wavHeader[0x5] = 0x00;
					wavHeader[0x6] = 0x00;
					wavHeader[0x7] = 0x00;
					
					if (alBank->inst[instrument]->sounds[sound]->key.keybase != 0)
						wavHeader[0x14] = alBank->inst[instrument]->sounds[sound]->key.keybase;//0x3C;
					else
						wavHeader[0x14] = 0x3C;
					wavHeader[0x15] = 0x00;
					wavHeader[0x16] = 0x00;
					wavHeader[0x17] = 0x00;

					fwrite(wavHeader, 1, 0x44, outFileTempRaw );

					delete [] wavHeader;
				}


				fclose(outFileTempRaw);
			}
			else
			{
				if ((alWave->adpcmWave == NULL) || (alWave->adpcmWave->book == NULL))
					return false;

				signed short* outRawData = new signed short[alWave->len * 4];
				int nSamples = decode(alWave->wavData, outRawData, alWave->len, alWave->adpcmWave->book);
				
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
				unsigned long chunkSize = 0x28 + (nSamples * 2) + 0x44 - 0x8;
				wavHeader[0x4] = ((chunkSize >> 0) & 0xFF);
				wavHeader[0x5] = ((chunkSize >> 8) & 0xFF);
				wavHeader[0x6] = ((chunkSize >> 16) & 0xFF);
				wavHeader[0x7] = ((chunkSize >> 24) & 0xFF);
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
				wavHeader[0x18] = (((unsigned long)samplingRateFloat >> 0) & 0xFF);
				wavHeader[0x19] = (((unsigned long)samplingRateFloat >> 8) & 0xFF);
				wavHeader[0x1A] = (((unsigned long)samplingRateFloat >> 16) & 0xFF);
				wavHeader[0x1B] = (((unsigned long)samplingRateFloat >> 24) & 0xFF);
				wavHeader[0x1C] = ((((unsigned long)samplingRateFloat * 2) >> 0) & 0xFF);
				wavHeader[0x1D] = ((((unsigned long)samplingRateFloat * 2) >> 8) & 0xFF);
				wavHeader[0x1E] = ((((unsigned long)samplingRateFloat * 2) >> 16) & 0xFF);
				wavHeader[0x1F] = ((((unsigned long)samplingRateFloat * 2) >> 24) & 0xFF);
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

				if (alWave->adpcmWave->loop != NULL)
				{
					unsigned char* wavHeader = new unsigned char[0x44];
					for (int x = 0; x < 0x44; x++)
						wavHeader[x] = 0x00;

					wavHeader[0x0] = 0x73;
					wavHeader[0x1] = 0x6D;
					wavHeader[0x2] = 0x70;
					wavHeader[0x3] = 0x6C;
					wavHeader[0x4] = 0x3C;
					wavHeader[0x5] = 0x00;
					wavHeader[0x6] = 0x00;
					wavHeader[0x7] = 0x00;
					
					if (alBank->inst[instrument]->sounds[sound]->key.keybase != 0)
						wavHeader[0x14] = alBank->inst[instrument]->sounds[sound]->key.keybase;//0x3C;
					else
						wavHeader[0x14] = 0x3C;
					wavHeader[0x15] = 0x00;
					wavHeader[0x16] = 0x00;
					wavHeader[0x17] = 0x00;
					
					wavHeader[0x24] = 0x01;
					wavHeader[0x25] = 0x00;
					wavHeader[0x26] = 0x00;
					wavHeader[0x27] = 0x00;

					wavHeader[0x34] = ((alWave->adpcmWave->loop->start >> 0) & 0xFF);
					wavHeader[0x35] = ((alWave->adpcmWave->loop->start >> 8) & 0xFF);
					wavHeader[0x36] = ((alWave->adpcmWave->loop->start >> 16) & 0xFF);
					wavHeader[0x37] = ((alWave->adpcmWave->loop->start >> 24) & 0xFF);
					wavHeader[0x38] = (((alWave->adpcmWave->loop->end) >> 0) & 0xFF);
					wavHeader[0x39] = (((alWave->adpcmWave->loop->end) >> 8) & 0xFF);
					wavHeader[0x3A] = (((alWave->adpcmWave->loop->end) >> 16) & 0xFF);
					wavHeader[0x3B] = (((alWave->adpcmWave->loop->end) >> 24) & 0xFF);

					if (alWave->adpcmWave->loop->count == 0xFFFFFFFF)
					{
						wavHeader[0x40] = 0x00;
						wavHeader[0x41] = 0x00;
						wavHeader[0x42] = 0x00;
						wavHeader[0x43] = 0x00;
					}
					else
					{
						wavHeader[0x40] = (((alWave->adpcmWave->loop->count) >> 0) & 0xFF);
						wavHeader[0x41] = (((alWave->adpcmWave->loop->count) >> 8) & 0xFF);
						wavHeader[0x42] = (((alWave->adpcmWave->loop->count) >> 16) & 0xFF);
						wavHeader[0x43] = (((alWave->adpcmWave->loop->count) >> 24) & 0xFF);
					}


					fwrite(wavHeader, 1, 0x44, outFileTempRaw );

					delete [] wavHeader;
				}
				else
				{
					unsigned char* wavHeader = new unsigned char[0x44];
					for (int x = 0; x < 0x44; x++)
						wavHeader[x] = 0x00;

					wavHeader[0x0] = 0x73;
					wavHeader[0x1] = 0x6D;
					wavHeader[0x2] = 0x70;
					wavHeader[0x3] = 0x6C;
					wavHeader[0x4] = 0x3C;
					wavHeader[0x5] = 0x00;
					wavHeader[0x6] = 0x00;
					wavHeader[0x7] = 0x00;
					
					if (alBank->inst[instrument]->sounds[sound]->key.keybase != 0)
						wavHeader[0x14] = alBank->inst[instrument]->sounds[sound]->key.keybase;//0x3C;
					else
						wavHeader[0x14] = 0x3C;
					wavHeader[0x15] = 0x00;
					wavHeader[0x16] = 0x00;
					wavHeader[0x17] = 0x00;

					fwrite(wavHeader, 1, 0x44, outFileTempRaw );

					delete [] wavHeader;
				}

				fclose(outFileTempRaw);

				delete [] outRawData;
			}

			return true;

		}
	}
	return false;
}

bool CN64AIFCAudio::AddSound(ALBank*& alBank, int instrument, CString rawWavFileName, unsigned long& samplingRate, bool type)
{
	AddSound(alBank, instrument);

	bool replace = ReplaceSoundWithWavData(alBank, instrument, (alBank->inst[instrument]->soundCount-1), rawWavFileName, samplingRate, type, PRIMARY);
	if (replace)
	{
		UpdateAudioOffsets(alBank);
	}
	return replace;
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
	if ((alBank->soundBankFormat != STANDARDFORMAT) && (alBank->soundBankFormat != SUPERMARIO64FORMAT))
	{
		::MessageBox(NULL, "Cannot delete sound in this format", "Cannot delete", NULL);
		return;
	}
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

	UpdateAudioOffsets(alBank);
}
void CN64AIFCAudio::MoveUpSound(ALBank*& alBank, int instrument, int sound)
{
	if (sound == 0)
		return;

	ALSound* soundAl = alBank->inst[instrument]->sounds[sound-1];
	alBank->inst[instrument]->sounds[sound-1] = alBank->inst[instrument]->sounds[sound];
	alBank->inst[instrument]->sounds[sound] = soundAl;

	UpdateAudioOffsets(alBank);
}

void CN64AIFCAudio::ExportPredictors(ALBank*& alBank, int instrument, int sound, CString fileName, byte primSel)
{
	FILE* outFile = fopen(fileName, "wb");
	if (outFile == NULL)
	{
		MessageBox(NULL, "Error opening file", "Error", NULL);
		return;
	}

	ALWave* alWave = NULL;
	if (primSel == PRIMARY)
	{
		alWave = &alBank->inst[instrument]->sounds[sound]->wav;
	}
	else if (primSel == PREVIOUS)
	{
		alWave = &alBank->inst[instrument]->sounds[sound]->wavPrevious;
	}
	else if (primSel == SECONDARY)
	{
		alWave = &alBank->inst[instrument]->sounds[sound]->wavSecondary;
	}

	unsigned long tempLong = Flip32Bit(alWave->adpcmWave->book->order);
	fwrite(&tempLong, 1, 4, outFile);

	tempLong = Flip32Bit(alWave->adpcmWave->book->npredictors);
	fwrite(&tempLong, 1, 4, outFile);

	for (int z = 0; z < (alWave->adpcmWave->book->order * alWave->adpcmWave->book->npredictors * 8); z++)
	{
		unsigned short tempShort = Flip16Bit(alWave->adpcmWave->book->predictors[z]);
		fwrite(&tempShort, 1, 2, outFile);
	}

	fclose(outFile);
}

void CN64AIFCAudio::ImportPredictors(ALBank*& alBank, int instrument, int sound, CString fileName, byte primSel)
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
		fclose(inFile);
		return;
	}


	unsigned char* buffer = new unsigned char[fileSize];
	fread(buffer, 1, fileSize, inFile);
	fclose(inFile);
	
	ALWave* alWave = NULL;
	if (primSel == PRIMARY)
	{
		alWave = &alBank->inst[instrument]->sounds[sound]->wav;
	}
	else if (primSel == PREVIOUS)
	{
		alWave = &alBank->inst[instrument]->sounds[sound]->wavPrevious;
	}
	else if (primSel == SECONDARY)
	{
		alWave = &alBank->inst[instrument]->sounds[sound]->wavSecondary;
	}

	alWave->adpcmWave->book->order = CharArrayToLong(&buffer[0]);
	alWave->adpcmWave->book->npredictors = CharArrayToLong(&buffer[4]);

	if (fileSize != (8 + ((alWave->adpcmWave->book->order * alWave->adpcmWave->book->npredictors * 8) * 2)))
	{
		delete [] buffer;
		MessageBox(NULL, "Error invalid file size", "Error", NULL);
		return;
	}

	delete [] alWave->adpcmWave->book->predictors;
	alWave->adpcmWave->book->predictors = new signed short[(alWave->adpcmWave->book->order * alWave->adpcmWave->book->npredictors * 8)];
	
	

	for (int z = 0; z < (alWave->adpcmWave->book->order * alWave->adpcmWave->book->npredictors * 8); z++)
	{
		alWave->adpcmWave->book->predictors[z] = CharArrayToShort(&buffer[0x8 + (z * 2)]);
	}

	delete [] buffer;

	UpdateAudioOffsets(alBank);
}

void CN64AIFCAudio::UpdateAudioOffsets(ALBank*& alBank)
{
	unsigned char* ctl = NULL;
	unsigned char* tbl = NULL;
	int ctlCounter, tblCounter;
	// update base offsets
	if (alBank->soundBankFormat == STANDARDFORMAT)
	{
		WriteAudio(alBank, ctl, ctlCounter, tbl, tblCounter);
	}
	else if (alBank->soundBankFormat == SUPERMARIO64FORMAT)
	{
		WriteAudioSuperMario(alBank, ctl, ctlCounter, tbl, tblCounter);
	}
	else if (alBank->soundBankFormat == N64PTRWAVETABLETABLEV2)
	{
		WriteAudioN64PtrWavetableV2(alBank, ctl, ctlCounter, tbl, tblCounter);
	}
	else if (alBank->soundBankFormat == N64PTRWAVETABLETABLEV1)
	{
		WriteAudioN64PtrWavetableV1(alBank, ctl, ctlCounter, tbl, tblCounter);
	}
	if (ctl != NULL)
		delete [] ctl;
	if (tbl != NULL)
		delete [] tbl;
}

void CN64AIFCAudio::ExportRawData(ALBank*& alBank, int instrument, int sound, CString fileName, byte primSel)
{
	FILE* outFile = fopen(fileName, "wb");
	if (outFile == NULL)
	{
		MessageBox(NULL, "Error opening file", "Error", NULL);
		return;
	}

	ALWave* alWave = NULL;
	if (primSel == PRIMARY)
	{
		alWave = &alBank->inst[instrument]->sounds[sound]->wav;
	}
	else if (primSel == PREVIOUS)
	{
		alWave = &alBank->inst[instrument]->sounds[sound]->wavPrevious;
	}
	else if (primSel == SECONDARY)
	{
		alWave = &alBank->inst[instrument]->sounds[sound]->wavSecondary;
	}

	fwrite(alWave->wavData, 1, alWave->len, outFile);

	fclose(outFile);
}

void CN64AIFCAudio::ImportRawData(ALBank*& alBank, int instrument, int sound, CString fileName, byte primSel)
{
	int fileSize = GetSizeFile(fileName);
	FILE* inFile = fopen(fileName, "rb");
	if (inFile == NULL)
	{
		MessageBox(NULL, "Error opening", "Error", NULL);
		return;
	}

	ALWave* alWave = NULL;
	if (primSel == PRIMARY)
	{
		alWave = &alBank->inst[instrument]->sounds[sound]->wav;
	}
	else if (primSel == PREVIOUS)
	{
		alWave = &alBank->inst[instrument]->sounds[sound]->wavPrevious;
	}
	else if (primSel == SECONDARY)
	{
		alWave = &alBank->inst[instrument]->sounds[sound]->wavSecondary;
	}

	alWave->len = fileSize;
	delete [] alWave->wavData;

	alWave->wavData = new unsigned char[fileSize];
	fread(alWave->wavData, 1, fileSize, inFile);
	fclose(inFile);

	if (alBank->soundBankFormat == SUPERMARIO64FORMAT)
	{
		alWave->adpcmWave->loop->start = 0;
		alWave->adpcmWave->loop->end = (fileSize * 7) / 4;
		alWave->adpcmWave->loop->count = 0;
	}

	UpdateAudioOffsets(alBank);
}

void CN64AIFCAudio::MoveDownSound(ALBank*& alBank, int instrument, int sound)
{
	if (sound == (alBank->inst[instrument]->soundCount-1))
		return;

	ALSound* soundAl = alBank->inst[instrument]->sounds[sound+1];
	alBank->inst[instrument]->sounds[sound+1] = alBank->inst[instrument]->sounds[sound];
	alBank->inst[instrument]->sounds[sound] = soundAl;

	UpdateAudioOffsets(alBank);
}

bool CN64AIFCAudio::ReadWavData(CString rawWavFileName, unsigned char*& rawData, int& rawLength, unsigned long& samplingRate, bool& hasLoopData, unsigned char& keyBase, unsigned long& loopStart, unsigned long& loopEnd, unsigned long& loopCount)
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

	bool returnFlag = false;

	while (!endFlag)
	{
		if (currentOffset >= (fileSize - 8))
			break;

		unsigned long sectionType = ((((((wavData[currentOffset] << 8) | wavData[currentOffset + 1]) << 8) | wavData[currentOffset + 2]) << 8) | wavData[currentOffset + 3]);

		if (sectionType == 0x666D7420) // fmt
		{
			unsigned long chunkSize = ((((((wavData[currentOffset + 0x7] << 8) | wavData[currentOffset + 0x6]) << 8) | wavData[currentOffset + 0x5]) << 8) | wavData[currentOffset + 0x4]);

			channels = ((wavData[currentOffset + 0xB] << 8) | wavData[currentOffset + 0xA]);

			if (channels != 0x0001)
			{
				MessageBox(NULL, "Warning: Only mono wav supported", "Error", NULL);
				endFlag = true;
				returnFlag = false;
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
				endFlag = true;
				returnFlag = false;
			}

			if (bitRate == 0x0010)
			{
				rawData = new unsigned char[rawLength];
				for (int x = 0; x < rawLength; x++)
				{
					rawData[x] = wavData[currentOffset + 0x8 + x];
				}
			
				returnFlag = true;
			}
			else
			{
				MessageBox(NULL, "Error only 16-bit PCM wav supported", "Error", NULL);
				endFlag = true;
				returnFlag = false;
			}

			currentOffset += rawLength + 8;
		}
		else if (sectionType == 0x736D706C) // smpl
		{
			unsigned long chunkSize = ((((((wavData[currentOffset + 0x7] << 8) | wavData[currentOffset + 0x6]) << 8) | wavData[currentOffset + 0x5]) << 8) | wavData[currentOffset + 0x4]);

			unsigned long numSampleBlocks = Flip32Bit(CharArrayToLong(&wavData[currentOffset+0x24]));
			if (numSampleBlocks > 0)
			{
				hasLoopData = true;

				keyBase = CharArrayToLong(&wavData[currentOffset+0x14]);
				loopStart = Flip32Bit(CharArrayToLong(&wavData[currentOffset+0x34]));
				loopEnd = Flip32Bit(CharArrayToLong(&wavData[currentOffset+0x38]));
				loopCount = Flip32Bit(CharArrayToLong(&wavData[currentOffset+0x40]));
				if (loopCount == 0)
					loopCount = 0xFFFFFFFF;
			}

			currentOffset += 8 + chunkSize;
		}
		else
		{
			unsigned long chunkSize = ((((((wavData[currentOffset + 0x7] << 8) | wavData[currentOffset + 0x6]) << 8) | wavData[currentOffset + 0x5]) << 8) | wavData[currentOffset + 0x4]);

			currentOffset += 8 + chunkSize;
		}
	}

	delete [] wavData;
	return returnFlag;
}

bool CN64AIFCAudio::ReplaceSoundWithWavData(ALBank*& alBank, int instrument, int sound, CString rawWavFileName, unsigned long& samplingRate, bool newType, byte primSel)
{
	unsigned char* wavPCMData;
	int rawLength;

	if (alBank->inst[instrument]->sounds[sound] == NULL)
		return false;

	bool hasLoopData = false;
	unsigned char keyBase = 0x3C;
	unsigned long loopStart = 0x00000000;
	unsigned long loopEnd = 0x00000000;
	unsigned long loopCount = 0x00000000;

	if (!ReadWavData(rawWavFileName, wavPCMData, rawLength, samplingRate, hasLoopData, keyBase, loopStart, loopEnd, loopCount))
	{
		return false;
	}

	ALWave* alWave = NULL;
	if (primSel == PRIMARY)
	{
		alWave = &alBank->inst[instrument]->sounds[sound]->wav;
	}
	else if (primSel == PREVIOUS)
	{
		alWave = &alBank->inst[instrument]->sounds[sound]->wavPrevious;
	}
	else if (primSel == SECONDARY)
	{
		alWave = &alBank->inst[instrument]->sounds[sound]->wavSecondary;
	}

	if (alWave->type == AL_ADPCM_WAVE)
	{
		if (alWave->adpcmWave->loop != NULL)
		{
			delete alWave->adpcmWave->loop;
			alWave->adpcmWave->loop = NULL;
		}

		if (alWave->adpcmWave->book != NULL)
		{
			if (alWave->adpcmWave->book->predictors != NULL)
			{
				delete [] alWave->adpcmWave->book->predictors;
				alWave->adpcmWave->book->predictors = NULL;
			}
			delete alWave->adpcmWave->book;
			alWave->adpcmWave->book = NULL;
		}

		delete alWave->adpcmWave->book;
		delete alWave->adpcmWave;
		alWave->adpcmWave = NULL;
		alWave->rawWave = NULL;
	}
	else
	{
		if (alWave->rawWave->loop != NULL)
		{
			delete alWave->rawWave->loop;
			alWave->rawWave->loop = NULL;
		}
		delete alWave->rawWave;
		alWave->rawWave = NULL;
		alWave->adpcmWave = NULL;
	}

	if (newType == AL_RAW16_WAVE)
	{
		alWave->type = AL_RAW16_WAVE;
		alWave->rawWave = new ALRAWWaveInfo();
		alWave->rawWave->loop = NULL;/*new ALRawLoop();
		alWave->rawWave->loop->start = 0;
		alWave->rawWave->loop->end = (rawLength-2);
		alWave->rawWave->loop->count = 0;*/

		delete [] alWave->wavData;
		alWave->wavData = new unsigned char[rawLength + 2];
		alWave->wavData[0] = 0x03;
		alWave->wavData[rawLength + 1] = 0x00;
		memcpy(&alWave->wavData[1], wavPCMData, rawLength);
		alWave->len = rawLength + 2;

		alBank->inst[instrument]->sounds[sound]->flags = 0;
		/*alBank->inst[instrument]->sounds[sound]->env.attackTime = 0;
		alBank->inst[instrument]->sounds[sound]->env.attackVolume = 0x7F7F;
		alBank->inst[instrument]->sounds[sound]->env.decayTime = 0xFFFFFFFF;
		alBank->inst[instrument]->sounds[sound]->env.decayVolume = 0;
		alBank->inst[instrument]->sounds[sound]->env.releaseTime = 0;
		
		alBank->inst[instrument]->sounds[sound]->key.detune = 0;
		if (hasLoopData)
			alBank->inst[instrument]->sounds[sound]->key.keybase = keyBase;
		else
			alBank->inst[instrument]->sounds[sound]->key.keybase = 0x3C;
		alBank->inst[instrument]->sounds[sound]->key.keymax = 0x7F;
		alBank->inst[instrument]->sounds[sound]->key.keymin = 0;
		alBank->inst[instrument]->sounds[sound]->key.velocitymax = 0x7F;
		alBank->inst[instrument]->sounds[sound]->key.velocitymin = 0;
		alBank->inst[instrument]->sounds[sound]->samplePan = 0x40;
		alBank->inst[instrument]->sounds[sound]->sampleVolume = 0x7F;*/
		alWave->flags = 0;

		if (hasLoopData)
		{
			alWave->rawWave->loop = new ALRawLoop();
			alWave->rawWave->loop->count = loopCount;
			alWave->rawWave->loop->start = loopStart;
			alWave->rawWave->loop->end = loopEnd;
		}
	}
	else
	{
		alWave->type = AL_ADPCM_WAVE;
		alWave->adpcmWave = new ALADPCMWaveInfo();
		alWave->adpcmWave->loop = NULL;/*new ALRawLoop();
		alWave->rawWave->loop->start = 0;
		alWave->rawWave->loop->end = (rawLength-2);
		alWave->rawWave->loop->count = 0;*/

		alWave->adpcmWave->book = new ALADPCMBook();

		int numberSamples = (rawLength / 2);
		signed short* pcmSamples = new signed short[numberSamples];

		for (int x = 0; x < numberSamples; x++)
		{
			pcmSamples[x] = (signed short)(((wavPCMData[x*2+1] << 8)) | wavPCMData[x*2]);
		}

		alWave->adpcmWave->book->predictors = determineBestPredictors(alBank, alWave->adpcmWave->book->npredictors, alWave->adpcmWave->book->order, pcmSamples, numberSamples);

		delete [] alWave->wavData;

		unsigned long vadpcmOutputLength;
		unsigned char* vadpcmData = new unsigned char[numberSamples];

		encode(pcmSamples, numberSamples, vadpcmData, vadpcmOutputLength, alWave->adpcmWave->book);

		alWave->wavData = new unsigned char[vadpcmOutputLength];
		memcpy(alWave->wavData, vadpcmData, vadpcmOutputLength);
		alWave->len = vadpcmOutputLength;

		if (alBank->soundBankFormat == SUPERMARIO64FORMAT)
		{
			alWave->adpcmWave->loop = new ALADPCMloop();
			alWave->adpcmWave->loop->start = 0;
			alWave->adpcmWave->loop->end = ((vadpcmOutputLength * 7) / 4);
			alWave->adpcmWave->loop->count = 0;
		}
		else
		{
			alBank->inst[instrument]->sounds[sound]->flags = 0;
			/*alBank->inst[instrument]->sounds[sound]->env.attackTime = 0;
			alBank->inst[instrument]->sounds[sound]->env.attackVolume = 0x7F7F;
			alBank->inst[instrument]->sounds[sound]->env.decayTime = 0xFFFFFFFF;
			alBank->inst[instrument]->sounds[sound]->env.decayVolume = 0;
			alBank->inst[instrument]->sounds[sound]->env.releaseTime = 0;
			alBank->inst[instrument]->sounds[sound]->key.detune = 0;
			if (hasLoopData)
				alBank->inst[instrument]->sounds[sound]->key.keybase = keyBase;
			else
				alBank->inst[instrument]->sounds[sound]->key.keybase = 0x3C;
			alBank->inst[instrument]->sounds[sound]->key.keymax = 0x7F;
			alBank->inst[instrument]->sounds[sound]->key.keymin = 0;
			alBank->inst[instrument]->sounds[sound]->key.velocitymax = 0x7F;
			alBank->inst[instrument]->sounds[sound]->key.velocitymin = 0;
			alBank->inst[instrument]->sounds[sound]->samplePan = 0x40;
			alBank->inst[instrument]->sounds[sound]->sampleVolume = 0x7F;*/
			alWave->flags = 0;

			if (hasLoopData)
			{
				alWave->adpcmWave->loop = new ALADPCMloop();
				alWave->adpcmWave->loop->count = loopCount;
				alWave->adpcmWave->loop->start = loopStart;
				alWave->adpcmWave->loop->end = loopEnd;
				alWave->adpcmWave->loop->unknown1 = 0;
				for (int x = 0; x < 0x10; x++)
					alWave->adpcmWave->loop->state[x] = alWave->adpcmWave->book->predictors[x];

			}
		}

		delete [] pcmSamples;
		delete [] vadpcmData;
	}

	delete [] wavPCMData;

	UpdateAudioOffsets(alBank);

	return true;
}

void CN64AIFCAudio::WriteAudioToFile(ALBank*& alBank, CString outFileNameCtl, CString outFileNameTbl)
{
	unsigned char* ctl;
	unsigned char *tbl;
	int ctlSize, tblSize;
	if (alBank->soundBankFormat == STANDARDFORMAT)
	{
		WriteAudio(alBank, ctl, ctlSize, tbl, tblSize);
	}
	else if (alBank->soundBankFormat == N64PTRWAVETABLETABLEV2)
	{
		WriteAudioN64PtrWavetableV2(alBank, ctl, ctlSize, tbl, tblSize);
	}
	else if (alBank->soundBankFormat == N64PTRWAVETABLETABLEV1)
	{
		WriteAudioN64PtrWavetableV1(alBank, ctl, ctlSize, tbl, tblSize);
	}
	else if (alBank->soundBankFormat == SUPERMARIO64FORMAT)
	{
		WriteAudioSuperMario(alBank, ctl, ctlSize, tbl, tblSize);
	}
	else if ((alBank->soundBankFormat == STARFOX64FORMAT) || (alBank->soundBankFormat == ZELDAFORMAT) || (alBank->soundBankFormat == FZEROFORMAT) || (alBank->soundBankFormat == TUROKFORMAT)  || (alBank->soundBankFormat == STANDARDRNCCOMPRESSED) || (alBank->soundBankFormat == STANDARDRNXCOMPRESSED) || (alBank->soundBankFormat == MARIOKART64FORMAT))
	{
		MessageBox(NULL, "Sorry, format not supported", "Error", NULL);
		return;
	}
	else if (alBank->soundBankFormat == N64PTRWAVETABLETABLEV2YAY0)
	{
		MessageBox(NULL, "Sorry, no encoding yet for YAY0 format", "Error", NULL);
		return;
	}
	else if (alBank->soundBankFormat == ARMYMENFORMAT)
	{
		MessageBox(NULL, "Sorry, no encoding yet for Army Men format", "Error", NULL);
		return;
	}
	else if (alBank->soundBankFormat == N64PTRWAVETABLETABLEV2ZLIB)
	{
		MessageBox(NULL, "Sorry, no encoding yet for ZLib N64Wave format", "Error", NULL);
		return;
	}
	else if (alBank->soundBankFormat == TITUS)
	{
		MessageBox(NULL, "Sorry, no encoding yet for Titus format", "Error", NULL);
		return;
	}
	else if (alBank->soundBankFormat == MKMYTHOLOGIES)
	{
		MessageBox(NULL, "Sorry, no encoding yet for BOFS format", "Error", NULL);
		return;
	}
	else if (alBank->soundBankFormat == N64PTRWAVETABLETABLEV2BLITZ)
	{
		MessageBox(NULL, "Sorry, no encoding yet for Blitz N64Wave format", "Error", NULL);
		return;
	}
	else if (alBank->soundBankFormat == ZLIBSN64)
	{
		MessageBox(NULL, "Sorry, no encoding yet for ZLIBSN64 format", "Error", NULL);
		return;
	}
	else if (alBank->soundBankFormat == SN64)
	{
		MessageBox(NULL, "Sorry, no encoding yet for SN64 format", "Error", NULL);
		return;
	}

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

	delete [] ctl;
	delete [] tbl;
}

void CN64AIFCAudio::WriteAudioN64PtrWavetableV1(ALBank*& alBank, unsigned char*& ctl, int& ctlSize, unsigned char*& tbl, int& tblSize)
{
	unsigned char* temporaryCtlBuffer = new unsigned char[0x1000000];
	unsigned char* temporaryTblBuffer = new unsigned char[0x1000000];
	unsigned long outputCtlCounter = 0;

	for (int x = 0; x < 0x1000000; x++)
		temporaryCtlBuffer[x] = 0x00;

	WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, 0x4E363420);
	outputCtlCounter += 4;
	WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, 0x50747254);
	outputCtlCounter += 4;
	WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, 0x61626C65);
	outputCtlCounter += 4;
	WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, 0x73202000);
	outputCtlCounter += 4;

	unsigned long outputTblCounter = 0;

	WriteLongToBuffer(temporaryTblBuffer, outputTblCounter, 0x4E363420);
	outputTblCounter += 4;
	WriteLongToBuffer(temporaryTblBuffer, outputTblCounter, 0x57617665);
	outputTblCounter += 4;
	WriteLongToBuffer(temporaryTblBuffer, outputTblCounter, 0x5461626C);
	outputTblCounter += 4;
	WriteLongToBuffer(temporaryTblBuffer, outputTblCounter, 0x65732000);
	outputTblCounter += 4;

	

	unsigned long* instrumentSoundStartLookup = new unsigned long[alBank->count];
	unsigned long* bookOffsetsWav = new unsigned long[alBank->count];
	unsigned long* adpcmRawLoopOffsetsWav = new unsigned long[alBank->count];
	unsigned long* tblOffsets = new unsigned long[alBank->count];

	WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->count);
	outputCtlCounter += 4;

	unsigned long startWriteInstrumentOffsets = outputCtlCounter;
	// placeholder
	outputCtlCounter += (alBank->count * 4);
	
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

		instrumentSoundStartLookup[x] = outputCtlCounter;

		int y = 0; // sound

		tblOffsets[x] = outputTblCounter;

		int foundSameTbl = false;

		/*for (int r = 0; r < x; r++)
		{
			int z = 0;

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
					tblOffsets[x] = tblOffsets[r];
					break;
				}
				else
				{
					continue;
				}
			}
		}*/

		WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, tblOffsets[x]);
		WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter + 4, alBank->inst[x]->sounds[y]->wav.len);

		if (!foundSameTbl)
		{
			for (int z = 0; z < alBank->inst[x]->sounds[y]->wav.len; z++)
			{
				temporaryTblBuffer[outputTblCounter + z] = alBank->inst[x]->sounds[y]->wav.wavData[z];
			}
			outputTblCounter += alBank->inst[x]->sounds[y]->wav.len;

			if ((outputTblCounter % 0x10) != 0)
			{
				int pad = 0x10 - (outputTblCounter % 0x10);
				for (int z = 0; z < pad; z++)
				{
					temporaryTblBuffer[outputTblCounter++] = 0;
				}
			}
		}

		int sameLoopBank = -1;
		int sameBank = -1;

		/*if (alBank->inst[x]->sounds[y]->wav.adpcmWave->loop != NULL)
		{
			int w = 0;
			for (int r = 0; r < x; r++)
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

						adpcmRawLoopOffsetsWav[x] = adpcmRawLoopOffsetsWav[r];
						sameLoopBank = r;
						break;
					}
				}
			}
		}
		 
		

		for (int r = 0; r < x; r++)
		{
			int w = 0;

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
					break;
				}
			}
		}*/

		
		unsigned long bookWriteSpot = outputCtlCounter + 0x10;
		unsigned long loopWriteSpot = outputCtlCounter + 0xC;

		outputCtlCounter = outputCtlCounter + 0x18;

		if (sameBank == -1)
		{
			bookOffsetsWav[x] = bookWriteSpot;
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
			bookOffsetsWav[x] = bookOffsetsWav[sameBank];
			WriteLongToBuffer(temporaryCtlBuffer, bookWriteSpot, bookOffsetsWav[sameBank]);
		}

		if (alBank->inst[x]->sounds[y]->wav.adpcmWave->loop != NULL)
		{
			if (sameLoopBank == -1)
			{
				adpcmRawLoopOffsetsWav[x] = outputCtlCounter;
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
				adpcmRawLoopOffsetsWav[x] = adpcmRawLoopOffsetsWav[sameLoopBank];
				WriteLongToBuffer(temporaryCtlBuffer, loopWriteSpot, adpcmRawLoopOffsetsWav[sameLoopBank]);
			}
		}
		else
		{
			adpcmRawLoopOffsetsWav[x] = 0x00000000;
			// null loop
			WriteLongToBuffer(temporaryCtlBuffer, loopWriteSpot, 0x0);
		}
	}

	WriteLongToBuffer(temporaryCtlBuffer, 0x2C, outputCtlCounter);

	for (int x = 0; x < alBank->count; x++)
	{
		WriteLongToBuffer(temporaryCtlBuffer, startWriteInstrumentOffsets, instrumentSoundStartLookup[x]);
		startWriteInstrumentOffsets += 4;
	}

	delete [] instrumentSoundStartLookup;
	delete [] bookOffsetsWav;
	delete [] adpcmRawLoopOffsetsWav;
	delete [] tblOffsets;

	
	ctl = temporaryCtlBuffer;
	tbl = temporaryTblBuffer;

	ctlSize = outputCtlCounter;
	tblSize = outputTblCounter;
	
}

void CN64AIFCAudio::WriteAudioN64PtrWavetableV2(ALBank*& alBank, unsigned char*& ctl, int& ctlSize, unsigned char*& tbl, int& tblSize)
{
	unsigned char* temporaryCtlBuffer = new unsigned char[0x1000000];
	unsigned char* temporaryTblBuffer = new unsigned char[0x1000000];
	unsigned long outputCtlCounter = 0;

	for (int x = 0; x < 0x1000000; x++)
		temporaryCtlBuffer[x] = 0x00;

	WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, 0x4E363420);
	outputCtlCounter += 4;
	WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, 0x50747254);
	outputCtlCounter += 4;
	WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, 0x61626C65);
	outputCtlCounter += 4;
	WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, 0x73563200);
	outputCtlCounter += 4;

	// don't name bank
	outputCtlCounter += 0x10;
	// placeholders for offsets
	outputCtlCounter += 0x10;

	unsigned long outputTblCounter = 0;

	WriteLongToBuffer(temporaryTblBuffer, outputTblCounter, 0x4E363420);
	outputTblCounter += 4;
	WriteLongToBuffer(temporaryTblBuffer, outputTblCounter, 0x57617665);
	outputTblCounter += 4;
	WriteLongToBuffer(temporaryTblBuffer, outputTblCounter, 0x5461626C);
	outputTblCounter += 4;
	WriteLongToBuffer(temporaryTblBuffer, outputTblCounter, 0x65732000);
	outputTblCounter += 4;

	

	unsigned long* instrumentSoundStartLookup = new unsigned long[alBank->count];
	unsigned long* bookOffsetsWav = new unsigned long[alBank->count];
	unsigned long* adpcmRawLoopOffsetsWav = new unsigned long[alBank->count];
	unsigned long* tblOffsets = new unsigned long[alBank->count];

	WriteLongToBuffer(temporaryCtlBuffer, 0x20, alBank->count);
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

		instrumentSoundStartLookup[x] = outputCtlCounter;

		int y = 0; // sound

		tblOffsets[x] = outputTblCounter;

		int foundSameTbl = false;

		for (int r = 0; r < x; r++)
		{
			int z = 0;

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
					tblOffsets[x] = tblOffsets[r];
					break;
				}
				else
				{
					continue;
				}
			}
		}

		WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, tblOffsets[x]);
		WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter + 4, alBank->inst[x]->sounds[y]->wav.len);

		if (!foundSameTbl)
		{
			for (int z = 0; z < alBank->inst[x]->sounds[y]->wav.len; z++)
			{
				temporaryTblBuffer[outputTblCounter + z] = alBank->inst[x]->sounds[y]->wav.wavData[z];
			}
			outputTblCounter += alBank->inst[x]->sounds[y]->wav.len;

			if ((outputTblCounter % 0x10) != 0)
			{
				int pad = 0x10 - (outputTblCounter % 0x10);
				for (int z = 0; z < pad; z++)
				{
					temporaryTblBuffer[outputTblCounter++] = 0;
				}
			}
		}

		int sameLoopBank = -1;


		if (alBank->inst[x]->sounds[y]->wav.adpcmWave->loop != NULL)
		{
			int w = 0;
			for (int r = 0; r < x; r++)
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

						adpcmRawLoopOffsetsWav[x] = adpcmRawLoopOffsetsWav[r];
						sameLoopBank = r;
						break;
					}
				}
			}
		}
		 
		int sameBank = -1;

		for (int r = 0; r < x; r++)
		{
			int w = 0;

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
					break;
				}
			}
		}

		
		unsigned long bookWriteSpot = outputCtlCounter + 0x10;
		unsigned long loopWriteSpot = outputCtlCounter + 0xC;

		outputCtlCounter = outputCtlCounter + 0x18;

		if (sameBank == -1)
		{
			bookOffsetsWav[x] = bookWriteSpot;
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
			bookOffsetsWav[x] = bookOffsetsWav[sameBank];
			WriteLongToBuffer(temporaryCtlBuffer, bookWriteSpot, bookOffsetsWav[sameBank]);
		}

		if (alBank->inst[x]->sounds[y]->wav.adpcmWave->loop != NULL)
		{
			if (sameLoopBank == -1)
			{
				adpcmRawLoopOffsetsWav[x] = outputCtlCounter;
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
				adpcmRawLoopOffsetsWav[x] = adpcmRawLoopOffsetsWav[sameLoopBank];
				WriteLongToBuffer(temporaryCtlBuffer, loopWriteSpot, adpcmRawLoopOffsetsWav[sameLoopBank]);
			}
		}
		else
		{
			adpcmRawLoopOffsetsWav[x] = 0x00000000;
			// null loop
			WriteLongToBuffer(temporaryCtlBuffer, loopWriteSpot, 0x0);
		}
	}

	WriteLongToBuffer(temporaryCtlBuffer, 0x2C, outputCtlCounter);

	for (int x = 0; x < alBank->count; x++)
	{
		WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, instrumentSoundStartLookup[x]);
		outputCtlCounter += 4;
	}
	WriteLongToBuffer(temporaryCtlBuffer, 0x24, outputCtlCounter);

	for (int x = 0; x < alBank->count; x++)
	{
		temporaryCtlBuffer[outputCtlCounter] = alBank->inst[x]->volume;
		outputCtlCounter++;
	}

	if ((outputCtlCounter % 4) != 0)
	{
		int pad = 4 - (outputCtlCounter % 4);
		for (int z = 0; z < pad; z++)
		{
			temporaryCtlBuffer[outputCtlCounter++] = 0;
		}
	}

	WriteLongToBuffer(temporaryCtlBuffer, 0x28, outputCtlCounter);

	delete [] instrumentSoundStartLookup;
	delete [] bookOffsetsWav;
	delete [] adpcmRawLoopOffsetsWav;
	delete [] tblOffsets;

	
	ctl = temporaryCtlBuffer;
	tbl = temporaryTblBuffer;

	ctlSize = outputCtlCounter;
	tblSize = outputTblCounter;
	
}

bool CN64AIFCAudio::CompareLoops(ALWave* wavIn, ALWave* wavOut)
{
	if (wavOut->adpcmWave == NULL)
		return false;

	if (wavOut->adpcmWave->loop == NULL)
		return false;

	if ((wavIn->adpcmWave->loop->start == wavOut->adpcmWave->loop->start) 
		&& (wavIn->adpcmWave->loop->end == wavOut->adpcmWave->loop->end)
		&& (wavIn->adpcmWave->loop->count == wavOut->adpcmWave->loop->count))
	{
		if (wavIn->adpcmWave->loop->start != 0)
		{
			for (int ww = 0; ww < 0x10; ww++)
			{
				if (wavIn->adpcmWave->loop->state[ww] != wavOut->adpcmWave->loop->state[ww])
				{
					return -1;
				}
			}
		}

		return true;
	}
	return false;
}

bool CN64AIFCAudio::CompareBooks(ALWave* wavIn, ALWave* wavOut)
{
	if (wavOut->adpcmWave == NULL)
		return false;

	if (wavOut->adpcmWave->book == NULL)
		return false;

	if ((wavIn->adpcmWave->book->order == wavOut->adpcmWave->book->order) 
		&& (wavIn->adpcmWave->book->npredictors == wavOut->adpcmWave->book->npredictors))
	{
		for (int ww = 0; ww < (wavIn->adpcmWave->book->order * wavIn->adpcmWave->book->npredictors * 8); ww++)
		{
			if (wavIn->adpcmWave->book->predictors[ww] != wavOut->adpcmWave->book->predictors[ww])
			{
				return false;
			}
		}
		return true;
	}
	return false;
}

bool CN64AIFCAudio::CompareWavs(ALWave* wavIn, ALWave* wavOut)
{
	if (wavOut->adpcmWave == NULL)
		return false;

	if (
		(wavIn->base == wavOut->base)
		&& (wavIn->len == wavOut->len)
		&& (wavIn->unknown1 == wavOut->unknown1)
		&& (wavIn->unknown2 == wavOut->unknown2)
		&& (wavIn->unknown3 == wavOut->unknown3)
		&& (wavIn->unknown4 == wavOut->unknown4)
		)
	{
		if (((wavIn->adpcmWave->loop == NULL) && (wavOut->adpcmWave->loop != NULL))
			|| ((wavIn->adpcmWave->loop != NULL) && (wavOut->adpcmWave->loop == NULL)))
		{
			return false;
		}

		if ((wavIn->adpcmWave->loop == NULL) && (wavOut->adpcmWave->loop == NULL))
		{
			
		}
		else
		{
			if (
				(wavIn->adpcmWave->loop->start == wavOut->adpcmWave->loop->start)
				&& (wavIn->adpcmWave->loop->end == wavOut->adpcmWave->loop->end)
				&& (wavIn->adpcmWave->loop->count == wavOut->adpcmWave->loop->count))
			{
				if (wavIn->adpcmWave->loop->start != 0)
				{
					for (int z = 0; z < 0x10; z++)
					{
						if (wavIn->adpcmWave->loop->state[z] != wavOut->adpcmWave->loop->state[z])
						{
							return false;
						}
					}
				}
			}
			else
			{
				return false;
			}
		}

		if ((wavIn->adpcmWave->book->npredictors == wavOut->adpcmWave->book->npredictors)
			&& (wavIn->adpcmWave->book->order == wavOut->adpcmWave->book->order))
		{
			for (int z = 0; z < (wavIn->adpcmWave->book->npredictors * wavIn->adpcmWave->book->order * 8); z++)
			{
				if (wavIn->adpcmWave->book->predictors[z] != wavOut->adpcmWave->book->predictors[z])
				{
					return false;
				}
			}

			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

bool CN64AIFCAudio::CompareTbls(ALWave* wavIn, ALWave* wavOut)
{
	if (
		(wavIn->len == wavOut->len)
		)
	{
		for (int rr = 0; rr < wavIn->len; rr++)
		{
			if (wavIn->wavData[rr] != wavOut->wavData[rr])
			{
				return false;
			}
		}
		
		return true;
	}
	return false;
}

void CN64AIFCAudio::WriteAudioSuperMario(ALBank*& alBank, unsigned char*& ctl, int& ctlSize, unsigned char*& tbl, int& tblSize)
{
	unsigned char* temporaryCtlBuffer = new unsigned char[0x1000000];
	unsigned char* temporaryTblBuffer = new unsigned char[0x1000000];
	for (int x = 0; x < 0x1000000; x++)
	{
		temporaryCtlBuffer[x] = 0;
		temporaryTblBuffer[x] = 0;
	}
	unsigned long outputCtlCounter = 0;
	
	// bank #
	int bankCount = 1;
	WriteShortToBuffer(temporaryCtlBuffer, outputCtlCounter, 0x0001);
	outputCtlCounter += 2;

	WriteShortToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->count);
	outputCtlCounter += 2;

	for (int x = 0; x < alBank->count; x++)
	{
		// offset to bank later in file
		outputCtlCounter += 8;
	}

	if ((outputCtlCounter % 0x10) != 0)
		outputCtlCounter += ((0x10 - (outputCtlCounter % 0x10)));

	unsigned long* instrumentALLookup = new unsigned long[alBank->count];
	unsigned long** instrumentSoundStartLookup = new unsigned long*[alBank->count];
	unsigned long** offsetsEnv = new unsigned long*[alBank->count];
	unsigned long** offsetsWav = new unsigned long*[alBank->count];
	unsigned long** bookOffsetsWav = new unsigned long*[alBank->count];
	unsigned long** adpcmRawLoopOffsetsWav = new unsigned long*[alBank->count];
	unsigned long** tblOffsets = new unsigned long*[alBank->count];

	unsigned long** offsetsWavSecondary = new unsigned long*[alBank->count];
	unsigned long** bookOffsetsWavSecondary = new unsigned long*[alBank->count];
	unsigned long** adpcmRawLoopOffsetsWavSecondary = new unsigned long*[alBank->count];
	unsigned long** tblOffsetsSecondary = new unsigned long*[alBank->count];

	unsigned long** offsetsWavPrevious = new unsigned long*[alBank->count];
	unsigned long** bookOffsetsWavPrevious = new unsigned long*[alBank->count];
	unsigned long** adpcmRawLoopOffsetsWavPrevious = new unsigned long*[alBank->count];
	unsigned long** tblOffsetsPrevious = new unsigned long*[alBank->count];

	for (int x = 0; x < alBank->count; x++)
	{
		instrumentSoundStartLookup[x] = new unsigned long[alBank->inst[x]->soundCount+alBank->inst[x]->countPercussion];
		offsetsEnv[x] = new unsigned long[alBank->inst[x]->soundCount+alBank->inst[x]->countPercussion];
		offsetsWav[x] = new unsigned long[alBank->inst[x]->soundCount+alBank->inst[x]->countPercussion];
		bookOffsetsWav[x] = new unsigned long[alBank->inst[x]->soundCount+alBank->inst[x]->countPercussion];
		adpcmRawLoopOffsetsWav[x] = new unsigned long[alBank->inst[x]->soundCount+alBank->inst[x]->countPercussion];
		tblOffsets[x] = new unsigned long[alBank->inst[x]->soundCount+alBank->inst[x]->countPercussion];
		
		offsetsWavSecondary[x] = new unsigned long[alBank->inst[x]->soundCount];
		bookOffsetsWavSecondary[x] = new unsigned long[alBank->inst[x]->soundCount];
		adpcmRawLoopOffsetsWavSecondary[x] = new unsigned long[alBank->inst[x]->soundCount];
		tblOffsetsSecondary[x] = new unsigned long[alBank->inst[x]->soundCount];

		offsetsWavPrevious[x] = new unsigned long[alBank->inst[x]->soundCount];
		bookOffsetsWavPrevious[x] = new unsigned long[alBank->inst[x]->soundCount];
		adpcmRawLoopOffsetsWavPrevious[x] = new unsigned long[alBank->inst[x]->soundCount];
		tblOffsetsPrevious[x] = new unsigned long[alBank->inst[x]->soundCount];
	}

	int numberUniqueTblSets = 0;
	unsigned long* tblOriginalOffsets = new unsigned long[alBank->count];
	
	for (int x = 0; x < alBank->count; x++)
	{
		alBank->inst[x]->tblBase = 0;
	}

	for (int x = 0; x < alBank->count; x++)
	{
		bool found = false;
		for (int y = 0; y < numberUniqueTblSets; y++)
		{
			if (alBank->inst[x]->tblBase == tblOriginalOffsets[y])
				found = true;
		}

		if (!found)
		{
			tblOriginalOffsets[numberUniqueTblSets] = alBank->inst[x]->tblBase;
			numberUniqueTblSets++;
		}
	}

	unsigned long* tblSizes = new unsigned long[numberUniqueTblSets];
	unsigned char** tblData = new unsigned char*[numberUniqueTblSets];

	for (int x = 0; x < numberUniqueTblSets; x++)
	{
		tblSizes[x] = 0;
		tblData[x] = new unsigned char[0x1000000];
		for (int y = 0; y < 0x1000000; y++)
			tblData[x][y] = 0;
	}

	for (int x = 0; x < alBank->count; x++)
	{
		int tblUniqueOffset = 0;
		for (int p = 0; p < numberUniqueTblSets; p++)
		{
			if (alBank->inst[x]->tblBase == tblOriginalOffsets[p])
			{
				tblUniqueOffset = p;
				break;
			}
		}


		instrumentALLookup[x] = outputCtlCounter;

		// tbl bank offset
		WriteLongToBuffer(temporaryCtlBuffer, 0x4 + (0x8 * x), instrumentALLookup[x]);


		WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->soundCount);
		outputCtlCounter += 4;
		WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->countPercussion);
		outputCtlCounter += 4;
		WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->unknown2);
		outputCtlCounter += 4;
		WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->unknown3);
		outputCtlCounter += 4;
		// Write percussion bank offsets here after
		outputCtlCounter += 4;

		unsigned long soundOffsetSpot = outputCtlCounter;

		outputCtlCounter += (alBank->inst[x]->soundCount * 4);

		// is this 8 or 0x10?
		if ((outputCtlCounter % 0x10) != 0)
		{
			int pad = 0x10 - (outputCtlCounter % 0x10);
			for (int z = 0; z < pad; z++)
			{
				temporaryCtlBuffer[outputCtlCounter++] = 0;
			}
		}

		unsigned long soundDataOffset = instrumentALLookup[x] + 0x10;

		

		for (int y = 0; y < alBank->inst[x]->soundCount; y++)
		{

			if (alBank->inst[x]->sounds[y] != NULL)
			{

				offsetsWavPrevious[x][y] = (outputCtlCounter - soundDataOffset);

				if (alBank->inst[x]->sounds[y]->hasWavePrevious)
				{
					int entireMatch = -1;
					if (entireMatch == -1)
					{
						for (int w = 0; w < y; w++)
						{
							if (entireMatch != -1)
								break;

							if (alBank->inst[x]->sounds[w] != NULL)
							{
								if (!(alBank->inst[x]->sounds[w]->hasWavePrevious))
									continue;

								if (CompareWavs(&alBank->inst[x]->sounds[y]->wavPrevious, &alBank->inst[x]->sounds[w]->wavPrevious))
								{
									entireMatch = w;
									offsetsWavPrevious[x][y] = offsetsWavPrevious[x][entireMatch];
									bookOffsetsWavPrevious[x][y] = bookOffsetsWavPrevious[x][entireMatch];
									tblOffsetsPrevious[x][y] = tblOffsetsPrevious[x][entireMatch];
									break;
								}
								else
								{
									continue;
								}
							}
						}
					}


					if (entireMatch == -1)
					{
						for (int w = 0; w < y; w++)
						{
							if (entireMatch != -1)
								break;

							if (alBank->inst[x]->sounds[w] != NULL)
							{

								if (CompareWavs(&alBank->inst[x]->sounds[y]->wavPrevious, &alBank->inst[x]->sounds[w]->wav))
								{
									entireMatch = w;
									offsetsWavPrevious[x][y] = offsetsWav[x][entireMatch];
									bookOffsetsWavPrevious[x][y] = bookOffsetsWav[x][entireMatch];
									tblOffsetsPrevious[x][y] = tblOffsets[x][entireMatch];
									break;
								}
								else
								{
									continue;
								}
							}
						}
					}

					if (entireMatch == -1)
					{
						for (int w = 0; w < y; w++)
						{
							if (entireMatch != -1)
								break;

							if (alBank->inst[x]->sounds[w] != NULL)
							{
								if (!(alBank->inst[x]->sounds[w]->hasWaveSecondary))
									continue;

								if (CompareWavs(&alBank->inst[x]->sounds[y]->wavPrevious, &alBank->inst[x]->sounds[w]->wavSecondary))
								{
									entireMatch = w;
									offsetsWavPrevious[x][y] = offsetsWavSecondary[x][entireMatch];
									bookOffsetsWavPrevious[x][y] = bookOffsetsWavSecondary[x][entireMatch];
									tblOffsetsPrevious[x][y] = tblOffsetsSecondary[x][entireMatch];
									break;										
								}
								else
								{
									continue;
								}
							}
						}
					}

					if (entireMatch == -1)
					{
						int sameBook = -1;

						unsigned long loopOutputPosition = outputCtlCounter + 0x20;
						if (alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book != NULL)
						{						
							bookOffsetsWavPrevious[x][y] = loopOutputPosition;

							/*if (sameBook == -1)
							{
								for (int w = 0; w < y; w++)
								{
									if (alBank->inst[x]->sounds[w] != NULL)
									{
										if (alBank->inst[x]->sounds[w]->hasWavePrevious)
										{
											if (CompareBooks(&alBank->inst[x]->sounds[y]->wavPrevious, &alBank->inst[x]->sounds[w]->wavPrevious))
											{
												sameBook = w;
												bookOffsetsWavPrevious[x][y] = bookOffsetsWavPrevious[x][w];
												break;
											}
										}
									}
								}
							}

							if (sameBook == -1)
							{
								for (int w = 0; w < y; w++)
								{
									if (alBank->inst[x]->sounds[w] != NULL)
									{
										if (CompareBooks(&alBank->inst[x]->sounds[y]->wavPrevious, &alBank->inst[x]->sounds[w]->wav))
										{
											sameBook = w;
											bookOffsetsWavPrevious[x][y] = bookOffsetsWav[x][w];
											break;
										}
									}
								}
							}

							if (sameBook == -1)
							{
								for (int w = 0; w < y; w++)
								{
									if (alBank->inst[x]->sounds[w] != NULL)
									{
										if (alBank->inst[x]->sounds[w]->hasWaveSecondary)
										{
											if (CompareBooks(&alBank->inst[x]->sounds[y]->wavPrevious, &alBank->inst[x]->sounds[w]->wavSecondary))
											{
												sameBook = w;
												bookOffsetsWavPrevious[x][y] = bookOffsetsWavSecondary[x][w];
												break;
											}
										}
									}
								}
							}*/

							if (sameBook == -1)
							{
								loopOutputPosition = loopOutputPosition + 0x8 + ((alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order) * 0x10);
								if ((loopOutputPosition % 0x10) != 0)
									loopOutputPosition = (loopOutputPosition + (0x10 - (loopOutputPosition % 0x10)));
							}

						}
						else
						{
							// Used?
							MessageBox(NULL, "Error book", "Error", NULL);
						}

						int sameLoop = -1;

						if (alBank->inst[x]->sounds[y]->wav.adpcmWave->loop != NULL)
						{
							
							adpcmRawLoopOffsetsWavPrevious[x][y] = loopOutputPosition;

							if (alBank->inst[x]->sounds[y]->wav.adpcmWave->loop != NULL)
							{
								/*if (sameLoop == -1)
								{
									for (int w = 0; w < y; w++)
									{
										if (alBank->inst[x]->sounds[w] != NULL)
										{
											if (alBank->inst[x]->sounds[w]->hasWavePrevious)
											{
												if (CompareLoops(&alBank->inst[x]->sounds[y]->wavPrevious, &alBank->inst[x]->sounds[w]->wavPrevious))
												{
													adpcmRawLoopOffsetsWavPrevious[x][y] = adpcmRawLoopOffsetsWavPrevious[x][w];
													sameLoop = w;
													break;
												}
											}
										}
									}
								}

								if (sameLoop == -1)
								{
									for (int w = 0; w < y; w++)
									{
										if (alBank->inst[x]->sounds[w] != NULL)
										{
											if (CompareLoops(&alBank->inst[x]->sounds[y]->wavPrevious, &alBank->inst[x]->sounds[w]->wav))
											{
												adpcmRawLoopOffsetsWavPrevious[x][y] = adpcmRawLoopOffsetsWav[x][w];
												sameLoop = w;
												break;
											}
										}
									}
								}

								if (sameLoop == -1)
								{
									for (int w = 0; w < y; w++)
									{
										if (alBank->inst[x]->sounds[w] != NULL)
										{
											if (alBank->inst[x]->sounds[w]->hasWaveSecondary)
											{
												if (CompareLoops(&alBank->inst[x]->sounds[y]->wavPrevious, &alBank->inst[x]->sounds[w]->wavSecondary))
												{
													adpcmRawLoopOffsetsWavPrevious[x][y] = adpcmRawLoopOffsetsWavSecondary[x][w];
													sameLoop = w;
													break;
												}
											}
										}
									}
								}*/
							}
							
						}
						else
						{
							// Used?
							MessageBox(NULL, "Error loop", "Error", NULL);
						}

						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wavPrevious.unknown1);
						outputCtlCounter += 4;

						tblOffsetsPrevious[x][y] = tblSizes[tblUniqueOffset];

						bool foundSameTbl = false;

						if (!foundSameTbl)
						{
							for (int r = 0; r < (x + 1); r++)
							{
								if (alBank->inst[x]->tblBase != alBank->inst[r]->tblBase)
									continue;

								int loopEnd = y;
								if (r != x)
								{
									loopEnd = alBank->inst[r]->soundCount;
								}
								for (int z = 0; z < loopEnd; z++)
								{
									if (alBank->inst[r]->sounds[z] != NULL)
									{
										if (alBank->inst[r]->sounds[z]->hasWavePrevious)
										{
											if (CompareTbls(&alBank->inst[x]->sounds[y]->wavPrevious, &alBank->inst[r]->sounds[z]->wavPrevious))
											{
												foundSameTbl = true;
												tblOffsetsPrevious[x][y] = tblOffsetsPrevious[r][z];
												break;
											}
										}
									}
								}
							}
						}

						if (!foundSameTbl)
						{
							for (int r = 0; r < (x + 1); r++)
							{
								if (alBank->inst[x]->tblBase != alBank->inst[r]->tblBase)
									continue;

								int loopEnd = y;
								if (r != x)
								{
									loopEnd = alBank->inst[r]->soundCount;
								}
								for (int z = 0; z < loopEnd; z++)
								{
									if (alBank->inst[r]->sounds[z] != NULL)
									{
										if (CompareTbls(&alBank->inst[x]->sounds[y]->wavPrevious, &alBank->inst[r]->sounds[z]->wav))
										{
											foundSameTbl = true;
											tblOffsetsPrevious[x][y] = tblOffsets[r][z];
											break;
										}
									}
								}
							}
						}

						if (!foundSameTbl)
						{
							for (int r = 0; r < (x + 1); r++)
							{
								if (alBank->inst[x]->tblBase != alBank->inst[r]->tblBase)
									continue;

								int loopEnd = y;
								if (r != x)
								{
									loopEnd = alBank->inst[r]->soundCount;
								}
								for (int z = 0; z < loopEnd; z++)
								{
									if (alBank->inst[r]->sounds[z] != NULL)
									{
										if (alBank->inst[r]->sounds[z]->hasWaveSecondary)
										{
											if (CompareTbls(&alBank->inst[x]->sounds[y]->wavPrevious, &alBank->inst[r]->sounds[z]->wavSecondary))
											{
												foundSameTbl = true;
												tblOffsetsPrevious[x][y] = tblOffsetsSecondary[r][z];
												break;
											}
										}
									}
								}
							}
						}

						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, tblOffsetsPrevious[x][y]);
						outputCtlCounter += 4;


						if (!foundSameTbl)
						{
							unsigned long curEntrySize = alBank->inst[x]->sounds[y]->wavPrevious.len;
							if ((alBank->inst[x]->sounds[y]->wavPrevious.len % 0x10) != 0)
								curEntrySize += (0x10 - (alBank->inst[x]->sounds[y]->wavPrevious.len % 0x10));

							for (int z = 0; z < alBank->inst[x]->sounds[y]->wavPrevious.len; z++)
							{
								tblData[tblUniqueOffset][tblSizes[tblUniqueOffset] + z] = alBank->inst[x]->sounds[y]->wavPrevious.wavData[z];
							}

							tblSizes[tblUniqueOffset] += curEntrySize;
							
							if ((tblSizes[tblUniqueOffset] % 0x10) != 0)
							{
								tblSizes[tblUniqueOffset] += (0x10 - (tblSizes[tblUniqueOffset] % 0x10));
							}
						}

						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, (adpcmRawLoopOffsetsWavPrevious[x][y] - soundDataOffset));
						outputCtlCounter += 4;

						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, (bookOffsetsWavPrevious[x][y] - soundDataOffset));
						outputCtlCounter += 4;

						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wavPrevious.len);
						outputCtlCounter += 4;

						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wavPrevious.unknown2);
						outputCtlCounter += 4;

						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wavPrevious.unknown3);
						outputCtlCounter += 4;

						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wavPrevious.unknown4);
						outputCtlCounter += 4;

						if (sameBook == -1)
						{
							WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->order);
							outputCtlCounter += 4;
							WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->npredictors);
							outputCtlCounter += 4;


							for (int z = 0; z < (alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->npredictors * 8); z++)
							{
								temporaryCtlBuffer[outputCtlCounter++] = ((alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->predictors[z] >> 8) & 0xFF);
								temporaryCtlBuffer[outputCtlCounter++] = ((alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->predictors[z]) & 0xFF);
							}

							if ((outputCtlCounter % 0x10) != 0)
								outputCtlCounter = outputCtlCounter + (0x10 - (outputCtlCounter % 0x10));
						}

						if (sameLoop == -1)
						{

							WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->start);
							outputCtlCounter += 4;
							WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->end);
							outputCtlCounter += 4;
							WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->count);
							outputCtlCounter += 4;
							WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->unknown1);
							outputCtlCounter += 4;

							if (alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->start != 0)
							{
								for (int z = 0; z < 0x10; z++)
								{
									temporaryCtlBuffer[outputCtlCounter++] = ((alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->state[z] >> 8) & 0xFF);
									temporaryCtlBuffer[outputCtlCounter++] = ((alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->state[z]) & 0xFF);
								}
							}

							if ((outputCtlCounter % 0x10) != 0)
								outputCtlCounter = outputCtlCounter + (0x10 - (outputCtlCounter % 0x10));
						}
					}
				}





				offsetsWav[x][y] = (outputCtlCounter - soundDataOffset);

				int entireMatch = -1;
				if (entireMatch == -1)
				{
					for (int w = 0; w < (y + 1); w++)
					{
						if (entireMatch != -1)
							break;

						if (alBank->inst[x]->sounds[w] != NULL)
						{
							if (!(alBank->inst[x]->sounds[w]->hasWavePrevious))
								continue;

							if (CompareWavs(&alBank->inst[x]->sounds[y]->wav, &alBank->inst[x]->sounds[w]->wavPrevious))
							{
								entireMatch = w;
								offsetsWav[x][y] = offsetsWavPrevious[x][entireMatch];
								bookOffsetsWav[x][y] = bookOffsetsWavPrevious[x][entireMatch];
								tblOffsets[x][y] = tblOffsetsPrevious[x][entireMatch];
								break;
							}
							else
							{
								continue;
							}
						}
					}
				}


				if (entireMatch == -1)
				{
					for (int w = 0; w < y; w++)
					{
						if (entireMatch != -1)
							break;

						if (alBank->inst[x]->sounds[w] != NULL)
						{
							if (CompareWavs(&alBank->inst[x]->sounds[y]->wav, &alBank->inst[x]->sounds[w]->wav))
							{
								entireMatch = w;
								offsetsWav[x][y] = offsetsWav[x][entireMatch];
								bookOffsetsWav[x][y] = bookOffsetsWav[x][entireMatch];
								tblOffsets[x][y] = tblOffsets[x][entireMatch];
								break;
							}
							else
							{
								continue;
							}
						}
					}
				}

				if (entireMatch == -1)
				{
					for (int w = 0; w < y; w++)
					{
						if (entireMatch != -1)
							break;

						if (alBank->inst[x]->sounds[w] != NULL)
						{
							if (!(alBank->inst[x]->sounds[w]->hasWaveSecondary))
								continue;

							if (CompareWavs(&alBank->inst[x]->sounds[y]->wav, &alBank->inst[x]->sounds[w]->wavSecondary))
							{
								entireMatch = w;
								offsetsWav[x][y] = offsetsWavSecondary[x][entireMatch];
								bookOffsetsWav[x][y] = bookOffsetsWavSecondary[x][entireMatch];
								tblOffsets[x][y] = tblOffsetsSecondary[x][entireMatch];
								break;
							}
						}
					}
				}

				if (entireMatch == -1)
				{
					int sameBook = -1;

					unsigned long loopOutputPosition = outputCtlCounter + 0x20;
					if (alBank->inst[x]->sounds[y]->wav.adpcmWave->book != NULL)
					{						
						bookOffsetsWav[x][y] = loopOutputPosition;

						/*if (sameBook == -1)
						{
							for (int w = 0; w < (y + 1); w++)
							{
								if (alBank->inst[x]->sounds[w] != NULL)
								{
									if (alBank->inst[x]->sounds[w]->hasWavePrevious)
									{
										if (CompareBooks(&alBank->inst[x]->sounds[y]->wav, &alBank->inst[x]->sounds[w]->wavPrevious))
										{
											sameBook = w;
											bookOffsetsWav[x][y] = bookOffsetsWavPrevious[x][w];
											break;
										}
									}
								}
							}
						}

						if (sameBook == -1)
						{
							for (int w = 0; w < y; w++)
							{
								if (alBank->inst[x]->sounds[w] != NULL)
								{
									if (CompareBooks(&alBank->inst[x]->sounds[y]->wav, &alBank->inst[x]->sounds[w]->wav))
									{
										sameBook = w;
										bookOffsetsWav[x][y] = bookOffsetsWav[x][w];
										break;
									}
								}
							}
						}

						if (sameBook == -1)
						{
							for (int w = 0; w < y; w++)
							{
								if (alBank->inst[x]->sounds[w] != NULL)
								{
									if (alBank->inst[x]->sounds[w]->hasWaveSecondary)
									{
										if (CompareBooks(&alBank->inst[x]->sounds[y]->wav, &alBank->inst[x]->sounds[w]->wavSecondary))
										{
											sameBook = w;
											bookOffsetsWav[x][y] = bookOffsetsWavSecondary[x][w];
											break;
										}
									}
								}
							}
						}*/

						if (sameBook == -1)
						{
							loopOutputPosition = loopOutputPosition + 0x8 + ((alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order) * 0x10);
							if ((loopOutputPosition % 0x10) != 0)
								loopOutputPosition = (loopOutputPosition + (0x10 - (loopOutputPosition % 0x10)));
						}

					}
					else
					{
						// Used?
						MessageBox(NULL, "Error book", "Error", NULL);
					}

					int sameLoop = -1;

					if (alBank->inst[x]->sounds[y]->wav.adpcmWave->loop != NULL)
					{
						
						adpcmRawLoopOffsetsWav[x][y] = loopOutputPosition;

						if (alBank->inst[x]->sounds[y]->wav.adpcmWave->loop != NULL)
						{
							/*if (sameLoop == -1)
							{
								for (int w = 0; w < (y + 1); w++)
								{
									if (alBank->inst[x]->sounds[w] != NULL)
									{
										if (alBank->inst[x]->sounds[w]->hasWavePrevious)
										{
											if (CompareLoops(&alBank->inst[x]->sounds[y]->wav, &alBank->inst[x]->sounds[w]->wavPrevious))
											{
												adpcmRawLoopOffsetsWav[x][y] = adpcmRawLoopOffsetsWavPrevious[x][w];
												sameLoop = w;
												break;
											}
										}
									}
								}
							}

							if (sameLoop == -1)
							{
								for (int w = 0; w < y; w++)
								{
									if (alBank->inst[x]->sounds[w] != NULL)
									{
										if (CompareLoops(&alBank->inst[x]->sounds[y]->wav, &alBank->inst[x]->sounds[w]->wav))
										{
											adpcmRawLoopOffsetsWav[x][y] = adpcmRawLoopOffsetsWav[x][w];
											sameLoop = w;
											break;
										}
									}
								}
							}

							if (sameLoop == -1)
							{
								for (int w = 0; w < y; w++)
								{
									if (alBank->inst[x]->sounds[w] != NULL)
									{
										if (alBank->inst[x]->sounds[w]->hasWaveSecondary)
										{
											if (CompareLoops(&alBank->inst[x]->sounds[y]->wav, &alBank->inst[x]->sounds[w]->wavSecondary))
											{
												adpcmRawLoopOffsetsWav[x][y] = adpcmRawLoopOffsetsWavSecondary[x][w];
												sameLoop = w;
												break;
											}
										}
									}
								}
							}*/
						}
						
					}
					else
					{
						// Used?
						MessageBox(NULL, "Error loop", "Error", NULL);
					}

					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wav.unknown1);
					outputCtlCounter += 4;

					tblOffsets[x][y] = tblSizes[tblUniqueOffset];

					bool foundSameTbl = false;

					if (!foundSameTbl)
					{
						for (int r = 0; r < (x + 1); r++)
						{
							if (alBank->inst[x]->tblBase != alBank->inst[r]->tblBase)
								continue;

							int loopEnd = (y + 1);
							if (r != x)
							{
								loopEnd = alBank->inst[r]->soundCount;
							}
							for (int z = 0; z < loopEnd; z++)
							{
								if (alBank->inst[r]->sounds[z] != NULL)
								{
									if (alBank->inst[r]->sounds[z]->hasWavePrevious)
									{
										if (CompareTbls(&alBank->inst[x]->sounds[y]->wav, &alBank->inst[r]->sounds[z]->wavPrevious))
										{
											foundSameTbl = true;
											tblOffsets[x][y] = tblOffsetsPrevious[r][z];
											break;
										}
									}
								}
							}
						}
					}

					if (!foundSameTbl)
					{
						for (int r = 0; r < (x + 1); r++)
						{
							if (alBank->inst[x]->tblBase != alBank->inst[r]->tblBase)
								continue;

							int loopEnd = y;
							if (r != x)
							{
								loopEnd = alBank->inst[r]->soundCount;
							}
							for (int z = 0; z < loopEnd; z++)
							{
								if (alBank->inst[r]->sounds[z] != NULL)
								{
									if (CompareTbls(&alBank->inst[x]->sounds[y]->wav, &alBank->inst[r]->sounds[z]->wav))
									{
										foundSameTbl = true;
										tblOffsets[x][y] = tblOffsets[r][z];
										break;
									}
								}
							}
						}
					}

					if (!foundSameTbl)
					{
						for (int r = 0; r < (x + 1); r++)
						{
							if (alBank->inst[x]->tblBase != alBank->inst[r]->tblBase)
								continue;

							int loopEnd = y;
							if (r != x)
							{
								loopEnd = alBank->inst[r]->soundCount;
							}
							for (int z = 0; z < loopEnd; z++)
							{
								if (alBank->inst[r]->sounds[z] != NULL)
								{
									if (alBank->inst[r]->sounds[z]->hasWaveSecondary)
									{
										if (CompareTbls(&alBank->inst[x]->sounds[y]->wav, &alBank->inst[r]->sounds[z]->wavSecondary))
										{
											foundSameTbl = true;
											tblOffsets[x][y] = tblOffsetsSecondary[r][z];
											break;
										}
									}
								}
							}
						}
					}

					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, tblOffsets[x][y]);
					outputCtlCounter += 4;


					if (!foundSameTbl)
					{
						unsigned long curEntrySize = alBank->inst[x]->sounds[y]->wav.len;
						if ((alBank->inst[x]->sounds[y]->wav.len % 0x10) != 0)
							curEntrySize += (0x10 - (alBank->inst[x]->sounds[y]->wav.len % 0x10));

						for (int z = 0; z < alBank->inst[x]->sounds[y]->wav.len; z++)
						{
							tblData[tblUniqueOffset][tblSizes[tblUniqueOffset] + z] = alBank->inst[x]->sounds[y]->wav.wavData[z];
						}

						tblSizes[tblUniqueOffset] += curEntrySize;
						
						if ((tblSizes[tblUniqueOffset] % 0x10) != 0)
						{
							tblSizes[tblUniqueOffset] += (0x10 - (tblSizes[tblUniqueOffset] % 0x10));
						}
					}

					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, (adpcmRawLoopOffsetsWav[x][y] - soundDataOffset));
					outputCtlCounter += 4;

					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, (bookOffsetsWav[x][y] - soundDataOffset));
					outputCtlCounter += 4;

					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wav.len);
					outputCtlCounter += 4;

					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wav.unknown2);
					outputCtlCounter += 4;

					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wav.unknown3);
					outputCtlCounter += 4;

					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wav.unknown4);
					outputCtlCounter += 4;

					if (sameBook == -1)
					{
						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order);
						outputCtlCounter += 4;
						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors);
						outputCtlCounter += 4;
						
						for (int z = 0; z < (alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8); z++)
						{
							temporaryCtlBuffer[outputCtlCounter++] = ((alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[z] >> 8) & 0xFF);
							temporaryCtlBuffer[outputCtlCounter++] = ((alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[z]) & 0xFF);
						}

						if ((outputCtlCounter % 0x10) != 0)
							outputCtlCounter = outputCtlCounter + (0x10 - (outputCtlCounter % 0x10));
					}

					if (sameLoop == -1)
					{
						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->start);
						outputCtlCounter += 4;
						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->end);
						outputCtlCounter += 4;
						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->count);
						outputCtlCounter += 4;
						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->unknown1);
						outputCtlCounter += 4;

						if (alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->start != 0)
						{
							for (int z = 0; z < 0x10; z++)
							{
								temporaryCtlBuffer[outputCtlCounter++] = ((alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->state[z] >> 8) & 0xFF);
								temporaryCtlBuffer[outputCtlCounter++] = ((alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->state[z]) & 0xFF);
							}
						}

						if ((outputCtlCounter % 0x10) != 0)
							outputCtlCounter = outputCtlCounter + (0x10 - (outputCtlCounter % 0x10));
					}
				}

				offsetsWavSecondary[x][y] = (outputCtlCounter - soundDataOffset);

				if (alBank->inst[x]->sounds[y]->hasWaveSecondary)
				{


					int entireMatch = -1;
					if (entireMatch == -1)
					{
						for (int w = 0; w < (y + 1); w++)
						{
							if (entireMatch != -1)
								break;

							if (alBank->inst[x]->sounds[w] != NULL)
							{
								if (!(alBank->inst[x]->sounds[w]->hasWavePrevious))
									continue;

								if (CompareWavs(&alBank->inst[x]->sounds[y]->wavSecondary, &alBank->inst[x]->sounds[w]->wavPrevious))
								{
									entireMatch = w;
									offsetsWavSecondary[x][y] = offsetsWavPrevious[x][entireMatch];
									bookOffsetsWavSecondary[x][y] = bookOffsetsWavPrevious[x][entireMatch];
									tblOffsetsSecondary[x][y] = tblOffsetsPrevious[x][entireMatch];
									break;
								}
								else
								{
									continue;
								}
							}
						}
					}


					if (entireMatch == -1)
					{
						for (int w = 0; w < (y + 1); w++)
						{
							if (entireMatch != -1)
								break;

							if (alBank->inst[x]->sounds[w] != NULL)
							{

								if (CompareWavs(&alBank->inst[x]->sounds[y]->wavSecondary, &alBank->inst[x]->sounds[w]->wav))
								{
									entireMatch = w;
									offsetsWavSecondary[x][y] = offsetsWav[x][entireMatch];
									bookOffsetsWavSecondary[x][y] = bookOffsetsWav[x][entireMatch];
									tblOffsetsSecondary[x][y] = tblOffsets[x][entireMatch];
									break;
								}
								else
								{
									continue;
								}
							}
						}
					}

					if (entireMatch == -1)
					{
						for (int w = 0; w < y; w++)
						{
							if (entireMatch != -1)
								break;

							if (alBank->inst[x]->sounds[w] != NULL)
							{
								if (!(alBank->inst[x]->sounds[w]->hasWaveSecondary))
									continue;

								if (CompareWavs(&alBank->inst[x]->sounds[y]->wavSecondary, &alBank->inst[x]->sounds[w]->wavSecondary))
								{
									entireMatch = w;
									offsetsWavSecondary[x][y] = offsetsWavSecondary[x][entireMatch];
									bookOffsetsWavSecondary[x][y] = bookOffsetsWavSecondary[x][entireMatch];
									tblOffsetsSecondary[x][y] = tblOffsetsSecondary[x][entireMatch];
									break;
								}
								else
								{
									continue;
								}
							}
						}
					}

					if (entireMatch == -1)
					{
						int sameBook = -1;

						unsigned long loopOutputPosition = outputCtlCounter + 0x20;
						if (alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book != NULL)
						{						
							bookOffsetsWavSecondary[x][y] = loopOutputPosition;

							/*if (sameBook == -1)
							{
								for (int w = 0; w < (y + 1); w++)
								{
									if (alBank->inst[x]->sounds[w] != NULL)
									{
										if (alBank->inst[x]->sounds[w]->hasWavePrevious)
										{
											if (CompareBooks(&alBank->inst[x]->sounds[y]->wavSecondary, &alBank->inst[x]->sounds[w]->wavPrevious))
											{
												sameBook = w;
												bookOffsetsWavSecondary[x][y] = bookOffsetsWavPrevious[x][w];
												break;
											}
										}
									}
								}
							}

							if (sameBook == -1)
							{
								for (int w = 0; w < (y + 1); w++)
								{
									if (alBank->inst[x]->sounds[w] != NULL)
									{
										if (CompareBooks(&alBank->inst[x]->sounds[y]->wavSecondary, &alBank->inst[x]->sounds[w]->wav))
										{
											sameBook = w;
											bookOffsetsWavSecondary[x][y] = bookOffsetsWav[x][w];
											break;
										}
									}
								}
							}

							if (sameBook == -1)
							{
								for (int w = 0; w < y; w++)
								{
									if (alBank->inst[x]->sounds[w] != NULL)
									{
										if (alBank->inst[x]->sounds[w]->hasWaveSecondary)
										{
											if (CompareBooks(&alBank->inst[x]->sounds[y]->wavSecondary, &alBank->inst[x]->sounds[w]->wavSecondary))
											{
												sameBook = w;
												bookOffsetsWavSecondary[x][y] = bookOffsetsWavSecondary[x][w];
												break;
											}
										}
									}
								}
							}*/

							if (sameBook == -1)
							{
								loopOutputPosition = loopOutputPosition + 0x8 + ((alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order) * 0x10);
								if ((loopOutputPosition % 0x10) != 0)
									loopOutputPosition = (loopOutputPosition + (0x10 - (loopOutputPosition % 0x10)));
							}

						}
						else
						{
							// Used?
							MessageBox(NULL, "Error book", "Error", NULL);
						}

						int sameLoop = -1;

						if (alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop != NULL)
						{
							
							adpcmRawLoopOffsetsWavSecondary[x][y] = loopOutputPosition;

							if (alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop != NULL)
							{
								/*if (sameLoop == -1)
								{
									for (int w = 0; w < (y + 1); w++)
									{
										if (alBank->inst[x]->sounds[w] != NULL)
										{
											if (alBank->inst[x]->sounds[w]->hasWavePrevious)
											{
												if (CompareLoops(&alBank->inst[x]->sounds[y]->wavSecondary, &alBank->inst[x]->sounds[w]->wavPrevious))
												{
													adpcmRawLoopOffsetsWavSecondary[x][y] = adpcmRawLoopOffsetsWavPrevious[x][w];
													sameLoop = w;
													break;
												}
											}
										}
									}
								}

								if (sameLoop == -1)
								{
									for (int w = 0; w < (y + 1); w++)
									{
										if (alBank->inst[x]->sounds[w] != NULL)
										{
											if (CompareLoops(&alBank->inst[x]->sounds[y]->wavSecondary, &alBank->inst[x]->sounds[w]->wav))
											{
												adpcmRawLoopOffsetsWavSecondary[x][y] = adpcmRawLoopOffsetsWav[x][w];
												sameLoop = w;
												break;
											}
										}
									}
								}

								if (sameLoop == -1)
								{
									for (int w = 0; w < y; w++)
									{
										if (alBank->inst[x]->sounds[w] != NULL)
										{
											if (alBank->inst[x]->sounds[w]->hasWaveSecondary)
											{
												if (CompareLoops(&alBank->inst[x]->sounds[y]->wavSecondary, &alBank->inst[x]->sounds[w]->wavSecondary))
												{
													adpcmRawLoopOffsetsWavSecondary[x][y] = adpcmRawLoopOffsetsWavSecondary[x][w];
													sameLoop = w;
													break;
												}
											}
										}
									}
								}*/
							}
						}
						else
						{
							// Used?
							MessageBox(NULL, "Error loop", "Error", NULL);
						}

						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wavSecondary.unknown1);
						outputCtlCounter += 4;

						tblOffsetsSecondary[x][y] = tblSizes[tblUniqueOffset];

						bool foundSameTbl = false;

						
						if (!foundSameTbl)
						{
							for (int r = 0; r < (x + 1); r++)
							{
								if (alBank->inst[x]->tblBase != alBank->inst[r]->tblBase)
									continue;

								int loopEnd = (y + 1);
								if (r != x)
								{
									loopEnd = alBank->inst[r]->soundCount;
								}
								for (int z = 0; z < loopEnd; z++)
								{
									if (alBank->inst[r]->sounds[z] != NULL)
									{
										if (alBank->inst[r]->sounds[z]->hasWavePrevious)
										{
											if (CompareTbls(&alBank->inst[x]->sounds[y]->wavSecondary, &alBank->inst[r]->sounds[z]->wavPrevious))
											{
												foundSameTbl = true;
												tblOffsetsSecondary[x][y] = tblOffsetsPrevious[r][z];
												break;
											}
										}
									}
								}
							}
						}

						if (!foundSameTbl)
						{
							for (int r = 0; r < (x + 1); r++)
							{
								if (alBank->inst[x]->tblBase != alBank->inst[r]->tblBase)
									continue;

								int loopEnd = (y + 1);
								if (r != x)
								{
									loopEnd = alBank->inst[r]->soundCount;
								}
								for (int z = 0; z < loopEnd; z++)
								{
									if (alBank->inst[r]->sounds[z] != NULL)
									{
										if (CompareTbls(&alBank->inst[x]->sounds[y]->wavSecondary, &alBank->inst[r]->sounds[z]->wav))
										{
											foundSameTbl = true;
											tblOffsetsSecondary[x][y] = tblOffsets[r][z];
											break;
										}
									}
								}
							}
						}

						if (!foundSameTbl)
						{
							for (int r = 0; r < (x + 1); r++)
							{
								if (alBank->inst[x]->tblBase != alBank->inst[r]->tblBase)
									continue;

								int loopEnd = y;
								if (r != x)
								{
									loopEnd = alBank->inst[r]->soundCount;
								}
								for (int z = 0; z < loopEnd; z++)
								{
									if (alBank->inst[r]->sounds[z] != NULL)
									{
										if (alBank->inst[r]->sounds[z]->hasWaveSecondary)
										{
											if (CompareTbls(&alBank->inst[x]->sounds[y]->wavSecondary, &alBank->inst[r]->sounds[z]->wavSecondary))
											{
												foundSameTbl = true;
												tblOffsetsSecondary[x][y] = tblOffsetsSecondary[r][z];
												break;
											}
										}
									}
								}
							}
						}

						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, tblOffsetsSecondary[x][y]);
						outputCtlCounter += 4;


						if (!foundSameTbl)
						{
							unsigned long curEntrySize = alBank->inst[x]->sounds[y]->wavSecondary.len;
							if ((alBank->inst[x]->sounds[y]->wavSecondary.len % 0x10) != 0)
								curEntrySize += (0x10 - (alBank->inst[x]->sounds[y]->wavSecondary.len % 0x10));

							for (int z = 0; z < alBank->inst[x]->sounds[y]->wavSecondary.len; z++)
							{
								tblData[tblUniqueOffset][tblSizes[tblUniqueOffset] + z] = alBank->inst[x]->sounds[y]->wavSecondary.wavData[z];
							}

							tblSizes[tblUniqueOffset] += curEntrySize;
							
							if ((tblSizes[tblUniqueOffset] % 0x10) != 0)
							{
								tblSizes[tblUniqueOffset] += (0x10 - (tblSizes[tblUniqueOffset] % 0x10));
							}
						}

						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, (adpcmRawLoopOffsetsWavSecondary[x][y] - soundDataOffset));
						outputCtlCounter += 4;

						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, (bookOffsetsWavSecondary[x][y] - soundDataOffset));
						outputCtlCounter += 4;

						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wavSecondary.len);
						outputCtlCounter += 4;

						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wavSecondary.unknown2);
						outputCtlCounter += 4;

						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wavSecondary.unknown3);
						outputCtlCounter += 4;

						WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wavSecondary.unknown4);
						outputCtlCounter += 4;

						
						if (sameBook == -1)
						{
							WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->order);
							outputCtlCounter += 4;
							WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->npredictors);
							outputCtlCounter += 4;

							for (int z = 0; z < (alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->npredictors * 8); z++)
							{
								temporaryCtlBuffer[outputCtlCounter++] = ((alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->predictors[z] >> 8) & 0xFF);
								temporaryCtlBuffer[outputCtlCounter++] = ((alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->predictors[z]) & 0xFF);
							}

							if ((outputCtlCounter % 0x10) != 0)
								outputCtlCounter = outputCtlCounter + (0x10 - (outputCtlCounter % 0x10));
						}

						if (sameLoop == -1)
						{
							WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->start);
							outputCtlCounter += 4;
							WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->end);
							outputCtlCounter += 4;
							WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->count);
							outputCtlCounter += 4;
							WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->unknown1);
							outputCtlCounter += 4;

							if (alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->start != 0)
							{
								for (int z = 0; z < 0x10; z++)
								{
									temporaryCtlBuffer[outputCtlCounter++] = ((alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->state[z] >> 8) & 0xFF);
									temporaryCtlBuffer[outputCtlCounter++] = ((alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->state[z]) & 0xFF);
								}
							}

							if ((outputCtlCounter % 0x10) != 0)
								outputCtlCounter = outputCtlCounter + (0x10 - (outputCtlCounter % 0x10));
						}
					}
				}
			}
		}

		int startPercussionInListing = alBank->inst[x]->soundCount;

		for (int y = 0; y < alBank->inst[x]->countPercussion; y++)
		{
			offsetsWav[x][startPercussionInListing+y] = (outputCtlCounter - soundDataOffset);

			int entireMatch = -1;
			if (entireMatch == -1)
			{
				for (int w = 0; w < alBank->inst[x]->soundCount; w++)
				{
					if (entireMatch != -1)
						break;

					if (alBank->inst[x]->sounds[w] != NULL)
					{
						
						if (!alBank->inst[x]->sounds[w]->hasWavePrevious)
							continue;

						if (CompareWavs(&alBank->inst[x]->alPercussion[y].wav, &alBank->inst[x]->sounds[w]->wavPrevious))
						{
							entireMatch = w;
							offsetsWav[x][startPercussionInListing+y] = offsetsWavPrevious[x][entireMatch];
							bookOffsetsWav[x][startPercussionInListing+y] = bookOffsetsWavPrevious[x][entireMatch];
							tblOffsets[x][startPercussionInListing+y] = tblOffsetsPrevious[x][entireMatch];
							break;
						}
					}
				}
			}

			if (entireMatch == -1)
			{
				for (int w = 0; w < alBank->inst[x]->soundCount; w++)
				{
					if (entireMatch != -1)
						break;

					if (alBank->inst[x]->sounds[w] != NULL)
					{
						if (CompareWavs(&alBank->inst[x]->alPercussion[y].wav, &alBank->inst[x]->sounds[w]->wav))
						{
							entireMatch = w;
							offsetsWav[x][startPercussionInListing+y] = offsetsWav[x][entireMatch];
							bookOffsetsWav[x][startPercussionInListing+y] = bookOffsetsWav[x][entireMatch];
							tblOffsets[x][startPercussionInListing+y] = tblOffsets[x][entireMatch];
							break;
						}
					}
				}
			}

			if (entireMatch == -1)
			{
				for (int w = 0; w < alBank->inst[x]->soundCount; w++)
				{
					if (entireMatch != -1)
						break;

					if (alBank->inst[x]->sounds[w] != NULL)
					{
						if (!alBank->inst[x]->sounds[w]->hasWaveSecondary)
							continue;

						if (CompareWavs(&alBank->inst[x]->alPercussion[y].wav, &alBank->inst[x]->sounds[w]->wavSecondary))
						{
							entireMatch = w;
							offsetsWav[x][startPercussionInListing+y] = offsetsWavSecondary[x][entireMatch];
							bookOffsetsWav[x][startPercussionInListing+y] = bookOffsetsWavSecondary[x][entireMatch];
							tblOffsets[x][startPercussionInListing+y] = tblOffsetsSecondary[x][entireMatch];
							break;
						}
					}
				}
			}

			if (entireMatch == -1)
			{				
				for (int w = 0; w < y; w++)
				{
					if (entireMatch != -1)
						break;


					if (CompareWavs(&alBank->inst[x]->alPercussion[y].wav, &alBank->inst[x]->alPercussion[w].wav))
					{
						entireMatch = w;
						offsetsWav[x][startPercussionInListing+y] = offsetsWav[x][startPercussionInListing+w];
						bookOffsetsWav[x][startPercussionInListing+y] = bookOffsetsWav[x][startPercussionInListing+w];
						tblOffsets[x][startPercussionInListing+y] = tblOffsets[x][startPercussionInListing+w];
						break;
					}
				}
			}



			if (entireMatch == -1)
			{
				int sameBook = -1;

				unsigned long loopOutputPosition = outputCtlCounter + 0x20;
				if (alBank->inst[x]->alPercussion[y].wav.adpcmWave->book != NULL)
				{						
					bookOffsetsWav[x][startPercussionInListing+y] = loopOutputPosition;

					/*if (sameBook == -1)
					{
						for (int w = 0; w < alBank->inst[x]->soundCount; w++)
						{
							if (alBank->inst[x]->sounds[w] != NULL)
							{
								if (alBank->inst[x]->sounds[w]->hasWavePrevious)
								{
									if (CompareBooks(&alBank->inst[x]->alPercussion[y].wav, &alBank->inst[x]->sounds[w]->wavPrevious))
									{
										sameBook = w;
										bookOffsetsWav[x][startPercussionInListing+y] = bookOffsetsWavPrevious[x][w];
										break;
									}
								}
							}
						}
					}

					if (sameBook == -1)
					{
						for (int w = 0; w < alBank->inst[x]->soundCount; w++)
						{
							if (alBank->inst[x]->sounds[w] != NULL)
							{
								if (CompareBooks(&alBank->inst[x]->alPercussion[y].wav, &alBank->inst[x]->sounds[w]->wav))
								{
									sameBook = w;
									bookOffsetsWav[x][startPercussionInListing+y] = bookOffsetsWav[x][w];
									break;
								}
							}
						}
					}

					if (sameBook == -1)
					{
						for (int w = 0; w < alBank->inst[x]->soundCount; w++)
						{
							if (alBank->inst[x]->sounds[w] != NULL)
							{
								if (alBank->inst[x]->sounds[w]->hasWaveSecondary)
								{
									if (CompareBooks(&alBank->inst[x]->alPercussion[y].wav, &alBank->inst[x]->sounds[w]->wavSecondary))
									{
										sameBook = w;
										bookOffsetsWav[x][startPercussionInListing+y] = bookOffsetsWavSecondary[x][w];
										break;
									}
								}
							}
						}
					}

					if (sameBook == -1)
					{
						for (int w = 0; w < y; w++)
						{
							if (CompareBooks(&alBank->inst[x]->alPercussion[y].wav, &alBank->inst[x]->alPercussion[w].wav))
							{
								sameBook = w;
								bookOffsetsWav[x][startPercussionInListing+y] = bookOffsetsWav[x][startPercussionInListing+w];
								break;
							}
						}
					}*/

					if (sameBook == -1)
					{
						loopOutputPosition = loopOutputPosition + 0x8 + ((alBank->inst[x]->alPercussion[y].wav.adpcmWave->book->npredictors * alBank->inst[x]->alPercussion[y].wav.adpcmWave->book->order) * 0x10);
						if ((loopOutputPosition % 0x10) != 0)
							loopOutputPosition = (loopOutputPosition + (0x10 - (loopOutputPosition % 0x10)));
					}

				}
				else
				{
					// Used?
					MessageBox(NULL, "Error book", "Error", NULL);
				}

				int sameLoop = -1;

				if (alBank->inst[x]->alPercussion[y].wav.adpcmWave->loop != NULL)
				{
					
					adpcmRawLoopOffsetsWav[x][startPercussionInListing+y] = loopOutputPosition;

					if (alBank->inst[x]->alPercussion[y].wav.adpcmWave->loop != NULL)
					{
						/*if (sameLoop == -1)
						{
							for (int w = 0; w < alBank->inst[x]->soundCount; w++)
							{
								if (alBank->inst[x]->sounds[w] != NULL)
								{
									if (alBank->inst[x]->sounds[w]->hasWavePrevious)
									{
										if (CompareLoops(&alBank->inst[x]->alPercussion[y].wav, &alBank->inst[x]->sounds[w]->wavPrevious))
										{
											adpcmRawLoopOffsetsWav[x][startPercussionInListing+y] = adpcmRawLoopOffsetsWavPrevious[x][w];
											sameLoop = w;
											break;
										}
									}
								}
							}
						}

						if (sameLoop == -1)
						{
							for (int w = 0; w < alBank->inst[x]->soundCount; w++)
							{
								if (alBank->inst[x]->sounds[w] != NULL)
								{
									if (CompareLoops(&alBank->inst[x]->alPercussion[y].wav, &alBank->inst[x]->sounds[w]->wav))
									{
										adpcmRawLoopOffsetsWav[x][startPercussionInListing+y] = adpcmRawLoopOffsetsWav[x][w];
										sameLoop = w;
										break;
									}
								}
							}
						}

						if (sameLoop == -1)
						{
							for (int w = 0; w < alBank->inst[x]->soundCount; w++)
							{
								if (alBank->inst[x]->sounds[w] != NULL)
								{
									if (alBank->inst[x]->sounds[w]->hasWaveSecondary)
									{
										if (CompareLoops(&alBank->inst[x]->alPercussion[y].wav, &alBank->inst[x]->sounds[w]->wavSecondary))
										{
											adpcmRawLoopOffsetsWav[x][startPercussionInListing+y] = adpcmRawLoopOffsetsWavSecondary[x][w];
											sameLoop = w;
											break;
										}
									}
								}
							}
						}

						if (sameLoop == -1)
						{
							for (int w = 0; w < y; w++)
							{
								if (CompareLoops(&alBank->inst[x]->alPercussion[y].wav, &alBank->inst[x]->alPercussion[w].wav))
								{
									sameBook = w;
									adpcmRawLoopOffsetsWav[x][startPercussionInListing+y] = adpcmRawLoopOffsetsWav[x][startPercussionInListing+w];
								}
							}
						}*/
					}
					
				}
				else
				{
					// Used?
					MessageBox(NULL, "Error loop", "Error", NULL);
				}

				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->alPercussion[y].wav.unknown1);
				outputCtlCounter += 4;

				tblOffsets[x][startPercussionInListing+y] = tblSizes[tblUniqueOffset];

				bool foundSameTbl = false;

				if (!foundSameTbl)
				{
					for (int r = 0; r < (x + 1); r++)
					{
						if (alBank->inst[x]->tblBase != alBank->inst[r]->tblBase)
							continue;

						int loopEnd = alBank->inst[r]->soundCount;
						for (int z = 0; z < loopEnd; z++)
						{
							if (alBank->inst[r]->sounds[z] != NULL)
							{
								if (alBank->inst[r]->sounds[z]->hasWavePrevious)
								{
									if (CompareTbls(&alBank->inst[x]->alPercussion[y].wav, &alBank->inst[r]->sounds[z]->wavPrevious))
									{
										foundSameTbl = true;
										tblOffsets[x][startPercussionInListing+y] = tblOffsetsPrevious[r][z];
										break;
									}
								}
							}
						}
					}
				}

				if (!foundSameTbl)
				{
					for (int r = 0; r < (x + 1); r++)
					{
						if (alBank->inst[x]->tblBase != alBank->inst[r]->tblBase)
							continue;

						int loopEnd = alBank->inst[r]->soundCount;
						for (int z = 0; z < loopEnd; z++)
						{
							if (alBank->inst[r]->sounds[z] != NULL)
							{
								if (CompareTbls(&alBank->inst[x]->alPercussion[y].wav, &alBank->inst[r]->sounds[z]->wav))
								{
									foundSameTbl = true;
									tblOffsets[x][startPercussionInListing+y] = tblOffsets[r][z];
									break;
								}
							}
						}
					}
				}

				if (!foundSameTbl)
				{
					for (int r = 0; r < (x + 1); r++)
					{
						if (alBank->inst[x]->tblBase != alBank->inst[r]->tblBase)
							continue;

						int loopEnd = alBank->inst[r]->soundCount;
						for (int z = 0; z < loopEnd; z++)
						{
							if (alBank->inst[r]->sounds[z] != NULL)
							{
								if (alBank->inst[r]->sounds[z]->hasWaveSecondary)
								{
									if (CompareTbls(&alBank->inst[x]->alPercussion[y].wav, &alBank->inst[r]->sounds[z]->wavSecondary))
									{
										foundSameTbl = true;
										tblOffsets[x][startPercussionInListing+y] = tblOffsetsSecondary[r][z];
										break;
									}
								}
							}
						}
					}
				}

				if (!foundSameTbl)
				{
					for (int r = 0; r < (x + 1); r++)
					{
						if (alBank->inst[x]->tblBase != alBank->inst[r]->tblBase)
							continue;

						int startPercussionInListingNew = alBank->inst[r]->soundCount;

						int loopEnd = y;
						if (r != x)
						{
							loopEnd = alBank->inst[r]->countPercussion;
						}
						for (int z = 0; z < loopEnd; z++)
						{
							if (CompareTbls(&alBank->inst[x]->alPercussion[y].wav, &alBank->inst[r]->alPercussion[z].wav))
							{
								foundSameTbl = true;
								tblOffsets[x][startPercussionInListing+y] = tblOffsets[r][startPercussionInListingNew+z];
								break;
							}
						}
					}
				}

				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, tblOffsets[x][startPercussionInListing+y]);
				outputCtlCounter += 4;


				if (!foundSameTbl)
				{
					unsigned long curEntrySize = alBank->inst[x]->alPercussion[y].wav.len;
					if ((alBank->inst[x]->alPercussion[y].wav.len % 0x10) != 0)
						curEntrySize += (0x10 - (alBank->inst[x]->alPercussion[y].wav.len % 0x10));

					for (int z = 0; z < alBank->inst[x]->alPercussion[y].wav.len; z++)
					{
						tblData[tblUniqueOffset][tblSizes[tblUniqueOffset] + z] = alBank->inst[x]->alPercussion[y].wav.wavData[z];
					}

					tblSizes[tblUniqueOffset] += curEntrySize;
					
					if ((tblSizes[tblUniqueOffset] % 0x10) != 0)
					{
						tblSizes[tblUniqueOffset] += (0x10 - (tblSizes[tblUniqueOffset] % 0x10));
					}
				}

				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, (adpcmRawLoopOffsetsWav[x][startPercussionInListing+y] - soundDataOffset));
				outputCtlCounter += 4;

				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, (bookOffsetsWav[x][startPercussionInListing+y] - soundDataOffset));
				outputCtlCounter += 4;

				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->alPercussion[y].wav.len);
				outputCtlCounter += 4;

				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->alPercussion[y].wav.unknown2);
				outputCtlCounter += 4;

				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->alPercussion[y].wav.unknown3);
				outputCtlCounter += 4;

				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->alPercussion[y].wav.unknown4);
				outputCtlCounter += 4;

				if (sameBook == -1)
				{
					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->alPercussion[y].wav.adpcmWave->book->order);
					outputCtlCounter += 4;
					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->alPercussion[y].wav.adpcmWave->book->npredictors);
					outputCtlCounter += 4;

					for (int z = 0; z < (alBank->inst[x]->alPercussion[y].wav.adpcmWave->book->order * alBank->inst[x]->alPercussion[y].wav.adpcmWave->book->npredictors * 8); z++)
					{
						temporaryCtlBuffer[outputCtlCounter++] = ((alBank->inst[x]->alPercussion[y].wav.adpcmWave->book->predictors[z] >> 8) & 0xFF);
						temporaryCtlBuffer[outputCtlCounter++] = ((alBank->inst[x]->alPercussion[y].wav.adpcmWave->book->predictors[z]) & 0xFF);
					}

					if ((outputCtlCounter % 0x10) != 0)
						outputCtlCounter = outputCtlCounter + (0x10 - (outputCtlCounter % 0x10));
				}

				if (sameLoop == -1)
				{

					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->alPercussion[y].wav.adpcmWave->loop->start);
					outputCtlCounter += 4;
					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->alPercussion[y].wav.adpcmWave->loop->end);
					outputCtlCounter += 4;
					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->alPercussion[y].wav.adpcmWave->loop->count);
					outputCtlCounter += 4;
					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->alPercussion[y].wav.adpcmWave->loop->unknown1);
					outputCtlCounter += 4;

					if (alBank->inst[x]->alPercussion[y].wav.adpcmWave->loop->start != 0)
					{
						for (int z = 0; z < 0x10; z++)
						{
							temporaryCtlBuffer[outputCtlCounter++] = ((alBank->inst[x]->alPercussion[y].wav.adpcmWave->loop->state[z] >> 8) & 0xFF);
							temporaryCtlBuffer[outputCtlCounter++] = ((alBank->inst[x]->alPercussion[y].wav.adpcmWave->loop->state[z]) & 0xFF);
						}
					}

					if ((outputCtlCounter % 0x10) != 0)
						outputCtlCounter = outputCtlCounter + (0x10 - (outputCtlCounter % 0x10));
				}
			}
		}

		for (int y = 0; y < alBank->inst[x]->soundCount; y++)
		{
			offsetsEnv[x][y] = (outputCtlCounter - soundDataOffset);

			if (alBank->inst[x]->sounds[y] != NULL)
			{

				bool foundPast = false;

				for (int z = 0; z < y; z++)
				{
					if (alBank->inst[x]->sounds[z] != NULL)
					{
						if (
							(alBank->inst[x]->sounds[y]->env.attackTime == alBank->inst[x]->sounds[z]->env.attackTime)
							&& (alBank->inst[x]->sounds[y]->env.decayTime == alBank->inst[x]->sounds[z]->env.decayTime)
							&& (alBank->inst[x]->sounds[y]->env.attackVolume == alBank->inst[x]->sounds[z]->env.attackVolume)
							&& (alBank->inst[x]->sounds[y]->env.decayVolume == alBank->inst[x]->sounds[z]->env.decayVolume)
							&& (alBank->inst[x]->sounds[y]->env.offset != 0xFFFFFFFF)
							&& (alBank->inst[x]->sounds[y]->env.offset == alBank->inst[x]->sounds[z]->env.offset)
							)
						{
							foundPast = true;
							offsetsEnv[x][y] = offsetsEnv[x][z];
							break;
						}
					}
				}

				if (foundPast)
					continue;

				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->env.attackTime);
				outputCtlCounter += 4;
				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->env.decayTime);
				outputCtlCounter += 4;
				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->env.attackVolumeLong);
				outputCtlCounter += 4;
				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->env.decayVolumeLong);
				outputCtlCounter += 4;
			}
		}

		for (int y = 0; y < alBank->inst[x]->countPercussion; y++)
		{
			offsetsEnv[x][startPercussionInListing+y] = (outputCtlCounter - soundDataOffset);

			bool foundPast = false;

			for (int z = 0; z < alBank->inst[x]->soundCount; z++)
			{
				if (alBank->inst[x]->sounds[z] != NULL)
				{
					if (
						(alBank->inst[x]->alPercussion[y].env.attackTime == alBank->inst[x]->sounds[z]->env.attackTime)
						&& (alBank->inst[x]->alPercussion[y].env.decayTime == alBank->inst[x]->sounds[z]->env.decayTime)
						&& (alBank->inst[x]->alPercussion[y].env.attackVolume == alBank->inst[x]->sounds[z]->env.attackVolume)
						&& (alBank->inst[x]->alPercussion[y].env.decayVolume == alBank->inst[x]->sounds[z]->env.decayVolume)
						&& (alBank->inst[x]->alPercussion[y].env.offset != 0xFFFFFFFF)
						&& (alBank->inst[x]->alPercussion[y].env.offset == alBank->inst[x]->sounds[z]->env.offset)
						)
					{
						foundPast = true;
						offsetsEnv[x][startPercussionInListing+y] = offsetsEnv[x][z];
						break;
					}
				}
			}

			if (foundPast)
				continue;

			for (int z = 0; z < y; z++)
			{
				if (
					(alBank->inst[x]->alPercussion[y].env.attackTime == alBank->inst[x]->alPercussion[z].env.attackTime)
					&& (alBank->inst[x]->alPercussion[y].env.decayTime == alBank->inst[x]->alPercussion[z].env.decayTime)
					&& (alBank->inst[x]->alPercussion[y].env.attackVolume == alBank->inst[x]->alPercussion[z].env.attackVolume)
					&& (alBank->inst[x]->alPercussion[y].env.decayVolume == alBank->inst[x]->alPercussion[z].env.decayVolume)
					&& (alBank->inst[x]->alPercussion[y].env.offset != 0xFFFFFFFF)
					&& (alBank->inst[x]->alPercussion[y].env.offset == alBank->inst[x]->alPercussion[z].env.offset)
					)
				{
					foundPast = true;
					offsetsEnv[x][startPercussionInListing+y] = offsetsEnv[x][startPercussionInListing+z];
					break;
				}
			}

			if (foundPast)
				continue;

			WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->alPercussion[y].env.attackTime);
			outputCtlCounter += 4;
			WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->alPercussion[y].env.decayTime);
			outputCtlCounter += 4;
			WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->alPercussion[y].env.attackVolumeLong);
			outputCtlCounter += 4;
			WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->alPercussion[y].env.decayVolumeLong);
			outputCtlCounter += 4;
		}

		for (int y = 0; y < alBank->inst[x]->soundCount; y++)
		{
			if (alBank->inst[x]->sounds[y] != NULL)
			{
				WriteLongToBuffer(temporaryCtlBuffer, soundOffsetSpot + (y * 0x4), (outputCtlCounter - soundDataOffset));

				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->unknown1);
				outputCtlCounter += 4;

				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, offsetsEnv[x][y]);
				outputCtlCounter += 4;

				if (alBank->inst[x]->sounds[y]->hasWavePrevious)
				{
					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, offsetsWavPrevious[x][y]);
					outputCtlCounter += 4;
				}
				else
				{
					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, 0x00000000);
					outputCtlCounter += 4;
				}

				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->unknown2);
				outputCtlCounter += 4;

				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, offsetsWav[x][y]);
				outputCtlCounter += 4;

				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->unknown3);
				outputCtlCounter += 4;

				if (alBank->inst[x]->sounds[y]->hasWaveSecondary)
				{
					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, offsetsWavSecondary[x][y]);
					outputCtlCounter += 4;
				}
				else
				{
					WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, 0x00000000);
					outputCtlCounter += 4;
				}

				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->sounds[y]->unknown4);
				outputCtlCounter += 4;
			}
			else
			{
				WriteLongToBuffer(temporaryCtlBuffer, soundOffsetSpot + (y * 0x4), 0x00000000);
			}
		}

		if ((outputCtlCounter % 0x10) != 0)
			outputCtlCounter += (0x10 - (outputCtlCounter % 0x10));

		

		if (alBank->inst[x]->countPercussion == 0)
		{
			WriteLongToBuffer(temporaryCtlBuffer, soundDataOffset, 0x00000000);
		}
		else
		{
			unsigned long startPercussionDataOffsets = (outputCtlCounter  - soundDataOffset);
			for (int y = 0; y < alBank->inst[x]->countPercussion; y++)
			{
				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->alPercussion[y].unknown1);
				outputCtlCounter += 4;

				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, offsetsWav[x][alBank->inst[x]->soundCount + y]);
				outputCtlCounter += 4;

				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, alBank->inst[x]->alPercussion[y].unknown2);
				outputCtlCounter += 4;

				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, offsetsEnv[x][alBank->inst[x]->soundCount + y]);
				outputCtlCounter += 4;
			}

			if ((outputCtlCounter % 0x10) != 0)
				outputCtlCounter += (0x10 - (outputCtlCounter % 0x10));

			WriteLongToBuffer(temporaryCtlBuffer, soundDataOffset, (outputCtlCounter  - soundDataOffset));

			for (int y = 0; y < alBank->inst[x]->countPercussion; y++)
			{
				WriteLongToBuffer(temporaryCtlBuffer, outputCtlCounter, startPercussionDataOffsets + (y * 0x10));
				outputCtlCounter += 4;
			}

			if ((outputCtlCounter % 0x10) != 0)
				outputCtlCounter += (0x10 - (outputCtlCounter % 0x10));
		}

		// ctl bank size
		WriteLongToBuffer(temporaryCtlBuffer, 0x4 + (0x8 * x) + 0x4, (outputCtlCounter - instrumentALLookup[x]));
	}

	unsigned long outputTblCounter = 0;

	WriteShortToBuffer(temporaryTblBuffer, outputTblCounter, 0x0002);
	outputTblCounter += 2;

	WriteShortToBuffer(temporaryTblBuffer, outputTblCounter, alBank->count);
	outputTblCounter += 2;

	for (int x = 0; x < alBank->count; x++)
	{
		// offset to bank later in file
		outputTblCounter += 8;
	}

	if ((outputTblCounter % 0x10) != 0)
		outputTblCounter += ((0x10 - (outputTblCounter % 0x10)));

	unsigned long endTblHeader = outputTblCounter;

	unsigned long* tblNewOffsets = new unsigned long[numberUniqueTblSets];
	for (int x = 0; x < numberUniqueTblSets; x++)
	{
		tblNewOffsets[x] = outputTblCounter;
		memcpy(&temporaryTblBuffer[outputTblCounter], tblData[x], tblSizes[x]);

		outputTblCounter += tblSizes[x];

		if ((outputTblCounter % 0x10) != 0)
			outputTblCounter += ((0x10 - (outputTblCounter % 0x10)));
	}

	for (int x = 0; x < alBank->count; x++)
	{
		for (int y = 0; y < numberUniqueTblSets; y++)
		{
			if (alBank->inst[x]->tblBase == tblOriginalOffsets[y])
			{
				WriteLongToBuffer(temporaryTblBuffer, 0x4 + (x * 0x8), tblNewOffsets[y]);
				WriteLongToBuffer(temporaryTblBuffer, 0x4 + (x * 0x8) + 0x4, tblSizes[y]);
				break;
			}
		}
	}

	delete [] tblNewOffsets;
	delete [] tblOriginalOffsets;
	delete [] tblSizes;
	
	for (int x = 0; x < numberUniqueTblSets; x++)
	{
		delete [] tblData[x];
	}
	delete[] tblData;

	for (int x = 0; x < alBank->count; x++)
	{
		delete [] instrumentSoundStartLookup[x];
		delete [] offsetsEnv[x];;
		delete [] offsetsWav[x];;
		delete [] bookOffsetsWav[x];;
		delete [] adpcmRawLoopOffsetsWav[x];;
		delete [] tblOffsets[x];;
		
		delete [] offsetsWavSecondary[x];;
		delete [] bookOffsetsWavSecondary[x];;
		delete [] adpcmRawLoopOffsetsWavSecondary[x];;
		delete [] tblOffsetsSecondary[x];;

		delete [] offsetsWavPrevious[x];;
		delete [] bookOffsetsWavPrevious[x];;
		delete [] adpcmRawLoopOffsetsWavPrevious[x];;
		delete [] tblOffsetsPrevious[x];;
	}

	delete [] instrumentSoundStartLookup;
	delete [] instrumentALLookup;
	delete [] offsetsEnv;
	delete [] offsetsWav;
	delete [] bookOffsetsWav;
	delete [] adpcmRawLoopOffsetsWav;
	delete [] tblOffsets;

	delete [] offsetsWavSecondary;
	delete [] bookOffsetsWavSecondary;
	delete [] adpcmRawLoopOffsetsWavSecondary;
	delete [] tblOffsetsSecondary;

	delete [] offsetsWavPrevious;
	delete [] bookOffsetsWavPrevious;
	delete [] adpcmRawLoopOffsetsWavPrevious;
	delete [] tblOffsetsPrevious;
	
	ctl = temporaryCtlBuffer;
	tbl = temporaryTblBuffer;

	ctlSize = outputCtlCounter;
	tblSize = outputTblCounter;
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


		
		if ((outputCtlCounter % 0x10) != 0)
		{
			outputCtlCounter = outputCtlCounter + (0x10 - (outputCtlCounter % 0x10));
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
	WriteShortToBuffer(temporaryCtlBuffer, outputCtlCounter, (alBank->samplerate * 2));
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
				if (alBank->inst[x]->alPercussion != NULL)
				{
					for (int y = 0; y < alBank->inst[x]->countPercussion; y++)
					{
						if (alBank->inst[x]->alPercussion[y].wav.type == AL_ADPCM_WAVE)
						{
							if (alBank->inst[x]->alPercussion[y].wav.adpcmWave->loop != NULL)
							{
								delete alBank->inst[x]->alPercussion[y].wav.adpcmWave->loop;
							}
							if (alBank->inst[x]->alPercussion[y].wav.adpcmWave->book != NULL)
							{
								delete [] alBank->inst[x]->alPercussion[y].wav.adpcmWave->book->predictors;
								delete alBank->inst[x]->alPercussion[y].wav.adpcmWave->book;
							}
							delete alBank->inst[x]->alPercussion[y].wav.adpcmWave;
						}
						else
						{
							if (alBank->inst[x]->alPercussion[y].wav.rawWave->loop != NULL)
							{
								delete alBank->inst[x]->alPercussion[y].wav.rawWave->loop;
							}
							delete alBank->inst[x]->alPercussion[y].wav.rawWave;
						}

						if (alBank->inst[x]->alPercussion[y].wav.wavData != NULL)
							delete [] alBank->inst[x]->alPercussion[y].wav.wavData;
					}

					delete [] alBank->inst[x]->alPercussion;
				}
				if (alBank->inst[x]->sounds != NULL)
				{
					for (int y = 0; y < alBank->inst[x]->soundCount; y++)
					{
						if (alBank->inst[x]->sounds[y] != NULL)
						{
							if (alBank->inst[x]->sounds[y]->wav.type == AL_ADPCM_WAVE)
							{
								if (alBank->inst[x]->sounds[y]->wav.adpcmWave->loop != NULL)
								{
									delete alBank->inst[x]->sounds[y]->wav.adpcmWave->loop;
								}
								if (alBank->inst[x]->sounds[y]->wav.adpcmWave->book != NULL)
								{
									delete [] alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors;
									delete alBank->inst[x]->sounds[y]->wav.adpcmWave->book;
								}
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




							if (alBank->inst[x]->sounds[y]->hasWavePrevious)
							{
								if (alBank->inst[x]->sounds[y]->wavPrevious.type == AL_ADPCM_WAVE)
								{
									if (alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop != NULL)
									{
										delete alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop;
									}
									delete [] alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->predictors;
									delete alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book;
									delete alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave;
								}
								else
								{
									if (alBank->inst[x]->sounds[y]->wavPrevious.rawWave->loop != NULL)
									{
										delete alBank->inst[x]->sounds[y]->wavPrevious.rawWave->loop;
									}
									delete alBank->inst[x]->sounds[y]->wavPrevious.rawWave;
								}

								if (alBank->inst[x]->sounds[y]->wavPrevious.wavData != NULL)
									delete [] alBank->inst[x]->sounds[y]->wavPrevious.wavData;
							}

							if (alBank->inst[x]->sounds[y]->hasWaveSecondary)
							{
								if (alBank->inst[x]->sounds[y]->wavSecondary.type == AL_ADPCM_WAVE)
								{
									if (alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop != NULL)
									{
										delete alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop;
									}
									delete [] alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->predictors;
									delete alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book;
									delete alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave;
								}
								else
								{
									if (alBank->inst[x]->sounds[y]->wavSecondary.rawWave->loop != NULL)
									{
										delete alBank->inst[x]->sounds[y]->wavSecondary.rawWave->loop;
									}
									delete alBank->inst[x]->sounds[y]->wavSecondary.rawWave;
								}

								if (alBank->inst[x]->sounds[y]->wavSecondary.wavData != NULL)
									delete [] alBank->inst[x]->sounds[y]->wavSecondary.wavData;
							}


							delete alBank->inst[x]->sounds[y];
						}
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


ALBank* CN64AIFCAudio::ReadAudioTurok(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, int bankTrueOffset, unsigned long mask, unsigned char* ROM, int bankNumber)
{
	ALBank* alBank = ReadAudio(&ROM[0], ctlSize, ctlOffset, tbl, bankTrueOffset, mask, bankNumber);
	alBank->soundBankFormat = TUROKFORMAT;
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioN64PtrWavetableV1(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl)
{
	unsigned short numberBands = 1;
	unsigned long bankOffset = ctlOffset + 0x14;

	// header size 0x14
	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = N64PTRWAVETABLETABLEV1;
	alBank->count = (unsigned short)CharArrayToLong(&ctl[ctlOffset+0x10]);
	alBank->flags = 0x0000;
	alBank->pad = 0x0;
	alBank->samplerate = 22050;
	alBank->percussion = 0x000;
	
	alBank->inst = NULL;

	if ((alBank->flags == 0x0000) || (alBank->flags == 0x0100)) // offset
	{
		alBank->inst = new ALInst*[alBank->count];

		for (int x = 0; x < alBank->count; x++)
		{
			alBank->inst[x] = new ALInst();
			alBank->inst[x]->samplerate = 0;
			alBank->inst[x]->sounds = NULL;
			alBank->inst[x]->alPercussion = NULL;
		}

		for (int x = 0; x < alBank->count; x++)
		{
			unsigned long offsetInstrument = CharArrayToLong(&ctl[bankOffset + x*4]);
			if (offsetInstrument == 0x0000)
			{
				alBank->inst[x]->soundCount = 0;
				alBank->inst[x]->sounds = NULL;
				continue;
			}

			if (alBank->flags == 0x0100)
				offsetInstrument = offsetInstrument & 0xFFFFFF;
			else
				offsetInstrument = ctlOffset + offsetInstrument;

			if ((alBank->inst[x]->flags == 0x0) || (alBank->inst[x]->flags == 0x1))
			{	
				alBank->inst[x]->soundCount = 1;

				alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

				for (int y = 0; y < alBank->inst[x]->soundCount; y++)
				{
					alBank->inst[x]->sounds[y] = new ALSound();
					alBank->inst[x]->sounds[y]->wav.wavData = NULL;
				}

				for (int y = 0; y < alBank->inst[x]->soundCount; y++)
				{
					alBank->inst[x]->sounds[y]->flags = 0x0000;

					if ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1))
					{
						alBank->inst[x]->sounds[y]->wav.base = CharArrayToLong(&ctl[offsetInstrument + 0x0]);
						alBank->inst[x]->sounds[y]->wav.len = CharArrayToLong(&ctl[offsetInstrument + 0x4]);
						alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];

						for (int  r = 0; r < alBank->inst[x]->sounds[y]->wav.len; r++)
						{
							alBank->inst[x]->sounds[y]->wav.wavData[r] = tbl[alBank->inst[x]->sounds[y]->wav.base + r];
						}
						

						alBank->inst[x]->sounds[y]->wav.type = AL_ADPCM_WAVE;
						alBank->inst[x]->sounds[y]->wav.flags = 0x0000;
						// MUST PAD to 4s

						if ((alBank->inst[x]->sounds[y]->wav.type == AL_ADPCM_WAVE) && ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1)))
						{
							alBank->inst[x]->sounds[y]->wav.adpcmWave = new ALADPCMWaveInfo();
							
							unsigned long predictorOffset = CharArrayToLong(&ctl[offsetInstrument + 0x10]);
							unsigned long loopOffset = CharArrayToLong(&ctl[offsetInstrument + 0xC]);

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

						
							if (predictorOffset != 0x00000000)
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
							else
							{
								MessageBox(NULL, "Error", "Missing predictors in ALSound wav", NULL);
								alBank->inst[x]->sounds[y]->wav.adpcmWave->book = NULL;
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


ALBank* CN64AIFCAudio::ReadRNCAudio(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, int bankNumber)
{
	unsigned char* outputDecompressed = new unsigned char[0x1000000];
	int fileSizeCompressed = -1;
	RncDecoder rnc;
	rnc.unpackM1(&ctl[ctlOffset], outputDecompressed, 0x0, fileSizeCompressed);
	ALBank* alBank = ReadAudio(&outputDecompressed[0], fileSizeCompressed, 0, tbl, 0, 0, bankNumber);
	alBank->soundBankFormat = STANDARDRNCCOMPRESSED;
	delete [] outputDecompressed;
	return alBank;
}

ALBank* CN64AIFCAudio::ReadRNXAudio(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, int bankNumber)
{
	unsigned char* outputDecompressedA = new unsigned char[0x1000000];
	unsigned char* outputDecompressedB = new unsigned char[0x1000000];
	int fileSizeCompressed = -1;
	RncDecoder rnc;
	int size1 = rnc.unpackM1(&ctl[ctlOffset], outputDecompressedA, 0x0, fileSizeCompressed);

	int nextCtlSpot = ctlOffset + fileSizeCompressed;
	nextCtlSpot += (8 - (fileSizeCompressed % 8));

	nextCtlSpot += 0x10;

	int size2 = rnc.unpackM1(&ctl[nextCtlSpot], outputDecompressedB, 0x0, fileSizeCompressed);

	unsigned char* outputDecompressed = new unsigned char[0x1000000];
	for (int x = 0; x < size1; x++)
	{
		outputDecompressed[x] = outputDecompressedA[x];
	}

	for (int x = 0; x < size2; x++)
	{
		outputDecompressed[x+size1] = outputDecompressedB[x];
	}

	delete [] outputDecompressedA;
	delete [] outputDecompressedB;

	ALBank* alBank = ReadAudio(&outputDecompressed[0], fileSizeCompressed, 0, tbl, 0, 0, bankNumber);
	alBank->soundBankFormat = STANDARDRNXCOMPRESSED;
	delete [] outputDecompressed;
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioN64SN64(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, int numberInstruments, unsigned long mask)
{

	unsigned short numberBands = 1;
	unsigned long bankOffset = ctlOffset + CharArrayToLong(&ctl[ctlOffset+0x2C]);

	// header size 0x30
	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = SN64;
	alBank->count = numberInstruments;

	alBank->flags = 0x0000;
	alBank->pad = 0x0;
	alBank->samplerate = 22050;
	alBank->percussion = 0x0000;

	unsigned long numberElementsSectionOne = 0;
	for (int x = 0; x < numberInstruments; x++)
	{
		unsigned long numberCheck = (CharArrayToLong(&ctl[ctlOffset + x*4]) & mask);
		if (numberCheck > numberElementsSectionOne)
			numberElementsSectionOne = numberCheck;
	}
	numberElementsSectionOne++;

	unsigned long dataOffset = ctlOffset + (numberInstruments * 0x4);
	if ((dataOffset % 0x8) != 0)
		dataOffset += 4;

	unsigned long numberElementsSectionTwo = 0;
	for (int x = 0; x < numberElementsSectionOne; x++)
	{
		unsigned long numberCheck = (CharArrayToShort(&ctl[dataOffset + x*0x14 + 0xA]) & mask);
		if (numberCheck > numberElementsSectionTwo)
			numberElementsSectionTwo = numberCheck;
	}
	numberElementsSectionTwo++;

	dataOffset = dataOffset + (numberElementsSectionOne * 0x14);
	if ((dataOffset % 0x8) != 0)
		dataOffset += 4;

	unsigned long numberLoops = 0x00000000;
	bool foundOneLoop = false;
	for (int x = 0; x < numberElementsSectionTwo; x++)
	{
		unsigned long numberCheck = (CharArrayToLong(&ctl[dataOffset + (x * 0x18) + 0x10]));
		if (numberCheck == 0xFFFFFFFF)
			continue;
		else
			foundOneLoop = true;

		if (numberCheck > numberLoops)
		{
			numberLoops = numberCheck;
		}
	}
	if (foundOneLoop)
		numberLoops++;
	
	alBank->inst = NULL;

	if ((alBank->flags == 0x0000) || (alBank->flags == 0x0100)) // offset
	{
		alBank->inst = new ALInst*[alBank->count];

		for (int x = 0; x < alBank->count; x++)
		{
			alBank->inst[x] = new ALInst();
			alBank->inst[x]->samplerate = 0;
			alBank->inst[x]->sounds = NULL;
			alBank->inst[x]->alPercussion = NULL;
		}

		for (int x = 0; x < alBank->count; x++)
		{
			unsigned long instrumentNumber = (CharArrayToLong(&ctl[ctlOffset + x*4]) & mask);

			alBank->inst[x]->soundCount = 1;
			alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

			for (int y = 0; y < alBank->inst[x]->soundCount; y++)
			{
				alBank->inst[x]->sounds[y] = new ALSound();
				alBank->inst[x]->sounds[y]->wav.wavData = NULL;
			}

			unsigned long dataOffset = ctlOffset + (numberInstruments * 0x4);
			if ((dataOffset % 0x8) != 0)
				dataOffset += 4;
			unsigned long dataNumber = (CharArrayToShort(&ctl[dataOffset + (instrumentNumber * 0x14)+ 0xA]) & mask);

			dataOffset = dataOffset + (numberElementsSectionOne * 0x14);
			if ((dataOffset % 0x8) != 0)
				dataOffset += 4;
			unsigned long rawDataOffset = dataOffset + (dataNumber * 0x18);

			unsigned long loopDataOffset;
			unsigned long predictorDataOffset;

			loopDataOffset = dataOffset + (numberElementsSectionTwo * 0x18) + 0x8;
			predictorDataOffset = dataOffset + (numberElementsSectionTwo * 0x18) + 0x8 + (numberLoops * 0x30);


			for (int y = 0; y < alBank->inst[x]->soundCount; y++)
			{
				alBank->inst[x]->sounds[y]->flags = 0x0000;

				if ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1))
				{
					alBank->inst[x]->sounds[y]->wav.base = CharArrayToLong(&ctl[rawDataOffset + 0x0]);
					alBank->inst[x]->sounds[y]->wav.len = CharArrayToLong(&ctl[rawDataOffset + 0x4]);
					alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];

					for (int  r = 0; r < alBank->inst[x]->sounds[y]->wav.len; r++)
					{
						alBank->inst[x]->sounds[y]->wav.wavData[r] = tbl[alBank->inst[x]->sounds[y]->wav.base + r];
					}
					

					alBank->inst[x]->sounds[y]->wav.type = AL_ADPCM_WAVE;
					alBank->inst[x]->sounds[y]->wav.flags = 0x0000;
					// MUST PAD to 4s

					if ((alBank->inst[x]->sounds[y]->wav.type == AL_ADPCM_WAVE) && ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1)))
					{
						alBank->inst[x]->sounds[y]->wav.adpcmWave = new ALADPCMWaveInfo();
						
						unsigned long predictorOffset = predictorDataOffset + 0x108 * dataNumber;
						unsigned long loopOffset = CharArrayToLong(&ctl[rawDataOffset + 0x10]);

						if (loopOffset != 0xFFFFFFFF)
						{
							loopOffset = loopDataOffset + loopOffset;
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

					
						if (predictorOffset != 0x00000000)
						{
							alBank->inst[x]->sounds[y]->wav.adpcmWave->book = new ALADPCMBook();
							alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order = CharArrayToLong(&ctl[predictorOffset + 0x0]);
							alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors = CharArrayToLong(&ctl[predictorOffset + 0x4]);
							alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors = new signed short[alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8];

							for (int z = 0; z < alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8; z++)
							{
								alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[z] = (signed short)CharArrayToShort(&ctl[predictorOffset + 0x8 + z * 2]);
							}
						}
						else
						{
							MessageBox(NULL, "Error", "Missing predictors in ALSound wav", NULL);
							alBank->inst[x]->sounds[y]->wav.adpcmWave->book = NULL;
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
	}
	else
	{
		DisposeALBank(alBank);
		MessageBox(NULL, "Error", "Unsupported type in ALBank", NULL);
		return NULL;
	}

	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioN64SN64Zlib(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, int numberInstruments, unsigned long mask)
{
	int fileSizeCompressed = -1;
	GECompression gedecompress;
	gedecompress.SetGame(MORTALKOMBAT);
	gedecompress.SetCompressedBuffer(&ctl[ctlOffset], ctlSize);
	int fileSizeUncompressed;
	unsigned char* outputDecompressed = gedecompress.OutputDecompressedBuffer(fileSizeUncompressed, fileSizeCompressed);

	ALBank* alBank = NULL;
	if (outputDecompressed != NULL)
	{
		unsigned long fileSizeUncompressedLong = fileSizeUncompressed;
		alBank = ReadAudioN64SN64(&outputDecompressed[0], fileSizeUncompressedLong, 0, tbl, numberInstruments, mask);
		alBank->soundBankFormat = ZLIBSN64;
		delete [] outputDecompressed;
	}
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioN64PtrWavetableBlitz(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl)
{
	unsigned char* outputDecompressed = new unsigned char[0x1000000];
	int fileSizeCompressed = -1;
	CBlitzDecoder blitzdec;
	blitzdec.dec(&ctl[ctlOffset], outputDecompressed, fileSizeCompressed);
	ALBank* alBank = ReadAudioN64PtrWavetableV2(&outputDecompressed[0], ctlSize, 0, tbl);
	alBank->soundBankFormat = N64PTRWAVETABLETABLEV2BLITZ;
	delete [] outputDecompressed;
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioN64Titus(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl)
{
	int fileSizeCompressed = -1;
	SupermanDecoder decode;
	int expectedSize = decode.header(&ctl[ctlOffset], fileSizeCompressed);
	unsigned char* outputDecompressed = new unsigned char[expectedSize];
	
	decode.dec(&ctl[ctlOffset+0x11], fileSizeCompressed, expectedSize, outputDecompressed);
	ALBank* alBank = ReadAudio(&outputDecompressed[0], ctlSize, 0, tbl, 0x0, 0x0, 0);
	alBank->soundBankFormat = TITUS;
	delete [] outputDecompressed;
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioN64PtrWavetableV2YAY0(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl)
{
	unsigned char* outputDecompressed = new unsigned char[0x1000000];
	int fileSizeCompressed = -1;
	YAY0 yay0;
	yay0.decodeAll(&ctl[ctlOffset], outputDecompressed, fileSizeCompressed);
	ALBank* alBank = ReadAudioN64PtrWavetableV2(&outputDecompressed[0], ctlSize, 0, tbl);
	alBank->soundBankFormat = N64PTRWAVETABLETABLEV2YAY0;
	delete [] outputDecompressed;
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioBnkB(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl)
{
	unsigned short numberBands = 1;
	int startOffset;
	if (ctl[ctlOffset+4] == 0x4)
		startOffset = 0x14;
	else if (ctl[ctlOffset+4] == 0x2)
		startOffset = 0xC;
	else
		startOffset = 0x14;
	unsigned long bankOffset = ctlOffset + startOffset;

	// header size 0x30
	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = BNKB;
	alBank->count = (unsigned short)CharArrayToShort(&ctl[ctlOffset+0x6]);
	alBank->flags = 0x0000;
	alBank->pad = 0x0;
	alBank->samplerate = 8000;
	alBank->percussion = 0x000;

	unsigned long tblOffset = ctlOffset + CharArrayToLong(&ctl[ctlOffset+8]);

	alBank->inst = NULL;

	if ((alBank->flags == 0x0000) || (alBank->flags == 0x0100)) // offset
	{
		alBank->inst = new ALInst*[alBank->count];

		for (int x = 0; x < alBank->count; x++)
		{
			alBank->inst[x] = new ALInst();
			alBank->inst[x]->samplerate = 0;
			alBank->inst[x]->sounds = NULL;
			alBank->inst[x]->alPercussion = NULL;
		}

		for (int x = 0; x < alBank->count; x++)
		{
			unsigned long offsetInstrument = CharArrayToLong(&ctl[bankOffset + x*4]);

			if (offsetInstrument == 0x0000)
			{
				alBank->inst[x]->soundCount = 0;
				alBank->inst[x]->sounds = NULL;
				continue;
			}

			offsetInstrument = bankOffset + x*4 + offsetInstrument;

			alBank->inst[x]->soundCount = 1;

			alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

			for (int y = 0; y < alBank->inst[x]->soundCount; y++)
			{
				alBank->inst[x]->sounds[y] = new ALSound();
				alBank->inst[x]->sounds[y]->wav.wavData = NULL;
			}

			for (int y = 0; y < alBank->inst[x]->soundCount; y++)
			{
				alBank->inst[x]->sounds[y]->wav.base = 0;
				unsigned long predictorOffset = 0;
				for (int z = (offsetInstrument + 2); z < (offsetInstrument + 0x100); z++)
				{
					if (CharArrayToShort(&ctl[z]) == 0x8804)
					{
						alBank->inst[x]->sounds[y]->wav.base = CharArrayToLong(&ctl[z + 2]);
						break;
					}
					else if ((CharArrayToShort(&ctl[z]) == 0x5054) || (CharArrayToLong(&ctl[z]) == 0xFF000000))
					{
						break;
					}
				}
				for (int z = (offsetInstrument + 2); z < (offsetInstrument + 0x100); z++)
				{
					if ((CharArrayToShort(&ctl[z]) == 0x8F88) && (ctl[z+2] == 0x00))
					{
						predictorOffset = z + 2;
						break;
					}
					else if ((CharArrayToShort(&ctl[z]) == 0x5054) || (CharArrayToLong(&ctl[z]) == 0xFF000000))
					{
						break;
					}
				}
				alBank->inst[x]->sounds[y]->wav.len = 0x7000;

				alBank->inst[x]->sounds[y]->wav.type = AL_ADPCM_WAVE;
				alBank->inst[x]->sounds[y]->wav.flags = 0x0000;
				// MUST PAD to 4s

				if (predictorOffset != 0x00000000)
				{
					alBank->inst[x]->sounds[y]->wav.type = AL_ADPCM_WAVE;
					alBank->inst[x]->sounds[y]->wav.adpcmWave = new ALADPCMWaveInfo();

					alBank->inst[x]->sounds[y]->wav.adpcmWave->loop = NULL;
					alBank->inst[x]->sounds[y]->wav.adpcmWave->book = new ALADPCMBook();
					alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order = CharArrayToLong(&ctl[predictorOffset + 0x0]);
					alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors = CharArrayToLong(&ctl[predictorOffset + 0x4]);
					alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors = new signed short[alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8];

					for (int z = 0; z < alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8; z++)
					{
						alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[z] = (signed short)CharArrayToShort(&ctl[predictorOffset + 0x8 + z * 2]);
					}

					for (int z = predictorOffset + ((alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order + alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors) * 0x10); z < (predictorOffset + ((alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order + alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors) * 0x10) + 0x7C); z++)
					{
						if (CharArrayToShort(&ctl[z]) == 0x8C02)
						{
							alBank->inst[x]->sounds[y]->wav.len = CharArrayToShort(&ctl[z+2]);
							break;
						}
						else if ((CharArrayToShort(&ctl[z]) == 0x5054) || (CharArrayToLong(&ctl[z]) == 0xFF000000))
						{
							break;
						}
					}
					alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];

					for (int  r = 0; r < alBank->inst[x]->sounds[y]->wav.len; r++)
					{
						alBank->inst[x]->sounds[y]->wav.wavData[r] = tbl[alBank->inst[x]->sounds[y]->wav.base + r];
					}
				}
				else
				{
					alBank->inst[x]->sounds[y]->wav.type = AL_RAW16_WAVE;
					alBank->inst[x]->sounds[y]->wav.rawWave = new ALRAWWaveInfo();
					alBank->inst[x]->sounds[y]->wav.rawWave->loop = NULL;
				
					for (int z = (offsetInstrument + 2); z < (offsetInstrument + 0x100); z++)
					{
						if (CharArrayToShort(&ctl[z]) == 0x8C02)
						{
							alBank->inst[x]->sounds[y]->wav.len = CharArrayToShort(&ctl[z+2]);
							break;
						}
						else if ((CharArrayToShort(&ctl[z]) == 0x5054) || (CharArrayToLong(&ctl[z]) == 0xFF000000))
						{
							break;
						}
					}
					alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];

					for (int  r = 0; r < alBank->inst[x]->sounds[y]->wav.len; r++)
					{
						alBank->inst[x]->sounds[y]->wav.wavData[r] = tbl[alBank->inst[x]->sounds[y]->wav.base + r];
					}
				}
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

ALBank* CN64AIFCAudio::ReadAudioN64MKMythologies(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, unsigned long uncompressedSize)
{
	unsigned char* outputDecompressed = new unsigned char[0x1000000];
	int fileSizeCompressed = -1;
	CMKMythologiesDecode compression;
	compression.dec(&ctl[ctlOffset], fileSizeCompressed, uncompressedSize, outputDecompressed);
	ALBank* alBank = ReadAudio(&outputDecompressed[0], ctlSize, 0, tbl, 0x0, 0x0, 0);
	alBank->soundBankFormat = MKMYTHOLOGIES;
	delete [] outputDecompressed;
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioArmyMenCompressed(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, bool littleEndianCompressedHeader, bool sarge2style)
{
	unsigned char* outputDecompressed = new unsigned char[0x1000000];
	int fileSizeCompressed = -1;
	n643docompression compression;
	compression.dec(&ctl[ctlOffset], compression.uncompressedSize(&ctl[ctlOffset], littleEndianCompressedHeader), outputDecompressed, fileSizeCompressed, littleEndianCompressedHeader, sarge2style);
	ALBank* alBank = ReadAudioN64PtrWavetableV2(&outputDecompressed[0], ctlSize, 0, tbl);
	alBank->soundBankFormat = ARMYMENFORMAT;
	delete [] outputDecompressed;
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioN64PtrWavetableV2(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl)
{
	unsigned short numberBands = 1;
	unsigned long bankOffset = ctlOffset + CharArrayToLong(&ctl[ctlOffset+0x2C]);

	// header size 0x30
	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = N64PTRWAVETABLETABLEV2;
	alBank->count = (unsigned short)CharArrayToLong(&ctl[ctlOffset+0x20]);
	unsigned long endOffsets = ctlOffset + CharArrayToLong(&ctl[ctlOffset+0x24]);
	unsigned long endExtraBytes = ctlOffset + CharArrayToLong(&ctl[ctlOffset + 0x28]);
	ctlSize = CharArrayToLong(&ctl[ctlOffset + 0x28]);
	alBank->flags = 0x0000;
	alBank->pad = 0x0;
	alBank->samplerate = 22050;
	alBank->percussion = 0x000;

	alBank->bankName.Format("%s", &ctl[ctlOffset + 0x14]);
	
	alBank->inst = NULL;

	if ((alBank->flags == 0x0000) || (alBank->flags == 0x0100)) // offset
	{
		alBank->inst = new ALInst*[alBank->count];

		for (int x = 0; x < alBank->count; x++)
		{
			alBank->inst[x] = new ALInst();
			alBank->inst[x]->samplerate = 0;
			alBank->inst[x]->sounds = NULL;
			alBank->inst[x]->alPercussion = NULL;
		}

		for (int x = 0; x < alBank->count; x++)
		{
			unsigned long offsetInstrument = CharArrayToLong(&ctl[bankOffset + x*4]);
			if (offsetInstrument == 0x0000)
			{
				alBank->inst[x]->soundCount = 0;
				alBank->inst[x]->sounds = NULL;
				continue;
			}

			if (alBank->flags == 0x0100)
				offsetInstrument = offsetInstrument & 0xFFFFFF;
			else
				offsetInstrument = ctlOffset + offsetInstrument;

			if ((alBank->inst[x]->flags == 0x0) || (alBank->inst[x]->flags == 0x1))
			{	
				alBank->inst[x]->soundCount = 1;

				alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

				for (int y = 0; y < alBank->inst[x]->soundCount; y++)
				{
					alBank->inst[x]->sounds[y] = new ALSound();
					alBank->inst[x]->sounds[y]->wav.wavData = NULL;
				}

				for (int y = 0; y < alBank->inst[x]->soundCount; y++)
				{
					alBank->inst[x]->sounds[y]->flags = 0x0000;

					if ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1))
					{
						alBank->inst[x]->sounds[y]->wav.base = CharArrayToLong(&ctl[offsetInstrument + 0x0]);
						alBank->inst[x]->sounds[y]->wav.len = CharArrayToLong(&ctl[offsetInstrument + 0x4]);
						alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];

						for (int  r = 0; r < alBank->inst[x]->sounds[y]->wav.len; r++)
						{
							alBank->inst[x]->sounds[y]->wav.wavData[r] = tbl[alBank->inst[x]->sounds[y]->wav.base + r];
						}
						

						alBank->inst[x]->sounds[y]->wav.type = AL_ADPCM_WAVE;
						alBank->inst[x]->sounds[y]->wav.flags = 0x0000;
						// MUST PAD to 4s

						if ((alBank->inst[x]->sounds[y]->wav.type == AL_ADPCM_WAVE) && ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1)))
						{
							alBank->inst[x]->sounds[y]->wav.adpcmWave = new ALADPCMWaveInfo();
							
							unsigned long predictorOffset = CharArrayToLong(&ctl[offsetInstrument + 0x10]);
							unsigned long loopOffset = CharArrayToLong(&ctl[offsetInstrument + 0xC]);

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

						
							if (predictorOffset != 0x00000000)
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
							else
							{
								MessageBox(NULL, "Error", "Missing predictors in ALSound wav", NULL);
								alBank->inst[x]->sounds[y]->wav.adpcmWave->book = NULL;
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

		for (int x = 0; x < alBank->count; x++)
		{
			alBank->inst[x]->volume = ctl[endOffsets + x];
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

ALBank* CN64AIFCAudio::ReadAudioFZero(unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl, int instrumentCount, unsigned char* rom)
{
	ALBank* alBank = new ALBank();

	unsigned short numberBands = 1;
	unsigned long bankOffset = 0;

	alBank->count = instrumentCount;
	alBank->soundBankFormat = FZEROFORMAT;
	alBank->flags = 0x0000;
	alBank->pad = 0x0000;
	alBank->samplerate = 11025;
	alBank->percussion = 0x0000;

	alBank->inst = NULL;

	if ((alBank->flags == 0x0000) || (alBank->flags == 0x0100)) // offset
	{
		alBank->inst = new ALInst*[alBank->count];

		for (int x = 0; x < alBank->count; x++)
		{
			alBank->inst[x] = new ALInst();
			alBank->inst[x]->samplerate = 0;
			alBank->inst[x]->sounds = NULL;
			alBank->inst[x]->alPercussion = NULL;
		}

		for (int x = 0; x < alBank->count; x++)
		{
			unsigned long offsetInstrument = CharArrayToLong(&ctl[ctlOffset + x*4 + 0x4]);

			if (offsetInstrument == 0x0000)
			{
				alBank->inst[x]->soundCount = 0;
				alBank->inst[x]->sounds = NULL;
				continue;
			}

			if (alBank->flags == 0x0100)
				offsetInstrument = offsetInstrument & 0xFFFFFF;
			else
				offsetInstrument = ctlOffset + offsetInstrument;
		
			unsigned long offsetSound1 = CharArrayToLong(&ctl[offsetInstrument + 0x10]);
			unsigned long offsetSound2 = CharArrayToLong(&ctl[offsetInstrument + 0x10 + 0x8]);

			alBank->inst[x]->soundCount = 1;
			if (offsetSound2 != 0x00000000)
				alBank->inst[x]->soundCount++;

			alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

			for (int y = 0; y < alBank->inst[x]->soundCount; y++)
			{
				alBank->inst[x]->sounds[y] = new ALSound();
				alBank->inst[x]->sounds[y]->wav.wavData = NULL;
			}

			for (int y = 0; y < alBank->inst[x]->soundCount; y++)
			{
				unsigned long offsetSound;
				if (y == 0)
					offsetSound = ctlOffset + offsetSound1;
				else
					offsetSound = ctlOffset + offsetSound2;

				alBank->inst[x]->sounds[y]->env.attackVolume = CharArrayToShort(&ctl[offsetInstrument + 0x2]);
				alBank->inst[x]->sounds[y]->env.decayVolume = CharArrayToShort(&ctl[offsetInstrument + 0x0]);
				//alBank->inst[x]->samplerate = CharArrayToLong(&ctl[offsetInstrument + 0x4]);

				alBank->inst[x]->sounds[y]->wav.base = CharArrayToLong(&ctl[offsetSound + 0x4]);
				alBank->inst[x]->sounds[y]->wav.len = CharArrayToLong(&ctl[offsetSound + 0x0]) & 0xFFFFFF;
				alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];

				for (int  r = 0; r < alBank->inst[x]->sounds[y]->wav.len; r++)
				{
					alBank->inst[x]->sounds[y]->wav.wavData[r] = tbl[alBank->inst[x]->sounds[y]->wav.base + r];
				}
				
				alBank->inst[x]->sounds[y]->wav.type == AL_ADPCM_WAVE;
				alBank->inst[x]->sounds[y]->wav.flags = 0x00;

				alBank->inst[x]->sounds[y]->wav.adpcmWave = new ALADPCMWaveInfo();

				unsigned long predictorOffset = CharArrayToLong(&ctl[offsetSound + 0xC]);
				if (predictorOffset != 0)
				{
					if (alBank->inst[x]->sounds[y]->flags == 0x1)
						predictorOffset = predictorOffset & 0xFFFFFF;
					else
						predictorOffset = ctlOffset + predictorOffset;

					alBank->inst[x]->sounds[y]->wav.adpcmWave->loop = NULL;

					alBank->inst[x]->sounds[y]->wav.adpcmWave->book = new ALADPCMBook();
					alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order = CharArrayToLong(&ctl[predictorOffset + 0x0]);
					alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors = CharArrayToLong(&ctl[predictorOffset + 0x4]);
					alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors = new signed short[alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8];

					for (int z = 0; z < alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8; z++)
					{
						alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[z] = (signed short)CharArrayToShort(&ctl[predictorOffset + 0x8 + z * 2]);
					}
				}

				unsigned long loopOffset = CharArrayToLong(&ctl[offsetSound + 0x8]);
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
			}
		}
	}

	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioZelda(unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl, int instrumentCount, unsigned long endSpot, unsigned char* rom)
{
	unsigned long offsetInstrumentSection1 = CharArrayToLong(&ctl[ctlOffset]);
	unsigned long offsetInstrumentSection2 = CharArrayToLong(&ctl[ctlOffset + 4]);
	unsigned long endOffsetInstrument1Section1;
	unsigned long endOffsetInstrument1Section2;

	int instrumentCountNormal = instrumentCount;
	int instrumentCountSection1 = 0;
	int instrumentCountSection2 = 0;

	if (offsetInstrumentSection1 != NULL)
	{
		if (offsetInstrumentSection2 != NULL)
		{
			endOffsetInstrument1Section1 = offsetInstrumentSection2;
			endOffsetInstrument1Section2 = endSpot - ctlOffset;

			instrumentCountSection1 = ((endOffsetInstrument1Section1 - offsetInstrumentSection1) / 4);
			instrumentCountSection2 = ((endOffsetInstrument1Section2 - offsetInstrumentSection2) / 8);
		}
		else
		{
			endOffsetInstrument1Section1 = endSpot - ctlOffset;

			instrumentCountSection1 = ((endOffsetInstrument1Section1 - offsetInstrumentSection1) / 4);
		}
	}
	else if (offsetInstrumentSection2 != NULL)
	{
		endOffsetInstrument1Section2 = endSpot - ctlOffset;
		instrumentCountSection2 = ((endOffsetInstrument1Section2 - offsetInstrumentSection2) / 8);
	}

	ALBank* alBank = new ALBank();

	unsigned short numberBands = 1;
	unsigned long bankOffset = 0;

	alBank->count = instrumentCount + instrumentCountSection1 + instrumentCountSection2;
	alBank->soundBankFormat = ZELDAFORMAT;
	alBank->flags = 0x0000;
	alBank->pad = 0x0000;
	alBank->samplerate = 22050;
	alBank->percussion = 0x0000;

	alBank->inst = NULL;

	if ((alBank->flags == 0x0000) || (alBank->flags == 0x0100)) // offset
	{
		alBank->inst = new ALInst*[alBank->count];

		for (int x = 0; x < alBank->count; x++)
		{
			alBank->inst[x] = new ALInst();
			alBank->inst[x]->samplerate = 0;
			alBank->inst[x]->sounds = NULL;
		}

		for (int x = 0; x < alBank->count; x++)
		{
			unsigned long offsetInstrument;
					
			if (x < instrumentCountNormal)
			{
				offsetInstrument = CharArrayToLong(&ctl[ctlOffset + x*4 + 0x8]);
				if (offsetInstrument == 0x0000)
				{
					alBank->inst[x]->soundCount = 0;
					alBank->inst[x]->sounds = NULL;
					alBank->inst[x]->alPercussion = NULL;
					continue;
				}
				offsetInstrument = ctlOffset + offsetInstrument;
				
				alBank->inst[x]->soundCount = 1;

				alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

				for (int y = 0; y < alBank->inst[x]->soundCount; y++)
				{
					alBank->inst[x]->sounds[y] = new ALSound();
					alBank->inst[x]->sounds[y]->wav.wavData = NULL;
				}

				for (int y = 0; y < alBank->inst[x]->soundCount; y++)
				{
					unsigned long offsetSoundData = offsetInstrument;

					alBank->inst[x]->sounds[y]->env.attackVolume = CharArrayToShort(&ctl[offsetInstrument + 0x2]);
					alBank->inst[x]->sounds[y]->env.decayVolume = CharArrayToShort(&ctl[offsetInstrument + 0x0]);
					alBank->inst[x]->sounds[y]->unknown3 = CharArrayToLong(&ctl[offsetInstrument + 0x14]);
					//alBank->inst[x]->samplerate = CharArrayToLong(&ctl[offsetInstrument + 0x4]);










					unsigned long offsetWaveTable = CharArrayToLong(&ctl[offsetSoundData+ 0x10]);
					offsetWaveTable = ctlOffset + offsetWaveTable;

					alBank->inst[x]->sounds[y]->wav.base = alBank->inst[x]->tblBase + CharArrayToLong(&ctl[offsetWaveTable + 0x4]);
					alBank->inst[x]->sounds[y]->wav.len = CharArrayToLong(&ctl[offsetWaveTable + 0x0]) & 0xFFFFFF;

					if (ctl[offsetWaveTable] == 0x30)
					{
						//30 special somehow
					}
					alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];

					for (int  r = 0; r < alBank->inst[x]->sounds[y]->wav.len; r++)
					{
						alBank->inst[x]->sounds[y]->wav.wavData[r] = tbl[alBank->inst[x]->sounds[y]->wav.base + r];
					}
					
					alBank->inst[x]->sounds[y]->wav.type = AL_ADPCM_WAVE;
					alBank->inst[x]->sounds[y]->wav.flags = 0;
					// MUST PAD to 4s

					if ((alBank->inst[x]->sounds[y]->wav.type == AL_ADPCM_WAVE) && ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1)))
					{
						alBank->inst[x]->sounds[y]->wav.adpcmWave = new ALADPCMWaveInfo();
						unsigned long loopOffset = CharArrayToLong(&ctl[offsetWaveTable + 0x8]);
						unsigned long predictorOffset = CharArrayToLong(&ctl[offsetWaveTable + 0xC]);

						if (loopOffset != 0)
						{
							loopOffset = ctlOffset + loopOffset;

							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop = new ALADPCMloop();
							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->start = CharArrayToLong(&ctl[loopOffset + 0x0]);
							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->end = CharArrayToLong(&ctl[loopOffset + 0x4]);
							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->count = CharArrayToLong(&ctl[loopOffset + 0x8]);
							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->unknown1 = CharArrayToLong(&ctl[loopOffset + 0xC]);

							if (alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->start != 0)
							{
								for (int z = 0; z < 0x10; z++)
								{
									alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->state[z] = CharArrayToShort(&ctl[loopOffset + 0x10 + z * 2]);
								}
							}
						}
						else
						{
							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop = NULL;
						}

						if (predictorOffset != 0)
						{
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


					unsigned long offsetWaveTableSecondary = CharArrayToLong(&ctl[offsetSoundData+ 0x18]);
					if (offsetWaveTableSecondary == 0x00000000)
						alBank->inst[x]->sounds[y]->hasWaveSecondary = false;
					else
					{
						alBank->inst[x]->sounds[y]->hasWaveSecondary = true;
						offsetWaveTableSecondary = ctlOffset + offsetWaveTableSecondary;

						alBank->inst[x]->sounds[y]->wavSecondary.base = alBank->inst[x]->tblBase + CharArrayToLong(&ctl[offsetWaveTableSecondary + 0x4]);
						alBank->inst[x]->sounds[y]->wavSecondary.len = CharArrayToLong(&ctl[offsetWaveTableSecondary + 0x0]) & 0xFFFFFF;

						alBank->inst[x]->sounds[y]->wavSecondary.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wavSecondary.len];

						if (ctl[offsetWaveTableSecondary] == 0x30)
						{
							//30 special somehow
						}

						for (int  r = 0; r < alBank->inst[x]->sounds[y]->wavSecondary.len; r++)
						{
							alBank->inst[x]->sounds[y]->wavSecondary.wavData[r] = tbl[alBank->inst[x]->sounds[y]->wavSecondary.base + r];
						}
						
						alBank->inst[x]->sounds[y]->wavSecondary.type = AL_ADPCM_WAVE;
						alBank->inst[x]->sounds[y]->wavSecondary.flags = 0;
						// MUST PAD to 4s

						if ((alBank->inst[x]->sounds[y]->wavSecondary.type == AL_ADPCM_WAVE) && ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1)))
						{
							alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave = new ALADPCMWaveInfo();
							unsigned long loopOffsetSecondary = CharArrayToLong(&ctl[offsetWaveTableSecondary + 0x8]);
							unsigned long predictorOffsetSecondary = CharArrayToLong(&ctl[offsetWaveTableSecondary + 0xC]);

							if (loopOffsetSecondary != 0)
							{
								loopOffsetSecondary = ctlOffset + loopOffsetSecondary;

								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop = new ALADPCMloop();
								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->start = CharArrayToLong(&ctl[loopOffsetSecondary + 0x0]);
								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->end = CharArrayToLong(&ctl[loopOffsetSecondary + 0x4]);
								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->count = CharArrayToLong(&ctl[loopOffsetSecondary + 0x8]);
								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->unknown1 = CharArrayToLong(&ctl[loopOffsetSecondary + 0xC]);

								if (alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->start != 0)
								{
									for (int z = 0; z < 0x10; z++)
									{
										alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->state[z] = CharArrayToShort(&ctl[loopOffsetSecondary + 0x10 + z * 2]);
									}
								}
							}
							else
							{
								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop = NULL;
							}

							if (predictorOffsetSecondary != 0)
							{
								predictorOffsetSecondary = ctlOffset + predictorOffsetSecondary;

								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book = new ALADPCMBook();
								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->order = CharArrayToLong(&ctl[predictorOffsetSecondary + 0x0]);
								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->npredictors = CharArrayToLong(&ctl[predictorOffsetSecondary + 0x4]);
								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->predictors = new signed short[alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->npredictors * 8];

								for (int z = 0; z < alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->npredictors * 8; z++)
								{
									alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->predictors[z] = (signed short)CharArrayToShort(&ctl[predictorOffsetSecondary + 0x8 + z * 2]);
								}
							}
						}
					}

					unsigned long offsetWaveTablePrevious = CharArrayToLong(&ctl[offsetSoundData+ 0x8]);
					if (offsetWaveTablePrevious == 0x00000000)
						alBank->inst[x]->sounds[y]->hasWavePrevious = false;
					else
					{
						alBank->inst[x]->sounds[y]->hasWavePrevious = true;
						offsetWaveTablePrevious = ctlOffset + offsetWaveTablePrevious;

						alBank->inst[x]->sounds[y]->wavPrevious.base = alBank->inst[x]->tblBase + CharArrayToLong(&ctl[offsetWaveTablePrevious + 0x4]);
						alBank->inst[x]->sounds[y]->wavPrevious.len = CharArrayToLong(&ctl[offsetWaveTablePrevious + 0x0]) & 0xFFFFFF;

						if (ctl[offsetWaveTablePrevious] == 0x30)
						{
							//30 special somehow
						}
						alBank->inst[x]->sounds[y]->wavPrevious.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wavPrevious.len];

						for (int  r = 0; r < alBank->inst[x]->sounds[y]->wavPrevious.len; r++)
						{
							alBank->inst[x]->sounds[y]->wavPrevious.wavData[r] = tbl[alBank->inst[x]->sounds[y]->wavPrevious.base + r];
						}
						
						alBank->inst[x]->sounds[y]->wavPrevious.type = AL_ADPCM_WAVE;
						alBank->inst[x]->sounds[y]->wavPrevious.flags = 0;
						// MUST PAD to 4s

						if ((alBank->inst[x]->sounds[y]->wavPrevious.type == AL_ADPCM_WAVE) && ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1)))
						{
							alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave = new ALADPCMWaveInfo();
							unsigned long loopOffsetPrevious = CharArrayToLong(&ctl[offsetWaveTablePrevious + 0x8]);
							unsigned long predictorOffsetPrevious = CharArrayToLong(&ctl[offsetWaveTablePrevious + 0xC]);

							if (loopOffsetPrevious != 0)
							{
								loopOffsetPrevious = ctlOffset + loopOffsetPrevious;

								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop = new ALADPCMloop();
								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->start = CharArrayToLong(&ctl[loopOffsetPrevious + 0x0]);
								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->end = CharArrayToLong(&ctl[loopOffsetPrevious + 0x4]);
								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->count = CharArrayToLong(&ctl[loopOffsetPrevious + 0x8]);
								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->unknown1 = CharArrayToLong(&ctl[loopOffsetPrevious + 0xC]);

								if (alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->start != 0)
								{
									for (int z = 0; z < 0x10; z++)
									{
										alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->state[z] = CharArrayToShort(&ctl[loopOffsetPrevious + 0x10 + z * 2]);
									}
								}
							}
							else
							{
								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop = NULL;
							}

							if (predictorOffsetPrevious != 0)
							{
								predictorOffsetPrevious = ctlOffset + predictorOffsetPrevious;

								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book = new ALADPCMBook();
								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->order = CharArrayToLong(&ctl[predictorOffsetPrevious + 0x0]);
								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->npredictors = CharArrayToLong(&ctl[predictorOffsetPrevious + 0x4]);
								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->predictors = new signed short[alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->npredictors * 8];

								for (int z = 0; z < alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->npredictors * 8; z++)
								{
									alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->predictors[z] = (signed short)CharArrayToShort(&ctl[predictorOffsetPrevious + 0x8 + z * 2]);
								}
							}
						}
					}
				}
			}
			else if (x < (instrumentCountNormal + instrumentCountSection1))
			{
				offsetInstrument = CharArrayToLong(&ctl[ctlOffset + offsetInstrumentSection1 + (x-instrumentCountNormal)*4]);
				if (offsetInstrument == 0x0000)
				{
					alBank->inst[x]->soundCount = 0;
					alBank->inst[x]->sounds = NULL;
					continue;
				}
				offsetInstrument = ctlOffset + offsetInstrument;

				unsigned long offsetSound = ctlOffset + CharArrayToLong(&ctl[offsetInstrument + 4]);;

				alBank->inst[x]->soundCount = 1;

				alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

				for (int y = 0; y < alBank->inst[x]->soundCount; y++)
				{
					alBank->inst[x]->sounds[y] = new ALSound();
					alBank->inst[x]->sounds[y]->wav.wavData = NULL;
				}

				for (int y = 0; y < alBank->inst[x]->soundCount; y++)
				{

					alBank->inst[x]->sounds[y]->env.attackVolume = CharArrayToShort(&ctl[offsetInstrument + 0x2]);
					alBank->inst[x]->sounds[y]->env.decayVolume = CharArrayToShort(&ctl[offsetInstrument + 0x0]);
					alBank->inst[x]->sounds[y]->unknown3 = CharArrayToLong(&ctl[offsetInstrument + 0x14]);
					//alBank->inst[x]->samplerate = CharArrayToLong(&ctl[offsetInstrument + 0x4]);


					alBank->inst[x]->sounds[y]->wav.base = CharArrayToLong(&ctl[offsetSound + 0x4]);
					alBank->inst[x]->sounds[y]->wav.len = CharArrayToLong(&ctl[offsetSound + 0x0]) & 0xFFFFFF;
					alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];

					if (ctl[offsetSound] == 0x03)
					{
						// special but not sure what
					}
					for (int  r = 0; r < alBank->inst[x]->sounds[y]->wav.len; r++)
					{
						alBank->inst[x]->sounds[y]->wav.wavData[r] = tbl[alBank->inst[x]->sounds[y]->wav.base + r];
					}
					
					alBank->inst[x]->sounds[y]->wav.type == AL_ADPCM_WAVE;
					alBank->inst[x]->sounds[y]->wav.flags = 0x00;

					alBank->inst[x]->sounds[y]->wav.adpcmWave = new ALADPCMWaveInfo();

					unsigned long predictorOffset = CharArrayToLong(&ctl[offsetSound + 0xC]);
					if (predictorOffset != 0)
					{
						if (alBank->inst[x]->sounds[y]->flags == 0x1)
							predictorOffset = predictorOffset & 0xFFFFFF;
						else
							predictorOffset = ctlOffset + predictorOffset;

						alBank->inst[x]->sounds[y]->wav.adpcmWave->loop = NULL;

						alBank->inst[x]->sounds[y]->wav.adpcmWave->book = new ALADPCMBook();
						alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order = CharArrayToLong(&ctl[predictorOffset + 0x0]);
						alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors = CharArrayToLong(&ctl[predictorOffset + 0x4]);
						alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors = new signed short[alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8];

						for (int z = 0; z < alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8; z++)
						{
							alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[z] = (signed short)CharArrayToShort(&ctl[predictorOffset + 0x8 + z * 2]);
						}
					}

					unsigned long loopOffset = CharArrayToLong(&ctl[offsetSound + 0x8]);
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
				}
				continue;
			}
			else if (x < (instrumentCountNormal + instrumentCountSection1 + instrumentCountSection2))
			{
				offsetInstrument = CharArrayToLong(&ctl[ctlOffset + offsetInstrumentSection2 + (x-instrumentCountNormal-instrumentCountSection1)*8]);
				if (offsetInstrument == 0x0000)
				{
					alBank->inst[x]->soundCount = 0;
					alBank->inst[x]->sounds = NULL;
					continue;
				}

				offsetInstrument = ctlOffset + offsetInstrument;

				unsigned long offsetSound = offsetInstrument;



				alBank->inst[x]->soundCount = 1;

				alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

				for (int y = 0; y < alBank->inst[x]->soundCount; y++)
				{
					alBank->inst[x]->sounds[y] = new ALSound();
					alBank->inst[x]->sounds[y]->wav.wavData = NULL;
				}

				for (int y = 0; y < alBank->inst[x]->soundCount; y++)
				{
					alBank->inst[x]->sounds[y]->env.attackVolume = CharArrayToShort(&ctl[offsetInstrument + 0x2]);
					alBank->inst[x]->sounds[y]->env.decayVolume = CharArrayToShort(&ctl[offsetInstrument + 0x0]);
					alBank->inst[x]->sounds[y]->unknown3 = CharArrayToLong(&ctl[offsetInstrument + 0x14]);
					//alBank->inst[x]->samplerate = CharArrayToLong(&ctl[offsetInstrument + 0x4]);


					alBank->inst[x]->sounds[y]->wav.base = CharArrayToLong(&ctl[offsetSound + 0x4]);
					alBank->inst[x]->sounds[y]->wav.len = CharArrayToLong(&ctl[offsetSound + 0x0]) & 0xFFFFFF;
					alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];

					if (ctl[offsetSound] == 0x03)
					{
						// special but not sure what
					}
					for (int  r = 0; r < alBank->inst[x]->sounds[y]->wav.len; r++)
					{
						alBank->inst[x]->sounds[y]->wav.wavData[r] = tbl[alBank->inst[x]->sounds[y]->wav.base + r];
					}
					
					alBank->inst[x]->sounds[y]->wav.type == AL_ADPCM_WAVE;
					alBank->inst[x]->sounds[y]->wav.flags = 0x00;

					alBank->inst[x]->sounds[y]->wav.adpcmWave = new ALADPCMWaveInfo();

					unsigned long predictorOffset = CharArrayToLong(&ctl[offsetSound + 0xC]);
					if (predictorOffset != 0)
					{
						if (alBank->inst[x]->sounds[y]->flags == 0x1)
							predictorOffset = predictorOffset & 0xFFFFFF;
						else
							predictorOffset = ctlOffset + predictorOffset;

						alBank->inst[x]->sounds[y]->wav.adpcmWave->loop = NULL;

						alBank->inst[x]->sounds[y]->wav.adpcmWave->book = new ALADPCMBook();
						alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order = CharArrayToLong(&ctl[predictorOffset + 0x0]);
						alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors = CharArrayToLong(&ctl[predictorOffset + 0x4]);
						alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors = new signed short[alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8];

						for (int z = 0; z < alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8; z++)
						{
							alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[z] = (signed short)CharArrayToShort(&ctl[predictorOffset + 0x8 + z * 2]);
						}
					}

					unsigned long loopOffset = CharArrayToLong(&ctl[offsetSound + 0x8]);
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
				}
				continue;
			}
		}
	}

	return alBank;
}


ALBank* CN64AIFCAudio::ReadAudioStarFox(unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl, int instrumentCount, unsigned long endSpot)
{
	unsigned short numberBands = 1;
	unsigned long bankOffset = 0;

	int instrumentCountNormal = instrumentCount;
	int instrumentCountSection1 = 0;

	unsigned long offsetInstrumentSection1 = CharArrayToLong(&ctl[ctlOffset]);
	unsigned long endOffsetInstrument1Section1 = 0;


	if (offsetInstrumentSection1 != NULL)
	{
		endOffsetInstrument1Section1 = endSpot - ctlOffset;
		instrumentCountSection1 = ((endOffsetInstrument1Section1 - offsetInstrumentSection1) / 4);
	}
	

	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = STARFOX64FORMAT;
	alBank->count = instrumentCount + instrumentCountSection1;
	alBank->flags = 0x0000;
	alBank->pad = 0x0000;
	alBank->samplerate = 8000;
	alBank->percussion = 0x0000;

	alBank->inst = NULL;

	if ((alBank->flags == 0x0000) || (alBank->flags == 0x0100)) // offset
	{
		alBank->inst = new ALInst*[alBank->count];

		for (int x = 0; x < alBank->count; x++)
		{
			alBank->inst[x] = new ALInst();
			alBank->inst[x]->samplerate = 0;
			alBank->inst[x]->sounds = NULL;
			alBank->inst[x]->alPercussion = NULL;
		}

		for (int x = 0; x < alBank->count; x++)
		{
			unsigned long offsetInstrument;
					
			if (x < instrumentCountNormal)
			{
				offsetInstrument = CharArrayToLong(&ctl[ctlOffset + x*4 + 0x4]);
				if (offsetInstrument == 0x0000)
				{
					alBank->inst[x]->soundCount = 0;
					alBank->inst[x]->sounds = NULL;
					alBank->inst[x]->alPercussion = NULL;
					continue;
				}
				offsetInstrument = ctlOffset + offsetInstrument;
				
				alBank->inst[x]->soundCount = 1;

				alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

				for (int y = 0; y < alBank->inst[x]->soundCount; y++)
				{
					alBank->inst[x]->sounds[y] = new ALSound();
					alBank->inst[x]->sounds[y]->wav.wavData = NULL;
				}

				for (int y = 0; y < alBank->inst[x]->soundCount; y++)
				{
					unsigned long offsetSoundData = offsetInstrument;

					alBank->inst[x]->sounds[y]->env.attackVolume = CharArrayToShort(&ctl[offsetInstrument + 0x2]);
					alBank->inst[x]->sounds[y]->env.decayVolume = CharArrayToShort(&ctl[offsetInstrument + 0x0]);
					alBank->inst[x]->sounds[y]->unknown3 = CharArrayToLong(&ctl[offsetInstrument + 0x14]);
					//alBank->inst[x]->samplerate = CharArrayToLong(&ctl[offsetInstrument + 0x4]);










					unsigned long offsetWaveTable = CharArrayToLong(&ctl[offsetSoundData+ 0x10]);
					offsetWaveTable = ctlOffset + offsetWaveTable;

					alBank->inst[x]->sounds[y]->wav.base = alBank->inst[x]->tblBase + CharArrayToLong(&ctl[offsetWaveTable + 0x4]);
					alBank->inst[x]->sounds[y]->wav.len = CharArrayToLong(&ctl[offsetWaveTable + 0x0]) & 0xFFFFFF;

					if (ctl[offsetWaveTable] == 0x30)
					{
						//30 special somehow
					}
					alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];

					for (int  r = 0; r < alBank->inst[x]->sounds[y]->wav.len; r++)
					{
						alBank->inst[x]->sounds[y]->wav.wavData[r] = tbl[alBank->inst[x]->sounds[y]->wav.base + r];
					}
					
					alBank->inst[x]->sounds[y]->wav.type = AL_ADPCM_WAVE;
					alBank->inst[x]->sounds[y]->wav.flags = 0;
					// MUST PAD to 4s

					if ((alBank->inst[x]->sounds[y]->wav.type == AL_ADPCM_WAVE) && ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1)))
					{
						alBank->inst[x]->sounds[y]->wav.adpcmWave = new ALADPCMWaveInfo();
						unsigned long loopOffset = CharArrayToLong(&ctl[offsetWaveTable + 0x8]);
						unsigned long predictorOffset = CharArrayToLong(&ctl[offsetWaveTable + 0xC]);

						if (loopOffset != 0)
						{
							loopOffset = ctlOffset + loopOffset;

							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop = new ALADPCMloop();
							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->start = CharArrayToLong(&ctl[loopOffset + 0x0]);
							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->end = CharArrayToLong(&ctl[loopOffset + 0x4]);
							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->count = CharArrayToLong(&ctl[loopOffset + 0x8]);
							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->unknown1 = CharArrayToLong(&ctl[loopOffset + 0xC]);

							if (alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->start != 0)
							{
								for (int z = 0; z < 0x10; z++)
								{
									alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->state[z] = CharArrayToShort(&ctl[loopOffset + 0x10 + z * 2]);
								}
							}
						}
						else
						{
							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop = NULL;
						}

						if (predictorOffset != 0)
						{
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


					unsigned long offsetWaveTableSecondary = CharArrayToLong(&ctl[offsetSoundData+ 0x18]);
					if (offsetWaveTableSecondary == 0x00000000)
						alBank->inst[x]->sounds[y]->hasWaveSecondary = false;
					else
					{
						alBank->inst[x]->sounds[y]->hasWaveSecondary = true;
						offsetWaveTableSecondary = ctlOffset + offsetWaveTableSecondary;

						alBank->inst[x]->sounds[y]->wavSecondary.base = alBank->inst[x]->tblBase + CharArrayToLong(&ctl[offsetWaveTableSecondary + 0x4]);
						alBank->inst[x]->sounds[y]->wavSecondary.len = CharArrayToLong(&ctl[offsetWaveTableSecondary + 0x0]) & 0xFFFFFF;

						alBank->inst[x]->sounds[y]->wavSecondary.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wavSecondary.len];

						if (ctl[offsetWaveTableSecondary] == 0x30)
						{
							//30 special somehow
						}

						for (int  r = 0; r < alBank->inst[x]->sounds[y]->wavSecondary.len; r++)
						{
							alBank->inst[x]->sounds[y]->wavSecondary.wavData[r] = tbl[alBank->inst[x]->sounds[y]->wavSecondary.base + r];
						}
						
						alBank->inst[x]->sounds[y]->wavSecondary.type = AL_ADPCM_WAVE;
						alBank->inst[x]->sounds[y]->wavSecondary.flags = 0;
						// MUST PAD to 4s

						if ((alBank->inst[x]->sounds[y]->wavSecondary.type == AL_ADPCM_WAVE) && ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1)))
						{
							alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave = new ALADPCMWaveInfo();
							unsigned long loopOffsetSecondary = CharArrayToLong(&ctl[offsetWaveTableSecondary + 0x8]);
							unsigned long predictorOffsetSecondary = CharArrayToLong(&ctl[offsetWaveTableSecondary + 0xC]);

							if (loopOffsetSecondary != 0)
							{
								loopOffsetSecondary = ctlOffset + loopOffsetSecondary;

								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop = new ALADPCMloop();
								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->start = CharArrayToLong(&ctl[loopOffsetSecondary + 0x0]);
								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->end = CharArrayToLong(&ctl[loopOffsetSecondary + 0x4]);
								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->count = CharArrayToLong(&ctl[loopOffsetSecondary + 0x8]);
								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->unknown1 = CharArrayToLong(&ctl[loopOffsetSecondary + 0xC]);

								if (alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->start != 0)
								{
									for (int z = 0; z < 0x10; z++)
									{
										alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->state[z] = CharArrayToShort(&ctl[loopOffsetSecondary + 0x10 + z * 2]);
									}
								}
							}
							else
							{
								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop = NULL;
							}

							if (predictorOffsetSecondary != 0)
							{
								predictorOffsetSecondary = ctlOffset + predictorOffsetSecondary;

								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book = new ALADPCMBook();
								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->order = CharArrayToLong(&ctl[predictorOffsetSecondary + 0x0]);
								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->npredictors = CharArrayToLong(&ctl[predictorOffsetSecondary + 0x4]);
								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->predictors = new signed short[alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->npredictors * 8];

								for (int z = 0; z < alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->npredictors * 8; z++)
								{
									alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->predictors[z] = (signed short)CharArrayToShort(&ctl[predictorOffsetSecondary + 0x8 + z * 2]);
								}
							}
						}
					}

					unsigned long offsetWaveTablePrevious = CharArrayToLong(&ctl[offsetSoundData+ 0x8]);
					if (offsetWaveTablePrevious == 0x00000000)
						alBank->inst[x]->sounds[y]->hasWavePrevious = false;
					else
					{
						alBank->inst[x]->sounds[y]->hasWavePrevious = true;
						offsetWaveTablePrevious = ctlOffset + offsetWaveTablePrevious;

						alBank->inst[x]->sounds[y]->wavPrevious.base = alBank->inst[x]->tblBase + CharArrayToLong(&ctl[offsetWaveTablePrevious + 0x4]);
						alBank->inst[x]->sounds[y]->wavPrevious.len = CharArrayToLong(&ctl[offsetWaveTablePrevious + 0x0]) & 0xFFFFFF;

						if (ctl[offsetWaveTablePrevious] == 0x30)
						{
							//30 special somehow
						}
						alBank->inst[x]->sounds[y]->wavPrevious.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wavPrevious.len];

						for (int  r = 0; r < alBank->inst[x]->sounds[y]->wavPrevious.len; r++)
						{
							alBank->inst[x]->sounds[y]->wavPrevious.wavData[r] = tbl[alBank->inst[x]->sounds[y]->wavPrevious.base + r];
						}
						
						alBank->inst[x]->sounds[y]->wavPrevious.type = AL_ADPCM_WAVE;
						alBank->inst[x]->sounds[y]->wavPrevious.flags = 0;
						// MUST PAD to 4s

						if ((alBank->inst[x]->sounds[y]->wavPrevious.type == AL_ADPCM_WAVE) && ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1)))
						{
							alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave = new ALADPCMWaveInfo();
							unsigned long loopOffsetPrevious = CharArrayToLong(&ctl[offsetWaveTablePrevious + 0x8]);
							unsigned long predictorOffsetPrevious = CharArrayToLong(&ctl[offsetWaveTablePrevious + 0xC]);

							if (loopOffsetPrevious != 0)
							{
								loopOffsetPrevious = ctlOffset + loopOffsetPrevious;

								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop = new ALADPCMloop();
								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->start = CharArrayToLong(&ctl[loopOffsetPrevious + 0x0]);
								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->end = CharArrayToLong(&ctl[loopOffsetPrevious + 0x4]);
								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->count = CharArrayToLong(&ctl[loopOffsetPrevious + 0x8]);
								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->unknown1 = CharArrayToLong(&ctl[loopOffsetPrevious + 0xC]);

								if (alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->start != 0)
								{
									for (int z = 0; z < 0x10; z++)
									{
										alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->state[z] = CharArrayToShort(&ctl[loopOffsetPrevious + 0x10 + z * 2]);
									}
								}
							}
							else
							{
								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop = NULL;
							}

							if (predictorOffsetPrevious != 0)
							{
								predictorOffsetPrevious = ctlOffset + predictorOffsetPrevious;

								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book = new ALADPCMBook();
								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->order = CharArrayToLong(&ctl[predictorOffsetPrevious + 0x0]);
								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->npredictors = CharArrayToLong(&ctl[predictorOffsetPrevious + 0x4]);
								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->predictors = new signed short[alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->npredictors * 8];

								for (int z = 0; z < alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->npredictors * 8; z++)
								{
									alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->predictors[z] = (signed short)CharArrayToShort(&ctl[predictorOffsetPrevious + 0x8 + z * 2]);
								}
							}
						}
					}
				}
			}
			else if (x < (instrumentCountNormal + instrumentCountSection1))
			{
				offsetInstrument = CharArrayToLong(&ctl[ctlOffset + offsetInstrumentSection1 + (x-instrumentCountNormal)*4]);
				if (offsetInstrument == 0x0000)
				{
					alBank->inst[x]->soundCount = 0;
					alBank->inst[x]->sounds = NULL;
					continue;
				}
				offsetInstrument = ctlOffset + offsetInstrument;

				unsigned long offsetSound = ctlOffset + CharArrayToLong(&ctl[offsetInstrument + 4]);;

				alBank->inst[x]->soundCount = 1;

				alBank->inst[x]->sounds = new ALSound*[alBank->inst[x]->soundCount];

				for (int y = 0; y < alBank->inst[x]->soundCount; y++)
				{
					alBank->inst[x]->sounds[y] = new ALSound();
					alBank->inst[x]->sounds[y]->wav.wavData = NULL;
				}

				for (int y = 0; y < alBank->inst[x]->soundCount; y++)
				{

					alBank->inst[x]->sounds[y]->env.attackVolume = CharArrayToShort(&ctl[offsetInstrument + 0x2]);
					alBank->inst[x]->sounds[y]->env.decayVolume = CharArrayToShort(&ctl[offsetInstrument + 0x0]);
					alBank->inst[x]->sounds[y]->unknown3 = CharArrayToLong(&ctl[offsetInstrument + 0x14]);
					//alBank->inst[x]->samplerate = CharArrayToLong(&ctl[offsetInstrument + 0x4]);


					alBank->inst[x]->sounds[y]->wav.base = CharArrayToLong(&ctl[offsetSound + 0x4]);
					alBank->inst[x]->sounds[y]->wav.len = CharArrayToLong(&ctl[offsetSound + 0x0]) & 0xFFFFFF;
					alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];

					if (ctl[offsetSound] == 0x03)
					{
						// special but not sure what
					}
					for (int  r = 0; r < alBank->inst[x]->sounds[y]->wav.len; r++)
					{
						alBank->inst[x]->sounds[y]->wav.wavData[r] = tbl[alBank->inst[x]->sounds[y]->wav.base + r];
					}
					
					alBank->inst[x]->sounds[y]->wav.type == AL_ADPCM_WAVE;
					alBank->inst[x]->sounds[y]->wav.flags = 0x00;

					alBank->inst[x]->sounds[y]->wav.adpcmWave = new ALADPCMWaveInfo();

					unsigned long predictorOffset = CharArrayToLong(&ctl[offsetSound + 0xC]);
					if (predictorOffset != 0)
					{
						if (alBank->inst[x]->sounds[y]->flags == 0x1)
							predictorOffset = predictorOffset & 0xFFFFFF;
						else
							predictorOffset = ctlOffset + predictorOffset;

						alBank->inst[x]->sounds[y]->wav.adpcmWave->loop = NULL;

						alBank->inst[x]->sounds[y]->wav.adpcmWave->book = new ALADPCMBook();
						alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order = CharArrayToLong(&ctl[predictorOffset + 0x0]);
						alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors = CharArrayToLong(&ctl[predictorOffset + 0x4]);
						alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors = new signed short[alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8];

						for (int z = 0; z < alBank->inst[x]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wav.adpcmWave->book->npredictors * 8; z++)
						{
							alBank->inst[x]->sounds[y]->wav.adpcmWave->book->predictors[z] = (signed short)CharArrayToShort(&ctl[predictorOffset + 0x8 + z * 2]);
						}
					}

					unsigned long loopOffset = CharArrayToLong(&ctl[offsetSound + 0x8]);
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
				}
				continue;
			}
		}
	}

	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioMario(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, unsigned long& tblSize)
{
	return ReadAudioMario(ctl, ctlSize, ctlOffset, tbl, tblSize, false);
}

ALBank* CN64AIFCAudio::ReadAudioMario(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, unsigned long& tblSize, bool marioKartStyle)
{
	ALBank* alBank = new ALBank();
	if (marioKartStyle)
		alBank->soundBankFormat = MARIOKART64FORMAT;
	else
		alBank->soundBankFormat = SUPERMARIO64FORMAT;
	alBank->count = CharArrayToShort(&ctl[ctlOffset+2]);
	alBank->flags = 0x0000;
	alBank->samplerate = 22050;

	alBank->inst = NULL;

	if ((alBank->flags == 0x0000) || (alBank->flags == 0x0100)) // offset
	{
		alBank->inst = new ALInst*[alBank->count];

		for (int x = 0; x < alBank->count; x++)
		{
			alBank->inst[x] = new ALInst();
			alBank->inst[x]->samplerate = 0;
			alBank->inst[x]->sounds = NULL;
			alBank->inst[x]->alPercussion = NULL;
		}

		for (int x = 0; x < alBank->count; x++)
		{
			unsigned long offsetInstrument = CharArrayToLong(&ctl[ctlOffset + 0x4 + x*8]);
			alBank->inst[x]->originalOffset = offsetInstrument;

			if (offsetInstrument == 0x0000)
			{
				alBank->inst[x]->soundCount = 0;
				alBank->inst[x]->sounds = NULL;
				continue;
			}

			offsetInstrument = ctlOffset + offsetInstrument;

			alBank->inst[x]->soundCount = CharArrayToLong(&ctl[offsetInstrument]);
			alBank->inst[x]->countPercussion = CharArrayToLong(&ctl[offsetInstrument+4]);
			alBank->inst[x]->unknown2 = CharArrayToLong(&ctl[offsetInstrument+8]);
			alBank->inst[x]->unknown3 = CharArrayToLong(&ctl[offsetInstrument+0xC]);

			unsigned long offsetSoundData = offsetInstrument + 0x10;

			alBank->inst[x]->tblBase = CharArrayToLong(&tbl[0x4 + 0x8 * x]);
			unsigned long tblLength = CharArrayToLong(&tbl[0x4 + 0x8 * x + 0x4]);

			if ((alBank->inst[x]->tblBase + tblLength) > tblSize)
				tblSize = (alBank->inst[x]->tblBase + tblLength);

			
			if (alBank->inst[x]->countPercussion > 0)
			{
				unsigned long percussionOffset = offsetSoundData + CharArrayToLong(&ctl[offsetInstrument+0x10]);
				alBank->inst[x]->alPercussion = new ALPercussion[alBank->inst[x]->countPercussion];

				for (int y = 0; y < alBank->inst[x]->countPercussion; y++)
				{
					unsigned long percussionItemOffset = offsetSoundData + CharArrayToLong(&ctl[percussionOffset+(4*y)]);
					
					unsigned long offsetWaveTable = offsetSoundData + CharArrayToLong(&ctl[percussionItemOffset+0x4]);

					alBank->inst[x]->alPercussion[y].unknown1 = CharArrayToLong(&ctl[percussionItemOffset+0x0]);
					alBank->inst[x]->alPercussion[y].unknown2 = CharArrayToLong(&ctl[percussionItemOffset+0x8]);

					unsigned long offsetEnvTable = offsetSoundData + CharArrayToLong(&ctl[percussionItemOffset+0xC]);

					alBank->inst[x]->alPercussion[y].env.attackTime = CharArrayToLong(&ctl[offsetEnvTable+ 0x0]);
					alBank->inst[x]->alPercussion[y].env.decayTime = CharArrayToLong(&ctl[offsetEnvTable+ 0x4]);
					alBank->inst[x]->alPercussion[y].env.attackVolumeLong = CharArrayToLong(&ctl[offsetEnvTable+ 0x8]);
					alBank->inst[x]->alPercussion[y].env.decayVolumeLong = CharArrayToLong(&ctl[offsetEnvTable+ 0xC]);
					alBank->inst[x]->alPercussion[y].env.offset = offsetEnvTable;

					alBank->inst[x]->alPercussion[y].wav.unknown1 = CharArrayToLong(&ctl[offsetWaveTable + 0x0]);
					alBank->inst[x]->alPercussion[y].wav.base = alBank->inst[x]->tblBase + CharArrayToLong(&ctl[offsetWaveTable + 0x4]);
					alBank->inst[x]->alPercussion[y].wav.len = CharArrayToLong(&ctl[offsetWaveTable + 0x10]);
					alBank->inst[x]->alPercussion[y].wav.unknown2 = CharArrayToLong(&ctl[offsetWaveTable + 0x14]);
					alBank->inst[x]->alPercussion[y].wav.unknown3 = CharArrayToLong(&ctl[offsetWaveTable + 0x18]);
					alBank->inst[x]->alPercussion[y].wav.unknown4 = CharArrayToLong(&ctl[offsetWaveTable + 0x1C]);

					alBank->inst[x]->alPercussion[y].wav.wavData = new unsigned char[alBank->inst[x]->alPercussion[y].wav.len];

					for (int  r = 0; r < alBank->inst[x]->alPercussion[y].wav.len; r++)
					{
						alBank->inst[x]->alPercussion[y].wav.wavData[r] = tbl[alBank->inst[x]->alPercussion[y].wav.base + r];
					}
					
					alBank->inst[x]->alPercussion[y].wav.type = AL_ADPCM_WAVE;
					alBank->inst[x]->alPercussion[y].wav.flags = 0;

					alBank->inst[x]->alPercussion[y].wav.adpcmWave = new ALADPCMWaveInfo();
					unsigned long loopOffset;
					unsigned long predictorOffset;
					if (marioKartStyle)
					{
						loopOffset = CharArrayToLong(&ctl[offsetWaveTable + 0x8]);
						predictorOffset = CharArrayToLong(&ctl[offsetWaveTable + 0xC]);

						if ((predictorOffset > 0xF00000) || (loopOffset > 0xF00000))
						{
							loopOffset = 0;
							predictorOffset = 0;
						}
					}
					else
					{
						loopOffset = CharArrayToLong(&ctl[offsetWaveTable + 0x8]);
						predictorOffset = CharArrayToLong(&ctl[offsetWaveTable + 0xC]);
					}

					if (loopOffset != 0)
					{
						loopOffset = offsetSoundData + loopOffset;

						alBank->inst[x]->alPercussion[y].wav.adpcmWave->loop = new ALADPCMloop();
						alBank->inst[x]->alPercussion[y].wav.adpcmWave->loop->start = CharArrayToLong(&ctl[loopOffset + 0x0]);
						alBank->inst[x]->alPercussion[y].wav.adpcmWave->loop->end = CharArrayToLong(&ctl[loopOffset + 0x4]);
						alBank->inst[x]->alPercussion[y].wav.adpcmWave->loop->count = CharArrayToLong(&ctl[loopOffset + 0x8]);
						alBank->inst[x]->alPercussion[y].wav.adpcmWave->loop->unknown1 = CharArrayToLong(&ctl[loopOffset + 0xC]);

						if (alBank->inst[x]->alPercussion[y].wav.adpcmWave->loop->start != 0)
						{
							for (int z = 0; z < 0x10; z++)
							{
								alBank->inst[x]->alPercussion[y].wav.adpcmWave->loop->state[z] = CharArrayToShort(&ctl[loopOffset + 0x10 + z * 2]);
							}
						}
					}
					else
					{
						alBank->inst[x]->alPercussion[y].wav.adpcmWave->loop = NULL;
					}

					if (predictorOffset != 0)
					{
						predictorOffset = offsetSoundData + predictorOffset;

						alBank->inst[x]->alPercussion[y].wav.adpcmWave->book = new ALADPCMBook();
						alBank->inst[x]->alPercussion[y].wav.adpcmWave->book->order = CharArrayToLong(&ctl[predictorOffset + 0x0]);
						alBank->inst[x]->alPercussion[y].wav.adpcmWave->book->npredictors = CharArrayToLong(&ctl[predictorOffset + 0x4]);
						alBank->inst[x]->alPercussion[y].wav.adpcmWave->book->predictors = new signed short[alBank->inst[x]->alPercussion[y].wav.adpcmWave->book->order * alBank->inst[x]->alPercussion[y].wav.adpcmWave->book->npredictors * 8];

						for (int z = 0; z < alBank->inst[x]->alPercussion[y].wav.adpcmWave->book->order * alBank->inst[x]->alPercussion[y].wav.adpcmWave->book->npredictors * 8; z++)
						{
							alBank->inst[x]->alPercussion[y].wav.adpcmWave->book->predictors[z] = (signed short)CharArrayToShort(&ctl[predictorOffset + 0x8 + z * 2]);
						}
					}
					else
					{
						alBank->inst[x]->alPercussion[y].wav.adpcmWave->book = NULL;
					}
				}

			}
			else
			{
				alBank->inst[x]->alPercussion = NULL;
			}

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
					

					unsigned long offsetSound = CharArrayToLong(&ctl[offsetSoundData + 4 + y*0x4]);
					if (offsetSound == 0x0)
					{
						delete [] alBank->inst[x]->sounds[y];
						alBank->inst[x]->sounds[y] = NULL;
						continue;
					}

					offsetSound = offsetSoundData + offsetSound;

					alBank->inst[x]->sounds[y]->flags = 0x0;
					
					unsigned long offsetEnvTable = CharArrayToLong(&ctl[offsetSound+ 0x4]);
					offsetEnvTable = offsetSoundData + offsetEnvTable;
					alBank->inst[x]->sounds[y]->env.attackTime = CharArrayToLong(&ctl[offsetEnvTable+ 0x0]);
					alBank->inst[x]->sounds[y]->env.decayTime = CharArrayToLong(&ctl[offsetEnvTable+ 0x4]);
					alBank->inst[x]->sounds[y]->env.attackVolumeLong = CharArrayToLong(&ctl[offsetEnvTable+ 0x8]);
					alBank->inst[x]->sounds[y]->env.decayVolumeLong = CharArrayToLong(&ctl[offsetEnvTable+ 0xC]);
					alBank->inst[x]->sounds[y]->env.offset = offsetEnvTable;

					alBank->inst[x]->sounds[y]->unknown1 = CharArrayToLong(&ctl[offsetSound+ 0x0]);
					alBank->inst[x]->sounds[y]->unknown2 = CharArrayToLong(&ctl[offsetSound+ 0xC]);
					alBank->inst[x]->sounds[y]->unknown3 = CharArrayToLong(&ctl[offsetSound+ 0x14]);
					alBank->inst[x]->sounds[y]->unknown4 = CharArrayToLong(&ctl[offsetSound+ 0x1C]);

					unsigned long offsetWaveTable = CharArrayToLong(&ctl[offsetSound+ 0x10]);
					offsetWaveTable = offsetSoundData + offsetWaveTable;

					alBank->inst[x]->sounds[y]->wav.unknown1 = CharArrayToLong(&ctl[offsetWaveTable + 0x0]);
					alBank->inst[x]->sounds[y]->wav.base = alBank->inst[x]->tblBase + CharArrayToLong(&ctl[offsetWaveTable + 0x4]);
					alBank->inst[x]->sounds[y]->wav.len = CharArrayToLong(&ctl[offsetWaveTable + 0x10]);
					alBank->inst[x]->sounds[y]->wav.unknown2 = CharArrayToLong(&ctl[offsetWaveTable + 0x14]);
					alBank->inst[x]->sounds[y]->wav.unknown3 = CharArrayToLong(&ctl[offsetWaveTable + 0x18]);
					alBank->inst[x]->sounds[y]->wav.unknown4 = CharArrayToLong(&ctl[offsetWaveTable + 0x1C]);

					alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];

					for (int  r = 0; r < alBank->inst[x]->sounds[y]->wav.len; r++)
					{
						alBank->inst[x]->sounds[y]->wav.wavData[r] = tbl[alBank->inst[x]->sounds[y]->wav.base + r];
					}
					
					alBank->inst[x]->sounds[y]->wav.type = AL_ADPCM_WAVE;
					alBank->inst[x]->sounds[y]->wav.flags = 0;
					// MUST PAD to 4s

					if ((alBank->inst[x]->sounds[y]->wav.type == AL_ADPCM_WAVE) && ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1)))
					{
						alBank->inst[x]->sounds[y]->wav.adpcmWave = new ALADPCMWaveInfo();
						unsigned long loopOffset = CharArrayToLong(&ctl[offsetWaveTable + 0x8]);
						unsigned long predictorOffset = CharArrayToLong(&ctl[offsetWaveTable + 0xC]);

						if (loopOffset != 0)
						{
							loopOffset = offsetSoundData + loopOffset;

							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop = new ALADPCMloop();
							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->start = CharArrayToLong(&ctl[loopOffset + 0x0]);
							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->end = CharArrayToLong(&ctl[loopOffset + 0x4]);
							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->count = CharArrayToLong(&ctl[loopOffset + 0x8]);
							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->unknown1 = CharArrayToLong(&ctl[loopOffset + 0xC]);

							if (alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->start != 0)
							{
								for (int z = 0; z < 0x10; z++)
								{
									alBank->inst[x]->sounds[y]->wav.adpcmWave->loop->state[z] = CharArrayToShort(&ctl[loopOffset + 0x10 + z * 2]);
								}
							}
						}
						else
						{
							alBank->inst[x]->sounds[y]->wav.adpcmWave->loop = NULL;
						}

						if (predictorOffset != 0)
						{
							predictorOffset = offsetSoundData + predictorOffset;

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


					unsigned long offsetWaveTableSecondary = CharArrayToLong(&ctl[offsetSound+ 0x18]);
					if (offsetWaveTableSecondary == 0x00000000)
						alBank->inst[x]->sounds[y]->hasWaveSecondary = false;
					else
					{
						alBank->inst[x]->sounds[y]->hasWaveSecondary = true;
						offsetWaveTableSecondary = offsetSoundData + offsetWaveTableSecondary;

						alBank->inst[x]->sounds[y]->wavSecondary.unknown1 = CharArrayToLong(&ctl[offsetWaveTableSecondary + 0x0]);
						alBank->inst[x]->sounds[y]->wavSecondary.base = alBank->inst[x]->tblBase + CharArrayToLong(&ctl[offsetWaveTableSecondary + 0x4]);
						alBank->inst[x]->sounds[y]->wavSecondary.len = CharArrayToLong(&ctl[offsetWaveTableSecondary + 0x10]);
						alBank->inst[x]->sounds[y]->wavSecondary.unknown2 = CharArrayToLong(&ctl[offsetWaveTableSecondary + 0x14]);
						alBank->inst[x]->sounds[y]->wavSecondary.unknown3 = CharArrayToLong(&ctl[offsetWaveTableSecondary + 0x18]);
						alBank->inst[x]->sounds[y]->wavSecondary.unknown4 = CharArrayToLong(&ctl[offsetWaveTableSecondary + 0x1C]);

						alBank->inst[x]->sounds[y]->wavSecondary.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wavSecondary.len];

						for (int  r = 0; r < alBank->inst[x]->sounds[y]->wavSecondary.len; r++)
						{
							alBank->inst[x]->sounds[y]->wavSecondary.wavData[r] = tbl[alBank->inst[x]->sounds[y]->wavSecondary.base + r];
						}
						
						alBank->inst[x]->sounds[y]->wavSecondary.type = AL_ADPCM_WAVE;
						alBank->inst[x]->sounds[y]->wavSecondary.flags = 0;
						// MUST PAD to 4s

						if ((alBank->inst[x]->sounds[y]->wavSecondary.type == AL_ADPCM_WAVE) && ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1)))
						{
							alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave = new ALADPCMWaveInfo();
							unsigned long loopOffsetSecondary = CharArrayToLong(&ctl[offsetWaveTableSecondary + 0x8]);
							unsigned long predictorOffsetSecondary = CharArrayToLong(&ctl[offsetWaveTableSecondary + 0xC]);

							if (loopOffsetSecondary != 0)
							{
								loopOffsetSecondary = offsetSoundData + loopOffsetSecondary;

								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop = new ALADPCMloop();
								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->start = CharArrayToLong(&ctl[loopOffsetSecondary + 0x0]);
								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->end = CharArrayToLong(&ctl[loopOffsetSecondary + 0x4]);
								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->count = CharArrayToLong(&ctl[loopOffsetSecondary + 0x8]);
								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->unknown1 = CharArrayToLong(&ctl[loopOffsetSecondary + 0xC]);

								if (alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->start != 0)
								{
									for (int z = 0; z < 0x10; z++)
									{
										alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->state[z] = CharArrayToShort(&ctl[loopOffsetSecondary + 0x10 + z * 2]);
									}
								}
							}
							else
							{
								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop = NULL;
							}

							if (predictorOffsetSecondary != 0)
							{
								predictorOffsetSecondary = offsetSoundData + predictorOffsetSecondary;

								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book = new ALADPCMBook();
								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->order = CharArrayToLong(&ctl[predictorOffsetSecondary + 0x0]);
								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->npredictors = CharArrayToLong(&ctl[predictorOffsetSecondary + 0x4]);
								alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->predictors = new signed short[alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->npredictors * 8];

								for (int z = 0; z < alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->npredictors * 8; z++)
								{
									alBank->inst[x]->sounds[y]->wavSecondary.adpcmWave->book->predictors[z] = (signed short)CharArrayToShort(&ctl[predictorOffsetSecondary + 0x8 + z * 2]);
								}
							}
						}
					}

					unsigned long offsetWaveTablePrevious = CharArrayToLong(&ctl[offsetSound+ 0x8]);
					if (offsetWaveTablePrevious == 0x00000000)
						alBank->inst[x]->sounds[y]->hasWavePrevious = false;
					else
					{
						alBank->inst[x]->sounds[y]->hasWavePrevious = true;
						offsetWaveTablePrevious = offsetSoundData + offsetWaveTablePrevious;

						alBank->inst[x]->sounds[y]->wavPrevious.unknown1 = CharArrayToLong(&ctl[offsetWaveTablePrevious + 0x0]);
						alBank->inst[x]->sounds[y]->wavPrevious.base = alBank->inst[x]->tblBase + CharArrayToLong(&ctl[offsetWaveTablePrevious + 0x4]);
						alBank->inst[x]->sounds[y]->wavPrevious.len = CharArrayToLong(&ctl[offsetWaveTablePrevious + 0x10]);
						alBank->inst[x]->sounds[y]->wavPrevious.unknown2 = CharArrayToLong(&ctl[offsetWaveTablePrevious + 0x14]);
						alBank->inst[x]->sounds[y]->wavPrevious.unknown3 = CharArrayToLong(&ctl[offsetWaveTablePrevious + 0x18]);
						alBank->inst[x]->sounds[y]->wavPrevious.unknown4 = CharArrayToLong(&ctl[offsetWaveTablePrevious + 0x1C]);

						alBank->inst[x]->sounds[y]->wavPrevious.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wavPrevious.len];

						for (int  r = 0; r < alBank->inst[x]->sounds[y]->wavPrevious.len; r++)
						{
							alBank->inst[x]->sounds[y]->wavPrevious.wavData[r] = tbl[alBank->inst[x]->sounds[y]->wavPrevious.base + r];
						}
						
						alBank->inst[x]->sounds[y]->wavPrevious.type = AL_ADPCM_WAVE;
						alBank->inst[x]->sounds[y]->wavPrevious.flags = 0;
						// MUST PAD to 4s

						if ((alBank->inst[x]->sounds[y]->wavPrevious.type == AL_ADPCM_WAVE) && ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1)))
						{
							alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave = new ALADPCMWaveInfo();
							unsigned long loopOffsetPrevious = CharArrayToLong(&ctl[offsetWaveTablePrevious + 0x8]);
							unsigned long predictorOffsetPrevious = CharArrayToLong(&ctl[offsetWaveTablePrevious + 0xC]);

							if (loopOffsetPrevious != 0)
							{
								loopOffsetPrevious = offsetSoundData + loopOffsetPrevious;

								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop = new ALADPCMloop();
								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->start = CharArrayToLong(&ctl[loopOffsetPrevious + 0x0]);
								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->end = CharArrayToLong(&ctl[loopOffsetPrevious + 0x4]);
								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->count = CharArrayToLong(&ctl[loopOffsetPrevious + 0x8]);
								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->unknown1 = CharArrayToLong(&ctl[loopOffsetPrevious + 0xC]);

								if (alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->start != 0)
								{
									for (int z = 0; z < 0x10; z++)
									{
										alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->state[z] = CharArrayToShort(&ctl[loopOffsetPrevious + 0x10 + z * 2]);
									}
								}
							}
							else
							{
								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop = NULL;
							}

							if (predictorOffsetPrevious != 0)
							{
								predictorOffsetPrevious = offsetSoundData + predictorOffsetPrevious;

								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book = new ALADPCMBook();
								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->order = CharArrayToLong(&ctl[predictorOffsetPrevious + 0x0]);
								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->npredictors = CharArrayToLong(&ctl[predictorOffsetPrevious + 0x4]);
								alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->predictors = new signed short[alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->npredictors * 8];

								for (int z = 0; z < alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->order * alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->npredictors * 8; z++)
								{
									alBank->inst[x]->sounds[y]->wavPrevious.adpcmWave->book->predictors[z] = (signed short)CharArrayToShort(&ctl[predictorOffsetPrevious + 0x8 + z * 2]);
								}
							}
						}
					}
				}
			}
		}
	}
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioRawAllowed(unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl, int ctlFlaggedOffset, unsigned long mask, int bankNumber)
{
	ALBank* alBank = ReadAudio(ctl, ctlSize, ctlOffset, tbl, ctlFlaggedOffset, mask, bankNumber);
	alBank->soundBankFormat = STANDARDFORMATRAWALLOWED;
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioN64PtrWavetableV2ZLIB(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl)
{
	int fileSizeCompressed = -1;
	GECompression gedecompress;
	gedecompress.SetGame(MORTALKOMBAT);
	gedecompress.SetCompressedBuffer(&ctl[ctlOffset], ctlSize);
	int fileSizeUncompressed;
	unsigned char* outputDecompressed = gedecompress.OutputDecompressedBuffer(fileSizeUncompressed, fileSizeCompressed);

	ALBank* alBank = NULL;
	if (outputDecompressed != NULL)
	{
		unsigned long fileSizeUncompressedLong = fileSizeUncompressed;
		alBank = ReadAudioN64PtrWavetableV2(&outputDecompressed[0], fileSizeUncompressedLong, 0, tbl);
		alBank->soundBankFormat = N64PTRWAVETABLETABLEV2ZLIB;
		delete [] outputDecompressed;
	}
	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudioRawTest(unsigned char* rawData, int size)
{
	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = STANDARDFORMAT;
	alBank->count = 1;
	alBank->flags = 0;
	alBank->pad = 0;
	alBank->samplerate = 11050;
	alBank->percussion = 0;
	
	alBank->inst = NULL;

	if ((alBank->flags == 0x0000) || (alBank->flags == 0x0100)) // offset
	{
		alBank->inst = new ALInst*[alBank->count];

		for (int x = 0; x < alBank->count; x++)
		{
			alBank->inst[x] = new ALInst();
			alBank->inst[x]->samplerate = 0;
			alBank->inst[x]->sounds = NULL;
			alBank->inst[x]->alPercussion = NULL;
		}

		for (int x = 0; x < alBank->count; x++)
		{
			alBank->inst[x]->volume = 0;
			alBank->inst[x]->pan = 0;
			alBank->inst[x]->priority = 0;
			alBank->inst[x]->flags = 0;
			alBank->inst[x]->tremType = 0;
			alBank->inst[x]->tremRate = 0;
			alBank->inst[x]->tremDepth = 0;
			alBank->inst[x]->tremDelay = 0;
			alBank->inst[x]->vibType = 0;
			alBank->inst[x]->vibRate = 0;
			alBank->inst[x]->vibDepth = 0;
			alBank->inst[x]->vibDelay = 0;
			alBank->inst[x]->bendRange = 0;
			alBank->inst[x]->soundCount = 1;

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
					alBank->inst[x]->sounds[y]->samplePan = 0;
					alBank->inst[x]->sounds[y]->sampleVolume = 0;
					alBank->inst[x]->sounds[y]->flags = 0;

					
					if ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1))
					{
						alBank->inst[x]->sounds[y]->env.attackTime = 0;
						alBank->inst[x]->sounds[y]->env.decayTime = 0;
						alBank->inst[x]->sounds[y]->env.releaseTime = 0;
						alBank->inst[x]->sounds[y]->env.attackVolume = 0;
						alBank->inst[x]->sounds[y]->env.decayVolume = 0;

						alBank->inst[x]->sounds[y]->key.velocitymin = 0;
						alBank->inst[x]->sounds[y]->key.velocitymax = 0;
						alBank->inst[x]->sounds[y]->key.keymin = 0;
						alBank->inst[x]->sounds[y]->key.keymax = 0;
						alBank->inst[x]->sounds[y]->key.keybase = 0;
						alBank->inst[x]->sounds[y]->key.detune = 0;

						alBank->inst[x]->sounds[y]->wav.base = 0;
						alBank->inst[x]->sounds[y]->wav.len = size;
						alBank->inst[x]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[x]->sounds[y]->wav.len];

						for (int  r = 0; r < alBank->inst[x]->sounds[y]->wav.len; r++)
						{
							alBank->inst[x]->sounds[y]->wav.wavData[r] = rawData[alBank->inst[x]->sounds[y]->wav.base + r];
						}
						

						alBank->inst[x]->sounds[y]->wav.type = AL_RAW16_WAVE;
						alBank->inst[x]->sounds[y]->wav.flags = 0;
						// MUST PAD to 4s

					
						alBank->inst[x]->sounds[y]->wav.rawWave = new ALRAWWaveInfo();
						alBank->inst[x]->sounds[y]->wav.rawWave->loop = NULL;
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

ALBank* CN64AIFCAudio::ReadAudio64dd(unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl)
{
	unsigned long header = CharArrayToLong(&ctl[ctlOffset]);
	if (header != 0x19970123)
	{
		MessageBox(NULL, "Error", "Bad header for 64dd bank", NULL);
		return NULL;
	}

	int numberWaveforms = CharArrayToLong(&ctl[ctlOffset+4]);
	unsigned long wavePointer = ctlOffset + CharArrayToLong(&ctl[ctlOffset+8]);
	unsigned long bookPointer = ctlOffset + CharArrayToLong(&ctl[ctlOffset+0xC]);
	unsigned long loopPointer = ctlOffset + CharArrayToLong(&ctl[ctlOffset+0x10]);
	unsigned long bookDataPointer = ctlOffset + CharArrayToLong(&ctl[ctlOffset+0x14]);
	unsigned long loopDataPointer = ctlOffset + CharArrayToLong(&ctl[ctlOffset+0x18]);
	unsigned long informationPointer = ctlOffset + CharArrayToLong(&ctl[ctlOffset+0x1C]);

	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = N64DD;
	alBank->count = 1;
	alBank->flags = 0;
	alBank->percussion = NULL;

	alBank->inst = new ALInst*[alBank->count];

	alBank->inst[0] = new ALInst();
	alBank->inst[0]->samplerate = 32000;
	alBank->inst[0]->sounds = NULL;
	alBank->inst[0]->alPercussion = NULL;

	alBank->inst[0]->soundCount = numberWaveforms;

	alBank->inst[0]->sounds = new ALSound*[alBank->inst[0]->soundCount];

	for (int y = 0; y < alBank->inst[0]->soundCount; y++)
	{
		alBank->inst[0]->sounds[y] = new ALSound();
		alBank->inst[0]->sounds[y]->wav.wavData = NULL;
	}

	for (int y = 0; y < alBank->inst[0]->soundCount; y++)
	{
		unsigned long wavePointerSound = ctlOffset + CharArrayToLong(&ctl[wavePointer + (y * 4)]);
		unsigned long bookPointerSound = ctlOffset + CharArrayToLong(&ctl[bookPointer + (y * 4)]);
		unsigned long loopPointerSound = ctlOffset + CharArrayToLong(&ctl[loopPointer + (y * 4)]);
		unsigned long informationPointerSound = informationPointer + (y * 0x20);

		WaveInformation waveInfo;
		waveInfo.version = CharArrayToLong(&ctl[informationPointerSound]);
		waveInfo.adpcmsize = CharArrayToLong(&ctl[informationPointerSound+4]);
		waveInfo.samplesize = CharArrayToLong(&ctl[informationPointerSound+8]);
		waveInfo.loop_startsamplepoint = CharArrayToLong(&ctl[informationPointerSound+0xC]);
		waveInfo.loop_endsamplepoint = CharArrayToLong(&ctl[informationPointerSound+0x10]);
		waveInfo.loop_count = CharArrayToShort(&ctl[informationPointerSound+0x14]);
		waveInfo.book_bytes = ctl[informationPointerSound+0x16];
		waveInfo.samplingbaseMIDIkey = ctl[informationPointerSound+0x17];
		waveInfo.samplingrate = CharArrayToFloat(&ctl[informationPointerSound+0x18]);
		waveInfo.tuning_C4_32K = CharArrayToFloat(&ctl[informationPointerSound+0x1C]);
		
		alBank->inst[0]->sounds[y]->flags = 0;

		alBank->inst[0]->sounds[y]->wav.sampleRateNotInDefaultNintendoSpec = waveInfo.samplingrate;
		alBank->inst[0]->sounds[y]->key.keybase = waveInfo.samplingbaseMIDIkey;
		alBank->inst[0]->sounds[y]->key.detune = waveInfo.tuning_C4_32K;

		alBank->inst[0]->sounds[y]->wav.base = wavePointerSound;
		alBank->inst[0]->sounds[y]->wav.len = waveInfo.adpcmsize;
		alBank->inst[0]->sounds[y]->wav.wavData = new unsigned char[alBank->inst[0]->sounds[y]->wav.len];

		for (int  r = 0; r < alBank->inst[0]->sounds[y]->wav.len; r++)
		{
			alBank->inst[0]->sounds[y]->wav.wavData[r] = tbl[alBank->inst[0]->sounds[y]->wav.base + r];
		}
		

		alBank->inst[0]->sounds[y]->wav.type = AL_ADPCM_WAVE;
		alBank->inst[0]->sounds[y]->wav.flags = 0;
		// MUST PAD to 4s
		

		if ((alBank->inst[0]->sounds[y]->wav.type == AL_ADPCM_WAVE) && ((alBank->inst[0]->sounds[y]->flags == 0x0) || (alBank->inst[0]->sounds[y]->flags == 0x1)))
		{
			alBank->inst[0]->sounds[y]->wav.adpcmWave = new ALADPCMWaveInfo();
			
			alBank->inst[0]->sounds[y]->wav.adpcmWave->loop = new ALADPCMloop();
			alBank->inst[0]->sounds[y]->wav.adpcmWave->loop->start = waveInfo.loop_startsamplepoint;
			alBank->inst[0]->sounds[y]->wav.adpcmWave->loop->end = waveInfo.loop_endsamplepoint;
			alBank->inst[0]->sounds[y]->wav.adpcmWave->loop->count = waveInfo.loop_count;
			for (int z = 0; z < 0x10; z++)
			{
				alBank->inst[0]->sounds[y]->wav.adpcmWave->loop->state[z] = CharArrayToShort(&ctl[loopPointerSound + z * 2]);
			}

			alBank->inst[0]->sounds[y]->wav.adpcmWave->book = new ALADPCMBook();
			alBank->inst[0]->sounds[y]->wav.adpcmWave->book->order = 2;
			alBank->inst[0]->sounds[y]->wav.adpcmWave->book->npredictors = 2;
			alBank->inst[0]->sounds[y]->wav.adpcmWave->book->predictors = new signed short[alBank->inst[0]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[0]->sounds[y]->wav.adpcmWave->book->npredictors * 8];

			for (int z = 0; z < alBank->inst[0]->sounds[y]->wav.adpcmWave->book->order * alBank->inst[0]->sounds[y]->wav.adpcmWave->book->npredictors * 8; z++)
			{
				alBank->inst[0]->sounds[y]->wav.adpcmWave->book->predictors[z] = (signed short)CharArrayToShort(&ctl[bookPointerSound + z * 2]);
			}
		}
		else
		{
			DisposeALBank(alBank);
			MessageBox(NULL, "Error", "Unsupported type in ALSound wav", NULL);
			return NULL;
		}
	}

	return alBank;
}

ALBank* CN64AIFCAudio::ReadAudio(unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl, int ctlFlaggedOffset, unsigned long mask, int bankNumber)
{
	unsigned short numberBands = CharArrayToShort(&ctl[ctlOffset+0x2]);
	unsigned long bankOffset = CharArrayToLong(&ctl[ctlOffset+ 0x4 + (bankNumber * 0x4)]);
	if (((bankOffset >> 16) & 0xF0) > 0)
		bankOffset = ctlFlaggedOffset + (bankOffset - mask);
	else
		bankOffset = ctlOffset + bankOffset;

	ALBank* alBank = new ALBank();
	alBank->soundBankFormat = STANDARDFORMAT;
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
			alBank->inst[x]->samplerate = 0;
			alBank->inst[x]->sounds = NULL;
			alBank->inst[x]->alPercussion = NULL;
		}

		for (int x = 0; x < alBank->count; x++)
		{
			unsigned long offsetInstrument = CharArrayToLong(&ctl[bankOffset+ 0xC + x*4]);
			if (offsetInstrument == 0x0000)
			{
				alBank->inst[x]->soundCount = 0;
				alBank->inst[x]->sounds = NULL;
				continue;
			}

			if (alBank->flags == 0x0100)
				offsetInstrument = ctlFlaggedOffset + (offsetInstrument - mask);
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
						offsetSound = ctlFlaggedOffset + (offsetSound - mask);
					else
						offsetSound = ctlOffset + offsetSound;

					alBank->inst[x]->sounds[y]->samplePan = ctl[offsetSound+0xC];
					alBank->inst[x]->sounds[y]->sampleVolume = ctl[offsetSound+0xD];
					alBank->inst[x]->sounds[y]->flags = ctl[offsetSound+0xE];

					
					if ((alBank->inst[x]->sounds[y]->flags == 0x0) || (alBank->inst[x]->sounds[y]->flags == 0x1))
					{
						unsigned long offsetEnv = CharArrayToLong(&ctl[offsetSound+ 0x0]);
						if (alBank->inst[x]->sounds[y]->flags == 0x1)
							offsetEnv = ctlFlaggedOffset + (offsetEnv - mask);
						else
							offsetEnv = ctlOffset + offsetEnv;

						unsigned long offsetKeyMap = CharArrayToLong(&ctl[offsetSound+ 0x4]);
						if (alBank->inst[x]->sounds[y]->flags == 0x1)
							offsetKeyMap = ctlFlaggedOffset + (offsetKeyMap - mask);
						else
							offsetKeyMap = ctlOffset + offsetKeyMap;

						unsigned long offsetWaveTable = CharArrayToLong(&ctl[offsetSound+ 0x8]);
						if (alBank->inst[x]->sounds[y]->flags == 0x1)
							offsetWaveTable = ctlFlaggedOffset + (offsetWaveTable - mask);
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
									loopOffset = ctlFlaggedOffset + (loopOffset - mask);
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
									predictorOffset = ctlFlaggedOffset + (predictorOffset - mask);
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
									loopOffset = ctlFlaggedOffset + (loopOffset - mask);
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

			results[numResults].bank = ReadAudio(&ROM[0], results[numResults].ctlSize, results[numResults].ctlOffset, &ROM[results[numResults].tblOffset], 0, 0xFFFFFFFF, 0);

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

		// to not make zelda crash but doesn't fix it
		if (pred >= (book->npredictors * book->order))
			pred -= (book->npredictors * book->order);
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

signed short* CN64AIFCAudio::determineBestPredictors(ALBank* alBank, unsigned long& npredictors, unsigned long& norder, signed short* inPCMSamples, int numberSamplesIn)
{
	int numPredTests = 0xB;
	
	signed short** predictors = new signed short*[numPredTests];
	float* entropySet = new float[numPredTests];

	for (int x = 0; x < numPredTests; x++)
	{
		predictors[x] = new signed short[0x10];
		entropySet[x] = 0;
	}

	int predNum = 0;

	// todo find some way to calculate this
	predictors[predNum][0] = 0xFAE2;
	predictors[predNum][1] = 0xFAD0;
	predictors[predNum][2] = 0xFE04;
	predictors[predNum][3] = 0x014F;
	predictors[predNum][4] = 0x0298;
	predictors[predNum][5] = 0x01CB;
	predictors[predNum][6] = 0x0029;
	predictors[predNum][7] = 0xFF03;
	predictors[predNum][8] = 0x081C;
	predictors[predNum][9] = 0x031A;
	predictors[predNum][10] = 0xFDF5;
	predictors[predNum][11] = 0xFBF2;
	predictors[predNum][12] = 0xFD32;
	predictors[predNum][13] = 0xFFC1;
	predictors[predNum][14] = 0x018B;
	predictors[predNum][15] = 0x01B9;

	unsigned long vadpcmOutputLength;
	unsigned char* vadpcmData = new unsigned char[numberSamplesIn];

	ALADPCMBook book;
	book.npredictors = 1;
	book.order = 2;
	book.predictors = predictors[predNum];
	entropySet[predNum] = encode(inPCMSamples, numberSamplesIn, vadpcmData, vadpcmOutputLength, &book);

	delete [] vadpcmData;
	predNum++;




	predictors[predNum][0] = 0xF8A1;
	predictors[predNum][1] = 0xF25D;
	predictors[predNum][2] = 0xED92;
	predictors[predNum][3] = 0xEA7A;
	predictors[predNum][4] = 0xE92C;
	predictors[predNum][5] = 0xE99D;
	predictors[predNum][6] = 0xEBA1;
	predictors[predNum][7] = 0xEEF3;
	predictors[predNum][8] = 0x0ECC;
	predictors[predNum][9] = 0x1400;
	predictors[predNum][10] = 0x175B;
	predictors[predNum][11] = 0x18C5;
	predictors[predNum][12] = 0x184B;
	predictors[predNum][13] = 0x161B;
	predictors[predNum][14] = 0x1280;
	predictors[predNum][15] = 0x0DDA;

	vadpcmData = new unsigned char[numberSamplesIn];

	book.npredictors = 1;
	book.order = 2;
	book.predictors = predictors[predNum];
	entropySet[predNum] = encode(inPCMSamples, numberSamplesIn, vadpcmData, vadpcmOutputLength, &book);

	delete [] vadpcmData;
	predNum++;


	predictors[predNum][0] = 0xFD78;
	predictors[predNum][1] = 0xFF80;
	predictors[predNum][2] = 0x00B4;
	predictors[predNum][3] = 0x004C;
	predictors[predNum][4] = 0xFFD6;
	predictors[predNum][5] = 0xFFE0;
	predictors[predNum][6] = 0x0007;
	predictors[predNum][7] = 0x000C;
	predictors[predNum][8] = 0x0194;
	predictors[predNum][9] = 0xFDC8;
	predictors[predNum][10] = 0xFF10;
	predictors[predNum][11] = 0x0085;
	predictors[predNum][12] = 0x0066;
	predictors[predNum][13] = 0xFFEA;
	predictors[predNum][14] = 0xFFDB;
	predictors[predNum][15] = 0x0000;

	vadpcmData = new unsigned char[numberSamplesIn];

	book.npredictors = 1;
	book.order = 2;
	book.predictors = predictors[predNum];
	entropySet[predNum] = encode(inPCMSamples, numberSamplesIn, vadpcmData, vadpcmOutputLength, &book);

	delete [] vadpcmData;
	predNum++;


	predictors[predNum][0] = 0x0210;
	predictors[predNum][1] = 0x00FD;
	predictors[predNum][2] = 0x0101;
	predictors[predNum][3] = 0x00BC;
	predictors[predNum][4] = 0x009C;
	predictors[predNum][5] = 0x007B;
	predictors[predNum][6] = 0x0063;
	predictors[predNum][7] = 0x004F;
	predictors[predNum][8] = 0x03D4;
	predictors[predNum][9] = 0x03E5;
	predictors[predNum][10] = 0x02D9;
	predictors[predNum][11] = 0x025E;
	predictors[predNum][12] = 0x01DE;
	predictors[predNum][13] = 0x0181;
	predictors[predNum][14] = 0x0133;
	predictors[predNum][15] = 0x00F6;

	vadpcmData = new unsigned char[numberSamplesIn];

	book.npredictors = 1;
	book.order = 2;
	book.predictors = predictors[predNum];
	entropySet[predNum] = encode(inPCMSamples, numberSamplesIn, vadpcmData, vadpcmOutputLength, &book);

	delete [] vadpcmData;
	predNum++;


	predictors[predNum][0] = 0x01CF;
	predictors[predNum][1] = 0x003E;
	predictors[predNum][2] = 0x0071;
	predictors[predNum][3] = 0x001D;
	predictors[predNum][4] = 0x001D;
	predictors[predNum][5] = 0x000B;
	predictors[predNum][6] = 0x0008;
	predictors[predNum][7] = 0x0003;
	predictors[predNum][8] = 0x0112;
	predictors[predNum][9] = 0x01F4;
	predictors[predNum][10] = 0x0081;
	predictors[predNum][11] = 0x0082;
	predictors[predNum][12] = 0x002F;
	predictors[predNum][13] = 0x0024;
	predictors[predNum][14] = 0x000F;
	predictors[predNum][15] = 0x000A;

	vadpcmData = new unsigned char[numberSamplesIn];

	book.npredictors = 1;
	book.order = 2;
	book.predictors = predictors[predNum];
	entropySet[predNum] = encode(inPCMSamples, numberSamplesIn, vadpcmData, vadpcmOutputLength, &book);

	delete [] vadpcmData;
	predNum++;

	predictors[predNum][0] = 0xFCC0;
	predictors[predNum][1] = 0xFBEC;
	predictors[predNum][2] = 0xFC33;
	predictors[predNum][3] = 0xFCE2;
	predictors[predNum][4] = 0xFDA2;
	predictors[predNum][5] = 0xFE4B;
	predictors[predNum][6] = 0xFED1;
	predictors[predNum][7] = 0xFF36;
	predictors[predNum][8] = 0x0A0B;
	predictors[predNum][9] = 0x095C;
	predictors[predNum][10] = 0x07AC;
	predictors[predNum][11] = 0x05D5;
	predictors[predNum][12] = 0x0435;
	predictors[predNum][13] = 0x02E9;
	predictors[predNum][14] = 0x01F2;
	predictors[predNum][15] = 0x0143;

	vadpcmData = new unsigned char[numberSamplesIn];

	book.npredictors = 1;
	book.order = 2;
	book.predictors = predictors[predNum];
	entropySet[predNum] = encode(inPCMSamples, numberSamplesIn, vadpcmData, vadpcmOutputLength, &book);

	delete [] vadpcmData;
	predNum++;

	predictors[predNum][0] = 0xFBF6;
	predictors[predNum][1] = 0xFBDF;
	predictors[predNum][2] = 0xFDD1;
	predictors[predNum][3] = 0xFFDA;
	predictors[predNum][4] = 0x00F4;
	predictors[predNum][5] = 0x010C;
	predictors[predNum][6] = 0x0097;
	predictors[predNum][7] = 0x0013;
	predictors[predNum][8] = 0x082E;
	predictors[predNum][9] = 0x0453;
	predictors[predNum][10] = 0x004B;
	predictors[predNum][11] = 0xFE1D;
	predictors[predNum][12] = 0xFDED;
	predictors[predNum][13] = 0xFED4;
	predictors[predNum][14] = 0xFFDA;
	predictors[predNum][15] = 0x0070;

	vadpcmData = new unsigned char[numberSamplesIn];

	book.npredictors = 1;
	book.order = 2;
	book.predictors = predictors[predNum];
	entropySet[predNum] = encode(inPCMSamples, numberSamplesIn, vadpcmData, vadpcmOutputLength, &book);

	delete [] vadpcmData;
	predNum++;

	predictors[predNum][0] = 0xFB63;
	predictors[predNum][1] = 0xF930;
	predictors[predNum][2] = 0xF89A;
	predictors[predNum][3] = 0xF901;
	predictors[predNum][4] = 0xF9F0;
	predictors[predNum][5] = 0xFB15;
	predictors[predNum][6] = 0xFC3C;
	predictors[predNum][7] = 0xFD46;
	predictors[predNum][8] = 0x0BD0;
	predictors[predNum][9] = 0x0CD4;
	predictors[predNum][10] = 0x0C21;
	predictors[predNum][11] = 0x0A84;
	predictors[predNum][12] = 0x0888;
	predictors[predNum][13] = 0x0688;
	predictors[predNum][14] = 0x04BA;
	predictors[predNum][15] = 0x0336;

	vadpcmData = new unsigned char[numberSamplesIn];

	book.npredictors = 1;
	book.order = 2;
	book.predictors = predictors[predNum];
	entropySet[predNum] = encode(inPCMSamples, numberSamplesIn, vadpcmData, vadpcmOutputLength, &book);

	delete [] vadpcmData;
	predNum++;

	predictors[predNum][0] = 0xF9D0;
	predictors[predNum][1] = 0xF692;
	predictors[predNum][2] = 0xF66C;
	predictors[predNum][3] = 0xF8B4;
	predictors[predNum][4] = 0xFC4A;
	predictors[predNum][5] = 0xFFFF;
	predictors[predNum][6] = 0x02DD;
	predictors[predNum][7] = 0x045D;
	predictors[predNum][8] = 0x0C30;
	predictors[predNum][9] = 0x0C62;
	predictors[predNum][10] = 0x096F;
	predictors[predNum][11] = 0x04CB;
	predictors[predNum][12] = 0x0002;
	predictors[predNum][13] = 0xFC4D;
	predictors[predNum][14] = 0xFA5C;
	predictors[predNum][15] = 0xFA44;

	vadpcmData = new unsigned char[numberSamplesIn];

	book.npredictors = 1;
	book.order = 2;
	book.predictors = predictors[predNum];
	entropySet[predNum] = encode(inPCMSamples, numberSamplesIn, vadpcmData, vadpcmOutputLength, &book);

	delete [] vadpcmData;
	predNum++;

	predictors[predNum][0] = 0xFA92;
	predictors[predNum][1] = 0xF99C;
	predictors[predNum][2] = 0xFC2B;
	predictors[predNum][3] = 0xFFD4;
	predictors[predNum][4] = 0x0266;
	predictors[predNum][5] = 0x02F0;
	predictors[predNum][6] = 0x01D5;
	predictors[predNum][7] = 0x0029;
	predictors[predNum][8] = 0x096A;
	predictors[predNum][9] = 0x05A5;
	predictors[predNum][10] = 0x0041;
	predictors[predNum][11] = 0xFC77;
	predictors[predNum][12] = 0xFBAB;
	predictors[predNum][13] = 0xFD4E;
	predictors[predNum][14] = 0xFFC4;
	predictors[predNum][15] = 0x018E;

	vadpcmData = new unsigned char[numberSamplesIn];

	book.npredictors = 1;
	book.order = 2;
	book.predictors = predictors[predNum];
	entropySet[predNum] = encode(inPCMSamples, numberSamplesIn, vadpcmData, vadpcmOutputLength, &book);

	delete [] vadpcmData;
	predNum++;

	predictors[predNum][0] = 0xFB0A;
	predictors[predNum][1] = 0xF88C;
	predictors[predNum][2] = 0xF7E3;
	predictors[predNum][3] = 0xF870;
	predictors[predNum][4] = 0xF9AD;
	predictors[predNum][5] = 0xFB31;
	predictors[predNum][6] = 0xFCB4;
	predictors[predNum][7] = 0xFE07;
	predictors[predNum][8] = 0x0C04;
	predictors[predNum][9] = 0x0D15;
	predictors[predNum][10] = 0x0C32;
	predictors[predNum][11] = 0x0A33;
	predictors[predNum][12] = 0x07C1;
	predictors[predNum][13] = 0x0551;
	predictors[predNum][14] = 0x032D;
	predictors[predNum][15] = 0x0179;

	vadpcmData = new unsigned char[numberSamplesIn];

	book.npredictors = 1;
	book.order = 2;
	book.predictors = predictors[predNum];
	entropySet[predNum] = encode(inPCMSamples, numberSamplesIn, vadpcmData, vadpcmOutputLength, &book);

	delete [] vadpcmData;
	predNum++;

	int numFinalPred = 4;
	npredictors = numFinalPred;
	norder = 2;

	selectionSortAscending(entropySet, predictors, numPredTests);

	

	signed short* finalPred = new signed short[numFinalPred * 0x10];
	for (int x = 0; x < numFinalPred; x++)
	{
		for (int y = 0; y < 0x10; y++)
		{
			finalPred[x*0x10+y] = predictors[x][y];
		}
	}

	for (int x = 0; x < numPredTests; x++)
	{
		delete [] predictors[x];
	}

	delete [] predictors;
	delete [] entropySet;

	/*FILE* inFile2 = fopen("C:\\temp\\predictorset.bin", "rb");
	fseek(inFile2, 0, SEEK_END);
	int size = ftell(inFile2);
	fseek(inFile2, 0, SEEK_SET);
	unsigned char* buffer2 = new unsigned char[size];
	fread(buffer2, 1, (size), inFile2);
	fclose(inFile2);

	float bestEntropy = 99999999999.0f;
	unsigned long bestPredOffset = 0;

	for (unsigned long x = 0; x < size; x += 0x20)
	{
		signed short* predictors = new signed short[0x10];
		for (int r = 0; r < 0x10; r++)
		{
			predictors[r] = (signed short)((buffer2[x+r*2+0] << 8) | buffer2[x+r*2+1]);
		}

		unsigned long vadpcmOutputLength;
		unsigned char* vadpcmData = new unsigned char[numberSamplesIn];

		ALADPCMBook book;
		book.npredictors = 1;
		book.order = 2;
		book.predictors = predictors;
		float entropy = encode(inPCMSamples, numberSamplesIn, vadpcmData, vadpcmOutputLength, &book);

		if (entropy < bestEntropy)
		{
			bestPredOffset = x;
			bestEntropy = entropy;
		}

		delete [] vadpcmData;
		delete [] predictors;
	}
	
	delete [] buffer2;

	signed short* finalPred = new signed short[0x10];
	for (int x = 0; x < 0x10; x++)
	{
		finalPred[x] = (signed short)((buffer2[bestPredOffset + x*2+0] << 8) | buffer2[bestPredOffset + x*2+1]);
	}*/

	return finalPred;
}

void CN64AIFCAudio::selectionSortAscending(float *array, signed short** arrayCopy, int length)
{
	int i,j,minat;
	float min;
	for(i=0;i<(length-1);i++)
	{
		minat=i;
		min=array[i];

		for(j=i+1;j<(length);j++) //select the min of the rest of array
		{
			if(min>array[j])   //ascending order for descending reverse
			{
				minat=j;  //the position of the min element 
				min=array[j];
			}
		}
		float temp=array[i] ;
		array[i]=array[minat];  //swap 
		array[minat]=temp;

		signed short tempArrayCopy[0x10];
		for (int r = 0; r < 0x10; r++)
		{
			tempArrayCopy[r] = arrayCopy[i][r];
			arrayCopy[i][r] = arrayCopy[minat][r];
			arrayCopy[minat][r] = tempArrayCopy[r];
		}
	}
}

int CN64AIFCAudio::determineBestEncodeIndex(signed short* pred1, signed short* pred2, signed short* lastSampleSet, signed short* inPCMSamples, float& bestFitIndex)
{
	signed short* tempSampleSet = new signed short[8];
	signed short* tmp = new signed short[8];

	int index = 0;
	bestFitIndex = 99999999999.0f;

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

float CN64AIFCAudio::encode(signed short* inPCMSamples, int numberSamplesIn, unsigned char* outVADPCM, unsigned long& lenOut, ALADPCMBook *book)
{
	float entropy = 0.0f;

	signed short* lastSampleSet = new signed short[8];
	for (int x = 0; x < 8; x++)
		lastSampleSet[x] = 0x0;

	signed short* tmp = new signed short[8];

	lenOut = 0;

	for (int y = 0; y < numberSamplesIn; y += 16)
	{
		float totalBestFitDelta = 0;

		signed short* pred1;
		signed short* pred2;

		int predictor = 0;
		int index = 0;

		pred1 = &book->predictors[predictor*0x10 + 0];
		pred2 = &book->predictors[predictor*0x10 + 8];

		index = determineBestEncodeIndex(pred1, pred2, lastSampleSet, &inPCMSamples[y], totalBestFitDelta);

		outVADPCM[lenOut++] = ((index << 4) | predictor);

		for (int r = 0; r < 2; r++)
		{
			signed short resultingValue[8];
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
						resultingValue[i] = newValue;
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

				entropy += bestFit;
			}

			for (int x = 0; x < 8; x++)
			{
				//lastSampleSet[x] = inPCMSamples[y+(r*8)+x];
				lastSampleSet[x] = resultingValue[x];
			}
		}
	}


	if ((numberSamplesIn % 16) != 0)
	{
		// just cut it off for now
	}

	delete [] lastSampleSet;
	delete [] tmp;

	return entropy;
}


void CN64AIFCAudio::InitializeKeyTable()
{
	for (int x = 0; x < 0xFF; x++)
	{
		CN64AIFCAudio::keyTable[x] = pow(2, (float)((60.0 - (float)x) / 12.0));
	}
}