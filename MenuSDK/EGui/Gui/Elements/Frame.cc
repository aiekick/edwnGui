#include "../../EGui.hh"

EGUI_API bool EGuiMain::Window(int id, const char* title, bool draggable) {
    //Set window identifier & dragging state, this is set so elements can identify what window they are parent to & for end window to know if it should handle drag.
    SetWindowId(id);
    SetWindowDragability(draggable);
    SetWindowDraggingBounds(MenuPos, MenuSize);
    SetItemIdentifier(GetItemIdentifier() + 1);

    //Background
    drawList.FilledRectangle(MenuPos, MenuSize, EGuiColors.FrameBackColor, EGuiStyle.FrameRounding);

    //Top bar
    drawList.FilledRectangle(MenuPos, Vec2(MenuSize.x, 25), EGuiColors.FrameHeaderColor, EGuiStyle.FrameRounding);
    drawList.FilledRectangle(MenuPos + Vec2(0, 20), Vec2(MenuSize.x, 10), EGuiColors.FrameHeaderColor);
    
    //Outline
    drawList.Rectangle(MenuPos, MenuSize, EGuiColors.FrameBorderColor, EGuiStyle.FrameRounding);

    //Title
    drawList.Text(title, MenuPos + Vec2(50, 6), LEFT, drawList.TitleFont, false, EGuiColors.TextColor);

	return true;
}

EGUI_API bool EGuiMain::EndWindow() {
    if (GetWindowDragability()) /* Changing the *true* statement on the end makes it so it can drag off screen off = no off screen */
        MenuPos = Dragging(GetWindowId(), { GetDraggingBounds().x, GetDraggingBounds().y }, { GetWindowSize().x, GetWindowSize().y }, true);

    // Restore all stored data.
    EGui.DisableInputArea = Rect(0, 0, 0, 0);
    SetItemIdentifier(0);
    return true;
}