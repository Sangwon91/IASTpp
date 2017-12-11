#ifndef IASTMODELERWIDGET_H
#define IASTMODELERWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QTextEdit>

// QtCharts
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>

QT_CHARTS_USE_NAMESPACE

#include <vector>

#include "isotherm_information.h"

#include "iast_modeler_plot.h"
#include "modeler_option_dialog.h"

class IastModelerWidget : public QWidget
    {
    Q_OBJECT
public:
    explicit IastModelerWidget(QWidget* parent = 0);

private:
    QWidget* createInputWidget();
    QWidget* createOutputWidget();
    QWidget* createPlotOptionWidget();

public slots:    
    void findButtonClicked();

    void plotThemeChanged(int index);
    void xScaleChanged(int state);
    void yScaleChanged(int state);

    void fitModel(const QString& text);

    void saveIsoFile();

private:
    std::vector<IsothermInformation> mIsoInfo;

    IastModelerPlot* mIastModelerPlot = nullptr;
    ModelerOptionDialog* mModelerOptionDialog = nullptr;

    QLineEdit* mFileEdit = nullptr;
    QComboBox* mModelCombo = nullptr;

    QTextEdit* mOutputText = nullptr;

    QCheckBox* mExceptInterpolator = nullptr;

    QCheckBox* mXLogScaleCheckBox = nullptr;
    QCheckBox* mYLogScaleCheckBox = nullptr;

    double mLastR2;
private:
    void readData();
    void setupIsothermInformation();
    };

#endif // IASTMODELERWIDGET_H
