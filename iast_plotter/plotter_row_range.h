#ifndef PLOTTERROWRANGE_H
#define PLOTTERROWRANGE_H

#include <QWidget>
#include <QCheckBox>
#include <QSpinBox>

class PlotterRowRange : public QWidget
{
    Q_OBJECT
public:
    explicit PlotterRowRange(int rowMax, QWidget *parent = 0);

    QCheckBox* getAllCheckBox();
    QSpinBox* getMinEdit();
    QSpinBox* getMaxEdit();

signals:

public slots:
    void changeState();
    void updateMaxValue();

private:
    void setup();

private:
    QCheckBox* mAllCheckBox;
    QSpinBox* mMinText;
    QSpinBox* mMaxText;

    int mRowMax;
};

#endif // PLOTTERROWRANGE_H
