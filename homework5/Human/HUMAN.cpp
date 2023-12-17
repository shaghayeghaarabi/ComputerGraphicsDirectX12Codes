

#include "d3dUtility.h"


IDirect3DDevice9* Device = 0; 
IDirect3DDevice9* Device1 = 0;
const int Width  = 640;
const int Height = 480;

ID3DXMesh* Objects[16] = {0, 0, 0, 0,0};
D3DXMATRIX  Worlds[16];
D3DMATERIAL9 Mtrls[16];
IDirect3DTexture9* Tex = 0;
// vertex buffer
IDirect3DVertexBuffer9* VB = 0;

// Vertex structure for planes
struct Vertex {
	Vertex() {}

	Vertex(float x, float y, float z,
		float nx, float ny, float nz,
		float u, float v) {
		_x = x;
		_y = y;
		_z = z;
		_nx = nx;
		_ny = ny;
		_nz = nz;
		_u = u;
		_v = v;
	}

	float _x, _y, _z;
	float _nx, _ny, _nz;
	float _u, _v;

	static const DWORD FVF;
};

const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

//
// Framework Functions
//
bool Setup()
{
	//
	// Create objects.
	//

	D3DXCreateSphere(Device, 0.90f, 20, 20, &Objects[0], 0);//haed
	D3DXCreateSphere(Device,0.10f, 20, 20, &Objects[1], 0);//left eye
	D3DXCreateSphere(Device, 0.1f, 20, 20, &Objects[2], 0);//right eye
	D3DXCreateTorus(Device, 0.25f, 0.02f, 20, 20, &Objects[3], 0);//mouth
	D3DXCreateSphere(Device, 0.2f, 20, 20, &Objects[4], 0);
	D3DXCreateSphere(Device, 0.2f, 20, 20, &Objects[5], 0);
	D3DXCreateCylinder(Device, 0.5f, 0.5f, 1.0f, 20, 20, &Objects[6], 0);//body
	D3DXCreateCylinder(Device, 0.1f, 0.1f, 0.5f, 20, 20, &Objects[7], 0);//neck
	D3DXCreateCylinder(Device, 0.5f, 1.0f, 1.0f, 20, 20, &Objects[8], 0);//daman
	D3DXCreateCylinder(Device, 0.1f, 0.1f, 2.0f, 20, 20, &Objects[9], 0);//left leg
	D3DXCreateCylinder(Device, 0.1f, 0.1f, 2.0f, 20, 20, &Objects[10], 0);//right legh
	D3DXCreateCylinder(Device, 0.05f, 0.05f, 0.8f, 20, 20, &Objects[11], 0);//left hand
	D3DXCreateCylinder(Device, 0.05f, 0.05f, 0.8f, 20, 20, &Objects[12], 0);//right hand
	D3DXCreateBox(Device, 1000.0, 1000.0, 0, &Objects[13],0);// erth
	D3DXCreateBox(Device, 1000.0, 1000.0, 0, &Objects[14],0);//sky
	// Build world matrices - position the objects in world space.
	//

	D3DXMatrixTranslation(&Worlds[0],  0.0f,  2.0f, 0.0f);
	D3DXMatrixTranslation(&Worlds[1],  0.5f, 2.5f, -0.5f);
	D3DXMatrixTranslation(&Worlds[2], -0.5f,  2.5f, -0.5f);
	D3DXMatrixTranslation(&Worlds[3],  0.0f,  2.0f, -0.75f);
	D3DXMatrixTranslation(&Worlds[4], 0.5f, 0.8f, 0.0f);
	D3DXMatrixTranslation(&Worlds[5], -0.5f, 0.8f, 0.0f);
	D3DXMatrixTranslation(&Worlds[6], 0.0f, 0.3f, 0.0f);
	D3DXMatrixTranslation(&Worlds[7], 0.0f, 1.0f, 0.0f);
	D3DXMatrixTranslation(&Worlds[8], 0.0f, -0.50f, 0.0f);
	D3DXMatrixTranslation(&Worlds[9], 0.5f, -1.50f, 0.0f);
	D3DXMatrixTranslation(&Worlds[10], -0.5f, -1.50f, 0.0f);
	D3DXMatrixTranslation(&Worlds[11], 0.6f, 0.4f, 0.0f);
	D3DXMatrixTranslation(&Worlds[12], -0.6f, 0.4f, 0.0f);
	D3DXMatrixTranslation(&Worlds[13], -0.6f, -2.5f, 0.0f);
	D3DXMatrixTranslation(&Worlds[14], -0.6f, 10.0f, 0.0f);
	//
	// Setup the object's materials.
	//

	
	Mtrls[0] = d3d::WHITE_MTRL;
	Mtrls[1] = d3d::BLUE_MTRL;
	Mtrls[2] = d3d::BLUE_MTRL;
	Mtrls[3] = d3d::BLACK_MTRL;
	Mtrls[4] = d3d::BLUE_MTRL;
	Mtrls[5] = d3d::BLUE_MTRL;
	Mtrls[6] = d3d::YELLOW_MTRL;
	Mtrls[7] = d3d::YELLOW_MTRL;
	Mtrls[8] = d3d::RED_MTRL;
	Mtrls[9] = d3d::WHITE_MTRL;
	Mtrls[10] = d3d::WHITE_MTRL;
	Mtrls[11] = d3d::WHITE_MTRL;
	Mtrls[12] = d3d::WHITE_MTRL;
	Mtrls[13]= d3d::GREEN_MTRL;
	Mtrls[14] = d3d::BLUE_MTRL;
	

	D3DXVECTOR3 dir(1.0f, -0.0f, 0.25f);
	D3DXCOLOR   c = d3d::WHITE;
	D3DLIGHT9 dirLight = d3d::InitDirectionalLight(&dir, &c);
	
	//
	// Set and Enable the light.
	//

	Device->SetLight(0, &dirLight);
	Device->LightEnable(0, true);
Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	Device->SetRenderState(D3DRS_SPECULARENABLE, false);

	//
	// Set lighting related render states.
	//
	D3DXCreateTextureFromFile(
		Device,
		L"crate.jpg",
		&Tex);
	Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR); 
	Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR); 
	Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR); 

	//
	// Set the projection matrix.
	//

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
			&proj,
			D3DX_PI * 0.25f, // 45 - degree
			(float)Width / (float)Height,
			1.0f,
			1000.0f);
	Device->SetTransform(D3DTS_PROJECTION, &proj);

	return true;
}

void Cleanup()
{
	for (int i = 0; i < 16; i++) {
		d3d::Release<ID3DXMesh*>(Objects[i]);
	}
	d3d::Release<IDirect3DTexture9*>(Tex);

}

bool Display(float timeDelta)
{
	if( Device )
	{
		D3DXMATRIX Rx;
		D3DXMATRIX Ry;
		// 
		// Update the scene: update camera position.
		//


		static float angle  = (3.0f * D3DX_PI) / 2.0f;
		static float height = 5.0f;
	
		if( ::GetAsyncKeyState(VK_LEFT) & 0x8000f )
			angle -= 0.5f * timeDelta;

		if( ::GetAsyncKeyState(VK_RIGHT) & 0x8000f )
			angle += 0.5f * timeDelta;

		if( ::GetAsyncKeyState(VK_UP) & 0x8000f )
			height += 5.0f * timeDelta;

		if( ::GetAsyncKeyState(VK_DOWN) & 0x8000f )
			height -= 5.0f * timeDelta;

		D3DXVECTOR3 position( cosf(angle) * 7.0f, height, sinf(angle) * 7.0f );
		D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
		D3DXMATRIX V;
		D3DXMatrixLookAtLH(&V, &position, &target, &up);

		Device->SetTransform(D3DTS_VIEW, &V);

		//
		// Draw the scene:
		//
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
		Device->BeginScene();

		for(int i = 0; i < 6; i++)
		{
			// set material and world matrix for ith object, then render
			// the ith object.
			Device->SetMaterial(&Mtrls[i]);
			Device->SetTransform(D3DTS_WORLD, &Worlds[i]);
			Objects[i]->DrawSubset(0);
		}
		for (int i = 6; i < 15; i++) {
			D3DXMatrixRotationX(&Rx, 3.14f / 2.0f);
			D3DXMATRIX p = Rx * Worlds[i];
			Device->SetMaterial(&Mtrls[i]); 
			Device->SetTransform(D3DTS_WORLD, &p);
			Objects[i]->DrawSubset(0);
		}
		Device->SetTexture(1, Tex);
		Objects[14]->DrawSubset(0); 
		Device->EndScene();
		Device->Present(0, 0, 0, 0);
	}
	return true;
}


//
// WndProc
//
LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;
		
	case WM_KEYDOWN:
		if( wParam == VK_ESCAPE )
			::DestroyWindow(hwnd);
		break;
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

//
// WinMain
//
int WINAPI WinMain(HINSTANCE hinstance,
				   HINSTANCE prevInstance, 
				   PSTR cmdLine,
				   int showCmd)
{
	if(!d3d::InitD3D(hinstance,
		Width, Height, true, D3DDEVTYPE_HAL, &Device))
	{
		::MessageBox(0, L"InitD3D() - FAILED", 0, 0);
		return 0;
	}
		
	if(!Setup())
	{
		::MessageBox(0, L"Setup() - FAILED", 0, 0);
		return 0;
	}

	d3d::EnterMsgLoop( Display );

	Cleanup();

	Device->Release();

	return 0;
}