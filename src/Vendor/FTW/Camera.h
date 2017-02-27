#pragma once

#include <Declspec.h>

struct D3DXMATRIX;
struct D3DXVECTOR2;

namespace FTW
{
	extern "C"
	{
		class Renderer;

		// it work only on Sprites
		class DECLSPEC Camera
		{
		private:
			static Renderer	    *s_pRenderer;

			int					m_width;
			int					m_height;
				
			D3DXVECTOR2			*m_pos;

			float				m_angle;

			D3DXVECTOR2			*m_scale;

			D3DXMATRIX			*m_matProjection;
			D3DXMATRIX			*m_matWorld;
			D3DXMATRIX			*m_matView;

		private:
			void Init();

		public:
			Camera();
			Camera(Renderer *renderer);
			~Camera();

			bool Create(int width, int height);

			void Render();

			void SetPos(float x, float y);
			void SetPos(D3DXVECTOR2 *pos);
			const D3DXVECTOR2 * const GetPos() { return m_pos; }

			void SetAngle(float angle) { m_angle = angle; }
		};
	}
}
