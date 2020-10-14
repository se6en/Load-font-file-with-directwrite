#pragma once

using namespace Microsoft::WRL;

class CD2DFontFileEnumerator : public IDWriteFontFileEnumerator
{
public:
   CD2DFontFileEnumerator(IDWriteFactory* pWriteFactory);

   virtual ULONG STDMETHODCALLTYPE AddRef();
   virtual ULONG STDMETHODCALLTYPE Release();
   virtual HRESULT STDMETHODCALLTYPE QueryInterface(_In_ REFIID riid, _Outptr_ void** ppOutput);

   STDMETHOD(MoveNext)(_Out_ BOOL* hasCurrentFile);
   STDMETHOD(GetCurrentFontFile)(_COM_Outptr_ IDWriteFontFile** fontFile);

   void SetFontFilePath(CString strFontFilePath);

private:
   LONG                      m_refCount;
   IDWriteFactory*           m_pWriteFactory;
   ComPtr<IDWriteFontFile>   m_pWriteFontFile;
   CString                   m_strFontFilePath;

};
