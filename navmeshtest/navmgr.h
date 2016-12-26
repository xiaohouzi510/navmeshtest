#ifndef __navmgrH__
#define __navmgrH__

#include <d3dx9.h>
#include <vector>
using namespace std;

class vector2;
class polygon;
class cell;
class triangle;

enum estage
{
	eline     = 0, 	//����״̬
	efindpath = 1,  //Ѱ·״̬
};

//���������
class navmgr
{
public:
	//���캯��
	navmgr();

	//��������
	~navmgr();

	//����һ����
	void add_point(int x,int y);

	//�������
	float distance_squared(const vector2 &src,const vector2 &target);

	//��������
	void build_triangle();

	//�������е�������
	void link_cells();

	//�����������
	void clear();

	//Ѱ·
	void find_path(vector2 start,vector2 end);

	vector<vector2>& get_lines(){return m_lines;}

	vector<polygon*> get_polygon(){return m_polygons;}

	vector<triangle*>& get_triangles(){return m_triangles;}

	vector<vector<vector2>>& get_all_path(){return m_all_path;}

	void init_out_polygon(int width,int height);

	bool is_counter_clock(vector<vector2> &vertexs);

	float multiply(const vector2 &sp,const vector2 &ep,const vector2 &op);

	void reverse_vertexs(vector<vector2> &vertexs);

	void reset_cell();

private:
	vector<vector2>   		m_lines;   	 	    //�߶��б�
	vector<polygon*>  		m_polygons;   		//������б�
	vector<cell*>     		m_cells;
	vector<triangle*> 		m_triangles;
	vector<vector<vector2>> m_all_path; 		//����·��
	polygon* 				m_out_polygon;
};

#endif