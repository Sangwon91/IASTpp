#ifndef HOME_WIDGET_H
#define HOME_WIDGET_H

#include <QWidget>
#include <QTextBrowser>
#include <QLabel>

class HomeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HomeWidget(QWidget *parent = 0);

signals:

public slots:

private:
    void setup();
private:
    QTextBrowser* mHomeText;
    QLabel* mHomeLabel;
};

#endif // HOME_WIDGET_H
