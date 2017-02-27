#include "Camera.h"

#include <Renderer.h>
#include <Logger.h>
#include <Common.h>

#include <assert.h>

#include <d3dx9math.h>

FTW::Renderer *FTW::Camera::s_pRenderer;

FTW::Camera::Camera()
{
	if (!s_pRenderer)
		s_pRenderer = Renderer::Get();

	assert(s_pRenderer);

	Init();
}

FTW::Camera::Camera(Renderer * renderer)
{
	s_pRenderer = renderer;
	assert(s_pRenderer);

	Init();
}

void FTW::Camera::Init()
{
	m_matProjection = nullptr;
	m_matWorld = nullptr;
	m_matView = nullptr;

	m_pos = nullptr;

	m_angle = 0;

	m_scale = nullptr;
}

FTW::Camera::~Camera()
{
	if (m_scale)
	{
		delete m_scale;
		m_scale = nullptr;
	}

	if (m_matProjection)
	{
		delete m_matProjection;
		m_matProjection = nullptr;
	}

	if (m_matWorld)
	{
		delete m_matWorld;
		m_matWorld = nullptr;
	}
	
	if (m_matView)
	{
		delete m_matView;
		m_matView = nullptr;
	}

	if (m_pos)
	{
		delete m_pos;
		m_pos = nullptr;
	}
}

bool FTW::Camera::Create(int width, int height)
{
	m_width = width;
	m_height = height;

	m_pos = new D3DXVECTOR2;

	m_scale = new D3DXVECTOR2;

	m_matProjection = new D3DXMATRIX;
	m_matWorld = new D3DXMATRIX;
	m_matView = new D3DXMATRIX;

	*m_pos = D3DXVECTOR2(0.f, 0.f);
	*m_scale = D3DXVECTOR2(1.f, 1.f);

	D3DXMatrixOrthoLH(m_matProjection, (float)m_width, -(float)m_height, 0.0f, 1.0f);
	D3DXMatrixIdentity(m_matWorld);

	return true;
}

void FTW::Camera::Render()
{
	s_pRenderer->GetDevice()->SetTransform(D3DTS_PROJECTION, m_matProjection);
	s_pRenderer->GetDevice()->SetTransform(D3DTS_WORLD, m_matWorld);
	s_pRenderer->GetDevice()->SetTransform(D3DTS_VIEW, m_matView);
}

void FTW::Camera::SetPos(float x, float y)
{
	SetPos(&D3DXVECTOR2(x, y));
}

void FTW::Camera::SetPos(D3DXVECTOR2 * pos)
{
	m_pos = pos;

	if (!m_matView)
		return;

	if (!m_scale)
		return;

	if (!m_pos)
		return;

	D3DXMatrixTransformation2D(m_matView, NULL, NULL, m_scale, &D3DXVECTOR2((float)m_width / 2, (float)m_height / 2), m_angle, &D3DXVECTOR2(-m_pos->x * m_scale->x, -m_pos->y * m_scale->y));
}
