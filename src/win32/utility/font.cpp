#include "font.h"
#include "string.h"

#include <strsafe.h>

using namespace dlgcpp;

HFONT dlgcpp::toGdiFont(const Font& font)
{
    LOGFONTW lf = {};
    StringCchCopyW(lf.lfFaceName, LF_FACESIZE, toWide(font.family).c_str());

    HDC hdc = GetDC(nullptr);
    lf.lfHeight = -MulDiv(abs(font.pointSize), GetDeviceCaps(hdc, LOGPIXELSY), 72);
    ReleaseDC(nullptr, hdc);

    lf.lfWeight = font.bold ? FW_BOLD : FW_NORMAL;
    lf.lfItalic = font.italic ? TRUE : FALSE;
    lf.lfUnderline = font.underline;
    lf.lfCharSet = font.symbolType ? SYMBOL_CHARSET : DEFAULT_CHARSET;

    return CreateFontIndirectW(&lf);
}

Font dlgcpp::toFont(HFONT gdiFont)
{
    Font font;

    LOGFONT lf = {};
    if (GetObject(gdiFont, sizeof(LOGFONT), &lf) == 0)
    {
        // Handle error
        return font;
    }

    // Set face name (assume ANSI for std::string)
    font.family = toBytes(lf.lfFaceName);

    // Convert height to point size
    HDC hdc = GetDC(nullptr);
    int dpi = GetDeviceCaps(hdc, LOGPIXELSY);
    ReleaseDC(nullptr, hdc);

    if (lf.lfHeight < 0)
        font.pointSize = -MulDiv(lf.lfHeight, 72, dpi); // negative height = character height
    else
        font.pointSize = MulDiv(lf.lfHeight, 72, dpi);  // positive height = cell height

    // Set style flags
    font.bold = (lf.lfWeight >= FW_BOLD);
    font.italic = lf.lfItalic != FALSE;
    font.underline = lf.lfUnderline != FALSE;

    // Symbol font detection (basic)
    font.symbolType = (lf.lfCharSet == SYMBOL_CHARSET);

    return font;
}