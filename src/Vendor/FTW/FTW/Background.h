/*
*********************************************************************
* File          : Background.h
* Project		: FTW
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include <Declspec.h>

struct IDirect3DVertexBuffer9;

namespace FTW
{
	class Renderer;
	class Texture;

	extern "C"
	{
		class DECLSPEC Background
		{
		private:
			static Renderer			*s_pRenderer;

			Texture					*m_pTexture;
			IDirect3DVertexBuffer9	*m_pVB;

			unsigned m_repeats;

		public:
			enum TypeRepeat
			{
				NO_REPEAT = 0,
				REPEAT_HOR,
				REPEAT_VER,
				REPEAT_BOTH
			};

		public:
			Background();
			Background(Renderer*);
			~Background();

			// set color to 0 to set default (255, 255, 255, 255)
			bool Create(Texture *tex, float x = 0.f, float y = 0.f, unsigned color = 0, unsigned widthRepeat = 0, unsigned heightRepeat = 0, TypeRepeat repeat = NO_REPEAT);
			void Destroy();
			
			void Render();
		}; 
	}
}

