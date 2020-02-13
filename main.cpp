#include "mainwidget.h"
#include <QApplication>
#include <QFontDatabase>
int main(int argc, char *argv[])
{

//    QByteArray width=qgetenv("QT_QPA_EGLFS_PHYSICAL_WIDTH");
//    width.setNum(800);
//    QByteArray height=qgetenv("QT_QPA_EGLFS_PHYSICAL_HEIGHT");
//    height.setNum(480);

    QApplication a(argc, argv);
 //   int id = QFontDatabase::addApplicationFont("/usr/local/qt5/lib/fonts/DroidSansFallback.ttf");
    //QString msyh = QFontDatabase::applicationFontFamilies (id).at(0);
//    QFont font(msyh,10);
 //   qDebug()<<msyh<<endl;
 //   font.setPointSize(20);
//    a.setFont(font);
    mainWidget w;
    w.show();
    return a.exec();
}
