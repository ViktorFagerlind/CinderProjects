#pragma once

#include "cinder/gl/Texture.h"
#include "cinder/Surface.h"

#include <map>

using namespace ci;
using namespace std;

//----------------------------------------------------------------------------------------------------------------------

class ImageLibraryItem
{
public:
  gl::Texture     m_texture;
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

  gl::Texture    getTexture (const string& name);
  ImageSourceRef getImage   (const string& name);

private:
  ImageLibraryItem& getOrAddItem (const string& name);


private:
  map<string, ImageLibraryItem> m_items;
};

//----------------------------------------------------------------------------------------------------------------------

