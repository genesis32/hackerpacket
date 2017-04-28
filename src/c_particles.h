#ifndef C_PARTICLES_H
#define C_PARTICLES_H

#include "stdafx.h"
#include "c_vector.h"
#include "c_texture.h"

enum PSYS_TYPE { PERSISTANT, EVOLULATIONARY };

class C_Particles {
public:
	class particle_t {
  public:
    particle_t() : visible(false), radius(1.0), next(NULL), speed(1.0) { }
    float  col[4];  // current color of the particle
		vec3_t pos, velocity;
		float  radius;
    float  speed;
    particle_t *next;
 		bool   visible;
	};
  C_Particles() { };
  C_Particles(int np);
  ~C_Particles();
	
  // render the current system
  void Render();

  bool AddParticle(float[], float[], float[], float);
  // number of prebuilt particles to put into the dead list.
  void InitSystem(int);
protected:
	PSYS_TYPE   type;
	float a[16];
  particle_t *alive_list;
  particle_t *dead_list;
};

#endif