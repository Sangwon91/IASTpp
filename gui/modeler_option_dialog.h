#ifndef MODELEROPTIONDIALOG_H
#define MODELEROPTIONDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QStackedLayout>
#include <QListWidget>

#include "../iast/isotherm_utility.hpp"
#include "isotherm_information.h"

class ModelerOptionDialog : public QDialog
    {
    Q_OBJECT

public:
    using InfosType = std::vector<IsothermInformation>;
    ModelerOptionDialog(const InfosType*info, QWidget* parent = nullptr);

    IsothermModeler::ParameterMap getParameterMap();

signals:
    void optionChanged();

public slots:
    void resetTable();

private:
    // Isotherm constraints... list for models
    QStackedLayout* mStackedLayout = nullptr;
    QListWidget* mListWidget = nullptr;
    const InfosType* mIsoInfo = nullptr;
    };

#endif // MODELEROPTIONDIALOG_H
