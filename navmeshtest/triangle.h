#ifndef __triangleH__
#define __triangleH__

#include "line2D.h"
#include <vector>

using namespace std;

enum etriangle_side
{
	SIDE_AB = 0,
	SIDE_BC = 1,
	SIDE_CA = 2,
};

class triangle
{
public:
	triangle();

	triangle(const vector2 &A,const vector2 &B,const vector2 &C);

	~triangle();

public:
	//获得三角形各项点
	const vector2& get_vertex_by_index(int index);

	//检测某点是否在一个三角形中
	bool is_vector2_in(const vector2 &pos);

	const line2D* get_side(int index){return m_sides[index];}

	const vector2& getA(){return m_A;}

	const vector2& getB(){return m_B;}

	const vector2& getC(){return m_C;}

	const vector2& get_center(){return m_center;}

	void set_flag(int flag){m_flag = flag;}

	int get_flag(){return m_flag;}

protected:
	void calculate_data();

	void clear();

protected:
	vector2 m_A;
	vector2 m_B;
	vector2 m_C;
	vector2 m_center;
	int m_flag;
	vector<line2D*> m_sides;
};

#endif