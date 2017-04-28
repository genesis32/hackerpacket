#include "stdafx.h";
#include "c_particles.h"
#include "c_texture.h"

C_Texture *tracer = NULL;

C_Particles::C_Particles(int np) : alive_list(NULL), dead_list(NULL)
{ 
  InitSystem(np);
}

C_Particles::~C_Particles()
{

}

void C_Particles::InitSystem(int nump)
{
  tracer = new C_Texture("particle.tga", false);
  for(int i=0; i < nump; i++)
  {
    particle_t *new_node = new particle_t;
    if(!dead_list)
      dead_list = new_node;
    else
    {
      new_node->next = dead_list;
      dead_list = new_node;
    }
  }
}

bool C_Particles::AddParticle(float c[], float p[], float v[], float r)
{
  if(dead_list)
  {
    particle_t *new_alive = dead_list;
    
    new_alive->col[0] = c[0];
    new_alive->col[1] = c[1];
    new_alive->col[2] = c[2];
    new_alive->col[3] = c[3];
    
    new_alive->pos[0] = p[0];
    new_alive->pos[1] = p[1];
    new_alive->pos[2] = p[2];

    new_alive->velocity[0] = v[0];
    new_alive->velocity[1] = v[1];
    new_alive->velocity[2] = v[2];

    new_alive->radius = r;
    new_alive->visible = true;
    new_alive->speed = 0.5f;

    dead_list = dead_list->next;
    if(!alive_list)
    {
      alive_list = new_alive;
      new_alive->next = NULL;
    }
    else
    {
      new_alive->next = alive_list;
      alive_list = new_alive;
    }
    return true;
  }
  return false;
}

void C_Particles::Render()
{
  vec3_t av, bv, cv, dv, x, y;


  tracer->Modulate(true);
  tracer->Use();
  // bind the texture
  glDisable(GL_DEPTH_TEST);

  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  glPushAttrib(GL_CURRENT_BIT);	
  glGetFloatv(GL_MODELVIEW_MATRIX, a);
  x.Set(a[0], a[4], a[8]);
  y.Set(a[1], a[5], a[9]);

  particle_t *prev = NULL;
  for(particle_t *trav = alive_list; trav; prev = trav, trav = trav->next)
  {	
    if(trav->pos[0] < -100.0 || trav->pos[0] > 100.0 || trav->pos[2] < -100.0 || trav->pos[2] > 100.0)
    {
      particle_t *tmp = trav->next;
      // if it's in the middle
      if(prev)
        prev->next = trav->next;
      else // if it's at the front of the list
        alive_list = alive_list->next;
      
      trav->next = dead_list;
      dead_list = trav;
      trav = tmp;
      if(!trav)
        break;
      // move it back to the dead list
    }
    
	  av = trav->pos + ((-x - y) * trav->radius);			
	  bv = trav->pos + ((x - y)  * trav->radius);
	  cv = trav->pos + ((x + y)  * trav->radius);
	  dv = trav->pos + ((-x + y) * trav->radius);

    trav->pos = trav->pos + (trav->speed * trav->velocity);

    for(int i=1; i <= 5; i++)
    {
    vec3_t new_pos = (0.4 * i * trav->velocity);      
    av -= new_pos;
    dv -= new_pos;

    glColor4fv(trav->col);		
    glBegin(GL_TRIANGLE_STRIP);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3fv(av.v);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3fv(bv.v);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3fv(dv.v);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3fv(cv.v);
    glEnd();
    }
  }

  glPopAttrib();
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);
  glDisable(GL_TEXTURE_2D);
}


