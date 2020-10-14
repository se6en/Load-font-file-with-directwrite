#include "stdafx.h"
#include "D2DDrawTextCtrl.h"
#include "D2DFontContext.h"

#define FONT_FILE          _T("harrypotter.TTF")
#define FONT_NAME          _T("Harry P")

IMPLEMENT_DYNAMIC(CD2DDrawTextCtrl, CStatic)

CD2DDrawTextCtrl::CD2DDrawTextCtrl()
{
   m_pD2DFactory = nullptr;
   m_pDevice = nullptr;
   m_pD2DContext = nullptr;
   m_pSwapChain = nullptr;
   m_pTargetBitmap = nullptr;

   m_pDWriteFactory = nullptr;
   m_pRenderTarget = nullptr;
   m_pTextFormat = nullptr;
   m_pTextLayout = nullptr;
   m_pSolidBrush = nullptr;
}

CD2DDrawTextCtrl::~CD2DDrawTextCtrl()
{

}

BEGIN_MESSAGE_MAP(CD2DDrawTextCtrl, CStatic)
   ON_WM_PAINT()
END_MESSAGE_MAP()

void CD2DDrawTextCtrl::PreSubclassWindow()
{
   CreateDeviceIndependentResources();

   CStatic::PreSubclassWindow();
}

void CD2DDrawTextCtrl::OnPaint()
{
   CPaintDC dc(this);

   if (m_pD2DFactory == nullptr)
   {
      return;
   }

   CRect rcClient;
   GetClientRect(rcClient);

   HRESULT hr = CreateDeviceResources();

   if (SUCCEEDED(hr))
   {
      D2D1_MATRIX_3X2_F transform = D2D1::IdentityMatrix();

      DWRITE_TEXT_METRICS textMetrics;
      m_pTextLayout->GetMetrics(&textMetrics);

      DWRITE_OVERHANG_METRICS overhangMetrics;
      m_pTextLayout->GetOverhangMetrics(&overhangMetrics);

      float overhangHeight = overhangMetrics.top - overhangMetrics.bottom;
      float fActualHeight = overhangHeight * textMetrics.height / textMetrics.layoutHeight;

      m_pD2DContext->BeginDraw();
      m_pD2DContext->SetTarget(m_pTargetBitmap.Get());
      m_pD2DContext->SetTransform(transform);
      m_pD2DContext->DrawTextLayout(
         D2D1::Point2F(0, 0),
         m_pTextLayout.Get(),
         m_pSolidBrush.Get()
      );
      m_pD2DContext->EndDraw();

      DXGI_PRESENT_PARAMETERS parameters = { 0 };
      parameters.DirtyRectsCount = 0;
      parameters.pDirtyRects = nullptr;
      parameters.pScrollRect = nullptr;
      parameters.pScrollOffset = nullptr;

      hr = m_pSwapChain->Present1(1, 0, &parameters);
   }
}

HRESULT CD2DDrawTextCtrl::CreateDeviceIndependentResources()
{
   HRESULT hr = S_OK;

   IDXGIAdapter *pDxgiAdapter = nullptr;
   ID3D11Device *pD3D11Device = nullptr;
   ID3D11DeviceContext *pD3D11DeviceContext = nullptr;
   IDXGIDevice1 *pDxgiDevice = nullptr;
   IDXGIFactory2 *pDxgiFactory = nullptr;
   IDXGISurface *pDxgiBackBuffer = nullptr;

   UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
   creationFlags |= D3D11_CREATE_DEVICE_DEBUG;

   D3D_FEATURE_LEVEL featureLevels[] = {
      //D3D_FEATURE_LEVEL_11_1,
      D3D_FEATURE_LEVEL_11_0,
      D3D_FEATURE_LEVEL_10_1,
      D3D_FEATURE_LEVEL_10_0
   };

   D3D_FEATURE_LEVEL featureLevel;

   hr = D3D11CreateDevice(pDxgiAdapter, D3D_DRIVER_TYPE_HARDWARE, nullptr, creationFlags, featureLevels, sizeof(featureLevels) / sizeof(D3D_FEATURE_LEVEL), D3D11_SDK_VERSION, &pD3D11Device, &featureLevel, &pD3D11DeviceContext);

   if (SUCCEEDED(hr))
   {
      hr = pD3D11Device->QueryInterface(__uuidof(IDXGIDevice1), (void **)&pDxgiDevice);
   }

   if (SUCCEEDED(hr))
   {
      hr = pDxgiDevice->GetAdapter(&pDxgiAdapter);
   }

   if (SUCCEEDED(hr))
   {
      hr = pDxgiAdapter->GetParent(IID_PPV_ARGS(&pDxgiFactory));
   }

   if (SUCCEEDED(hr))
   {
      DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullscreenDesc;
      fullscreenDesc.RefreshRate.Numerator = 60;
      fullscreenDesc.RefreshRate.Denominator = 1;
      fullscreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
      fullscreenDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
      fullscreenDesc.Windowed = FALSE;

      DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
      swapChainDesc.Height = 0;
      swapChainDesc.Width = 0;
      swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
      swapChainDesc.BufferCount = 2;
      swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
      swapChainDesc.Flags = 0;
      swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
      swapChainDesc.SampleDesc.Count = 1;
      swapChainDesc.SampleDesc.Quality = 0;
      swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
      swapChainDesc.Stereo = FALSE;
      swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

      hr = pDxgiFactory->CreateSwapChainForHwnd(pD3D11Device, m_hWnd, &swapChainDesc, nullptr, nullptr, &m_pSwapChain);
   }

   if (SUCCEEDED(hr))
   {
      hr = m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pDxgiBackBuffer));
   }

   if (SUCCEEDED(hr))
   {
      hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory), &m_pD2DFactory);
   }

   if (SUCCEEDED(hr))
   {
      hr = m_pD2DFactory->CreateDevice(pDxgiDevice, &m_pDevice);
   }

   if (SUCCEEDED(hr))
   {
      hr = m_pDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_pD2DContext);
      m_pD2DContext->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
   }

   if (SUCCEEDED(hr))
   {
      D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(
         D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
         D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
         96.0f,
         96.0f
      );

      hr = m_pD2DContext->CreateBitmapFromDxgiSurface(pDxgiBackBuffer, &bitmapProperties, &m_pTargetBitmap);
   }

   if (SUCCEEDED(hr))
   {
      hr = DWriteCreateFactory(
         DWRITE_FACTORY_TYPE_SHARED,
         __uuidof(IDWriteFactory),
         &m_pDWriteFactory
      );
   }

   CString strFont = _T("Tahoma");
   float fSize = 50.f;
   CString strContent = _T("Harry Potter");


   if (SUCCEEDED(hr))
   {
      hr = m_pDWriteFactory->CreateTextFormat(
         strFont,
         nullptr,
         DWRITE_FONT_WEIGHT_NORMAL,
         DWRITE_FONT_STYLE_NORMAL,
         DWRITE_FONT_STRETCH_NORMAL,
         fSize,
         L"en-us",
         &m_pTextFormat
      );
   }

   if (SUCCEEDED(hr))
   {
      m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

      hr = m_pDWriteFactory->CreateTextLayout(
         strContent,
         strContent.GetLength(),
         m_pTextFormat.Get(),
         459,
         302,
         &m_pTextLayout
      );
   }

   if (SUCCEEDED(hr))
   {
      TCHAR strPath[MAX_PATH + 1];
      GetModuleFileName(AfxGetApp()->m_hInstance, strPath, MAX_PATH);

      CString strAppPath = strPath;
      int nPos = strAppPath.ReverseFind(_T('\\'));

      CString strFontFilePath = strAppPath.Mid(0, nPos + 1);
      strFontFilePath += FONT_FILE;

      if (PathFileExists(strFontFilePath))
      {
         IDWriteFontCollection* pFontCollection = nullptr;

         CD2DFontContext fontContext(m_pDWriteFactory);

         hr = fontContext.CreateFontCollection(strFontFilePath, &pFontCollection);

         if (SUCCEEDED(hr))
         {
            DWRITE_TEXT_RANGE textRange = { 0, 5 };
            m_pTextLayout->SetFontFamilyName(FONT_NAME, textRange);
            m_pTextLayout->SetFontCollection(pFontCollection, textRange);
         }


      }

   }

   return hr;
}

HRESULT CD2DDrawTextCtrl::CreateDeviceResources()
{
   HRESULT hr = S_OK;

   if (m_pRenderTarget == nullptr)
   {
      CRect rcClient;
      GetClientRect(rcClient);

      D2D1_SIZE_U size = D2D1::SizeU(
         rcClient.Width(),
         rcClient.Height()
      );

      D2D1_RENDER_TARGET_PROPERTIES renderTargetProperty = D2D1::RenderTargetProperties(
         /*D2D1_RENDER_TARGET_TYPE_DEFAULT*/D2D1_RENDER_TARGET_TYPE_HARDWARE,
         D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
         96.0,
         96.0
      );

      hr = m_pD2DFactory->CreateHwndRenderTarget(
         /*D2D1::RenderTargetProperties()*/renderTargetProperty,
         D2D1::HwndRenderTargetProperties(GetSafeHwnd(), size, D2D1_PRESENT_OPTIONS_IMMEDIATELY),
         &m_pRenderTarget
      );

      if (SUCCEEDED(hr))
      {
         hr = m_pD2DContext->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::White),
            &m_pSolidBrush
         );
      }
   }

   return hr;
}