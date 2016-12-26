#ifndef __vector2H__
#define __vector2H__

#include <math.h>
class cell;

class vector2
{
public:
	vector2() : m_x(0.0f),m_y(0.0f){};

	vector2(float x,float y) : m_x(x),m_y(y){};

	~vector2(){};

	float getx()const {return m_x;}

	float gety()const {return m_y;}

	void setx(float x){m_x = x;}

	void sety(float y){m_y = y;}

	float lenght()const {return sqrt(length_squared());}

	float length_squared()const {return m_x*m_x + m_y*m_y;}

	float dot(const vector2 &other)const {return m_x*other.m_x + m_y*other.m_y;}

	void normalize(){float len = lenght(); m_x /=len; m_y /=len;}

	vector2 subtract(const vector2 &other) const{return vector2(m_x-other.m_x,m_y-other.m_y);}

	bool equals(const vector2 &other) const {return abs(m_x - other.m_x) <= 0.001f && abs(m_y - other.m_y) <= 0.001f;}

	float m_x;
	float m_y;
};

class circle
{
public:
	circle():m_r(0.0f){}
	~circle(){}

	vector2 m_center;
	float   m_r;
};

class rectangle
{
public:
	rectangle():m_leftx(0.0f),m_lefty(0.0f),m_rightx(0.0f),m_righty(0.0f){}
	rectangle(const circle& cir)
	{
		m_leftx   = cir.m_center.m_x - cir.m_r;
		m_lefty   = cir.m_center.m_y - cir.m_r;
		m_rightx  = cir.m_center.m_x + cir.m_r;
		m_righty  = cir.m_center.m_y + cir.m_r;
	}

	~rectangle(){}

	bool contains(float x,float y)
	{
		if(x >= m_leftx && x <= m_rightx && y >= m_lefty && y <= m_righty)
		{
			return true;
		}

		return false;
	}

public:
	float m_leftx;
	float m_lefty;
	float m_rightx;
	float m_righty;
};

class waypoint
{
public:
	vector2 m_position;
	cell*   m_cell;

	waypoint(cell *cll,vector2 pos)
	{
		m_position = pos;
		m_cell     = cll;
	}
};

#endif