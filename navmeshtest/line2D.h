#ifndef __line2DH__
#define __line2DH__

#include "vector2.h"

//线段和点关系
enum eline_vetex_type
{
	eon_line  = 1,
	eon_left  = 2,
	eon_right = 3,
};

//线段和线段的关系
enum eline_line_type
{
	COLLINEAR  			= 1,
	PARALELL 			= 2,
	SEGMENTS_INTERSECT  = 3,
	B_BISECTS_A 		= 4,
	LINES_INTERSECT 	= 5,
	A_BISECTS_B 		= 6,
};

class line2D
{
public:
	line2D(const vector2 &A,const vector2 &B);

	~line2D();

public:
	//向量化
	void normal();

	const vector2& getA()const{return m_A;}

	const vector2& getB()const{return m_B;}

	void setA(vector2 pos);

	void setB(vector2 pos);

	eline_vetex_type line_vetex_type(const vector2 &p)const;

	eline_vetex_type line_vetex_type1(const vector2 &p) const;

	eline_line_type line_line_type(const line2D &other,vector2& result);

	bool equals(const line2D &other) const;

private:
	vector2 m_A;
	vector2 m_B;
	vector2 m_N;
};

#endif