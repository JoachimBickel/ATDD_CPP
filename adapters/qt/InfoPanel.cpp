#include "InfoPanel.h"

#include <QFormLayout>

namespace viewer::qt {

InfoPanel::InfoPanel(QWidget* parent)
    : QWidget(parent),
      vertices_(new QLabel(tr("–"), this)),
      triangles_(new QLabel(tr("–"), this)),
      boundsSize_(new QLabel(tr("–"), this))
{
    auto* layout = new QFormLayout(this);
    layout->addRow(tr("Vertices:"), vertices_);
    layout->addRow(tr("Triangles:"), triangles_);
    layout->addRow(tr("Size:"), boundsSize_);
}

void InfoPanel::showInfo(const ports::ModelInfo& info)
{
    vertices_->setText(QString::number(info.vertexCount));
    triangles_->setText(QString::number(info.triangleCount));

    const auto extent = info.bounds.max - info.bounds.min;
    boundsSize_->setText(QStringLiteral("%1 × %2 × %3")
                             .arg(extent.x, 0, 'g', 4)
                             .arg(extent.y, 0, 'g', 4)
                             .arg(extent.z, 0, 'g', 4));
}

}  // namespace viewer::qt
