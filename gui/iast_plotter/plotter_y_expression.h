#ifndef PLOTTERYEXPRESSION_H
#define PLOTTERYEXPRESSION_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>

class PlotterYExpression : public QWidget
{
    Q_OBJECT
public:
    explicit PlotterYExpression(QWidget *parent = 0);

    QTableWidget* getTable();

signals:

public slots:
    void addRow();
    void deleteRows();
    void showHelpMessage();

private:
    void setup();

private:
    QTableWidget* mTable;
    QPushButton* mAddButton;
    QPushButton* mDeleteButton;
};

#endif // PLOTTERYEXPRESSION_H
