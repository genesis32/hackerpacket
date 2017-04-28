#ifndef C_TEXTURE_H
#define C_TEXTURE_H

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

typedef unsigned char byte;

#define MAX_FILENAME 1024

class C_Texture {
public:
	struct tga_header_t{
		byte numIden;
		byte colorMapType;
		byte imageType;
		byte colorMapSpec[5]; // not used, just here to take up space
		byte origX[2];
		byte origY[2];
		byte width[2];
		byte height[2];
		byte bpp;
		byte imageDes; 
	};
	C_Texture() {}
	C_Texture(const char *filename, bool b_mipmap); // construct our texture object
	~C_Texture();
	void Use();         // make this texture part of the state
	void Delete();      // delete this texture

	void Upload();
	void SetTexState();

	// retrieve data from our texture
	GLubyte      *Get_PixelData();
	unsigned int Get_Width();
	unsigned int Get_Height();
	GLint        Get_Format();
	GLuint       Get_ID();
	char *       Get_Filename();


	void SetWrapS(GLint);
	void SetWrapT(GLint);
	void Modulate(bool);

	bool IsMipMapped();

	void SetMinFilter(GLint);
	void SetMagFilter(GLint);

	GLubyte      *image_data;      // raw image data
private:

	int  ParseImage(byte *);
	bool FlipImageData(void);
	byte *GrabData(byte *, int);
	byte *GetGray(byte *, int);
	byte *GetRGB(byte *, int);
	byte *GetRGBA(byte *, int);

	char         tex_filename[MAX_FILENAME];
	unsigned int         width, height, bpp, size;
	GLuint       tex_id;
	GLint        image_format; // either RGB or RGBA
	bool         mipmap_enabled; // mip mapping enabled
	GLint        int_format;

	GLint        wrapS, wrapT;
	GLint        minFilter, magFilter;
	GLint        texEnv;
};

#endif 