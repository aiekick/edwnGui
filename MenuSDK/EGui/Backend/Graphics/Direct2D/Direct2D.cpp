#include "Direct2D.h"
d2dWrapper D2DWrapper;

void d2dWrapper::BeginDraw()
{
    EGui.m_pRenderTarget->BeginDraw();
}

void d2dWrapper::EndDraw()
{
    EGui.m_pRenderTarget->EndDraw();
}

void d2dWrapper::ClearScreen(Color clr)
{
    EGui.m_pRenderTarget->Clear(D2D1::ColorF(clr.r() / 255.f, clr.g() / 255.f, clr.b() / 255.f, clr.a() / 255.f));
}