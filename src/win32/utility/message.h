#pragma once

#include "dlgcpp/defs.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

namespace dlgcpp
{
    struct DialogMessage
    {
        UINT wMsg = 0;
        WPARAM wParam = 0;
        LPARAM lParam = 0;

        // the message result code
        LONG_PTR msgResult = 0;
        // the dialog proc result
        LRESULT dlgResult = 0;
    };

    class MessageLockValue
    {
        friend class MessageLocker;

    public:
        inline bool get() const
        {
            return _value;
        }

    protected:
        inline void set(bool value)
        {
            _value = value;
        }

    private:
        bool _value = false;
    };

    class MessageLocker
    {
    public:
        inline MessageLocker(MessageLockValue& f)
            : _flag(f)
        {
            _flag.set(true);
        }

        inline ~MessageLocker()
        {
            _flag.set(false);
        }

    private:
        MessageLockValue& _flag;
    };

    class MessageProcessor
    {
    public:
        static int beginLoop(HWND hwnd, HACCEL hAccel = nullptr);
    };
}
