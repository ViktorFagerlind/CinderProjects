#pragma once

#include "PhongMaterial.h"

#include "cinder/Perlin.h"
#include "cinder/Vector.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Matrix44.h"
#include "cinder/Tween.h"
#include "cinder/gl/gl.h"


#include <vector>
#include <list>

using std::list;

using namespace ci;

//----------------------------------------------------------------------------------------------------------------------

class Flower
{
public:
  Flower (const float size, const vec3& through, const vec3& left, const vec3& position);

  void draw ();

private:
  static gl::VboMesh& getVboMesh ();

private: 
  Anim<float> m_size;

  Matrix44f m_orientation;

  vec3 m_position;
  vec3 m_through;
  vec3 m_left;
};

//----------------------------------------------------------------------------------------------------------------------

class Leaf
{
public:
  Leaf (const float size, const vec3& through, const vec3& left, const vec3& position);

  void draw ();

private:
  static gl::VboMesh& getVboMesh ();

private:
  Anim<float> m_size;

  Matrix44f m_orientation;

  vec3 m_position;
};
//----------------------------------------------------------------------------------------------------------------------

class BranchJoint
{
public:
  BranchJoint (const vec3& position, const vec3& normal, const float length)
  : m_position (position),
    m_normal (normal),
    m_length (length)
  {
  }

  vec3 getEndPosition ()  {return m_position + m_length * m_normal;}

public:
  vec3       m_normal;
  const float m_length;
  vec3       m_position;
};

//----------------------------------------------------------------------------------------------------------------------
class Branch;

class Branch
{
public:
  class Branch (const float               radius, 
                const vec3&              position, 
                const vec3&              previusNormal, 
                const vec3&              normal, 
                const uint32_t            fullLife,
                const float               jointLength);
  
  void update ();
	
	void draw (gl::GlslProg& shader);

	void drawLeaves ();

	void drawFlowers ();

private:
  void updateSelf ();
	
  void drawSelf (gl::GlslProg& shader);

private:
  const float                       m_radius;
  const float                       m_jointLength;

  float                             m_currentRadius;

  uint32_t                          m_life;
  const uint32_t                    m_fullLife;
  float                             m_jointLife;
  uint32_t                          m_currentJoint;

  vec3                             m_position;
  vec3                             m_normal;

  std::vector<BranchJoint>          m_joints;
  
  std::list<Branch>                 m_subBranches;
  
  std::vector<vec3>                m_drawPoints;
  std::vector<vec3>                m_normals;
  std::vector<float>                m_radie;

  std::list<Leaf>                   m_leaves;
  std::list<Flower>                 m_flowers;
};

//----------------------------------------------------------------------------------------------------------------------

class Tree
{
public:
  Tree (const vec3&        position, 
        const vec3&        direction);

  void update ();
	
	void draw ();

  static uint32_t getAnimationCounter () {return m_animationCounter;}

  static void incrementAnimationCounter () {m_animationCounter++;}

private:
  shared_ptr<PhongMaterial> m_branchMaterial;
  shared_ptr<PhongMaterial> m_leafMaterial;
  shared_ptr<PhongMaterial> m_flowerMaterial;

  Branch          m_branch;

  static uint32_t m_animationCounter;
};