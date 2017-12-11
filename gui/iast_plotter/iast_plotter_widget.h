#ifndef IASTPLOTTERWIDGET_H
#define IASTPLOTTERWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QGroupBox>
#include <QTableWidget>

#include "iast_plotter_data.h"
#include "iast_plotter_option.h"

class IastPlotterWidget : public QWidget
{
    Q_OBJECT
public:
    explicit IastPlotterWidget(QTableWidget* in, QTableWidget* out, QWidget *parent = 0);
    void setInputOutputTable(QTableWidget* in, QTableWidget* out);

    void autoInitialize();

signals:

public slots:
    void plot();
    void changeOptionWidget();
    void changeDataWidget();
    void moveToCenter();

private:
    QTableWidget* mInputTable;
    QTableWidget* mOutputTable;

    IastPlotterData* mPlotterData;
    IastPlotterOption* mPlotterOption;

    QPushButton* mDataShowHide;
    QGroupBox* mDataGroup;

    QPushButton* mMoreLess;
    QGroupBox* mOptionGroup;

    bool isHide;
    bool isDataHidden;

    //QWidget* mDummyWidget;

private:
    void setup();
};

#endif // IASTPLOTTERWIDGET_H
