#pragma once

#include <QLabel>
#include <QWidget>

#include <viewer/ports/ModelInfo.h>

namespace viewer::qt {

// The model-info side panel: vertex/triangle counts and bounding-box size.
class InfoPanel : public QWidget {
    Q_OBJECT

public:
    explicit InfoPanel(QWidget* parent = nullptr);

    void showInfo(const ports::ModelInfo& info);

private:
    QLabel* vertices_;
    QLabel* triangles_;
    QLabel* boundsSize_;
};

}  // namespace viewer::qt