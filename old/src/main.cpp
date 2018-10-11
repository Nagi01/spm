#include <QSplashScreen>
#include <QPixmap>
#include "qt_toolbox.h"
#include <QtWidgets/QApplication>
#include <QtGui>

#define TR(s) (QString::fromLocal8Bit(s))

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QSplashScreen splash(QPixmap("E:/DEVEL/SurgicalProcessIdentificationSystem/Build/src/img/logo.jpg"));
	splash.show();

	splash.showMessage(QObject::tr("Initialize"));

	for (int i = 0; i < 10000000; i++);
	Qt_toolBox w;
	w.show();
	splash.finish(&w);
	return a.exec();
}
