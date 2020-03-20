#include "stdafx.h"
#include "delaunay.h"
#include "vector2.h"
#include "line2D.h"
#include "triangle.h"
#include "polygon.h"

#define PI (3.1415926)

delaunay::delaunay(vector<polygon*> &polygon) :
m_polygon(polygon)
{

}

delaunay::~delaunay()
{
	clear();
}

//生成三角剖
bool delaunay::create_delaunay(vector<triangle*> &result)
{
	//保存所有多边形的边和点
	init();

	line2D* out_line = get_init_outedge();
	if(out_line == NULL)
	{
		return false;
	}

	vector<line2D*> lines;
	lines.push_back(out_line);
	int flag = 1;
	while(lines.size() > 0)
	{
		line2D* edge = *lines.begin();
		lines.erase(lines.begin());
		vector2 p3;
		bool is_find = findDT(*edge,p3);
		if(!is_find)
		{
			continue;
		}
		line2D line13(edge->getA(),p3);
		line2D line32(p3,edge->getB());
		triangle *trg = new triangle(edge->getA(),edge->getB(),p3);

		result.push_back(trg);
		trg->set_flag(flag);
		++flag;

		int index = 0;
		if(constraint_edge(line13,m_edges) < 0)
		{
			index = constraint_edge(line13,lines);
			if(index > -1)
			{
				delete lines[index];
				lines.erase(lines.begin() + index);
			}
			else
			{
				lines.push_back(new line2D(line13));
			}
		}

		if(constraint_edge(line32,m_edges) < 0)
		{
			index = constraint_edge(line32,lines);
			if(index > -1)
			{
				delete lines[index];
				lines.erase(lines.begin() + index);
			}
			else
			{
				lines.push_back(new line2D(line32));
			}
		}
	}

	return true;
}

//初始化函数
void delaunay::init()
{
	for(int i = 0;i < m_polygon.size();++i)
	{
		put_vertex_edge(m_polygon[i]->get_vertex());
	}
}

//一个多边形所有边和点
void delaunay::put_vertex_edge(const vector<vector2> &vertexs)
{
	const vector2 *start = &vertexs[0];
	for(int i = 0;i < vertexs.size();++i)
	{
		m_vertexs.push_back(vertexs[i]);
		if(i >= 1)
		{
			const vector2 &end = vertexs[i];
			m_edges.push_back(new line2D(*start,end));
			start = &end;
		}
	}

	//最后一个点
	m_edges.push_back(new line2D(*start,vertexs[0]));
}

//获取外边界
line2D* delaunay::get_init_outedge()
{
	const vector<vector2>& out_vetexs = m_polygon[0]->get_vertex();
	//外边界个数为out_vetexs.size()个，一般为4个，即屏幕窗口
	for(int i = 0;i < out_vetexs.size();++i)
	{
		bool is_find = true;
		for(int j = 0;j < m_vertexs.size();++j)
		{
			if(m_vertexs[j].equals(m_edges[i]->getA()) || m_vertexs[j].equals(m_edges[i]->getB()))
			{
				continue;
			}
			if (m_edges[i]->line_vetex_type(m_vertexs[j]) == eon_line)
			{
				is_find = false;
				break;
			}
		}

		if(is_find)
		{
			return m_edges[i];
		}
	}

	return NULL;
}

//找到DT点
bool delaunay::findDT(const line2D &line,vector2 &result)
{
	vector<vector2> allpoint;
	for(int i = 0;i < m_vertexs.size();++i)
	{
		if(!isVisible_point_line(line,m_vertexs[i]))
		{
			continue;
		}
		allpoint.push_back(m_vertexs[i]);
	}
	if(allpoint.size() == 0)
	{
		return false;
	}
	const vector2 &p1 = line.getA();
	const vector2 &p2 = line.getB();
	vector2 p3 = allpoint[0];
	bool loop = true;
	while(loop)
	{
		loop = false;
		circle cir;
		circumCircle(p1,p2,p3,cir);
		rectangle rect(cir);
		float angle132 = abs(line_angle(p1, p3, p2));	// ∠p1p3p2
		for(int i = 0;i < allpoint.size();++i)
		{
			if(allpoint[i].equals(p1) || allpoint[i].equals(p2) || allpoint[i].equals(p3))
			{
				continue;
			}
			if(!rect.contains(allpoint[i].m_x,allpoint[i].m_y))
			{
				continue;
			}
			float a1 = abs(line_angle(p1, allpoint[i], p2));
			if (a1 > angle132)
			{
				p3   = allpoint[i];
				allpoint.erase(allpoint.begin()+i);
				loop = true;
				break;
			}
		}
	}

	result = p3;

	return true;
}

//点是否为直线的可见点
bool delaunay::isVisible_point_line(const line2D &line,const vector2 &p)
{
	if(p.equals(line.getA()) || p.equals(line.getB()))
	{
		return false;
	}
	
	if(line.line_vetex_type(p) != eon_right)
	{
		return false;
	}

	if(!isvisiblepoint(line.getA(),p))
	{
		return false;
	}

	if(!isvisiblepoint(line.getB(),p))
	{
		return false;
	}

	return true;
}

//两个点是否为可见点
bool delaunay::isvisiblepoint(const vector2 &A,const vector2 &B)
{
	line2D line_A_B(A,B);
	vector2 interscet;
	for(int i = 0;i < m_edges.size();++i)
	{
		if(m_edges[i]->line_line_type(line_A_B,interscet) == SEGMENTS_INTERSECT)
		{
			if(!A.equals(interscet) && !B.equals(interscet))
			{
				return false;
			}
		}
	}

	return true;
}

void delaunay::circumCircle(const vector2 &p1,const vector2 &p2,const vector2 &p3,circle &cir)
{
	float m1,m2,mx1,mx2,my1,my2;
	float dx,dy,rsqr,drsqr;
	float xc, yc, r;

	/* Check for coincident points */

	if (abs(p1.m_y-p2.m_y) < 0.001f && abs(p2.m_y-p3.m_y) < 0.001f )
	{
		return ;
	}

	m1 = - (p2.m_x - p1.m_x) / (p2.m_y - p1.m_y);
	m2 = - (p3.m_x-p2.m_x) / (p3.m_y-p2.m_y);
	mx1 = (p1.m_x + p2.m_x) / 2.0;
	mx2 = (p2.m_x + p3.m_x) / 2.0;
	my1 = (p1.m_y + p2.m_y) / 2.0;
	my2 = (p2.m_y + p3.m_y) / 2.0;

	if ( abs(p2.m_y-p1.m_y) < 0.001f )
	{
		xc = (p2.m_x + p1.m_x) / 2.0;
		yc = m2 * (xc - mx2) + my2;
	}
	else if ( abs(p3.m_y - p2.m_y) < 0.001f )
	{
		xc = (p3.m_x + p2.m_x) / 2.0;
		yc = m1 * (xc - mx1) + my1;
	}
	else
	{
		xc = (m1 * mx1 - m2 * mx2 + my2 - my1) / (m1 - m2);
		yc = m1 * (xc - mx1) + my1;
	}

	dx = p2.m_x - xc;
	dy = p2.m_y - yc;
	rsqr = dx*dx + dy*dy;

	cir.m_r = sqrt(rsqr);
	cir.m_center.m_x = xc;
	cir.m_center.m_y = yc;
}

/**
* 返回顶角在o点，起始边为os，终止边为oe的夹角, 即∠soe (单位：弧度)
* 角度小于pi，返回正值;   角度大于pi，返回负值
*/
float delaunay::line_angle(const vector2 &s, const vector2 &o, const vector2 &e)
{
	float cosfi, fi, norm;
	float dsx = s.m_x - o.m_x;
	float dsy = s.m_y - o.m_y;
	float dex = e.m_x - o.m_x;
	float dey = e.m_y - o.m_y;

	cosfi  = dsx*dex + dsy*dey;
	norm   = (dsx*dsx + dsy*dsy) * (dex*dex + dey*dey);
	cosfi /= sqrt( norm );

	if (cosfi >=  1.0 ) return 0;
	if (cosfi <= -1.0 ) return -PI;

	fi = acos(cosfi);
	if (dsx*dey - dsy*dex > 0)
	{
		return fi;      // 说明矢量os 在矢量 oe的顺时针方向
	}

	return -fi;
}

//边是否在集合中
int delaunay::constraint_edge(const line2D &line,const vector<line2D*> &edges)
{
	for(int i = 0;i < edges.size();++i)
	{
		if(edges[i]->equals(line))
		{
			return i;
		}
	}

	return -1;
}

void delaunay::clear()
{
	for(int i = 0;i < m_edges.size();++i)
	{
		delete m_edges[i];
	}

	m_edges.clear();
}