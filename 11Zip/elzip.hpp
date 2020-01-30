#ifndef ZIP_HPP
#define ZIP_HPP

#include <filesystem>

namespace elz
{
    bool extractZip(const std::string& zipname, const std::string& target = ".");
}

#endif // ZIP_HPP