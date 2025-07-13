#pragma once

#include "defs.h"

#include <memory>
#include <string>
#include <vector>

namespace dlgcpp
{
    class IClipboard;
    typedef std::shared_ptr<IClipboard> ISharedClipboard;

    class IClipboard
    {
    public:
        virtual const std::string& text() const = 0;
        virtual void text(const std::string& value) = 0;
        virtual void clear() = 0;
    };

    class Clipboard : public IClipboard
    {
    public:
        explicit Clipboard();
        virtual ~Clipboard();

        // IClipboard impl.
        const std::string& text() const override;
        void text(const std::string& text) override;
        void clear() override;
    };
}
