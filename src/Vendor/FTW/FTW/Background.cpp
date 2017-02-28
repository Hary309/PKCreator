/*
*********************************************************************
* File          : Background.cpp
* Project		: FTW
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "Background.h"

#include <Renderer.h>
#include <Texture.h>
#include <Common.h>
#include <Logger.h>

#include <assert.h>
#include <stdexcept>

FTW::Renderer *FTW::Background::s_pRenderer;

FTW::Background::Background()
{
	if (!s_pRenderer)
		s_pRenderer = Renderer::Get();

	assert(s_pRenderer);
}

FTW::Background::Background(Renderer *renderer)
{
	s_pRenderer = renderer;

	assert(s_pRenderer);
}

FTW::Background::~Background()
{
	Destroy();
}

bool FTW::Background::Create(Texture *tex, float x, float y, unsigned color, unsigned widthRepeat, unsigned heightRepeat, TypeRepeat repeatMode)
{
	assert(s_pRenderer);
	assert(tex);

	m_pTexture = tex;

	if (!m_pTexture)
	{
		Logger::Error("Pointer to texture is empty!");

#ifdef THROW_EXCEPTION
		throw std::runtime_error("Pointer to texture is empty!");
#else
		return false;
#endif
	}

	VERTEX_2D_TEX *vertices = { 0 };
	unsigned vertexSize = 0;

	unsigned wndWidth = s_pRenderer->GetWndWidth();
	unsigned wndHeight = s_pRenderer->GetWndHeight();
	unsigned texWidth = (unsigned)m_pTexture->GetFrameWidth();
	unsigned texHeight = (unsigned)m_pTexture->GetFrameHeight();

	if (widthRepeat == 0)
		widthRepeat = wndWidth;

	if (heightRepeat == 0)
		heightRepeat = wndHeight;

	if (color == 0)
		color = D3DCOLOR_ARGB(255, 255, 255, 255);

	switch (repeatMode)
	{
		case NO_REPEAT:
		{
			vertices = new VERTEX_2D_TEX[4];

			vertices[0] = { x + texWidth, y,			 0, 1, color, 1.f, 0.f };
			vertices[1] = { x,			  y,			 0, 1, color, 0.f, 0.f };
			vertices[2] = { x,			  y + texHeight, 0, 1, color, 0.f, 1.f };
			vertices[3] = { x + texWidth, y + texHeight, 0, 1, color, 1.f, 1.f };

			m_repeats = 1;

			vertexSize = sizeof(VERTEX_2D_TEX) * 4;
		} break;
		case REPEAT_VER:
		{
			unsigned verRepeats = (unsigned)ceil((float)widthRepeat / (float)texWidth);

			vertices = new VERTEX_2D_TEX[4 * verRepeats];

			for (unsigned i = 0; i < verRepeats; ++i)
			{
				unsigned index = i * 4;

				vertices[index + 0] = { x + texWidth, y,             0, 1, color, 1.f, 0.f };
				vertices[index + 1] = { x,			  y,			 0, 1, color, 0.f, 0.f };
				vertices[index + 2] = { x,			  y + texHeight, 0, 1, color, 0.f, 1.f };
				vertices[index + 3] = { x + texWidth, y + texHeight, 0, 1, color, 1.f, 1.f };

				x += texWidth;
			}

			m_repeats = verRepeats;

			vertexSize = sizeof(VERTEX_2D_TEX) * verRepeats * 4;
		} break;
		case REPEAT_HOR:
		{
			unsigned horRepeats = (unsigned)ceil((float)heightRepeat / (float)texHeight);

			vertices = new VERTEX_2D_TEX[4 * horRepeats];

			for (unsigned i = 0; i < horRepeats; ++i)
			{
				unsigned index = i * 4;

				vertices[index + 0] = { x + texWidth, y,			 0, 1, color, 1.f, 0.f };
				vertices[index + 1] = { x,            y,			 0, 1, color, 0.f, 0.f };
				vertices[index + 2] = { x,            y + texHeight, 0, 1, color, 0.f, 1.f };
				vertices[index + 3] = { x + texWidth, y + texHeight, 0, 1, color, 1.f, 1.f };

				y += texHeight;
			}

			m_repeats = horRepeats;

			vertexSize = sizeof(VERTEX_2D_TEX) * horRepeats * 4;
		} break;
		case REPEAT_BOTH:
		{
			unsigned verRepeats = (unsigned)ceil((float)widthRepeat / (float)texWidth);
			unsigned horRepeats = (unsigned)ceil((float)heightRepeat / (float)texHeight);

			vertices = new VERTEX_2D_TEX[4 * verRepeats * horRepeats];

			float yStart = y;

			for (unsigned i = 0; i < verRepeats; ++i)
			{
				for (unsigned j = 0; j < horRepeats; ++j)
				{
					unsigned index = ((i * verRepeats) + j) * 4;

					vertices[index + 0] = { x + texWidth, y,			 0, 1, color, 1.f, 0.f };
					vertices[index + 1] = { x,            y,			 0, 1, color, 0.f, 0.f };
					vertices[index + 2] = { x,            y + texHeight, 0, 1, color, 0.f, 1.f };
					vertices[index + 3] = { x + texWidth, y + texHeight, 0, 1, color, 1.f, 1.f };
				
					y += texHeight;
				}

				x += texWidth;
				y = yStart;
			}

			m_repeats = verRepeats * horRepeats;

			vertexSize = sizeof(VERTEX_2D_TEX) * m_repeats * 4;
		} break;
	}

	if (vertexSize <= 0)
	{
		Logger::Error("Vertex size is too small!");

#ifdef THROW_EXCEPTION
		throw std::runtime_error("Vertex size is too small!");
#else
		return false;
#endif
	}

	assert(vertices);

	if (!vertices)
		return false;

	HRESULT result = s_pRenderer->GetDevice()->CreateVertexBuffer(vertexSize, D3DUSAGE_DYNAMIC, VERTEX_2D_TEX::FVF, D3DPOOL_DEFAULT, &m_pVB, 0);

	if (FAILED(result))
	{
		if (vertices)
			delete vertices;
	
		const char *error = FTW::GetDXErrorMsg(result);

		Logger::Error("Cannot create Vertex Buffer! %s", error);

#ifdef THROW_EXCEPTION
		throw std::runtime_error(error);
#else
		return false;
#endif

		delete[] error;
	}

	void* pVoid;
	result = m_pVB->Lock(0, 0, (void**)&pVoid, 0);
	
	if (FAILED(result))
	{
		if (vertices)
			delete vertices;

		Destroy();

		const char *error = FTW::GetDXErrorMsg(result);

		Logger::Error("Cannot Lock Vertex Buffer! %s", error);

#ifdef THROW_EXCEPTION
		throw std::runtime_error(error);
#else
		return false;
#endif

		delete[] error;
	}
	
	memcpy(pVoid, vertices, vertexSize);
	m_pVB->Unlock();

	if (vertices)
		delete vertices;



	return true;
}

void FTW::Background::Destroy()
{
	if (m_pVB)
	{
		m_pVB->Release();
		m_pVB = nullptr;
	}
}

void FTW::Background::Render()
{
	IDirect3DDevice9 *device = Renderer::Get()->GetDevice();

	device->SetTexture(0, m_pTexture->GetTexture());

	device->SetStreamSource(0, m_pVB, 0, sizeof(VERTEX_2D_TEX));
	device->SetFVF(VERTEX_2D_TEX::FVF);

	for (unsigned i = 0; i < m_repeats; ++i)
	{
		device->DrawPrimitive(D3DPT_TRIANGLEFAN, 4 * i, 2);
	}
}
