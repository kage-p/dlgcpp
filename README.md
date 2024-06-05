
# DLGCPP

DLGCPP allows you to construct your user interfaces quickly and reliably and with less complexity.

The framework is intended to be lightweight with no third-party dependencies, using C++17 constructs and the native Win32 API

## Syntax 

A set of classes are provided which encapsulate the Win32 DIALOG functions

Code safety is achieved through the use of shared_ptr instead of unsafe memory pointers. Class interfaces are also provided to support unit testing of the calling code

## Components

Support for dialog boxes and various native controls (labels, buttons, text and list boxes) is provided. Support for system dialogs, including file, folder and message boxes are also available. It is also possible to use custom controls which have been registered using the RegisterClass function and follow standard Win32 control design patterns.

Positioning is performed using dialog units (DUs) and various common properties including fonts, cursors, and colors are supported for all controls. Many controls support additional properties depending on native support.

## Events

Events are supported by both dialogs and controls by exposing an Event object to which handlers can be attached using the += operator.
The native window handle (HWND) of dialogs and controls can be accessed directly for further customisation, and the dialog window procedure (DLGPROC) supports override.

## Example

User interfaces can be quickly constructed using standard C++ method calls, as shown in the following example:

    #include "dlgcpp/dlgcpp.h"
    -
    int main()
    {
        auto dlg = std::make_shared<Dialog>();
        dlg->resize(500,250);
        dlg->center();
    -
        auto label = std::make_shared<Label>(dlg, "Hello DLGCPP!", Position{10, 10, 100, 15});
        label->colors(Color::Blue, Color::White);
        dlg->add(label);
    -
        dlg->exec();
        return 0;
    }

The first group of statements initiate a dialog box window, sizing and centering it. The second group creates a text label and adds color customisation. The final exec() statement displays the modal dialog box and starts a message (event) processing loop, which continues to execute until the dialog is closed..

## Integration

The framework can be easily integrated into a CMAKE project structure, and only needs a single header "dlgcpp.h" to be included

A small demo app 'dlgdemo' is included within the project. This can be excluded from the build by setting DLGCPP_DEMO to OFF in your CMAKE script.

## License

DLGCPP is open source and licensed under the Apache 2.0 permissive license. You are welcome to use it for free

If you do use DLGCPP, please include a reference to the author somewhere in your application, similar to:

    Portions use the DLGCPP library, (c) Kevin Peel 2024. See www.kgpsoftware.com for details
