#include "font_loader.h"
#include "string_encoder.h"

#include <strsafe.h>

using namespace dlgcpp;

HFONT FontLoader::toGdiFont(const Font& font)
{
    LOGFONTW lf = {};
    StringCchCopyW(
        lf.lfFaceName,
        LF_FACESIZE,
        StringEncoder::toWide(font.family()).c_str());

    HDC hdc = GetDC(nullptr);
    lf.lfHeight = -MulDiv(abs(font.pointSize()), GetDeviceCaps(hdc, LOGPIXELSY), 72);
    ReleaseDC(nullptr, hdc);

    lf.lfWeight = font.bold() ? FW_BOLD : FW_NORMAL;
    lf.lfItalic = font.italic() ? TRUE : FALSE;
    lf.lfUnderline = font.underline();
    lf.lfCharSet = font.symbolType() ? SYMBOL_CHARSET : DEFAULT_CHARSET;

    return CreateFontIndirectW(&lf);
}

Font FontLoader::toFont(HFONT gdiFont)
{
    LOGFONT lf = {};
    if (GetObject(gdiFont, sizeof(LOGFONT), &lf) == 0)
    {
        // Handle error
        return Font();
    }

    // Set face name (assume ANSI for std::string)
    auto family = StringEncoder::toBytes(lf.lfFaceName);

    // Convert height to point size
    HDC hdc = GetDC(nullptr);
    int dpi = GetDeviceCaps(hdc, LOGPIXELSY);
    ReleaseDC(nullptr, hdc);

    int pointSize;
    if (lf.lfHeight < 0)
        pointSize = -MulDiv(lf.lfHeight, 72, dpi); // negative height = character height
    else
        pointSize = MulDiv(lf.lfHeight, 72, dpi);  // positive height = cell height

    // Set style flags
    auto bold = (lf.lfWeight >= FW_BOLD);
    auto italic = lf.lfItalic != FALSE;
    auto underline = lf.lfUnderline != FALSE;

    // Symbol font detection (basic)
    auto symbolType = (lf.lfCharSet == SYMBOL_CHARSET);

    return Font(family, pointSize, bold, italic, underline, symbolType);
}