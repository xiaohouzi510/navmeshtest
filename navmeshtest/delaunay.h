#ifndef __delaunayH__
#define __delaunayH__

#include <vector>
class polygon;
class triangle;
class vector2;
class line2D;
class circle;

using namespace std;

class delaunay
{
public:
	delaunay(vector<polygon*> &polygon);

	~delaunay();

public:
	//生成三角剖
	bool create_delaunay(vector<triangle*> &result);

private:
	//初始化函数
	void init();

	//一个多边形所有边和点
	void put_vertex_edge(const vector<vector2> &vertexs);

	//获取外边界
	line2D* get_init_outedge();

	//找到DT点
	bool findDT(const line2D &line,vector2 &result);

	//点是否为直线的可见点
	bool isVisible_point_line(const line2D &line,const vector2 &p);

	//两个点是否为可见点
	bool isvisiblepoint(const vector2 &A,const vector2 &B);

	//外接圆
	void circumCircle(const vector2 &p1,const vector2 &p2,const vector2 &p3,circle &cir);

	/**
	* 返回顶角在o点，起始边为os，终止边为oe的夹角, 即∠soe (单位：弧度)
	* 角度小于pi，返回正值;   角度大于pi，返回负值
	*/
	float line_angle(const vector2 &s, const vector2 &o, const vector2 &e);

	//边是否在集合中
	int constraint_edge(const line2D &line,const vector<line2D*> &edges);

	void clear();

private:
	vector<polygon*>    &m_polygon; 		//所有多边形
	vector<vector2> 	m_vertexs; 			//所有顶点
	vector<line2D*>     m_edges; 			//所有边
};

#endif