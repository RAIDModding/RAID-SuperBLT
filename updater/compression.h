#include <string>
#include <memory>

namespace raidhook
{
    struct ZIPFileData
    {
        std::string filepath;
        std::string decompressedData;
        int compressedSize;
        int uncompressedSize;
    };

    bool ExtractZIPArchive(const std::string& path, const std::string& extractPath);
}
