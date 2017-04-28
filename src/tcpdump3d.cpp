#include "StdAfx.h"
#include "tcpdump3d.h"
#include "c_vector.h"

TcpDump3D::TcpDump3D(void) : w(0), h(0)
{

}

TcpDump3D::~TcpDump3D(void)
{
  for(int i=0; i < w; i++)
    delete[] buildings[i];
 
  delete[] buildings;

  if(floor_tex)
    delete floor_tex;
}

bool TcpDump3D::HostExists(const char *ip, unsigned long time, char *buff, int bufflen)
{
  for(int i=0; i < w; i++)
    for(int j=0; j < h; j++)
      if(strcmp(ip, buildings[i][j].GetHost()) == 0)
      {
        buildings[i][j].UpdateTime(time);       
        buildings[i][j].UpdateContents(buff, bufflen);
        GenerateParticle(buildings[i][j]);
        return true;
      }
  return false;
}

bool TcpDump3D::AddHost(const char *ip, unsigned long time, char *buff, int bufflen)
{
  // shitty linear search here
  for(int i=0; i < w; i++)
		for(int j=0; j < h; j++)
		  if(!buildings[i][j].IsAlive() && strcmp(ip, buildings[i][j].GetHost()) != 0)
      {
        buildings[i][j].AddHost(ip, time);
        buildings[i][j].UpdateContents(buff, bufflen);     
        return true;
      }
  return false;
}

void TcpDump3D::GenerateParticle(CBuilding &building)
{
 float c[4], p[3], v[3], r;
 c[0] = 0.0;
 c[1] = RANDOM_NUM;
 c[2] = 1.0;
 c[3] = 1.0;

 p[0] = (building.x + (building.GetWidth()/2.0f));
 p[1] = RANDOM_NUM * 40.0f;
 p[2] = (building.z - (building.GetWidth()/2.0f));

 v[0] = 4.0f * RANDOM_NUM - 2.0f;
 v[1] = 0.0;
 v[2] = 4.0f * RANDOM_NUM - 2.0f;

 r = 2.0f;

 particles.AddParticle(c, p, v, r);
}

void TcpDump3D::InitGrid(unsigned int x, unsigned int y)
{
  w = x; h = y;

  FLOOR = glGenLists(1);
	glNewList(FLOOR, GL_COMPILE);
	glBegin(GL_TRIANGLE_STRIP);

	glTexCoord2d(0.0, 0.0);
	glVertex3f(-100.0, 0.0, 100.0);

	glTexCoord2d(0.0, 1.0);
	glVertex3f(-100.0, 0.0, -100.0);

	glTexCoord2d(1.0, 0.0);
	glVertex3f(100.0, 0.0,  100.0);

	glTexCoord2d(1.0, 1.0);
	glVertex3f(100.0, 0.0, -100.0);

	glEnd();
	glEndList();

  float incx = 200.0f / (float)w;
  float incy = 200.0f / (float)h;

  buildings = new CBuilding *[x];
  for(int i=0; i < x; i++)
    buildings[i] = new CBuilding[y];

	for(int i=0; i < x; i++)
	{
		for(int j=0; j < y; j++)
		{
			float xp = -100.0 + ((i * incx) + buildings[i][j].GetWidth() / 2.0f);
			float yp =  100.0 - ((j * incy) + buildings[i][j].GetWidth() / 2.0f);
			buildings[i][j].SetPos(xp, yp);
		}
	}

	particles.InitSystem(150);
 
	floor_tex = new C_Texture("pcb.tga", false);
	if(floor_tex)
		floor_tex->Modulate(false);
}

void TcpDump3D::Render(CCamera &cam)
{
	if(floor_tex)
		floor_tex->Use();

  glCallList(FLOOR);

  particles.Render();

	for(int i=0; i < w; i++)
		for(int j=0; j < h; j++)
	    if(buildings[i][j].IsAlive())
        buildings[i][j].Render();
}
