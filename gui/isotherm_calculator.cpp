#include "isotherm_calculator.h"

// IAST GUI
#include "iast_gui_exception.h"

// IAST core
#include "../iast/isotherm_factory.hpp"
#include "../iast/isotherm_exception.hpp"

// Qt
#include <QFileDialog>
#include <QFileInfo>
#include <QStringList>
#include <QSharedPointer>
#include <QVector>
#include <QColor>
#include <QBrush>
#include <QFont>
#include <QPen>

// C++
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <functional>
#include <ciso646>

// QCustomPlot
#include "qcustomplot/qcustomplot.h"

// Logger
#include "iast_gui_logger.h"

namespace // Space span helpers
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

    return space;
    }
}

IsothermCalculator::IsothermCalculator(QWidget *parent) : QWidget(parent)
    {
    this->setup();
    }

void
IsothermCalculator::importIsotherm()
    {
    auto filePath = QFileDialog::getOpenFileName(nullptr, tr("Open File"),
                    QString (), "Isotherm (*.isotherm)");

    mPathLine->setText(filePath);

    IsothermFactory factory;

    try {
        auto iso = factory.create(filePath.toStdString());
        mIsothermInfoText->setPlainText(iso->getInfoString().c_str());
        }
    catch (IsothermException& e)
        {
        mIsothermInfoText->setPlainText("Invalid isotherm file");
        }

    mOutputText->setPlainText("");
    }

void
IsothermCalculator::calculate()
    {
    bool ok = false;

    double p0;
    double p1;
    double num;

    p0 = mPMinLine->text().toDouble(&ok);
    if (ok)
        {
        p1 = mPMaxLine->text().toDouble(&ok);

        if (ok)
            num = mNumPointsLine->text().toInt(&ok);
        }

    if (not ok)
        {
        int r = QMessageBox::critical(this, tr("Error"),
                tr("Invalid text in pressure range"), QMessageBox::Close);

        mOutputText->clear();

        if (r == QMessageBox::Close)
            return;
        }

    if (ok)
        {
        std::function< std::vector<double>(double, double, int) > space;

        if (mLogscaleCheck->isChecked())
            space = logspace;
        else
            space = linspace;

        std::vector<double> range;

        try {
            range = space(p0, p1, num);
            }
        catch (IastGuiException& e)
            {
            int r = QMessageBox::critical(this, tr("Error"),
                    tr("Invalid pressure range"), QMessageBox::Close);

            mOutputText->clear();

            if (r == QMessageBox::Close)
                return;
            }

        IsothermFactory factory;
        std::shared_ptr<Isotherm> iso;

        try {
            iso = factory.create(mPathLine->text().toStdString());
            }
        catch (IsothermException& e)
            {
            int r = QMessageBox::critical(this, tr("Error"),
                    tr("Invalid isotherm file"), QMessageBox::Close);

            mOutputText->clear();

            if (r == QMessageBox::Close)
                return;
            }

        std::stringstream ss;
        ss.precision(10);
        int width = 22;

        ss << std::setw(3 * width) << std::setfill('=') << "";
        ss << std::setfill(' ') << '\n';

        ss << std::setw(width) << "Pressure" <<
              std::setw(width) << "Uptake" <<
              std::setw(width) << "Spreading Pressure" <<
              '\n';

        ss << std::setw(3 * width) << std::setfill('=') << "";
        ss << std::setfill(' ') << '\n';

        for (const auto& p : range)
            ss << std::setw(width) << p <<
                  std::setw(width) << iso->loading(p) <<
                  std::setw(width) << iso->spressure(p) <<
                  '\n';

        std::string str = ss.str();
        str.resize(str.size() - 1);

        mOutputText->setPlainText(str.c_str());
        }

    }

void
IsothermCalculator::exportOutput()
    {
    auto lines = mOutputText->toPlainText().split('\n', QString::SkipEmptyParts);

    // No data
    if (lines.size() < 4)
        {
        int r = QMessageBox::critical(this, tr("Error"),
                tr("No output"), QMessageBox::Close);

        if (r == QMessageBox::Close)
            return;
        }

    // get file path
    auto fileName = QFileDialog::getSaveFileName(nullptr, tr ("Save output"));

    std::ofstream ofs (fileName.toStdString());
    char tab = '\t';

    ofs << "Pressure" << tab <<
           "Uptake" << tab <<
           "SpreadingPressure" << tab <<
           std::endl;

    int size = lines.size();
    for (int i = 3; i < size - 1; ++i)
        {
        auto values = lines[i].split(' ', QString::SkipEmptyParts);
        ofs << values[0].toStdString() << tab <<
               values[1].toStdString() << tab <<
               values[2].toStdString() << tab <<
               std::endl;
        }

        {
        auto values = lines[size - 1].split(' ', QString::SkipEmptyParts);
        ofs << values[0].toStdString() << tab <<
               values[1].toStdString() << tab <<
               values[2].toStdString() << tab;
        }

    }

void
IsothermCalculator::plotOutput()
    {
    auto lines = mOutputText->toPlainText().split('\n', QString::SkipEmptyParts);

    // No data
    if (lines.size() < 4)
        {
        int r = QMessageBox::critical(this, tr("Error"),
                tr("No output to plot"), QMessageBox::Close);

        if (r == QMessageBox::Close)
            return;
        }

    auto plot = new QCustomPlot;
    plot->setAttribute(Qt::WA_DeleteOnClose);
    plot->setWindowTitle(tr("Isotherm Plot"));
    connect(this, SIGNAL(destroyed(QObject*)),
            plot, SLOT(close()));

    plot->legend->setVisible(true);
    plot->legend->setFont(QFont {"Arial", 12});

    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    plot->addGraph();

    QFileInfo info {mPathLine->text()};
    plot->graph()->setName(info.baseName());

    QPen pen;
    pen.setWidth(2);
    pen.setColor(QColor {"blue"});

    plot->graph()->setPen(pen);
    plot->graph()->setLineStyle(QCPGraph::LineStyle::lsLine);
    plot->graph()->setScatterStyle(QCPScatterStyle {QCPScatterStyle::ssDisc, pen, QBrush {}, 8});

    QVector<double> xValues;
    QVector<double> yValues;

    int size = lines.size();
    for (int i = 3; i < size; ++i)
        {
        auto values = lines[i].split(' ', QString::SkipEmptyParts);

        xValues << values[0].toDouble();
        yValues << values[1].toDouble();
        }

    plot->graph()->setData(xValues, yValues);

    plot->xAxis2->setVisible(false);
    plot->xAxis2->setTickLabels(false);

    plot->yAxis2->setVisible(false);
    plot->yAxis2->setTickLabels(false);

    if (mLogscaleCheck->isChecked())
        {
        plot->xAxis->setScaleType(QCPAxis::stLogarithmic);
        plot->xAxis2->setScaleType(QCPAxis::stLogarithmic);

        QSharedPointer<QCPAxisTickerLog> logTicker(new QCPAxisTickerLog);
        plot->xAxis->setTicker(logTicker);
        plot->xAxis2->setTicker(logTicker);
        plot->xAxis->setNumberFormat("eb"); // e = exponential, b = beautiful decimal powers
        plot->xAxis->setNumberPrecision(0); // makes sure "1*10^4" is displayed only as "10^4"
        }

    plot->xAxis->setLabel("Pressure");
    plot->xAxis->setLabelFont(QFont {"Arial", 12});

    plot->yAxis->setLabel("Uptake");
    plot->yAxis->setLabelFont(QFont {"Arial", 12});

    plot->rescaleAxes();
    plot->axisRect()->setupFullAxesBox();

    plot->setMinimumHeight(500);
    plot->setMinimumWidth(600);

    plot->resize(plot->width(), this->height());
    plot->move(this->pos().rx() + this->width(), this->pos().ry());

    plot->show();
    }

void
IsothermCalculator::setup()
    {
    this->setWindowTitle(tr ("Isotherm Calculator"));
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setMinimumHeight(500);

    mImportButton = new QPushButton {tr ("Import *.isotherm")};
    connect(mImportButton, SIGNAL(clicked(bool)),
            this, SLOT(importIsotherm()));

    mExportButton = new QPushButton {tr ("Export")};
    connect(mExportButton, SIGNAL(clicked(bool)),
            this, SLOT(exportOutput()));

    mCalculateButton = new QPushButton {tr ("Calculate")};
    connect(mCalculateButton, SIGNAL(clicked(bool)),
            this, SLOT(calculate()));

    mPlotButton = new QPushButton {tr("Plot")};
    // Automatically recalculate
    connect(mPlotButton, SIGNAL(clicked(bool)),
            this, SLOT(calculate()));
    connect(mPlotButton, SIGNAL(clicked(bool)),
            this, SLOT(plotOutput()));

    mPathLine = new QLineEdit;
    mPathLine->setReadOnly(true);

    mPMinLine = new QLineEdit;
    mPMinLine->setText("1");
    mPMaxLine = new QLineEdit;
    mPMaxLine->setText("10");
    mNumPointsLine = new QLineEdit;
    mNumPointsLine->setText("10");

    // TextEdits
    mIsothermInfoText = new QTextEdit;
    mIsothermInfoText->setFont(QFont("consolas", 10));
    mIsothermInfoText->setMinimumWidth(500);
    mIsothermInfoText->setReadOnly(true);

    mOutputText = new QTextEdit;
    mOutputText->setFont(QFont("consolas", 10));
    mOutputText->setReadOnly(true);

    mLogscaleCheck = new QCheckBox {tr ("")};

    auto lay0 = new QHBoxLayout;
    lay0->addWidget(mPathLine);
    lay0->addWidget(mImportButton);

    auto lay1 = new QHBoxLayout;
    lay1->addWidget(mIsothermInfoText);

    auto lay2 = new QGridLayout;

    //auto frame = new QFrame;
    //frame->

    lay2->addWidget(new QLabel {tr("P Min")}, 0, 0);
    lay2->addWidget(new QLabel {tr("P Max")}, 0, 1);
    lay2->addWidget(new QLabel {tr("# of Points")}, 0, 2);
    lay2->addWidget(new QLabel {tr("Logscale")}, 0, 3);
    lay2->addWidget(mPMinLine, 1, 0);
    lay2->addWidget(mPMaxLine, 1, 1);
    lay2->addWidget(mNumPointsLine, 1, 2);
    lay2->addWidget(mLogscaleCheck, 1, 3, Qt::AlignCenter);

    auto lay3 = new QHBoxLayout;
    lay3->addWidget(mOutputText);

    auto lay4 = new QHBoxLayout;
    lay4->addStretch();
    lay4->addWidget(mCalculateButton);
    lay4->addWidget(mPlotButton);
    lay4->addWidget(mExportButton);

    auto layout = new QVBoxLayout;
    layout->addLayout(lay0);
    layout->addLayout(lay1);
    layout->addLayout(lay2);
    layout->addLayout(lay3);
    layout->addLayout(lay4);

    this->setLayout(layout);
    }
