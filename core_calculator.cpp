#include "core_calculator.h"

// Qt headers
#include <QDebug>
#include <QMessageBox>
#include <QFileInfo>
#include <QProgressBar>

// C++ headers
#include <thread>
#include <ciso646>

// IAST cores
#include "iast/iast_core.hpp"
#include "iast/iast_exception.hpp"
#include "iast/isotherm_factory.hpp"

// Logger
#include "iast_gui_logger.h"

CoreCalculator::CoreCalculator(CoreIsothermTable* isoWidget,
                               CoreInputWidget* inputWidget,
                               CoreOutputWidget* outputWidget,
                               QObject *parent) :
    QObject (parent),
    mIsoWidget (isoWidget),
    mInputWidget (inputWidget),
    mOutputWidget (outputWidget)
    {
    this->setup();
    }

void
CoreCalculator::calculate()
try {
    IsothermFactory factory;

    Iast::IsothermVector isotherms;

    // Import Isotherms
    auto isoTable = mIsoWidget->table();

    int size = isoTable->rowCount();

    if (size < 2)
        {
        int r = QMessageBox::critical(nullptr, tr("Error"),
            tr("# of species < 2"), QMessageBox::Close);

        if (r == QMessageBox::Close)
            return;
        }

    for (int i = 0; i < size; ++i)
        {
        auto path = isoTable->item(i, 0)->text();

        if (not QFileInfo (path).exists())
            {
            QString msg {"Isotherm file does not exist:\n" + path};

            int r = QMessageBox::critical(nullptr, tr("Error"),
                tr(msg.toStdString().c_str()), QMessageBox::Close);

            if (r == QMessageBox::Close)
                return;
            }

        // Invalid Isofile Exception is needed.
        isotherms.push_back(factory.create(path.toStdString()));
        IAST_GUI_LOG(isotherms.back()->getInfoString());
        }

    Iast iast;

    iast.setIsotherms(isotherms);

    auto inputTable = mInputWidget->table();
    size = inputTable->rowCount();

    Iast::Mode mode;

    auto extensive = inputTable->horizontalHeaderItem(0)->text().toStdString();
    auto intensive = inputTable->horizontalHeaderItem(1)->text().toStdString();

    if (extensive == "Pressure" and intensive[0] == 'y')
        mode = Iast::FIX_PY;
    else if (extensive == "Pressure" and intensive[0] == 'x')
        mode = Iast::FIX_PX;
    else if (extensive == "Uptake" and intensive[0] == 'x')
        mode = Iast::FIX_NX;
    else
        ; // What?

    IAST_GUI_LOG("IAST MODE:", mode);

    auto outputTable = mOutputWidget->table();
    outputTable->clear();
    outputTable->setRowCount(0);

    int cols = inputTable->columnCount();
    outputTable->setColumnCount(cols + 1);

    if (mode == Iast::FIX_PY)
        {
        outputTable->setHorizontalHeaderItem(0,
            new QTableWidgetItem ("Uptake"));

        for (int c = 1; c < cols + 1; ++c)
            {
            auto str = ("x(" + isoTable->item(c - 1, 1)->text() + ')').toStdString();
            outputTable->setHorizontalHeaderItem(c,
                new QTableWidgetItem(str.c_str()));
            }
        }
    else if (mode == Iast::FIX_PX)
        {
        outputTable->setHorizontalHeaderItem(0,
            new QTableWidgetItem ("Uptake"));

        for (int c = 1; c < cols + 1; ++c)
            {
            auto str = ("y(" + isoTable->item(c - 1, 1)->text() + ')').toStdString();
            outputTable->setHorizontalHeaderItem(c,
                new QTableWidgetItem(str.c_str()));
            }
        }
    else if (mode == Iast::FIX_NX)
        {
        outputTable->setHorizontalHeaderItem(0,
            new QTableWidgetItem ("Pressure"));

        for (int c = 1; c < cols + 1; ++c)
            {
            auto str = ("y(" + isoTable->item(c - 1, 1)->text() + ')').toStdString();
            outputTable->setHorizontalHeaderItem(c,
                new QTableWidgetItem(str.c_str()));
            }
        }

    /*
    auto progress = new QProgressBar;
    progress->setAttribute(Qt::WA_DeleteOnClose);
    progress->setMaximum(size - 1);
    progress->setMinimum(0);
    progress->show();
    */
    for (int i = 0; i < size; ++i)
        {
        /*
        progress->setValue(i);

        if (progress->value() == progress->maximum())
            progress->close();
        */

        // Add new line for output table
        int n = outputTable->rowCount();
        outputTable->insertRow(n);

        // Neglecct exception for now
        // Checker added 2017-01-03
        bool okFinal = true;
        bool ok;

        //qDebug() << __FILE__ << __LINE__ << "Empty cells test starts";
        IAST_GUI_LOG("Check Empty Cells");
        if (inputTable->item(i, 0) == nullptr)
            {
            int r = QMessageBox::critical(nullptr, tr("Error"),
                tr("Empty cells exist in input"), QMessageBox::Close);

            if (r == QMessageBox::Close)
                return;
            }

        if (inputTable->item(i, 0)->text().isEmpty())
            {
            int r = QMessageBox::critical(nullptr, tr("Error"),
                tr("Empty cells exist in input"), QMessageBox::Close);

            if (r == QMessageBox::Close)
                return;
            }
        qDebug() << __FILE__ << __LINE__ << "Empty cells test ends";

        double extense = inputTable->item(i, 0)->text().toDouble(&ok);
        if (not ok)
            okFinal = false;

        double ysum = 0.0;
        std::vector<double> intenses;
        for (int c = 1; c < cols; ++c)
            {
            if (inputTable->item(i, c) == nullptr)
                {
                int r = QMessageBox::critical(nullptr, tr("Error"),
                    tr("Empty cells exist in input"), QMessageBox::Close);

                if (r == QMessageBox::Close)
                    return;
                }

            if (inputTable->item(i, c)->text().isEmpty())
                {
                int r = QMessageBox::critical(nullptr, tr("Error"),
                    tr("Empty cells exist in input"), QMessageBox::Close);

                if (r == QMessageBox::Close)
                    return;
                }

            double val = inputTable->item(i, c)->text().toDouble(&ok);

            if (not ok)
                okFinal = false;

            ysum += val;
            intenses.push_back(val);
            }

        intenses.push_back(1.0 - ysum);

        double extenseResult;
        std::vector<double> intenseResult;

        IAST_GUI_LOG("Start IAST Calculation", "Row:", i);

        try {
            if (okFinal)
                std::tie(extenseResult, intenseResult) =
                    iast.calculate(mode, extense, intenses).getResult();
            }
        catch (IastException& e)
            {
            okFinal = false;
            }

        if (not okFinal)
            {
            for (int c = 0; c < cols + 1; ++c)
                outputTable->setItem(n, c, new QTableWidgetItem ("0"));

            continue;
            }

        auto num = QString::number(extenseResult);
        outputTable->setItem(n, 0, new QTableWidgetItem (num));

        for (int c = 1; c < cols + 1; ++c)
            {
            auto num = QString::number(intenseResult[c - 1]);
            outputTable->setItem(n, c, new QTableWidgetItem (num));
            }
        }
    }
catch (std::exception& e)
    {
    qDebug() << e.what();
    }

void
CoreCalculator::setup()
    {

    }
