
# DLGCPP

DLGCPP allows you to construct your user interfaces quickly and reliably and with less complexity.

The framework is intended to be lightweight, using pure C++17 constructs and the native Win32 API. It does not contain any third-party dependencies

## Syntax 

A set of object classes are provided which encapsulate the Win32 DIALOG functions. With DLGCPP it is possible to create a full application using object-oriented syntax and without using the Win32 API.

Code safety is achieved through the use of shared pointers instead of unsafe memory pointers. Class interfaces are also provided to support unit testing of the calling code. Classes can be extended or new classes added to encapsulate those controls which are not provided by DLGCPP.

All strings are treated as UTF-8 and are converted between the underlying Windows Unicode format.

## Components

Support for dialog boxes and various native controls (e.g. labels, buttons, text and list boxes) is provided. Support for system dialogs, including file, folder and message boxes are also available. It is also possible to use custom controls which have been registered using the RegisterClass function and follow standard Win32 control design patterns.

Dialogs support modeless or modal interation through the exec() method which initiates an event loop for that dialog.
Both dialogs and controls support common properties including fonts, cursors, and colors. Many controls support additional properties depending on native support.

## Events

Events are supported by both dialogs and controls by exposing an Event object to which handlers can be attached using the += operator.
The native window handle (HWND) of dialogs and controls can be accessed through the handle() convenience function, and the dialog window procedure (DLGPROC) supports override to allow processing of all messages sent to the window.

## Example

User interfaces can be quickly constructed using standard C++ method calls, as shown in the following example:

    #include "dlgcpp/dlgcpp.h"
    -
    using namespace dlgcpp;
    using namespace dlgcpp::controls;
    -
    int main()
    {
        auto dlg = std::make_shared<Dialog>();
        dlg->resize({500,250});
        dlg->center();
    -
        auto label = std::make_shared<Label>("Hello DLGCPP!", Position{10, 10, 100, 15});
        label->colors(Color::Blue, Color::White);
        dlg->add(label);
    -
        dlg->exec();
        return 0;
    }

The first group of statements initiate a dialog box window, sizing and centering it. The second group creates a text label with color customisation, and then adds this to the parent dialog. The final exec() statement displays the modal dialog box and starts a message (event) processing loop, which continues to execute until the dialog is closed..

Consumers are required to allocate all DLGCPP classes using shared_ptr. Controls are added to a dialog using the simple dlg->add(control) statement which also invokes the creation function. A reference to the control's shared_ptr is stored within the parent so it does not have to be stored elsewhere.

Dialog and control positioning is calculated using dialog units (DUs). The control position is always relative to the parent dialog.

Two methods for creating the UI are common, the first as shown above is by creating the dialog and controls from an external method. Event handlers would also be allocated either as lambdas or functions.

In the second method, the Dialog class can be inherited with control statements and events enclosed within the derived class. Controls when stored as members of the derived class, can be accessed at any point. Additional customisations can be achieved through overriding protected methods in the derived class. This is similar to the approach used by Windows Forms.

While either approach is fine, if you have a large application it may be better to use the second approach to reduce the amount of code and improve readability.

## Integration

The framework can be easily integrated into a CMAKE project structure, and only requires a single header "dlgcpp.h" to be included by the consumer.

A small demo app 'dlgdemo' is included within the project. This can be excluded from the build by setting DLGCPP_DEMO to OFF in your CMAKE script.

## History

DLGCPP is a port of an older C wrapper library 'libdlg'. This library was originally designed to help with porting both C and BASIC code (e.g. PowerBASIC) to C++ and was first implemented around 2002. It was used in many production applications.

## License

DLGCPP is open source and licensed under the Apache 2.0 permissive license. You are welcome to use it for free

If you do use DLGCPP, please include a reference to the author somewhere in your application, similar to:

    Portions use the DLGCPP library, (c) Kevin Peel 2024. See www.kgpsoftware.com for details
