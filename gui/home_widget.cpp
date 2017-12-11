#include "home_widget.h"
#include "iast_gui.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QUrl>
#include <QFont>
#include <QPushButton>

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

    auto bottomLayout = new QHBoxLayout;
    bottomLayout->addStretch();
    bottomLayout->addWidget(button);
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
