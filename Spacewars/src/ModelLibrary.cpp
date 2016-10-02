#include "ModelLibrary.h"

#include "cinder/ObjLoader.h"
#include "cinder/gl/gl.h"

#include "Macros.h"
#include "ImageLibrary.h"

using namespace ci;
using namespace ci::app;


//----------------------------------------------------------------------------------------------------------------------

ModelElement::ModelElement (gl::Material          material, 
                            shared_ptr<TriMesh>   mesh, 
                            gl::Texture          *texture, 
                            const float           scale, 
                            const vec3&          relativePosition)
{
  m_material          = material;

  if (texture != NULL)
  {
    m_textured = true;
    m_texture  = *texture;
  }
  else
    m_textured = false;

  m_mesh              = mesh;
  m_scale             = scale;
  m_relativePosition  = relativePosition;
}

ModelElement::~ModelElement ()
{
}

void ModelElement::getMinMax (float& xMin, float& xMax, float& yMin, float& yMax)
{
  const vector<vec3>& verts = m_mesh->getVertices ();

  xMin = xMax = verts[0].x;
  yMin = yMax = verts[0].y;

  for (uint32_t i=1; i<verts.size (); i++)
  {
    const vec3 *v = &verts[i];

    if (v->x < xMin)
      xMin = v->x;
    else if (v->x > xMax)
      xMax = v->x;

    if (v->y < yMin)
      yMin = v->y;
    else if (v->y > yMax)
      yMax = v->y;
  }

  xMin *= m_scale;
  xMax *= m_scale;
  yMin *= m_scale;
  yMax *= m_scale;

  xMin += m_relativePosition.x;
  xMax += m_relativePosition.x;
  yMin += m_relativePosition.y;
  yMax += m_relativePosition.y;
}

void ModelElement::draw ()
{
  gl::pushModelView ();

  gl::translate (m_relativePosition);
  gl::scale (m_scale, m_scale, m_scale);

  m_material.apply ();

  if (m_textured)
    m_texture.bind ();

  gl::draw (*m_mesh.get ());

  if (m_textured)
    m_texture.unbind ();

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

void Model::getMinMax (float& xMin, float& xMax, float& yMin, float& yMax)
{
  float xMinTmp, xMaxTmp, yMinTmp, yMaxTmp;

  m_elements[0].getMinMax (xMin, xMax, yMin, yMax);

  for (uint32_t i=1; i<m_elements.size (); i++)
  {
    m_elements[i].getMinMax (xMinTmp, xMaxTmp, yMinTmp, yMaxTmp);

    if (xMinTmp < xMin)
      xMin = xMinTmp;
    if (xMaxTmp > xMax)
      xMax = xMaxTmp;

    if (yMinTmp < yMin)
      yMin = yMinTmp;
    if (yMaxTmp > yMax)
      yMax = yMaxTmp;
  }
}

void Model::draw (vec2 position, const vec3& rotation)
{
  gl::pushModelView ();

  gl::translate (position);
  gl::rotate (vec3 (0, 0, toDegrees (rotation.z)));
  gl::rotate (vec3 (0, toDegrees (rotation.y), 0));
  gl::rotate (vec3 (toDegrees (rotation.x), 0, 0));

  gl::enable  (GL_RESCALE_NORMAL);

  for (vector<ModelElement>::iterator it=m_elements.begin (); it != m_elements.end (); it++)
    it->draw ();

  gl::disable (GL_RESCALE_NORMAL);

  gl::popModelView ();
}

//----------------------------------------------------------------------------------------------------------------------

shared_ptr<TriMesh> ModelLibrary::getOrAddMesh (string name)
{
  map<string, shared_ptr<TriMesh>>::iterator it = m_meshes.find (name);

  if (it != m_meshes.end())
    return it->second;

  shared_ptr<TriMesh> mesh;

  ObjLoader loader (LOAD_MESH_FILE_OR_RESOURCE(name));

  mesh.reset (new TriMesh());
  loader.load (mesh.get ());

  m_meshes[name] = mesh;

  return mesh;
}

//----------------------------------------------------------------------------------------------------------------------

ModelLibrary::ModelLibrary ()
{
  shared_ptr<Model>   model;
  
  //gl::Texture bodyTexture = ImageLibrary::getSingleton ().getTexture ("basic particle 2.png");

  //---- create models ----------

  // space barrel
  {
    vector<ModelElement> elements;
    gl::Material         material (Color (.1f, .1f, .1f),    // Ambient
                                   Color (.5f, .6f, .6f),    // Diffuse
                                   Color::white(),           // Specular
                                   25.f,                     // Shininess
                                   Color (0.f, 0.f, 0.f),    // Emission
                                   GL_FRONT_AND_BACK);       // Face

    elements.push_back (ModelElement (material,
                                      getOrAddMesh ("space_barell_body_v08.obj"),
                                      NULL, //&bodyTexture,
                                      8.f));

    material.setDiffuse   (Color (.6f, .4f, .4f));
    elements.push_back (ModelElement (material,
                                      getOrAddMesh ("space_barell_engine_v08.obj"),
                                      NULL,
                                      8.f));

    material.setDiffuse   (Color (.3f, .3f, .3f));
    material.setShininess (40.f);
    elements.push_back (ModelElement (material,
                                      getOrAddMesh ("space_barell_windshield_v08.obj"),
                                      NULL,
                                      8.f));

    m_models["space_barrel"] = shared_ptr<Model> (new Model (elements));
  }

  // Enemy arrow
  {
    vector<ModelElement> elements;
    gl::Material         material (Color (.1f, .1f, .1f),    // Ambient 
                                   Color (.5f, .5f, .8f),    // Diffuse
                                   Color::white(),           // Specular
                                   5.f,                      // Shininess
                                   Color (0.f, 0.f, 0.f),    // Emission
                                   GL_FRONT_AND_BACK);       // Face

    elements.push_back (ModelElement (material,
                                      getOrAddMesh ("space_arrow_body.obj"),
                                      NULL,
                                      17.f));

    material.setDiffuse   (Color (.4f, .4f, .4f));
    elements.push_back (ModelElement (material,
                                      getOrAddMesh ("space_arrow_engine.obj"),
                                      NULL,
                                      17.f));

    material.setDiffuse   (Color (.3f, .3f, .3f));
    material.setShininess (40.f);
    elements.push_back (ModelElement (material,
                                      getOrAddMesh ("space_arrow_window.obj"),
                                      NULL,
                                      17.f));

    m_models["enemy_arrow"] = shared_ptr<Model> (new Model (elements));
  }

  // Enemy bot
  {
    vector<ModelElement> elements;
    gl::Material         material (Color (.1f, .1f, .1f),    // Ambient 
                                   Color (.9f, .6f, .4f),    // Diffuse
                                   Color::white(),           // Specular
                                   15.f,                     // Shininess
                                   Color (0.f, 0.f, 0.f),    // Emission
                                   GL_FRONT_AND_BACK);       // Face

    elements.push_back (ModelElement (material,
                                      getOrAddMesh ("space_bot_body.obj"),
                                      NULL,
                                      8.f));

    elements.push_back (ModelElement (material,
                                      getOrAddMesh ("space_bot_wings.obj"),
                                      NULL,
                                      8.f));

    material.setDiffuse   (Color (.3f, .3f, .3f));
    elements.push_back (ModelElement (material,
                                      getOrAddMesh ("space_bot_flanges.obj"),
                                      NULL,
                                      8.f));

    material.setShininess (40.f);
    elements.push_back (ModelElement (material,
                                      getOrAddMesh ("space_bot_lens.obj"),
                                      NULL,
                                      8.f));


    m_models["enemy_bot"] = shared_ptr<Model> (new Model (elements));
  }
}

ModelLibrary::~ModelLibrary ()
{
}

//----------------------------------------------------------------------------------------------------------------------

