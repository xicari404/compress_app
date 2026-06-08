#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Set application style
    app.setStyle("Fusion");
    
    // Create and show main window
    MainWindow window;
    window.setWindowTitle("Compression Pro - Optimize Your Disk");
    window.show();
    
    return app.exec();
}
