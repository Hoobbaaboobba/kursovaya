#include "mainwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Set application information
    QApplication::setApplicationName("Property Buyer Database");
    QApplication::setApplicationVersion("1.0");
    QApplication::setOrganizationName("Your Organization");
    QApplication::setOrganizationDomain("yourdomain.com");

    // Set default locale
    QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));

    // Create and show main window
    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
} 