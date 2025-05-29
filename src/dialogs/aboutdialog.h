#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = nullptr);

private:
    void setupUI();
    void setupConnections();

    QLabel *titleLabel;
    QLabel *authorLabel;
    QLabel *instituteLabel;
    QLabel *courseLabel;
    QLabel *groupLabel;
    QPushButton *closeButton;
};

#endif // ABOUTDIALOG_H 