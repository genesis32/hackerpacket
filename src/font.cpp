#include "stdafx.h"
#include "font.h"
#include "c_texture.h"

C_Texture *f_texture = NULL;
GLfloat tPoints[257][2]; // font map
GLfloat tIncX = 0.0f;    // used for texture coords, x axis amount to move
GLfloat tIncY = 0.0f;    // used for texture coords, y axis amount to move
int     region = 0, font_size = 12;
int     region_dims[] = { 0, 0, 0, 0 };
C_FontCoord coord;
GLfloat font_color[] = { 1.0, 1.0, 1.0 }; // default is white

void LoadFontTexture(const char *fn, int row, int col){
  f_texture = new C_Texture(fn, false);
  if(!f_texture){
    printf("FATAL ERROR: Unable to load font texture %s\n", fn);
    return;
  }
  f_texture->SetWrapS(GL_CLAMP);
  f_texture->SetWrapT(GL_CLAMP);

  f_texture->Upload(); // glTexImage2D;

  if ((row && col) && (row * col) == 256)
    MakeFontMap(row, col);
  else
    MakeFontMap(FONT_BLOCK_ROW, FONT_BLOCK_COL);
}

void MakeFontMap(GLint row, GLint col){
  GLint i = 0;
  GLfloat x, y;

  tIncX = (GLfloat)pow(col, -1);
  tIncY = (GLfloat)pow(row, -1);

  for (y = 1 - tIncY; y >= 0; y -= tIncY){
    for (x = 0; x <= 1 - tIncX; x += tIncX, i++){
      tPoints[i][0] = x;
      tPoints[i][1] = y;
    }
  }
}

void DrawChar(GLint x, GLint y, char c){
  GLint ci = c;
  
  short x_maxc = x + (font_size * coord.right);
  short y_maxc = y + (font_size * coord.up);

  float x_maxt = tPoints[ci][0] + tIncX;
  float y_maxt = tPoints[ci][1] + tIncY;

  glBegin(GL_TRIANGLE_FAN);
  glTexCoord2f(tPoints[ci][0], tPoints[ci][1]);
  glVertex2s(x, y);

  glTexCoord2f(x_maxt, tPoints[ci][1]);
  glVertex2s(x_maxc, y);

  glTexCoord2f(x_maxt, y_maxt);
  glVertex2s(x_maxc, y_maxc);

  glTexCoord2f(tPoints[ci][0], y_maxt);
  glVertex2s(x, y_maxc);
  glEnd();
}

void RenderString(GLint xpos, GLint ypos, char *buffer){
  GLint px = region ? region_dims[0] : xpos;
  GLint py = region ? region_dims[1] - (coord.up * font_size) : ypos;

  for (char *where = buffer; *where; where++){
    if (*where == '\t'){
      px += (coord.right * font_size)*5; // five spaces
    }
    else if (*where == '\n'){
      py -= (coord.up * font_size);
      px = region ? region_dims[0] : xpos;
    }
    else if (*where == '\\'){

    }
    else{
      DrawChar(px, py, *where);
      px += (coord.right * font_size);
    }
  }
}

void DrawFontString(GLint xpos, GLint ypos, char *msg, ...){
  va_list argptr;
  char buf[32768];	
  va_start(argptr, msg);
  vsprintf(buf, msg, argptr);
  va_end(argptr);

  //GL_TextureEnable(TMU0, true);
  //GL_DepthBufferEnable(false);

  //GL_BlendEnable(true);

  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);

  glDisable(GL_DEPTH_TEST);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  // the string is now contained in buf
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();

  GLint vPort[4];
  glGetIntegerv(GL_VIEWPORT, vPort);
  glOrtho(vPort[0], vPort[0] + vPort[2], vPort[1], vPort[1] + vPort[3], -1, 1);

  if(f_texture){
    f_texture->Modulate(true);
    f_texture->Use();
  }

  glColor3fv(font_color);	
  RenderString(xpos, ypos, buf);

  glMatrixMode(GL_PROJECTION);
  glPopMatrix();   
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();

  glDisable(GL_BLEND);
  glDisable(GL_TEXTURE_2D);

  glEnable(GL_DEPTH_TEST);

  //GL_TextureEnable(TMU0, false);
  //GL_DepthBufferEnable(true);
}

void SetFontSize(unsigned int s){
  font_size = s;
}

void SetFontColor(float r, float g, float b){
  font_color[0] = r;
  font_color[1] = g;
  font_color[2] = b;
}