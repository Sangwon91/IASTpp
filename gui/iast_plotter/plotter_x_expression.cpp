#include "plotter_x_expression.h"

#include <QLabel>
#include <QVBoxLayout>

PlotterXExpression::PlotterXExpression(QWidget *parent) : QWidget(parent)
    {
    this->setup();
    }

void
PlotterXExpression::setup()
    {
    auto label = new QLabel (tr("Expression"));
    mExpression = new QLineEdit;

    auto layout = new QVBoxLayout;
    layout->addWidget(label);
    layout->addWidget(mExpression);
    layout->addStretch();

    this->setLayout(layout);
    }

QString
PlotterXExpression::getExpression()
    {
    return mExpression->text();
    }

void
PlotterXExpression::setExpression(QString exp)
    {
    mExpression->setText(exp);
    }
