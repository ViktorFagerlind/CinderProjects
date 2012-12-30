#pragma once

#include "cinder/gl/GlslProg.h"
#include "cinder/DataSource.h"

using namespace cinder;

class ShaderHelper
{
public:
  static gl::GlslProg loadShader(const fs::path &vertexShaderPath, const fs::path &fragmentShaderPath)
  {
    gl::GlslProg shader;

    try 
    {
		  shader = gl::GlslProg (loadFile (vertexShaderPath), 
                             loadFile (fragmentShaderPath));
	  }	
    catch (gl::GlslProgCompileExc &exc) 
    {
		  std::cout << "Shader compile error:" << std::endl;
		  std::cout << exc.what();
	  }	
    catch (...) 
    {
		  std::cout << "Unable to load shader" << std::endl;
	  }

    return shader;
  }

};