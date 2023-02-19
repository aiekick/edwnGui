#pragma once
#include "../../../EGui.hh"

struct d2dWrapper {
    void BeginDraw();
    void EndDraw();
    void ClearScreen(Color clr);
};

extern d2dWrapper D2DWrapper;