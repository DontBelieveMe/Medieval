#include "AudioArea.h"

AudioArea::AudioArea(float x, float y, int radius)
	: position(x, y), radius(radius)
{}

void AudioArea::addSource(Source *source)
{
	sources.push_back(source);
}