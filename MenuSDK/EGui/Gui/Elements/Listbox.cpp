#include "../../EGui.hpp"

struct ListBoxData {
	std::string search;
	int selected;

	struct AlphaSettings {
		float alpha;
	};

	std::map<int, AlphaSettings> alpha_settings;
};

std::map<int, ListBoxData> list_box_data;

bool EGuiMain::ListBox(const char* title, int* selected, std::vector<std::string> options) {
	SetItemIdentifier(GetItemIdentifier() + 1);

	Vec2 Pos = NextDrawPos + Vec2(12 + EGuiStyle.Padding, 0);
	Vec2 Size = { GetChildSize().x - ((12 + EGuiStyle.Padding) * 2 + EGuiStyle.Padding), 18 };
	
	bool should_render = Input.IsRectInRect(NextDrawPos, Size, Vec2(GetChildArea().x, GetChildArea().y), Vec2(GetChildArea().w, GetChildArea().h));

	Vec2 OriginalPos = GetNextDrawPos();
	SetNextDrawPosEx({ 12 + EGuiStyle.Padding, 0 });

	if (should_render) {
		//Handle input
		for (int i = 0; i < options.size(); i++) {
			if (Input.ButtonBehaviour(Pos + Vec2(0, Size.y * (i + 1)), Size, PRESS))
				*selected = i;
		}

		//Render element
		renderer.BorderedRectangle(Pos, Size, EGuiColors.ElementBackColor, EGuiColors.ElementBorderColor, EGuiStyle.ElementRounding, CORNER_TOP);
		renderer.Text(Fonts.Primary, title, Pos + Vec2(6, 3), EGuiColors.TextColor, LEFT);

		renderer.BorderedRectangle(Pos + Vec2(0, Size.y), { Size.x, Size.y * options.size() }, EGuiColors.ElementBackColor, EGuiColors.ElementBorderColor, EGuiStyle.ElementRounding, CORNER_BOTTOM);
		for (int i = 0; i < options.size(); i++) {
			renderer.Text(Fonts.Primary, options[i].c_str(), Pos + Vec2(6, 3 + (Size.y * (i + 1))), *selected == i ? EGuiColors.MenuTheme : EGuiColors.TextColor, LEFT);

			if (*selected == i) {
				renderer.FilledRectangle(Pos + Vec2(1, Size.y * (i + 1)), { 2, Size.y }, EGuiColors.MenuTheme);
				renderer.Gradient(Pos + Vec2(2, Size.y * (i + 1)), { Size.x - 3, Size.y }, EGuiColors.MenuTheme.OverrideAlpha(75), EGuiColors.MenuTheme.OverrideAlpha(0));
			}
		}
	}

	SetNextDrawPos(OriginalPos);
	SetNextDrawPosEx({0, (Size.y * (options.size() + 1)) + EGuiStyle.Padding});

	return true;
}