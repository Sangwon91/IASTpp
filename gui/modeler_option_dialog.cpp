#include "modeler_option_dialog.h"

#include <QLabel>
#include <QDebug>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QTableWidget>
#include <QStackedLayout>
#include <QPushButton>
#include <QApplication>
#include <QMessageBox>

#include <stdexcept>

#include "iast_gui_exception.h"

// Logger
#include "iast_gui_logger.h"

ModelerOptionDialog::ModelerOptionDialog(const InfosType* info, QWidget *parent) : QDialog (parent)
    {
    mIsoInfo = info;

    this->setWindowTitle("Advanced Options");
    this->resize(680, 300);

    mListWidget = new QListWidget;

    for (const auto& info : *mIsoInfo)
        if (info.isModel())
            mListWidget->addItem(tr(info.fullName().c_str()));

    mStackedLayout = new QStackedLayout;

    for (const auto& info : *mIsoInfo)
        if (info.isModel())
            {
            auto widget = new QTableWidget;
            widget->setColumnCount(3);
            widget->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Initial Guess")));
            widget->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Min")));
            widget->setHorizontalHeaderItem(2, new QTableWidgetItem(tr("Max")));
            widget->horizontalHeader()->setStretchLastSection(true);

            widget->setRowCount(info.numParams());

            int count = 0;
            for (auto& param : info.paramNames())
                {
                // General initial guess
                widget->setVerticalHeaderItem(count, new QTableWidgetItem(tr(param.c_str())));
                widget->setItem(count, 0, new QTableWidgetItem("1.0"));
                widget->setItem(count, 1, new QTableWidgetItem("0.0"));
                widget->setItem(count, 2, new QTableWidgetItem("10000.0"));

                // Special initial guess for some parameters.
                // 1. BET
                if (info.fullName() == "BET" && param == "k2")
                    widget->item(count, 0)->setText("1e-5");

                // Increase count...
                count++;
                }

            mStackedLayout->addWidget(widget);
            }

    connect(mListWidget, SIGNAL(currentRowChanged(int)),
            mStackedLayout, SLOT(setCurrentIndex(int)));

    mListWidget->setCurrentRow(0);

    auto topLayout = new QHBoxLayout;
    topLayout->addWidget(mListWidget);
    topLayout->addLayout(mStackedLayout);

    auto applyButton = new QPushButton (tr("&Apply"));
    connect(applyButton, &QPushButton::clicked,
            [=]() {emit optionChanged();});

    auto resetButton = new QPushButton (tr("&Reset"));
    connect(resetButton, SIGNAL(clicked(bool)),
            this, SLOT(resetTable()));

    auto closeButton = new QPushButton (tr("&Close"));
    connect(closeButton, SIGNAL(clicked(bool)),
            this, SLOT(close()));

    auto bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget(resetButton);
    bottomLayout->addStretch();
    bottomLayout->addWidget(applyButton);
    bottomLayout->addWidget(closeButton);

    auto layout = new QVBoxLayout;
    layout->addLayout(topLayout);
    layout->addLayout(bottomLayout);

    this->setLayout(layout);
    }

IsothermModeler::ParameterMap
ModelerOptionDialog::getParameterMap()
    {
    using FitParameter = IsothermModeler::FitParameter;
    using FitParameters = IsothermModeler::FitParameters;
    using ParameterMap = IsothermModeler::ParameterMap;

    ParameterMap paramMap;

    int index = 0;
    for (const auto& info : *mIsoInfo)
        if (info.isModel())
            {
            QTableWidget* widget = static_cast<QTableWidget*>(mStackedLayout->widget(index));

            qDebug() << "Pointer = " << widget;

            paramMap[info.shortName()] = FitParameters {};

            for (int i = 0; i < info.numParams(); ++i)
                {
                qDebug() << "Current isotherm = " << info.shortName().c_str();

                bool ok1 = false;
                bool ok2 = false;
                bool ok3 = false;

                double init = widget->item(i, 0)->text().toDouble(&ok1);
                double min  = widget->item(i, 1)->text().toDouble(&ok2);
                double max  = widget->item(i, 2)->text().toDouble(&ok3);

                if (!(ok1 && ok2 && ok3))
                    {
                    int r = QMessageBox::critical(this, tr("Error"),
                        tr("Invalid value for initial guess or constraints"), QMessageBox::Close);

                    if (r == QMessageBox::Close)
                        throw IastGuiException {__FILE__, __LINE__, "Invalid value for initial guess or constraints"};
                    }

                paramMap[info.shortName()].push_back(FitParameter {init, min, max});
                }

            index++;
            }

    return paramMap;
    }

void
ModelerOptionDialog::resetTable()
    {
    int count = mStackedLayout->count();

    for (int i = 0; i < count; ++i)
        {
        QTableWidget* widget = static_cast<QTableWidget*>(mStackedLayout->widget(i));
        auto name = mListWidget->item(i)->text();

        int nRow = widget->rowCount();

        for (int r = 0; r < nRow; ++r)
            {
            widget->item(r, 0)->setText("1.0");
            widget->item(r, 1)->setText("0.0");
            widget->item(r, 2)->setText("10000.0");
            qDebug() << i << "    " << r;

            // Special initial guess for some parameters.
            // 1. BET
            auto param = widget->verticalHeaderItem(r)->text();
            if (name == "BET" && param == "k2")
                widget->item(r, 0)->setText("1e-5");
            }
        }

    emit optionChanged();
    }
