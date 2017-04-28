#ifndef FONT_H
#define FONT_H

#include <OpenGL/gl.h>

// Load the texture you want for the font. This font should be 8-bit
void LoadFontTexture(const char *filename, int row = 0, int col = 0);

void MakeFontMap(GLint row, GLint col);

//   Draw a character
void DrawChar(GLint, GLint, char);

//   Traverse a string and call draw character and compute deltas
void RenderString(GLint, GLint, char *);

//   Set the current font size for the system
void SetFontSize(unsigned int);

// set the font color for the system
void SetFontColor(float, float, float);

// Draw a string (similiar to printf)
extern "C" void DrawFontString(GLint xpos, GLint ypos, char *msg, ...);

const int FONT_BLOCK_COL = 16, FONT_BLOCK_ROW = 16;

struct C_FontCoord {
	C_FontCoord() {up = 1; right = 1; dim[0] = dim[1] = dim[2] = dim[3] = 0;}
	
	void Setup(GLint l, GLint r, GLint b, GLint t);
	
	GLint up;   
	GLint right;
	GLint dim[4];
};



#endif