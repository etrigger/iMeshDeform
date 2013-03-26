#include "subspace/gui.hh"
#include <string.h>

namespace subspace {
  Object::Object(TriangleMesh *pmesh) : mesh(pmesh){

    //compute bounding box
    pmesh->need_tstrips();
    pmesh->need_bsphere();
    size = 2*pmesh->bsphere.r;
    center = pmesh->bsphere.center;

    xf = XForm::identity();

    glNormalPointer(GL_FLOAT, 0, pmesh->normals_tpd);
    glVertexPointer(3, GL_FLOAT, 0, pmesh->vertices_tpd);    
    int vn = pmesh->numberofvertices;
    color_base = new GLubyte[4*vn];
    
    for( int i=0 ; i<vn ; ++i ) {
      if(pmesh->is_rigid[i]!=0) {
      color_base[4*i] = 128; color_base[4*i+1] = 128; color_base[4*i+2] = 128; color_base[4*i+3] = 192;
      }
      else {
      color_base[4*i] = 77; color_base[4*i+1] = 128; color_base[4*i+2] = 154; color_base[4*i+3] = 192;
      }
    }
  }

  Object::Object(TetrahedronMesh *pmesh) : mesh(pmesh) {
    //compute bounding box
    pmesh->surface.need_tstrips();
    pmesh->surface.need_bsphere();
    size = 2*pmesh->surface.bsphere.r;
    center = pmesh->surface.bsphere.center;

    xf = XForm::identity();
    
    glNormalPointer(GL_FLOAT, 0, pmesh->normals_tpd);
    glVertexPointer(3, GL_FLOAT, 0, pmesh->vertices_tpd);    
    int vn = pmesh->numberofvertices;
    color_base = new GLubyte[4*vn];
    
    for( int i=0 ; i<vn ; ++i ) {
      if(pmesh->is_rigid[i]!=0) {
      color_base[4*i] = 128; color_base[4*i+1] = 128; color_base[4*i+2] = 128; color_base[4*i+3] = 192;
      }
      else {
      color_base[4*i] = 77; color_base[4*i+1] = 128; color_base[4*i+2] = 154; color_base[4*i+3] = 192;
      }
    }
  }

  void draw_tstrips(Mesh2d *mesh) {
    static bool use_glArrayElement = false;
    static bool tested_renderer = false;
    if (!tested_renderer) {
      use_glArrayElement = !!strstr((const char *) glGetString(GL_RENDERER), "Intel");
      tested_renderer = true;
    }

    const int *t = &mesh->tstrips[0];
    const int *end = t + mesh->tstrips.size();
    if (use_glArrayElement) {
      while (likely(t < end)) {
	glBegin(GL_TRIANGLE_STRIP);
	int striplen = *t++;
	for (int i = 0; i < striplen; i++)
	  glArrayElement(*t++);
	glEnd();
      }
    } else {
      while (likely(t < end)) {
	int striplen = *t++;
	glDrawElements(GL_TRIANGLE_STRIP, striplen, GL_UNSIGNED_INT, t);
	t += striplen;
      }
    }
  }

  //  void draw_tstrips(const Mesh *themesh)
  void TriangleMesh::draw()
  {
    draw_tstrips(this);
  }

  void TetrahedronMesh::draw()
  {
    draw_tstrips(&surface);
  }

  void Object::draw() {

    glEnable(GL_COLOR_MATERIAL);
    glColor4f(0.3, 0.5, 0.6, 0.75);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);

    //glCullFace(GL_BACK);

    //glDepthMask(GL_FALSE);
    //glEnable(GL_BLEND);

    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
    glMultMatrixf(xf);	  
    //glDrawElements(GL_TRIANGLES, 3*mesh->faces.size(), GL_UNSIGNED_INT, &mesh->faces[0][0]);
    //draw_tstrips(mesh);
    mesh->draw();
    //glDisable(GL_BLEND);
    //glDepthMask(GL_TRUE);
    glDisable(GL_COLOR_MATERIAL);
  }

  void Object::back_draw(){
    glDisable(GL_LIGHTING);
    glMultMatrixf(xf);	  
    //glDrawElements(GL_TRIANGLES, 3*mesh->faces.size(), GL_UNSIGNED_INT, &mesh->faces[0][0]);
    //draw_tstrips(mesh);
    mesh->draw();
    glEnable(GL_LIGHTING);
  }
  
}

