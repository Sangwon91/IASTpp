#ifndef COREOUTPUTWIDGET_H
#define COREOUTPUTWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QTableWidget>

#include "iast_plotter/iast_plotter_widget.h"

class CoreOutputWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CoreOutputWidget(QWidget *parent = 0);
    QTableWidget* table();

    void getIsothermTable(QTableWidget* isothermTable);
    void getInputTable(QTableWidget* inputTable);

protected:
    virtual void keyPressEvent(QKeyEvent* event) override;

signals:

public slots:
    void cleanUpTable();
    void exportTable();
    void openPlotter();

private:
    QTableWidget* mTable;
    QPushButton* mExportButton;
    QPushButton* mPlotButton;
    QPushButton* mCleanButton;

    // For exporting functionality
    QTableWidget* mInputTable;

    QTableWidget* mIsothermTable;

    IastPlotterWidget* mPlotter;

    void setup();
    void copy();
};

#endif // COREOUTPUTWIDGET_H
