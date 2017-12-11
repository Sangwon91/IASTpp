#ifndef PLOTTERXEXPRESSION_H
#define PLOTTERXEXPRESSION_H

#include <QWidget>
#include <QLineEdit>
#include <QString>

class PlotterXExpression : public QWidget
{
    Q_OBJECT
public:
    explicit PlotterXExpression(QWidget *parent = 0);
    QString getExpression();
    void setExpression(QString exp);

signals:

public slots:

private:
    void setup();

private:
    QLineEdit* mExpression;
};

#endif // PLOTTERXEXPRESSION_H
