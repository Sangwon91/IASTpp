#include "plotter_core.h"

#include <QHBoxLayout>
#include <QFrame>
#include <QtMath>
#include <QFont>
#include <QPen>
#include <QBrush>

PlotterCore::PlotterCore(QWidget *parent) :
    QWidget(parent),
    mPlot {nullptr},
    mIsXLogScale {false},
    mIsYLogScale {false},
    mHasLine {true},
    mHasPoint {true},
    mLineWidth {2},
    mPointSize {5}
    {
    this->setup();
    }

void
PlotterCore::setup()
    {
    this->setWindowTitle(tr("Graph"));
    this->setGeometry(400, 250, 540, 390);

    // Build plot.

    // To prevent memory leak.
    if (mPlot != nullptr)
        delete mPlot;

    mPlot = new QCustomPlot;

    mPlot->legend->setVisible(true);
    mPlot->legend->setFont(QFont("Arial, 9"));

    mPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    mPlot->xAxis2->setVisible(true);
    mPlot->xAxis2->setTickLabels(false);

    mPlot->yAxis2->setVisible(true);
    mPlot->yAxis2->setTickLabels(false);

    auto layout = new QHBoxLayout;
    layout->addWidget(mPlot);

    this->setLayout(layout);
    }

void
PlotterCore::setXValues(const QVector<double>& in)
    {
    mXValues = in;
    }

void
PlotterCore::setYValuesList(const QList< QVector<double> >& in)
    {
    mYValuesList = in;
    }

void
PlotterCore::setLegends(const QList<QString>& in)
    {
    mLegends = in;
    }
void
PlotterCore::setXLogScale(bool b)
    {
    mIsXLogScale = b;
    }

void
PlotterCore::setYLogScale(bool b)
    {
    mIsYLogScale = b;
    }

void
PlotterCore::setXAxisLabel(const QString& label)
    {
    mXAxisLabel = label;
    }

void
PlotterCore::setYAxisLabel(const QString& label)
    {
    mYAxisLabel = label;
    }

void
PlotterCore::setHasLine(bool b)
    {
    mHasLine = b;
    }

void
PlotterCore::setHasPoint(bool b)
    {
    mHasPoint = b;
    }
void
PlotterCore::setLineWidth(int w)
    {
    mLineWidth = w;
    }

void
PlotterCore::setPointSize(int s)
    {
    mPointSize = s;
    }

void
PlotterCore::plot()
    {
    mPlot->clearGraphs();

    auto color = [](int i)
        {
        /*
        i += QCPGraph::lsNone;

        return QColor(qSin(i * 1 + 1.2) * 100 + 100,
                      qSin(i * 0.3 + 0) * 100 + 100,
                      qSin(i * 0.3 + 1.5) * 100 + 100);
        */

        int s = 256 / 6;

        QColor c;
        c.setHsv(s * i,
                 240 - 30 * (2 * s * i / 256),
                 240 - 20 * (2 * s * i / 256));

        return c;
        };

    QPen pen;
    pen.setWidth(mLineWidth);

    // 9 types of marker...
    QList<QCPScatterStyle::ScatterShape> markerStyles;
    markerStyles << QCPScatterStyle::ssCircle <<
                    QCPScatterStyle::ssSquare <<
                    QCPScatterStyle::ssDiamond <<
                    QCPScatterStyle::ssTriangle <<
                    QCPScatterStyle::ssTriangleInverted <<
                    QCPScatterStyle::ssDisc <<
                    QCPScatterStyle::ssPlus <<
                    QCPScatterStyle::ssCross <<
                    QCPScatterStyle::ssStar;

    int size = mYValuesList.size();
    for (int i = 0; i < size; ++i)
        {
        pen.setColor(color(i));

        mPlot->addGraph();

        mPlot->graph()->setPen(pen);
        mPlot->graph()->setName(mLegends.at(i));

        if (mHasLine)
            mPlot->graph()->setLineStyle((QCPGraph::LineStyle::lsLine));
        else
            mPlot->graph()->setLineStyle((QCPGraph::LineStyle::lsNone));

        if (mHasPoint)
            {
            QBrush brush;
            brush.setColor(color(i));

            QCPScatterStyle style(markerStyles[i % 8], pen, brush, mPointSize);

            mPlot->graph()->setScatterStyle(style);
            }

        mPlot->graph()->setData(mXValues, mYValuesList.at(i));
        //mPlot->graph()->rescaleAxes(true);
        }

    // Set scale option
    if (mIsXLogScale)
        {
        mPlot->xAxis->setScaleType(QCPAxis::stLogarithmic);
        mPlot->xAxis2->setScaleType(QCPAxis::stLogarithmic);
        QSharedPointer<QCPAxisTickerLog> logTicker(new QCPAxisTickerLog);
        mPlot->xAxis->setTicker(logTicker);
        mPlot->xAxis2->setTicker(logTicker);
        mPlot->xAxis->setNumberFormat("eb"); // e = exponential, b = beautiful decimal powers
        mPlot->xAxis->setNumberPrecision(0); // makes sure "1*10^4" is displayed only as "10^4"
        }

    if (mIsYLogScale)
        {
        mPlot->yAxis->setScaleType(QCPAxis::stLogarithmic);
        mPlot->yAxis2->setScaleType(QCPAxis::stLogarithmic);
        QSharedPointer<QCPAxisTickerLog> logTicker(new QCPAxisTickerLog);
        mPlot->yAxis->setTicker(logTicker);
        mPlot->yAxis2->setTicker(logTicker);
        mPlot->yAxis->setNumberFormat("eb"); // e = exponential, b = beautiful decimal powers
        mPlot->yAxis->setNumberPrecision(0); // makes sure "1*10^4" is displayed only as "10^4"
        }

    // Label Font
    QFont font = QFont("Arial", 12);

    mPlot->xAxis->setLabel(mXAxisLabel);
    mPlot->xAxis->setLabelFont(font);

    mPlot->yAxis->setLabel(mYAxisLabel);
    mPlot->yAxis->setLabelFont(font);

    mPlot->rescaleAxes();
    mPlot->axisRect()->setupFullAxesBox();

    mPlot->replot();
    }

void
PlotterCore::saveAs()
    {
    QString filter;
    auto fileName = QFileDialog::getSaveFileName(nullptr, tr("Export Graph"),
                    QString {}, tr ("png (*.png);;jpg (*.jpg);;pdf (*.pdf);;bmp (*.bmp)"),
                    &filter);

    qDebug() << __FILE__ << __LINE__ << "File name:" << fileName;
    qDebug() << __FILE__ << __LINE__ << "Filter:" << filter;

    if (filter == "png (*.png)")
        {
        if (!fileName.endsWith(".png", Qt::CaseInsensitive))
            fileName += ".png";

        mPlot->savePng(fileName);
        }
    else if (filter == "jpg (*.jpg)")
        {
        if (!fileName.endsWith(".jpg", Qt::CaseInsensitive))
            fileName += ".jpg";

        mPlot->saveJpg(fileName);
        }
    else if (filter == "pdf (*.pdf)")
        {
        if (!fileName.endsWith(".pdf", Qt::CaseInsensitive))
            fileName += ".pdf";

        mPlot->savePdf(fileName);
        }
    else if (filter == "bmp (*.bmp)")
        {
        if (!fileName.endsWith(".bmp", Qt::CaseInsensitive))
            fileName += ".bmp";

        mPlot->saveBmp(fileName);
        }
    }
