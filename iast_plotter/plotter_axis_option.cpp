#include "plotter_axis_option.h"

#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <Qt>

PlotterAxisOption::PlotterAxisOption(QWidget *parent) : QWidget(parent)
    {
    this->setup();
    }

void
PlotterAxisOption::setup()
    {
    mAutoCheckBox = new QCheckBox (tr("Auto"));
    connect(mAutoCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(changeState()));

    mAutoCheckBox->setEnabled(false);

    //mMinText = new QLineEdit (tr(""));
    mMinSpin = new QDoubleSpinBox;
    mMinSpin->setMaximum(1e300);
    mMinSpin->setMinimum(-1e300);
    auto minLabel = new QLabel (tr("Min"));

    //mMaxText = new QLineEdit (tr(""));
    mMaxSpin = new QDoubleSpinBox;
    mMaxSpin->setMaximum(1e300);
    mMaxSpin->setMinimum(-1e300);
    auto maxLabel = new QLabel (tr("Max"));

    connect(mMinSpin, SIGNAL(valueChanged(double)),
            this, SLOT(updateMaxSpin()));

    mLabelText = new QLineEdit (tr(""));
    auto labelLabel = new QLabel (tr("Label"));

    auto gridLayout = new QGridLayout;
    gridLayout->addWidget(minLabel, 0, 0);
    gridLayout->addWidget(mMinSpin, 0, 1);
    gridLayout->addWidget(maxLabel, 1, 0);
    gridLayout->addWidget(mMaxSpin, 1, 1);
    gridLayout->addWidget(labelLabel, 3, 0);
    gridLayout->addWidget(mLabelText, 3, 1);

    mLogscaleCheckBox = new QCheckBox (tr("Logscale"));

    auto layout = new QVBoxLayout;
    layout->addWidget(mLogscaleCheckBox);
    layout->addSpacing(5);
    layout->addWidget(mAutoCheckBox);
    layout->addLayout(gridLayout);


    this->setLayout(layout);

    mAutoCheckBox->setCheckState(Qt::Checked);
    }

void
PlotterAxisOption::changeState()
    {
    if (mAutoCheckBox->checkState() == Qt::Unchecked)
        {
        mMinSpin->setEnabled(true);
        mMaxSpin->setEnabled(true);
        }
    else if (mAutoCheckBox->checkState() == Qt::Checked)
        {
        mMinSpin->setEnabled(false);
        mMaxSpin->setEnabled(false);
        }
    }

void
PlotterAxisOption::updateMaxSpin()
    {
    mMaxSpin->setMinimum(mMinSpin->value());
    }

bool
PlotterAxisOption::isLogScale()
    {
    if (mLogscaleCheckBox->checkState() == Qt::Checked)
        return true;
    else
        return false;
    }

QString
PlotterAxisOption::getLabel()
    {
    return mLabelText->text();
    }

void
PlotterAxisOption::setLabel(const QString& label)
    {
    mLabelText->setText(label);
    }

QCheckBox*
PlotterAxisOption::getLogScaleCheckBox()
    {
    return mLogscaleCheckBox;
    }
