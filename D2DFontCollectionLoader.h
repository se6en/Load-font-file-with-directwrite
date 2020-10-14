#pragma once

class CD2DFontCollectionLoader : public IDWriteFontCollectionLoader
{
public:
   CD2DFontCollectionLoader();

   virtual ULONG STDMETHODCALLTYPE AddRef();
   virtual ULONG STDMETHODCALLTYPE Release();
   virtual HRESULT STDMETHODCALLTYPE QueryInterface(_In_ REFIID riid, _Outptr_ void** ppOutput);

   STDMETHOD(CreateEnumeratorFromKey)(
      _In_ IDWriteFactory* factory,
      _In_reads_bytes_(collectionKeySize) void const* collectionKey,
      UINT32 collectionKeySize,
      _COM_Outptr_ IDWriteFontFileEnumerator** fontFileEnumerator
      );

   void SetFontFilePath(CString strFontFilePath);

private:
   LONG                                          m_refCount;
   CString                                       m_strFontFilePath;

};