#ifndef COREISOTHERMTABLE_H
#define COREISOTHERMTABLE_H

#include <QWidget>
#include <QPushButton>
#include <QTableWidget>

#include "core_variable_chooser.h"

class CoreIsothermTable : public QWidget
{
    Q_OBJECT
public:
    explicit CoreIsothermTable(QWidget *parent = 0);
    QTableWidget* table();

signals:
    void makeClicked(int rowCount, int extensive, int intensive);
    void afterUp();
    void afterDown();

public slots:
    void addClicked();
    void deleteClicked();
    void upClicked();
    void downClicked();

private:
    void setup();

private:
    QTableWidget* mIsothermTable;
    QPushButton* mAddButton;
    QPushButton* mDeleteButton;
    QPushButton* mUpButton;
    QPushButton* mDownButton;
    QPushButton* mUpdateButton;
    CoreVariableChooser* mVariableChooser;
    QPushButton* mMakeButton;
};

#endif // COREISOTHERMTABLE_H
