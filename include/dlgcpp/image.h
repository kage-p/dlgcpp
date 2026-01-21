#pragma once

#include <ostream>
#include <string>

namespace dlgcpp
{
    class ImageSource
    {
    public:
        ImageSource(const std::string& id = {}, bool isIcon = false, bool isFile = false) :
            _id(id),
            _isIcon(isIcon),
            _isFile(isFile)
        {
        }

        ImageSource(const ImageSource& other) :
            _id(other._id),
            _isIcon(other._isIcon),
            _isFile(other._isFile)
        {
        }

        // location of a file or resource
        inline const std::string& id() const { return _id; }

        // image is an icon, else a bitmap
        inline bool isIcon() const { return _isIcon; }

        // id is a file, else a resource
        inline bool isFile() const { return _isFile; }

        inline bool operator==(const ImageSource& other) const
        {
            return (
                _id == other._id &&
                _isIcon == other._isIcon &&
                _isFile == other._isFile);
        }

        inline bool operator!=(const ImageSource& other) const
        {
            return !(*this == other);
        }

        friend std::ostream& operator<<(std::ostream& os, const ImageSource& obj)
        {
            return os <<
                "{ id: " << obj._id <<
                ", isIcon: " << obj._isIcon <<
                ", isFile: " << obj._isFile <<
                " }";
        }

    private:
        std::string _id;
        bool _isIcon = false;
        bool _isFile = false;
    };
}