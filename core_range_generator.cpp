#include "core_range_generator.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QCheckBox>
#include <QMessageBox>
#include <QDebug>
#include <QSpinBox>
#include <QApplication>
#include <QDesktopWidget>

#include <sstream>
#include <vector>
#include <functional>

#include <cmath>

#include "iast_gui_exception.h"

// Simple Helper class
DoubleSpinBox::DoubleSpinBox(QWidget* parent) : QDoubleSpinBox {parent} {}

QString
DoubleSpinBox::textFromValue(double value) const
    {
    std::stringstream ss;
    ss.precision(15);
    ss << value;

    return QString {ss.str().c_str()};
    }

void
DoubleSpinBox::setState(int v)
    {
    if (v == 1)
        this->setEnabled(false);
    else
        this->setEnabled(true);
    }

namespace // Helper functions
{
std::vector<double>
linspace(double x0, double x1, int n)
    {
    if (n < 2)
        return std::vector<double> {x0};
        //throw IastGuiException {__FILE__, __LINE__, "n < 2"};

    if (x1 < x0)
        throw IastGuiException {__FILE__, __LINE__, "x1 < x0"};

    constexpr double tiny = 1.0e-10;
    double dx = (x1 - x0) / static_cast<double>(n - 1);

    if (dx < tiny)
        return std::vector<double> {x0};

    std::vector<double> space;
    double x = x0;
    for (int i = 0; i < n; ++i)
        {
        space.push_back(x);
        x += dx;
        }
    //for (double x = x0; x <= x1 + tiny; x += dx)
    //    {
    //    space.push_back(x);
    //   }

    return space;
    }

std::vector<double>
logspace(double x0, double x1, int n)
    {
    if (n < 2)
        return std::vector<double> {x0};
        //throw IastGuiException {__FILE__, __LINE__, "n < 2"};

    if (x1 < x0)
        throw IastGuiException {__FILE__, __LINE__, "x1 < x0"};

    if (x0 < 0.0 || x1 < 0.0)
        throw IastGuiException {__FILE__, __LINE__, "x1 < 0 or x0 < 0"};

    //constexpr double tiny = 1.0e-10;
    double dx = std::pow(x1 / x0, 1.0 / static_cast<double>(n - 1));

    //if (abs(dx - 1.0) < tiny)
    //    return std::vector<double> {x0};

    std::vector<double> space;
    double x = x0;
    for (int i = 0; i < n; ++i)
        {
        space.push_back(x);
        x *= dx;
        }
    //for (double x = x0; x <= x1 + tiny; x *= dx)
    //    {
    //    space.push_back(x);
    //    }

    return space;
    }
}

CoreRangeGenerator::CoreRangeGenerator(QTableWidget* data, QWidget* parent) :
    QWidget (parent), mData (data)
    {
    this->setup();
    }

void
CoreRangeGenerator::generate()
    {
    std::function< std::vector<double>(double, double, int) > space;
    std::vector< std::vector<double> > ranges;

    int row = mTable->rowCount();

    for (int r = 0; r < row; ++r)
        {
        //Qt::CheckState state = static_cast<QCheckBox*>(mTable->cellWidget(r, 3))->checkState();

        Qt::CheckState state = mCheckBoxList[r]->checkState();

        qDebug() << "State:" << state;

        if (state == Qt::Unchecked)
            space = linspace;
        else if (state == Qt::Checked)
            space = logspace;

        double start = static_cast<QDoubleSpinBox*>(mTable->cellWidget(r, 0))->value();
        double end   = static_cast<QDoubleSpinBox*>(mTable->cellWidget(r, 1))->value();
        double steps = static_cast<QSpinBox*>      (mTable->cellWidget(r, 2))->value();

        qDebug() << __FILE__ << __LINE__ << "Start" << start;
        qDebug() << __FILE__ << __LINE__ << "End"   << end;
        qDebug() << __FILE__ << __LINE__ << "Steps" << steps;

        std::vector<double> range;

        try {
            range = space(start, end, steps);
            }
        catch (IastGuiException& e)
            {
            int r = QMessageBox::critical(this, tr("Error"),
                tr(e.what()), QMessageBox::Close);

            if (r == QMessageBox::Close)
                return;
            }

        ranges.push_back(range);
        }

    int size = ranges.size();

    std::vector<int> idxs;
    std::vector<int> ends;

    for (auto& range : ranges)
        {
        idxs.push_back(0);
        ends.push_back(range.size());
        qDebug() << __FILE__ << __LINE__ << "(Start, End) = " << idxs.back() << ends.back();
        }

    // Expand data table and put variables
    // So generate input variables

    // http://stackoverflow.com/questions/9555864/variable-nested-for-loops
    idxs.push_back(0);
    ends.push_back(0);
    int p = 0;
    while (idxs[size] == 0)
        {
        // Stuff here
        //std::stringstream ss;
        //for (auto& i : idxs)
        //    ss << i << "    ";

        //qDebug() << "Nested Loop Test = " << ss.str().c_str();

        bool valid = true;
        double tiny = 1.e-10;

        double ysum = 0.0;
        for (int i = 1; i < size; ++i)
            {
            int index = idxs[i];
            auto& range = ranges[i];

            if (range[index] < tiny)
                {
                valid = false;
                break;
                }

            ysum += range[index];
            }

        if (ysum >= (1.0 - tiny))
            valid = false;

        // Generate only if valid composition
        if (valid)
            {
            int row = mData->rowCount();
            mData->insertRow(row);

            for (int i = 0; i < size; ++i)
                {
                int index = idxs[i];
                auto& range = ranges[i];
                auto num = QString::number(range[index]).toStdString();
                mData->setItem(row, i, new QTableWidgetItem (tr(num.c_str())));
                }
            }
        // Stuff ends
        idxs[0]++;

        while (idxs[p] == ends[p])
            {
            idxs[p] = 0;
            idxs[++p]++;
            if (idxs[p] != ends[p])
                p = 0;
            }
        }

    emit generated();
    }

void
CoreRangeGenerator::setup()
    {
    this->setWindowTitle(tr("Range Generator"));
    this->setWindowIcon(QIcon(tr(":/images/generator.png")));

    mTable = new QTableWidget;
    mTable->setColumnCount(4);
    mTable->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Start")));
    mTable->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("End")));
    mTable->setHorizontalHeaderItem(2, new QTableWidgetItem(tr("# Points")));
    mTable->setHorizontalHeaderItem(3, new QTableWidgetItem(tr("Logscale")));

    mTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    mTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    int n = mData->columnCount();

    mTable->setRowCount(n);

    auto ext = mData->horizontalHeaderItem(0)->text().toStdString();
    mTable->setVerticalHeaderItem(0, new QTableWidgetItem (tr(ext.c_str())));
        {
        /*
        // For centering...
        auto wid = new QWidget;
        auto chk = new QCheckBox;
        auto lay = new QHBoxLayout;

        //chk->setCheckState(Qt::Unchecked);

        lay->addWidget(chk);
        lay->setAlignment(Qt::AlignCenter);
        lay->setContentsMargins(0, 0, 0, 0);

        wid->setLayout(lay);

        mTable->setCellWidget(0, 3, wid);
        */
        }

    for (int i = 1; i < n; ++i)
        {
        auto intensive = mData->horizontalHeaderItem(i)->text().toStdString();
        mTable->setVerticalHeaderItem(i, new QTableWidgetItem (tr(intensive.c_str())));
        }

    for (int r = 0; r < n; ++r)
        {
        // Add double spin box for real values...
        for (int c : {0, 1})
            {
            auto dspin = new DoubleSpinBox;
            dspin->setDecimals(10);
            dspin->setMinimum(0);
            dspin->setMaximum(1);

            if (r == 0)
                {
                dspin->setMaximum(1e30);
                dspin->setValue(std::pow(10, c));
                }
            else
                {
                dspin->setValue(1.0 / static_cast<double>(n));
                dspin->setSingleStep(0.01);
                }

            mTable->setCellWidget(r, c, dspin);
            }

        // Add spin box to # points area
        auto spin = new QSpinBox;
        spin->setMinimum(1);
        spin->setMaximum(10000000);

        mTable->setCellWidget(r, 2, spin);

        /*
        // Add logscale check box...
        // Somewhat verbose due to centering...
        auto chk = new QCheckBox;
        auto lay = new QHBoxLayout;

        lay->addWidget(chk);
        lay->setAlignment(Qt::AlignCenter);
        lay->setContentsMargins(0, 0, 0, 0);

        auto wid = new QWidget;
        wid->setLayout(lay);

        mTable->setCellWidget(r, 3, wid);
        */
        }

    // Add checkbox list
    for (int r = 0; r < n; ++r)
        {
        auto chk = new QCheckBox;
        auto lay = new QHBoxLayout;

        lay->addWidget(chk);
        lay->setAlignment(Qt::AlignCenter);
        lay->setContentsMargins(0, 0, 0, 0);

        auto wid = new QWidget;
        wid->setLayout(lay);

        mTable->setCellWidget(r, 3, wid);

        mCheckBoxList << chk;
        }

    // connect signal and initialize
    for (int r = 0; r < n; ++r)
        {
        auto end = static_cast<DoubleSpinBox*>(mTable->cellWidget(r, 1));
        auto point = static_cast<QSpinBox*>(mTable->cellWidget(r, 2));
        connect(point, SIGNAL(valueChanged(int)),
                end, SLOT(setState(int)));

        if (r != 0)
            end->setEnabled(false);
        else
            point->setValue(19);
        }

    mGenerateButton = new QPushButton (tr("Append"));
    connect(mGenerateButton, SIGNAL(clicked(bool)), this, SLOT(generate()));

    QFont font = qApp->font();
    font.setBold(true);
    mCleanGenerateButton = new QPushButton(tr("Generate"));
    mCleanGenerateButton->setIcon(QIcon(tr(":/images/reload.png")));
    mCleanGenerateButton->setStatusTip(tr("Clean up input table and generate range"));
    mCleanGenerateButton->setFont(font);
    connect(mCleanGenerateButton, &QPushButton::clicked,
            [this]() {mData->setRowCount(0);});
    connect(mCleanGenerateButton, SIGNAL(clicked(bool)),
            this, SLOT(generate()));

    mCloseButton = new QPushButton (tr("Close"));
    connect(mCloseButton, SIGNAL(clicked(bool)), this, SLOT(close()));

    auto bottomLayout = new QHBoxLayout;
    bottomLayout->addStretch();
    bottomLayout->addWidget(mGenerateButton);
    bottomLayout->addWidget(mCleanGenerateButton);
    bottomLayout->addWidget(mCloseButton);

    auto layout = new QVBoxLayout;
    layout->addWidget(mTable);
    layout->addLayout(bottomLayout);

    this->setLayout(layout);

    this->setMinimumSize(mTable->sizeHint().width() * 2.0, n * 35 + 70);
    this->resize(mTable->sizeHint().width() * 2.0, n * 35 + 70);
    this->move(QApplication::desktop()->screen()->rect().center() - this->rect().center());
    //this->resize(mTable->sizeHint().width() * 2.0, mTable->sizeHint().height() * 1.2);

    // Memory leak?
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->show();
    }
