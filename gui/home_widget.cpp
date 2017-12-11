#include "home_widget.h"
//#include "iast_gui.h"

#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QUrl>
#include <QFont>
#include <QPushButton>
#include <QDesktopServices>

#include <ciso646>

// Logger
#include "iast_gui_logger.h"

HomeWidget::HomeWidget(QWidget *parent) : QWidget(parent)
    {
    this->setup();
    }

void
HomeWidget::setup()
    {
    mWebView = new QWebEngineView;
    mWebView->load(QUrl {"https://sangwon91.github.io/IASTpp/"});

    auto button = new QPushButton {tr("Go to Home")};
    connect(button, SIGNAL(clicked()), this, SLOT(gotoHome()));

    auto button2 = new QPushButton {tr("Open in Browser")};
    connect(button2, SIGNAL(clicked()), this, SLOT(openInBrowser()));

    auto bottomLayout = new QHBoxLayout;
    bottomLayout->addStretch();
    bottomLayout->addWidget(button);
    bottomLayout->addWidget(button2);
    bottomLayout->addStretch();

    auto layout = new QVBoxLayout;
    layout->addWidget(mWebView);
    layout->addLayout(bottomLayout);

    this->setLayout(layout);
    }


void
HomeWidget::gotoHome()
    {
    mWebView->load(QUrl {"https://sangwon91.github.io/IASTpp/"});
    }

void
HomeWidget::openInBrowser()
    {
    bool opened = QDesktopServices::openUrl(mWebView->url());

    if (not opened)
        {
        int r = QMessageBox::critical(nullptr, tr("Error"),
            tr("Url open fails"), QMessageBox::Close);

        if (r == QMessageBox::Close)
            return;
        }
    }
