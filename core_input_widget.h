#ifndef COREINPUTWIDGET_H
#define COREINPUTWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>

#include "core_range_generator.h"

class CoreInputWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CoreInputWidget(QWidget *parent = 0);
    QTableWidget* table();

    void getIsothermTable(QTableWidget* isothermTable);

protected:
    virtual void keyPressEvent(QKeyEvent* event) override;

signals:
    void calculateClicked();
    void tableMade();
    void tableChanged();

public slots:
    void makeTable(int row, int extensive, int intensive);
    void addRow();
    void addRows(int num);
    void addRowsWithSpinbox();
    void deleteRows();
    void openGenerator();
    void calculate();

private:
    QTableWidget* mTable;
    QPushButton* mGeneratorButton;
    QPushButton* mAddButton;
    QPushButton* mDeleteButton;
    QPushButton* mCalculateButton;

    QTableWidget* mIsothermTable;

    CoreRangeGenerator* mRangeGenerator;

    void setup();
    void copy();
    void paste();
};

#endif // COREINPUTWIDGET_H
