#include "../../EGui.hh"

EGUI_API bool EGuiMain::Window(int id, const char* title, bool draggable) {
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
    drawList.FilledRectangle(MenuPos[WindowId], MenuSize[WindowId], EGuiColors.FrameBackColor, EGuiStyle.FrameRounding);

    //Top bar
    drawList.FilledRectangle(MenuPos[WindowId], Vec2(MenuSize[WindowId].x, 25), EGuiColors.FrameHeaderColor, EGuiStyle.FrameRounding);
    drawList.FilledRectangle(MenuPos[WindowId] + Vec2(0, 20), Vec2(MenuSize[WindowId].x, 10), EGuiColors.FrameHeaderColor);
    
    //Outline
    drawList.Rectangle(MenuPos[WindowId], MenuSize[WindowId], EGuiColors.FrameBorderColor, EGuiStyle.FrameRounding);

    //Title
    drawList.Text(title, MenuPos[WindowId] + Vec2(50, 6), LEFT, drawList.TitleFont, false, EGuiColors.TextColor);

    //Resize Area
    drawList.FilledRectangle(MenuPos[WindowId] + MenuSize[WindowId] - Vec2(10, 10), Vec2(10, 10), EGuiColors.FrameHeaderColor, 2);

	return true;
}

EGUI_API bool EGuiMain::EndWindow() {
    if (GetWindowDragability()) /* Changing the *true* statement on the end makes it so it can drag off screen off = no off screen */
        MenuPos[WindowId] = Dragging(GetWindowId(), { GetDraggingBounds().x, GetDraggingBounds().y }, { GetWindowSize().x, GetWindowSize().y }, true);

    MenuSize[WindowId] = Resizing(GetWindowId(), MenuPos[WindowId], MenuSize[WindowId]);

    // Restore all stored data.
    EGui.DisableInputArea = Rect(0, 0, 0, 0);
    SetItemIdentifier(0);
    return true;
}