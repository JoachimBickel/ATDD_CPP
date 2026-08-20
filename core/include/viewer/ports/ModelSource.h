#pragma once

#include <string>

namespace viewer::ports {

// Outbound port: supplies the raw contents of a model file. Implemented by an
// adapter (e.g. the filesystem); faked in tests so parsing needs no real files.
class ModelSource {
public:
    virtual ~ModelSource() = default;
    virtual std::string read(const std::string& path) = 0;
};

}  // namespace viewer::ports
