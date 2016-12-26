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
	//��ʼ�������θ����е�ͱ߳���
	void init();

	//��������
	void setindex(int index){m_index = index;}

	//��ȡ����
	int getindex(){return m_index;}

	//��Ⲣ�����ӱ�
	void check_and_link(cell &other);

	void set_session(int id){m_sessionId = id;}

	int get_session(){return m_sessionId;}

	//���ĳ����һ������������
	int getlink(etriangle_side side);

	//���ñ���һ������������
	void setlink(etriangle_side side,cell& other);

	//����hֵ
	void computeHeuristic(const vector2 &target);

	//���ø����
	void set_parent(cell *parent){m_parent = parent;}

	//��ȡ�����
	cell* get_parent(){return m_parent;}

	//�����Ƿ�Ϊ����
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
	//����
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