#include <time.h>
#include "stdafx.h"
#include "building.h"
#include "font.h"

extern time_t curr_time;

CBuilding::CBuilding(void) : width(15.0), height(0.1), max_height(40.0), x(0.0), z(0.0), rising(false), alive(false),
falling(false)
{
  contents = new C_Texture("blank.tga", false);
}

CBuilding::CBuilding(float w, float h) : width(w), height(h), max_height(50.0), x(0.0), z(0.0), rising(false), alive(false),
falling(false)
{
}

CBuilding::~CBuilding(void)
{
  delete contents;
}

void CBuilding::SetMaxHeight(float mh)
{
  max_height = mh;
}

bool CBuilding::AtMaxHeight()
{
  if(height >= max_height)
  {
    rising = false;
    return true;
  }
  return false;
}

void CBuilding::SetWidth(float w)
{
  width = w;
}

void CBuilding::SetPos(float dx, float dz)
{
  x = dx;
  z = dz;
}

void CBuilding::IncreaseHeight(float amt)
{
  if(height >= max_height)
    rising = false;
  else  
    height += amt;
}

void CBuilding::UpdateContents(char *buff, int bufflen)
{
  unsigned char pixels[128*128*3];
  char disp[128];

  int len = 6;
  int i=0, ypos = 120;

  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);

  SetFontSize(8);
  SetFontColor(1.0, 1.0, 1.0);
  DrawFontString(5, ypos, host);
  ypos -= 10.0;

  if(bufflen > 128)
    bufflen = 128;

  while(i+len < bufflen)
  {
    sprintf(disp, "%02x %02x %02x %02x %02x %02x", 
      buff[i], 
      buff[i+1],
      buff[i+2],
      buff[i+3],
      buff[i+4],
      buff[i+5]);

    SetFontColor(0.0, 1.0, 1.0);
    DrawFontString(13, ypos, disp);
    i += len;
    ypos -= 10;
  }
  glReadPixels(0, 0, 128, 128, GL_RGB, GL_UNSIGNED_BYTE, pixels);
  contents->Use();
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 128, 128, GL_RGB, GL_UNSIGNED_BYTE, pixels);
  glClearColor(0.0, 0.0, 0.0, 0.0);
}

void CBuilding::DecreaseHeight(float amt)
{
  if(height <= 0.1f)
  {
    alive = false;
    falling = false;
    strcpy(host, "");
  }
  else
    height -= amt;
}

void CBuilding::AddHost(const char *ip, unsigned long time)
{
  strcpy(host, ip);
  alive = true;
  time_touched = time;
  Raise();
}

void CBuilding::Render()
{
  if(time_touched <= curr_time-10)
  {
    rising = false;
    falling = true;
  }
  if(rising)
    IncreaseHeight(0.25);
  else if(falling)
    DecreaseHeight(0.25);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glTranslatef(x, 0.0, z);

  glDepthMask(GL_FALSE);

  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE);

  glEnable(GL_TEXTURE_2D);
  contents->Modulate(true);
  contents->Use();

  glColor4f(1.0, 1.0, 1.0, 1.0);

  glBegin(GL_QUADS);

  // front face
  glTexCoord2d(1.0, 0.0);
  glVertex3f(0.0, 0.0, 0.0);
  glTexCoord2d(1.0, 1.0);
  glVertex3f(0.0, height, 0.0);
  glTexCoord2d(0.0, 1.0);
  glVertex3f(width, height, 0.0);
  glTexCoord2d(0.0, 0.0);
  glVertex3f(width, 0.0, 0.0);

  // top
  glVertex3f(0.0, height, 0.0);
  glVertex3f(0.0, height, -width);
  glVertex3f(width, height, -width);
  glVertex3f(width, height, 0.0);

  // left side
  glTexCoord2d(0.0, 0.0);
  glVertex3f(0.0, 0.0, 0.0);
  glTexCoord2d(0.0, 1.0);
  glVertex3f(0.0, height, 0.0);
  glTexCoord2d(1.0, 1.0);
  glVertex3f(0.0, height, -width);
  glTexCoord2d(1.0, 0.0);
  glVertex3f(0.0, 0.0, -width);

  // right side
  glTexCoord2d(0.0, 0.0);
  glVertex3f(width, 0.0, 0.0);
  glTexCoord2d(0.0, 1.0);
  glVertex3f(width, height, 0.0);
  glTexCoord2d(1.0, 1.0);
  glVertex3f(width, height, -width);
  glTexCoord2d(1.0, 0.0);
  glVertex3f(width, 0.0, -width);

  // back side
  glTexCoord2d(0.0, 0.0);
  glVertex3f(width, 0.0, -width);
  glTexCoord2d(0.0, 1.0);
  glVertex3f(width, height, -width);
  glTexCoord2d(1.0, 1.0);
  glVertex3f(0.0, height, -width);
  glTexCoord2d(1.0, 0.0);
  glVertex3f(0.0, 0.0, -width);

  glEnd();
  glPopMatrix();
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_BLEND);
  glDepthMask(GL_TRUE);
}
