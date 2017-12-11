#ifndef PLOTTERSTYLEOPTION_H
#define PLOTTERSTYLEOPTION_H

#include <QWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>

class PlotterStyleOption : public QWidget
{
    Q_OBJECT
public:
    explicit PlotterStyleOption(QWidget *parent = 0);

    bool hasLine();
    bool hasPoint();

    int getLineWidth();
    int getPointSize();

signals:

public slots:

private:
    void setup();

private:
    QCheckBox* mLineCheckBox;
    QCheckBox* mPointCheckBox;

    QSpinBox* mLineWidth;
    QSpinBox* mPointSize;

    QComboBox* mThemeComboBox;
};

#endif // PLOTTERSTYLEOPTION_H
