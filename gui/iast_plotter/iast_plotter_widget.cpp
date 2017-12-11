#include "iast_plotter_widget.h"

#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QString>
#include <QVector>
#include <QList>
#include <QMessageBox>
#include <QStackedLayout>
#include <QDesktopWidget>

#include <cmath>

#include "plotter_core.h"

// ExpressionParser
#include "../expression_parser.h"

IastPlotterWidget::IastPlotterWidget(QTableWidget *in, QTableWidget *out, QWidget *parent) :
    mInputTable {in},
    mOutputTable {out},
    QWidget(parent),
    isHide {true},
    isDataHidden {true}
    {
    this->setup();
    }

void
IastPlotterWidget::setup()
    {
    int dataGroupHeight = 400;
    int optionGroupHeight = 280;
    int buttonGroupHeight = 50;

    this->setWindowTitle(tr("Plotter"));
    this->setWindowIcon(QIcon(tr(":/images/plotter.png")));

    //mDummyWidget = new QWidget (this);

    mPlotterData = new IastPlotterData;

    mDataGroup = new QGroupBox (tr("Data"));
    auto dataLayout = new QHBoxLayout;
    dataLayout->addWidget(mPlotterData);
    mDataGroup->setLayout(dataLayout);
    mDataGroup->setMinimumWidth(600);
    mDataGroup->setMinimumHeight(dataGroupHeight);
    mDataGroup->hide();

    mPlotterOption = new IastPlotterOption (mOutputTable->rowCount() - 1);

    mOptionGroup = new QGroupBox (tr("Options"));
    auto optionLayout = new QHBoxLayout;
    optionLayout->addWidget(mPlotterOption);
    mOptionGroup->setLayout(optionLayout);
    mOptionGroup->setMinimumHeight(optionGroupHeight);
    mOptionGroup->hide();

    auto plotButton = new QPushButton (tr("Plot"));
    plotButton->setMinimumWidth(100);
    QFont bfont = qApp->font();
    bfont.setBold(true);
    plotButton->setFont(bfont);

    connect(plotButton, SIGNAL(clicked(bool)),
            this, SLOT(plot()));

    mMoreLess = new QPushButton (tr("Show Options"));
    mMoreLess->setIcon(QIcon(tr(":/images/cogwheel.png")));
    connect(mMoreLess, SIGNAL(clicked(bool)),
            this, SLOT(changeOptionWidget()));

    auto resizeWidget = [this, buttonGroupHeight, dataGroupHeight, optionGroupHeight]()
        {
        if (isHide && isDataHidden)
            {
            this->setMinimumHeight(buttonGroupHeight);
            this->resize(500, buttonGroupHeight);
            }
        else if (isHide && !isDataHidden)
            {
            this->setMinimumHeight(dataGroupHeight + buttonGroupHeight);
            this->adjustSize();
            }
        else if (!isHide && isDataHidden)
            {
            this->setMinimumHeight(optionGroupHeight + buttonGroupHeight);
            this->adjustSize();
            }
        else if (!isHide && !isDataHidden)
            {
            this->setMinimumHeight(dataGroupHeight + optionGroupHeight + buttonGroupHeight);
            this->adjustSize();
            }

        this->moveToCenter();
        };

    connect(mMoreLess, &QPushButton::clicked,
            resizeWidget);

    mDataShowHide = new QPushButton (tr("Show Data"));
    connect(mDataShowHide, SIGNAL(clicked(bool)),
            this, SLOT(changeDataWidget()));

    connect(mDataShowHide, &QPushButton::clicked,
            resizeWidget);

    auto plotLayout = new QHBoxLayout;
    plotLayout->addWidget(mDataShowHide);
    plotLayout->addWidget(mMoreLess);
    plotLayout->addStretch();
    plotLayout->addWidget(plotButton);
    //plotLayout->addStretch();

    auto layout = new QVBoxLayout;
    layout->addWidget(mDataGroup);
    layout->addWidget(mOptionGroup);
    layout->addStretch();
    layout->addLayout(plotLayout);

    this->setLayout(layout);
    //this->resize(700, 300);
    this->setMinimumWidth(500);
    //this->setMinimumHeight(50);
    //this->setMinimumSize(QSize(500, 50));
    qDebug() << __FILE__ << __LINE__ << "SIZE HINT:" << this->sizeHint();
    this->resize(500, buttonGroupHeight);

    this->moveToCenter();

    this->autoInitialize();
    }

void
IastPlotterWidget::setInputOutputTable(QTableWidget* in, QTableWidget* out)
    {
    mInputTable = in;
    mOutputTable = out;
    }

void
IastPlotterWidget::plot()
    {
    if (mPlotterData->getYExpression()->getTable()->rowCount() < 1)
        return;

    //auto gen = new CoreRangeGenerator (mTable);

    auto plotter = new PlotterCore;

    // Some settings should be here.

    ExpressionParser parser;
    ExpressionParser::VariableMap vm;

    int rowMin = 0;
    int rowMax = mInputTable->rowCount();

    // Min/max value of row should be modified here!!
    // Use PlotterRowRange class...

    // Code here
    auto rowRange = mPlotterOption->getRowRange();
    if (rowRange->getAllCheckBox()->checkState() == Qt::Unchecked)
        {
        rowMin = rowRange->getMinEdit()->value();
        rowMax = rowRange->getMaxEdit()->value();
        }

    // ---------------------------------------------------------------

    // i/oColMin = 0 always.
    int iColMax = mInputTable->columnCount();
    int oColMax = mOutputTable->columnCount();

    QString xExpression = mPlotterData->getXExpression()->getExpression();
    parser.setExpression(xExpression.toStdString());

    // Evaluate x values for x of data points from expression.
    QVector<double> xValues;

    try {
        for (int r = rowMin; r < rowMax; ++r)
            {
            for (int c = 0; c < iColMax; ++c)
                {
                QString key = "I";
                key += QString::number(c + 1);

                vm[key.toStdString()] = mInputTable->item(r, c)->text().toDouble();
                }

            for (int c = 0; c < oColMax; ++c)
                {
                QString key = "O";
                key += QString::number(c + 1);

                vm[key.toStdString()] = mOutputTable->item(r, c)->text().toDouble();
                }

            xValues << parser.evaluate(vm);
            qDebug() << xValues.back();
            }
        }
    catch (std::exception& e)
        {
        int r = QMessageBox::critical(this, tr("Error"),
                tr(e.what()), QMessageBox::Close);

        if (r == QMessageBox::Close)
            return;
        }

    auto yTable = mPlotterData->getYExpression()->getTable();

    QList<QString> yExpressions;
    for (int i = 0; i < yTable->rowCount(); ++i)
        yExpressions << yTable->item(i, 0)->text();

    // Evaluate y values for y of data points from expresssion.
    QList< QVector<double> > yValuesList;
    try {
        for (int i = 0; i < yTable->rowCount(); ++i)
            {
            QString yExpression = yExpressions[i];

            parser.setExpression(yExpression.toStdString());

            QVector<double> yValues;
            for (int r = rowMin; r < rowMax; ++r)
                {
                for (int c = 0; c < iColMax; ++c)
                    {
                    QString key = "I";
                    key += QString::number(c + 1);

                    vm[key.toStdString()] = mInputTable->item(r, c)->text().toDouble();
                    }

                for (int c = 0; c < oColMax; ++c)
                    {
                    QString key = "O";
                    key += QString::number(c + 1);

                    vm[key.toStdString()] = mOutputTable->item(r, c)->text().toDouble();
                    }

                yValues << parser.evaluate(vm);
                qDebug() << yValues.back();
                }

            yValuesList << yValues;
            }
        }
    catch (std::exception& e)
        {
        int r = QMessageBox::critical(this, tr("Error"),
                tr(e.what()), QMessageBox::Close);

        if (r == QMessageBox::Close)
            return;
        }

    QList<QString> legends;
    for (int i = 0; i < yTable->rowCount(); ++i)
        {
        legends << yTable->item(i, 1)->text();
        qDebug() << legends.back();
        }

    plotter->setXValues(xValues);
    plotter->setYValuesList(yValuesList);
    plotter->setLegends(legends);
    plotter->setXLogScale(mPlotterOption->isXLogScale());
    plotter->setYLogScale(mPlotterOption->isYLogScale());
    plotter->setXAxisLabel(mPlotterOption->getXAxisLabel());
    plotter->setYAxisLabel(mPlotterOption->getYAxisLabel());
    plotter->setHasLine(mPlotterOption->hasLine());
    plotter->setHasPoint(mPlotterOption->hasPoint());
    plotter->setLineWidth(mPlotterOption->getLineWidth());
    plotter->setPointSize(mPlotterOption->getPointSize());

    // Finally replot.
    plotter->plot();

    auto button = new QPushButton (tr("Export"));
    button->setIcon(QIcon(tr(":/images/save.png")));
    connect(button, SIGNAL(clicked(bool)),
            plotter, SLOT(saveAs()));

    auto bottom = new QHBoxLayout;
    bottom->addStretch();
    bottom->addWidget(button);

    auto layout = new QVBoxLayout;
    layout->addWidget(plotter);
    layout->addLayout(bottom);

    auto widget = new QWidget;
    widget->setLayout(layout);
    widget->setAttribute(Qt::WA_DeleteOnClose);

    widget->resize(650, 500);
    widget->move(QApplication::desktop()->screen()->rect().center() - widget->rect().center());

    widget->setWindowTitle(tr("Graph"));
    widget->setWindowIcon(QIcon(tr(":/images/plotter.png")));

    widget->show();

    connect(this, SIGNAL(destroyed(QObject*)),
            widget, SLOT(close()));
    }

void
IastPlotterWidget::changeOptionWidget()
    {
    if (isHide)
        {
        mOptionGroup->show();
        mMoreLess->setText(tr("Hide Options"));
        }
    else
        {
        mOptionGroup->hide();
        mMoreLess->setText(tr("Show Options"));
        }

    isHide = !isHide;
    }

void
IastPlotterWidget::changeDataWidget()
    {
    if (isDataHidden)
        {
        mDataGroup->show();
        mDataShowHide->setText(tr("Hide Data"));
        }
    else
        {
        mDataGroup->hide();
        mDataShowHide->setText(tr("Show Data"));
        }

    isDataHidden = !isDataHidden;
    }

// Parse input & output... and set the initial value of plotter...
void
IastPlotterWidget::autoInitialize()
    {
    QString extIn  =  mInputTable->horizontalHeaderItem(0)->text();
    QString extOut = mOutputTable->horizontalHeaderItem(0)->text();

    QString intIn  =  mInputTable->horizontalHeaderItem(1)->text();

    // do not suggest anything...
    if (extIn == "Pressure" && intIn[0] == 'x')
        return;

    // Parse calculation mode...

    int rowCount = mInputTable->rowCount();
    int colCount = mInputTable->columnCount();

    int varyingColumn = 0;
    bool isLogScale = false;

    // Find varying column in input table
    if (rowCount >= 2)
        {
        for (int c = 0; c < colCount; ++c)
            {
            QString text1 = mInputTable->item(0, c)->text();
            QString text2 = mInputTable->item(1, c)->text();

            if (text1 != text2)
                {
                varyingColumn = c;
                break;
                }
            }
        }

    qDebug() << __FILE__ << __LINE__ << "varying column:" << varyingColumn;

    // Find scale of varying column
    if (rowCount >= 3)
        {
        double tiny = 1e-5;

        double value0 = mInputTable->item(0, varyingColumn)->text().toDouble();
        double value1 = mInputTable->item(1, varyingColumn)->text().toDouble();
        double value2 = mInputTable->item(2, varyingColumn)->text().toDouble();

        double ratio0 = value0 / value1;
        double ratio1 = value1 / value2;

        if (std::abs(ratio0 - ratio1) < tiny)
            isLogScale = true;
        }

    // Set scale
    mPlotterOption->getXAxisOption()->getLogScaleCheckBox()->setChecked(isLogScale);

    qDebug() << __FILE__ << __LINE__ << "is log scale?" << isLogScale;

    // Set label...

    if (varyingColumn == 0)
        mPlotterOption->getXAxisOption()->setLabel(extIn);
    else
        {
        QString header = mInputTable->horizontalHeaderItem(varyingColumn)->text();
        // Remove 'x('/'y(' and ')'
        //header = header.mid(2, header.size() - 3);
        mPlotterOption->getXAxisOption()->setLabel(header);
        }

    mPlotterOption->getYAxisOption()->setLabel(extOut);

    // Set expressions...

    auto xExp = mPlotterData->getXExpression();
    auto yExp = mPlotterData->getYExpression();

    auto x = "I" + QString::number(varyingColumn + 1);
    xExp->setExpression(x);

    for (int c = 0; c < colCount; ++c)
        {
        yExp->addRow();
        yExp->getTable()->item(c, 0)->setText("O1 * O" + QString::number(c + 2));
        }

    for (int c = 0; c < colCount; ++c)
        {
        QString header = mOutputTable->horizontalHeaderItem(c + 1)->text();
        yExp->getTable()->item(c, 1)->setText(header.mid(2, header.size() - 3));
        }


    // Find row range
    if (rowCount >= 3)
        {
        auto rowRange = mPlotterOption->getRowRange();

        bool found = false;

        for (int c = 0; c < colCount; ++c)
            {
            if (c == varyingColumn)
                continue;

            QString previous = mInputTable->item(0, c)->text();
            for (int r = 1; r < rowCount; ++r)
                {
                QString current = mInputTable->item(r, c)->text();
                QString error = mOutputTable->item(r, 0)->text();

                if (previous != current || error == "0")
                    {
                    rowRange->getAllCheckBox()->setChecked(false);
                    rowRange->getMinEdit()->setValue(0);
                    rowRange->getMaxEdit()->setValue(r);

                    found = true;

                    break;
                    }

                previous = current;
                }

            if (found)
                break;
            }
        }

    }

void
IastPlotterWidget::moveToCenter()
    {
    this->move(QApplication::desktop()->screen()->rect().center() - this->rect().center());
    }
