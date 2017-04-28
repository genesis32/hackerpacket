#include "stdafx.h"
#include "camera.h"
#include <math.h>

CCamera::CCamera(void) 
{
	pos[0] = 120.0f; 
  pos[1] = 30.0f;
	pos[2] = 120.0f;

  rot_angles[0] = 0.0; 
  rot_angles[1] = 0.0;
  rot_angles[2] = -46.0f;
}

CCamera::~CCamera(void)
{
}

void CCamera::Load()
{
	glMatrixMode(GL_MODELVIEW);	
	glPushMatrix();
	glRotatef(rot_angles[ROLL],  0.0, 0.0, 1.0);
	glRotatef(rot_angles[PITCH], 1.0, 0.0, 0.0);
	glRotatef(rot_angles[YAW],   0.0, 1.0, 0.0);
	glTranslatef(-pos[0], -pos[1], -pos[2]);
}

void CCamera::Unload()
{
	glPopMatrix();
}

void CCamera::Move(float num_units)
{
	float x_offset = my_sin(rot_angles[YAW])*num_units;
	float z_offset = my_cos(rot_angles[YAW])*num_units;

  float y_offset = my_sin(rot_angles[PITCH])*num_units;

	pos[0] += x_offset;
  pos[1] -= y_offset;
  pos[2] -= z_offset;
}

void CCamera::Strafe(float num_units)
{
	float x_offset = my_sin(rot_angles[YAW]+90.0f)*num_units;
	float z_offset = my_cos(rot_angles[YAW]+90.0f)*num_units;

	pos[0] += x_offset;
	pos[2] -= z_offset;
}
