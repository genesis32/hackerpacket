#pragma once

#include <OpenGL/gl.h>
#include "c_texture.h"

class CBuilding
{
public:
	CBuilding(void);
	CBuilding(float, float);
	~CBuilding(void);

	void SetMaxHeight(float);
	void SetPos(float, float);
	void SetWidth(float);
	void IncreaseHeight(float);
  void DecreaseHeight(float);
	bool AtMaxHeight();
	void Render();
  void AddHost(const char *, unsigned long);

 	int  GetWidth() { return width; }
  void Raise() { rising = true; }
  void UpdateTime(unsigned long time) { time_touched = time; }
  bool IsAlive() { return alive; }  
  char *GetHost() { return host; }

  void UpdateContents(char *, int);

	int x, z;
protected:
  C_Texture *contents;
  bool rising, alive, falling;
  char host[16];
	float width, height, max_height;
  unsigned long time_touched;
};
