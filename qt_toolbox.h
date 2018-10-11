#ifndef QT_TOOLBOX_H
#define QT_TOOLBOX_H

#include <QtWidgets/QMainWindow>
#include "ui_qt_toolbox.h"
#include <QLabel>
#include <QSplitter>
#include <QDirModel>
#include <QStackedLayout>
#include <QScrollArea>
#include <QCHeckBox>
#include <QSlider>
#include <QPushButton>
#include <QMultiMedia>
#include <QtMultimediaWidgets>


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
	//�O���t�B�J�����\�z�����C�x���g�֐�
	void createMenuBar();
	void createStatusBar();
	void createClientArea();

	int hhmmTimerID;
	void readImageFile(QString fnm);

	QLabel *hhmmLabel;

	QSplitter *makeLeftArea();
	//QWidget *makeRightArea();

	QDirModel *model;
	QLabel *labelPath;
	QLabel *labelFile;
	QLabel *labelSize;
	QLabel *labelDate;
	QLabel *labelType;

	//�E�E�B�W�F�b�g
	//QWidget *createGridWidget();
	QTabWidget *createTabWidget();

	QStackedLayout *stack;
	QScrollArea *scrollStack;

	QWidget *makeImageTab();
	QWidget *makeVideoTab();
	QWidget *makeEmptyTab();
	QWidget *makeDetectionTab();
	QWidget *makePositionTab();
	//QWidget *makeIdentificationTab();
	
	QLineEdit *editImageFile;
	QLabel *labelImage;
	QComboBox *comboScale;
	QPixmap pixmapOriginal;
	QLineEdit *editMRIFile;
	QLineEdit *editLogFile;
	QLineEdit *editSulciFile;
	QCheckBox *check_slicer;

	//�p��o
	QLineEdit *editSurgicalVideoFile;

	void setMediaFile(QString fnm);

	QLineEdit *editMediaFile;
	QComboBox *comboRate;
	QSlider *sliderTime;
	QLabel *labelPlayTime;
	QPushButton *buttonStop;
	QPushButton *buttonPlay;
	QSlider *sliderVolume;
	QToolButton *buttonMute;
	QMediaPlayer *player;
	QVideoWidget *videoWidget;

private slots:
	//�C�x���g�֐�
	void windowInfo();
	void dialogFileOpen();
	void dialogFileSave();
	void dialogDirPath();
	//void dialogColor();
	//void dialogFont();
	void dialogMsgWarn();
	void dialogMsgOK();
	void dialogMsgYesNo();
	//void displayStandardPixmap();
	//void checkStandardCursor();
	void closeDialog();
	//���̈�̃t�@�C���̒��g������X���b�g�֐�
	void diaplayFileInfo(const QModelIndex &index);
	//�E�̈�̃{�^�����������ʂ⃉�W�I�{�^���C�v���p�e�B�N���A�{�^���̃X���b�g�֐�
	void clickButtonToggled();
	//void clickRadioClear();
	void clickPropertyClear();
	void clickDispImage();
	void clickDispPropertyImage();
	void activeScaleCombo(int);

	void clickSurgicalDetection();
	void clickPropertySurgicalDetection();

	void clickDispPropertyMRISet();
	void clickDispPropertySulciSet();
	void clickDispPropertyLogSet();
	void clickSurgicalPosition();
	
	void clickPlayMedia();
	void clickPlayPropertyMedia();
	void activeRateCombo(int comboIx);
	void moveTimeSlider(int seconds);
	void clickStopButton();
	void clickLeftButton();
	void clickPlayButton();
	void clickRightButton();
	void moveVolumeSlider(int volume);
	void clickMuteButton();

	void changeDuration(qint64);
	void changePosition(qint64);
	void changeMediaStatus(QMediaPlayer::MediaStatus);
	void changeBufferStatus(int);
	void onError();
	void changeState(QMediaPlayer::State);
	void changeVolume(int);
	void changeMute(bool);
};
#endif // QT_TOOLBOX_H
