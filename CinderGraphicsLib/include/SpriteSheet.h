#pragma once

#include "cinder/Cinder.h"
#include "cinder/gl/Texture.h"
#include "cinder/Xml.h"
#include "cinder/Timeline.h"
#include <stdio.h>

#include "SpriteData.h"
#include "SpriteDataParser.h"

class SpriteSheet{
public:
	
	static const int FORMAT_TEXTUREPACKER_GENERIC_XML = 1;

	void draw();
	void update();
	void setFrame(int frameNum);
	int getCurrentFrame();
	void getTotalFrames();

	void bind();
	void unbind();

	ci::Anim<float> x;
	ci::Anim<float> y;
	ci::Anim<float> scale;
	ci::Anim<float> rotation;
	ci::Anim<float> alpha;


	void init(ci::gl::Texture spriteImage, std::string xmlPath, int DataFormat);

private:
	int m_currentFrame;
	int m_totalFrames;
	ci::gl::Texture m_spriteImage;
	std::vector<SpriteData> m_spriteData;
	int m_textureWidth;
	int m_textureHeight;

};
