#include "plotter_style_option.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>

PlotterStyleOption::PlotterStyleOption(QWidget *parent) : QWidget(parent)
    {
    this->setup();
    }

void
PlotterStyleOption::setup()
    {
    auto shapeLabel = new QLabel (tr("Shape"));

    qDebug() << "Allocate line, point";
    mLineCheckBox = new QCheckBox (tr("Line"));
    mLineCheckBox->setCheckState(Qt::Checked);

    mPointCheckBox = new QCheckBox (tr("Point"));
    mPointCheckBox->setCheckState(Qt::Checked);

    auto checkLayout = new QHBoxLayout;
    checkLayout->addWidget(mLineCheckBox);
    checkLayout->addWidget(mPointCheckBox);

    auto lwLabel = new QLabel (tr("Line width"));
    mLineWidth = new QSpinBox;
    mLineWidth->setValue(2);

    auto psLabel = new QLabel (tr("Point size"));
    mPointSize = new QSpinBox;
    mPointSize->setValue(8);

    qDebug() << "Build main layout";
    auto layout = new QVBoxLayout;
    //layout->addWidget(mLineCheckBox);
    //layout->addWidget(mPointCheckBox);
    layout->addWidget(shapeLabel);
    layout->addLayout(checkLayout);
    layout->addSpacing(5);
    layout->addWidget(lwLabel);
    layout->addWidget(mLineWidth);
    layout->addSpacing(5);
    layout->addWidget(psLabel);
    layout->addWidget(mPointSize);
    //layout->addSpacing(10);
    //layout->addLayout(themeLayout);
    layout->addStretch();

    qDebug() << "Set main layout";
    this->setLayout(layout);

    qDebug() << "Routine ends";
    }

bool
PlotterStyleOption::hasLine()
    {
    if (mLineCheckBox->checkState() == Qt::Checked)
        return true;
    else
        return false;
    }

bool
PlotterStyleOption::hasPoint()
    {
    if (mPointCheckBox->checkState() == Qt::Checked)
        return true;
    else
        return false;
    }


int
PlotterStyleOption::getLineWidth()
    {
    return mLineWidth->value();
    }

int
PlotterStyleOption::getPointSize()
    {
    return mPointSize->value();
    }
