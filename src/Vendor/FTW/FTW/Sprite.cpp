/*
*********************************************************************
* File          : Sprite.cpp
* Project		: FTW
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "Sprite.h"

#include <Renderer.h>
#include <Texture.h>

#include <Logger.h>

#include <Common.h>
#include <stdexcept>

#include <assert.h>

FTW::Renderer *FTW::Sprite::s_pRenderer;

FTW::Sprite::Sprite()
{
	if (!s_pRenderer)
		s_pRenderer = Renderer::Get();

	assert(s_pRenderer);

	Init();
}

FTW::Sprite::Sprite(Renderer *renderer)
{
	s_pRenderer = renderer;
	assert(s_pRenderer);

	Init();
}

void FTW::Sprite::Init()
{
	m_pSprite = nullptr;
	m_pTexture = nullptr;

	m_pos = new D3DXVECTOR2;
	m_center = new D3DXVECTOR2;

	m_localTex = true;

	m_animSpeed = 0.75f;

	SetColor(255, 255, 255, 255);
}

FTW::Sprite::~Sprite()
{
	Destroy();
}

bool FTW::Sprite::Create(Texture *tex, bool local)
{
	assert(s_pRenderer);
	assert(tex);

	m_localTex = local;

	m_pTexture = tex;

	HRESULT result = D3DXCreateSprite(s_pRenderer->GetDevice(), &m_pSprite);

	if (FAILED(result))
	{
		Destroy();

		const char *error = FTW::GetDXErrorMsg(result);

		Logger::Error("Cannot create sprite! %s", error);

#ifdef THROW_EXCEPTION
		throw std::runtime_error(error);
#else
		return false;
#endif

		delete[] error;
	}

	assert(m_pSprite);

	if (tex->IsAnim())
	{
		m_currFrame = 0;
		m_currRow = 0;
	}

	SetCenter((float)tex->GetFrameWidth() / 2, (float)tex->GetFrameHeight() / 2);

	return true;
}

void FTW::Sprite::Destroy()
{
	if (m_pSprite)
	{
		m_pSprite->Release();
		m_pSprite = nullptr;
	}

	if (m_pTexture && m_localTex)
	{
		delete m_pTexture;
		m_pTexture = nullptr;
	}

	if (m_pos)
	{
		delete m_pos;
		m_pos = nullptr;
	}

	if (m_center)
	{
		delete m_center;
		m_center = nullptr;
	}
}		

void FTW::Sprite::Pulse()
{
	if (m_pTexture->IsAnim())
	{
		assert(m_animInited);

		unsigned maxFrames = m_startFrame + m_nFrames - 1;
		unsigned maxRows = m_startRow + m_nRows - 1;

		if (!m_animEnd && (m_tmLastFrameUpdate + unsigned(m_frameSpaceUpdate * m_animSpeed) < GetTickCount()))
		{
			m_tmLastFrameUpdate = GetTickCount();

			// if last frame
			if (m_currFrame == maxFrames)
			{
				// if last row
				if (m_currRow == maxRows)
				{
					// if loop, restart animation
					if (m_animLoop)
					{
						m_currRow = m_startRow;
						m_currFrame = m_startFrame;
					}
					else // if not loop, stop animation
						m_animEnd = true;
				}
				else if (m_currRow < maxRows) // if still animation
				{
					m_currRow++;
				}
			}
			else if (m_currFrame < maxFrames)
			{
				if (!m_animEnd)
					m_currFrame++;
			}
		}
	}
}

void FTW::Sprite::Render(float x, float y)
{
	Render(D3DXVECTOR2(x, y));
}

void FTW::Sprite::Render(D3DXVECTOR2 pos)
{
	assert(m_pSprite);
	assert(m_pTexture);
	assert(m_pTexture->GetTexture());

	if (!m_pSprite || !m_pTexture || !m_pTexture->GetTexture())
		return;

	if (m_transformByCamera)
		m_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_OBJECTSPACE);
	else
		m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	{
		D3DXMATRIX matrix;
		float angle = 2 * D3DX_PI * m_direction / 360;

		D3DXMatrixTransformation2D(&matrix, NULL, NULL, NULL, m_center, angle, &pos);
		m_pSprite->SetTransform(&matrix);

		if (m_pTexture->IsAnim())
		{
			RECT rct;
			rct.left = (int)m_pTexture->GetFrameWidth()  * m_currFrame;
			rct.right = (int)m_pTexture->GetFrameWidth()  * (m_currFrame + 1);
			rct.top = (int)m_pTexture->GetFrameHeight() * m_currRow;
			rct.bottom = (int)m_pTexture->GetFrameHeight() * (m_currRow + 1);

			m_pSprite->Draw(m_pTexture->GetTexture(), &rct, 0, 0, m_color);
		}
		else
		{
			m_pSprite->Draw(m_pTexture->GetTexture(), 0, 0, 0, m_color);
		}
	}
	m_pSprite->End();
}

void FTW::Sprite::SetupAnim(bool loop, unsigned frames, unsigned rows)
{
	m_startFrame = 0;
	m_startRow = 0;

	m_nFrames = frames;
	m_nRows = rows;

	m_animLoop = loop;

	m_currFrame = m_startFrame;
	m_currRow = m_startRow;

	m_tmLastFrameUpdate = GetTickCount();

	m_animInited = true;
}

void FTW::Sprite::SetupAnim(bool loop, unsigned startFrame, unsigned frames, unsigned startRow, unsigned rows)
{
	m_startFrame = startFrame;
	m_startRow = startRow;

	m_nFrames = frames;
	m_nRows = rows;

	m_animLoop = loop;

	m_currFrame = m_startFrame;
	m_currRow = m_startRow;

	m_tmLastFrameUpdate = GetTickCount();

	m_animInited = true;
}
