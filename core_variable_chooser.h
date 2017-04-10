#ifndef COREVARIABLECHOOSER_H
#define COREVARIABLECHOOSER_H

#include <QWidget>
#include <QPushButton>
#include <QRadioButton>

class CoreVariableChooser : public QWidget
{
    Q_OBJECT
public:
    enum Extensive {Pressure = 0, Uptake};
    enum Intensive {X = 0, Y};

    explicit CoreVariableChooser(QWidget *parent = 0);
    int getExtensive();
    int getIntensive();

signals:
    void changed();

public slots:

private:
    QRadioButton* mPressure;
    QRadioButton* mUptake;
    QRadioButton* mX;
    QRadioButton* mY;

    void setup();
};

#endif // COREVARIABLECHOOSER_H
