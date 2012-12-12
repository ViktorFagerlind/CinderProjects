#include "SpriteSheet.h"

using namespace std;
using namespace ci;


void SpriteSheet::init(ci::gl::Texture spriteImage, std::string xmlPath, int DataFormat){
		m_spriteData = SpriteDataParser::parseSpriteData(xmlPath, DataFormat);
		m_spriteImage = spriteImage;
		m_currentFrame = 0;
		m_totalFrames = m_spriteData.size();
		m_textureWidth = spriteImage.getWidth();
		m_textureHeight = spriteImage.getHeight();

		x = 0;
		y = 0;
		scale = 1.0;
		rotation = 0.0;
		alpha = 1.0;
}

void SpriteSheet::bind()
{
}

void SpriteSheet::unbind()
{
}

void SpriteSheet::draw()
{
	SpriteData frame = m_spriteData[m_currentFrame];
	
	gl::pushMatrices();
	gl::translate(x,y,0);
	gl::rotate(rotation);
	gl::scale(scale,scale,1.0);

	float u = (float)frame.x;
	float v = frame.y;
   // v += frame.h;
    //v = frame.oH - v;
    float s = u + frame.w;
    float t = v + frame.h;

	bool textureRectangle = true;
	ci::Rectf rect(frame.oX,frame.oY,frame.oX+frame.w, frame.oY+frame.h);

	GLfloat verts[8];
	GLfloat texCoords[8];

	m_spriteImage.enableAndBind();

	//top right
	verts[0*2+0] = rect.getX2(); texCoords[0*2+0] = s/m_textureWidth;
	verts[0*2+1] = rect.getY1(); texCoords[0*2+1] = v/m_textureHeight;
	
	//top left
	verts[1*2+0] = rect.getX1(); texCoords[1*2+0] = u/m_textureWidth;
	verts[1*2+1] = rect.getY1(); texCoords[1*2+1] = v/m_textureHeight;
	
	//bottom right
	verts[2*2+0] = rect.getX2(); texCoords[2*2+0] = s/m_textureWidth;
	verts[2*2+1] = rect.getY2(); texCoords[2*2+1] = t/m_textureHeight;
	
	//bottom left
	verts[3*2+0] = rect.getX1(); texCoords[3*2+0] = u/m_textureWidth;
	verts[3*2+1] = rect.getY2(); texCoords[3*2+1] = t/m_textureHeight;

	glVertexPointer( 2, GL_FLOAT, 0, verts );
	glTexCoordPointer( 2, GL_FLOAT, 0, texCoords );

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );

	gl::color(cinder::ColorA(alpha,alpha,alpha,alpha));
	glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
	gl::color(cinder::ColorA(1.0,1.0,1.0,1.0));
	m_spriteImage.disable();
	m_spriteImage.unbind();


	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );	


	gl::popMatrices();

}

void SpriteSheet::update()
{
	m_currentFrame = (m_currentFrame +=1) % m_totalFrames;
}

void SpriteSheet::setFrame(int frame)
{
	m_currentFrame = frame % m_totalFrames;
}

int SpriteSheet::getCurrentFrame()
{
	return 0;
}