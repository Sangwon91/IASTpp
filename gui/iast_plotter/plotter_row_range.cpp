#include "plotter_row_range.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <Qt>

PlotterRowRange::PlotterRowRange(int rowMax, QWidget *parent) : QWidget(parent),
    mRowMax {rowMax}
    {
    this->setup();
    }

void
PlotterRowRange::setup()
    {
    mAllCheckBox = new QCheckBox (tr("All"));
    connect(mAllCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(changeState()));

    mMinText = new QSpinBox;
    mMinText->setMaximum(mRowMax);
    auto minLabel = new QLabel (tr("Min index"));

    mMaxText = new QSpinBox;
    mMaxText->setMaximum(mRowMax);
    auto maxLabel = new QLabel (tr("Max index"));

    connect(mMinText, SIGNAL(valueChanged(int)),
            this, SLOT(updateMaxValue()));

    auto minMaxLayout = new QGridLayout;
    minMaxLayout->addWidget(minLabel, 0, 0);
    minMaxLayout->addWidget(mMinText, 0, 1);
    minMaxLayout->addWidget(maxLabel, 1, 0);
    minMaxLayout->addWidget(mMaxText, 1, 1);

    auto layout = new QVBoxLayout;

    layout->addWidget(mAllCheckBox);
    layout->addLayout(minMaxLayout);
    layout->addStretch();

    this->setLayout(layout);

    mAllCheckBox->setCheckState(Qt::Checked);
    }

void
PlotterRowRange::changeState()
    {
    if (mAllCheckBox->checkState() == Qt::Unchecked)
        {
        mMinText->setEnabled(true);
        mMaxText->setEnabled(true);
        }
    else if (mAllCheckBox->checkState() == Qt::Checked)
        {
        mMinText->setEnabled(false);
        mMaxText->setEnabled(false);
        }
    }

QCheckBox*
PlotterRowRange::getAllCheckBox()
    {
    return mAllCheckBox;
    }

QSpinBox*
PlotterRowRange::getMinEdit()
    {
    return mMinText;
    }

QSpinBox*
PlotterRowRange::getMaxEdit()
    {
    return mMaxText;
    }

void
PlotterRowRange::updateMaxValue()
    {
    mMaxText->setMinimum(mMinText->value());
    }
