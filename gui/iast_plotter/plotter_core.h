#ifndef PLOTTERCORE_H
#define PLOTTERCORE_H

#include <QWidget>
#include <QString>
#include <QVector>
#include <QList>

// QCustomPlot
#include "../qcustomplot/qcustomplot.h"

class PlotterCore : public QWidget
{
    Q_OBJECT
public:
    explicit PlotterCore(QWidget *parent = 0);

    void setXValues(const QVector<double>& in);
    void setYValuesList(const QList< QVector<double> >& in);
    void setLegends(const QList<QString>& in);
    void setXLogScale(bool b);
    void setYLogScale(bool b);
    void setXAxisLabel(const QString& label);
    void setYAxisLabel(const QString& label);
    void setHasLine(bool b);
    void setHasPoint(bool b);
    void setLineWidth(int w);
    void setPointSize(int s);

    void plot();

signals:

public slots:
    void saveAs();

private:
    void setup();

private:
    QCustomPlot* mPlot;

    QVector<double> mXValues;
    QList< QVector<double> > mYValuesList;
    QList<QString> mLegends;

    bool mIsXLogScale;
    bool mIsYLogScale;

    QString mXAxisLabel;
    QString mYAxisLabel;

    bool mHasLine;
    bool mHasPoint;

    int mLineWidth;
    int mPointSize;
};

#endif // PLOTTERCORE_H
