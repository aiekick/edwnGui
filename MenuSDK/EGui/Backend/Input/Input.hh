#pragma once
#include "../../EGui.hh"

enum input_type
{
	HOLD,
	PRESS
};

struct EInput
{
	EGUI_API Vec2 GetWindowPos();
	EGUI_API Vec2 GetWindowSize();
	EGUI_API bool IsMouseHoveringRect(Vec2 pos, Vec2 size);
	EGUI_API bool ButtonBehaviour(Vec2 pos, Vec2 size, int button_type);
	EGUI_API bool IsKeyDown(int key);
	EGUI_API bool IsKeyPressed(int key);
	EGUI_API Vec2 GetMousePos();
};

extern EInput Input;