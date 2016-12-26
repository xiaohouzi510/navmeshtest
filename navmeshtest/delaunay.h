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
	//����������
	bool create_delaunay(vector<triangle*> &result);

private:
	//��ʼ������
	void init();

	//һ����������бߺ͵�
	void put_vertex_edge(const vector<vector2> &vertexs);

	//��ȡ��߽�
	line2D* get_init_outedge();

	//�ҵ�DT��
	bool findDT(const line2D &line,vector2 &result);

	//���Ƿ�Ϊֱ�ߵĿɼ���
	bool isVisible_point_line(const line2D &line,const vector2 &p);

	//�������Ƿ�Ϊ�ɼ���
	bool isvisiblepoint(const vector2 &A,const vector2 &B);

	//���Բ
	void circumCircle(const vector2 &p1,const vector2 &p2,const vector2 &p3,circle &cir);

	/**
	* ���ض�����o�㣬��ʼ��Ϊos����ֹ��Ϊoe�ļн�, ����soe (��λ������)
	* �Ƕ�С��pi��������ֵ;   �Ƕȴ���pi�����ظ�ֵ
	*/
	float line_angle(const vector2 &s, const vector2 &o, const vector2 &e);

	//���Ƿ��ڼ�����
	int constraint_edge(const line2D &line,const vector<line2D*> &edges);

	void clear();

private:
	vector<polygon*>    &m_polygon; 		//���ж����
	vector<vector2> 	m_vertexs; 			//���ж���
	vector<line2D*>     m_edges; 			//���б�
};

#endif