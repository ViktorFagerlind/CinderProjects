#pragma once

#include "cinder/gl/gl.h"

using namespace cinder;

class Misc
{
public:

  static bool CheckForOpenGlError ()
  {
    GLenum errorCode = glGetError ();

    bool isError = errorCode != GL_NO_ERROR;

    if (isError)
    {
      std::string errorString;
      switch (errorCode)
      {
        case GL_INVALID_ENUM:
          errorString = "GL_INVALID_ENUM";
          break;
        case GL_INVALID_VALUE:
          errorString = "GL_INVALID_VALUE";
          break;
        case GL_INVALID_OPERATION:
          errorString = "GL_INVALID_OPERATION";
          break;
        case GL_STACK_OVERFLOW:
          errorString = "GL_STACK_OVERFLOW";
          break;
        case GL_STACK_UNDERFLOW:
          errorString = "GL_STACK_UNDERFLOW";
          break;
        case GL_OUT_OF_MEMORY:
          errorString = "GL_OUT_OF_MEMORY";
          break;
      }

		  std::cout << "OpenGL error: " << errorString << std::endl;
    }

    return isError;
  }



};

