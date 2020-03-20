#include "stdafx.h"
#include "line2D.h"

line2D::line2D(const vector2 &A,const vector2 &B):
m_A(A),
m_B(B)
{
	normal();
}

line2D::~line2D()
{

}

//向量化
void line2D::normal()
{
	m_N.m_x = m_B.m_x - m_A.m_x;
	m_N.m_y = m_B.m_y - m_A.m_y;
	m_N.normalize();

	float y = m_N.m_y;
	m_N.m_y = m_N.m_x;
	m_N.m_x = -y;
}

void line2D::setA(vector2 pos)
{
	m_A = pos;
	normal();
}

void line2D::setB(vector2 pos)
{
	m_B = pos;
	normal();
}

//判断点p在直线a点朝向b点方向的位置
eline_vetex_type line2D::line_vetex_type(const vector2 &p) const
{
	vector2 normal = p.subtract(m_A);
	float value    = normal.dot(m_N);

	if(value > 0.0001f)
	{
		return eon_right;
	}
	else if(value < -0.0001f)
	{
		return eon_left;
	}

	return eon_line;
}

eline_vetex_type line2D::line_vetex_type1(const vector2 &p) const
{
	vector2 one = m_B.subtract(m_A);
	vector2 tow = p.subtract(m_A);
	float value = one.m_x*tow.m_y - tow.m_x*one.m_y;
	if(value > 0.0001f)
	{
		return eon_right;
	}
	else if(value < -0.0001f)
	{
		return eon_left;
	}

	return eon_line;
}

eline_line_type line2D::line_line_type(const line2D &other,vector2& result)
{
	const vector2 &otherA = other.getA();
	const vector2 &otherB = other.getB();

	float denom = (otherB.m_y-otherA.m_y)*(m_B.m_x-m_A.m_x)-(otherB.m_x-otherA.m_x)*(m_B.m_y-m_A.m_y);
	float u0 = (otherB.m_x-otherA.m_x)*(m_A.m_y-otherA.m_y)-(otherB.m_y-otherA.m_y)*(m_A.m_x-otherA.m_x);
	float u1 = (otherA.m_x-m_A.m_x)*(m_B.m_y-m_A.m_y)-(otherA.m_y-m_A.m_y)*(m_B.m_x-m_A.m_x);

	if(denom == 0.0f)
	{
		if(u0 == 0.0f && u1 == 0.0f)
			return COLLINEAR;
		else
			return PARALELL;
	}
	else
	{
		u0 = u0/denom;
		u1 = u1/denom;

		float x = m_A.m_x + u0*(m_B.m_x - m_A.m_x);
		float y = m_A.m_y + u0*(m_B.m_y - m_A.m_y);

		result.m_x = x;
		result.m_y = y;

		if (u0 >= 0.0f && u0 <= 1.0f && u1 >= 0.0f && u1 <= 1.0f)
		{
			return SEGMENTS_INTERSECT;
		}
		else if (u1 >= 0.0f && u1 <= 1.0f)
		{
			return A_BISECTS_B;
		}
		else if (u0 >= 0.0f && u0 <= 1.0f)
		{
			return B_BISECTS_A;
		}
	}

	return LINES_INTERSECT;
}

bool line2D::equals(const line2D &other) const
{
	return (m_A.equals(other.getA()) && m_B.equals(other.getB())) || (m_A.equals(other.getB()) && m_B.equals(other.getA()));
}