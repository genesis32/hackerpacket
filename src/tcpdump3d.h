#pragma once

#include <OpenGL/gl.h>
#include "building.h"
#include "c_texture.h"
#include "c_particles.h"
#include "camera.h"

class TcpDump3D
{
public:
  TcpDump3D(void);
  ~TcpDump3D(void);

  void InitGrid(unsigned int, unsigned int);
  bool AddHost(const char *, unsigned long, char *, int);
  void GenerateParticle(CBuilding &);
  bool HostExists(const char *, unsigned long, char *, int);
  void AdvanceTime(int);
  bool DeleteHost(const char *);
  void Render(CCamera &);
protected:
  GLuint FLOOR;
  CBuilding **buildings;
  unsigned int w, h;
  C_Texture *floor_tex;

  C_Particles particles;
};
