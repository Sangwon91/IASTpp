#ifndef PLOTTERAXISOPTION_H
#define PLOTTERAXISOPTION_H

#include <QWidget>
#include <QCheckBox>
#include <QLineEdit>
#include <QString>
#include <QDoubleSpinBox>

class PlotterAxisOption : public QWidget
{
    Q_OBJECT
public:
    explicit PlotterAxisOption(QWidget *parent = 0);

    bool isLogScale();
    QString getLabel();
    void setLabel(const QString& label);

    QCheckBox* getLogScaleCheckBox();

signals:

public slots:
    void changeState();
    void updateMaxSpin();
private:
    void setup();

private:
    QCheckBox* mAutoCheckBox;
    //QLineEdit* mMinText;
    //QLineEdit* mMaxText;
    QDoubleSpinBox* mMinSpin;
    QDoubleSpinBox* mMaxSpin;
    QCheckBox* mLogscaleCheckBox;
    QLineEdit* mLabelText;
};

#endif // PLOTTERAXISOPTION_H
