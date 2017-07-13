#include <QApplication>
#include "SnakeDlg.h"
#include <QTextCodec>
#include <QTime>
#include <QTranslator>
#include <QLocale>
int main(int argc, char *argv[])
{
    QTextCodec::setCodecForTr (QTextCodec::codecForName ("utf-8"));
    qsrand (QTime::currentTime ().msec ());
    QApplication a(argc, argv);
    QTranslator tr;
    tr.load ("qt_" + QLocale::system ().name (),"/usr/share/qt4/translations");
    a.installTranslator(&tr);
    SnakeDlg w;
    w.show();
    return a.exec();
}
