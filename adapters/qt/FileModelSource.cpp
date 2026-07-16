#include "FileModelSource.h"

#include <fstream>
#include <sstream>

namespace viewer::qt {

std::string FileModelSource::read(const std::string& path)
{
    std::ifstream file(path, std::ios::binary);
    std::ostringstream content;
    content << file.rdbuf();
    return content.str();
}

}  // namespace viewer::qt