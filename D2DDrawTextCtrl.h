#pragma once

using namespace Microsoft::WRL;

class CD2DDrawTextCtrl : public CStatic
{
   DECLARE_DYNAMIC(CD2DDrawTextCtrl)
public:
   CD2DDrawTextCtrl();
   virtual ~CD2DDrawTextCtrl();

   afx_msg void OnPaint();

   virtual void PreSubclassWindow();

   DECLARE_MESSAGE_MAP()

public:
   HRESULT CreateDeviceIndependentResources();
   HRESULT CreateDeviceResources();

private:
   ComPtr<ID2D1Factory2>                     m_pD2DFactory;
   ComPtr<ID2D1Device1>                      m_pDevice;
   ComPtr<ID2D1DeviceContext1>               m_pD2DContext;
   ComPtr<IDXGISwapChain1>                   m_pSwapChain;
   ComPtr<ID2D1Bitmap1>                      m_pTargetBitmap;
   ComPtr<ID2D1HwndRenderTarget>             m_pRenderTarget;

   ComPtr<IDWriteFactory>                    m_pDWriteFactory;
   ComPtr<IDWriteTextFormat>                 m_pTextFormat;
   ComPtr<IDWriteTextLayout>                 m_pTextLayout;
   ComPtr<ID2D1SolidColorBrush>              m_pSolidBrush;
};
