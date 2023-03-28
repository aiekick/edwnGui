// This code defines a Graphics struct and implements methods for creating a Direct3D device,
// starting and ending a scene, and cleaning up the Direct3D context.

#include "DirectX.hpp"

// Define the Graphics struct.
graphics Graphics;

void graphics::SetupRenderStates(IDirect3DDevice9 *Device) {
    Device->SetPixelShader(nullptr);
    Device->SetVertexShader(nullptr);
    Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
    Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    Device->SetRenderState(D3DRS_ZENABLE, FALSE);
    Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
    Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    Device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
    Device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE);
    Device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_INVSRCALPHA);
    Device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
    Device->SetRenderState(D3DRS_FOGENABLE, FALSE);
    Device->SetRenderState(D3DRS_RANGEFOGENABLE, FALSE);
    Device->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
    Device->SetRenderState(D3DRS_STENCILENABLE, FALSE);
    Device->SetRenderState(D3DRS_CLIPPING, TRUE);
    Device->SetRenderState(D3DRS_LIGHTING, FALSE);
    Device->SetRenderState(D3DRS_SRGBWRITEENABLE, FALSE);
    Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    Device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
    Device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    Device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
    Device->SetTexture(NULL, NULL);
    Device->SetPixelShader(nullptr);
}

//Creates our DirectX9 Device and Sets up our Paramaters.
void graphics::Create() {
    EGui.D3D = Direct3DCreate9(D3D_SDK_VERSION);

    EGui.Paramaters.Windowed = TRUE;
    EGui.Paramaters.SwapEffect = D3DSWAPEFFECT_DISCARD;
    EGui.Paramaters.BackBufferFormat = D3DFMT_UNKNOWN;
    EGui.Paramaters.EnableAutoDepthStencil = TRUE;
    EGui.Paramaters.AutoDepthStencilFormat = D3DFMT_D16;
    EGui.Paramaters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    //EGui.Paramaters.PresentationInterval = D3DPRESENT_INTERVAL_ONE; //Present with VSync
    EGui.Paramaters.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES;
    EGui.Paramaters.MultiSampleQuality = 0;
    
    // create device
    if (EGui.D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, EGui.hwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &EGui.Paramaters, &EGui.Device) < 0)
        return;

    // set device render states.
    SetupRenderStates(EGui.Device);
}

// Begin a scene for rendering.
void graphics::Begin() {
    // clear our scene for next draw.
    EGui.Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_RGBA(0, 0, 0, NULL), 1.f, 0);
    EGui.Device->BeginScene();
}

// End the current scene.
void graphics::End() {
    EGui.Device->EndScene();
    EGui.Device->Present(NULL, NULL, NULL, NULL);
}

// Used or WndProc for window updates.
void graphics::OnDeviceLost(LPARAM lParam) {
    EGui.Paramaters.BackBufferWidth = LOWORD(lParam);
    EGui.Paramaters.BackBufferHeight = HIWORD(lParam);
    ResetDevice();

    renderer.Release();
    renderer.Create();
}

// Reset the Direct3D device.
void graphics::ResetDevice() {
    EGui.Device->Reset(&EGui.Paramaters);
    Create();
}

// Clean up the Direct3D context.
void graphics::Cleanup() {
    if (EGui.Device) { EGui.Device->Release(); EGui.Device = NULL; }
    if (EGui.D3D) { EGui.D3D->Release(); EGui.D3D = NULL; }
    renderer.Release();
}