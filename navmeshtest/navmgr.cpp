#include "stdafx.h"
#include "navmgr.h"
#include <math.h>
#include "vector2.h"
#include "triangle.h"
#include "polygon.h"
#include "delaunay.h"
#include "cell.h"
#include "navmesh.h"


//构造函数
navmgr::navmgr():
m_out_polygon(NULL)
{

}

//析构函数
navmgr::~navmgr()
{
	clear();
	if(m_out_polygon != NULL)
	{
		delete m_out_polygon;
		m_out_polygon = NULL;
	}
}

//加入一个点
void navmgr::add_point(int x,int y)
{
	vector2 new_point((float)x,(float)y);
	//生成一个多边形
	if(m_lines.size() > 0 && distance_squared(m_lines[0],new_point) <= 10.0f)
	{
		if(m_lines.size() >= 3)
		{
			if(!is_counter_clock(m_lines))
			{
				reverse_vertexs(m_lines);
			}
			polygon *ploy = new polygon(m_lines);
			m_polygons.push_back(ploy);
		}

		m_lines.clear();
	}
	else
	{
		m_lines.push_back(new_point);
	}
}

//两点距离
float navmgr::distance_squared(const vector2 &src,const vector2 &target)
{
	return sqrt((src.m_x - target.m_x)*(src.m_x - target.m_x) + (src.m_y - target.m_y)*(src.m_y - target.m_y));
}

//构建网格
void navmgr::build_triangle()
{
	if(m_out_polygon != NULL)
	{
		m_polygons.insert(m_polygons.begin(),m_out_polygon);
	}
	m_lines.clear();
	delaunay d(m_polygons);
	if(!d.create_delaunay(m_triangles))
	{
		return;
	}

	for(int i = 0;i < m_triangles.size();++i)
	{
		const vector2 &cur_A = m_triangles[i]->get_vertex_by_index(0);
		const vector2 &cur_B = m_triangles[i]->get_vertex_by_index(1);
		const vector2 &cur_C = m_triangles[i]->get_vertex_by_index(2);

		cell *cur_cell = new cell(cur_A,cur_B,cur_C);
		cur_cell->setindex(i);
		cur_cell->set_flag(m_triangles[i]->get_flag());
		m_cells.push_back(cur_cell);
	}

	link_cells();

	if(m_out_polygon != NULL)
	{
		m_polygons.erase(m_polygons.begin());
	}
}

//链接所有的三角形
void navmgr::link_cells()
{
	for(int i = 0;i < m_cells.size();++i)
	{
		for(int j = 0;j < m_cells.size();++j)
		{
			if(i == j)
			{
				continue;
			}
			m_cells[i]->check_and_link(*m_cells[j]);
		}
	}
}

//清空所有数据
void navmgr::clear()
{
	for(int i = 0;i < m_polygons.size();++i)
	{
		delete m_polygons[i];
	}

	for(int i = 0;i < m_cells.size();++i)
	{
		delete m_cells[i];
	}

	for(int i = 0;i < m_triangles.size();++i)
	{
		delete m_triangles[i];
	}
	m_polygons.clear();
	m_cells.clear();
	m_triangles.clear();
	m_all_path.clear();
}

//寻路
void navmgr::find_path(vector2 start,vector2 end)
{
	navmesh nav(m_cells);
	m_all_path.push_back(vector<vector2>());
	nav.find_path(start,end,m_all_path.back());
	reset_cell();
}

void navmgr::init_out_polygon(int width,int height)
{
	if(m_out_polygon == NULL)
	{
		m_out_polygon = new polygon(vector<vector2>());
	}

	vector<vector2> vertexs;
	vertexs.push_back(vector2(0,0));
	vertexs.push_back(vector2(width,0));
	vertexs.push_back(vector2(width,height));
	vertexs.push_back(vector2(0,height));

	m_out_polygon->set_vertex(vertexs);
}

bool navmgr::is_counter_clock(vector<vector2> &vertexs)
{
	if (vertexs.size() < 3)
	{
		return false;
	}

	vector2 topPt = vertexs[0];
	int topID     = 0;
	for(int i = 0;i < vertexs.size();++i)
	{
		if (topPt.m_y > vertexs[i].m_y)
		{
			topPt = vertexs[i];
			topID = i;
		}
		else if (topPt.m_y == vertexs[i].m_y)
		{
			if (topPt.m_x > vertexs[i].m_x)
			{
				topPt = vertexs[i];
				topID = i;
			}
		}
	}

	//凸点的邻点
	int lastId = topID-1 >=0 ? topID-1 : vertexs.size() - 1;
	int nextId = topID+1 >= vertexs.size() ? 0 : topID + 1;
	vector2 last = vertexs[lastId];
	vector2 next = vertexs[nextId];

	//判断
	if (multiply(last, next, topPt) < 0)
	{
		return true;
	}

	return false;
}

/**
 * r=multiply(sp,ep,op),得到(sp-op)*(ep-op)的叉积
 * r>0:ep在矢量opsp的逆时针方向；
 * r=0：opspep三点共线；
 * r<0:ep在矢量opsp的顺时针方向
 */
float navmgr::multiply(const vector2 &sp,const vector2 &ep,const vector2 &op)
{
	return((sp.m_x-op.m_x)*(ep.m_y-op.m_y)-(ep.m_x-op.m_x)*(sp.m_y-op.m_y));
}

void navmgr::reverse_vertexs(vector<vector2> &vertexs)
{
	int start_index = 0;
	int end_index   = vertexs.size();
	end_index = end_index - 1;
	while(start_index < end_index)
	{
		vector2 start_ver = vertexs[start_index];
		vertexs[start_index] = vertexs[end_index];
		vertexs[end_index] = start_ver;
		++start_index;
		--end_index;
	}
}

void navmgr::reset_cell()
{
	for(int i = 0;i < m_cells.size();++i)
	{
		m_cells[i]->reset_cell();
	}
}