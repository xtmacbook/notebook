//----------------------------------------------------------------------------------
// File:   Clipmaps.cpp
// Author: Evgeny Makarov
// Email:  sdkfeedback@nvidia.com
// 
// Copyright (c) 2007 NVIDIA Corporation. All rights reserved.
//
// TO  THE MAXIMUM  EXTENT PERMITTED  BY APPLICABLE  LAW, THIS SOFTWARE  IS PROVIDED
// *AS IS*  AND NVIDIA AND  ITS SUPPLIERS DISCLAIM  ALL WARRANTIES,  EITHER  EXPRESS
// OR IMPLIED, INCLUDING, BUT NOT LIMITED  TO, IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL  NVIDIA OR ITS SUPPLIERS
// BE  LIABLE  FOR  ANY  SPECIAL,  INCIDENTAL,  INDIRECT,  OR  CONSEQUENTIAL DAMAGES
// WHATSOEVER (INCLUDING, WITHOUT LIMITATION,  DAMAGES FOR LOSS OF BUSINESS PROFITS,
// BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR ANY OTHER PECUNIARY LOSS)
// ARISING OUT OF THE  USE OF OR INABILITY  TO USE THIS SOFTWARE, EVEN IF NVIDIA HAS
// BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
//
//
//----------------------------------------------------------------------------------

#include <DXUT.h>
#include <DXUTcamera.h>
#include <DXUTshapes.h>
#include <DXUTgui.h>
#include <DXUTsettingsdlg.h>
#include "sdkmisc.h"
#include "Clipmaps.h"
#include "JPEG_Preprocessor.h"
#include <math.h>
#include <vector>
#include <string>

#define SOURCE_FILES_NUM 5

WCHAR g_SrcMediaPath[SOURCE_FILES_NUM][MAX_PATH] = {  L"Clipmaps//Mars16k.jpg",
                                                      L"Clipmaps//Mars8k.jpg",
                                                      L"Clipmaps//Mars4k.jpg",
                                                      L"Clipmaps//Mars2k.jpg",
                                                      L"Clipmaps//Mars1k.jpg" };
                                                      
WCHAR g_SrcMediaPathHM[SOURCE_FILES_NUM][MAX_PATH] = {  L"Clipmaps//MarsHm16k.jpg",
                                                        L"Clipmaps//MarsHm8k.jpg",
                                                        L"Clipmaps//MarsHm4k.jpg",
                                                        L"Clipmaps//MarsHm2k.jpg",
                                                        L"Clipmaps//MarsHm1k.jpg" };
                                                      
WCHAR g_DstMediaPath[SOURCE_FILES_NUM][MAX_PATH];
WCHAR g_DstMediaPathHM[SOURCE_FILES_NUM][MAX_PATH];

#define SPHERE_MERIDIAN_SLICES_NUM 128
#define SPHERE_PARALLEL_SLICES_NUM 128

#define FILE_BLOCK_SIZE 512

#define CLIPMAP_STACK_SIZE_MAX 4096
#define CLIPMAP_STACK_SIZE_MIN 1024
#define MIPMAP_LEVELS_MAX 7

enum eRenderingTechniques
{
    RENDER_TRILINEAR,
    RENDER_ANISOTROPIC,
    RENDER_COLOR_MIPS,
    TECHNIQUES_NUM,
};

const LPCSTR RENDER_TECHNIQUES[] =
{
    "Trilinear",
    "Anisotropic",
    "ColoredMips",
};

const LPCWSTR RENDER_TECHNIQUESW[] =
{
    L"Trilinear",
    L"Anisotropic",
    L"Colored mips",
};

class SelfCamera :public CModelViewerCamera
{
public:

	
	LRESULT HandleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		CBaseCamera::HandleMessages(hWnd, uMsg, wParam, lParam);

		if (((uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONDBLCLK) && m_nRotateModelButtonMask & MOUSE_LEFT_BUTTON) ||
			((uMsg == WM_MBUTTONDOWN || uMsg == WM_MBUTTONDBLCLK) && m_nRotateModelButtonMask & MOUSE_MIDDLE_BUTTON) ||
			((uMsg == WM_RBUTTONDOWN || uMsg == WM_RBUTTONDBLCLK) && m_nRotateModelButtonMask & MOUSE_RIGHT_BUTTON))
		{
			int iMouseX = (short)LOWORD(lParam);
			int iMouseY = (short)HIWORD(lParam);
			m_WorldArcBall.OnBegin(iMouseX, iMouseY);
			iMouseX_download = iMouseX;
		}

		if (((uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONDBLCLK) && m_nRotateCameraButtonMask & MOUSE_LEFT_BUTTON) ||
			((uMsg == WM_MBUTTONDOWN || uMsg == WM_MBUTTONDBLCLK) &&
				m_nRotateCameraButtonMask & MOUSE_MIDDLE_BUTTON) ||
				((uMsg == WM_RBUTTONDOWN || uMsg == WM_RBUTTONDBLCLK) && m_nRotateCameraButtonMask & MOUSE_RIGHT_BUTTON))
		{
			int iMouseX = (short)LOWORD(lParam);
			int iMouseY = (short)HIWORD(lParam);
			m_ViewArcBall.OnBegin(iMouseX, iMouseY);
			iMouseX_download = iMouseX;

		}

		if (uMsg == WM_MOUSEMOVE)
		{
			int iMouseX = (short)LOWORD(lParam);
			int iMouseY = (short)HIWORD(lParam);
			if (focusX) iMouseX = iMouseX_download;
			m_WorldArcBall.OnMove(iMouseX, iMouseY);
			m_ViewArcBall.OnMove(iMouseX, iMouseY);
		}

		if ((uMsg == WM_LBUTTONUP && m_nRotateModelButtonMask & MOUSE_LEFT_BUTTON) ||
			(uMsg == WM_MBUTTONUP && m_nRotateModelButtonMask & MOUSE_MIDDLE_BUTTON) ||
			(uMsg == WM_RBUTTONUP && m_nRotateModelButtonMask & MOUSE_RIGHT_BUTTON))
		{
			m_WorldArcBall.OnEnd();
		}

		if ((uMsg == WM_LBUTTONUP && m_nRotateCameraButtonMask & MOUSE_LEFT_BUTTON) ||
			(uMsg == WM_MBUTTONUP && m_nRotateCameraButtonMask & MOUSE_MIDDLE_BUTTON) ||
			(uMsg == WM_RBUTTONUP && m_nRotateCameraButtonMask & MOUSE_RIGHT_BUTTON))
		{
			m_ViewArcBall.OnEnd();
		}

		if (uMsg == WM_CAPTURECHANGED)
		{
			if ((HWND)lParam != hWnd)
			{
				if ((m_nRotateModelButtonMask & MOUSE_LEFT_BUTTON) ||
					(m_nRotateModelButtonMask & MOUSE_MIDDLE_BUTTON) ||
					(m_nRotateModelButtonMask & MOUSE_RIGHT_BUTTON))
				{
					m_WorldArcBall.OnEnd();
				}

				if ((m_nRotateCameraButtonMask & MOUSE_LEFT_BUTTON) ||
					(m_nRotateCameraButtonMask & MOUSE_MIDDLE_BUTTON) ||
					(m_nRotateCameraButtonMask & MOUSE_RIGHT_BUTTON))
				{
					m_ViewArcBall.OnEnd();
				}
			}
		}

		if (uMsg == WM_LBUTTONDOWN ||
			uMsg == WM_LBUTTONDBLCLK ||
			uMsg == WM_MBUTTONDOWN ||
			uMsg == WM_MBUTTONDBLCLK ||
			uMsg == WM_RBUTTONDOWN ||
			uMsg == WM_RBUTTONDBLCLK ||
			uMsg == WM_LBUTTONUP ||
			uMsg == WM_MBUTTONUP ||
			uMsg == WM_RBUTTONUP ||
			uMsg == WM_MOUSEWHEEL ||
			uMsg == WM_MOUSEMOVE)
		{
			m_bDragSinceLastUpdate = true;
		}

		return FALSE;
	}

	bool focusX = true;
	int iMouseX_download;

};

//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
int g_ClipmapStackSize = 4096;
int g_UpdateRegionSize = 64;
int g_RenderingTechnique = 1;

ID3DX10Font*          g_pFont = NULL;
ID3DX10Sprite*        g_pSprite = NULL;
CDXUTTextHelper*      g_pTxtHelper = NULL;
ID3D10Effect*         g_pEffect = NULL;
ID3D10InputLayout*    g_pVertexLayout = NULL;
ID3D10Device*         g_pD3DDevice = NULL;
ID3D10Buffer*         g_pSphereIndexBuffer;
SelfCamera    g_RotateCamera;

bool g_showHelp = false;

int        g_SourceImageWidth;
int        g_SourceImageHeight;
int        g_SourceImageMipsNum;
int**      g_ppSourceImageMipsSize = NULL;
int*       g_pMipDataOffset = NULL;     // Offset in bytes for each mip level data stored in a temporary file
D3DXVECTOR2 g_StackPosition;            // Virtual clipmap stack position (defined using normalized texture coordinates)
int        g_StackDepth;                // Number of layers in a clipmap stack

std::vector<int>    g_StackSizeList;
std::vector<int>    g_UpdateRegionSizeList;

ID3D10Texture2D*             g_pPyramidTexture      = NULL; // Texture which is used to store coarse mip levels
ID3D10ShaderResourceView*    g_pPyramidTextureSRV   = NULL;
ID3D10Texture2D*             g_pPyramidTextureHM    = NULL; // Height map for pyramid texture
ID3D10ShaderResourceView*    g_pPyramidTextureHMSRV = NULL;
ID3D10Texture2D*             g_pStackTexture        = NULL; // Clipmap stack texture
ID3D10ShaderResourceView*    g_pStackTextureSRV     = NULL;

D3DXVECTOR3 g_LightPosition;
D3DXVECTOR3 g_EyePosition;
D3DXMATRIX  g_MProjection;
D3DXMATRIX  g_MView;
D3DXMATRIX  g_MWorldViewProjection;

int **g_ppUpdatePositions;                           // Defines positions for each clipmap layer where new data should be placed

bool g_bSampleInitialized    = false;
bool g_bUseParallax          = true;
bool g_bShowStackLevels      = false;

float g_MipmapColors[MIPMAP_LEVELS_MAX][3] =
{
    {0.5f, 1.0f, 0.0f},
    {0.0f, 1.0f, 1.0f},
    {1.0f, 1.0f, 0.0f},
    {0.0f, 1.0f, 0.0f},
    {0.0f, 0.0f, 1.0f},
    {1.0f, 0.0f, 1.0f},
    {1.0f, 0.0f, 0.0f},
};

CD3DSettingsDlg                g_D3DSettingsDlg;
CDXUTDialogResourceManager     g_DialogResourceManager;
CDXUTDialog                    g_HUD;
CDXUTDialog                    g_SampleUI_Startup;
CDXUTDialog                    g_SampleUI_Runtime;

//--------------------------------------------------------------------------------------
// UI control IDs
//--------------------------------------------------------------------------------------
#define IDC_STATIC             -1
#define IDC_TOGGLEFULLSCREEN    1
#define IDC_TOGGLEREF           2
#define IDC_CHANGEDEVICE        3
#define IDC_STACK_SIZE          5
#define IDC_UPDATE_SIZE         6
#define IDC_START_SAMPLE        7
#define IDC_RENDER_TECHNIQUES   8
#define IDC_RENDER_PARALLAX     9
#define IDC_SHOW_STACK_LEVELS   10

//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
void        CalculateClipmapParameters();
HRESULT     CreateClipmapTextures( ID3D10Device* pd3dDevice );
void        InitStackTexture( ID3D10Device* pd3dDevice );
void        UpdateStackTexture( ID3D10Device* pd3dDevice );
void        UpdateMipPosition( int &position, int offset );
HRESULT     InitGUI( ID3D10Device* pd3dDevice );
void        CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
HRESULT     CreateIndexBuffer( ID3D10Device* pd3dDevice, ID3D10Buffer **ppIndexBuffer );

//--------------------------------------------------------------------------------------
// Reject any D3D10 devices that aren't acceptable by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK IsD3D10DeviceAcceptable( UINT Adapter, UINT Output, D3D10_DRIVER_TYPE DeviceType, DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
    return true;
}


//--------------------------------------------------------------------------------------
// Called right before creating a D3D9 or D3D10 device, allowing the app to modify the device settings as needed
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, void* pUserContext )
{
    // For the first device created if its a REF device, optionally display a warning dialog box
    static bool s_bFirstTime = true;
    
    if( s_bFirstTime )
    {
        s_bFirstTime = false;
        
        if( (DXUT_D3D9_DEVICE == pDeviceSettings->ver && pDeviceSettings->d3d9.DeviceType == D3DDEVTYPE_REF) ||
            (DXUT_D3D10_DEVICE == pDeviceSettings->ver && pDeviceSettings->d3d10.DriverType == D3D10_DRIVER_TYPE_REFERENCE) )
        {
            DXUTDisplaySwitchingToREFWarning( pDeviceSettings->ver );
        }
    }
    
    return true;
}


//--------------------------------------------------------------------------------------
// Create any D3D10 resources that aren't dependant on the back buffer
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D10CreateDevice( ID3D10Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
    HRESULT hr;
    
    g_bSampleInitialized = false;

    V_RETURN( g_DialogResourceManager.OnD3D10CreateDevice(pd3dDevice) );
    V_RETURN( g_D3DSettingsDlg.OnD3D10CreateDevice(pd3dDevice ) );

    V_RETURN( D3DX10CreateFont( pd3dDevice, 15, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, 
                                OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
                                L"Arial", &g_pFont ) );
    V_RETURN( D3DX10CreateSprite( pd3dDevice, 512, &g_pSprite ) );
    g_pTxtHelper = new CDXUTTextHelper( g_pFont, g_pSprite, 15 );

    V_RETURN( LoadEffect( pd3dDevice, L"Clipmaps.fx", &g_pEffect ) );
    
    for( int i = 0; i < SOURCE_FILES_NUM; i++ )
    {
        V_RETURN( DXUTFindDXSDKMediaFileCch( g_DstMediaPath[i], MAX_PATH, g_SrcMediaPath[i] ) );
        V_RETURN( DXUTFindDXSDKMediaFileCch( g_DstMediaPathHM[i], MAX_PATH, g_SrcMediaPathHM[i] ) );
    }
    
    D3DX10_IMAGE_INFO imageInfo;
    D3DX10GetImageInfoFromFile( g_DstMediaPath[0], NULL, &imageInfo, &hr );
    
    g_SourceImageWidth = imageInfo.Width;
    g_SourceImageHeight = imageInfo.Height;
    
    UINT baseDimension = CLIPMAP_STACK_SIZE_MAX;

    while( baseDimension >= CLIPMAP_STACK_SIZE_MIN )
    {
        g_StackSizeList.push_back(baseDimension);
        baseDimension >>= 1;
    }
    
    V_RETURN( CreateIndexBuffer( pd3dDevice, &g_pSphereIndexBuffer ) );
    V_RETURN( InitGUI( pd3dDevice) );
    
    D3DXVECTOR3 lookAt( 0.0f, 0.0f, 0.0f );
    g_EyePosition = D3DXVECTOR3(0.0f, 0.0f, 3.0f);

    g_RotateCamera.SetViewParams( &g_EyePosition, &lookAt );
    g_RotateCamera.SetModelCenter( D3DXVECTOR3(0.0f, 0.0f, 0.0f));
    g_RotateCamera.SetButtonMasks( 0, MOUSE_WHEEL, MOUSE_LEFT_BUTTON );
    g_RotateCamera.SetRadius( 3.0f, 1.3f, 20.0f );
    
    g_pD3DDevice = pd3dDevice;
    
    return S_OK;
}


//--------------------------------------------------------------------------------------
// Create any D3D10 resources that depend on the back buffer
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D10ResizedSwapChain( ID3D10Device* pd3dDevice, IDXGISwapChain *pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
    HRESULT hr;
    
    V_RETURN( g_D3DSettingsDlg.OnD3D10ResizedSwapChain(pd3dDevice, pBackBufferSurfaceDesc) );
    V_RETURN( g_DialogResourceManager.OnD3D10ResizedSwapChain(pd3dDevice, pBackBufferSurfaceDesc) );

    g_HUD.SetLocation( pBackBufferSurfaceDesc->Width - 170, 0 );
    g_HUD.SetSize( 170, 170 );

    g_SampleUI_Startup.SetLocation( ( pBackBufferSurfaceDesc->Width - g_SampleUI_Startup.GetWidth() ) / 2, ( pBackBufferSurfaceDesc->Height - g_SampleUI_Startup.GetHeight() ) / 2 );
    
    g_SampleUI_Runtime.SetLocation( pBackBufferSurfaceDesc->Width-170, pBackBufferSurfaceDesc->Height-300 );
    g_SampleUI_Runtime.SetSize( 170, 300 );

    float aspectRatio = (float)pBackBufferSurfaceDesc->Width / (float)pBackBufferSurfaceDesc->Height;
    g_RotateCamera.SetProjParams( D3DX_PI * 0.2f, aspectRatio, 0.01f, 1000.0f );
    g_RotateCamera.SetWindow( pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height );
    
    g_pEffect->GetVariableByName( "g_ScreenAspectRatio" )->AsScalar()->SetFloat( aspectRatio );

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( double fTime, float fElapsedTime, void* pUserContext )
{
    static float startTime = 0.0f;
    static bool renderedScene = false;
    
    if( g_bSampleInitialized == true && renderedScene == false )
        startTime = (float)fTime;

    renderedScene = g_bSampleInitialized;
    g_RotateCamera.FrameMove(fElapsedTime);
    
    g_LightPosition.x = cosf(((float)fTime - startTime) * 0.05f) * 10.0f;
    g_LightPosition.z = sinf(((float)fTime - startTime) * 0.05f) * 10.0f;
    
    g_pEffect->GetVariableByName( "g_LightPosition" )->AsVector()->SetFloatVectorArray( (float*)&g_LightPosition, 0 , 3 );
}


//--------------------------------------------------------------------------------------
// Render the help and statistics text
//--------------------------------------------------------------------------------------
void RenderText()
{
    g_pTxtHelper->Begin();
    g_pTxtHelper->SetInsertionPos( 2, 0 );
    g_pTxtHelper->SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
    g_pTxtHelper->DrawTextLine( DXUTGetFrameStats( true ) );
    g_pTxtHelper->DrawTextLine( DXUTGetDeviceStats() );    
  
    if( g_showHelp )
    {
        UINT nBackBufferHeight = DXUTGetDXGIBackBufferSurfaceDesc()->Height;
        g_pTxtHelper->SetInsertionPos( 2, nBackBufferHeight- 15 * 6 );
        g_pTxtHelper->SetForegroundColor( D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f ) );
        g_pTxtHelper->DrawTextLine( L"Controls:" );

        g_pTxtHelper->SetInsertionPos( 20, nBackBufferHeight - 15 * 5 );
        g_pTxtHelper->DrawTextLine( L"Rotate model: Left mouse button\n"
                                    L"Zoom: Mouse wheel\n");

        g_pTxtHelper->SetInsertionPos( 300, nBackBufferHeight - 15 * 5 );
        g_pTxtHelper->DrawTextLine( L"Hide help: F1\n" 
                                    L"Quit: ESC\n" );
    }
    else
    {
        g_pTxtHelper->SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
        g_pTxtHelper->DrawTextLine( L"Press F1 for help" );
    }

    g_pTxtHelper->End();
}


//--------------------------------------------------------------------------------------
// Render the scene using the D3D10 device
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D10FrameRender( ID3D10Device* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
    static float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

    if( g_D3DSettingsDlg.IsActive() )
    {
        g_D3DSettingsDlg.OnRender( fElapsedTime );
        return;
    }

    // Clear render target and the depth stencil 
    pd3dDevice->ClearRenderTargetView( DXUTGetD3D10RenderTargetView(), clearColor );
    pd3dDevice->ClearDepthStencilView( DXUTGetD3D10DepthStencilView(), D3D10_CLEAR_DEPTH, 1.0, 0 );

    if( g_bSampleInitialized )
    {
        g_MProjection = *g_RotateCamera.GetProjMatrix();

        float length = 0.0f;

        g_EyePosition = (D3DXVECTOR3)*g_RotateCamera.GetEyePt();

        length = D3DXVec3Length(&g_EyePosition);
        if( length < 1.1f )
        {
            g_EyePosition *= 1.1f / length;

            D3DXVECTOR3 lookAt(0.0f, 0.0f, 0.0f);
            g_RotateCamera.SetViewParams( &g_EyePosition, &lookAt );
        }
            
        g_MView = *g_RotateCamera.GetViewMatrix();
        
        g_MWorldViewProjection = g_MView * g_MProjection;

        g_pEffect->GetVariableByName( "g_ModelViewProj" )->AsMatrix()->SetMatrix( g_MWorldViewProjection );
        g_pEffect->GetVariableByName( "g_EyePosition" )->AsVector()->SetFloatVectorArray( (float*)&g_EyePosition, 0 , 3 );
        
        float vectorRight[3] = {g_RotateCamera.GetViewMatrix()->_11, g_RotateCamera.GetViewMatrix()->_21, g_RotateCamera.GetViewMatrix()->_31 };
        float vectorUp[3] = {g_RotateCamera.GetViewMatrix()->_12, g_RotateCamera.GetViewMatrix()->_22, g_RotateCamera.GetViewMatrix()->_32 };
            
        g_pEffect->GetVariableByName( "g_WorldRight" )->AsVector()->SetFloatVectorArray( (float*)vectorRight, 0, 3 );
        g_pEffect->GetVariableByName( "g_WorldUp" )->AsVector()->SetFloatVectorArray( (float*)vectorUp, 0, 3 );
        
        UpdateStackTexture( pd3dDevice );

        g_pEffect->GetVariableByName( "g_StackCenter" )->AsVector()->SetFloatVectorArray( (float*)g_StackPosition, 0, 2 );

        static ID3D10Buffer *pBuffers[] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
        static unsigned pStrides[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
        static unsigned pOffsets[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
        
        pd3dDevice->IASetInputLayout( NULL );
        pd3dDevice->IASetVertexBuffers(0, D3D10_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT, pBuffers, pStrides, pOffsets );
        pd3dDevice->IASetIndexBuffer( g_pSphereIndexBuffer, DXGI_FORMAT_R32_UINT, 0 );
        pd3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
        
        if( g_bUseParallax && g_RenderingTechnique < 2 )
            g_pEffect->GetTechniqueByName( RENDER_TECHNIQUES[g_RenderingTechnique] )->GetPassByIndex( 1 )->Apply( 0 );
        else
            g_pEffect->GetTechniqueByName( RENDER_TECHNIQUES[g_RenderingTechnique] )->GetPassByIndex( 0 )->Apply( 0 );
        
        pd3dDevice->DrawIndexed( SPHERE_MERIDIAN_SLICES_NUM * SPHERE_PARALLEL_SLICES_NUM * 6, 0, 0 );

        if( g_bShowStackLevels )
        {
            pd3dDevice->IASetIndexBuffer( NULL, DXGI_FORMAT_R32_UINT, 0 );
            pd3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_POINTLIST );
            g_pEffect->GetTechniqueByName( "StackDrawPass" )->GetPassByIndex( 0 )->Apply( 0 );
            pd3dDevice->Draw( g_StackDepth, 0 );
        }
        
        g_SampleUI_Runtime.OnRender( fElapsedTime );
    }
    else
    {
        g_SampleUI_Startup.OnRender( fElapsedTime );
    }

    g_HUD.OnRender( fElapsedTime );
    RenderText();
}


//--------------------------------------------------------------------------------------
// Release D3D10 resources created in OnD3D10ResizedSwapChain 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D10ReleasingSwapChain( void* pUserContext )
{
    g_DialogResourceManager.OnD3D10ReleasingSwapChain();
}


//--------------------------------------------------------------------------------------
// Release D3D10 resources created in OnD3D10CreateDevice 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D10DestroyDevice( void* pUserContext )
{
    SAFE_RELEASE( g_pFont );
    SAFE_RELEASE( g_pSprite );
    SAFE_RELEASE( g_pEffect );
    SAFE_RELEASE( g_pPyramidTexture );
    SAFE_RELEASE( g_pPyramidTextureSRV );
    SAFE_RELEASE( g_pPyramidTextureHM );
    SAFE_RELEASE( g_pPyramidTextureHMSRV );
    SAFE_RELEASE( g_pStackTexture );
    SAFE_RELEASE( g_pStackTextureSRV );
    SAFE_RELEASE( g_pVertexLayout );
    SAFE_RELEASE( g_pSphereIndexBuffer );
    
    SAFE_DELETE( g_pTxtHelper );

    g_D3DSettingsDlg.OnD3D10DestroyDevice();
    g_DialogResourceManager.OnD3D10DestroyDevice();

    g_JPEG_Manager.Release();
    
    if( g_ppSourceImageMipsSize )
    {
        for( int i = 0; i < g_SourceImageMipsNum; i++ )
        {
            delete [] g_ppSourceImageMipsSize[i];
        }
        
        delete [] g_ppSourceImageMipsSize;
        g_ppSourceImageMipsSize = NULL;
    }
    
    if( g_ppUpdatePositions )
    {
        for( int i = 0; i < g_StackDepth; i++ )
        {
            delete [] g_ppUpdatePositions[i];
        }
        
        delete [] g_ppUpdatePositions;
        g_ppUpdatePositions = NULL;
    }
    
    if( g_pMipDataOffset )
    {
        delete [] g_pMipDataOffset;
        g_pMipDataOffset = NULL;     
    }
}


//--------------------------------------------------------------------------------------
// Handle messages to the application
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, 
                          bool* pbNoFurtherProcessing, void* pUserContext )
{
    // Try to pass messages to settings dialog
    if( g_D3DSettingsDlg.IsActive() )
    {
        g_D3DSettingsDlg.MsgProc( hWnd, uMsg, wParam, lParam );
        return 0;
    }

    if( g_HUD.MsgProc( hWnd, uMsg, wParam, lParam ) )
    {
        return 0;
    }

    if( g_bSampleInitialized )
    {
        if( g_SampleUI_Runtime.MsgProc( hWnd, uMsg, wParam, lParam ) )
            return 0;
    }
    else
    {
        if( g_SampleUI_Startup.MsgProc( hWnd, uMsg, wParam, lParam ) )
            return 0;
    }

    g_RotateCamera.HandleMessages(hWnd, uMsg, wParam, lParam);
    
    return 0;
}


static float currentAgree = 0.0f;
//--------------------------------------------------------------------------------------
// Handle key presses
//--------------------------------------------------------------------------------------
void CALLBACK OnKeyboard( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
    if( !bKeyDown )
        return;

    if( nChar == VK_F1 )
    {
        g_showHelp = !g_showHelp;
    }

	if (nChar == VK_UP)
	{
		static float agree = 20.0f;

		currentAgree += agree;

		float z = cos(currentAgree * D3DX_PI / 180.0f);
		float y = sin(currentAgree * D3DX_PI / 180.0f);

		D3DXVECTOR3 lookAt(0.0f, 0.0f, 0.0f);

		g_RotateCamera.SetViewParams(&D3DXVECTOR3(0.0f, 3.0 * y, 3.0 * z), &lookAt);

		int number = 10;
		wchar_t str[256];
		swprintf(str,256, L"x: 0 y:%f z:%f \n",y,z);
		OutputDebugString(str);
	}

	if (nChar == VK_DOWN)
	{
		static float agree = 20.0f;

		currentAgree -= agree;

		float z = cos(currentAgree * D3DX_PI / 180.0f);
		float y = sin(currentAgree * D3DX_PI / 180.0f);

		D3DXVECTOR3 lookAt(0.0f, 0.0f, 0.0f);

		g_RotateCamera.SetViewParams(&D3DXVECTOR3(0.0f, 3.0 * y, 3.0 * z), &lookAt);
		int number = 10;
		wchar_t str[256];
		swprintf(str, 256, L"x: 0 y:%f z:%f \n", y, z);
		OutputDebugString(str);
	}
}


//--------------------------------------------------------------------------------------
// Handle mouse button presses
//--------------------------------------------------------------------------------------
void CALLBACK OnMouse( bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown, 
                       bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta, 
                       int xPos, int yPos, void* pUserContext )
{
}


//--------------------------------------------------------------------------------------
// Call if device was removed.  Return true to find a new device, false to quit
//--------------------------------------------------------------------------------------
bool CALLBACK OnDeviceRemoved( void* pUserContext )
{
    return true;
}


//--------------------------------------------------------------------------------------
// Initialize everything and go into a render loop
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

    // DXUT will create and use the best device (either D3D9 or D3D10) 
    // that is available on the system depending on which D3D callbacks are set below

    // Set general DXUT callbacks
    DXUTSetCallbackFrameMove( OnFrameMove );
    DXUTSetCallbackKeyboard( OnKeyboard );
    DXUTSetCallbackMouse( OnMouse );
    DXUTSetCallbackMsgProc( MsgProc );
    DXUTSetCallbackDeviceChanging( ModifyDeviceSettings );
    DXUTSetCallbackDeviceRemoved( OnDeviceRemoved );

    // Set the D3D10 DXUT callbacks. Remove these sets if the app doesn't need to support D3D10
    DXUTSetCallbackD3D10DeviceAcceptable( IsD3D10DeviceAcceptable );
    DXUTSetCallbackD3D10DeviceCreated( OnD3D10CreateDevice );
    DXUTSetCallbackD3D10SwapChainResized( OnD3D10ResizedSwapChain );
    DXUTSetCallbackD3D10FrameRender( OnD3D10FrameRender );
    DXUTSetCallbackD3D10SwapChainReleasing( OnD3D10ReleasingSwapChain );
    DXUTSetCallbackD3D10DeviceDestroyed( OnD3D10DestroyDevice );

    // IMPORTANT: set SDK media search path to include source directory of this sample, when started from .\Bin
    HRESULT hr;
    V_RETURN( DXUTSetMediaSearchPath(L"..\\Source\\Clipmaps") );
    
    // Perform any application-level initialization here

    g_D3DSettingsDlg.Init( &g_DialogResourceManager );
    
    int iY = 10; 
    g_HUD.Init( &g_DialogResourceManager );
    g_HUD.SetCallback( OnGUIEvent );
    g_HUD.AddButton( IDC_TOGGLEFULLSCREEN, L"Toggle full screen", 35, iY, 125, 22 );
    g_HUD.AddButton( IDC_TOGGLEREF, L"Toggle REF (F3)", 35, iY += 24, 125, 22, VK_F3 );
    g_HUD.AddButton( IDC_CHANGEDEVICE, L"Change device (F2)", 35, iY += 24, 125, 22, VK_F2 );

    DXUTInit( true, true, NULL ); // Parse the command line, show msgboxes on error, no extra command line params
    DXUTSetCursorSettings( true, true ); // Show the cursor and clip it when in full screen
    DXUTCreateWindow( L"Clipmaps" );
    DXUTCreateDevice( true, 640, 480 );  
    DXUTMainLoop(); // Enter into the DXUT render loop

    return DXUTGetExitCode();
}


//--------------------------------------------------------------------------------------
// Load effect from file
//--------------------------------------------------------------------------------------
HRESULT LoadEffect( ID3D10Device* pd3dDevice, WCHAR *pFileName, ID3D10Effect **ppEffect )
{
    HRESULT hr;
    WCHAR path[MAX_PATH];

    V_RETURN( DXUTFindDXSDKMediaFileCch( path, MAX_PATH, pFileName ) );
    V_RETURN( D3DX10CreateEffectFromFile( path, NULL, NULL, "fx_4_0", D3D10_SHADER_NO_PRESHADER, 0, pd3dDevice, NULL, NULL, ppEffect, NULL, &hr ) );

    return S_OK;
}


HRESULT InitGUI( ID3D10Device* pd3dDevice )
{
    g_SampleUI_Startup.Init( &g_DialogResourceManager );
    g_SampleUI_Startup.SetCallback( OnGUIEvent );
    g_SampleUI_Startup.EnableKeyboardInput( true );
    
    CDXUTComboBox* pComboBox = NULL;
    
    g_SampleUI_Startup.AddStatic( IDC_STATIC, L"Stack size: ", 0, 0, 150, 24 );
    g_SampleUI_Startup.AddStatic( IDC_STATIC, L"Larger stack size produces better image quality.", 0, 40, 350, 24 );
    g_SampleUI_Startup.AddStatic( IDC_STATIC, L"Note: Loading can take several minutes.", 50, 180, 255, 24 );
    
    g_SampleUI_Startup.AddComboBox( IDC_STACK_SIZE, 120, 0, 120, 24, ' ', false, &pComboBox );

    WCHAR boxValue[256];
    int location = 0;

    for( std::vector<int>::iterator it = g_StackSizeList.begin(); it != g_StackSizeList.end(); ++it )
    {
        wsprintf( boxValue, L"%dx%d", *it, *it );
        pComboBox->AddItem( boxValue, IntToPtr( location ) );
        location++;
    }
    
    pComboBox->SetDropHeight( 28 );

    g_SampleUI_Startup.AddButton( IDC_START_SAMPLE, L"Start", 250, 0, 60, 24, VK_F5 );
    g_SampleUI_Startup.SetSize( 350, 24 );
    
    g_SampleUI_Runtime.Init( &g_DialogResourceManager );
    g_SampleUI_Runtime.SetCallback( OnGUIEvent );
    g_SampleUI_Runtime.EnableKeyboardInput( true );
    g_SampleUI_Runtime.AddStatic( IDC_STATIC, L"(R)ender mode", 0, 0, 105, 25 );
    g_SampleUI_Runtime.AddComboBox( IDC_RENDER_TECHNIQUES, 0, 25, 140, 24, 'R', false, &pComboBox );
    
    for( int i = 0; i < 3; ++i )
        pComboBox->AddItem( RENDER_TECHNIQUESW[i], IntToPtr(i) );
    
    pComboBox->SetDropHeight( 28 );
    pComboBox->SetSelectedByIndex( g_RenderingTechnique );
    
    g_SampleUI_Runtime.AddCheckBox( IDC_RENDER_PARALLAX, L"Use (P)arallax mapping", 0, 120, 175, 22, g_bUseParallax, 'P');
    g_SampleUI_Runtime.AddCheckBox( IDC_SHOW_STACK_LEVELS, L"Show stack (L)evels", 0, 160, 175, 22, g_bShowStackLevels, 'L');
    
    return S_OK;
}


HRESULT CreateClipmapTextures( ID3D10Device* pd3dDevice )
{
    HRESULT hr;
    D3DX10_IMAGE_INFO imageInfo;
    D3DX10_IMAGE_LOAD_INFO imageLoadInfo;
    
    D3DX10GetImageInfoFromFile( g_DstMediaPath[g_StackDepth], NULL, &imageInfo, &hr );

    imageLoadInfo.Width = imageInfo.Width;
    imageLoadInfo.Height = imageInfo.Height;
    imageLoadInfo.Depth = imageInfo.Depth;
    imageLoadInfo.FirstMipLevel = 0;
    imageLoadInfo.MipLevels = g_SourceImageMipsNum - g_StackDepth;
    imageLoadInfo.MiscFlags = imageInfo.MiscFlags;
    imageLoadInfo.Format = imageInfo.Format;
    imageLoadInfo.Usage = D3D10_USAGE_DEFAULT;
    imageLoadInfo.BindFlags = D3D10_BIND_SHADER_RESOURCE;
    imageLoadInfo.CpuAccessFlags = 0;
    imageLoadInfo.pSrcInfo = &imageInfo;
    
    D3DX10CreateTextureFromFile( pd3dDevice, g_DstMediaPath[g_StackDepth], &imageLoadInfo, NULL, (ID3D10Resource**)&g_pPyramidTexture, &hr );

    D3D10_TEXTURE2D_DESC texDesc;
    g_pPyramidTexture->GetDesc( &texDesc );
    
    D3D10_SHADER_RESOURCE_VIEW_DESC SRVDesc;
    ZeroMemory( &SRVDesc, sizeof(SRVDesc) );
    SRVDesc.Format = texDesc.Format;
    SRVDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
    SRVDesc.Texture2D.MipLevels = texDesc.MipLevels;
    SRVDesc.Texture2D.MostDetailedMip = 0;
    
    V_RETURN( pd3dDevice->CreateShaderResourceView( g_pPyramidTexture, &SRVDesc, &g_pPyramidTextureSRV ) );
    
    D3DX10GetImageInfoFromFile( g_DstMediaPathHM[g_StackDepth], NULL, &imageInfo, &hr );

    imageLoadInfo.Width = imageInfo.Width;
    imageLoadInfo.Height = imageInfo.Height;
    imageLoadInfo.Depth = imageInfo.Depth;
    imageLoadInfo.FirstMipLevel = 0;
    imageLoadInfo.MipLevels = g_SourceImageMipsNum - g_StackDepth;
    imageLoadInfo.MiscFlags = imageInfo.MiscFlags;
    imageLoadInfo.Format = imageInfo.Format;
    imageLoadInfo.Usage = D3D10_USAGE_DEFAULT;
    imageLoadInfo.BindFlags = D3D10_BIND_SHADER_RESOURCE;
    imageLoadInfo.CpuAccessFlags = 0;
    imageLoadInfo.pSrcInfo = &imageInfo;
    
    D3DX10CreateTextureFromFile( pd3dDevice, g_DstMediaPathHM[g_StackDepth], &imageLoadInfo, NULL, (ID3D10Resource**)&g_pPyramidTextureHM, &hr );

    g_pPyramidTexture->GetDesc( &texDesc );
    ZeroMemory( &SRVDesc, sizeof(SRVDesc) );
    SRVDesc.Format = texDesc.Format;
    SRVDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
    SRVDesc.Texture2D.MipLevels = texDesc.MipLevels;
    SRVDesc.Texture2D.MostDetailedMip = 0;
    
    V_RETURN( pd3dDevice->CreateShaderResourceView( g_pPyramidTextureHM, &SRVDesc, &g_pPyramidTextureHMSRV ) );

    ZeroMemory( &texDesc, sizeof(texDesc) );
    texDesc.ArraySize = g_StackDepth;
    texDesc.Usage = D3D10_USAGE_DEFAULT;
    texDesc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
    texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texDesc.Width = g_ClipmapStackSize;
    texDesc.Height = g_ClipmapStackSize;
    texDesc.MipLevels = 1;
    texDesc.SampleDesc.Count = 1;
    
    V_RETURN( pd3dDevice->CreateTexture2D(&texDesc, NULL, &g_pStackTexture) );
   
    ZeroMemory( &SRVDesc, sizeof(SRVDesc) );
    SRVDesc.Format = texDesc.Format;
    SRVDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2DARRAY;
    SRVDesc.Texture2DArray.ArraySize = g_StackDepth;
    SRVDesc.Texture2DArray.FirstArraySlice = 0;
    SRVDesc.Texture2DArray.MipLevels = 1;
    SRVDesc.Texture2DArray.MostDetailedMip = 0;
    
    V_RETURN( pd3dDevice->CreateShaderResourceView( g_pStackTexture, &SRVDesc, &g_pStackTextureSRV ) );

    g_pEffect->GetVariableByName( "PyramidTexture" )->AsShaderResource()->SetResource( g_pPyramidTextureSRV );
    g_pEffect->GetVariableByName( "PyramidTextureHM" )->AsShaderResource()->SetResource( g_pPyramidTextureHMSRV );
    g_pEffect->GetVariableByName( "StackTexture" )->AsShaderResource()->SetResource( g_pStackTextureSRV );

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Create an index buffer to be used for procedural sphere mesh rendering
//--------------------------------------------------------------------------------------
HRESULT CreateIndexBuffer( ID3D10Device* pd3dDevice, ID3D10Buffer **ppIndexBuffer )
{
    HRESULT hr;
    int IndexNumber = SPHERE_MERIDIAN_SLICES_NUM * SPHERE_PARALLEL_SLICES_NUM * 6;

    unsigned *pIndices = new unsigned[IndexNumber];
    unsigned indexCount = 0;

    for( int i = 0; i < SPHERE_PARALLEL_SLICES_NUM; i++ )
    {
        for( int j = 0; j < SPHERE_MERIDIAN_SLICES_NUM; j++ )
        {
            pIndices[indexCount] = i * (SPHERE_MERIDIAN_SLICES_NUM + 1) + j;
            indexCount++;

            pIndices[indexCount] = (i + 1) * (SPHERE_MERIDIAN_SLICES_NUM + 1) + j + 1;
            indexCount++;
            
            pIndices[indexCount] = (i + 1) * (SPHERE_MERIDIAN_SLICES_NUM + 1) + j;
            indexCount++;
            
            pIndices[indexCount] = i * (SPHERE_MERIDIAN_SLICES_NUM + 1) + j;
            indexCount++;
            
            pIndices[indexCount] = i * (SPHERE_MERIDIAN_SLICES_NUM + 1) + j + 1;
            indexCount++;
            
            pIndices[indexCount] = (i + 1) * (SPHERE_MERIDIAN_SLICES_NUM + 1) + j + 1;
            indexCount++;
        }
    }

    D3D10_BUFFER_DESC desc;
    memset( &desc, 0, sizeof(D3D10_BUFFER_DESC) );
    desc.Usage = D3D10_USAGE_DEFAULT;
    desc.ByteWidth = sizeof (unsigned int) * IndexNumber;
    desc.BindFlags = D3D10_BIND_INDEX_BUFFER;

    D3D10_SUBRESOURCE_DATA data;
    data.pSysMem = pIndices;
        
    V_RETURN( pd3dDevice->CreateBuffer( &desc, &data, &g_pSphereIndexBuffer ) )

    if( pIndices )
    {
        delete [] pIndices;
        pIndices = NULL;
    }
    
    return S_OK;
}


void InitStackTexture( ID3D10Device* pd3dDevice )
{
    int mipCornerLU[2];
    int mipCornerRD[2];
    int tileBlockSize = 0;
    int blockIndex[2];
    int subBlockIndex[2];
    int blockCorner[2];
    
    D3D10_BOX SubResourceBox;
    SubResourceBox.front = 0;
    SubResourceBox.back = 1;
    
    int srcBlock[2];
    int dstBlock[2];
    
    for( int i = 0; i < g_StackDepth; ++i )
    {
        mipCornerLU[0] = int(g_StackPosition.x * g_ppSourceImageMipsSize[i][0] - g_ClipmapStackSize* 0.5f);
        mipCornerLU[1] = int(g_StackPosition.y * g_ppSourceImageMipsSize[i][1] - g_ClipmapStackSize* 0.5f);

        mipCornerRD[0] = int(g_StackPosition.x * g_ppSourceImageMipsSize[i][0] + g_ClipmapStackSize* 0.5f);    
        mipCornerRD[1] = int(g_StackPosition.y * g_ppSourceImageMipsSize[i][1] + g_ClipmapStackSize* 0.5f);

        tileBlockSize = int(g_UpdateRegionSize / pow(2.0, i));
        
        SubResourceBox.top = 0;
        SubResourceBox.bottom = SubResourceBox.top + tileBlockSize;

        if( g_ppSourceImageMipsSize[i][1] == g_ClipmapStackSize )
        {
            g_ppUpdatePositions[i][1] = g_ClipmapStackSize / 2;
            
            for( int j = mipCornerLU[1] + g_ClipmapStackSize / 2; j < mipCornerRD[1]; j += tileBlockSize )
            {
                SubResourceBox.left = 0;
                SubResourceBox.right = SubResourceBox.left + tileBlockSize;

                blockIndex[1] = j / FILE_BLOCK_SIZE;
                blockCorner[1] = blockIndex[1] * FILE_BLOCK_SIZE;
                subBlockIndex[1] = ( j - blockCorner[1] ) / tileBlockSize;
            
                for( int k = mipCornerLU[0]; k < mipCornerRD[0]; k += tileBlockSize )
                {
                    int tempBoundary = k;

                    if( tempBoundary < 0 )
                        tempBoundary += g_ppSourceImageMipsSize[i][0];
                
                    blockIndex[0] = tempBoundary / FILE_BLOCK_SIZE;
                                
                    blockCorner[0] = blockIndex[0] * FILE_BLOCK_SIZE;
                    subBlockIndex[0] = ( tempBoundary - blockCorner[0] ) / tileBlockSize; 

                    srcBlock[0] = tempBoundary;
                    srcBlock[1] = j;
                
                    dstBlock[0] = SubResourceBox.left;
                    dstBlock[1] = SubResourceBox.top;
                
                    g_JPEG_Manager.AddBlock( i, srcBlock, dstBlock );
                    
                    SubResourceBox.left += tileBlockSize;
                    SubResourceBox.right += tileBlockSize;
                }
                
                g_JPEG_Manager.Update( pd3dDevice, i );

                SubResourceBox.top += tileBlockSize;
                SubResourceBox.bottom += tileBlockSize;
            }

            SubResourceBox.top = g_ClipmapStackSize/ 2;
            SubResourceBox.bottom = SubResourceBox.top + tileBlockSize;

            for( int j = mipCornerLU[1]; j < mipCornerRD[1] - g_ClipmapStackSize* 0.5; j += tileBlockSize )
            {
                SubResourceBox.left = 0;
                SubResourceBox.right = SubResourceBox.left + tileBlockSize;

                blockIndex[1] = j / FILE_BLOCK_SIZE;
                blockCorner[1] = blockIndex[1] * FILE_BLOCK_SIZE;
                subBlockIndex[1] = ( j - blockCorner[1] ) / tileBlockSize;
            
                for( int k = mipCornerLU[0]; k < mipCornerRD[0]; k += tileBlockSize )
                {
                    int tempBoundary = k;

                    if( tempBoundary < 0 )
                        tempBoundary += g_ppSourceImageMipsSize[i][0];
                
                    blockIndex[0] = tempBoundary / FILE_BLOCK_SIZE;

                    blockCorner[0] = blockIndex[0] * FILE_BLOCK_SIZE;
                    subBlockIndex[0] = ( tempBoundary - blockCorner[0] ) / tileBlockSize; 

                    srcBlock[0] = tempBoundary;
                    srcBlock[1] = j;
                
                    dstBlock[0] = SubResourceBox.left;
                    dstBlock[1] = SubResourceBox.top;
                
                    g_JPEG_Manager.AddBlock( i, srcBlock, dstBlock );
                    
                    SubResourceBox.left += tileBlockSize;
                    SubResourceBox.right += tileBlockSize;
                }
                
                g_JPEG_Manager.Update( pd3dDevice, i );

                SubResourceBox.top += tileBlockSize;
                SubResourceBox.bottom += tileBlockSize;
            }
        }
        else
        {
            for( int j = mipCornerLU[1]; j < mipCornerRD[1]; j += tileBlockSize )
            {
                SubResourceBox.left = 0;
                SubResourceBox.right = SubResourceBox.left + tileBlockSize;

                blockIndex[1] = j / FILE_BLOCK_SIZE;
                blockCorner[1] = blockIndex[1] * FILE_BLOCK_SIZE;
                subBlockIndex[1] = ( j - blockCorner[1] ) / tileBlockSize;
            
                for( int k = mipCornerLU[0]; k < mipCornerRD[0]; k += tileBlockSize )
                {
                    int tempBoundary = k;

                    if( tempBoundary < 0 )
                        tempBoundary += g_ppSourceImageMipsSize[i][0];
                
                    blockIndex[0] = tempBoundary / FILE_BLOCK_SIZE;

                    blockCorner[0] = blockIndex[0] * FILE_BLOCK_SIZE;
                    subBlockIndex[0] = ( tempBoundary - blockCorner[0] ) / tileBlockSize; 

                    srcBlock[0] = tempBoundary;
                    srcBlock[1] = j;
                
                    dstBlock[0] = SubResourceBox.left;
                    dstBlock[1] = SubResourceBox.top;
                
                    g_JPEG_Manager.AddBlock( i, srcBlock, dstBlock );
                   
                    SubResourceBox.left += tileBlockSize;
                    SubResourceBox.right += tileBlockSize;
                }
                
                g_JPEG_Manager.Update( pd3dDevice, i );

                SubResourceBox.top += tileBlockSize;
                SubResourceBox.bottom += tileBlockSize;
            }
        }
    }
}


//--------------------------------------------------------------------------------------
// Calculate base parameters using defined sizes
//--------------------------------------------------------------------------------------
void CalculateClipmapParameters()
{
    g_StackDepth = 0;
    int dimensionMax = g_SourceImageWidth;
    
    if( g_SourceImageHeight > g_SourceImageWidth )
        dimensionMax = g_SourceImageHeight;
    
    g_SourceImageMipsNum = int( log( (double)dimensionMax + 1.0 ) / log( 2.0 ) ) + 1;
    
    for( int i = 0; i < dimensionMax; ++i )
    {
        if( dimensionMax / pow(2.0f, g_StackDepth) > g_ClipmapStackSize)
        {
            g_StackDepth++;
        }
        else
        {
            break;
        }
    }

    g_ppSourceImageMipsSize = new int*[g_SourceImageMipsNum];

    for( int i = 0; i < g_SourceImageMipsNum; ++i )
    {
        g_ppSourceImageMipsSize[i] = new int[2];
        
        g_ppSourceImageMipsSize[i][0] = g_SourceImageWidth / (unsigned)pow( 2.0f, i );
        g_ppSourceImageMipsSize[i][1] = g_SourceImageHeight / (unsigned)pow( 2.0f, i );
    }
    
    g_StackPosition.x = 0.0f;
    g_StackPosition.y = 0.5f;

    assert(g_StackDepth);
    
    g_pMipDataOffset = new int[g_StackDepth];
    g_ppUpdatePositions = new int*[g_StackDepth];

    for( int i = 0; i < g_StackDepth; ++i )
    {
        g_ppUpdatePositions[i] = new int[2];
        
        g_ppUpdatePositions[i][0] = 0;
        g_ppUpdatePositions[i][1] = 0;
    }
    
    g_pEffect->GetVariableByName( "g_StackDepth" )->AsScalar()->SetInt( g_StackDepth );
    
    D3DXVECTOR2 scaleFactor;
    scaleFactor.x = (float)g_SourceImageWidth / g_ClipmapStackSize;
    scaleFactor.y = (float)g_SourceImageHeight / g_ClipmapStackSize;
    g_pEffect->GetVariableByName( "g_ScaleFactor" )->AsVector()->SetFloatVectorArray( scaleFactor, 0, 2 );
    
    int textureSize[2];
    textureSize[0] = g_SourceImageWidth;
    textureSize[1] = g_SourceImageHeight;

    g_pEffect->GetVariableByName( "g_TextureSize" )->AsVector()->SetIntVectorArray( (int*)textureSize, 0, 2 );
    g_pEffect->GetVariableByName( "g_MipColors" )->AsVector()->SetFloatVectorArray( (float*)g_MipmapColors, 0, MIPMAP_LEVELS_MAX * 3 );

    g_pEffect->GetVariableByName( "g_SphereMeridianSlices" )->AsScalar()->SetInt( SPHERE_MERIDIAN_SLICES_NUM );
    g_pEffect->GetVariableByName( "g_SphereParallelSlices" )->AsScalar()->SetInt( SPHERE_PARALLEL_SLICES_NUM );
}


void outPrint_2(const wchar_t* msg,float x,float y)
{
	int number = 10;
	wchar_t str[256];
	std::wstring m(msg);
	m += std::wstring(L"\n");
	swprintf(str, 256, m.c_str(), x, y);
	OutputDebugString(str);
}

void outPrint_4(const wchar_t* msg, float x, float y, float z, float w)
{
	wchar_t str[1024];
	std::wstring m(msg);
	m += std::wstring(L"\n");
	swprintf(str, 1024, m.c_str(), x, y,z,w);
	OutputDebugString(str);
}


//--------------------------------------------------------------------------------------
// Perform stack texture updates based on a new viewer's position
//--------------------------------------------------------------------------------------
void UpdateStackTexture( ID3D10Device* pd3dDevice )
{
    D3DXVECTOR2 updateBorder;
    float length = sqrtf( g_EyePosition.x * g_EyePosition.x + g_EyePosition.z * g_EyePosition.z );
    float posHorizontal;
    float posVertical;

    // Positions are calculated in order to map current viewer's position
    // to a rectangular region of a source image mapped on a spherical mesh.
    // Positions are represented by values clamped to [0 - 1] range in each dimension
    if( g_EyePosition.z >= 0 )
    {
        if( g_EyePosition.x <= 0 )
            posHorizontal = atanf( -( g_EyePosition.x / g_EyePosition.z ) ) / (D3DX_PI * 2);
        else
            posHorizontal = 1.0f - atanf( g_EyePosition.x / g_EyePosition.z ) / (D3DX_PI * 2);
    }
    else
    {
        posHorizontal = 0.5f - atanf((g_EyePosition.x / g_EyePosition.z)) / (D3DX_PI * 2);
    }
        
    posVertical = 0.5f - atanf(g_EyePosition.y / length) / D3DX_PI;
    
    int srcBlock[2];
    int dstBlock[2];
    
    // Calculate border sizes to be updated
    updateBorder.x = posHorizontal - g_StackPosition.x;
    
    if( updateBorder.x > 0 )
    {
        if( updateBorder.x > 0.5f )
            updateBorder.x = updateBorder.x - 1.0f;
    }
    else
    {
        if( updateBorder.x < -0.5f )
            updateBorder.x += 1.0f; 
    }

    updateBorder.y = posVertical - g_StackPosition.y;

    int updateBorderSize[2];
    updateBorderSize[0] = int( updateBorder.x * g_SourceImageWidth );
    updateBorderSize[1] = int( updateBorder.y * g_SourceImageHeight );

    int tileBlockSize = 0;
    int mipCornerLU[2];
    int mipCornerRD[2];

    D3D10_BOX SubResourceBox;
    SubResourceBox.front = 0;
    SubResourceBox.back = 1;

    unsigned correction = 0;

    // Update square region when we move "Image space Right"
    if( updateBorderSize[0] > g_UpdateRegionSize )
    {
		OutputDebugString(L"==================================================\n");
		OutputDebugString(L"eye move right \n");
        // Update all clipmap stack layers one by one
        for( int i = 0; i < g_StackDepth; ++i )
        {            
            tileBlockSize = g_UpdateRegionSize / (unsigned)pow(2.0, i);

            // Calculate regions of source image that should be updated in the current stack layer
            mipCornerLU[0] = int(g_StackPosition.x * g_ppSourceImageMipsSize[i][0]) + g_ClipmapStackSize / 2;
            mipCornerRD[0] = mipCornerLU[0] + tileBlockSize;
            mipCornerLU[1] = int(g_StackPosition.y * g_ppSourceImageMipsSize[i][1]) - g_ClipmapStackSize / 2;
            mipCornerRD[1] = int(g_StackPosition.y * g_ppSourceImageMipsSize[i][1]) + g_ClipmapStackSize / 2;

			if (mipCornerLU[0] > int(g_ppSourceImageMipsSize[i][0] - tileBlockSize))
				mipCornerLU[0] -= g_ppSourceImageMipsSize[i][0];

            if( mipCornerLU[1] < 0 )
            {
				if (i == 0)  OutputDebugString(L" mipCornerLU[1] < 0 and mipCornerLU[1] = 0 \n ");
                correction = -mipCornerLU[1];
                mipCornerLU[1] = 0;
            }
            else if( mipCornerLU[1] >= g_ppSourceImageMipsSize[i][1] )
            {
                UpdateMipPosition(g_ppUpdatePositions[i][0], tileBlockSize);
				if (i == 0) 	OutputDebugString(L" mipCornerLU[1] > maxsize and update g_ppUpdatePositions[0] \n  ");
                continue;
            }

            if( mipCornerRD[1] <= 0 )
            {
                UpdateMipPosition(g_ppUpdatePositions[i][0], tileBlockSize);
                continue;
            }
			else if (mipCornerRD[1] > g_ppSourceImageMipsSize[i][1])
			{
				if (i == 0) 	OutputDebugString(L" mipCornerRD[1] > maxsize and  mipCornerRD[1] = maxsize \n  ");
				mipCornerRD[1] = g_ppSourceImageMipsSize[i][1];
			}

            SubResourceBox.left = g_ppUpdatePositions[i][0];
            SubResourceBox.right = SubResourceBox.left + tileBlockSize;
            SubResourceBox.top = g_ppUpdatePositions[i][1] + correction;

			if (i == 0)
			{
				outPrint_4(L"add block from : (%f,%f) to (%f,%f) ", mipCornerLU[0], mipCornerLU[1], mipCornerLU[0] + tileBlockSize, mipCornerRD[1]);
				outPrint_4(L"the box is origin : (%f,%f) to (%f,%f) ", SubResourceBox.left, SubResourceBox.top, SubResourceBox.left + tileBlockSize, SubResourceBox.top + (mipCornerRD[1] - mipCornerLU[1]));
			}

            for( int j = mipCornerLU[1]; j < mipCornerRD[1]; j += tileBlockSize )
            {
                SubResourceBox.bottom = SubResourceBox.top + tileBlockSize;

                srcBlock[0] = mipCornerLU[0];
                srcBlock[1] = j;
                
                dstBlock[0] = SubResourceBox.left;
                dstBlock[1] = SubResourceBox.top;
                

                g_JPEG_Manager.AddBlock( i, srcBlock, dstBlock );

                SubResourceBox.top += tileBlockSize;

                if( SubResourceBox.top > UINT(g_ClipmapStackSize - tileBlockSize) )
                    SubResourceBox.top = 0;
            }
        
            g_JPEG_Manager.Update( pd3dDevice, i );
            UpdateMipPosition( g_ppUpdatePositions[i][0], tileBlockSize );
        }

        g_StackPosition.x += (float)g_UpdateRegionSize / g_SourceImageWidth;
    }

    // Update square region when we move "Image space Left"
    if( updateBorderSize[0] < -g_UpdateRegionSize )
    {
		OutputDebugString(L"==================================================\n");
		OutputDebugString(L"eye move left \n");

		outPrint_4(L"eye  :(%f , %f)  and stackpost (%f , %f)", g_EyePosition.x, g_EyePosition.y,g_StackPosition.x, g_StackPosition.y);


        for( int i = 0; i < g_StackDepth; ++i )
        {
            tileBlockSize = g_UpdateRegionSize / (unsigned)pow(2.0, i);

            UpdateMipPosition(g_ppUpdatePositions[i][0], -tileBlockSize);
            
            mipCornerLU[0] = int(g_StackPosition.x * g_ppSourceImageMipsSize[i][0]) - g_ClipmapStackSize / 2 - tileBlockSize;
            mipCornerRD[0] = mipCornerLU[0] + tileBlockSize;
            mipCornerLU[1] = int(g_StackPosition.y * g_ppSourceImageMipsSize[i][1]) - g_ClipmapStackSize / 2;
            mipCornerRD[1] = int(g_StackPosition.y * g_ppSourceImageMipsSize[i][1]) + g_ClipmapStackSize / 2;

			 

			if (mipCornerLU[0] < 0)
				mipCornerLU[0] += g_ppSourceImageMipsSize[i][0];

            if( mipCornerLU[1] < 0 )
            {
                correction = -mipCornerLU[1];
				if (i == 0) 	OutputDebugString(L" mipCornerLU[1] < 0 and  mipCornerLU[1] = 0 \n  ");

                mipCornerLU[1] = 0;
            }
			else if (mipCornerLU[1] >= g_ppSourceImageMipsSize[i][1])
			{
				if (i == 0) 	OutputDebugString(L" mipCornerLU[1] > 0 and continue \n  ");
				continue;
			}

			if (mipCornerRD[1] <= 0)
			{
				if (i == 0) 	OutputDebugString(L" mipCornerRD[1] <= 0 and  continue \n  ");
				continue;
			}

			else if (mipCornerRD[1] > g_ppSourceImageMipsSize[i][1])
			{
				if (i == 0) 	OutputDebugString(L" mipCornerRD[1] > maxisize and  mipCornerRD[1] = maxise\n  ");

				mipCornerRD[1] = g_ppSourceImageMipsSize[i][1];
			}

            SubResourceBox.left = g_ppUpdatePositions[i][0];
            SubResourceBox.right = SubResourceBox.left + tileBlockSize;
            SubResourceBox.top = g_ppUpdatePositions[i][1] + correction;

			if (i == 0)
			{
				outPrint_4(L"add block from : (%f,%f) to (%f,%f) ", mipCornerLU[0], mipCornerLU[1], mipCornerLU[0] + tileBlockSize, mipCornerRD[1]);
				outPrint_4(L"the box is origin : (%f,%f) to (%f,%f) ", SubResourceBox.left, SubResourceBox.top, SubResourceBox.left + tileBlockSize, SubResourceBox.top + (mipCornerRD[1] - mipCornerLU[1]));
			}

            for( int j = mipCornerLU[1]; j < mipCornerRD[1]; j += tileBlockSize )
            {
                SubResourceBox.bottom = SubResourceBox.top + tileBlockSize;

                srcBlock[0] = mipCornerLU[0];
                srcBlock[1] = j;
                
                dstBlock[0] = SubResourceBox.left;
                dstBlock[1] = SubResourceBox.top;
                
			 

                g_JPEG_Manager.AddBlock( i, srcBlock, dstBlock );

                SubResourceBox.top += tileBlockSize;

                if( SubResourceBox.top > UINT(g_ClipmapStackSize - tileBlockSize) )
                    SubResourceBox.top = 0;
            }
            
            g_JPEG_Manager.Update( pd3dDevice, i );
        }
        
        g_StackPosition.x -= (float)g_UpdateRegionSize / g_SourceImageWidth;
    }

    if( g_StackPosition.x < 0.0f )
        g_StackPosition.x = 1.0f + g_StackPosition.x;
    else if( g_StackPosition.x > 1.0f )
        g_StackPosition.x -= 1.0f;
    
    // Update square region when we move "Image space Down"
    if( updateBorderSize[1] > g_UpdateRegionSize )
    {
		OutputDebugString(L"==================================================\n");

		OutputDebugString(L"eye move down \n");

        for( int i = 0; i < g_StackDepth; ++i )
        {
            correction = 0;
            
            tileBlockSize = g_UpdateRegionSize / (unsigned)pow(2.0, i);

            mipCornerLU[0] = int(g_StackPosition.x * g_ppSourceImageMipsSize[i][0]) - g_ClipmapStackSize / 2;
                            
            int stepsNum = g_ClipmapStackSize / tileBlockSize;

            mipCornerLU[1] = int(g_StackPosition.y * g_ppSourceImageMipsSize[i][1]) + g_ClipmapStackSize / 2;
            mipCornerRD[1] = mipCornerLU[1] + tileBlockSize;

		 
			if (mipCornerLU[0] < 0)
				mipCornerLU[0] += g_ppSourceImageMipsSize[i][0];

			if (mipCornerLU[1] < 0)
			{
				if (i == 0) 	OutputDebugString(L" mipCornerLU[1] < 0 and  mipCornerLU[1] = 0\n  ");
				mipCornerLU[1] = 0;

			}
            else if( mipCornerLU[1] >= g_ppSourceImageMipsSize[i][1] )
            {
                UpdateMipPosition(g_ppUpdatePositions[i][1], tileBlockSize);
				if (i == 0) 	OutputDebugString(L" mipCornerLU[1] > maxisize and  continue\n  ");

                continue;
            }

			if (mipCornerRD[1] <= 0)
			{
				if (i == 0) 	OutputDebugString(L" mipCornerRD[1] <=  0 and  continue\n  ");
				continue;

			}
			else if (mipCornerRD[1] > g_ppSourceImageMipsSize[i][1])
			{
				if (i == 0) 	OutputDebugString(L" mipCornerRD[1] > maxisize  0 and  = maxisize\n  ");
				mipCornerRD[1] = g_ppSourceImageMipsSize[i][1];

			}

            SubResourceBox.top = g_ppUpdatePositions[i][1];
            SubResourceBox.bottom = SubResourceBox.top + tileBlockSize;
            SubResourceBox.left = g_ppUpdatePositions[i][0];

            while( SubResourceBox.left > UINT(g_ClipmapStackSize - tileBlockSize) )
                SubResourceBox.left -= g_ClipmapStackSize;

			if (i == 0)
			{
				outPrint_4(L"add block from : (%f,%f) to (%f,%f) ", mipCornerLU[0], mipCornerLU[1], mipCornerLU[0] + g_ClipmapStackSize, mipCornerRD[1]);
				outPrint_4(L"the box is origin : (%f,%f) to (%f,%f) ", SubResourceBox.left, SubResourceBox.top, SubResourceBox.left + tileBlockSize, SubResourceBox.top + (mipCornerRD[1] - mipCornerLU[1]));
			}

            for( int step = 0, j = mipCornerLU[0]; step < stepsNum; step++, j += tileBlockSize )
            {
                if( j == g_ppSourceImageMipsSize[i][0] )
                    j = 0;

                SubResourceBox.right = SubResourceBox.left + tileBlockSize;
                
                srcBlock[0] = j;
                srcBlock[1] = mipCornerLU[1];
                
                dstBlock[0] = SubResourceBox.left;
                dstBlock[1] = SubResourceBox.top;

                g_JPEG_Manager.AddBlock( i, srcBlock, dstBlock );

                SubResourceBox.left += tileBlockSize;

                if( SubResourceBox.left > UINT( g_ClipmapStackSize - tileBlockSize ) )
                    SubResourceBox.left = 0;
            }

            g_JPEG_Manager.Update( pd3dDevice, i );
            UpdateMipPosition( g_ppUpdatePositions[i][1], tileBlockSize );
        }

        g_StackPosition.y += (float)g_UpdateRegionSize / g_SourceImageHeight;
    }
    
    // Update square region when we move "Image space Up"
    if( updateBorderSize[1] < -g_UpdateRegionSize )
    {
		OutputDebugString(L"==================================================\n");

		OutputDebugString(L"eye move up \n");

        for( int i = 0; i < g_StackDepth; ++i )
        {
            correction = 0;
            
            tileBlockSize = g_UpdateRegionSize / (unsigned)pow(2.0, i);

            mipCornerLU[0] = int(g_StackPosition.x * g_ppSourceImageMipsSize[i][0]) - g_ClipmapStackSize / 2;
          
            int stepsNum = g_ClipmapStackSize / tileBlockSize;
            

			if (mipCornerLU[0] < 0)
				mipCornerLU[0] += g_ppSourceImageMipsSize[i][0];

            UpdateMipPosition(g_ppUpdatePositions[i][1], -tileBlockSize);
                        
            mipCornerLU[1] = int(g_StackPosition.y * g_ppSourceImageMipsSize[i][1]) - g_ClipmapStackSize / 2 - tileBlockSize;

            if( mipCornerLU[1] < 0 )
            {
				if (i == 0) 	OutputDebugString(L" mipCornerLU[1] < 0  0 and  += maxisize\n  ");
                mipCornerLU[1] += g_ppSourceImageMipsSize[i][1];
            }

            mipCornerRD[1] = mipCornerLU[1] + tileBlockSize;

			if (mipCornerRD[1] <= 0)
			{
				if (i == 0) 	OutputDebugString(L" mipCornerRD[1] <= 0  0 and  continue\n  ");
				continue;

			}
			else if (mipCornerRD[1] > g_ppSourceImageMipsSize[i][1])
			{
				if (i == 0) 	OutputDebugString(L" mipCornerRD[1] > maxsize  0 and  = maxsize\n  ");
				mipCornerRD[1] = g_ppSourceImageMipsSize[i][1];

			}

			if (i == 0)
			{
				outPrint_4(L"add block from : (%f,%f) to (%f,%f) ", mipCornerLU[0], mipCornerLU[1], mipCornerLU[0] + g_ClipmapStackSize, mipCornerRD[1]);
			}

            SubResourceBox.top = g_ppUpdatePositions[i][1];
            SubResourceBox.bottom = SubResourceBox.top + tileBlockSize;
            SubResourceBox.left = g_ppUpdatePositions[i][0];

            while( SubResourceBox.left > UINT(g_ClipmapStackSize - tileBlockSize) )
                SubResourceBox.left -= g_ClipmapStackSize;

			if (i == 0)outPrint_4(L"the box is origin : (%f,%f) to (%f,%f) ", SubResourceBox.left, SubResourceBox.top, SubResourceBox.left + tileBlockSize, SubResourceBox.top + (mipCornerRD[1] - mipCornerLU[1]));
            for( int step = 0, j = mipCornerLU[0]; step < stepsNum; step++, j += tileBlockSize )
            {
                if( j == g_ppSourceImageMipsSize[i][0] )
                    j = 0;
            
                SubResourceBox.right = SubResourceBox.left + tileBlockSize;

                srcBlock[0] = j;
                srcBlock[1] = mipCornerLU[1];
                
                dstBlock[0] = SubResourceBox.left;
                dstBlock[1] = SubResourceBox.top;
                

                g_JPEG_Manager.AddBlock( i, srcBlock, dstBlock );

                SubResourceBox.left += tileBlockSize;

                if( SubResourceBox.left > UINT(g_ClipmapStackSize - tileBlockSize) )
                    SubResourceBox.left = 0;
            }
            
            g_JPEG_Manager.Update( pd3dDevice, i );
        }

        g_StackPosition.y -= (float)g_UpdateRegionSize / g_SourceImageHeight;
    }

    if( g_StackPosition.y < 0.0f )
        g_StackPosition.y = 1.0f + g_StackPosition.y;
    else if( g_StackPosition.y > 1.0f )
        g_StackPosition.y -= 1.0f;
}


//--------------------------------------------------------------------------------------
// Calculate new positions of updatable regions in each clipmap stack level.
// Positions define the upper left corner of updatable square region
//--------------------------------------------------------------------------------------
void UpdateMipPosition( int &position, int offset )
{
    position += offset;

    if( offset > 0 )
    {
        if( position > g_ClipmapStackSize - offset)
            position = 0;
    }
    else
    {
        if( position < 0 )
            position = g_ClipmapStackSize + offset;
    }    
}


//--------------------------------------------------------------------------------------
// Handles the GUI events
//--------------------------------------------------------------------------------------
void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
    int blocksPerLayer = 0;
        
    switch( nControlID )
    {
        case IDC_TOGGLEFULLSCREEN:
            DXUTToggleFullScreen();
            break;
        case IDC_TOGGLEREF:
            DXUTToggleREF();
            break;
        case IDC_CHANGEDEVICE:
            g_D3DSettingsDlg.SetActive( !g_D3DSettingsDlg.IsActive() );
            break;
        case IDC_START_SAMPLE:
            g_ClipmapStackSize = g_StackSizeList[ g_SampleUI_Startup.GetComboBox( IDC_STACK_SIZE )->GetSelectedIndex() ];
            CalculateClipmapParameters();
            blocksPerLayer = g_SourceImageWidth / g_UpdateRegionSize;

            g_UpdateRegionSize = (int)( 16.0 * pow( 2.0f, g_StackDepth - 1 ) );

            g_JPEG_Manager.Intitialize( g_StackDepth, g_DstMediaPath, g_DstMediaPathHM );
            g_JPEG_Manager.AllocateBlocks( blocksPerLayer );
            g_JPEG_Manager.AllocateTextures( g_pD3DDevice, g_ClipmapStackSize, g_UpdateRegionSize );

            CreateClipmapTextures( g_pD3DDevice );
            InitStackTexture( g_pD3DDevice );

            g_bSampleInitialized = true;

            break;
        case IDC_RENDER_TECHNIQUES:
            g_RenderingTechnique = g_SampleUI_Runtime.GetComboBox(IDC_RENDER_TECHNIQUES)->GetSelectedIndex();
            break;
        case IDC_RENDER_PARALLAX:
            g_bUseParallax = !g_bUseParallax;
            break;
        case IDC_SHOW_STACK_LEVELS:
            g_bShowStackLevels = !g_bShowStackLevels;
            break;
    }
}
