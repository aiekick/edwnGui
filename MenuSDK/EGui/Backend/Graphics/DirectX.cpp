// This code defines a Graphics struct and implements methods for creating a Direct3D device,
// starting and ending a scene, and cleaning up the Direct3D context.

#include "DirectX.hpp"

// Define the Graphics struct.
Graphics graphics;

// Create a Direct3D device.
bool Graphics::CreateD3D()
{
    EGui.D3D = Direct3DCreate9(D3D_SDK_VERSION);
    return (EGui.D3D != nullptr);
}

// Initialize the Direct3D presentation parameters.
void Graphics::CreateD3DPP()
{
    EGui.d3dparams.Windowed = true;
    EGui.d3dparams.SwapEffect = D3DSWAPEFFECT_DISCARD;
    EGui.d3dparams.BackBufferFormat = D3DFMT_UNKNOWN;
}

// Create a Direct3D device for rendering.
bool Graphics::CreateD3DDevice()
{
    HRESULT result = EGui.D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, EGui.hwnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING, &EGui.d3dparams, &EGui.Device);
    if (FAILED(result) || !EGui.Device) {
        EGui.D3D->Release();
        return false;
    }

    /* success, set default states, from imgui */
    EGui.Device->SetPixelShader(nullptr);
    EGui.Device->SetVertexShader(nullptr);
    EGui.Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    EGui.Device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
    EGui.Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    EGui.Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    EGui.Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    EGui.Device->SetRenderState(D3DRS_ZENABLE, FALSE);
    EGui.Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    EGui.Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
    EGui.Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    EGui.Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    EGui.Device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
    EGui.Device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE);
    EGui.Device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_INVSRCALPHA);
    EGui.Device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
    EGui.Device->SetRenderState(D3DRS_FOGENABLE, FALSE);
    EGui.Device->SetRenderState(D3DRS_RANGEFOGENABLE, FALSE);
    EGui.Device->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
    EGui.Device->SetRenderState(D3DRS_STENCILENABLE, FALSE);
    EGui.Device->SetRenderState(D3DRS_CLIPPING, TRUE);
    EGui.Device->SetRenderState(D3DRS_LIGHTING, FALSE);
    EGui.Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    EGui.Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    EGui.Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    EGui.Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    EGui.Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    EGui.Device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    EGui.Device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
    EGui.Device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
    EGui.Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    EGui.Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

    return true;
}

// Begin a scene for rendering.
bool Graphics::BeginScene()
{
    if (EGui.NoWindowHeader)
        EGui.Device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, NULL, 1.0f, 0);
    else
    {
        /* Does not support transparency */
        EGui.Device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_RGBA(15, 15, 15, NULL), 1.f, 0);
    }

    return EGui.Device->BeginScene();
}

// End the current scene.
bool Graphics::EndScene()
{
    /* custom cursor example */
    //renderer.Sprite(renderer.MouseTexture, Input.GetMousePos(), Vec2(10, 15));
    return EGui.Device->EndScene();
}

// Present the current scene to the screen.
bool Graphics::Present()
{
    return EGui.Device->Present(NULL, NULL, NULL, NULL);
}

// Clean up the Direct3D context.
void Graphics::Cleanup()
{
    renderer.ReleaseObjects();
    EGui.Device->Release();
    EGui.D3D->Release();
}

// Reset the Direct3D device.
bool Graphics::ResetDevice() {
    return EGui.Device->Reset(&EGui.d3dparams) && CreateD3DDevice();
}

// Used or WndProc for window updates.
void Graphics::Update(LPARAM lParam) {
    EGui.d3dparams.BackBufferWidth = LOWORD(lParam);
    EGui.d3dparams.BackBufferHeight = HIWORD(lParam);
    ResetDevice();

    renderer.ReleaseObjects();
    renderer.Init();
}