#pragma once

#include <memory>

class CD2DFontCollectionLoader;

using namespace Microsoft::WRL;

class CD2DFontContext
{
public:
   CD2DFontContext(ComPtr<IDWriteFactory> pFactory);
   ~CD2DFontContext();

   HRESULT Initialize();

   HRESULT CreateFontCollection(
      CString strFontFile,
      OUT IDWriteFontCollection** result
   );

private:
   ComPtr<IDWriteFactory>                              m_pWriteFactory;
   std::shared_ptr<CD2DFontCollectionLoader>                m_pFontCollectionLoader;
};

