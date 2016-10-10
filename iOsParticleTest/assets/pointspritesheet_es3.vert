#version 300 es

uniform mat4	ciModelViewProjection;

uniform float uNofWidthImages;  // The number of images for each row
uniform float uNofHeightImages; // The number of images for each column
uniform float uTotalNofImages;  // The total number of images

in vec4			ciPosition;
in vec4			ciColor;
in float  	vfSize;
in float    vfLife;

out lowp vec4	Color;
out lowp vec2 TextureCoord;
out lowp vec2 ImageSize;

const float FullParticleLife = 100.0;

void main( void )
{
	gl_Position	 = ciModelViewProjection * ciPosition;
  gl_PointSize = vfSize;
  
  float imageNo  = floor (((FullParticleLife - vfLife) / FullParticleLife) * uTotalNofImages);
  float imageRow = floor (imageNo / uNofWidthImages);
  float imageCol = imageNo - imageRow * uNofWidthImages;
  imageRow = uNofHeightImages - imageRow - 1.0; // Invert row since starting from lower left corner
  
  TextureCoord = vec2 (imageCol/uNofWidthImages, imageRow/uNofHeightImages);
  ImageSize = vec2 (1.0f/uNofWidthImages, 1.0/uNofHeightImages);
  
  if (vfLife <= 0.0)
    Color = vec4(0);
  else
  	Color = ciColor;
}
