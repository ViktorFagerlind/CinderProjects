#include "ImageLibrary.h"

#include "cinder/ImageIo.h"

#include "SpriteDataParser.h"
#include "Macros.h"

using namespace ci;
using namespace ci::app;


//----------------------------------------------------------------------------------------------------------------------

gl::TextureRef ImageLibrary::getTexture (const string& name)
{
  return getOrAddItem (name).m_texture;
}

ImageSourceRef ImageLibrary::getImage   (const string& name)
{
  return getOrAddItem (name).m_image;
}

ImageLibraryItem& ImageLibrary::getOrAddItem (const string& name)
{
  map<string, ImageLibraryItem>::iterator it = m_items.find (name);

  if (it != m_items.end())
    return it->second;

  // Load image if not found in library
  ImageLibraryItem item;
  item.m_image    = loadImage (LOAD_IMAGE_FILE_OR_RESOURCE (name));
  item.m_texture  = gl::Texture::create(item.m_image);

  m_items[name]   = item;

  return m_items[name];
}

//----------------------------------------------------------------------------------------------------------------------

TextureAnimLibraryItem& TextureAnimLibrary::getOrAddItem (const string& textureName, const string& xmlName)
{
  map<string, TextureAnimLibraryItem>::iterator it = m_items.find (textureName);

  if (it != m_items.end())
    return it->second;

  // Load image if not found in library
  TextureAnimLibraryItem item;

  item.m_spriteData.reset ( 
    new vector<SpriteData> (SpriteDataParser::parseSpriteData (LOAD_MOVIE_FILE_OR_RESOURCE (xmlName),
                                                               SpriteSheet::FORMAT_TEXTUREPACKER_GENERIC_XML)));

  item.m_texture  = gl::Texture::create(loadImage (LOAD_MOVIE_FILE_OR_RESOURCE (textureName)));


  m_items[textureName] = item;

  return m_items[textureName];
}
