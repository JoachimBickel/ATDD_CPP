#pragma once

#include <string>

#include <viewer/ports/ModelSource.h>

namespace viewer::qt {

// Filesystem adapter for the ModelSource port: reads model files from disk.
class FileModelSource : public ports::ModelSource {
public:
    std::string read(const std::string& path) override;
};

}  // namespace viewer::qt
