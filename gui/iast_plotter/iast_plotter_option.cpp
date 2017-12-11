#include "iast_plotter_option.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>

IastPlotterOption::IastPlotterOption(int rowMax, QWidget *parent) : QWidget(parent),
    mRowMax {rowMax}
    {
    this->setup();
    }

void
IastPlotterOption::setup()
    {
    mXAxisOption = new PlotterAxisOption;
    auto xGroup = new QGroupBox (tr("X Axis"));
    auto xLayout = new QHBoxLayout;
    xLayout->addWidget(mXAxisOption);
    xGroup->setLayout(xLayout);

    mYAxisOption = new PlotterAxisOption;
    auto yGroup = new QGroupBox (tr("Y Axis"));
    auto yLayout = new QHBoxLayout;
    yLayout->addWidget(mYAxisOption);
    yGroup->setLayout(yLayout);

    mStyleOption = new PlotterStyleOption;
    auto styleGroup = new QGroupBox (tr("Style"));
    auto styleLayout = new QHBoxLayout;
    styleLayout->addWidget(mStyleOption);
    styleGroup->setLayout(styleLayout);

    mRowRange = new PlotterRowRange (mRowMax);
    auto rangeGroup = new QGroupBox (tr("Row Range"));
    auto rangeLayout = new QHBoxLayout;
    rangeLayout->addWidget(mRowRange);
    rangeGroup->setLayout(rangeLayout);

    auto layout = new QHBoxLayout;

    layout->addWidget(xGroup);
    layout->addWidget(yGroup);
    layout->addWidget(styleGroup);
    layout->addWidget(rangeGroup);

    this->setLayout(layout);
    }

bool
IastPlotterOption::isXLogScale()
    {
    return mXAxisOption->isLogScale();
    }

bool
IastPlotterOption::isYLogScale()
    {
    return mYAxisOption->isLogScale();
    }

QString
IastPlotterOption::getXAxisLabel()
    {
    return mXAxisOption->getLabel();
    }

QString
IastPlotterOption::getYAxisLabel()
    {
    return mYAxisOption->getLabel();
    }

bool
IastPlotterOption::hasLine()
    {
    return mStyleOption->hasLine();
    }

bool
IastPlotterOption::hasPoint()
    {
    return mStyleOption->hasPoint();
    }

int
IastPlotterOption::getLineWidth()
    {
    return mStyleOption->getLineWidth();
    }

int
IastPlotterOption::getPointSize()
    {
    return mStyleOption->getPointSize();
    }


PlotterAxisOption*
IastPlotterOption::getXAxisOption()
    {
    return mXAxisOption;
    }

PlotterAxisOption*
IastPlotterOption::getYAxisOption()
    {
    return mYAxisOption;
    }

PlotterRowRange*
IastPlotterOption::getRowRange()
    {
    return mRowRange;
    }
