#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
#include <memory.h>
#endif

#include "stdafx.h"
#include "c_texture.h"

C_Texture::C_Texture(const char *filename, bool b_mipmap){
	// Targa data type -- uncompressed, RGBA image; element
	// 3 is always 2.
	strcpy(tex_filename, filename);

	FILE *iFile = 0;
	byte *buffer = 0;

	if (!filename){
		printf("C_Texture(): NULL filename\n");
		return;
	}

	// TODO: FIXME
	NSString *nsFilePath = [[NSString alloc ] initWithUTF8String: filename];
	const char *s = [[[NSBundle mainBundle] pathForResource:nsFilePath ofType: nil] cStringUsingEncoding: NSASCIIStringEncoding];

	if (!(iFile = fopen(s, "rb"))){
		printf("C_Texture(): could not open %s\n", filename);
		return;		
	}

	fseek(iFile, 0, SEEK_END);

	int file_size = ftell(iFile);

	fseek(iFile, 0, SEEK_SET);

	if (!(buffer = new byte[file_size + 1]))	{
		fclose(iFile);
		return;
	}

	fread(buffer, 1, file_size, iFile);
	fclose(iFile);


	int ret = ParseImage(buffer);
	delete [] buffer;

	if(ret == -1){
		printf("ERROR: ParseImage() failed on %s\n", filename);
		return;
	}

	if(bpp == 24)
		image_format = int_format = GL_RGB;

	else if(bpp == 32)
		image_format = int_format = GL_RGBA;

	else if(bpp == 8)
		image_format = int_format = GL_ALPHA;

	mipmap_enabled = b_mipmap;
	if(mipmap_enabled) {
		SetMinFilter(GL_LINEAR_MIPMAP_LINEAR);
		SetMagFilter(GL_LINEAR_MIPMAP_LINEAR);		
	} else {
		SetMinFilter(GL_LINEAR);
		SetMagFilter(GL_LINEAR);
	}
	SetWrapS(GL_REPEAT);
	SetWrapT(GL_REPEAT);
	Modulate(true);

	glGenTextures(1, &tex_id); // generate our textures id
	Upload();
}

C_Texture::~C_Texture(){
	Delete();
	//	free(image_data);
	delete[] image_data;
}

void C_Texture::SetMinFilter(GLint rep){
	minFilter = rep;
}

void C_Texture::SetMagFilter(GLint rep){
	magFilter = rep;
}

bool C_Texture::IsMipMapped(){
	return mipmap_enabled;
}

void C_Texture::SetWrapS(GLint rep){
	wrapS = rep;
}

void C_Texture::SetWrapT(GLint rep){
	wrapT = rep;
}

void C_Texture::Modulate(bool mod){
	if(mod)
		texEnv = GL_MODULATE;
	else
		texEnv = GL_DECAL;
}

GLubyte *C_Texture::Get_PixelData(){
	return image_data;  
}

unsigned int C_Texture::Get_Width(){
	return width;
}

unsigned int C_Texture::Get_Height(){
	return height;
}

GLint C_Texture::Get_Format(){
	return image_format;
}

char *C_Texture::Get_Filename(){
	return tex_filename;
}

GLuint C_Texture::Get_ID(){
	return tex_id;
}

void C_Texture::Upload(){
	Use();
	if (mipmap_enabled)
		gluBuild2DMipmaps(GL_TEXTURE_2D, int_format, width, height, image_format,
		GL_UNSIGNED_BYTE, image_data);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, int_format, width, height, 0, image_format,
		GL_UNSIGNED_BYTE, image_data);
}

void C_Texture::SetTexState(){
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, texEnv);
}

void C_Texture::Use(){
	glBindTexture(GL_TEXTURE_2D, tex_id); 
	SetTexState();
}

void C_Texture::Delete(){
	glDeleteTextures(1, &tex_id);
}

int C_Texture::ParseImage(byte *buffer){
	bool flip_imagedata = false;

	if (!buffer)
		return -1;

	byte *where = buffer;
	tga_header_t *header = (tga_header_t *) where;

	where += (sizeof(tga_header_t) + header->numIden);

	if (header->colorMapType != 0)
		return -1;

	if (header->imageType != 2 && header->imageType != 3)
		return -1;

	if (header->imageDes & (1 << 5))
		flip_imagedata = true;

	width = header->width[0] + header->width[1] * 256; 
	height = header->height[0] + header->height[1] * 256; 
	bpp = header->bpp; 
	size = width * height; 

	// make sure we are loading a supported type 
	if (bpp != 32 && bpp != 24 && bpp != 8)
		return -1; 

	image_data = GrabData(where, size);

	// no image data 
	if (image_data == 0)
		return -1;

	if(flip_imagedata && FlipImageData())
		return 1;

	return 1;
}

// flip the image data from a top-down format to a bottom-up format
bool C_Texture::FlipImageData(void){
	byte *buf, *top_ptr, *bottom_ptr;
	unsigned int i, pixelSize, lineSize;

	switch(bpp) {
	case 24:
		pixelSize = 3;
		break;
	case 32:
		pixelSize = 4;
		break;
	default:
		return false;
	}

	lineSize = width * pixelSize;
	buf = new byte[lineSize];
	if(!buf)
		return false;

	top_ptr = image_data;
	bottom_ptr = image_data + (height - 1)*lineSize;
	for(i = 0; i < height/2 && top_ptr < bottom_ptr; i++) {
		// grab line from top side
		memcpy(buf,top_ptr,lineSize);
		// put line from bottom into top
		memcpy(top_ptr,bottom_ptr,lineSize);
		// put line from top into bottom
		memcpy(bottom_ptr,buf,lineSize);
		// move pointers
		top_ptr += lineSize;
		bottom_ptr -= lineSize;
	}

	delete [] buf;
	return true;
}	

byte *C_Texture::GetRGBA(byte *buff, int size){
	byte *rgba = new byte[size * 4];
	byte temp;
	int i;

	if (rgba == 0)
		return 0;

	if (!memcpy(rgba, buff, size * 4)){
		delete [] rgba;
		return 0;
	}

	for (i = 0; i < size * 4; i += 4){
		temp = rgba[i];
		rgba[i] = rgba[i + 2];
		rgba[i + 2] = temp;
	}

	return rgba;
}

byte *C_Texture::GetRGB(byte *buff, int size){
	byte *rgb = new byte[size * 3];
	byte temp;
	int i;

	if (rgb == 0)
		return 0;

	if (!memcpy(rgb, buff, size * 3)){
		delete [] rgb;
		return 0;
	}

	for (i = 0; i < size * 3; i += 3){
		temp = rgb[i];
		rgb[i] = rgb[i + 2];
		rgb[i + 2] = temp;
	}
	return rgb;
}

byte *C_Texture::GetGray(byte *buff, int size){
	byte *grayData = new byte[size];

	if (grayData == 0)
		return 0;

	if (!memcpy(grayData, buff, size)){
		delete [] grayData;
		return 0;
	}
	return grayData;
}

byte *C_Texture::GrabData(byte *buffer, int size){
	if (bpp == 32)
		return GetRGBA(buffer, size);
	else if (bpp == 24)
		return GetRGB(buffer, size);	
	else if (bpp == 8)
		return GetGray(buffer, size);

	return 0;
}