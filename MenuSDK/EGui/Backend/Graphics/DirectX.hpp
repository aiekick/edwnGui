#pragma once
#include "../../EGui.hpp"

struct Graphics {
	void SetupRenderStates(IDirect3DDevice9 *Device);
	void Create();

	void Begin();
	void End();

	void OnDeviceLost(LPARAM lParam);
	void ResetDevice();
	void Cleanup();
};

extern Graphics graphics;