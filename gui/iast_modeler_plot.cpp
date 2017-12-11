#include "iast_modeler_plot.h"

#include <QEasingCurve>
#include <QMessageBox>

#include <QDebug>

#include <cmath>

// Logger
#include "iast_gui_logger.h"

IastModelerPlot::IastModelerPlot(QWidget* parent) : QChartView (parent)
    {
    mModelSeries = new QLineSeries (this);
    mModelSeries->setName(tr("Model"));

    mDataSeries = new QScatterSeries (this);
    mDataSeries->setName(tr("Data"));
    mDataSeries->setMarkerSize(10.0);

    mChart = new QChart;

    mChart->setPlotAreaBackgroundVisible(false);
    mChart->setTheme(QChart::ChartTheme::ChartThemeLight);
    mChart->setAnimationOptions(QChart::AnimationOption::SeriesAnimations);
    mChart->setAnimationDuration(100);
    mChart->setAnimationEasingCurve(QEasingCurve (QEasingCurve::Type::InOutQuad));

    mChart->addSeries(mModelSeries);
    mChart->addSeries(mDataSeries);

    // Default axis. linear scale
    mChart->createDefaultAxes();

    mChart->axisX()->setTitleText(tr("Pressure"));
    mChart->axisY()->setTitleText(tr("Uptake"));

    // Connect to chart
    this->setChart(mChart);

    // For better looks
    this->setRenderHint(QPainter::Antialiasing);
    //this->setStyleSheet("background-color:white;");

    // End QtCharts Test -----------------------------------------------------
    }

void
IastModelerPlot::changeTheme(int index)
    {
    switch (index)
        {
        case QChart::ChartThemeLight:
            mChart->setTheme(QChart::ChartThemeLight);
            break;

        case QChart::ChartThemeBlueCerulean:
            mChart->setTheme(QChart::ChartThemeBlueCerulean);
            break;

        case QChart::ChartThemeDark:
            mChart->setTheme(QChart::ChartThemeDark);
            break;

        case QChart::ChartThemeBrownSand:
            mChart->setTheme(QChart::ChartThemeBrownSand);
            break;

        case QChart::ChartThemeBlueNcs:
            mChart->setTheme(QChart::ChartThemeBlueNcs);
            break;

        case QChart::ChartThemeHighContrast:
            mChart->setTheme(QChart::ChartThemeHighContrast);
            break;

        case QChart::ChartThemeBlueIcy:
            mChart->setTheme(QChart::ChartThemeBlueIcy);
            break;

        case QChart::ChartThemeQt:
            mChart->setTheme(QChart::ChartThemeQt);
            break;

        default:
            break;
        }
    }

void
IastModelerPlot::changeScaleX(AxisScale scale)
    {
    // Do nothing for same scale
    if (scale == mXScale)
        return;

    QAbstractAxis* axisXX;

    if (scale == AxisScale::Log)
        {
        int logMax = std::ceil(std::log10(mMaxX));
        int logMin = std::floor(std::log10(mMinX));

        if (logMax - logMin < 1)
            {
            int r = QMessageBox::critical(this, tr("Error"),
                tr("The range of x axis is too short."), QMessageBox::Close);

            if (r == QMessageBox::Close)
                return;
            }

        auto axisX = new QLogValueAxis;
        axisXX = axisX;

        axisX->setLabelFormat("%.0e");
        axisX->setBase(10);

        mChart->setAxisX(axisX);

        mXScale = scale;
        }
    else if (scale == AxisScale::Linear)
        {
        auto axisX = new QValueAxis;
        axisXX = axisX;

        axisX->setTitleText("Pressure");

        mChart->setAxisX(axisX);

        mXScale = scale;
        }

    mModelSeries->attachAxis(axisXX);
    mDataSeries->attachAxis(axisXX);

    mChart->axisX()->setTitleText("Pressure");
    mChart->axisY()->setTitleText("Uptake");

    this->fitAxisRange();
    }

void
IastModelerPlot::changeScaleY(AxisScale scale)
    {
    qDebug() << "Y scale change!";
    qDebug() << "state = " << scale;
    // Do nothing for same scale
    if (scale == mYScale)
        return;

    QAbstractAxis* axisYY;

    if (scale == AxisScale::Log)
        {
        qDebug() << "y log scale occurs";
        int logMax = std::ceil(std::log10(mMaxY));
        int logMin = std::floor(std::log10(mMinY));

        if (logMax - logMin < 1)
            {
            int r = QMessageBox::critical(this, tr("Error"),
                tr("The range of y axis is too short."), QMessageBox::Close);

            if (r == QMessageBox::Close)
                return;
            }

        auto axisY = new QLogValueAxis;
        axisYY = axisY;

        axisY->setLabelFormat("%.0e");
        axisY->setBase(10);

        mChart->setAxisY(axisY);

        mYScale = scale;
        }
    else if (scale == AxisScale::Linear)
        {
        qDebug() << "y linear scale occurs";
        auto axisY = new QValueAxis;
        axisYY = axisY;

        axisY->setTitleText("Pressure");

        mChart->setAxisY(axisY);

        mYScale = scale;
        }
    else
        {
        qDebug() << "Unpredictable case occurs";
        }

    mModelSeries->attachAxis(axisYY);
    mDataSeries->attachAxis(axisYY);

    mChart->axisX()->setTitleText("Pressure");
    mChart->axisY()->setTitleText("Uptake");

    this->fitAxisRange();
    }

void
IastModelerPlot::setModelPoints(const Points& x, const Points& y)
    {
    qDebug() << "model clear";
    qDebug() << mModelSeries;
    mModelSeries->clear();

    qDebug() << "Size test";
    int imax = x.size() < y.size() ? x.size() : y.size();


    for (int i = 0; i < imax; ++i)
        mModelSeries->append(x[i], y[i]);
    }

void
IastModelerPlot::setDataPoints(const Points& x, const Points&y)
    {
    mDataSeries->clear();

    int imax = x.size() < y.size() ? x.size() : y.size();

    for (int i = 0; i < imax; ++i)
        mDataSeries->append(x[i], y[i]);

    this->findDataMinMax();
    this->fitAxisRange();
    }

void
IastModelerPlot::fitAxisRange()
    {
    if (mXScale == AxisScale::Log)
        {
        double logMax = std::pow(10, std::ceil(std::log10(mMaxX)));
        double logMin = std::pow(10, std::floor(std::log10(mMinX)));

        mChart->axisX()->setRange(logMin, logMax);
        }
    else // Linear scale
        {
        mChart->axisX()->setRange(0, mMaxX * 1.1);
        }

    if (mYScale == AxisScale::Log)
        {
        double logMax = std::pow(10, std::ceil(std::log10(mMaxY)));
        double logMin = std::pow(10, std::floor(std::log10(mMinY)));

        mChart->axisY()->setRange(logMin, logMax);
        }
    else // Linear scale
        {
        mChart->axisY()->setRange(0, mMaxY * 1.1);
        }
    }

void
IastModelerPlot::findDataMinMax()
    {
    // std algorithm also available
    double minX = 1.0e300;
    double minY = 1.0e300;

    double maxX = -1.0e300;
    double maxY = -1.0e300;

    int count = mDataSeries->count();

    if (count == 0)
        return;

    for (int i = 0; i < count; ++i)
        {
        double x = mDataSeries->at(i).x();
        double y = mDataSeries->at(i).y();

        if (x < minX && x > 0.0)
            minX = x;

        if (y < minY && y > 0.0)
            minY = y;

        if (x > maxX)
            maxX = x;

        if (y > maxY)
            maxY = y;
        }

    mMinX = minX;
    mMaxX = maxX;

    mMinY = minY;
    mMaxY = maxY;
    }

double
IastModelerPlot::minX()
    {
    return mMinX;
    }

double
IastModelerPlot::maxX()
    {
    return mMaxX;
    }

double
IastModelerPlot::minY()
    {
    return mMinY;
    }

double
IastModelerPlot::maxY()
    {
    return mMaxY;
    }

double
IastModelerPlot::x(int i)
    {
    return mDataSeries->at(i).x();
    }

double
IastModelerPlot::y(int i)
    {
    return mDataSeries->at(i).y();
    }

int
IastModelerPlot::count()
    {
    return mDataSeries->count();
    }
