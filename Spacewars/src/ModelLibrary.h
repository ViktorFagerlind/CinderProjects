#pragma once

#include "cinder/Vector.h"
#include "cinder/TriMesh.h"
#include "cinder/gl/Material.h"
#include "cinder/gl/Texture.h"

#include <vector>
#include <map>

using namespace ci;
using namespace std;

//----------------------------------------------------------------------------------------------------------------------

class ModelElement
{
public:
  ModelElement (gl::Material          material, 
                shared_ptr<TriMesh>   mesh, 
                gl::Texture          *texture           = NULL, 
                const float           scale             = 1.f, 
                const vec3&          relativePosition  = vec3 (0.f,0.f,0.f));

  virtual ~ModelElement ();

  void draw ();

  void getMinMax (float& xMin, float& xMax, float& yMin, float& yMax);

private:
  gl::Material	      m_material;

  shared_ptr<TriMesh> m_mesh;

  bool                m_textured;
  gl::Texture         m_texture;

  float               m_scale;

  vec3               m_relativePosition;
};

//----------------------------------------------------------------------------------------------------------------------

class Model
{
public:
  Model (const vector<ModelElement>& elements);

  virtual ~Model ();

  void getMinMax (float& xMin, float& xMax, float& yMin, float& yMax);

  void draw (vec2 position, const vec3& rotation);

private:
  vector<ModelElement> m_elements;
};

//----------------------------------------------------------------------------------------------------------------------

class ModelLibrary
{
private:
  ModelLibrary ();

public:
  virtual ~ModelLibrary ();

  shared_ptr<Model> getModel (string name) {return m_models[name];};

  static ModelLibrary& getSingleton ()
  {
      static ModelLibrary instance;

      return instance;
  }

private:
  shared_ptr<TriMesh> getOrAddMesh (string name);

private:
  map<string, shared_ptr<TriMesh>> m_meshes;

  map<string, shared_ptr<Model>>   m_models;
};

//----------------------------------------------------------------------------------------------------------------------

