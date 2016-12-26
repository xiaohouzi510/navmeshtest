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
	eline     = 0, 	//画线状态
	efindpath = 1,  //寻路状态
};

//网格管理器
class navmgr
{
public:
	//构造函数
	navmgr();

	//析构函数
	~navmgr();

	//加入一个点
	void add_point(int x,int y);

	//两点距离
	float distance_squared(const vector2 &src,const vector2 &target);

	//构建网格
	void build_triangle();

	//链接所有的三角形
	void link_cells();

	//清空所有数据
	void clear();

	//寻路
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
	vector<vector2>   		m_lines;   	 	    //线段列表
	vector<polygon*>  		m_polygons;   		//多边形列表
	vector<cell*>     		m_cells;
	vector<triangle*> 		m_triangles;
	vector<vector<vector2>> m_all_path; 		//所有路径
	polygon* 				m_out_polygon;
};

#endif