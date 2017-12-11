#include "iast_modeler_widget.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QSplitter>
#include <QStyle>
#include <QLabel>
#include <QFrame>
#include <QPushButton>
#include <QEasingCurve>
#include <QLineEdit>
#include <QComboBox>
#include <QRect>
#include <QList>
#include <QCheckBox>
#include <QTextEdit>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QRegExp>
#include <QScrollArea>
#include <QDebug>
#include <QFont>

// QtCharts
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLogValueAxis>

QT_CHARTS_USE_NAMESPACE

#include <string>
#include <vector>
#include <cmath>
#include <ciso646>

// Iast core modules
#include "../iast/isotherm_factory.hpp"
#include "../iast/isotherm_utility.hpp"
#include "iast_gui_exception.h"

// Logger
#include "iast_gui_logger.h"

IastModelerWidget::IastModelerWidget(QWidget* parent) : QWidget (parent)
    {
    this->setupIsothermInformation();
    // Make plot widget
    mIastModelerPlot = new IastModelerPlot (this);
    auto plotWidget = mIastModelerPlot;

    auto rightLayout = new QHBoxLayout (this);
    rightLayout->addWidget(plotWidget);

    auto rightWidget = new QWidget (this);
    rightWidget->setLayout(rightLayout);

    // Make input & output widgets
    auto inputWidget = this->createInputWidget();
    auto inputLayout = new QVBoxLayout (this);
    inputLayout->addWidget(inputWidget);

    auto inputGroupBox = new QGroupBox (tr("Input"), this);
    inputGroupBox->setLayout(inputLayout);

    auto outputWidget = this->createOutputWidget();
    auto outputLayout = new QVBoxLayout (this);
    outputLayout->addWidget(outputWidget);

    auto outputGroupBox = new QGroupBox (tr("Output"), this);
    outputGroupBox->setLayout(outputLayout);

    auto plotOptionWidget = this->createPlotOptionWidget();

    auto plotOptionLayout = new QVBoxLayout (this);
    plotOptionLayout->addWidget(plotOptionWidget);

    auto plotOptionGroupBox = new QGroupBox (tr("Plot Option"), this);
    //plotOptionGroupBox->setFlat(true);
    plotOptionGroupBox->setLayout(plotOptionLayout);

    auto leftLayout = new QVBoxLayout (this);
    leftLayout->addWidget(inputGroupBox);
    leftLayout->addWidget(outputGroupBox);
    leftLayout->addStretch();
    leftLayout->addWidget(plotOptionGroupBox);

    //auto leftWidget = new QWidget (this);
    //leftWidget->setLayout(leftLayout);
    //leftWidget->setGeometry(QRect(0, 0, 5, 6));

    auto leftWidget = new QWidget (this);

    leftWidget->setLayout(leftLayout);
    leftWidget->setGeometry(QRect(0, 0, 5, 6));

    // Splitter -----------
    auto splitter = new QSplitter (Qt::Horizontal, this);
    //auto handle = splitter->handle(1);

    // Change outline of splitter not splitter handle
    splitter->setFrameStyle(QFrame::WinPanel | QFrame::Raised);
    splitter->setLineWidth(2);
    splitter->setMidLineWidth(1);
    splitter->setHandleWidth (10);

    splitter->addWidget(rightWidget);
    splitter->addWidget(leftWidget);

    QList<int> sizes {500, 300};
    splitter->setSizes(sizes);

    // Modeler Layout -----
    auto layout = new QHBoxLayout (this);
    layout->addWidget(splitter);

    this->setLayout(layout);
    }

void
IastModelerWidget::readData()
    {
    auto path = mFileEdit->text();

    QFile file (path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
        int r = QMessageBox::critical(this, tr("Error"),
                tr("File open fails."), QMessageBox::Close);

        mFileEdit->setText("");

        if (r == QMessageBox::Close)
            return;
        }

    QTextStream in (&file);

    IastModelerPlot::Points xs, ys;

    while (!in.atEnd())
        {
        QString line = in.readLine();
        auto tokens = line.split(QRegExp("[ \\t\\n\\r]"), QString::SkipEmptyParts);

        // Neglect empty lines.
        if (tokens.size() == 0)
            {
            continue;
            }
        else if (tokens.size() != 2)
            {
            // Prevent additional column.
            int r = QMessageBox::critical(this, tr("Error"),
                    tr("Invalid data format"), QMessageBox::Close);

            mFileEdit->setText("");

            if (r == QMessageBox::Close)
                return;
            }


        bool okForX = false;
        bool okForY = false;
        double x = tokens[0].toDouble(&okForX);
        double y = tokens[1].toDouble(&okForY);

        // Non numerical values are prevented.
        if ((not okForX) or (not okForY))
            {
            int r = QMessageBox::critical(this, tr("Error"),
                    tr("Invalid data format"), QMessageBox::Close);

            mFileEdit->setText("");

            if (r == QMessageBox::Close)
                return;
            }

        if (x <= 0.0)
            continue;

        if (y <= 0.0)
            continue;

        xs.push_back(x);
        ys.push_back(y);
        }

    qDebug() << "Arrive here.";

    mIastModelerPlot->setDataPoints(xs, ys);

    qDebug() << "Fit module begins";
    this->fitModel(mModelCombo->currentText());

    // ? Should be fixed.
    //mXLogScaleCheckBox->setCheckState(Qt::Unchecked);
    //mYLogScaleCheckBox->setCheckState(Qt::Unchecked);
    }

void
IastModelerWidget::fitModel(const QString& text)
    {
    qDebug() << "Enter fit model";

    std::string path = mFileEdit->text().toStdString();

    if (path.empty())
        {
        int r = QMessageBox::critical(this, tr("Error"),
                tr("Please select data file path."), QMessageBox::Close);

        if (r == QMessageBox::Close)
            return;
        }

    IsothermModeler modeler;

    qDebug() << "Get param map from widget";
    IsothermModeler::ParameterMap paramMap;

    try {
        paramMap = mModelerOptionDialog->getParameterMap();
        }
    catch (const IastGuiException& e)
        {
        qDebug() << e.what();
        return;
        }

    try {
        for (auto& param : paramMap)
            {
            modeler.setParameter(param.first, param.second);
            }
        }
    catch (std::exception& e)
        {
        qDebug() << e.what();
        }

    std::shared_ptr<Isotherm> iso;

    qDebug() << "Read data from file";
    std::vector<double> x, y;

    try {
        mLastR2 = 1.0;

        ::readTwoColumns(path, x, y);

        qDebug() << "Fit iso";
        if (text == "Auto")
            {
            double rsq = 0.9999;
            if (mExceptInterpolator->checkState() == Qt::Checked)
                rsq = -1.0;

            iso = modeler.autofit(x, y, IsothermModeler::Weight::ONE_OVER_Y, rsq);
            }
        else if (text == "Interpolator")
            {
            IsothermFactory factory;
            iso = factory.create("interpolator", {x, y});
            }

        for (auto& info : mIsoInfo)
            {
            if (info.fullName() == "Interpolator")
                break;

            if (text == info.fullName().c_str())
                {
                iso = modeler.fit(info.shortName(), x, y);
                break;
                }
            }

        mLastR2 = modeler.getRSquare();
        }
    catch(std::exception& e)
        {
        qDebug() << e.what();
        return;
        }

    qDebug() << "Plot iso";

    int count = mIastModelerPlot->count();

    IastModelerPlot::Points xs, ys;

    for (int i = 0; i < count - 1; ++i)
        {
        double x0 = mIastModelerPlot->x(i);
        double x1 = mIastModelerPlot->x(i + 1);
        double dx = (x1 - x0) / 20.0;

        try {
            for (double x = x0; x <= x1; x +=dx)
                {
                double y = iso->loading(x);
                xs.push_back(x);
                ys.push_back(y);
                }
            }
        catch (...)
            {
            // Just neglect
            }
        }

    {
    double x0 = mIastModelerPlot->x(count - 1);
    double x1 = 1.1 * x0;
    double dx = (x1 - x0) / 20.0;

    try {
        for (double x = x0; x <= x1; x +=dx)
            {
            double y = iso->loading(x);
            xs.push_back(x);
            ys.push_back(y);
            }
        }
    catch (...)
        {
        // Just neglect
        }
    }

    qDebug() << "Set axis setting";
    // Find and then save data range.

    mIastModelerPlot->setModelPoints(xs, ys);

    QString resultText;
    resultText += "# ==============================================================\n";
    resultText += "# Fitting Result: R^2 = " + QString::number(mLastR2) + "\n";
    resultText += iso->getInfoString().c_str();

    mOutputText->setPlainText(resultText);
    }

void
IastModelerWidget::saveIsoFile()
    {
    auto text = mOutputText->toPlainText();
    if (text.isEmpty())
        return;

    auto dir = QFileInfo(mFileEdit->text()).absolutePath() + "/";
    auto guess = dir + QFileInfo(mFileEdit->text()).completeBaseName(); // + ".isotherm";

    auto fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                        guess, tr ("Isotherm (*.isotherm)"));

    if (fileName.isEmpty())
        return;

    QFile file (fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Text);

    QTextStream stream (&file);

    //stream << "# R^2 = " << mLastR2 << endl;
    stream << text;
    }

void
IastModelerWidget::findButtonClicked()
    {
    auto filePath = QFileDialog::getOpenFileName(nullptr, tr("Open File"));

    if (filePath.isEmpty())
        return;

    mFileEdit->setText(filePath);

    this->readData();
    }

QWidget*
IastModelerWidget::createInputWidget()
    {
    auto fileLabel = new QLabel (tr("File"));

    mFileEdit = new QLineEdit;
    mFileEdit->setReadOnly(true);

    auto findButton = new QPushButton (tr("Import raw data"));
    findButton->setIcon(QIcon(tr(":/images/plus.png")));
    connect(findButton, SIGNAL(clicked()), this, SLOT(findButtonClicked()));

    auto modelLabel = new QLabel (tr("Model"));

    mModelCombo = new QComboBox;
    mModelCombo->addItem(tr("Auto"));

    for (auto& info : mIsoInfo)
        mModelCombo->addItem(tr(info.fullName().c_str()));

    connect(mModelCombo, SIGNAL(currentIndexChanged(const QString&)),
            this, SLOT(fitModel(const QString&)));

    mModelerOptionDialog = new ModelerOptionDialog (&mIsoInfo, this);
    mModelerOptionDialog->setModal(false);
    qDebug() << __FILE__ << __LINE__ << "Check option modality" << mModelerOptionDialog->isModal();
    auto advancedButton = new QPushButton (tr("More..."));
    advancedButton->setIcon(QIcon(tr(":/images/cogwheel.png")));
    connect(advancedButton, SIGNAL(clicked(bool)),
            mModelerOptionDialog, SLOT(show()));

    connect(mModelerOptionDialog, &ModelerOptionDialog::finished,
            [=](int) {fitModel(mModelCombo->currentText());});

    connect(mModelerOptionDialog, &ModelerOptionDialog::optionChanged,
            [=]() {fitModel(mModelCombo->currentText());});

    mExceptInterpolator = new QCheckBox {tr("Except Interpolator")};
    connect(mExceptInterpolator, &QCheckBox::stateChanged,
            [=]() {fitModel(mModelCombo->currentText());});

    auto rowLayout1 = new QHBoxLayout;
    auto rowLayout2 = new QHBoxLayout;
    auto rowLayout3 = new QHBoxLayout;
    auto rowLayout4 = new QHBoxLayout;
    auto rowLayout5 = new QHBoxLayout;

    rowLayout1->addWidget(fileLabel);
    //rowLayout1->addStretch();

    rowLayout2->addWidget(mFileEdit);
    rowLayout2->addWidget(findButton);

    rowLayout3->addWidget(modelLabel);
    //rowLayout3->addStretch();

    rowLayout4->addWidget(mModelCombo);

    rowLayout5->addWidget(mExceptInterpolator);
    rowLayout5->addStretch();
    rowLayout5->addWidget(advancedButton);

    auto layout = new QVBoxLayout;

    layout->addLayout(rowLayout1);
    layout->addLayout(rowLayout2);
    layout->addLayout(rowLayout3);
    layout->addLayout(rowLayout4);
    layout->addSpacing(20);
    layout->addLayout(rowLayout5);

    auto widget = new QWidget (this);
    widget->setLayout(layout);

    return widget;
    }

QWidget*
IastModelerWidget::createOutputWidget()
    {
    mOutputText = new QTextEdit;
    mOutputText->setReadOnly(true);
    mOutputText->setFont(QFont("consolas", 10));


    auto saveButton = new QPushButton (tr("Export as *.isotherm"));
    saveButton->setIcon(QIcon(tr(":/images/save.png")));
    connect(saveButton, SIGNAL(clicked(bool)),
            this, SLOT(saveIsoFile()));

    auto rowLayout1 = new QHBoxLayout;
    auto rowLayout2 = new QHBoxLayout;

    rowLayout1->addWidget(mOutputText);

    rowLayout2->addStretch();
    rowLayout2->addWidget(saveButton);

    auto layout = new QVBoxLayout;

    layout->addLayout(rowLayout1);
    layout->addLayout(rowLayout2);

    auto widget = new QWidget (this);
    widget->setLayout(layout);
    //widget->setMinimumHeight(500);
    mOutputText->setMinimumWidth(350);

    return widget;
    }

QWidget*
IastModelerWidget::createPlotOptionWidget()
    {
    auto themeLabel = new QLabel (tr("Theme"));
    auto themeCombo = new QComboBox;

    themeCombo->addItem(tr("Light"));
    themeCombo->addItem(tr("BlueCerulean"));
    themeCombo->addItem(tr("Dark"));
    themeCombo->addItem(tr("BrownSand"));
    themeCombo->addItem(tr("BlueNcs"));
    themeCombo->addItem(tr("HighContrast"));
    themeCombo->addItem(tr("BlueIcy"));
    themeCombo->addItem(tr("Qt"));

    connect(themeCombo, SIGNAL(currentIndexChanged(int)),
            this, SLOT(plotThemeChanged(int)));

    auto logScaleLabel = new QLabel (tr("Log Scale"));

    mXLogScaleCheckBox = new QCheckBox (tr("x axis"));
    connect(mXLogScaleCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(xScaleChanged(int)));

    mYLogScaleCheckBox = new QCheckBox (tr("y axis"));
    connect(mYLogScaleCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(yScaleChanged(int)));

    auto rowLayout1 = new QHBoxLayout;
    auto rowLayout2 = new QHBoxLayout;
    auto rowLayout3 = new QHBoxLayout;
    auto rowLayout4 = new QHBoxLayout;

    rowLayout1->addWidget(themeLabel);

    rowLayout2->addWidget(themeCombo);

    rowLayout3->addWidget(logScaleLabel);

    rowLayout4->addWidget(mXLogScaleCheckBox);
    rowLayout4->addWidget(mYLogScaleCheckBox);
    rowLayout4->addStretch();

    auto layout = new QVBoxLayout;

    layout->addLayout(rowLayout1);
    layout->addLayout(rowLayout2);
    layout->addLayout(rowLayout3);
    layout->addLayout(rowLayout4);

    auto widget = new QWidget (this);
    widget->setLayout(layout);

    //themeCombo->setCurrentIndex(1);

    return widget;
    }

void
IastModelerWidget::plotThemeChanged(int index)
    {
    mIastModelerPlot->changeTheme(index);
    }

void
IastModelerWidget::xScaleChanged(int state)
    {
    if (state == Qt::Checked)
        mIastModelerPlot->changeScaleX(IastModelerPlot::Log);
    else
        mIastModelerPlot->changeScaleX(IastModelerPlot::Linear);
    }

void
IastModelerWidget::yScaleChanged(int state)
    {
    if (state == Qt::Checked)
        mIastModelerPlot->changeScaleY(IastModelerPlot::Log);
    else
        mIastModelerPlot->changeScaleY(IastModelerPlot::Linear);
    }

void
IastModelerWidget::setupIsothermInformation()
    {
    mIsoInfo.push_back({"Langmuir", "langmuir", 2, {"q", "k"}, true});
    mIsoInfo.push_back({"Dual-Site Langmuir", "dsl", 4, {"q1", "k1", "q2", "k2"}, true});
    mIsoInfo.push_back({"Langmuir-Freundlich", "lf", 3, {"q", "k", "n"}, true});
    mIsoInfo.push_back({"Dual-Site Langmuir-Freundlich", "dslf", 6, {"q1", "k1", "n1", "q2", "k2", "n2"}, true});
    mIsoInfo.push_back({"BET", "bet", 3, {"q", "k1", "k2"}, true});
    mIsoInfo.push_back({"Quadratic", "quadratic", 3, {"q", "k1", "k2"}, true});
    mIsoInfo.push_back({"Henry", "henry", 1, {"k"}, true});
    mIsoInfo.push_back({"Interpolator", "interpolator", 0, {}, false});
    }
