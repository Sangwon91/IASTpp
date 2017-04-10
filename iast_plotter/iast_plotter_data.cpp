#include "iast_plotter_data.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>

IastPlotterData::IastPlotterData(QWidget *parent) : QWidget(parent)
    {
    this->setup();
    }

void
IastPlotterData::setup()
    {
    mXExpression = new PlotterXExpression;
    auto xGroup = new QGroupBox (tr("X Value"));
    auto xLayout = new QHBoxLayout;
    xLayout->addWidget(mXExpression);
    xGroup->setLayout(xLayout);

    mYExpression = new PlotterYExpression;
    auto yGroup = new QGroupBox (tr("Y Value"));
    auto yLayout = new QHBoxLayout;
    yLayout->addWidget(mYExpression);
    yGroup->setLayout(yLayout);

    auto topLayout = new QHBoxLayout;
    topLayout->addWidget(xGroup);

    auto bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget(yGroup);

    auto layout = new QVBoxLayout;
    layout->addLayout(topLayout);
    layout->addLayout(bottomLayout);

    this->setLayout(layout);
    }

PlotterXExpression*
IastPlotterData::getXExpression()
    {
    return mXExpression;
    }

PlotterYExpression*
IastPlotterData::getYExpression()
    {
    return mYExpression;
    }
