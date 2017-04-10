#ifndef IASTMODELERPLOT_H
#define IASTMODELERPLOT_H

#include <QWidget>

// QtCharts
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLogValueAxis>

QT_CHARTS_USE_NAMESPACE

#include <vector>

class IastModelerPlot : public QChartView
{
    Q_OBJECT

public:
    enum AxisScale {Linear = 0, Log};

    using Points = std::vector<double>;

    IastModelerPlot(QWidget* parent = nullptr);

    void changeTheme(int index);

    void changeScaleX(AxisScale scale);
    void changeScaleY(AxisScale scale);

    void setModelPoints(const Points& x, const Points& y);
    void setDataPoints(const Points& x, const Points& y);

    double minX();
    double maxX();

    double minY();
    double maxY();

    double x(int i);
    double y(int i);

    int count();
private:

    QChart* mChart = nullptr;
    QLineSeries* mModelSeries = nullptr;
    QScatterSeries* mDataSeries = nullptr;

    // Minimum and maximum of data points
    double mMinX;
    double mMaxX;

    double mMinY;
    double mMaxY;

    // Current axes scale : linear (defualt)
    AxisScale mXScale = AxisScale::Linear;
    AxisScale mYScale = AxisScale::Linear;

    void fitAxisRange();
    void findDataMinMax();
};

#endif // IASTMODELERPLOT_H
