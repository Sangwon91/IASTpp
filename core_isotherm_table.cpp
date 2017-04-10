#include "core_isotherm_table.h"

#include <QTableWidgetItem>
#include <QTableWidgetSelectionRange>
#include <QTableView>
#include <QHeaderView>
#include <QModelIndex>
#include <QAbstractItemView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QFileInfo>
#include <QDebug>
#include <QGroupBox>
#include <QFont>
#include <QApplication>

CoreIsothermTable::CoreIsothermTable(QWidget* parent) : QWidget (parent)
    {
    this->setup();
    }

QTableWidget*
CoreIsothermTable::table()
    {
    return mIsothermTable;
    }

void
CoreIsothermTable::addClicked()
    {
    auto filePaths = QFileDialog::getOpenFileNames(nullptr, tr("Open File"),
                        QString (), "Isotherm (*.isotherm)");

    if (filePaths.isEmpty())
        {
        qDebug() << __FILE__ << __LINE__ << "No Selections";
        return;
        }

    for (const auto& path : filePaths)
        {
        auto name = QFileInfo(path).completeBaseName();

        auto item1 = new QTableWidgetItem (path);
        auto item2 = new QTableWidgetItem (name);

        int count = mIsothermTable->rowCount();

        mIsothermTable->insertRow(count);
        mIsothermTable->setItem(count, 0, item1);
        mIsothermTable->setItem(count, 1, item2);
        }

    mIsothermTable->setCurrentCell(mIsothermTable->rowCount() - 1, 0);
    }

void
CoreIsothermTable::deleteClicked()
    {
    if (mIsothermTable->rowCount() < 1)
        return;

    auto ranges = mIsothermTable->selectedRanges();
    if (ranges.length() == 0)
        return;

    auto range = ranges[0];

    int top = range.topRow();
    int bottom = range.bottomRow();

    qDebug() << __FILE__ << __LINE__ << "Top = " << top << "Bottom = " << bottom;

    for (int i = top; i <= bottom; ++i)
        mIsothermTable->removeRow(top);

    mIsothermTable->setCurrentCell(top, 0);
    }

void
CoreIsothermTable::upClicked()
    {
    if (mIsothermTable->rowCount() < 1)
        return;

    auto ranges = mIsothermTable->selectedRanges();
    if (ranges.length() == 0)
        return;

    auto range = ranges[0];

    int top = range.topRow();

    if (top <= 0)
        return;

    int site = top - 1;

    for (int c : {0, 1})
        {
        auto item1 = mIsothermTable->takeItem(top, c);
        auto item2 = mIsothermTable->takeItem(site, c);

        mIsothermTable->setItem(top, c, item2);
        mIsothermTable->setItem(site, c, item1);
        }

    mIsothermTable->setCurrentCell(site, 0);

    emit afterUp();
    }

void
CoreIsothermTable::downClicked()
    {
    if (mIsothermTable->rowCount() < 1)
        return;

    auto ranges = mIsothermTable->selectedRanges();
    if (ranges.length() == 0)
        return;

    auto range = ranges[0];
    int top = range.topRow();

    if (top + 1 == mIsothermTable->rowCount())
        return;

    int site = top + 1;

    for (int c : {0, 1})
        {
        auto item1 = mIsothermTable->takeItem(top, c);
        auto item2 = mIsothermTable->takeItem(site, c);

        mIsothermTable->setItem(top, c, item2);
        mIsothermTable->setItem(site, c, item1);
        }

    mIsothermTable->setCurrentCell(site, 0);

    emit afterDown();
    }

void
CoreIsothermTable::setup()
    {
    mIsothermTable = new QTableWidget;
    mIsothermTable->setColumnCount(2);
    mIsothermTable->setHorizontalHeaderItem(0, new QTableWidgetItem (tr("Path")));
    mIsothermTable->setHorizontalHeaderItem(1, new QTableWidgetItem (tr("Name")));
    mIsothermTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    mIsothermTable->setColumnWidth(0, 400);
    mIsothermTable->horizontalHeader()->setStretchLastSection(true);

    auto makeSignal = [this]()
        {
        emit makeClicked(mIsothermTable->rowCount(),
            mVariableChooser->getExtensive(),
            mVariableChooser->getIntensive());
        };

    //connect(mIsothermTable, &QTableWidget::cellChanged, makeSignal);
    //connect(mIsothermTable, &QTableWidget::itemChanged, makeSignal);

    QFont font = qApp->font();
    font.setBold(true);
    mAddButton = new QPushButton (tr("Import *.isotherm"));
    mAddButton->setIcon(QIcon(tr(":/images/plus.png")));
    mAddButton->setFont(font);
    //mAddButton->setMinimumHeight(20);
    mAddButton->setStatusTip(tr ("Import isotherms"));

    connect(mAddButton, SIGNAL(clicked(bool)), this, SLOT(addClicked()));
    connect(mAddButton, &QPushButton::clicked, makeSignal);

    mDeleteButton = new QPushButton (tr("Delete"));
    mDeleteButton->setIcon(QIcon(tr(":/images/minus.png")));
    //mDeleteButton->setMinimumHeight(20);
    mDeleteButton->setStatusTip(tr ("Delete selected isotherms"));

    connect(mDeleteButton, SIGNAL(clicked(bool)), this, SLOT(deleteClicked()));
    connect(mDeleteButton, &QPushButton::clicked, makeSignal);

    mUpButton = new QPushButton (tr("Up"));
    connect(mUpButton, SIGNAL(clicked(bool)), this, SLOT(upClicked()));
    connect(this, &CoreIsothermTable::afterUp, makeSignal);

    mDownButton = new QPushButton (tr("Down"));
    connect(mDownButton, SIGNAL(clicked(bool)), this, SLOT(downClicked()));
    connect(this, &CoreIsothermTable::afterDown, makeSignal);

    auto updown = new QHBoxLayout;
    updown->addWidget(mUpButton);
    updown->addWidget(mDownButton);

    mVariableChooser = new CoreVariableChooser;
    connect(mVariableChooser, &CoreVariableChooser::changed, makeSignal);

    mUpdateButton = new QPushButton (tr("Update"));
    mUpdateButton->setIcon(QIcon(tr(":/images/reload.png")));
    connect(mUpdateButton, SIGNAL(clicked(bool)), mVariableChooser, SIGNAL(changed()));

    //mMakeButton = new QPushButton (tr("Make Table"));
    //connect(mMakeButton, &QPushButton::clicked, makeSignal);

    auto variableGroup = new QGroupBox (tr("Variable"));
        {
        auto dummyLayout = new QVBoxLayout;
        dummyLayout->addWidget(mVariableChooser);
        //dummyLayout->addWidget(mMakeButton);
        variableGroup->setLayout(dummyLayout);
        }

    auto leftLayout = new QVBoxLayout;
    leftLayout->addWidget(mAddButton);
    leftLayout->addWidget(mDeleteButton);
    leftLayout->addLayout(updown);
    leftLayout->addStretch();
    leftLayout->addWidget(mUpdateButton);
    leftLayout->addWidget(variableGroup);

    auto layout = new QHBoxLayout;
    layout->addWidget(mIsothermTable);
    layout->addLayout(leftLayout);

    this->setLayout(layout);
    }
