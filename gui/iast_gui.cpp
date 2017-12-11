#include "iast_gui.h"

#include <QSplashScreen>
#include <QApplication>
#include <QDesktopWidget>
#include <QPixmap>
#include <QThread>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QIcon>
#include <QLabel>
#include <QRect>
#include <QStatusBar>
#include <QTabWidget>
#include <QMessageBox>
#include <QString>
#include <QStackedLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPixmap>
#include <QFile>
#include <QDebug>
#include <QScrollArea>
#include <QSizePolicy>
#include <QStyleFactory>
#include <QMessageLogger>

#include "home_widget.h"
#include "iast_modeler_widget.h"
#include "iast_core_widget.h"
#include "isotherm_calculator.h"

// Logger
#include "iast_gui_logger.h"

const QString IastGui::ProgramVersion = "1.0.1";

IastGui::IastGui(QWidget* parent)
    : QMainWindow(parent)
    {
    QMessageLogger(__FILE__, __LINE__, 0).debug() << "Pop splash screen";
    this->popSplashScreen();

    this->setToWhiteSkin();

    this->setupStatusBar();
    this->setupMeneBar();
    //this->setupToolBar();

    auto list = new QListWidget;
    //list->resize(list->width(), 100);

    auto homeItem = new QListWidgetItem {tr("Home")};
    homeItem->setFlags(Qt::NoItemFlags | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    homeItem->setIcon(QPixmap {":images/Home_ListLogo.png"});
    list->addItem(homeItem);

    auto modelerItem = new QListWidgetItem ("Modeler");
    modelerItem->setFlags(Qt::NoItemFlags | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    modelerItem->setIcon(QPixmap {":images/Modeler_ListLogo.png"});
    list->addItem(modelerItem);

    auto iastItem = new QListWidgetItem ("IAST");
    iastItem->setFlags(Qt::NoItemFlags | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    iastItem->setIcon(QPixmap {":images/IAST_ListLogo.png"});
    list->addItem(iastItem);

    list->setIconSize(QSize(40, 40));
    list->setViewMode(QListWidget::IconMode);
    list->setSelectionMode(QListWidget::SingleSelection);
    //list->setLayoutMode(QListView::Batched);
    //list->setFlow(QListWidget::TopToBottom);
    //list->setSizePolicy(1);

    auto moduleLayout = new QHBoxLayout;
    moduleLayout->addWidget(list);

    auto moduleGroup = new QGroupBox (tr("Module"));
    moduleGroup->setLayout(moduleLayout);
    moduleGroup->setFixedHeight(105);

    //auto modeler = new IastModelerWidget (this);

    auto home_ = new HomeWidget {this};
    home_->setMinimumHeight(400);
    auto home = new QScrollArea;
    home->setWidget(home_);
    home->setWidgetResizable(true);

    auto modeler_ = new IastModelerWidget (this);
    modeler_->setMinimumHeight(600);

    auto modeler = new QScrollArea;
    modeler->setWidget(modeler_);
    modeler->setWidgetResizable(true);

    auto iast_ = new IastCoreWidget;
    iast_->setMinimumHeight(700);
    auto iast = new QScrollArea;
    iast->setWidget(iast_);
    iast->setWidgetResizable(true);

    auto stacked = new QStackedLayout;
    stacked->addWidget(home);
    stacked->addWidget(modeler);
    stacked->addWidget(iast);

    connect(list, SIGNAL(currentRowChanged(int)),
            stacked, SLOT(setCurrentIndex(int)));

    //auto layout = new QHBoxLayout;
    auto layout = new QVBoxLayout;
    //layout->addStretch();
    layout->addWidget(moduleGroup);
    layout->addLayout(stacked);

    auto widget = new QWidget;
    //auto widget = new QScrollArea;
    //widget->setWidgetResizable(true);
    //widget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    //widget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    widget->setLayout(layout);

    this->setCentralWidget(widget);

    // Set the size of main window
    QRect screenSize = QDesktopWidget ().availableGeometry(this);

    int height = screenSize.height();
    int width  = screenSize.width();
    int len = width > height * 1.5 * 0.8 ? height * 1.5 * 0.8 : width * 0.8;

    this->setGeometry(QRect ((width - len) * 0.5, height * 0.1,
                             len, height * 0.8));

    //this->setToWhiteSkin();
    //this->setToBlackSkin();

    this->setAttribute(Qt::WA_DeleteOnClose);
    this->show();
    }

IastGui::~IastGui()
    {

    }

void
IastGui::popSplashScreen()
    {
    auto splash = new QSplashScreen (this, QPixmap (":/images/splash.png"));

    splash->showMessage(tr("Setting up the main window...               "),
                        Qt::AlignRight, Qt::white);

    splash->show();
    splash->finish(this);
    }

void
IastGui::setupMeneBar()
    {
    // Setup File Menu


    //Setup Tools menu =============================================================
    auto openIsothermCalculator = new QAction(tr ("Isotherm &Calculator"), this);
    openIsothermCalculator->setStatusTip(tr ("Open Isotherm Calculator"));
    connect(openIsothermCalculator, SIGNAL(triggered(bool)),
            this, SLOT(openIsothermCalculator()));

    auto toolMenu = this->menuBar()->addMenu(tr ("&Tool"));
    toolMenu->addAction(openIsothermCalculator);


    // Setup option menu ===========================================================
    auto whiteSkinAction = new QAction(tr("&White"));
    connect(whiteSkinAction, SIGNAL(triggered(bool)),
            this, SLOT(setToWhiteSkin()));

    auto blackSkinAction = new QAction(tr("&Black"));
    connect(blackSkinAction, SIGNAL(triggered(bool)),
            this, SLOT(setToBlackSkin()));

    auto skinMenu = this->menuBar()->addMenu(tr("&Skin"));
    skinMenu->addAction(whiteSkinAction);
    skinMenu->addAction(blackSkinAction);


    // Setup Help menu =============================================================
    auto aboutQtAction = new QAction(tr("About &IAST++"), this);
    aboutQtAction->setIcon(QIcon (":/images/question.png"));
    aboutQtAction->setStatusTip(tr("Show the IAST++'s About box"));

    connect(aboutQtAction, &QAction::triggered, this, &IastGui::aboutIastPlusPlus);

    auto helpMenu = this->menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutQtAction);
    }

void
IastGui::setupToolBar()
    {
    auto aboutQtAction = new QAction(tr("About &IAST++"), this);
    aboutQtAction->setIcon(QIcon (":/images/question.png"));
    aboutQtAction->setStatusTip(tr("Show the IAST++'s About box"));

    connect(aboutQtAction, &QAction::triggered, this, &IastGui::aboutIastPlusPlus);

    auto helpToolBar = this->addToolBar(tr("&Help"));
    helpToolBar->addAction(aboutQtAction);
    }

void
IastGui::setupStatusBar()
    {
    statusBar()->showMessage(tr("Ready"));
    statusBar()->setSizeGripEnabled(false);
    }

void
IastGui::aboutIastPlusPlus()
    {
    QString caption;
    caption = tr(
        "<h3>IAST++</h3>"
        "<p>Program Version %1.</p>"
        ).arg(IastGui::ProgramVersion);

    QString text;
    text = tr(
        "<p>KAIST & Saudi Aramco</p>"
        "<p>Developer: Sangwon Lee</p> "
        "<p>Homepage: <a href=\"http://%1/\">%1</a> "
        "<p>Homepage: <a href=\"http://%2/\">%2</a> "
    ).arg("sangwon91.github.io/IASTpp/").arg("sites.google.com/site/iastcpp/");

    QMessageBox* msgBox = new QMessageBox (this);
    msgBox->setAttribute(Qt::WA_DeleteOnClose);
    msgBox->setWindowTitle(tr("About IAST++"));
    msgBox->setText(caption);
    msgBox->setInformativeText(text);
    msgBox->setMinimumWidth(1000);

    QPixmap pm (":images/aboutLogo.png");
    if (!pm.isNull())
        msgBox->setIconPixmap(pm);

    msgBox->show();
    }

void
IastGui::setToBlackSkin()
    {
    // Global settings ------------------------------------------------

    // 1. Make new font.
    QFont newFont = QFont("Verdana", 9);
    newFont.setStyleStrategy(QFont::PreferAntialias);

    // 2. Load stylesheet.
    QFile styleFile(":qdarkstyle/style.qss");
    styleFile.open(QFile::ReadOnly);

    qApp->setStyleSheet(styleFile.readAll());
    qApp->setFont(newFont);

    // ----------------------------------------------------------------
    }

void
IastGui::setToWhiteSkin()
    {
    qApp->setStyleSheet("");
    qApp->setStyle(QStyleFactory::create("Fusion"));
    qApp->setFont(QFont("Verdana", 9));
    }

void
IastGui::openIsothermCalculator()
    {
    auto calculator = new IsothermCalculator;
    connect(this, SIGNAL(destroyed(QObject*)),
            calculator, SLOT(close()));

    calculator->show();
    }
