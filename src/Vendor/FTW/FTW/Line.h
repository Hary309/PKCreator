/*
*********************************************************************
* File          : Line.h
* Project		: FTW
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include <Declspec.h>

struct ID3DXLine;

namespace FTW
{
	class Renderer;

	extern "C"
	{
		class DECLSPEC Line
		{
		private:
			static Renderer		*s_pRenderer;

			ID3DXLine			*m_pLine;
			unsigned			m_color;

		public:
			Line();
			Line(Renderer*);
			~Line();

			// @ color - D3DCOLOR
			bool Create(unsigned color, float width = 1.f);
			void Render(float x1, float y1, float x2, float y2);

			void SetAntialias(bool);
			void SetWidth(float width);
			void SetColor(unsigned color) { m_color = color; }

			float GetWidth();
			unsigned GetColor() { return m_color; }
		};
	}
}