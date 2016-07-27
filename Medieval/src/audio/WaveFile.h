/*
* @author Barney Wilks
*/

#pragma once
#include <stdint.h>

#include "../includes.h"
#include "OpenAL/al.h"

typedef unsigned char byte;	// Move this somewhere else

namespace wave 
{
	namespace detail {
		struct RIFF
		{
			char    chunkId[4];
			int32_t chunkSize;
			char    format[4];
		};

		struct FMT
		{
			char subChunk1ID[4];
			int32_t subChunk1Size;
			int16_t audioFormat;
			int16_t numChannels;
			int32_t sampleRate;
			int32_t byteRate;
			int16_t blockAlign;
			int16_t bitsPerSample;
		};

		struct DATA
		{
			char subChunk2ID[4];
			int32_t subChunk2Size;
		};
	}

	class WaveFile
	{
	private:
		ALsizei size;
		ALint   frequency;
		ALenum  format;
		byte   *soundData;

		detail::DATA _data;
		detail::FMT  _format;
		detail::RIFF _riff;

	public:
		WaveFile(const std::string& path);
		~WaveFile();

		void destroy();

		inline ALsizei getSize() const { return size; }
		inline ALint   getFreq() const { return frequency; }
		inline ALenum  getFormat() const { return format; }

		inline byte   *data() const { return soundData; }
		
	};

}