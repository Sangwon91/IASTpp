#include "core_variable_chooser.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QButtonGroup>

CoreVariableChooser::CoreVariableChooser(QWidget *parent) : QWidget(parent)
    {
    this->setup();
    }

int
CoreVariableChooser::getExtensive()
    {
    if (mPressure->isChecked())
        return Extensive::Pressure;
    else
        return Extensive::Uptake;
    }

int
CoreVariableChooser::getIntensive()
    {
    if (mX->isChecked())
        return Intensive::X;
    else
        return Intensive::Y;
    }

void
CoreVariableChooser::setup()
    {
    mPressure = new QRadioButton (tr("Pressure"));
    mPressure->setChecked(true);
    connect(mPressure, SIGNAL(clicked(bool)), this, SIGNAL(changed()));

    mUptake = new QRadioButton (tr("Uptake"));
    connect(mUptake, SIGNAL(clicked(bool)), this, SIGNAL(changed()));

    auto extense = new QButtonGroup (this);
    extense->addButton(mPressure);
    extense->addButton(mUptake);

    auto extenseLayout = new QVBoxLayout;
    extenseLayout->addWidget(mPressure);
    extenseLayout->addWidget(mUptake);
    //extenseLayout->addStretch();

    mX = new QRadioButton (tr("x"));
    mY = new QRadioButton (tr("y"));
    mY->setChecked(true);
    connect(mX, SIGNAL(clicked(bool)), this, SIGNAL(changed()));
    connect(mY, SIGNAL(clicked(bool)), this, SIGNAL(changed()));

    auto intense = new QButtonGroup (this);
    intense->addButton(mX);
    intense->addButton(mY);

    auto intenseLayout = new QVBoxLayout;
    intenseLayout->addWidget(mX);
    intenseLayout->addWidget(mY);
    //intenseLayout->addStretch();

    //mMakeButton = new QPushButton (tr("Make Table"));
    //connect(mMakeButton, SIGNAL(clicked(bool)), this, SLOT(signalEmitter()));

    //auto makeLayout = new QVBoxLayout;

    //makeLayout->addWidget(mMakeButton);
    //makeLayout->addStretch();

    auto layout1 = new QHBoxLayout;

    layout1->addLayout(extenseLayout);
    layout1->addSpacing(10);
    layout1->addLayout(intenseLayout);
    //layout->addSpacing(10);

    auto layout = new QVBoxLayout;
    layout->addLayout(layout1);
    //layout->addSpacing(10);
    //layout->addStretch();
    //layout->addLayout(makeLayout);

    this->setLayout(layout);
    }
