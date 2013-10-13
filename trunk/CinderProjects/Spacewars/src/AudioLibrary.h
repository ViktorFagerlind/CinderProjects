#pragma once

#include "cinder/audio/Io.h"
#include "cinder/audio/Output.h"

#include <vector>

using namespace ci;
using namespace std;

//----------------------------------------------------------------------------------------------------------------------

enum AudioClip
{
  AudioClip_ThemeSong_E,
  AudioClip_Lazer_E,
  AudioClip_Explosion_E,
  AudioClip_NoSound_E
};

//----------------------------------------------------------------------------------------------------------------------

class AudioLibrary
{
private:
  AudioLibrary ();

public:
  virtual ~AudioLibrary ();

  static AudioLibrary& getSingleton ();

  void play (AudioClip clip);

private:
  vector<audio::SourceRef> sources;
};

//----------------------------------------------------------------------------------------------------------------------

