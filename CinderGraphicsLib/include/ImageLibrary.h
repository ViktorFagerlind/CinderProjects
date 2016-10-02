#pragma once

#include "cinder/gl/Texture.h"
#include "cinder/Surface.h"

#include "SpriteData.h"

#include <map>

using namespace ci;
using namespace std;

//----------------------------------------------------------------------------------------------------------------------

class ImageLibraryItem
{
public:
  gl::TextureRef  m_texture;
  ImageSourceRef  m_image;
};

//----------------------------------------------------------------------------------------------------------------------

class ImageLibrary
{
private:
  ImageLibrary () {};

public:
  static ImageLibrary& getSingleton ()
  {
      static ImageLibrary instance;

      return instance;
  }

  gl::TextureRef getTexture (const string& name);
  ImageSourceRef getImage   (const string& name);

private:
  ImageLibraryItem& getOrAddItem (const string& name);

private:
  map<string, ImageLibraryItem> m_items;
};

//----------------------------------------------------------------------------------------------------------------------

class TextureAnimLibraryItem
{
public:
  gl::TextureRef                 m_texture;
  shared_ptr<vector<SpriteData>> m_spriteData;
};

//----------------------------------------------------------------------------------------------------------------------

class TextureAnimLibrary
{
private:
  TextureAnimLibrary () {};

public:
  static TextureAnimLibrary& getSingleton ()
  {
      static TextureAnimLibrary instance;

      return instance;
  }

  TextureAnimLibraryItem& getOrAddItem (const string& textureName, const string& xmlName);

private:
  map<string, TextureAnimLibraryItem> m_items;
};

//----------------------------------------------------------------------------------------------------------------------

