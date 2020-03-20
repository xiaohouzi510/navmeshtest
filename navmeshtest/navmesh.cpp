#include "stdafx.h"
#include "navmesh.h"
#include <algorithm>
#include "vector2.h"
#include "cell.h"

int navmesh::g_path_sessionId = 0;

navmesh::navmesh(vector<cell*> &cells) :
m_cells(cells)
{

}

navmesh::~navmesh()
{
	clear();
}

//寻路
bool navmesh::find_path(vector2 start,vector2 end,vector<vector2> &result)
{
	++navmesh::g_path_sessionId;
	cell *start_cell = find_cell_by_vector2(start);
	cell *end_cell   = find_cell_by_vector2(end);
	if(start_cell == NULL || end_cell == NULL)
	{
		return false;
	}

	if(start_cell == end_cell)
	{
		result.push_back(start);
		result.push_back(end);
	}
	else
	{
		build_path(start,end,start_cell,end_cell,result);
	}

	return true;
}

//比较函数
bool compare(cell *left,cell *right)
{
	return left->getF() > right->getF();
}

//构建路径
bool navmesh::build_path(const vector2 &start,const vector2 &end,cell* start_cell,cell* end_cell,vector<vector2> &result)
{
	end_cell->set_session(navmesh::g_path_sessionId);
	m_openlist.push_back(end_cell);
	push_heap(m_openlist.begin(),m_openlist.end(),compare);
	bool is_find = false;

	while(m_openlist.size() > 0)
	{
		pop_heap(m_openlist.begin(),m_openlist.end(),compare);
		cell *cur_node = m_openlist.back();
		m_openlist.pop_back();
		m_closelist.push_back(cur_node);
		if(cur_node == start_cell)
		{
			is_find = true;
			break;
		}
		int adjacentId = 0;
		for(int i = 0;i < 3; ++i)
		{
			adjacentId = cur_node->getlink((etriangle_side)i);
			if(adjacentId < 0)
			{
				continue;
			}

			cell *adjacent_node = m_cells[adjacentId];
			if(adjacent_node == NULL)
			{
				continue;
			}

			if(adjacent_node->get_session() != navmesh::g_path_sessionId)
			{
				adjacent_node->set_session(navmesh::g_path_sessionId);
				adjacent_node->set_open(true);
				adjacent_node->set_parent(cur_node);
				adjacent_node->computeHeuristic(start);
				float value_g = cur_node->getG() + cur_node->get_wall_distance(i,end);
				adjacent_node->setG(value_g);
				adjacent_node->set_arrival_wall(cur_node->getindex());
				m_openlist.push_back(adjacent_node);
				push_heap(m_openlist.begin(),m_openlist.end(),compare);
			}
			else
			{
				int index = cur_node->get_arrivalwall();
				float value_g = cur_node->getG() + cur_node->get_wall_distance(index,end);
				if (value_g + adjacent_node->getH() < adjacent_node->getF())
				{
					adjacent_node->setG(value_g);
					adjacent_node->set_parent(cur_node);
					adjacent_node->set_arrival_wall(cur_node->getindex());
					make_heap(m_openlist.begin(),m_openlist.end(),compare);
				}
			}
		}
	}

	if(!is_find)
	{
		return false;
	}

	get_path(start,end,result);

	return true;
}

//构建最短路径
bool navmesh::get_path(const vector2 &start,vector2 end,vector<vector2> &result)
{
	vector<cell*> cur_cells;
	get_cell_path(cur_cells);

	waypoint w_p(cur_cells[0],start);
	result.push_back(w_p.m_position);
	int start_index = 0;
	while(!w_p.m_position.equals(end))
	{
		get_furthest_way_point(w_p,cur_cells,end,start_index);
		result.push_back(w_p.m_position);
	}

	return true;
}

//根据坐标点找cell
cell* navmesh::find_cell_by_vector2(const vector2 &pos)
{
	for(int i = 0;i < m_cells.size();++i)
	{
		if(!m_cells[i]->is_vector2_in(pos))
		{
			continue;
		}

		return m_cells[i];
	}

	return NULL;
}

void navmesh::get_cell_path(vector<cell*> &result)
{
	cell *cur_node = m_closelist[m_closelist.size() - 1];
	while(cur_node != NULL)
	{
		result.push_back(cur_node);
		cur_node = cur_node->get_parent();
	}
}

//拐角点
void navmesh::get_furthest_way_point(waypoint &w_p,vector<cell*> &cells,vector2 &end,int &start_index)
{
	int arrivalwall_id = w_p.m_cell->get_arrivalwall();
	const line2D *out_side = w_p.m_cell->get_side(arrivalwall_id);
	cell *last_cell = w_p.m_cell;
	vector2 lastA = out_side->getA();
	vector2 lastB = out_side->getB();
	line2D lineA(w_p.m_position,lastA);
	line2D lineB(w_p.m_position,lastB);

	vector2 testA;
	vector2 testB;
	for(int i = start_index + 1;i < cells.size();++i)
	{
		if(i == cells.size() - 1)
		{
			testA = end;
			testB = end;
		}
		else
		{
			arrivalwall_id = cells[i]->get_arrivalwall();
			out_side = cells[i]->get_side(arrivalwall_id);
			testA = out_side->getA();
			testB = out_side->getB();
		}
		
		if(!lastA.equals(testA))
		{
			if(lineB.line_vetex_type(testA) == eon_right)
			{
				w_p.m_position = lastB;
				w_p.m_cell = last_cell;
				return ;
			}
			else if(lineA.line_vetex_type(testA) != eon_left)
			{
				start_index = i;
				lastA = testA;
				last_cell = cells[i];
				lineA.setB(lastA);
			}
		}

		if(!lastB.equals(testB))
		{
			if(lineA.line_vetex_type(testB) == eon_left)
			{
				w_p.m_position = lastA;
				w_p.m_cell = last_cell;
				return ;
			}
			else if(lineB.line_vetex_type(testB) != eon_right)
			{
				start_index = i;
				lastB = testB;
				last_cell = cells[i];
				lineB.setB(lastB);
			}
		}
	}
	w_p.m_position = end;
	w_p.m_cell = cells[cells.size() - 1];
	return ;
}

void navmesh::clear()
{
	m_openlist.clear();
	m_closelist.clear();
}