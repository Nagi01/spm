#include "qt_toolbox.h"
#include <QtWidgets/QApplication>
#include <QtGui>

#define TR(s) (QString::fromLocal8Bit(s))

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Qt_toolBox w;
	w.show();
	return a.exec();
}
