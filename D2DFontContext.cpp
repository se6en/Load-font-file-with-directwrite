#include "stdafx.h"
#include "D2DFontContext.h"
#include "D2DFontCollectionLoader.h"

CD2DFontContext::CD2DFontContext(ComPtr<IDWriteFactory> pFactory)
{
   m_pWriteFactory = pFactory;
   m_pFontCollectionLoader = std::make_shared<CD2DFontCollectionLoader>();
}

CD2DFontContext::~CD2DFontContext()
{
   m_pWriteFactory->UnregisterFontCollectionLoader((IDWriteFontCollectionLoader*)m_pFontCollectionLoader.get());
}

HRESULT CD2DFontContext::Initialize()
{
   if (m_pFontCollectionLoader == nullptr)
   {
      return E_FAIL;
   }

   HRESULT hr = m_pWriteFactory->RegisterFontCollectionLoader((IDWriteFontCollectionLoader*)m_pFontCollectionLoader.get());

   return hr;
}

HRESULT CD2DFontContext::CreateFontCollection(CString strFontFile, OUT IDWriteFontCollection** result)
{
   *result = NULL;

   if (m_pFontCollectionLoader == nullptr)
   {
      return E_FAIL;
   }

   m_pFontCollectionLoader->SetFontFilePath(strFontFile);

   HRESULT hr = S_OK;

   UINT collectionKey = 0;
   std::vector<unsigned int> cKeys = std::vector<unsigned int>(0);

   cKeys.push_back(collectionKey);
   const void *fontCollectionKey = &cKeys.back();
   UINT32 keySize = sizeof(collectionKey);

   hr = Initialize();
   if (FAILED(hr))
   {
      return hr;
   }

   hr = m_pWriteFactory->CreateCustomFontCollection((IDWriteFontCollectionLoader*)m_pFontCollectionLoader.get(),
      fontCollectionKey,
      keySize,
      result
   );

   return hr;
}
