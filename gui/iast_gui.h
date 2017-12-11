#ifndef IAST_GUI_H
#define IAST_GUI_H

#include <QMainWindow>
#include <QString>

class IastGui : public QMainWindow
    {
    Q_OBJECT

public:
    IastGui(QWidget* parent = nullptr);
    ~IastGui();

public slots:
    void aboutIastPlusPlus();
    void setToBlackSkin();
    void setToWhiteSkin();
    void openIsothermCalculator();

private:
    void popSplashScreen();
    void setupMeneBar();
    void setupToolBar();
    void setupStatusBar();

public:
    static const QString ProgramVersion;
    };

#endif // IAST_GUI_H
