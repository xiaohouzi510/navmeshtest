#include "stdafx.h"
#include "triangle.h"

triangle::triangle():
m_flag(0)
{

}

triangle::triangle(const vector2 &A,const vector2 &B,const vector2 &C) :
m_A(A),
m_B(B),
m_C(C),
m_flag(0)
{
	calculate_data();
}

triangle::~triangle()
{
	clear();
}

//获得三角形各项点
const vector2& triangle::get_vertex_by_index(int index)
{
	switch(index)
	{
	case 0:
		{
			return m_A;
		}
	case 1:
		{
			return m_B;
		}
	case 2:
		{
			return m_C;
		}
	}

	return m_A;
}

//检测某点是否在一个三角形中
bool triangle::is_vector2_in(const vector2 &pos)
{
	int count = 0;
	for(int i = 0;i < m_sides.size();++i)
	{
		if(m_sides[i]->line_vetex_type(pos) != eon_left)
		{
			++count;
		}
	}

	return count == 3;
}

void triangle::calculate_data()
{
	float x = m_A.m_x + m_B.m_x + m_C.m_x;
	float y = m_A.m_y + m_B.m_y + m_C.m_y;
	m_center.m_x = x / 3;
	m_center.m_y = y / 3;
	m_sides.clear();
	m_sides.resize(3);
	m_sides[SIDE_AB] = new line2D(m_A,m_B);
	m_sides[SIDE_BC] = new line2D(m_B,m_C);
	m_sides[SIDE_CA] = new line2D(m_C,m_A);
}

void triangle::clear()
{
	for(int i = 0;i < m_sides.size();++i)
	{
		delete m_sides[i];
	}
	m_sides.clear();
}