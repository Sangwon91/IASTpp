#ifndef ISOTHERMCALCULATOR_H
#define ISOTHERMCALCULATOR_H

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QCheckBox>

class IsothermCalculator : public QWidget
{
    Q_OBJECT
public:
    explicit IsothermCalculator(QWidget *parent = 0);

signals:

public slots:
    void importIsotherm();
    void calculate();
    void exportOutput();
    void plotOutput();

private:
    void setup();

private:
    // Buttons
    QPushButton* mImportButton;
    QPushButton* mExportButton;
    QPushButton* mCalculateButton;
    QPushButton* mPlotButton;

    // LineEdits
    QLineEdit* mPathLine;

    QLineEdit* mPMinLine;
    QLineEdit* mPMaxLine;
    QLineEdit* mNumPointsLine;

    // TextEdits
    QTextEdit* mIsothermInfoText;
    QTextEdit* mOutputText;

    // Logscale check
    QCheckBox* mLogscaleCheck;
};

#endif // ISOTHERMCALCULATOR_H
