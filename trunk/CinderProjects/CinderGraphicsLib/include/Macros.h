#pragma once

#include "cinder/app/App.h"

#include <string>

#define NOF_ELEMENTS(ARRAY) ((uint32_t)(sizeof(ARRAY)/sizeof(ARRAY[0])))

#if defined (CINDER_COCOA_TOUCH)
#define LOAD_IMAGE_FILE_OR_RESOURCE(Filename)  loadResource (Filename)
#else
#define LOAD_IMAGE_FILE_OR_RESOURCE(Filename)  loadFile ("../Media/Images/" + std::string (Filename))
#endif

#if defined (CINDER_COCOA_TOUCH)
#define LOAD_MOVIE_FILE_OR_RESOURCE(Filename)  loadResource (Filename)
#else
#define LOAD_MOVIE_FILE_OR_RESOURCE(Filename)  loadFile ("../Media/Movies/" + std::string (Filename))
#endif

#if defined (CINDER_COCOA_TOUCH)
#define LOAD_MESH_FILE_OR_RESOURCE(Filename)  loadResource (Filename)
#else
#define LOAD_MESH_FILE_OR_RESOURCE(Filename)  loadFile ("../Media/Meshes/" + std::string (Filename))
#endif

#if defined (CINDER_COCOA_TOUCH)
#define LOAD_AUDIO_RESOURCE(Filename)  loadResource (Filename)
#else
#define LOAD_AUDIO_RESOURCE(Filename)  loadFile ("../Media/Audio/" + std::string (Filename))
#endif

