#include "home_widget.h"
#include "iast_gui.h"

#include <QHBoxLayout>
#include <QFont>

HomeWidget::HomeWidget(QWidget *parent) : QWidget(parent)
    {
    this->setup();
    }

void
HomeWidget::setup()
    {
    QString htmlText = tr(R"^(<h1 id="iast++-%1"><a name="iast++-%1" href="#iast++-%1"></a>IAST++ %1</h1>
<h2 id="software-information"><a name="software-information" href="#software-information"></a>Software Information</h2>
<ul>
<li>Developer: Sangwon Lee (%2)</li>
<li>Official Homepage: <a href="%3">%3</a></li>
<li>Company: <a href="%4">Molecular Simulation Group, KAIST</a></li>
</ul>
<p>Please cite <a href="%5">me</a>, if you find it useful.</p>)^").
    arg(IastGui::ProgramVersion).
    arg("lsw91@kaist.ac.kr").
    arg("https://sites.google.com/site/iastcpp").
    arg("http://molsim.kaist.ac.kr").
    arg("https://sites.google.com/site/iastcpp");

    QFont font {"", 20};
    font.setStyleStrategy(QFont::PreferAntialias);

    /* Case 1
    mHomeLabel = new QLabel;
    mHomeLabel->setOpenExternalLinks(true);
    mHomeLabel->setAlignment(Qt::AlignCenter);
    mHomeLabel->setText(htmlText);    
    mHomeLabel->setFont(font);

    auto layout = new QHBoxLayout;
    layout->addWidget(mHomeLabel);
    */

    // Case 2
    mHomeText = new QTextBrowser;
    mHomeText->setText(htmlText);
    mHomeText->setAlignment(Qt::AlignCenter);
    mHomeText->setFont(font);
    mHomeText->setOpenExternalLinks(true);

    auto layout = new QHBoxLayout;
    layout->addWidget(mHomeText);

    this->setLayout(layout);
    }
