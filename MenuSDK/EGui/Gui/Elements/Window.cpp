#include "../../EGui.hpp"

static std::unordered_map<int, bool> outline_up;
static std::unordered_map<int, float> outline_alpha;
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
    SetWindowDraggingBounds(MenuPos[WindowId], MenuSize[WindowId]);
    SetItemIdentifier(GetItemIdentifier() + 1);

    //Background
    renderer.FilledRectangle(MenuPos[WindowId], MenuSize[WindowId], EGuiColors.FrameBackColor, EGuiStyle.FrameRounding);

    //Top bar
    renderer.FilledRectangle(MenuPos[WindowId], Vec2(MenuSize[WindowId].x, 25), EGuiColors.FrameHeaderColor, EGuiStyle.FrameRounding);
    renderer.FilledRectangle(MenuPos[WindowId] + Vec2(0, 20), Vec2(MenuSize[WindowId].x, 10), EGuiColors.FrameHeaderColor);
    
    float delta_time = timing.getDeltaTime();

    if (IsResizing(GetWindowId()) && outline_up[GetItemIdentifier()])
        outline_alpha[GetWindowId()] = clamp(Animations.lerp(outline_alpha[GetWindowId()], 255.f, delta_time * 4), 0.f, 255.f);
    else
        outline_alpha[GetWindowId()] = clamp(Animations.lerp(outline_alpha[GetWindowId()], 0.f, delta_time * 4), 0.f, 255.f);

    if (outline_alpha[GetWindowId()] <= 10.f) //min alpha
        outline_up[GetWindowId()] = !outline_up[GetWindowId()];
    else if (outline_alpha[GetWindowId()] >= 254.f) // max alpha
        outline_up[GetWindowId()] = !outline_up[GetWindowId()];

    //Outline
    renderer.Rectangle(MenuPos[WindowId], MenuSize[WindowId], EGuiColors.FrameBorderColor, EGuiStyle.FrameRounding);

    renderer.PushAlpha(outline_alpha[GetWindowId()]);
    renderer.Rectangle(MenuPos[WindowId], MenuSize[WindowId], EGuiColors.MenuTheme, EGuiStyle.FrameRounding);
    renderer.PopAlpha();

    //Title
    renderer.Text(renderer.TitleFont, title, MenuPos[WindowId] + Vec2(20, 7), EGuiColors.TextColor, LEFT);

    //Resizing area.
    SetResizingArea(GetWindowId(), MenuPos[WindowId] + MenuSize[WindowId] - Vec2(12, 12), { 12, 12 });
    renderer.Line(MenuPos[WindowId] + MenuSize[WindowId] - Vec2(12, 0), MenuPos[WindowId] + MenuSize[WindowId] - Vec2(0, 12), EGuiColors.FrameBorderColor);

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
    DisableInputArea = Rect(0, 0, 0, 0);
    SetItemIdentifier(0);
    return true;
}