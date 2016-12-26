#include "stdafx.h"
#include "d3dUtility.h"
#include "navmgr.h"
#include "vector2.h"
#include "triangle.h"
#include "polygon.h"

using namespace std;
enum efind_path_type
{
	epath_null  = 0,
	epath_start = 1,
	epath_end   = 2,
};

enum eobject_type
{
	elines_type    = 0,
	epolygon_type  = 1,
	etriangle_type = 2,
	epath_type     = 3,
};

IDirect3DDevice9* Device = 0;
navmgr *g_nav_mgr  		 = new navmgr();
LPD3DXLINE g_pLine  	 = NULL; //Direct3D线对象
estage g_stage 			 = eline;
D3DXVECTOR2 g_start_pos(-1,-1);
efind_path_type g_find_path_type = epath_null;
ID3DXFont* g_font = NULL;

const int Width  = 1600;
const int Height = 900;

D3DCOLOR g_color[efindpath+1][epath_type+1] = {
	{D3DCOLOR_XRGB(135,135,135),D3DCOLOR_XRGB(135,135,135),D3DCOLOR_XRGB(135,135,135),D3DCOLOR_XRGB(135,135,135)},
	{D3DCOLOR_XRGB(135,135,135),D3DCOLOR_XRGB(32,178,170),D3DCOLOR_XRGB(238,64,0),D3DCOLOR_XRGB(238,201,0)}
};

WCHAR g_prompt_test[2][256] = {
	{L"左键绘制多边形,按回车生成三角剖"},
	{L"左键点击start点和end点,进入寻路,按回车回到绘制多边形界面"}
};

void draw_line();
void draw_polygon();
void draw_triangle();
void draw_path();
void draw_point(D3DXVECTOR2 &pos,WCHAR name[]);
void draw_prompt_text();

bool Setup()
{
    // 创建Direct3D线对象
    if (FAILED(D3DXCreateLine(Device, &g_pLine)))
    {
        return false;
    }


    D3DXFONT_DESC           font_desc;
    // create the font
    ZeroMemory(&font_desc, sizeof(font_desc));

    // set font descripter
    lstrcpyW(font_desc.FaceName, TEXT("Arial"));
    font_desc.Height = 32;

    // Creates a font object indirectly for both a device and a font
    D3DXCreateFontIndirect(Device, &font_desc, &g_font);

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
			&proj,
			D3DX_PI * 0.5f, // 90 - degree
			(float)Width / (float)Height,
			1.0f,
			1000.0f);
	Device->SetTransform(D3DTS_PROJECTION, &proj);

	Device->SetRenderState(D3DRS_LIGHTING, false);

	return true;
}

void Cleanup()
{

}

bool Display(float timeDelta)
{
	if(Device)
	{
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,0xdddddd/*D3DCOLOR_XRGB(43, 43, 43)*/, 1.0f, 0);
		Device->BeginScene();

		draw_line();
		draw_polygon();
		draw_triangle();
		draw_path();
		if(g_find_path_type == epath_start)
		{
			draw_point(g_start_pos,L"start");
		}
		draw_prompt_text();

		Device->EndScene();
		Device->Present(0, 0, 0, 0);
	}
	return true;
}

void draw_prompt_text()
{
	RECT rect = {Width/2 - 400,10,Width/2 + 400,Width/2+10};
	g_font->DrawText(
		NULL,
		g_prompt_test[g_stage],
		-1, // size of string or -1 indicates null terminating string
		&rect,            // rectangle text is to be formatted to in windows coords
		DT_TOP | DT_CENTER, // draw in the top left corner of the viewport
		D3DCOLOR_XRGB(0,205,0));      // black text
}

void draw_line()
{
	vector<vector2>& lines = g_nav_mgr->get_lines();
	if(lines.size() == 0)
	{
		return ;
	}

	D3DXVECTOR2 *d3d_vector2 = new D3DXVECTOR2[lines.size()];
	for(int i = 0;i < lines.size();++i)
	{
		d3d_vector2[i].x = lines[i].m_x;
		d3d_vector2[i].y = lines[i].m_y;
	}

	g_pLine->SetWidth(5.f);
    g_pLine->SetAntialias(true);
	g_pLine->Draw(d3d_vector2,lines.size(),g_color[g_stage][elines_type]);
	draw_point(D3DXVECTOR2(lines[0].m_x,lines[0].m_y),L"");

	delete [] d3d_vector2;
}

void draw_polygon()
{
	vector<polygon*>& polygons = g_nav_mgr->get_polygon();
	if(polygons.size() == 0)
	{
		return ;
	}

	for(int i = 0;i < polygons.size();++i)
	{
		const vector<vector2>& vertexs = polygons[i]->get_vertex();
		D3DXVECTOR2 *d3d_vector2       = new D3DXVECTOR2[vertexs.size() + 1];
		for(int j = 0;j < vertexs.size();++j)
		{
			d3d_vector2[j].x = vertexs[j].m_x;
			d3d_vector2[j].y = vertexs[j].m_y;
		}

		d3d_vector2[vertexs.size()].x = vertexs[0].m_x;
		d3d_vector2[vertexs.size()].y = vertexs[0].m_y;

		g_pLine->SetWidth(7.f);
	    g_pLine->SetAntialias(true);
		g_pLine->Draw(d3d_vector2,vertexs.size() + 1,g_color[g_stage][epolygon_type]);

		delete [] d3d_vector2;
	}
}

void draw_triangle()
{
	vector<triangle*>& triangles = g_nav_mgr->get_triangles();
	if(triangles.size() == 0)
	{
		return ;
	}

	for(int i = 0;i < triangles.size();++i)
	{
		D3DXVECTOR2 *d3d_vector2 = new D3DXVECTOR2[4];
		const vector2 &pA = triangles[i]->getA();
		const vector2 &pB = triangles[i]->getB();
		const vector2 &pC = triangles[i]->getC();

		d3d_vector2[0].x = pA.m_x;
		d3d_vector2[0].y = pA.m_y;

		d3d_vector2[1].x = pB.m_x;
		d3d_vector2[1].y = pB.m_y;

		d3d_vector2[2].x = pC.m_x;
		d3d_vector2[2].y = pC.m_y;

		d3d_vector2[3].x = pA.m_x;
		d3d_vector2[3].y = pA.m_y;

		g_pLine->SetWidth(1.f);
	    g_pLine->SetAntialias(true);
		g_pLine->Draw(d3d_vector2,4,g_color[g_stage][etriangle_type]);

		delete [] d3d_vector2;
		continue;
		const vector2 &center = triangles[i]->get_center();
		RECT rect = {center.m_x,center.m_y,center.m_x+40,center.m_y+60};
		char str[16] = {0};
		sprintf(str, "%d", triangles[i]->get_flag());
		g_font->DrawTextA(
			NULL,
			str,
			-1, // size of string or -1 indicates null terminating string
			&rect,            // rectangle text is to be formatted to in windows coords
			DT_TOP | DT_LEFT, // draw in the top left corner of the viewport
			D3DCOLOR_XRGB(0,205,0));      // black text
	}
}

void draw_path()
{
	vector<vector<vector2>>& paths = g_nav_mgr->get_all_path();
	if(paths.size() == 0)
	{
		return ;
	}

	for(int i = 0;i < paths.size();++i)
	{
		D3DXVECTOR2 *d3d_vector2 = new D3DXVECTOR2[paths[i].size()];
		for(int j = 0;j < paths[i].size();++j)
		{
			d3d_vector2[j].x = paths[i][j].m_x;
			d3d_vector2[j].y = paths[i][j].m_y;
		}
		g_pLine->SetWidth(5.f);
	    g_pLine->SetAntialias(true);
		g_pLine->Draw(d3d_vector2,paths[i].size(),g_color[g_stage][epath_type]);

		delete [] d3d_vector2;

		float x = paths[i][0].m_x;
		float y = paths[i][0].m_y;
		draw_point(D3DXVECTOR2(x,y),L"start");

		x = paths[i][paths[i].size() - 1].m_x;
		y = paths[i][paths[i].size() - 1].m_y;

		draw_point(D3DXVECTOR2(x,y),L"end");
	}
}

void draw_point(D3DXVECTOR2 &pos,WCHAR name[])
{
	if(pos.x < 0 || pos.y < 0)
	{
		return ;
	}
	float velue = 6;

	D3DXVECTOR2 all_pos1[2];
	all_pos1[0].x = pos.x - velue;
	all_pos1[0].y = pos.y;

	all_pos1[1].x = pos.x + velue;
	all_pos1[1].y = pos.y;

	g_pLine->SetWidth(velue*2);
	g_pLine->SetAntialias(true);
	g_pLine->Draw(all_pos1,2,D3DCOLOR_XRGB(0,205,0));

	RECT rect = {pos.x - 100,pos.y,pos.x + 100,pos.y + 100};
	g_font->DrawText(
	NULL,
	name,
	-1, // size of string or -1 indicates null terminating string
	&rect,            // rectangle text is to be formatted to in windows coords
	DT_TOP | DT_CENTER, // draw in the top left corner of the viewport
	D3DCOLOR_XRGB(0,205,0));      // black text
}

//
// WndProc
//
LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if( wParam == VK_ESCAPE )
		{
			::DestroyWindow(hwnd);
		}
		if(wParam ==VK_RETURN)
		{
			switch(g_stage)
			{
				case eline:
				{
					g_stage = efindpath;
					g_nav_mgr->build_triangle();
					break;
				}
				case efindpath:
				{
					g_nav_mgr->clear();
					g_stage = eline;
					g_find_path_type = epath_null;
					break;
				}
				default:
				{
					break;
				}
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			switch(g_stage)
			{
				case eline:
				{
					g_nav_mgr->add_point(x,y);
					break;
				}
				case efindpath:
				{
					if(g_find_path_type == epath_null)
					{
						g_start_pos.x = x;
						g_start_pos.y = y;
						g_find_path_type  = epath_start;
					}
					else
					{
						g_find_path_type = epath_null;
						vector2 start_pos(g_start_pos.x,g_start_pos.y);
						vector2 end_pos(x,y);
						g_nav_mgr->find_path(start_pos,end_pos);
					}
					break;
				}
				default:
				{
					break;
				}
			}
			break;
		}
	case WM_RBUTTONDOWN:
		break;
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

//
// WinMain
//
int WINAPI WinMain(HINSTANCE hinstance,
				   HINSTANCE prevInstance,
				   PSTR cmdLine,
				   int showCmd)
{
	if(!d3d::InitD3D(hinstance,
		Width, Height, true, D3DDEVTYPE_HAL, &Device,g_nav_mgr))
	{
		::MessageBox(0, L"InitD3D() - FAILED", 0, 0);
		return 0;
	}

	if(!Setup())
	{
		::MessageBox(0, L"Setup() - FAILED", 0, 0);
		return 0;
	}

	d3d::EnterMsgLoop( Display );

	Cleanup();

	Device->Release();

	return 0;
}