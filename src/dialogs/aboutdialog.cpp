#include "aboutdialog.h"
#include <QApplication>
#include <QFont>

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUI();
    setupConnections();
}

void AboutDialog::setupUI()
{
    setWindowTitle(tr("О программе"));
    setFixedSize(400, 300);

    QVBoxLayout *layout = new QVBoxLayout(this);

    // Title
    titleLabel = new QLabel(tr("Система управления базой данных покупателей недвижимости"), this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    // Author information
    authorLabel = new QLabel(tr("Автор: [Ваше полное имя]"), this);
    authorLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(authorLabel);

    instituteLabel = new QLabel(tr("Институт: [Ваш институт]"), this);
    instituteLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(instituteLabel);

    courseLabel = new QLabel(tr("Курс: [Ваш курс]"), this);
    courseLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(courseLabel);

    groupLabel = new QLabel(tr("Группа: [Ваша группа]"), this);
    groupLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(groupLabel);

    // Version
    QLabel *versionLabel = new QLabel(tr("Версия: 1.0"), this);
    versionLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(versionLabel);

    // Close button
    closeButton = new QPushButton(tr("Закрыть"), this);
    layout->addWidget(closeButton);

    setLayout(layout);
}

void AboutDialog::setupConnections()
{
    connect(closeButton, &QPushButton::clicked, this, &AboutDialog::accept);
} 