#include "iast_core_widget.h"

#include <QGroupBox>
#include <QVBoxLayout>

#include "core_isotherm_table.h"
#include "core_input_widget.h"
#include "core_output_widget.h"
#include "core_calculator.h"

// Logger
#include "iast_gui_logger.h"

IastCoreWidget::IastCoreWidget(QWidget *parent) : QWidget(parent)
    {
    this->setup();
    }

void
IastCoreWidget::setup()
    {
    auto isothermGroup = new QGroupBox (tr("Isotherms"));
    auto table = new CoreIsothermTable;
    auto lay1 = new QVBoxLayout;
    lay1->addWidget(table);
    isothermGroup->setLayout(lay1);

    //auto variableGroup = new QGroupBox (tr("Variables"));
    //auto variable = new CoreVariableChooser;
    //auto lay3 = new QVBoxLayout;
    //lay3->addWidget(variable);
    //variableGroup->setLayout(lay3);

    auto inputGroup = new QGroupBox (tr("Inputs"));
    auto input = new CoreInputWidget;
    auto lay2 = new QVBoxLayout;

    input->getIsothermTable(table->table());

    lay2->addWidget(input);
    inputGroup->setLayout(lay2);

    connect(table, SIGNAL(makeClicked(int,int,int)),
            input, SLOT(makeTable(int,int,int)));

    auto outputGroup = new QGroupBox (tr("Outputs"));
    auto output = new CoreOutputWidget;
    auto lay3 = new QVBoxLayout;

    output->getIsothermTable(table->table());
    output->getInputTable(input->table());

    lay3->addWidget(output);
    outputGroup->setLayout(lay3);

    // Not a QWidget but QObject
    auto calculator = new CoreCalculator(table, input, output, this);
    connect(input, SIGNAL(calculateClicked()), calculator, SLOT(calculate()));
    connect(input, SIGNAL(calculateClicked()), output, SLOT(openPlotter()));

    //connect(input->table(), SIGNAL(cellChanged(int,int)),
    //       output->table(), SLOT(clear()));
    //connect(input->table(), SIGNAL(itemChanged(QTableWidgetItem*)),
    //        output->table(), SLOT(clear()));

    connect(input, SIGNAL(tableChanged()),
            output, SLOT(cleanUpTable()));

    auto layout = new QVBoxLayout;

    layout->addWidget(isothermGroup);
    //layout->addWidget(variableGroup);
    layout->addWidget(inputGroup);
    layout->addWidget(outputGroup);

    this->setLayout(layout);
    }
