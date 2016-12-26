#ifndef __polygonH__
#define __polygonH__

#include <vector>
#include "vector2.h"

using namespace std;

class polygon
{
public:
	polygon(vector<vector2> &vertexs);

	~polygon();

public:
	const vector<vector2>& get_vertex()const {return m_vertexs;}

	void set_vertex(const vector<vector2> &vertexs){m_vertexs = vertexs;}

private:
	vector<vector2> m_vertexs;   	 	//点列表
	rectangle 		rect; 				//包围盒
};

#endif