#pragma once

#include "AudioLibrary.h"
#include "Macros.h"

using namespace ci::app;

//----------------------------------------------------------------------------------------------------------------------

AudioLibrary::AudioLibrary ()
{
  audio::SourceRef source;
  
  source = audio::load (LOAD_AUDIO_RESOURCE ("SpaceGame.mp3"));
  sources.push_back (source);

  source = audio::load (LOAD_AUDIO_RESOURCE ("Lazer 01 lower.wav"));
  sources.push_back (source);

  source = audio::load (LOAD_AUDIO_RESOURCE ("Explosion 01 lower.wav"));
  sources.push_back (source);
}

AudioLibrary::~AudioLibrary ()
{
}

AudioLibrary& AudioLibrary::getSingleton ()
{
  static AudioLibrary instance;

  return instance;
}

void AudioLibrary::play (AudioClip clip)
{
//  if (clip == AudioClip_Lazer_E)
//    audio::Output::setVolume (0.03f);

  if (clip != AudioClip_NoSound_E)
	  audio::Output::play (sources[clip]);
}

//----------------------------------------------------------------------------------------------------------------------

