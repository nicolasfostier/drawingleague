#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QThread>

// Check if there is an update available on github
#include "include/checkforupdate.h"
// Main window of the program
#include "include/windows/mainwindow.h"



int main(int argc, char *argv[])
{
    // Creation and configuration of the Qt application
    QApplication app(argc, argv);
    app.setApplicationName("Drawing League");
    app.setOrganizationName("Nicolas Fostier");
    app.setOrganizationDomain("nicolasfostier.free.fr");
    app.setApplicationVersion("1.0.1");
    app.setWindowIcon(QIcon(":/images/logo.ico"));

    // Force the app to use the same language as the system
    QString locale = QLocale::system().name().section('_', 0, 0);
    QTranslator translator;
    translator.load(QString("drawingleague_") + locale, ":/lang");
    app.installTranslator(&translator);

    // Register some meta-type
    qRegisterMetaType<Room>();
    qRegisterMetaType<Message>();
    qRegisterMetaType<Player>();
    qRegisterMetaType<DrawingToolType>();

    // Check if there is an update available on github
    // (It's executed on its own thread and will delete himself when its task is over)
    new CheckForUpdate(app.applicationVersion());

    // Create the main window of the program and open it
    MainWindow mainWindow;
    mainWindow.show();

    // Execute the Qt application : enter the event loop
    return app.exec();
}
