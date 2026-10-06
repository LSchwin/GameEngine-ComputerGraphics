#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include "Math/Vector2.h"

namespace nu
{
	class Renderer
	{
	public:
		bool Initialize(const char* name, int width, int height);
		void Shutdown();

		void Clear() const;
		void Present() const;

		void SetColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255) const;
		void SetColor(float r, float g, float b, float a = 1.0f) const;

		void DrawPoint(float x, float y) const;
		void DrawFillRect(float x, float y, float w, float h) const;
		void DrawRect(float x, float y, float w, float h) const;
		void DrawLine(float x1, float y1, float x2, float y2) const;

		void DrawModel(const class Model& model, const struct Transform& transform) const;
		void DrawTexture(const class Texture& texture, float x, float y, float angle = 0.0f, float scale = 1.0f, bool flipH = false, const Vector2& origin = Vector2{ 0.5f, 0.5f }, float offset = 0.0f) const;
		void DrawTexture(const class Texture& texture, const struct Rect& source, float x, float y, float angle = 0.0f, float scale = 1.0f, bool flipH = false, const Vector2& origin = Vector2{ 0.5f, 0.5f }, float offset = 0.0f) const;

		int GetWidth() { return m_width;  }
		int GetHeight() { return m_height;  }

		void SetCamera(const Vector2& camera) { m_camera = camera; }
		void EnableCamera(bool enable = true) { m_cameraEnabled = enable; }

		friend class Text;
		friend class Texture;


	private:
		SDL_Window* m_window = nullptr;
		SDL_Renderer* m_renderer = nullptr;

		bool m_cameraEnabled = true;
		Vector2 m_camera{ 0.0f, 0.0f };

		int m_width = 0;
		int m_height = 0;
	};
}