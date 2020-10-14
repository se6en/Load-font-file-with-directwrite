#include "stdafx.h"
#include "D2DFontCollectionLoader.h"
#include "D2DFontFileEnumerator.h"

CD2DFontCollectionLoader::CD2DFontCollectionLoader() : m_refCount(1)
{

}

ULONG STDMETHODCALLTYPE CD2DFontCollectionLoader::AddRef()
{
   m_refCount++;
   return m_refCount;
}

ULONG STDMETHODCALLTYPE CD2DFontCollectionLoader::Release()
{
   m_refCount--;

   if (m_refCount == 0)
   {
      delete this;
      return 0;
   }
   else
   {
      return m_refCount;
   }
}

HRESULT STDMETHODCALLTYPE CD2DFontCollectionLoader::QueryInterface(_In_ REFIID riid, _Outptr_ void** ppOutput)
{
   if (riid == IID_IUnknown || riid == __uuidof(IDWriteFontCollectionLoader))
   {
      *ppOutput = this;
      AddRef();
      return S_OK;
   }

   *ppOutput = nullptr;
   return E_NOINTERFACE;
}

HRESULT CD2DFontCollectionLoader::CreateEnumeratorFromKey(
   _In_ IDWriteFactory* factory,
   _In_reads_bytes_(collectionKeySize) void const* collectionKey,
   UINT32 collectionKeySize,
   _COM_Outptr_ IDWriteFontFileEnumerator** fontFileEnumerator
)
{
   *fontFileEnumerator = nullptr;

   HRESULT hr = S_OK;

   if (collectionKeySize % sizeof(UINT) != 0)
   {
      return E_INVALIDARG;
   }

   CD2DFontFileEnumerator* pFontFileEnumerator = new CD2DFontFileEnumerator(factory);

   if (pFontFileEnumerator == nullptr)
   {
      return E_OUTOFMEMORY;
   }

   ASSERT(!m_strFontFilePath.IsEmpty());

   pFontFileEnumerator->SetFontFilePath(m_strFontFilePath);

   UINT const* fontfileCollectionKey = static_cast<UINT const*>(collectionKey);
   UINT32 const fontfileKeySize = collectionKeySize;

   if (FAILED(hr))
   {
      delete pFontFileEnumerator;
      return hr;
   }

   *fontFileEnumerator = (IDWriteFontFileEnumerator*)pFontFileEnumerator;

   return hr;
}

void CD2DFontCollectionLoader::SetFontFilePath(CString strFontFilePath)
{
   m_strFontFilePath = strFontFilePath;
}
