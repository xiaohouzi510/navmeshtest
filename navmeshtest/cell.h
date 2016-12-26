#ifndef __cellH__
#define __cellH__

#include "triangle.h"
#include <vector>
using namespace std;

class cell : public triangle
{
public:
	cell(const vector2 &A,const vector2 &B,const vector2 &C);

	~cell();

public:
	//初始化三角形各边中点和边长度
	void init();

	//设置索引
	void setindex(int index){m_index = index;}

	//获取索引
	int getindex(){return m_index;}

	//检测并且链接边
	void check_and_link(cell &other);

	void set_session(int id){m_sessionId = id;}

	int get_session(){return m_sessionId;}

	//获得某边另一个三角形索引
	int getlink(etriangle_side side);

	//设置边另一个三角形索引
	void setlink(etriangle_side side,cell& other);

	//计算h值
	void computeHeuristic(const vector2 &target);

	//设置父结点
	void set_parent(cell *parent){m_parent = parent;}

	//获取父结点
	cell* get_parent(){return m_parent;}

	//设置是否为开放
	void set_open(bool open){m_isopen = open;}

	bool get_open(){return m_isopen;}

	float getH(){return m_h;}

	void setH(float value){m_h = value;setF(m_h + m_g);}

	float getF(){return m_f;}

	void setF(float value){m_f = value;}

	float getG(){return m_g;}

	void setG(float value){m_g = value;setF(m_h + m_g);}

	void set_arrival_wall(int index);

	float get_wall_distance(int index){return m_wall_distance[index];}

	int get_arrivalwall(){return m_arrivalwall;}

	void reset_cell();

protected:
	//链接
	bool link(const vector2 &pA,const vector2 &pB,cell &other);

private:
	int   m_index;
	int   m_sessionId;
	float m_f;
	float m_h;
	float m_g;
	bool  m_isopen;
	cell* m_parent;
	int   m_arrivalwall;
	vector<int>     m_link_index;
	vector<vector2> m_wall_midpoint;
	vector<float>   m_wall_distance;
};

#endif