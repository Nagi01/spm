#ifndef QT_TOOLBOX_H
#define QT_TOOLBOX_H

#include <QtWidgets/QMainWindow>
#include "ui_qt_toolbox.h"
#include <QLabel>
#include <QSplitter>
#include <QDirModel>
#include <QStackedLayout>
#include <QScrollArea>

class Qt_toolBox : public QMainWindow
{
	Q_OBJECT

protected:
	void timerEvent(QTimerEvent *event);

public:
	Qt_toolBox(QWidget *parent = 0);
	~Qt_toolBox();

private:
	Ui::Qt_toolBoxClass ui;
	//グラフィカルを構築したイベント関数
	void createMenuBar();
	void createStatusBar();
	void createClientArea();

	int hhmmTimerID;
	void readImageFile(QString fnm);

	QLabel *hhmmLabel;

	QSplitter *makeLeftArea();
	QWidget *makeRightArea();

	QDirModel *model;
	QLabel *labelPath;
	QLabel *labelFile;
	QLabel *labelSize;
	QLabel *labelDate;
	QLabel *labelType;

	//右ウィジェット
	QWidget *createGridWidget();
	QTabWidget *createTabWidget();

	QStackedLayout *stack;
	QScrollArea *scrollStack;

	QWidget *makeImageTab();
	QWidget *makeVideoTab();
	QWidget *makeEmptyTab();
	
	QLineEdit *editImageFile;
	QLabel *labelImage;
	QComboBox *comboScale;
	QPixmap pixmapOriginal;

private slots:
	//イベント関数
	void windowInfo();
	void dialogFileOpen();
	void dialogFileSave();
	void dialogDirPath();
	//void dialogColor();
	//void dialogFont();
	void dialogMsgWarn();
	void dialogMsgYesNo();
	//void displayStandardPixmap();
	//void checkStandardCursor();
	void closeDialog();
	//左領域のファイルの中身を見るスロット関数
	void diaplayFileInfo(const QModelIndex &index);
	//右領域のボタン押した結果やラジオボタン，プロパティクリアボタンのスロット関数
	void clickButtonToggled();
	//void clickRadioClear();
	void clickPropertyClear();
	void clickDispImage();
	void clickDispPropertyImage();
	void activeScaleCombo(int);
};
#endif // QT_TOOLBOX_H
