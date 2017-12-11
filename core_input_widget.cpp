#include "core_input_widget.h"

#include <QDebug>
#include <QLabel>
#include <QString>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QApplication>
#include <QClipboard>
#include <QRegExp>
#include <QKeyEvent>
#include <QApplication>
#include <QDesktopWidget>

#include <sstream>
#include <vector>
#include <cmath>

#include "iast_gui_exception.h"
// For enum Extensive and Intensive
#include "core_variable_chooser.h"

#include "core_range_generator.h"

// Logger
#include "iast_gui_logger.h"

CoreInputWidget::CoreInputWidget(QWidget *parent) : QWidget(parent),
    mRangeGenerator {nullptr}
    {
    this->setup();
    }

QTableWidget*
CoreInputWidget::table()
    {
    return mTable;
    }

void
CoreInputWidget::getIsothermTable(QTableWidget* isothermTable)
    {
    mIsothermTable = isothermTable;
    }

void
CoreInputWidget::makeTable(int row, int extensive, int intensive)
    {
    if (extensive == CoreVariableChooser::Uptake &&
        intensive == CoreVariableChooser::Y)
        {
        int r = QMessageBox::critical(this, tr("Error"),
                tr("Unsupported mode"), QMessageBox::Close);

        if (r == QMessageBox::Close)
            return;
        }

    if (row < 2)
        return;

    mTable->clear();
    mTable->setRowCount(0);
    mTable->setColumnCount(row);

    if (extensive == CoreVariableChooser::Pressure)
        mTable->setHorizontalHeaderItem(0, new QTableWidgetItem (tr("Pressure")));
    else
        mTable->setHorizontalHeaderItem(0, new QTableWidgetItem (tr("Uptake")));

    QString g;

    if (intensive == CoreVariableChooser::X)
        g = "x";
    else
        g = "y";

    for (int i = 1; i < row; ++i)
        {
        //auto num = QString::number(i);
        //auto str = (g + num).toStdString();
        auto str =
            (g + '(' + mIsothermTable->item(i - 1, 1)->text() + ')')
            .toStdString();
        mTable->setHorizontalHeaderItem(
            i, new QTableWidgetItem (tr(str.c_str())));
        }

    emit tableMade();
    }

void
CoreInputWidget::addRow()
    {
    int count = mTable->rowCount();
    qDebug() << __FILE__ << __LINE__ << "Count = " << count;

    mTable->insertRow(count);

    for (int c = 0; c < mTable->columnCount(); ++c)
        mTable->setItem(count, c, new QTableWidgetItem);

    mTable->setCurrentCell(count, 0);

    emit tableChanged();
    }

void
CoreInputWidget::addRows(int num)
    {
    for (int i = 0; i < num; ++i)
        this->addRow();
    }

void
CoreInputWidget::addRowsWithSpinbox()
    {
    if (mTable->columnCount() < 2)
        {
        int r = QMessageBox::critical(this, tr("Error"),
                tr("Import isotherms"), QMessageBox::Close);

        if (r == QMessageBox::Close)
            return;
        }

    auto label = new QLabel (tr("Number of rows"));

    auto spin = new QSpinBox;
    spin->setMaximum(1e5);
    spin->setMinimum(1);
    spin->setMinimumHeight(30);
    //spin->setAttribute(Qt::WA_DeleteOnClose);

    auto top = new QHBoxLayout;
    top->addWidget(label);
    top->addWidget(spin);

    auto addbtn = new QPushButton(tr("Add"));
    auto bottom = new QHBoxLayout;
    bottom->addStretch();
    bottom->addWidget(addbtn);

    auto layout = new QVBoxLayout;
    layout->addLayout(top);
    layout->addLayout(bottom);

    auto widget = new QWidget;
    widget->setLayout(layout);
    widget->setAttribute(Qt::WA_DeleteOnClose);
    widget->setWindowTitle(tr("Add rows"));
    widget->move(QApplication::desktop()->screen()->rect().center() - widget->rect().center());
    widget->setWindowModality(Qt::ApplicationModal);
    widget->setMinimumWidth(300);

    connect(addbtn, &QPushButton::clicked,
            [this, spin]() { this->addRows(spin->value()); }
            );

    connect(addbtn, &QPushButton::clicked,
            [widget]() { widget->close(); }
            );

    widget->show();
    }

void
CoreInputWidget::deleteRows()
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

    emit tableChanged();
    }

void
CoreInputWidget::openGenerator()
    {
    if (mTable->columnCount() < 2)
        return;

    if (mRangeGenerator != nullptr)
        delete mRangeGenerator;

    mRangeGenerator = new CoreRangeGenerator (mTable);

    connect(mRangeGenerator, &CoreRangeGenerator::destroyed,
            [this]() {mRangeGenerator = nullptr;});

    connect(this, SIGNAL(tableMade()),
            mRangeGenerator, SLOT(close()));

    connect(mRangeGenerator, SIGNAL(generated()),
            this, SLOT(calculate()));

    connect(this, SIGNAL(destroyed(QObject*)),
            mRangeGenerator, SLOT(close()));
    }

void
CoreInputWidget::calculate()
    {
    emit calculateClicked();
    }

void
CoreInputWidget::keyPressEvent(QKeyEvent* event)
    {
    if (event->matches(QKeySequence::Copy))
        this->copy();
    else if (event->matches(QKeySequence::Paste))
        this->paste();
    else if (event->matches(QKeySequence::Delete))
        this->deleteRows();
    }

void
CoreInputWidget::copy()
    {
    auto ranges = mTable->selectedRanges();
    if (ranges.size() == 0)
        {
        qDebug() << __FILE__ << __LINE__ << "No Copy area";
        return;
        }

    QString str;

    auto range = ranges[0];


    for (int i = 0; i < range.rowCount(); ++i)
        {
        if (i > 0)
            str += "\n";

        for (int j = 0; j < range.columnCount(); ++j)
            {
            if (j > 0)
                str += "\t";

            auto item = mTable->item(i, j);

            if (item == nullptr)
                continue;

            str += item->text();
            }

        }

    QApplication::clipboard()->setText(str);
    }

void
CoreInputWidget::paste()
    {
    int nRows = mTable->rowCount();
    int nCols = mTable->columnCount();

    auto ranges = mTable->selectedRanges();
    if (ranges.size() == 0)
        {
        qDebug() << __FILE__ << __LINE__ << "No Selection";
        return;
        }

    auto str = QApplication::clipboard()->text();
    auto range = ranges[0];

    auto rows = str.split('\n');

    int r = range.topRow();
    for (auto& row : rows)
        {
        if (r >= nRows)
            break;

        int c = range.leftColumn();
        for (const auto& num : row.split(QRegExp("[, \\t\\n\\r]"), QString::SkipEmptyParts))
            {
            if (c >= nCols)
                break;

            mTable->setItem(r, c, new QTableWidgetItem (num));
            c++;
            }
        r++;
        }

    emit tableChanged();
    }

void
CoreInputWidget::setup()
    {
    mTable = new QTableWidget;

    mGeneratorButton = new QPushButton (tr("Open Generator"));
    mGeneratorButton->setStatusTip(tr ("Open input generator"));
    mGeneratorButton->setIcon(QIcon(tr(":/images/generator.png")));
    connect(mGeneratorButton, SIGNAL(clicked(bool)),
            this, SLOT(openGenerator()));

    mAddButton = new QPushButton (tr("Add Rows"));
    mAddButton->setStatusTip(tr ("Add empty rows"));
    connect(mAddButton, SIGNAL(clicked(bool)),
            this, SLOT(addRowsWithSpinbox()));

    mDeleteButton = new QPushButton (tr("Delete"));
    mDeleteButton->setStatusTip(tr ("Delete selected rows"));

    connect(mDeleteButton, SIGNAL(clicked(bool)),
            this, SLOT(deleteRows()));

    mCalculateButton = new QPushButton (tr("Calculate"));
    mCalculateButton->setStatusTip(tr ("Calculate mixture isotherm using IAST"));
    mCalculateButton->setIcon(QIcon(tr(":/images/calculator.png")));

    connect(mCalculateButton, SIGNAL(clicked(bool)),
            this, SLOT(calculate()));

    connect(this, SIGNAL(tableMade()),
            this, SLOT(openGenerator()));

    auto leftLayout = new QVBoxLayout;

    leftLayout->addWidget(mGeneratorButton);

    leftLayout->addWidget(mAddButton);
    leftLayout->addWidget(mDeleteButton);
    leftLayout->addStretch();
    leftLayout->addWidget(mCalculateButton);

    auto layout = new QHBoxLayout;
    layout->addWidget(mTable);
    layout->addLayout(leftLayout);

    this->setLayout(layout);
    }
