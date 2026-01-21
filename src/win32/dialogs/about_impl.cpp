#include "about_impl.h"
#include "dlgcpp/dialogs/dialog.h"
#include "utility/string_encoder.h"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <shellapi.h>

using namespace dlgcpp;
using namespace dlgcpp::controls;
using namespace dlgcpp::dialogs;

AboutDialogImpl::AboutDialogImpl(
    AboutDialog* aboutDialog) :
    _aboutDialog(aboutDialog)
{
}

void AboutDialogImpl::openUrl(ISharedDialog parent, const std::string& url) const
{
    HWND hwnd = reinterpret_cast<HWND>(parent->handle().value());

    auto urlWide = StringEncoder::toWide(url);

    ShellExecuteW(hwnd, L"open", urlWide.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

void AboutDialogImpl::openSystemInfo(ISharedDialog parent)
{
    auto hwnd = reinterpret_cast<HWND>(parent->handle().value());
    ShellExecuteW(hwnd, L"open", L"MSINFO32.EXE", NULL, NULL, SW_SHOWNORMAL);
}
