/*
*********************************************************************
* File          : Font.cpp
* Project		: FTW
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "Font.h"

#include <Renderer.h>
#include <Common.h>
#include <Logger.h>

#include <assert.h>
#include <stdexcept>

FTW::Renderer *FTW::Font::s_pRenderer;

FTW::Font::Font()
{
	m_pFont = nullptr;

	if (!s_pRenderer)
		s_pRenderer = Renderer::Get();

	assert(s_pRenderer);
}

FTW::Font::Font(Renderer *renderer)
{
	m_pFont = nullptr;

	s_pRenderer = renderer;
	assert(s_pRenderer);
}

FTW::Font::~Font()
{
	if (m_pFont)
	{
		m_pFont->Release();
		m_pFont = nullptr;
	}
}

bool FTW::Font::Create(const char *fontName, int height)
{
	assert(s_pRenderer);

	HRESULT result = D3DXCreateFont(s_pRenderer->GetDevice(), height, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, fontName, &m_pFont);

	if (FAILED(result))
	{
		const char *error = FTW::GetDXErrorMsg(result);

		Logger::Error("Cannot create font! %s", error);

#ifdef THROW_EXCEPTION
		throw std::runtime_error(error);
#else
		return false;
#endif

		delete[] error;
	}

	m_height = height;

	return true;
}

bool FTW::Font::Create(const char *fontPath, const char *fontName, int height)
{
	assert(s_pRenderer);

	AddFontResourceExA(fontPath, FR_PRIVATE, 0);

	HRESULT result = D3DXCreateFontA(s_pRenderer->GetDevice(), height, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, fontName, &m_pFont);

	if (FAILED(result))
	{
		const char *error = FTW::GetDXErrorMsg(result);

		Logger::Error("Cannot create font! %s", error);

#ifdef THROW_EXCEPTION
		throw std::runtime_error(error);
#else
		return false;
#endif

		delete[] error;
	}

	m_height = height;

	return true;
}

bool FTW::Font::Create(void * data, int dataSize, const char * fontName, int height)
{
	unsigned long nFonts = 0;

	AddFontMemResourceEx(data, dataSize, 0, &nFonts);

	HRESULT result = D3DXCreateFontA(s_pRenderer->GetDevice(), height, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, fontName, &m_pFont);
	
	if (FAILED(result))
	{
		const char *error = FTW::GetDXErrorMsg(result);

		Logger::Error("Cannot create font! %s", error);

#ifdef THROW_EXCEPTION
		throw std::runtime_error(error);
#else
		return false;
#endif

		delete[] error;
	}

	m_height = height;

	return true;
}

void FTW::Font::Render(char *text, float x, float y, D3DCOLOR color, TextPos pos, float border, D3DCOLOR borderColor)
{
	assert(m_pFont);

	if (!m_pFont)
		return;

	RECT rect = { (int)x, (int)y, (int)x, m_height };

	if (border)
	{
		Render(text, x - border, y - border, borderColor, pos);
		Render(text, x - border, y + border, borderColor, pos);
		Render(text, x + border, y + border, borderColor, pos);
		Render(text, x + border, y - border, borderColor, pos);
	}

	if (FAILED(m_pFont->DrawText(NULL, text, -1, &rect, DT_NOCLIP | pos, color)))
		return;
}
