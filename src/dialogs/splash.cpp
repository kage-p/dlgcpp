#include "splash.h"
#include "../utility.h"

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::dialogs;

SplashDialog::SplashDialog(std::shared_ptr<IDialog> parent)
    : _parent(parent)
{     
}

const std::string& SplashDialog::logoBitmapId() const
{
    return _props.logoBitmapId;
}

void SplashDialog::logoBitmapId(const std::string& value)
{
    _props.logoBitmapId = value;
}

const std::string& SplashDialog::message() const
{
    return _props.message;
}

void SplashDialog::message(const std::string& value)
{
    _props.message = value;
}

int SplashDialog::timeout() const
{
    return _props.timeout;
}

void SplashDialog::timeout(int value)
{
    _props.timeout = value;
}

void SplashDialog::show()
{
    _splashDialog.reset();

    if (_props.logoBitmapId.empty())
        return;

    auto dlg = std::make_shared<Dialog>(_parent);
    _splashDialog = dlg;
    dlg->type(DialogType::Frameless);

    auto logoImage = std::make_shared<Image>(dlg, Position{0, 0, 0, 0});
    logoImage->colors(Color::Black, Color::White);
    logoImage->autoSize(true);
    logoImage->imageId(_props.logoBitmapId);
    dlg->add(logoImage);

    auto pos = logoImage->p();

    if (!_props.message.empty())
    {
        auto messageLabel = std::make_shared<Label>(dlg, _props.message, Position{3,pos._cy-15,pos._cx-6,12});
        messageLabel->font(Font{"sans serif", 8, true});
        messageLabel->colors(Color::LtGray, Color::Blue);
        dlg->add(messageLabel);
        BringWindowToTop((HWND)messageLabel->handle());
    }

    dlg->resize(pos._cx, pos._cy);
    dlg->center();
    dlg->visible(true);
    SetWindowPos((HWND)dlg->handle(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    dlg->ClickEvent() += [this](){
        _splashDialog->close();
    };

    if (_props.timeout > 0)
    {
        dlg->timer(_props.timeout, [this](){
            _splashDialog->close();
        });
    }
}

void SplashDialog::close()
{
    _splashDialog.reset();
}
/*

//------------------------------------------------------------------------------
// Displays splash dialog
//
//  (opt) hwndParent is the handle of a window to highlight after closure.
//  (opt) dwDelay is number of milliseconds to display.
//  nModeless is nonzero to return from the function immediately.
//  szBitmapID is either a resource ID or file name.
//  (opt) IsFile is nonzero if sBitmapID is a file name, else it is a resource.
//  (opt) szDispText is a string to display on the screen.
//  (opt) clrDispText is the color of the displayed text, if any.
//  (opt) nTransparent is TRUE if the bitmap is transparent (regions). First pixel is used.
//
// Returns HWND if modeless, or zero if modal.
//------------------------------------------------------------------------------
LIBDLGPROC(HWND) dlgSplash(HWND hwndParent,
                           TWORD32 dwDelay,
                           TINT nModeless,
                           const TSTR *pszBitmapID,
                           TINT nIsFile,                // Optional
                           const TSTR *pszDispText,     // Optional
                           TRGBA clrDispText,           // Optional
                           TINT nTransparent)           // Optional
{

    HWND hDlg;
    HBITMAP hBmp;
    HRGN hRegion = 0;
    BITMAP tObj;

    gRDLG_nSplashActive = TRUE;

    // Default delay...
    if (!dwDelay) dwDelay = 2250;

    if (nIsFile)
        // Load from file...
        hBmp = (HBITMAP)LoadImage(0, pszBitmapID, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    else
        // Load from resource...
        hBmp = (HBITMAP)LoadImage(GetModuleHandle(NULL), pszBitmapID, IMAGE_BITMAP, 0,0,0);

    // Get size of bitmap and size window accordingly...
    if (!GetObject(hBmp, sizeof(BITMAP), &tObj)) return 0;

    // Create the dialog...
    hDlg = dlgNew(HWND_DESKTOP, NULL, 0, 0, 0, 0, WS_POPUP, WS_EX_TOOLWINDOW);
    SetWindowPos(hDlg, HWND_TOPMOST, (GetSystemMetrics(SM_CXSCREEN)/2)-(tObj.bmWidth/2),
                 (GetSystemMetrics(SM_CYSCREEN)/2)-(tObj.bmHeight/2),
                 tObj.bmWidth, tObj.bmHeight, 0);

    if (nTransparent)
    {
        hRegion = gdtGetRegion(hBmp, (TRGBA)-1);
        if (hRegion) SetWindowRgn(hDlg, hRegion, TRUE);
    }



    // Save window handle for activation...
    dlgUserSet(hDlg, 1, (LPVOID)hwndParent);
    dlgUserSet(hDlg, 2, (LPVOID)hBmp);
    dlgUserSet(hDlg, 3, (LPVOID)hRegion);
    SetTimer(hDlg, 100, dwDelay, 0);


    if (!nModeless)
    {
        dlgShowModal(hDlg, &Internal_dlgSplashProc, 0);
        return 0;
    }
    else
    {
        dlgShowModeless(hDlg, &Internal_dlgSplashProc, SW_SHOW, 0);
        return hDlg;
    }

}

LRESULT CALLBACK Internal_dlgSplashProc(HWND hDlg, UINT wMsg, WPARAM, LPARAM)
{

    HBITMAP hBmp;
    HRGN hRegion;
    HWND hwndParent;
    HDC hdc;
    PAINTSTRUCT ps;

    switch (wMsg)
    {
    case WM_TIMER:
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_KEYDOWN:
        // React to mouse or key movements, or when the dialog times out...
        KillTimer(hDlg, 100);
        gRDLG_nSplashActive = FALSE;

        // Activate chosen window, if any...
        hwndParent = (HWND)dlgUserGet(hDlg, 1);
        if (IsWindow(hwndParent)) SetForegroundWindow(hwndParent);
        dlgEnd(hDlg, 0);
        break;

    case WM_PAINT:
        // Draw the bitmap directly on the window...
        hBmp = (HBITMAP)dlgUserGet(hDlg, 2);
        BeginPaint(hDlg, &ps);
        hdc = CreateCompatibleDC(ps.hdc);
        SelectObject(hdc, hBmp);
        BitBlt(ps.hdc, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right-ps.rcPaint.left, ps.rcPaint.bottom-ps.rcPaint.top, hdc, ps.rcPaint.left, ps.rcPaint.top, SRCCOPY);
        DeleteDC(hdc);
        EndPaint(hDlg, &ps);
        return 0;

    case WM_DESTROY:
        // Free objects...
        hBmp = (HBITMAP)dlgUserGet(hDlg, 2);
        hRegion = (HRGN)dlgUserGet(hDlg, 3);
        DeleteObject(hBmp);
        if (hRegion) DeleteObject(hRegion);
        return 0;
    }

    return 0;

}


*/
