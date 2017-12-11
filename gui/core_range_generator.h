#ifndef CORERANGEGENERATOR_H
#define CORERANGEGENERATOR_H

#include <QWidget>
#include <QPushButton>
#include <QTableWidget>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QList>

class CoreRangeGenerator : public QWidget
{
    Q_OBJECT
public:
    explicit CoreRangeGenerator(QTableWidget* data, QWidget *parent = 0);

signals:
    void generated();

public slots:
    void generate();

private:
    QTableWidget* mData;  // Input table.
    QTableWidget* mTable; // Table in generator which is built at construction.
    QPushButton* mGenerateButton;      // Just append inputs
    QPushButton* mCleanGenerateButton; // Remove present input table and
                                       // regenerate that.
    QPushButton* mCloseButton;         // Close without any actions.

    QList<QCheckBox*> mCheckBoxList;

    void setup();
};

// Simple Helper class
class DoubleSpinBox : public QDoubleSpinBox
    {
    Q_OBJECT

public slots:
    void setState(int v);

public:
    DoubleSpinBox(QWidget* parent = nullptr);
    virtual QString textFromValue(double value) const override;
    };

#endif // CORERANGEGENERATOR_H
