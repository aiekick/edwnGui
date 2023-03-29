#include "../../EGui.hpp"

static std::map<int, bool> outline_up;
static std::map<int, float> outline_alpha;
bool EGuiMain::Window(int id, const char* title, bool draggable) {
    if (!(NextWindowPos == Vec2(0, 0))) {
        MenuPos[id] = NextWindowPos;
        NextWindowPos = { 0, 0 };
    }

    if (!(NextWindowSize == Vec2(0, 0))) {
        MenuSize[id] = NextWindowSize;
        NextWindowSize = { 0, 0 };
    }

    //Set window identifier & dragging state, this is set so elements can identify what window they are parent to & for end window to know if it should handle drag.
    SetWindowId(id);
    SetWindowDragability(draggable);
    SetWindowDraggingBounds(MenuPos[id], MenuSize[id]);
    SetItemIdentifier(GetItemIdentifier() + 1);

    Vec2 Pos = MenuPos[id];
    Vec2 Size = MenuSize[id];

    //Background
    renderer.FilledRectangle(Pos, Size, EGuiColors.FrameBackColor, EGuiStyle.FrameRounding);

    //Top bar
    renderer.FilledRectangle(Pos, Vec2(MenuSize[WindowId].x, 30), EGuiColors.FrameHeaderColor, EGuiStyle.FrameRounding);

    //Outline
    renderer.Rectangle(Pos, Size, EGuiColors.FrameBorderColor, EGuiStyle.FrameRounding);

    //Title
    renderer.Text(Fonts.TitleFont, title, Pos + Vec2(EGuiStyle.Padding + 4, 7), EGuiColors.TextColor, LEFT);

    //Resizing area.
    SetResizingArea(GetWindowId(),Pos + Size - Vec2(12, 12), { 12, 12 });
    renderer.Line(Pos +Size - Vec2(12, 0),Pos + Size - Vec2(0, 12), EGuiColors.FrameBorderColor);

	return true;
}

bool EGuiMain::EndWindow() {
    RenderColorPickers();
    RenderKeybinds();
    RenderPopups();
    RenderCombos();
    RenderMultiCombos();

    MenuPos[WindowId] = Dragging(GetWindowId(), { GetDraggingBounds().x, GetDraggingBounds().y }, { GetWindowSize().x, GetWindowSize().y }, false);
    MenuSize[WindowId] = Resizing(GetWindowId(), MenuSize[WindowId]);

    // Restore all stored data.
    DisableInputArea.clear();
    SetItemIdentifier(0);
    return true;
}