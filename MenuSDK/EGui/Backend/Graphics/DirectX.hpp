#pragma once
#include "../../EGui.hpp"

struct Graphics {
	void Update(LPARAM lParam);
	bool CreateD3D();
	void CreateD3DPP();
	bool CreateD3DDevice();
	bool BeginScene();
	bool EndScene();
	bool Present();
	bool ResetDevice();
	void Cleanup();

	/* Shortcuts to make code shorter and for ease of use */
	bool Begin() {
		//ShowCursor(FALSE);
		BeginScene();

		return true;
	}

	bool End() {
		auto h = EndScene();
	    h = Present();

		return h;
	}
};

extern Graphics graphics;