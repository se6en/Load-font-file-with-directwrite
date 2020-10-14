#include "stdafx.h"
#include "D2DFontFileEnumerator.h"

CD2DFontFileEnumerator::CD2DFontFileEnumerator(IDWriteFactory* pWriteFactory) : m_refCount(1)
{
   m_pWriteFactory = pWriteFactory;
}

ULONG STDMETHODCALLTYPE CD2DFontFileEnumerator::AddRef()
{
   m_refCount++;
   return m_refCount;
}

ULONG STDMETHODCALLTYPE CD2DFontFileEnumerator::Release()
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

HRESULT STDMETHODCALLTYPE CD2DFontFileEnumerator::QueryInterface(_In_ REFIID riid, _Outptr_ void** ppOutput)
{
   if (riid == IID_IUnknown || riid == __uuidof(IDWriteFontFileEnumerator))
   {
      *ppOutput = this;
      AddRef();
      return S_OK;
   }

   *ppOutput = nullptr;
   return E_NOINTERFACE;
}

HRESULT CD2DFontFileEnumerator::MoveNext(_Out_ BOOL* hasCurrentFile)
{
   if (*hasCurrentFile == TRUE)
   {
      *hasCurrentFile = FALSE;

      return S_OK;
   }

   *hasCurrentFile = FALSE;
   m_pWriteFontFile = nullptr;

   HRESULT hr = m_pWriteFactory->CreateFontFileReference(m_strFontFilePath, NULL, &m_pWriteFontFile);

   if (SUCCEEDED(hr))
   {
      *hasCurrentFile = TRUE;
   }

   return hr;
}

HRESULT CD2DFontFileEnumerator::GetCurrentFontFile(_COM_Outptr_ IDWriteFontFile** fontFile)
{
   *fontFile = m_pWriteFontFile.Get();

   return m_pWriteFontFile != nullptr ? S_OK : E_FAIL;
}

void CD2DFontFileEnumerator::SetFontFilePath(CString strFontFilePath)
{
   m_strFontFilePath = strFontFilePath;
}