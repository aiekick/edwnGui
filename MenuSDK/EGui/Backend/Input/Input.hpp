#pragma once
#include "../../EGui.hpp"

enum input_type
{
	HOLD,
	PRESS
};

struct EInput
{
	bool IsMouseHoveringRect(Vec2 pos, Vec2 size);
	bool ButtonBehaviour(Vec2 pos, Vec2 size, int button_type);
	bool IsKeyDown(int key);
	bool IsKeyPressed(int key);
	Vec2 GetMousePos();
};

extern EInput Input;