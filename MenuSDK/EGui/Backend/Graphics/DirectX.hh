#pragma once
#include "../../EGui.hh"

struct Graphics {
	EGUI_API void Update(LPARAM lParam);
	EGUI_API bool CreateD3D();
	EGUI_API void CreateD3DPP();
	EGUI_API bool CreateD3DDevice();
	EGUI_API bool BeginScene();
	EGUI_API bool EndScene();
	EGUI_API bool Present();
	EGUI_API bool ResetDevice();
	EGUI_API void Cleanup();

	/* Shortcuts to make code shorter and for ease of use */
	EGUI_API bool Begin() {
		BeginScene();
		return true;
	}

	EGUI_API bool End() {
		auto h = EndScene();
	    h = Present();

		return h;
	}
};

extern Graphics graphics;