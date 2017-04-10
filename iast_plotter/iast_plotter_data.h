#ifndef IASTPLOTTERDATA_H
#define IASTPLOTTERDATA_H

#include <QWidget>

#include "plotter_x_expression.h"
#include "plotter_y_expression.h"
#include "plotter_row_range.h"

class IastPlotterData : public QWidget
{
    Q_OBJECT
public:
    explicit IastPlotterData(QWidget *parent = 0);

    PlotterXExpression* getXExpression();
    PlotterYExpression* getYExpression();

signals:

public slots:

private:
    PlotterXExpression* mXExpression;
    PlotterYExpression* mYExpression;
    PlotterRowRange* mRowRange;

private:
    void setup();
};

#endif // IASTPLOTTERDATA_H
