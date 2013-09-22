#include "ImageLibrary.h"

#include "cinder/ImageIo.h"

#include "Macros.h"

using namespace ci;
using namespace ci::app;


//----------------------------------------------------------------------------------------------------------------------

gl::Texture ImageLibrary::getTexture (const string& name)
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
  item.m_texture  = gl::Texture (item.m_image);

  m_items[name]   = item;

  return m_items[name];
}

