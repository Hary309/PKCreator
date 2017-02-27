/*
*********************************************************************
* File          : Renderer.h
* Project		: FTW
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include <d3dx9.h>
#include <Windows.h>
#include <Declspec.h>

namespace FTW
{
	extern "C"
	{
		struct DECLSPEC VERTEX_2D_DIF
		{
			float x, y, z, rhw;
			D3DCOLOR color;
			static const DWORD FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE;
		};

		struct DECLSPEC VERTEX_2D_TEX
		{
			float x, y, z, rhw;
			D3DCOLOR color;
			float tx, ty;
			static const DWORD FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
		};

		class DECLSPEC Renderer
		{
		private:
			IDirect3D9			*m_pD3D;
			IDirect3DDevice9    *m_pDevice;

			HWND				m_hWnd;

			unsigned			m_wndWidth;
			unsigned			m_wndHeight;

			static Renderer		*s_pInst;

		public:
			Renderer();
			~Renderer();

			bool Init(HWND window);
			void Shutdown();

			HWND CreateWnd(unsigned width, unsigned height, const char *title, WNDPROC wndproc, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT);

			// @bgColor if 0 default white
			void BeginScene(D3DCOLOR bgColor = 0);
			void EndScene();

			IDirect3DDevice9 *GetDevice()   { return m_pDevice; }

			unsigned GetWndWidth()			{ return m_wndWidth; }
			unsigned GetWndHeight()			{ return m_wndHeight; }

			HWND GetWindow()				{ return m_hWnd; }

			static Renderer *Get();
		};
	}
}