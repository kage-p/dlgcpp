#include "image_reader.h"
#include "string_encoder.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <objbase.h>

#include <gdiplus.h>

using namespace dlgcpp;

/// <summary>
/// Loads a bitmap-based image such as bmp/jpg/png/gif, etc.
/// </summary>
HBITMAP ImageReader::loadWithGDIPlus(
    const ImageSource& image,
    const dlgcpp::Size& desiredSize,
    dlgcpp::Color maskColor)
{
    // Initialize GDI+ once
    static bool gdiPlusInitialized = false;
    static ULONG_PTR gdiplusToken = 0;
    if (!gdiPlusInitialized)
    {
        Gdiplus::GdiplusStartupInput gdiplusStartupInput;
        Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
        gdiPlusInitialized = true;
    }

    std::shared_ptr<Gdiplus::Bitmap> source;

    if (image.isFile())
    {
        // load image from file
        source = std::make_shared<Gdiplus::Bitmap>(StringEncoder::toWide(image.id()).c_str());
    }
    else
    {
        // load image from resource
        HRSRC hRes = FindResourceW(nullptr, StringEncoder::toWide(image.id()).c_str(), RT_RCDATA);
        if (!hRes)
        {
            DLGCPP_CERR("Image resource not found: " << image.id());
            return nullptr;
        }

        HGLOBAL hGlob = LoadResource(nullptr, hRes);
        if (!hGlob)
        {
            DLGCPP_CERR("Failed to load image resource: " << image.id());
            return nullptr;
        }

        void* pResData = LockResource(hGlob);
        DWORD size = SizeofResource(nullptr, hRes);
        if (!pResData || size == 0)
        {
            DLGCPP_CERR("Invalid image resource data: " << image.id());
            return nullptr;
        }

        // Create stream from memory
        HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, size);
        if (!hGlobal)
        {
            DLGCPP_CERR("GlobalAlloc() failed for image " << image.id());
            return nullptr;
        }

        void* pData = GlobalLock(hGlobal);
        memcpy(pData, pResData, size);
        GlobalUnlock(hGlobal);

        IStream* pStream = nullptr;
        if (CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) != S_OK)
        {
            GlobalFree(hGlobal);
            DLGCPP_CERR("CreateStreamOnHGlobal() failed for image " << image.id());
            return nullptr;
        }

        source = std::make_shared<Gdiplus::Bitmap>(pStream);
        pStream->Release();
    }

    if (!source || source->GetLastStatus() != Gdiplus::Ok)
    {
        DLGCPP_CERR("Failed to load image " << image.id());
        return nullptr;
    }

    UINT targetW = desiredSize.empty() ? source->GetWidth() : desiredSize.width();
    UINT targetH = desiredSize.empty() ? source->GetHeight() : desiredSize.height();

    Gdiplus::Bitmap composed(targetW, targetH, source->GetPixelFormat());
    Gdiplus::Graphics g(&composed);

    // Handle mask color or transparency
    bool useMask = (maskColor != dlgcpp::Color::None && maskColor != Color::Default);
    Gdiplus::Color bgColor = useMask
        ? Gdiplus::Color(GetRValue((COLORREF)maskColor), GetGValue((COLORREF)maskColor), GetBValue((COLORREF)maskColor))
        : Gdiplus::Color(0, 0, 0, 0); // full transparent

    // Always fill the background
    g.SetCompositingMode(Gdiplus::CompositingModeSourceCopy);
    g.Clear(bgColor);

    // Draw image
    g.SetCompositingMode(Gdiplus::CompositingModeSourceOver);
    g.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
    g.DrawImage(source.get(), 0, 0, targetW, targetH);

    HBITMAP hBitmap = nullptr;
    if (composed.GetHBITMAP(bgColor, &hBitmap) != Gdiplus::Ok)
    {
        DLGCPP_CERR("GetHBITMAP() failed for image " << image.id());
        return nullptr;
    }

    return hBitmap;
}

/// <summary>
/// Loads an image from ImageSource and returns it's handle (or null if load failed)
/// Optional pixel size can be specified to resize the image.
/// </summary>
HANDLE ImageReader::load(
    const ImageSource& image,
    Size& sizePx,
    dlgcpp::Color maskColor)
{
    if (image.id().empty())
        return NULL;

    auto imageType = (image.isIcon() ? IMAGE_ICON : IMAGE_BITMAP);

    UINT loadFlags = 0; //LR_CREATEDIBSECTION;

    HINSTANCE hInstRes = NULL;

    if (image.isFile())
    {
        loadFlags |= LR_LOADFROMFILE;
    }
    else
    {
        hInstRes = GetModuleHandle(NULL);
    }

    if (sizePx.empty())
        loadFlags |= LR_DEFAULTSIZE;

    HANDLE hImage = LoadImageW(
        hInstRes,
        StringEncoder::toWide(image.id()).c_str(),
        imageType,
        sizePx.width(),
        sizePx.height(),
        loadFlags);

    if (hImage == NULL)
    {
        hImage = loadWithGDIPlus(
            image,
            sizePx,
            maskColor);
        if (hImage == NULL)
        {
            sizePx = Size();
            DLGCPP_CERR("Failed to load image with id " << image.id());
            return NULL;
        }
    }

    sizePx = Size();

    if (!image.isIcon())
    {
        auto bitmap = BITMAP();
        if (GetObject(hImage, sizeof(BITMAP), &bitmap) == FALSE)
        {
            DeleteObject(hImage);
            return NULL;
        }

        sizePx = Size(bitmap.bmWidth, bitmap.bmHeight);
    }
    else
    {
        ICONINFO iconInfo;
        if (GetIconInfo((HICON)hImage, &iconInfo) == FALSE)
        {
            DeleteObject(hImage);
            return NULL;
        }

        auto bitmap = BITMAP();
        if (GetObject(iconInfo.hbmColor, sizeof(BITMAP), &bitmap))
        {
            sizePx = Size(bitmap.bmWidth, bitmap.bmHeight);
        }

        DeleteObject(iconInfo.hbmColor);
        DeleteObject(iconInfo.hbmMask);
    }
    return hImage;
}
