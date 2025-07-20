#pragma once

#include <string>

namespace dlgcpp
{
    struct ImageSource
    {
        // location of a file or resource
        std::string id;
        // image is an icon, else a bitmap
        bool isIcon = false;
        // id is a file, else a resource
        bool isFile = false;

        inline bool operator==(const ImageSource& other) const
        {
            return (id == other.id &&
                isIcon == other.isIcon &&
                isFile == other.isFile);
        }
        inline bool operator!=(const ImageSource& other) const
        {
            return !(*this == other);
        }
    };
}