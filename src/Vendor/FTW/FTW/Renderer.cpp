/*
*********************************************************************
* File          : Renderer.cpp
* Project		: FTW
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "Renderer.h"

#include <Common.h>
#include <Logger.h>

#include <stdexcept>

FTW::Renderer *FTW::Renderer::s_pInst;

FTW::Renderer::Renderer()
{
	m_pD3D = nullptr;
	m_pDevice = nullptr;
	s_pInst = this;
}

FTW::Renderer::~Renderer()
{
	Renderer::Shutdown();
}

bool FTW::Renderer::Init(HWND window)
{
	Logger::Msg("Initializing DirectX...");

	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (!m_pD3D)
	{
		Logger::Error("Cannot initialize DirectX!");
#ifdef THROW_EXCEPTION
		throw std::runtime_error("Cannot initialize DirectX!");
#else
		return false;
#endif
	}

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = true;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.hDeviceWindow = window;

	HRESULT result = m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_pDevice);
	
	if (FAILED(result))
	{
		Shutdown();

		const char *error = FTW::GetDXErrorMsg(result);

		Logger::Error("Cannot initialize DirectX! %s", error);

#ifdef THROW_EXCEPTION
		throw std::runtime_error(error);
#else
		return false;
#endif

		delete[] error;
	}

	m_hWnd = window;

	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pDevice->SetRenderState(D3DRS_LIGHTING, false);

	Logger::Msg("DirectX initialized!");

	return true;
}

void FTW::Renderer::Shutdown()
{
	Logger::Msg("Shutting down Renderer...");

	if (m_pD3D)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}

	if (m_pDevice)
	{
		m_pDevice->Release();
		m_pDevice = nullptr;
	}
}

HWND FTW::Renderer::CreateWnd(unsigned width, unsigned height, const char *title, WNDPROC wndproc, int x, int y)
{
	Logger::Msg("Creating window with client size: %dx%d \"%s\"...", width, height, title);

	m_wndWidth = width;
	m_wndHeight = height;

	HINSTANCE hInst = GetModuleHandle(0);

	WNDCLASS wc = { 0 };
	wc.hInstance = hInst;
	wc.lpfnWndProc = wndproc;
	wc.lpszClassName = "WndClass";
	wc.lpszMenuName = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hIcon = LoadIcon(hInst, IDI_WINLOGO);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hCursor = LoadCursor(0, IDC_ARROW);

	if (!RegisterClass(&wc))
	{
		Logger::Error("Cannot register window class");

#ifdef THROW_EXCEPTION
		throw std::runtime_error("Cannot register window class");
#else
		return false;
#endif
	}

	RECT wndRect = { 0, 0, (int)m_wndWidth, (int)m_wndHeight };
	AdjustWindowRect(&wndRect, WS_OVERLAPPEDWINDOW, false);

	int realWndWidth = wndRect.right - wndRect.left;
	int realWndHeight = wndRect.bottom - wndRect.top;

	m_hWnd = CreateWindowEx(WS_EX_CLIENTEDGE, "WndClass", title, WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,
		x, y, realWndWidth, realWndHeight, 0, 0, hInst, 0);

	if (!m_hWnd)
	{
		Logger::Error("Cannot create window!");

#ifdef THROW_EXCEPTION
		throw std::runtime_error("Cannot create window!");
#else
		return false;
#endif
	}


	RECT rect;

	GetClientRect(m_hWnd, &rect);

	m_wndWidth = rect.right - rect.left;
	m_wndHeight = rect.bottom - rect.top;

	Logger::Msg("Created window: %dx%d with client size: %dx%d", realWndWidth, realWndHeight, m_wndWidth, m_wndHeight);

	ShowWindow(m_hWnd, SW_SHOW);

	Logger::Msg("Window has been created!");

	return m_hWnd;
}

void FTW::Renderer::BeginScene(D3DCOLOR bgColor)
{
	if (bgColor == 0)
		bgColor = D3DCOLOR_ARGB(255, 255, 255, 255);

	m_pDevice->Clear(0, 0, D3DCLEAR_TARGET, bgColor, 1, 0);
	m_pDevice->BeginScene();
}

void FTW::Renderer::EndScene()
{
	m_pDevice->EndScene();
	m_pDevice->Present(0, 0, 0, 0);
}

FTW::Renderer * FTW::Renderer::Get()
{
	return s_pInst;
}
