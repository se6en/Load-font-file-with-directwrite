# Load font file with DirectWrite
# 10/14/2020
Show how to load font from font file through DirectWrite without installing the font. Also use the font to draw text.

## Sample project files
---
### D2DFontFileEnumerator(.h and .cpp)
---
This was used to load font file with CreateFontFileReference function of IDWriteFactory. The description can be found on msdn (https://docs.microsoft.com/en-us/windows/win32/api/dwrite/nf-dwrite-idwritefactory-createfontfilereference).

This inherit IDWriteFontFileEnumerator class, so the function of MoveNext and GetCurrentFontFile will be called several times due to the font file count. Since we just support one font file in this case, we just check the "hasCurrentFile" parameter in MoveNext function. If this parameter is TRUE, it means that the font file has been loaded before. We just return S_OK and make the hasCurrentFile to FALSE to stop the loop.

### D2DFontCollectionLoader(.h and .cpp)
This inherit from IDWriteFontCollectionLoader. In my perspective, it is more like a linker between the font context and font file enumerator. IDWriteFactory need a pointer to IDWriteFontCollectionLoader while creating font with font file, but it should also need to know the related IDWriteFontFileEnumerator. This font loader create the IDWriteFontFileEnumerator and return the pointer while the CreateEnumeratorFromKey function was called. You can find more information about IDWriteFontCollectionLoader in : https://docs.microsoft.com/en-us/windows/win32/api/dwrite/nn-dwrite-idwritefontcollectionloader

### D2DFontContext(.h and .cpp)
We use CreateCustomFontCollection API of IDWriteFactory to create font directly through font file. You can see more about this function on msdn (https://docs.microsoft.com/en-us/windows/win32/api/dwrite/nf-dwrite-idwritefactory-createcustomfontcollection).

Also, we can create fonts with multiple font files, the paramters collectionKey and collectionKeySize can be used to do that. But since I have not try that myself, I will just leave it here and may do this in the future.

### D2DDrawTextCtrl(.h and .cpp)
This is the window we used to create DirectWrite and draw text. The only thing here we need to notice that the font name must be the same as the name in the font file. For example, we click the .TTF file without installing it, we can find that the name is "Harry P", so the name we used to SetFontFamilyName must be the same as "Harry P". If the name was wrong, the font set will failed and the text will still be draw in the original font. And the font file path can be get in anyway you want.
![Image](https://raw.githubusercontent.com/se6en/Load-font-file-with-directwrite/master/Font_File_Thumbnail.png)
>>The font file must be copied to the same folder as *.exe before run the application.

