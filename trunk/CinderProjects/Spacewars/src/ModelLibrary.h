#pragma once

#include "cinder/Vector.h"
#include "cinder/TriMesh.h"
#include "cinder/gl/Material.h"

#include <vector>
#include <map>

using namespace ci;
using namespace std;

//----------------------------------------------------------------------------------------------------------------------

class ModelElement
{
public:
  ModelElement (gl::Material material, shared_ptr<TriMesh> mesh, const float scale = 1.f, const Vec3f& relativePosition = Vec3f (0.f,0.f,0.f));

  virtual ~ModelElement ();

  void draw ();

private:
  gl::Material	      m_material;

  shared_ptr<TriMesh> m_mesh;

  float               m_scale;

  Vec3f               m_relativePosition;
};

//----------------------------------------------------------------------------------------------------------------------

class Model
{
public:
  Model (const vector<ModelElement>& elements);

  virtual ~Model ();

  void draw (Vec2f position, float zAngle, float yAngle);

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
  void addMesh (string name);

private:
  map<string, shared_ptr<TriMesh>> m_meshes;

  map<string, shared_ptr<Model>>   m_models;
};

//----------------------------------------------------------------------------------------------------------------------

