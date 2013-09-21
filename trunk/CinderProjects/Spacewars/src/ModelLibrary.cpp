#include "ModelLibrary.h"

#include "cinder/ObjLoader.h"
#include "cinder/gl/gl.h"

#include "Macros.h"

using namespace ci;
using namespace ci::app;


//----------------------------------------------------------------------------------------------------------------------

ModelElement::ModelElement (gl::Material material, shared_ptr<TriMesh> mesh, const float scale, const Vec3f& relativePosition)
{
  m_material          = material;
  m_mesh              = mesh;
  m_scale             = scale;
  m_relativePosition  = relativePosition;
}

ModelElement::~ModelElement ()
{
}

void ModelElement::draw ()
{
  gl::pushModelView ();

  gl::translate (m_relativePosition);
  gl::scale (m_scale, m_scale, m_scale);

  m_material.apply ();

  gl::draw (*m_mesh.get ());

  gl::popModelView ();
}

//----------------------------------------------------------------------------------------------------------------------

Model::Model (const vector<ModelElement>& elements)
: m_elements (elements)
{
}

Model::~Model ()
{
}

void Model::draw (Vec2f position, float zAngle, float yAngle)
{
  gl::pushModelView ();

  gl::translate (position);
  gl::rotate (Vec3f (0, 0, toDegrees (zAngle)));

  gl::rotate (Vec3f (0, toDegrees (yAngle), 0));

  gl::enable  (GL_RESCALE_NORMAL);

  for (vector<ModelElement>::iterator it=m_elements.begin (); it != m_elements.end (); it++)
    it->draw ();

  gl::disable (GL_RESCALE_NORMAL);

  gl::popModelView ();
}

//----------------------------------------------------------------------------------------------------------------------

void ModelLibrary::addMesh (string name)
{
  shared_ptr<TriMesh> mesh;

  ObjLoader loader (LOAD_MESH_FILE_OR_RESOURCE(name));

  mesh.reset (new TriMesh());
  loader.load (mesh.get ());

  m_meshes[name] = mesh;
}

//----------------------------------------------------------------------------------------------------------------------

ModelLibrary::ModelLibrary ()
{
  shared_ptr<Model>   model;
  

  //---- load meshes ----------
  addMesh ("shuttle_export.obj");
  addMesh ("space_barell_windshield_v08.obj");
  addMesh ("space_barell_engine_v08.obj");
  addMesh ("space_barell_body_v08.obj");
  addMesh ("Rock.obj");

  //---- create models ----------

  // main_vessel
  {
    vector<ModelElement> elements;
    gl::Material         material (Color (.1f, .1f, .1f),    // Ambient 
                                   Color (.5f, .6f, .6f),    // Diffuse
                                   Color::white(),           // Specular
                                   25.f,                     // Shininess
                                   Color (0.f, 0.f, 0.f),    // Emission
                                   GL_FRONT_AND_BACK);       // Face

    elements.push_back (ModelElement (material,                         // material
                                      m_meshes["space_barell_body_v08.obj"],       // mesh
                                      8.f,                              // scale
                                      Vec3f (0.f, 0.f, 0.f)));          // relativePosition

    material.setDiffuse   (Color (.6f, .4f, .4f));
    elements.push_back (ModelElement (material,                         // material
                                      m_meshes["space_barell_engine_v08.obj"],       // mesh
                                      8.f,                              // scale
                                      Vec3f (0.f, 0.f, 0.f)));          // relativePosition

    material.setDiffuse   (Color (.3f, .3f, .3f));
    material.setShininess (40.f);
    elements.push_back (ModelElement (material,                         // material
                                      m_meshes["space_barell_windshield_v08.obj"],       // mesh
                                      8.f,                              // scale
                                      Vec3f (0.f, 0.f, 0.f)));          // relativePosition

    m_models["main_vessel"] = shared_ptr<Model> (new Model (elements));
  }

  // enemy_vessel
  {
    vector<ModelElement> elements;
    gl::Material         material (Color (.1f, .1f, .1f),    // Ambient 
                                   Color (.8f, .6f, .5f),    // Diffuse
                                   Color::white(),           // Specular
                                   5.f,                      // Shininess
                                   Color (0.f, 0.f, 0.f),    // Emission
                                   GL_FRONT_AND_BACK);       // Face

    elements.push_back (ModelElement (material,                       // material
                                      m_meshes["shuttle_export.obj"], // mesh
                                      .7f,                            // scale
                                      Vec3f (10.f, 23.f, 0.f)));      // relativePosition

    m_models["enemy_vessel"] = shared_ptr<Model> (new Model (elements));
  }
}

ModelLibrary::~ModelLibrary ()
{
}

//----------------------------------------------------------------------------------------------------------------------

