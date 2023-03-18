#include "../../EGui.hpp"

bool EGuiMain::Keybind(const char* id, int& key)
{
	static std::unordered_map<const char*, bool> binding;

	std::string keystr = key == NULL ? "..." : keys_list[key].data();

	Vec2 Area = renderer.GetTextSize(renderer.Verdana, ("[" + keystr + "]").c_str());
	Vec2 Pos = { (GetChildPos().x + GetChildSize().x - EGuiStyle.Padding * 2) - Area.x, PreviousDrawPos.y};

	if (Input.ButtonBehaviour(Pos, Area, PRESS))
		binding[id] = !binding[id];

	if (binding[id]) {
		if (Input.IsKeyPressed(VK_ESCAPE) || Input.IsKeyPressed(VK_RETURN)) {
			binding[id] = !binding[id];
			key = NULL;
		}

		if (!Input.IsKeyPressed(VK_LBUTTON)) {
			for (int i = 0; i < 256; i++) {
				if (Input.IsKeyPressed(i)) {
					key = i;
					binding[id] = false;
				}
			}
		}
	}

	renderer.Text(renderer.Verdana, ("[" + (key == NULL ? "..." : keystr) + "]").c_str(), Pos + Vec2(EGuiStyle.Padding, 0), binding[id] ? EGuiColors.MenuTheme : EGuiColors.TextColor, LEFT);

	return true;
}