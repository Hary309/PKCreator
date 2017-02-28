/*
*********************************************************************
* File          : Texture.h
* Project		: FTW
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include <Declspec.h>

struct IDirect3DTexture9;

namespace FTW
{
	class Renderer;

	extern "C"
	{
		class DECLSPEC Texture
		{
			static Renderer			*s_pRenderer;

			IDirect3DTexture9		*m_pTexture;

			bool					m_animation;
			float					m_frameWidth;
			float					m_frameHeight;

			unsigned				m_nTotalFrames;
			unsigned				m_nTotalRows;

		public:
			Texture();
			Texture(Renderer*);
			~Texture();

			// @ width & height - full image size
			// @ colorKey - what color will be transparent, 0 = none
			bool Create(const char *filePath, unsigned width, unsigned height, unsigned colorKey = 0, unsigned totalFrames = 0, unsigned totalRows = 1);
			// @ width & height - full image size
			// @ colorKey - what color will be transparent, 0 = none
			bool Create(void *data, int dataSize, const char *fileName, unsigned width, unsigned height, unsigned colorKey = 0, unsigned totalFrames = 0, unsigned totalRows = 1);

			unsigned GetFrames() { return m_nTotalFrames; }
			unsigned GetRows() { return m_nTotalRows; }

			float GetFrameWidth() { return m_frameWidth; }
			float GetFrameHeight() { return m_frameHeight; }

			float GetTotalWidth()  { return m_nTotalFrames == 0 ? 1 : (m_frameWidth  * m_nTotalFrames); }
			float GetTotalHeight() { return m_nTotalRows   == 0 ? 1 : (m_frameHeight * m_nTotalRows);   }

			bool IsAnim() { return m_animation; }

			IDirect3DTexture9 *GetTexture() { return m_pTexture; }
		};
	}
}
