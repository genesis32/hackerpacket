#ifndef __VEC3T__
#define __VEC3T__

#include <math.h>
#include "mathlib.h"

// i doubt i need anymore than this
typedef float vec2_t[2];

class vec3_t {
public:
    // constructors
    vec3_t() {v[0] = v[1] = v[2] = 0;}
    vec3_t(float px, float py, float pz) {v[0] = px; v[1] = py; v[2] = pz;}
    vec3_t(const vec3_t &pVec)
    {v[0] = pVec.v[0]; v[1] = pVec.v[1]; v[2] = pVec.v[2];}
    vec3_t(const float *pVec) {v[0] = pVec[0]; v[1] = pVec[1]; v[2] = pVec[2];}

    // assign / equality / greater / less / etc
    vec3_t operator=(const vec3_t &pVec)
    {return vec3_t(v[0] = pVec.v[0], v[1] = pVec.v[1], v[2] = pVec.v[2]);}
    vec3_t operator=(const float *ptr)
    {return vec3_t(v[0] = ptr[0], v[1] = ptr[1], v[2] = ptr[2]);}
    int operator==(const vec3_t &pVec)
    {return(v[0] == pVec.v[0] && v[1] == pVec.v[1] && v[2] == pVec.v[2]);}
    int operator==(const float *pVec)
    {return(v[0] == pVec[0] && v[1] == pVec[1] && v[2] == pVec[2]);}
    inline int operator!=(const vec3_t &pVec)
    {return !((*this) == pVec);}
    inline int operator!=(const float *pVec)
    {return !(pVec == (*this));}
    int operator<(const vec3_t vec)
    {return((v[0] < vec[0]) && (v[1] < vec[1]) && (v[2] < vec[2]));}
    int operator<=(const vec3_t vec)
    {return((v[0] <= vec[0]) && (v[1] <= vec[1]) && (v[2] <= vec[2]));}
    int operator>(const vec3_t vec)
    {return((v[0] > vec[0]) && (v[1] > vec[1]) && (v[2] > vec[2]));}
    int operator>=(const vec3_t vec)
    {return((v[0] >= vec[0]) && (v[1] >= vec[1]) && (v[2] >= vec[2]));}

    // indexing into the array, no bound checks
    const float &operator[](int ndx) const {return v[ndx];}
    float &operator[](int ndx)             {return v[ndx];}
    operator float*(void)                  {return v;}

    // + - / * operations
    vec3_t &operator+=(const vec3_t &pVec) {*this = *this + pVec; return *this;}
    vec3_t &operator-=(const vec3_t &pVec) {*this = *this - pVec; return *this;}
    vec3_t &operator*=(const vec3_t &pVec) {*this = *this * pVec; return *this;}
    vec3_t &operator*=(float val)          {*this = *this * val; return *this;}
    vec3_t &operator/=(const vec3_t &pVec) {*this = *this / pVec; return *this;}
    vec3_t &operator/=(float val)          {*this = *this / val; return *this;}

    vec3_t operator+(const vec3_t &pVec)
    {return vec3_t(v[0] + pVec.v[0], v[1] + pVec.v[1], v[2] + pVec.v[2]);}
    vec3_t operator-(const vec3_t &pVec)
    {return vec3_t(v[0] - pVec.v[0], v[1] - pVec.v[1], v[2] - pVec.v[2]);}
    vec3_t operator*(const vec3_t &pVec)
    {return vec3_t(v[0] * pVec.v[0], v[1] * pVec.v[1], v[2] * pVec.v[2]);}
    vec3_t operator*(float val)
    {return vec3_t(v[0] * val, v[1] * val, v[2] * val);}
    friend vec3_t operator*(float val, const vec3_t &v)
    {return vec3_t(v[0] * val, v[1] * val, v[2] * val);}
    vec3_t operator/(const vec3_t &pVec)  
    {return vec3_t(v[0] / pVec.v[0], v[1] / pVec.v[1], v[2] / pVec.v[2]);}
    vec3_t operator/(float val)
    {return vec3_t(v[0] / val, v[1] / val, v[2] / val);}
    vec3_t operator-(void) {return vec3_t(-v[0], -v[1], -v[2]);}

    // + - * / operations that don't return a vec3_t
    void Clear(void)                    {v[0] = v[1] = v[2] = 0;}
    void Set(float x, float y, float z) {v[0] = x; v[1] = y; v[2] = z;}
    void Set(vec3_t &p)                 {v[0] = p[0]; v[1] = p[1]; v[2] = p[2];}
    void Add(vec3_t &a, vec3_t &b)
    {v[0] = a.v[0] + b.v[0]; v[1] = a.v[1] + b.v[1]; v[2] = a.v[2] + b.v[2];}
    void Add(vec3_t &a)
    {v[0] += a.v[0]; v[1] += a.v[1]; v[2] += a.v[2];}
    void Subtract(vec3_t &a, vec3_t &b)
    {v[0] = a.v[0] - b.v[0]; v[1] = a.v[1] - b.v[1]; v[2] = a.v[2] - b.v[2];}
    void Subtract(vec3_t &a)
    {v[0] -= a.v[0]; v[1] -= a.v[1]; v[2] -= a.v[2];}
    void Multiply(vec3_t &a, vec3_t &b)
    {v[0] = a.v[0] * b.v[0]; v[1] = a.v[1] * b.v[1]; v[2] = a.v[2] * b.v[2];}
    void Multiply(vec3_t &a)
    {v[0] *= a.v[0]; v[1] *= a.v[1]; v[2] *= a.v[2];}
    void Divide(vec3_t &a, vec3_t &b)
    {v[0] = a.v[0] / b.v[0]; v[1] = a.v[1] / b.v[1]; v[2] = a.v[2] / b.v[2];}
    void Divide(vec3_t &a)
    {v[0] /= a.v[0]; v[1] /= a.v[1]; v[2] /= a.v[2];}
    void Scale(float val)
    {v[0] *= val; v[1] *= val; v[2] *= val;}
    void Fabs(vec3_t &src) {v[0] = (float) fabs(src.v[0]);
        v[1] = (float) fabs(src.v[1]); v[2] = (float) fabs(src.v[2]);}

	 float Dot(const vec3_t &pVec)
    {return v[0] * pVec.v[0] + v[1] * pVec.v[1] + v[2] * pVec.v[2];}
	 	 
    // misc garbage
	 void Normalize(void);
	 float Length(void);
	 void Cross(const vec3_t &p, const vec3_t &q);
	 float GetDistance(const vec3_t &dest);
	 
	 void RotateX(float amnt, vec3_t &dest);
	 void RotateY(float amnt, vec3_t &dest);
	 void RotateZ(float amnt, vec3_t &dest);
	 
	 float GetX(void) { return v[0]; }
	 float GetY(void) { return v[1]; }
	 float GetZ(void) { return v[2]; }
	 void ClampVec(float min, float max);		
	 
 	 float v[3];

};

inline void vec3_t::Cross(const vec3_t &p, const vec3_t &q){
	v[0] = (p.v[1] * q.v[2]) - (p.v[2] * q.v[1]);
	v[1] = (p.v[2] * q.v[0]) - (p.v[0] * q.v[2]);
	v[2] = (p.v[0] * q.v[1]) - (p.v[1] * q.v[0]);
}

inline void vec3_t::Normalize(void){
	float length, len = 0;
	
	length = Length();
	
	if ( length == 0 )
		return;
	
	len = 1.0f / length;
	
	v[0] *= len;
	v[1] *= len;
	v[2] *= len;
}

inline float vec3_t::Length(void){
	float length = (v[0] * v[0]) + (v[1] * v[1]) + (v[2] * v[2]);
	
	return(float) sqrt(length);
}



// should have some sort of wrap around, not important
inline void vec3_t::ClampVec(float min, float max){
	if ( v[0] > max || v[0] < min )
		v[0] = 0;
	
	if ( v[1] > max || v[1] < min )
		v[1] = 0;
	
	if ( v[2] > max || v[2] < min )
		v[2] = 0;
}

inline float vec3_t::GetDistance(const vec3_t &dest){
	float d1 = dest[0] - v[0];
	float d2 = dest[1] - v[1];
	float d3 = dest[2] - v[2];
	
	return(float) sqrt((d1 * d1) + (d2 * d2) + (d3 * d3));
}

inline void vec3_t::RotateX(float amnt, vec3_t &dest){
	float s = my_sin(amnt);
	float c = my_cos(amnt);
	float y = v[1];
	float z = v[2];
	
	dest[0] = v[0];
	dest[1] = (y * c) - (z * s);
	dest[2] = (y * s) + (z * c);
}

inline void vec3_t::RotateY(float amnt, vec3_t &dest){
	float s = my_sin(amnt);
	float c = my_cos(amnt);
	float x = v[0];
	float z = v[2];
	
	dest[0] = (x * c) + (z * s);
	dest[1] = v[1];
	dest[2] = (z * c) - (x * s);
}

inline void vec3_t::RotateZ(float amnt, vec3_t &dest){
	float s = my_sin(amnt);
	float c = my_cos(amnt);
	float x = v[0];
	float y = v[1];
	
	dest[0] = (x * c) - (y * s);
	dest[1] = (y * c) + (x * s);
	dest[2] = v[2];
}

#endif 