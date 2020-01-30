#include "elzip.hpp"
#include "unzipper.h"
#include <fstream>

namespace fs = std::filesystem;

namespace elz
{
    bool extractZip(const std::string& zipName, const std::string& target)
    {
        ziputils::unzipper zipFile;
        zipFile.open(zipName.c_str());
        for (const auto& filename : zipFile.getFilenames())
        {
            fs::path cDir(target + (fs::path(filename).parent_path().string().empty() ? "" : "/") + fs::path(filename).parent_path().string());
            fs::path cFile(target + "/" + filename);
            fs::path fillPath;
            for (const auto& pathPart : cDir) {
                fillPath /= pathPart;
                if (!exists(fillPath)) {
                    create_directory(fillPath);
                    if (!exists(fillPath)) return false;
                }
            }
            zipFile.openEntry(filename.c_str());
            std::ofstream wFile;
            wFile.open(cFile.string());
            if (!wFile.is_open()) return false;
            zipFile >> wFile;
            wFile.close();
        }
        return true;
    }
}