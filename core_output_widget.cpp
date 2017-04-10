#include "core_output_widget.h"

#include <QApplication>
#include <QClipboard>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QTableWidgetSelectionRange>
#include <QKeySequence>
#include <QFileDialog>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

#include <ciso646>

#include "iast_plotter/iast_plotter_widget.h"

CoreOutputWidget::CoreOutputWidget(QWidget *parent) :
    QWidget(parent),
    mPlotter {nullptr}
    {
    this->setup();
    }

QTableWidget*
CoreOutputWidget::table()
    {
    return mTable;
    }

void
CoreOutputWidget::getIsothermTable(QTableWidget* isothermTable)
    {
    mIsothermTable = isothermTable;
    }

void
CoreOutputWidget::getInputTable(QTableWidget* inputTable)
    {
    mInputTable = inputTable;
    }

void
CoreOutputWidget::keyPressEvent(QKeyEvent* event)
    {
    if (event->matches(QKeySequence::Copy))
        this->copy();
    }

void
CoreOutputWidget::cleanUpTable()
    {
    mTable->clear();
    mTable->setRowCount(0);
    mTable->setColumnCount(0);
    }

void
CoreOutputWidget::copy()
    {
    auto ranges = mTable->selectedRanges();
    if (ranges.size() == 0)
        {
        qDebug() << __FILE__ << __LINE__ << "No Copy area";
        return;
        }

    QString str;

    auto range = ranges[0];

    for (int i = 0; i < range.rowCount(); ++i)
        {
        if (i > 0)
            str += "\n";

        for (int j = 0; j < range.columnCount(); ++j)
            {
            if (j > 0)
                str += "\t";

            str += mTable->item(i, j)->text();
            }
        }

    QApplication::clipboard()->setText(str);
    }

void
CoreOutputWidget::exportTable()
    {
    QString str;

    if (mInputTable == nullptr)
        return;

    if (mTable == nullptr)
        return;

    if (mTable->rowCount() <= 0)
        {
        int r = QMessageBox::critical(this, tr("Error"),
            tr("No outputs"), QMessageBox::Close);

        if (r == QMessageBox::Close)
            return;
        }

    if (mTable->rowCount() != mInputTable->rowCount())
        {
        int r = QMessageBox::critical(this, tr("Error"),
            tr("Input rows != Output rows"), QMessageBox::Close);

        if (r == QMessageBox::Close)
            return;
        }

    for (int i = 0; i < mInputTable->columnCount(); ++i)
        str += mInputTable->horizontalHeaderItem(i)->text() + '\t';

    for (int i = 0; i < mTable->columnCount(); ++i)
        str += mTable->horizontalHeaderItem(i)->text() + '\t';

    str[str.length() - 1] = '\n';

    for (int r = 0; r < mTable->rowCount(); ++r)
        {
        for (int c = 0; c < mInputTable->columnCount(); ++c)
            str += mInputTable->item(r, c)->text() + '\t';

        for (int c = 0; c < mTable->columnCount(); ++c)
            str += mTable->item(r, c)->text() + '\t';

        str[str.length() - 1] = '\n';
        }

    str.resize(str.length() - 1);
    //qDebug() << str;

    auto fileName = QFileDialog::getSaveFileName(this, tr("Export File"),
                        QString {}, tr ("txt (*.txt)"));

    if (fileName.isEmpty())
        return;

    if (not fileName.endsWith(".txt"))
        fileName += ".txt";

    QFile file (fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Text);

    QTextStream stream (&file);

    stream << str;
    }

void
CoreOutputWidget::openPlotter()
    {
    if (mTable->columnCount() < 2)
        {
        int r = QMessageBox::critical(this, tr("Error"),
            tr("Click 'Calculate' before plotting"), QMessageBox::Close);

        if (r == QMessageBox::Close)
            return;
        }

    if (mPlotter != nullptr)
        delete mPlotter;

    mPlotter = new IastPlotterWidget {mInputTable, mTable};

    // Get table information.
    //gen->setInputOutputTable(mInputTable, mTable);
    //gen->plot();

    // Memory leak?
    mPlotter->setAttribute(Qt::WA_DeleteOnClose);
    mPlotter->show();

    connect(this, SIGNAL(destroyed(QObject*)),
            mPlotter, SLOT(close()));

    connect(mPlotter, &IastPlotterWidget::destroyed,
            [this](){mPlotter = nullptr;});
    }

void
CoreOutputWidget::setup()
    {
    mTable = new QTableWidget;
    mExportButton = new QPushButton (tr("Export"));
    mExportButton->setStatusTip(tr ("Export IAST results"));
    mExportButton->setIcon(QIcon(tr(":/images/save.png")));

    mPlotButton = new QPushButton (tr("Open Plotter"));
    mPlotButton->setStatusTip(tr("Open output plotting widget"));
    mPlotButton->setIcon(QIcon(tr(":/images/plotter.png")));
    connect(mPlotButton, SIGNAL(clicked(bool)),
            this, SLOT(openPlotter()));

    mCleanButton = new QPushButton (tr("Delete All"));
    mCleanButton->setStatusTip(tr ("Delete all rows"));

    connect(mExportButton, SIGNAL(clicked(bool)),
            this, SLOT(exportTable()));

    connect(mCleanButton, SIGNAL(clicked(bool)),
            this, SLOT(cleanUpTable()));

    auto leftLayout = new QVBoxLayout;

    leftLayout->addWidget(mExportButton);
    leftLayout->addWidget(mPlotButton);
    leftLayout->addStretch();
    leftLayout->addWidget(mCleanButton);

    auto layout = new QHBoxLayout;
    layout->addWidget(mTable);
    layout->addLayout(leftLayout);

    this->setLayout(layout);
    }
