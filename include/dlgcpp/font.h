#pragma once

#include <ostream>
#include <string>

namespace dlgcpp
{
    class Font
    {
    public:
        Font(
            const std::string& family = {},
            int pointSize = 8,
            bool bold = false,
            bool italic = false,
            bool underline = false,
            bool symbolType = false) :
            _family(family),
            _pointSize(pointSize),
            _bold(bold),
            _italic(italic),
            _underline(underline),
            _symbolType(symbolType)
        {
        }

        Font(const Font& other) :
            _family(other._family),
            _pointSize(other._pointSize),
            _bold(other._bold),
            _italic(other._italic),
            _underline(other._underline),
            _symbolType(other._symbolType)
        {
        }

        // font family name
        inline const std::string& family() const { return _family; }
        // font size in points
        inline int pointSize() const { return _pointSize; }
        // bold text
        inline bool bold() const { return _bold; }
        // italic text
        inline bool italic() const { return _italic; }
        // underline text
        inline bool underline() const { return _underline; }
        // symbol font (e.g. Wingdings)
        inline bool symbolType() const { return _symbolType; }

        inline bool operator==(const Font& other) const
        {
            return (_family == other._family &&
                _pointSize == other._pointSize &&
                _bold == other._bold &&
                _italic == other._italic &&
                _underline == other._underline &&
                _symbolType == other._symbolType);
        }

        inline bool operator!=(const Font& other) const
        {
            return !(*this == other);
        }

        friend std::ostream& operator<<(std::ostream& os, const Font& obj)
        {
            return os <<
                "{ family: " << obj._family <<
                ", pointSize: " << obj._pointSize <<
                ", bold: " << obj._bold <<
                ", italic: " << obj._italic <<
                ", underline: " << obj._underline <<
                ", symbolType: " << obj._symbolType <<
                " }";
        }

    private:
        std::string _family;
        int _pointSize = 8;
        bool _bold = false;
        bool _italic = false;
        bool _underline = false;
        bool _symbolType = false;
    };
}