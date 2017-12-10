#pragma once
#include <GL/glew.h>

namespace GameEngine {

	// x and y data
	struct Position {
		float x;
		float y;
	};

	// red, green, blue and alpha data
	struct ColorRGBA8 {
		ColorRGBA8() : r(0), g(0), b(0), a(255) {	}
		ColorRGBA8(GLubyte R, GLubyte G, GLubyte B, GLubyte A) : r(R), g(G), b(B), a(A) {	}
		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a;
	};

	struct UV {
		float u;
		float v;
	};

	struct Vertex {
		Position position;

		// 4 bytes for RGB
		ColorRGBA8 color;

		// UV Texture coordinates
		UV uv;

		void setPosition(float x, float y) {
			position.x = x;
			position.y = y;
		}

		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
		}

		void setUV(float u, float v) {
			uv.u = u;
			uv.v = v;
		}
	};
}