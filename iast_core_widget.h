#ifndef IASTCOREWIDGET_H
#define IASTCOREWIDGET_H

#include <QWidget>

class IastCoreWidget : public QWidget
{
    Q_OBJECT
public:
    explicit IastCoreWidget(QWidget *parent = 0);

signals:

public slots:

private:
    void setup();
};

#endif // IASTCOREWIDGET_H
