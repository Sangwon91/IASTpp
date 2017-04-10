#ifndef CORECALCULATOR_H
#define CORECALCULATOR_H

#include <QObject>
#include <QTableWidget>
#include <QPushButton>
#include <QProgressBar>

#include "core_isotherm_table.h"
#include "core_input_widget.h"
#include "core_output_widget.h"

class CoreCalculator : public QObject
{
    Q_OBJECT
public:
    explicit CoreCalculator(CoreIsothermTable* isoWidget,
                            CoreInputWidget* inputWidget,
                            CoreOutputWidget* outputWidget,
                            QObject *parent = 0);

signals:

public slots:
    void calculate();

private:
    CoreIsothermTable* mIsoWidget;
    CoreInputWidget* mInputWidget;
    CoreOutputWidget* mOutputWidget;

    void setup();
};

#endif // CORECALCULATOR_H
