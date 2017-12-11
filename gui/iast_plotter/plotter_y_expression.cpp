#include "plotter_y_expression.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QDebug>
#include <QMessageBox>

PlotterYExpression::PlotterYExpression(QWidget *parent) : QWidget(parent)
    {
    this->setup();
    }

void
PlotterYExpression::setup()
    {
    mTable = new QTableWidget;
    mTable->setColumnCount(2);
    mTable->setHorizontalHeaderItem(0, new QTableWidgetItem (tr("Expression")));
    mTable->setHorizontalHeaderItem(1, new QTableWidgetItem (tr("Legend")));

    mTable->setColumnWidth(0, 200);
    mTable->setRowHeight(0, 100);
    mTable->horizontalHeader()->setStretchLastSection(true);


    mAddButton = new QPushButton (tr("Add"));
    connect(mAddButton, SIGNAL(clicked(bool)),
            this, SLOT(addRow()));

    mDeleteButton = new QPushButton (tr("Delete"));
    connect(mDeleteButton, SIGNAL(clicked(bool)),
            this, SLOT(deleteRows()));

    auto helpButton = new QPushButton (tr ("Help"));
    connect(helpButton, SIGNAL(clicked(bool)),
            this, SLOT(showHelpMessage()));

    auto rightLayout = new QVBoxLayout;
    rightLayout->addWidget(mAddButton);
    rightLayout->addWidget(mDeleteButton);
    rightLayout->addStretch();
    rightLayout->addWidget(helpButton);

    auto layout = new QHBoxLayout;
    layout->addWidget(mTable);
    layout->addLayout(rightLayout);

    this->setLayout(layout);
    }

QTableWidget*
PlotterYExpression::getTable()
    {
    return mTable;
    }

void
PlotterYExpression::addRow()
    {
    int count = mTable->rowCount();
    qDebug() << __FILE__ << __LINE__ << "Count = " << count;

    mTable->insertRow(count);
    mTable->setItem(count, 0, new QTableWidgetItem);
    mTable->setItem(count, 1, new QTableWidgetItem);
    mTable->setCurrentCell(count, 0);
    }

void
PlotterYExpression::deleteRows()
    {
    if (mTable->rowCount() < 1)
        return;

    auto ranges = mTable->selectedRanges();

    if (ranges.size() == 0)
        return;

    auto range = ranges[0];

    int top = range.topRow();
    int bottom = range.bottomRow();

    qDebug() << __FILE__ << __LINE__ << "Top = " << top << "Bottom = " << bottom;

    for (int i = top; i <= bottom; ++i)
        mTable->removeRow(top);

    mTable->setCurrentCell(top - 1, 0);
    }

void
PlotterYExpression::showHelpMessage()
    {
    QString caption;

    caption = tr("<h3>How To Use Plotter</h3>");

    QString text;
    text = tr(
        "<p>I1 means first column of input table</p>"
        "<p>I2 means second column of input table... and so on</p>"
        "<p></p>"
        "<p>O1 means first column of output table</p>"
        "<p>O2 means second column of output table... and so on</p>"
        "You can also use mathematical expression like cos(O1 * I2) ^ 2 + 3.3");

    QMessageBox* msgBox = new QMessageBox (this);
    msgBox->setAttribute(Qt::WA_DeleteOnClose);
    msgBox->setWindowTitle(tr("About Plotter"));
    msgBox->setText(caption);
    msgBox->setInformativeText(text);

    msgBox->setStyleSheet("QLabel{min-width: 500px;}");

    msgBox->show();
    }
