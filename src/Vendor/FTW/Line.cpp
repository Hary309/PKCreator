/*
*********************************************************************
* File          : Line.cpp
* Project		: FTW
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "Line.h"

#include <Renderer.h>
#include <Common.h>
#include <Logger.h>

#include <assert.h>
#include <stdexcept>

FTW::Renderer *FTW::Line::s_pRenderer;

FTW::Line::Line()
{
	if (!s_pRenderer)
		s_pRenderer = Renderer::Get();

	assert(s_pRenderer);
}

FTW::Line::Line(Renderer *renderer)
{
	s_pRenderer = renderer;

	assert(s_pRenderer);
}


FTW::Line::~Line()
{
	if (m_pLine)
	{
		m_pLine->Release();
		m_pLine = nullptr;
	}
}

bool FTW::Line::Create(unsigned color, float width)
{
	HRESULT result = D3DXCreateLine(Renderer::Get()->GetDevice(), &m_pLine);

	if (FAILED(result))
	{
		const char *error = FTW::GetDXErrorMsg(result);

		Logger::Error("Cannot create line! %s", error);

#ifdef THROW_EXCEPTION
		throw std::runtime_error(error);
#else
		return false;
#endif

		delete[] error;
	}

	m_pLine->SetWidth(width);
	m_pLine->SetPattern(0xffffffff);

	m_color = color == 0 ? D3DCOLOR_ARGB(255, 0, 0, 0) : color;

	return true;
}

void FTW::Line::Render(float x1, float y1, float x2, float y2)
{
	assert(m_pLine);

	if (m_pLine)
	{
		D3DXVECTOR2 points[2];

		points[0] = D3DXVECTOR2(x1, y1);
		points[1] = D3DXVECTOR2(x2, y2);

		m_pLine->Draw(points, 2, m_color);
	}
}

void FTW::Line::SetAntialias(bool antialias)
{
	m_pLine->SetAntialias(antialias);
}

void FTW::Line::SetWidth(float width)
{
	m_pLine->SetWidth(width);
}

float FTW::Line::GetWidth()
{
	return m_pLine->GetWidth();
}