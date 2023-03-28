#include "../../EGui.hpp"

enum KeyMode {
	HOLD_ON = 0,
	HOLD_OFF,
	TOGGLE,
	ALWAYS
};

struct KeybindInfo {
	KeybindInfo() { key = 0, key_mode = 0; open = false;  binding = false; }
	int key;
	int key_mode;
	bool open;

	//state of keybind.
	bool binding;
};

struct KeybindMenu {
	const char* title;
	int key_mode;
	Vec2 pos;
	Vec2 size;
};

static std::map<int, KeybindMenu> keybind_menu; //used to display the menu
static std::map<int, KeybindInfo> keybind_info;

bool EGuiMain::Keybind(const char* title, bool* key_state)
{
	SetItemIdentifier(GetItemIdentifier() + 1);

	std::string keystr = keybind_info[GetItemIdentifier()].key == NULL ? "..." : keys_list[keybind_info[GetItemIdentifier()].key].data();

	Vec2 Area = renderer.GetTextSize(Fonts.Primary, ("[" + keystr + "]").c_str());
	Vec2 Pos = { (GetChildPos().x + GetChildSize().x - EGuiStyle.Padding * 2 - 12) - Area.x, PreviousDrawPos.y};
	static Vec2 Size = { 60, 60 };

	if (Input.ButtonBehaviour(Pos, Area, PRESS) && Input.IsMouseHoveringRect(Vec2(GetChildArea().x, GetChildArea().y), Vec2(GetChildArea().w, GetChildArea().h)))
		keybind_info[GetItemIdentifier()].binding = !keybind_info[GetItemIdentifier()].binding;

	if (keybind_info[GetItemIdentifier()].binding) {
		if (Input.IsKeyPressed(VK_ESCAPE) || Input.IsKeyPressed(VK_RETURN)) {
			keybind_info[GetItemIdentifier()].binding = !keybind_info[GetItemIdentifier()].binding;
			keybind_info[GetItemIdentifier()].key = NULL;
		}

		if (!Input.IsKeyPressed(VK_LBUTTON)) {
			for (int i = 0; i < 256; i++) {
				if (Input.IsKeyPressed(i)) {
					keybind_info[GetItemIdentifier()].key = i;
					keybind_info[GetItemIdentifier()].binding = false;
				}
			}
		}
	}

	//used to open/close the display for key mode.
	if (Input.ButtonBehaviour(Pos, Area, PRESS, VK_RBUTTON) && Input.IsMouseHoveringRect(Vec2(GetChildArea().x, GetChildArea().y), Vec2(GetChildArea().w, GetChildArea().h)))
		keybind_info[GetItemIdentifier()].open = !keybind_info[GetItemIdentifier()].open;
	else if ((keybind_info[GetItemIdentifier()].open && (Input.IsKeyPressed(VK_LBUTTON) && !Input.IsMouseHoveringRect(Pos + Vec2(Area.x + 10, 0), Size))) || !Input.IsRectInRect(NextDrawPos, Size, Vec2(GetChildArea().x, GetChildArea().y), Vec2(GetChildArea().w, GetChildArea().h))) 
		keybind_info[GetItemIdentifier()].open = false;

	if (keybind_info[GetItemIdentifier()].open) {
		KeybindMenu temp_menu;
		temp_menu.title = title;
		temp_menu.key_mode = keybind_info[GetItemIdentifier()].key_mode;
		temp_menu.pos = Pos + Vec2(Area.x + 10, 0);
		temp_menu.size = Size;

		keybind_menu[keybind_menu.size() + 1] = temp_menu;
		
		Vec2 TextSize = renderer.GetTextSize(Fonts.Primary, "HOLD OFF");

		for (int u = 0; u <= KeyMode::ALWAYS; u++)
		{
			if (Input.ButtonBehaviour(Pos + Vec2(Area.x + 10, 3 + ((TextSize.y + 2) * u)), { Size.x, TextSize.y }, PRESS)) {
				keybind_info[GetItemIdentifier()].key_mode = u;
				keybind_info[GetItemIdentifier()].open = false;
			}
		}

		SetDisableInputArea(Pos + Vec2(Area.x + 10, 0), Size);
	}

	if (!keybind_info[GetItemIdentifier()].binding) {
		switch (keybind_info[GetItemIdentifier()].key_mode) {
		case HOLD_ON:
			*key_state = Input.IsKeyDown(keybind_info[GetItemIdentifier()].key);
			break;
		case HOLD_OFF:
			*key_state = !Input.IsKeyDown(keybind_info[GetItemIdentifier()].key);
			break;
		case TOGGLE:
			if (Input.IsKeyPressed(keybind_info[GetItemIdentifier()].key))
				*key_state = !(*key_state);
			break;
		case ALWAYS:
			*key_state = true;
			break;
		}
	}

	if (keybind_info[GetItemIdentifier()].key == NULL)
		*key_state = false;

	if (Input.IsRectInRect(NextDrawPos, Size, Vec2(GetChildArea().x, GetChildArea().y), Vec2(GetChildArea().w, GetChildArea().h)))
		renderer.Text(Fonts.Primary, ("[" + (keybind_info[GetItemIdentifier()].key == NULL ? "..." : keystr) + "]").c_str(), Pos + Vec2(EGuiStyle.Padding, 0), keybind_info[GetItemIdentifier()].binding ? EGuiColors.MenuTheme : EGuiColors.TextColor, LEFT);

	return true;
}

//used only when rendering text on popup.
const char* GetKeyModeState(int mode) {
	const char* key_mode = "none";

	switch (mode) {
	case HOLD_ON:
		key_mode = "hold on";
		break;
	case HOLD_OFF:
		key_mode = "hold off";
		break;
	case TOGGLE:
		key_mode = "toggle";
		break;
	case ALWAYS:
		key_mode = "always";
		break;
	}

	return key_mode;
}

void EGuiMain::RenderKeybinds() {
	for (int i = 1; i <= keybind_menu.size(); i++) {
		Vec2 Size = keybind_menu[i].size;
		Vec2 TextSize = renderer.GetTextSize(Fonts.Primary, "HOLD OFF");

		renderer.BorderedRectangle(keybind_menu[i].pos, Size, EGuiColors.ChildBgColor, EGuiColors.FrameBorderColor, EGuiStyle.ElementRounding);

		for (int u = 0; u <= KeyMode::ALWAYS; u++)
			renderer.Text(Fonts.Primary, GetKeyModeState(u), keybind_menu[i].pos + Vec2(Size.x / 2, 2 + ((TextSize.y + 2) * u)), (keybind_menu[i].key_mode == u ? EGuiColors.MenuTheme : EGuiColors.TextColor), CENTER);
	}

	//reset
	keybind_menu.clear();
}