#include "main_window.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a{argc, argv};
    a.setOrganizationName("oan");
    a.setApplicationName("envview");

    EnvView mw;
    mw.show();

    return a.exec();
}

