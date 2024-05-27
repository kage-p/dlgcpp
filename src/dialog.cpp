#include "dialog.h"
#include "control.h"
#include "utility.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
//#include <CommDlg.h>
//#include <ShellAPI.h>
//#include <ShlObj.h>

using namespace dlgcpp;

INT_PTR WINAPI DialogWndProc(HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam);

Dialog::Dialog(std::shared_ptr<IDialog> parent) :
    _parent(parent)
{
    _props._p._cx = 600;
    _props._p._cy = 400;
    _rebuild();
}

Dialog::~Dialog()
{
    _dump();
}

int Dialog::nextId()
{
    auto id = _nextId;
    _nextId++;
    return id;
}

int Dialog::exec()
{
    auto hwndDlg = reinterpret_cast<HWND>(_props._hwnd);

    auto msg = MSG();
    while (IsWindow(hwndDlg))
    {
        if (GetMessage(&msg, NULL, 0, 0) > 0)
        {
            //if (!pDlgData->hAccel || !TranslateAccelerator(hDlg, pDlgData->hAccel, &msg))
            {
                if (!IsDialogMessage(hwndDlg, &msg))
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
            }
        }
    }
    return 0; // TODO: get the result code
}

bool Dialog::visible() const
{
    return _props._visible;
}

void Dialog::visible(bool value)
{
    _props._visible = value;
}

const Position& Dialog::p() const
{
    return _props._p;
}

void Dialog::p(const Position& p)
{
    _props._p = p;

    if (_props._hwnd == NULL)
        return;

    auto hwnd = reinterpret_cast<HWND>(_props._hwnd);

    // Convert units to pixels
    auto rc = RECT();
    SetRect(&rc, p._x, p._y, p._cx, p._cy);
    MapDialogRect(hwnd, &rc);

    bool redraw = true;
    SetWindowPos(hwnd, 0, rc.left, rc.top, rc.right, rc.bottom, SWP_NOZORDER | (redraw ? 0 : SWP_NOREDRAW) );
}

const std::string& Dialog::title() const
{
    return _props._text;
}

void Dialog::title(const std::string& value)
{
    _props._text = value;
    if (_props._hwnd != NULL)
        SetWindowTextW(reinterpret_cast<HWND>(_props._hwnd),
                       toWide(_props._text).c_str());
}

void* Dialog::handle()
{
    return _props._hwnd;
}

std::shared_ptr<IDialog> Dialog::parent()
{
    return _parent;
}

const std::vector<std::shared_ptr<IControl>>& Dialog::children() const
{
    return _children;
}

void Dialog::close()
{
    _dump();
}

std::shared_ptr<IDialog> Dialog::dialog(const std::string& title, const Position& p)
{
    // TODO: complete child dlg impl.
    auto dialog = std::make_shared<Dialog>(*this);
    //dialog->id(nextId());
    dialog->p(p);
    dialog->title(title);
    //add(dialog);
    return dialog;
}

std::shared_ptr<IControl> Dialog::button(const std::string& text, const Position& p)
{
    auto control = std::make_shared<Button>(*this);
    control->id(nextId());
    control->p(p);
    control->text(text);
    add(control);
    return control;
}

std::shared_ptr<IControl> Dialog::label(const std::string& text, const Position& p)
{
    auto control = std::make_shared<Label>(*this);
    control->id(nextId());
    control->p(p);
    control->text(text);
    add(control);
    return control;
}

std::shared_ptr<IControl> Dialog::listBox(const Position& p)
{
    auto control = std::make_shared<ListBox>(*this);
    control->id(nextId());
    control->p(p);
    add(control);
    return control;
}

std::shared_ptr<IControl> Dialog::textBox(const std::string& text, const Position& p)
{
    auto control = std::make_shared<TextBox>(*this);
    control->id(nextId());
    control->p(p);
    control->text(text);
    add(control);
    return control;
}

void Dialog::add(std::shared_ptr<IControl> child)
{
    auto it = std::find(_children.begin(), _children.end(), child);
    if (it != _children.end())
        return;
    _children.push_back(child);
}

void Dialog::remove(std::shared_ptr<IControl> child)
{
    auto it = std::find(_children.begin(), _children.end(), child);
    if (it == _children.end())
        return;
    _children.erase(it);
}

void Dialog::_rebuild()
{
    _dump();

    // Use the Windows dialog font
    std::wstring fontFace = L"MS Shell Dlg";
    unsigned short fontSize = 8; // Pointsize
    std::wstring text;

#ifdef UNICODE
    text = _props._text;
#else
    text = toWide(_props._text);
#endif
    size_t cbCaption = text.size();

    // Check params first...
    unsigned int styles = WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_BORDER | DS_CENTER | DS_MODALFRAME; // Default style is modal popup dialog.
    unsigned int exStyles = 0;

    if (_props._visible)
        styles |= WS_VISIBLE;

    size_t cbFont = fontFace.size();

    // Params ok. Create heap for dialog...
    size_t cbVoid = sizeof(DLGTEMPLATE) + 4 + ((cbCaption+1) * 2) + ((cbFont+1) * 2) + 2;
    DLGTEMPLATE* dlg = (DLGTEMPLATE*)GlobalAlloc(GPTR, cbVoid);
    if (dlg == NULL)
        return;

    dlg->style = styles | DS_3DLOOK | DS_SETFONT | DS_SETFOREGROUND;
    dlg->dwExtendedStyle = exStyles;
    dlg->x = (short)_props._p._x;
    dlg->y = (short)_props._p._y;
    dlg->cx = (short)_props._p._cx;
    dlg->cy = (short)_props._p._cy;

    // WSTR:
    size_t offset = sizeof(DLGTEMPLATE);

    // menu
    offset += 2; // Terminator

    // dialog class
    offset += 2; // Terminator

    // caption
    if (cbCaption > 0)
    {
        memcpy((char*)dlg + offset, text.c_str(), (cbCaption * 2));
        offset += (cbCaption * 2);
    }
    offset += 2; // Terminator

    if (cbFont > 0)
    {
        // font (optional)
        memcpy((char*)dlg + offset, &fontSize, 2);
        offset += 2;
        memcpy((char*)dlg + offset, fontFace.c_str(), (cbFont * 2));
        offset += (cbFont * 2);
    }
    //offset += 2; // Terminator

    HWND hwndParent = NULL;
    if (_parent != nullptr)
        hwndParent = reinterpret_cast<HWND>(_parent->handle());

    // Create the dialog, passing private data struct
    auto hwnd = CreateDialogIndirectParam(GetModuleHandle(NULL), dlg, hwndParent, &DialogWndProc, 0);
    GlobalFree(dlg);

    if (hwnd == NULL)
        return;
    SetProp(hwnd, "this", this);
    _props._hwnd = hwnd;
}

void Dialog::_dump()
{
    if (_props._hwnd == NULL)
        return;

    // it does not use EndDialog as we may be simulating a model dialog with exec()
    auto hwnd = reinterpret_cast<HWND>(_props._hwnd);
    DestroyWindow(hwnd);
    _props._hwnd = nullptr;
}

INT_PTR WINAPI DialogWndProc(HWND hDlg, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
    auto pthis = reinterpret_cast<Dialog*>(GetProp(hDlg, "this"));

    switch (wMsg)
    {
    case WM_SYSCOMMAND:
        // Close X button on dialog...
        if ((wParam & 0xFFF0) == SC_CLOSE)
        {
            if (pthis != nullptr)
                pthis->close();
        }
        break;
    }

    return 0;
}
