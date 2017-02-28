/*
*********************************************************************
* File          : Sprite.h
* Project		: FTW
* Developers    : Piotr Krupa (piotrkrupa06@gmail.com)
*********************************************************************
*/

#pragma once

#include <Declspec.h>

#include <d3dx9.h>

struct ID3DXSprite;
struct IDirect3DTexture9;

namespace FTW
{
	class Renderer;
	class Texture;

	extern "C"
	{
		class DECLSPEC Sprite
		{
		private:
			static Renderer		*s_pRenderer;

			ID3DXSprite			*m_pSprite;
			Texture				*m_pTexture;
			bool				m_localTex;

			float				m_direction;

			D3DXVECTOR2			*m_pos;
			D3DXVECTOR2			*m_center;
			D3DCOLOR			m_color;

			unsigned			m_currFrame;
			unsigned			m_currRow;
			unsigned			m_tmLastFrameUpdate;
			const unsigned		m_frameSpaceUpdate = 100; // in ms
			float				m_animSpeed;

			bool				m_animInited;

			bool				m_transformByCamera;

			bool				m_animLoop;
			bool				m_animEnd; // if anim end

			// position where animation start
			unsigned			m_startFrame;
			unsigned			m_startRow;
			
			// number of frames and rows to show
			unsigned			m_nFrames;
			unsigned			m_nRows;

		private:
			void Init();

		public:
			Sprite();
			Sprite(Renderer*);
			~Sprite();


			// @local - if true texture will be automatically released
			// if false you must manually release texture
			bool Create(Texture *tex, bool local = false);

			void Destroy();

			void Pulse();
			void Render(float x, float y);
			void Render(D3DXVECTOR2 pos);

			void SetCenter(float x, float y) { *m_center = D3DXVECTOR2(x, y); }
			void SetCenter(D3DXVECTOR2 cen) { *m_center = cen; }

			void SetupAnim(bool loop, unsigned frames, unsigned rows = 1);
			void SetupAnim(bool loop, unsigned startFrame, unsigned frames, unsigned startRow = 0, unsigned rows = 1);

			// Less == faster
			void  SetAnimSpeed(float speed) { m_animSpeed = speed; }
			float GetAnimSpeed() { return m_animSpeed; }

			// set true if you want to move when Camera move
			void SetTransformByCamera(bool transform) { m_transformByCamera = transform; }

			Texture *GetTex() { return m_pTexture; }

			// in degrees
			void  SetDir(float dir) { m_direction = dir; }
			// in degrees
			float GetDir() { return m_direction; }

			void  SetColor(unsigned char a, unsigned char r = 255, unsigned char g = 255, unsigned char b = 255)
			{
				m_color = D3DCOLOR_ARGB(a, r, g, b);
			}

			void	 SetColor(D3DCOLOR col) { m_color = col; }

			D3DCOLOR GetColor() { return m_color; }
			void     GetColor(unsigned char &a, unsigned char &r, unsigned char &g, unsigned char &b)
			{
				a = unsigned char(m_color >> 24);
				r = unsigned char(m_color >> 16);
				g = unsigned char(m_color >> 8);
				b = unsigned char(m_color);
			}
		};
	}
}
