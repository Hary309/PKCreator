/*
*********************************************************************
* File          : Texture.cpp
* Project		: FTW
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#include "Texture.h"

#include <Renderer.h>
#include <Common.h>
#include <Logger.h>

#include <assert.h>
#include <stdexcept>
#include <string>

FTW::Renderer *FTW::Texture::s_pRenderer;

FTW::Texture::Texture()
{
	if (!s_pRenderer)
		s_pRenderer = Renderer::Get();

	assert(s_pRenderer);
}

FTW::Texture::Texture(Renderer *renderer)
{
	s_pRenderer = renderer;
}

FTW::Texture::~Texture()
{
	if (m_pTexture)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}
}

bool FTW::Texture::Create(const char *filePath, unsigned width, unsigned height, unsigned colorKey, unsigned totalFrames, unsigned totalRows)
{
	Logger::Msg("Loading texture from file \"%s\" %dx%d", filePath, width, height);

	assert(s_pRenderer);

	HRESULT result = D3DXCreateTextureFromFileEx(s_pRenderer->GetDevice(), filePath, width, height, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, colorKey, 0, 0, &m_pTexture);

	if (FAILED(result))
	{
		const char *error = FTW::GetDXErrorMsg(result);

		Logger::Error("Failed to open file \"%s\". %s", filePath, error);

#ifdef THROW_EXCEPTION
		throw std::runtime_error(error);
#else
		return false;
#endif

		delete[] error;
	}

	assert(m_pTexture);

	// if it isn't anim
	if (totalFrames == 0)
	{
		m_frameWidth = (float)width;
		m_frameHeight = (float)height;
	}
	else if (totalFrames > 0) // if it is anim
	{
		m_animation = true;

		m_nTotalFrames = totalFrames;
		m_nTotalRows = totalRows == 0 ? 1 : totalRows;

		m_frameWidth = (float)width / m_nTotalFrames;
		m_frameHeight = (float)height / m_nTotalRows;
	}

	return true;
}

bool FTW::Texture::Create(void *data, int dataSize, const char *fileName, unsigned width, unsigned height, unsigned colorKey, unsigned totalFrames, unsigned totalRows)
{
	Logger::Msg("Loading texture from memory \"%s\" %dx%d", fileName, width, height);

	if (!data)
	{
		char error[256] = { 0 };

		sprintf_s(error, "Invalid data!");

		Logger::Error(error);

#ifdef THROW_EXCEPTION
		throw std::runtime_error(error);
#else
		return false;
#endif
	}

	assert(s_pRenderer);

	if (dataSize <= 0)
	{
		char error[256] = { 0 };

		sprintf_s(error, "Data size is too small! (%d)", dataSize);

		Logger::Error(error);

#ifdef THROW_EXCEPTION
		throw std::runtime_error(error);
#else
		return false;
#endif
	}

	HRESULT result = D3DXCreateTextureFromFileInMemoryEx(s_pRenderer->GetDevice(), data, dataSize, width, height, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, colorKey, 0, 0, &m_pTexture);
	
	if (FAILED(result))
	{
		const char *error = FTW::GetDXErrorMsg(result);

		Logger::Error("Failed to open file \"%s\". %s", fileName, error);

#ifdef THROW_EXCEPTION
		throw std::runtime_error(error);
#else
		return false;
#endif

		delete[] error;
	}

	assert(m_pTexture);

	// if it isn't anim
	if (totalFrames == 0)
	{
		m_frameWidth = (float)width;
		m_frameHeight = (float)height;
	}
	else if (totalFrames > 0) // if it is anim
	{
		m_animation = true;

		m_nTotalFrames = totalFrames;
		m_nTotalRows = totalRows == 0 ? 1 : totalRows;

		m_frameWidth = (float)width / m_nTotalFrames;
		m_frameHeight = (float)height / m_nTotalRows;
	}

	return true;
}
