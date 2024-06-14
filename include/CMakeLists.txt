cmake_minimum_required(VERSION 3.27)

set(target dlgcpp)

set(headers
    dlgcpp/dlgcpp.h
    dlgcpp/child.h
    dlgcpp/control.h
    dlgcpp/defs.h
    dlgcpp/dialog.h
    dlgcpp/event.h
    dlgcpp/menu.h
    dlgcpp/controls/button.h
    dlgcpp/controls/checkbox.h
    dlgcpp/controls/combobox.h
    dlgcpp/controls/custom.h
    dlgcpp/controls/image.h
    dlgcpp/controls/label.h
    dlgcpp/controls/listbox.h
    dlgcpp/controls/slider.h
    dlgcpp/controls/textbox.h
    dlgcpp/dialogs/about.h
    dlgcpp/dialogs/file.h
    dlgcpp/dialogs/folder.h
    dlgcpp/dialogs/message.h
    dlgcpp/dialogs/property.h
    dlgcpp/dialogs/splash.h
)

add_library(${target} INTERFACE ${headers})


