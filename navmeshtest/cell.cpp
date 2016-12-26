#include "stdafx.h"
#include "cell.h"
#include <math.h>
#include <assert.h>

cell::cell(const vector2 &A,const vector2 &B,const vector2 &C) :
triangle(A,B,C),
m_index(0),
m_sessionId(0),
m_f(0),
m_h(0),
m_g(0),
m_isopen(false),
m_parent(NULL),
m_arrivalwall(0)
{
	init();
}

cell::~cell()
{

}

//初始化三角形各边中点和边长度
void cell::init()
{
	m_link_index.resize(3,-1);
	m_wall_distance.resize(3);

	m_wall_midpoint.push_back(vector2((m_A.m_x + m_B.m_x) / 2.0, (m_A.m_y + m_B.m_y) / 2.0));
	m_wall_midpoint.push_back(vector2((m_C.m_x + m_B.m_x) / 2.0, (m_C.m_y + m_B.m_y) / 2.0));
	m_wall_midpoint.push_back(vector2((m_C.m_x + m_A.m_x) / 2.0, (m_C.m_y + m_A.m_y) / 2.0));

	vector2 vec;
	vec = m_wall_midpoint[0].subtract(m_wall_midpoint[1]);
	m_wall_distance[0] = vec.lenght();

	vec = m_wall_midpoint[1].subtract(m_wall_midpoint[2]);
	m_wall_distance[1] = vec.lenght();

	vec = m_wall_midpoint[2].subtract(m_wall_midpoint[0]);
	m_wall_distance[2] = vec.lenght();
}

//检测并且链接边
void cell::check_and_link(cell &other)
{
	if (getlink(SIDE_AB) == -1 && other.link(m_A, m_B, *this))
	{
		setlink(SIDE_AB, other);
	}
	else if (getlink(SIDE_BC) == -1 && other.link(m_B, m_C, *this))
	{
		setlink(SIDE_BC, other);
	}
	else if (getlink(SIDE_CA) == -1 && other.link(m_C, m_A, *this))
	{
		setlink(SIDE_CA, other);
	}
}

//获得某边另一个三角形索引
int cell::getlink(etriangle_side side)
{
	if(side < SIDE_AB || side > SIDE_CA)
	{
		return -1;
	}

	return m_link_index[side];
}

//设置边另一个三角形索引
void cell::setlink(etriangle_side side,cell& other)
{
	if(side < SIDE_AB || side > SIDE_CA)
	{
		assert(false);
		return ;
	}

	m_link_index[side] = other.getindex();
}

//计算h值
void cell::computeHeuristic(const vector2 &target)
{
	m_h = abs(target.m_x - m_center.m_x) + abs(target.m_y - m_center.m_y);
}

void cell::set_arrival_wall(int index)
{
	if (index == m_link_index[0])
	{
		m_arrivalwall = 0;
		return ;
	}
	else if (index == m_link_index[1])
	{
		m_arrivalwall = 1;
		return ;
	}
	else if (index == m_link_index[2])
	{
		m_arrivalwall = 2;
		return ;
	}
	assert(false);
	return ;
}

void cell::reset_cell()
{
	m_sessionId = 0;
	m_f = 0;
	m_h = 0;
	m_g = 0;
	m_isopen = false;
	m_parent = NULL;
	m_arrivalwall = 0;
}

//链接
bool cell::link(const vector2 &pA,const vector2 &pB,cell &other)
{
	if (m_A.equals(pA))
	{
		if (m_B.equals(pB))
		{
			m_link_index[SIDE_AB] = other.getindex();
			return true;
		}
		else if (m_C.equals(pB))
		{
			m_link_index[SIDE_CA] = other.getindex();
			return true;
		}
	}
	else if (m_B.equals(pA))
	{
		if (m_A.equals(pB))
		{
			m_link_index[SIDE_AB] = other.getindex();
			return true;
		}
		else if (m_C.equals(pB))
		{
			m_link_index[SIDE_BC] = other.getindex();
			return true;
		}
	}
	else if (m_C.equals(pA))
	{
		if (m_A.equals(pB))
		{
			m_link_index[SIDE_CA] = other.getindex();
			return true;
		}
		else if (m_B.equals(pB))
		{
			m_link_index[SIDE_BC] = other.getindex();
			return true;
		}
	}

	return false;
}
