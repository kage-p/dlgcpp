#pragma once

#include <ostream>
#include <vector>

namespace dlgcpp
{
    /// <summary>
    /// Drop files event
    /// </summary>
    class DropFilesEvent : public std::vector<std::string>
    {
    public:
        friend std::ostream& operator<<(std::ostream& os, const DropFilesEvent& obj)
        {
            os << "{ ";
            for (const auto& file : obj)
            {
                os << "\"" << file << "\", ";
            }
            os << " }";
            return os;
        }
    };
}