#include "HuiRuWorkStation.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QApplication::setStyle("Fusion");

    HuiRuWorkStation window;

    window.show();

    return app.exec();
}
