#include "Camera.hpp"

ViewCamera Camera;

void ViewCamera::SetPosition(Vec ang)
{
    m_position = D3DXVECTOR3(ang.x, ang.y, ang.z);
}

void ViewCamera::Update()
{
    D3DXMATRIX viewMatrix;
    D3DXMatrixLookAtLH(&viewMatrix, &m_position, &m_lookAt, &m_upVector);
    EGui.Device->SetTransform(D3DTS_VIEW, &viewMatrix);
}