#ifndef __navmeshH__
#define __navmeshH__

#include <vector>

using namespace std;

class cell;
class vector2;
class waypoint;

class navmesh
{
public:
	navmesh(vector<cell*> &cells);

	~navmesh();

public:
	//Ѱ·
	bool find_path(vector2 start,vector2 end,vector<vector2> &result);

private:
	//����·��
	bool build_path(vector2 start,vector2 end,cell* start_cell,cell* end_cell,vector<vector2> &result);

	//�������·��
	bool get_path(vector2 start,vector2 end,vector<vector2> &result);

	//�����������cell
	cell* find_cell_by_vector2(const vector2 &pos);

	void get_cell_path(vector<cell*> &result);

	//�սǵ�
	waypoint get_furthest_way_point(waypoint &w_p,vector<cell*> &cells,vector2 &end);

	int index_cell(vector<cell*> &cells,cell *c_cell);

	void clear();

private:
	vector<cell*> &m_cells;
	vector<cell*> m_openlist;
	vector<cell*> m_closelist;

	static int g_path_sessionId;
};

#endif