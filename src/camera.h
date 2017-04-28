#pragma once

#include <OpenGL/gl.h>
#include "mathlib.h"

const int ROLL  = 1;
const int PITCH = 0;
const int YAW   = 2;

class CCamera
{
public:
	CCamera(void);
	~CCamera(void);

	void Load();
	void Unload();

	void Move(float);
	void Strafe(float);

  void RotateX(float amt) { rot_angles[YAW] += amt; 	
                            if(rot_angles[YAW] > 360)
	                          	rot_angles[YAW] = rot_angles[YAW] - 360;  
  }
  
  void RotateY(float amt) { rot_angles[PITCH] += amt; 
                            if(rot_angles[PITCH] > 360)
	                            rot_angles[PITCH] = rot_angles[PITCH] - 360;  
  }
  void RotateZ(float amt) { rot_angles[ROLL] += amt; 
                              if(rot_angles[ROLL] > 360)
		                            rot_angles[ROLL] = rot_angles[ROLL] - 360;   
  }
  float pos[3];
	float rot_angles[3];
};
