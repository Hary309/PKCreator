/*
*********************************************************************
* File          : Font.h
* Project		: FTW
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include <Declspec.h>

#include <d3dx9.h>

struct ID3DXFont;

namespace FTW
{
	extern "C"
	{
		class Renderer;

		class DECLSPEC Font
		{
		private:
			static Renderer	    *s_pRenderer;

			ID3DXFont			*m_pFont;
			int					m_height;

		public:
			enum TextPos
			{
				LEFT = DT_LEFT,
				CENTER = DT_CENTER,
				RIGHT = DT_RIGHT
			};

		public:
			Font();
			Font(Renderer *renderer);
			~Font();

			bool Create(const char *fontName, int height = 20);
			bool Create(const char *fontPath, const char *fontName, int height = 20);
			bool Create(void *data, int dataSize, const char *fontName, int height = 20);

			void Render(char *text, float x, float y, D3DCOLOR color, TextPos pos = LEFT, float border = 0, D3DCOLOR borderColor = 0);
		
			int GetHeight() { return m_height; }
		};
	}
}