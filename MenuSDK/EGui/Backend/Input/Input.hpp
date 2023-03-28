#pragma once
#include "../../EGui.hpp"

enum input_type {
	HOLD,
	PRESS
};

struct EInput {
	bool IsPointInRect(Vec2 point, Vec2 pos, Vec2 size);
	bool IsRectInRect(Vec2 pos, Vec2 size, Vec2 pos2, Vec2 size2);
	bool IsMouseHoveringRect(Vec2 pos, Vec2 size);
	bool ButtonBehaviour(Vec2 pos, Vec2 size, int button_type, char key = VK_LBUTTON);
	bool IsKeyDown(int key);
	bool IsKeyPressed(int key);

	Vec2 MousePos = {};
	void UpdateMousePos(const Vec2 point);
	Vec2 GetMousePos();

	float MouseWheelDelta = 0.f;
	void UpdateMouseWheelDelta(float delta);
	float GetMouseWheelDelta();
};

extern EInput Input;