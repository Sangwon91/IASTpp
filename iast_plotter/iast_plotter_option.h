#ifndef IASTPLOTTEROPTION_H
#define IASTPLOTTEROPTION_H

#include <QWidget>
#include <QString>

#include "plotter_axis_option.h"
#include "plotter_style_option.h"
#include "plotter_row_range.h"

class IastPlotterOption : public QWidget
{
    Q_OBJECT
public:
    explicit IastPlotterOption(int rowMax, QWidget *parent = 0);

    bool isXLogScale();
    bool isYLogScale();

    QString getXAxisLabel();
    QString getYAxisLabel();

    bool hasLine();
    bool hasPoint();

    int getLineWidth();
    int getPointSize();

    PlotterAxisOption* getXAxisOption();
    PlotterAxisOption* getYAxisOption();
    PlotterRowRange* getRowRange();

signals:

public slots:

private:
    void setup();

private:
    PlotterAxisOption* mXAxisOption;
    PlotterAxisOption* mYAxisOption;
    PlotterStyleOption* mStyleOption;
    PlotterRowRange* mRowRange;

    int mRowMax;
};

#endif // IASTPLOTTEROPTION_H
