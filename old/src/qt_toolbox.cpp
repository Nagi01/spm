#include "qt_toolbox.h"
#include "surgicaldetection.h"
#include "surgicalposition.h"
#include "surgicalidentification.h"

#include <QWidget>
#include <QDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QFont> 
#include <QDateTime>
#include <QTreeView>
#include <QFormLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QGroupBox>
#include <QRadioButton>
#include <QLineEdit>
#include <QPixmap>
#include <QComboBox>
#include <QHeaderView>
#include <QProgressDialog>

#include <QMultiMedia>
#include <QtMultimediaWidgets>

//visualstudioだと自動的にshiftJISにてビルドします．
//Qtは標準的にUTFでのみ日本語の使用が可能となっているので，
//下記のようにQString型の8bitへと変換しています．
#define tr(s) (QString::fromLocal8Bit(s))

Qt_toolBox::Qt_toolBox(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setWindowTitle(tr("Toolbox"));
	setGeometry(50, 40, 800, 800);
	setCursor(Qt::ArrowCursor);
	QIcon icon = QApplication::style()->standardIcon(QStyle::SP_DirHomeIcon);
	setWindowIcon(icon);
	setStyleSheet("QSplitter::handle {background: darkgrey; margin: 1px;}");
	createMenuBar();
	createStatusBar();
	createClientArea();
}

Qt_toolBox::~Qt_toolBox()
{
}

void Qt_toolBox::createMenuBar() 
{
	//1.グロナビにファイルという項目を作成します．
	QIcon iconInf = QApplication::style()->standardIcon(QStyle::SP_MessageBoxInformation);
	QAction *infoAct = new QAction(iconInf, tr("情報表示"), this);
	infoAct->setShortcut(tr("Ctrl+1"));
	infoAct->setStatusTip(tr("ウインドウ情報の表示"));
	connect(infoAct, SIGNAL(triggered()), this, SLOT(windowInfo()));

	QIcon iconExt = QApplication::style()->standardIcon(QStyle::SP_DialogCloseButton);
	QAction *exitAct = new QAction(iconExt, tr("ウインドウを閉じる"), this);
	exitAct->setShortcut(tr("Ctrl+9"));
	exitAct->setStatusTip(tr("アプリケーションの終了"));
	connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

	QMenu *fileMenu = menuBar()->addMenu(tr("ファイル"));
	fileMenu->addAction(infoAct);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAct);
	
	//2.アクションを2つ作り，サブメニューに追加します．
	QAction *msgWarnAct = new QAction(tr("警告メッセージ"), this);
	connect(msgWarnAct, SIGNAL(triggered()), this, SLOT(dialogMsgWarn()));	

	QAction *msgYesNoAct = new QAction(tr("確認メッセージ"), this);
	connect(msgYesNoAct, SIGNAL(triggered()), this, SLOT(dialogMsgYesNo()));

	QMenu *msgMenu = new QMenu(tr("メッセージ"), this);
	msgMenu->addAction(msgWarnAct);
	msgMenu->addAction(msgYesNoAct);
	
	//グロナビにダイアログを作成アクションを5つ
	QAction *openAct = new QAction(tr("ファイルを開く"), this);
	connect(openAct, SIGNAL(triggered()), this, SLOT(dialogFileOpen()));
	QAction *saveAct = new QAction(tr("名前を付けて保存"), this);
	connect(saveAct, SIGNAL(triggered()), this, SLOT(dialogFileSave()));
	QAction *pathAct = new QAction(tr("フォルダ選択"), this);
	connect(pathAct, SIGNAL(triggered()), this, SLOT(dialogDirPath()));
	//QAction *colorAct = new QAction(tr("色選択"), this);
	//connect(colorAct, SIGNAL(triggered()), this, SLOT(dialogColor()));
	//QAction *fontAct = new QAction(tr("フォント選択"), this);
	//connect(fontAct, SIGNAL(triggered()), this, SLOT(dialogFont()));

	QMenu *dialogMenu = menuBar()->addMenu(tr("ダイアログ"));
	dialogMenu->addAction(openAct);
	dialogMenu->addAction(saveAct);
	dialogMenu->addAction(pathAct);
	//dialogMenu->addAction(colorAct);
	//dialogMenu->addAction(fontAct);
	dialogMenu->addSeparator();
	dialogMenu->addMenu(msgMenu);

	//4.おまけpixmap
	//QAction *pixmapAct = new QAction(tr("標準Pixmap一覧"), this);
	//connect(pixmapAct, SIGNAL(triggered()), this, SLOT(displayStandardPixmap()));
	//QAction *cursorAct = new QAction(tr("標準カーソル"), this);
	//connect(cursorAct, SIGNAL(triggered()), this, SLOT(checkStandardCursor()));

	//QMenu *refMenu = menuBar()->addMenu(tr("参考"));
	//refMenu->addAction(pixmapAct);
	//refMenu->addAction(cursorAct);

	//5.ツールバーにメニュー項目を追加
	QToolBar *fileToolBar = addToolBar(tr("File"));
	fileToolBar->addAction(infoAct);
	fileToolBar->addAction(exitAct);

	//QToolBar *dialogToolBar = addToolBar(tr("Dialog"));
	//dialogToolBar->addAction(colorAct);
	//dialogToolBar->addAction(fontAct);
}

void Qt_toolBox::windowInfo()
{
	/*
	QMessageBox msgBox;
	msgBox.setText("The document has been modified.");
	msgBox.exec();
	*/
	
	QString tmp = "Others";
	switch (windowState())
	{
	case Qt::WindowNoState: tmp = "Normal"; break;
	case Qt::WindowMinimized: tmp = "Icon"; break;
	case Qt::WindowFullScreen: tmp = "Full"; break;
	}
	QString str = QString("位置: (%1, %2)\n サイズ%3%4\n状態: %5").arg(this->x()).arg(this->y()).arg(this->width()).arg(this->height()).arg(tmp);
	QMessageBox::information(this, tr("情報表示"), str);
}

void Qt_toolBox::dialogFileOpen()
{
	QString strFileName = QFileDialog::getOpenFileName(
		this, tr("ファイルを開く"), "C:/DEVEL", tr("テキスト(*.txt);;すべて(*.*)")
		);
	if (!strFileName.isEmpty()) {} 
}

void Qt_toolBox::dialogFileSave()
{
	QString strFileName = QFileDialog::getSaveFileName(
		this, tr("名前を付けて保存"), ".", tr("画像(*.png *.gif *.jpg);;すべて(*.*)")
		);
	if (!strFileName.isEmpty()) {}
}

void Qt_toolBox::dialogDirPath()
{
	QFileDialog::Options options = QFileDialog::DontResolveSymlinks
		| QFileDialog::ShowDirsOnly | QFileDialog::HideNameFilterDetails;

	QString strDir = QFileDialog::getExistingDirectory(
		this, tr("フォルダ選択"), tr("E:/"), options
		);
	if (!strDir.isEmpty()) {} 
}

void Qt_toolBox::dialogMsgWarn()
{
	QMessageBox::warning(this, tr("警告"), tr("問題が発生したのだ"));
}

void Qt_toolBox::dialogMsgOK()
{
	QMessageBox::warning(this, tr("終了"), tr("処理が終わったのだ"));
}


void Qt_toolBox::dialogMsgYesNo()
{
	//mboxという変数を作成
	QMessageBox mbox(this);
	mbox.setIcon(QMessageBox::Question);
	mbox.setWindowTitle(tr("確認メッセージタイトル"));
	mbox.setText(tr("コメントなのだー"));
	mbox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	mbox.setDefaultButton(QMessageBox::No);
	mbox.setButtonText(QMessageBox::Yes, tr("おｋなのだ"));
	mbox.setButtonText(QMessageBox::No, tr("だめなのだ"));

	//入力した結果を情報アイコンのメッセージボックスで表示
	int iRet = mbox.exec();
	QString sRet = (iRet == QMessageBox::Yes ? "おｋなのだ" : "だめなのだ");
	QMessageBox::information(this, tr("押した結果"), sRet);
}

void Qt_toolBox::closeDialog()
{
	QWidget *win = static_cast<QWidget *>(sender())->window();
	if (win->inherits("QDialog")){
		QDialog *dialog = static_cast<QDialog *>(win);
		dialog->done(0);
	}
}

void Qt_toolBox::createStatusBar()
{
	statusBar()->showMessage(tr("手術工程同定解析システム"));
	
	QLabel *label1 = new QLabel;
	label1->setFixedSize(200, 20);
	label1->setText(tr("時刻:"));
	label1->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

	QLabel *label2 = new QLabel;
	label2->setFixedSize(100, 20);
	label2->setText(tr("00:00"));
	label2->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	label2->setStyleSheet("color: white; background: brown;");

	//QPixmap pixmap = QPixmap("C:/img/logo.jpg");
	QIcon icon = QIcon("E:/DEVEL/SurgicalProcessIdentificationSystem/Build/src/img/logo.jpg");
	QLabel *logo_label = new QLabel;
	logo_label->setFixedSize(100, 100);
	logo_label->setScaledContents(true);
	//logo_label->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	//logo_label->setStyleSheet("background:violet;");
	logo_label->setPixmap(icon.pixmap(icon.availableSizes().last()));
	
	//QLabel *label3 = new QLabel;
	//label3->setScaledContents(true);
	//QIcon icon = QApplication::style()->standardIcon(QStyle::SP_TitleBarMenuButton);
	//label3->setPixmap(icon.pixmap(icon.availableSizes().last()));
	statusBar()->addPermanentWidget(label1);
	statusBar()->addPermanentWidget(label2);
	statusBar()->addPermanentWidget(logo_label);

	hhmmLabel = label2;
	hhmmTimerID = startTimer(1000);
}

void Qt_toolBox::timerEvent(QTimerEvent *event)
{
	if (event->timerId() == hhmmTimerID) {
		QDateTime nowTime = QDateTime().currentDateTime();
		QString str = nowTime.toString("hh:mm");
		if (str != hhmmLabel->text()) {
			hhmmLabel->setText(str);
		}
	}
}

void Qt_toolBox::createClientArea()
{
	QSplitter *vsplit_Left = makeLeftArea();
	QTabWidget *vsplit_Right = createTabWidget();

	QSplitter *hsplit = new QSplitter();
	hsplit->setOrientation(Qt::Horizontal);
	hsplit->addWidget(vsplit_Left);
	hsplit->addWidget(vsplit_Right);

	setCentralWidget(hsplit);
}

QSplitter* Qt_toolBox::makeLeftArea()
{
	model = new QDirModel;
	QTreeView *tree = new QTreeView;
	tree->setModel(model);
	tree->header()->resizeSection(0, 240);//240：横
	connect(tree, SIGNAL(clicked(const QModelIndex &)),
		this, SLOT(diaplayFileInfo(const QModelIndex &)));

	labelPath = new QLabel;
	labelFile = new QLabel;
	labelSize = new QLabel;
	labelDate = new QLabel;
	labelType = new QLabel;

	QFormLayout *form = new QFormLayout();
	form->setLabelAlignment(Qt::AlignRight);
	form->addRow(tr(""), new QLabel(tr("プロパティ表示")));
	form->addRow(tr("パス："), labelPath);
	form->addRow(tr("ファイル："), labelFile);
	form->addRow(tr("サイズ："), labelSize);
	form->addRow(tr("更新日："), labelDate);
	form->addRow(tr("タイプ："), labelType);
	
	QWidget *widget = new QWidget;
	widget->setMinimumSize(260, 200);
	widget->setLayout(form);
	widget->setStyleSheet("background:white");

	QScrollArea *scroll = new QScrollArea;
	scroll->setMaximumHeight(250);
	scroll->setWidget(widget);

	QSplitter *vsplit = new QSplitter();
	vsplit->setOrientation(Qt::Vertical);
	vsplit->addWidget(tree);
	vsplit->addWidget(scroll);
	return vsplit;
}

//ファイルツリーをクリックしたときにファイルの情報を表示する
void Qt_toolBox::diaplayFileInfo(const QModelIndex &index)
{
	QFileInfo info = model->fileInfo(index);
	labelPath->setText(info.absolutePath());
	labelFile->setText(info.fileName());

	QLocale loc = QLocale::system();
	labelSize->setText(loc.toString(info.size()) + " byte");
	labelDate->setText((info.lastModified()).toString("yyyy/MM/dd  hh:mm:ss"));

	QString type = "";
	if (info.isFile()) type += tr("ファイル ");
	if (info.isDir()) { type += tr("フォルダ  "); labelSize->setText(""); }
	if (info.isHidden()) type += tr("Hidden ");
	if (info.isReadable()) type += tr("R");
	if (info.isWritable()) type += tr("W");
	if (info.isExecutable()) type += tr("X");
	labelType->setText(type);
}

//ボタンが押されたら色が変わる
void Qt_toolBox::clickButtonToggled()
{
	QPushButton *button = static_cast<QPushButton *>(sender());
	if (button->isChecked()) {
		stack->setCurrentIndex(1);
		button->setStyleSheet("background:#404040; color:white; border:2px inset gray");
	}
	else {
		stack->setCurrentIndex(0);
		button->setStyleSheet("background:#fbffe2; color:black; border:2px outset gray");
	}
}

//ボタンを押したらプロパティエリアをクリアにする
void Qt_toolBox::clickPropertyClear()
{
	labelPath->setText("");
	labelFile->setText("");
	labelSize->setText("");
	labelDate->setText("");
	labelType->setText("");
}

/*
QWidget *Qt_toolBox::createGridWidget()
{
	QLabel *labelTitle = new QLabel(tr("サンプル表示（イベントなし）"));
	labelTitle->setFixedHeight(24);
	labelTitle->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	labelTitle->setStyleSheet("background:#00aaff; color:white;");

	QLabel *labelEdit = new QLabel(tr("テキスト入力"));
	labelEdit->setMaximumWidth(110);

	QGridLayout *grid = new QGridLayout;
	grid->addWidget(labelTitle, 0, 0, 1, 3);
	grid->addWidget(labelEdit, 1, 0);
	
	QWidget *widgetGrid = new QWidget;
	widgetGrid->setLayout(grid);
	widgetGrid->setStyleSheet("background:#e0e0e0;");

	return widgetGrid;
}
*/

//右エリアタブの生成
QTabWidget *Qt_toolBox::createTabWidget()
{
	/*
	QWidget *widgetGroup = makeGroupTab();
	QWidget *widgetText = makeTextTab();
	
	QWidget *widgetCalendar = makeCalendarTab();
	*/
	QWidget *widgetImage = makeImageTab();
	QWidget *widgetVideo = makeVideoTab();
	QWidget *widgetEmpty = makeEmptyTab();
	QWidget *widgetSurgicalDetection = makeDetectionTab();
	QWidget *widgetSurgicalPosition = makePositionTab();
	//QWidget *widgetSurgicalIdentification = makeIdentificationTab();

	
	QTabWidget *tab = new QTabWidget;
	/*
	tab->addTab(widgetGroup, "グループ");
	tab->addTab(widgetText, "文書");
	
	tab->addTab(widgetPlayer, "動画");
	tab->addTab(widgetCalendar, "暦");
	*/
	tab->addTab(widgetImage, tr( "画像の表示"));
	tab->addTab(widgetEmpty, tr("テスト画面"));
	tab->addTab(widgetSurgicalPosition, tr("術具位置"));
	tab->addTab(widgetVideo, tr("動画の表示"));
	tab->addTab(widgetSurgicalDetection, tr("術具検出"));
	
	//tab->addTab(widgetSurgicalIdentification, tr("手術工程同定"));
	return tab;
}

//最小構成

QWidget *Qt_toolBox::makeEmptyTab()
{
	QLabel *label = new QLabel;
	label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	label->setText(tr("テスト用"));

	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addWidget(label);

	QWidget *widget = new QWidget;
	widget->setLayout(vbox);

	return(widget);
}

/////////////////////////////////////////////////////////
//画像表示機能部分
/////////////////////////////////////////////////////////
QWidget *Qt_toolBox::makeImageTab()
{
	QLabel *label = new QLabel(tr("ファイル名"));
	editImageFile = new QLineEdit;

	QPushButton *button1 = new QPushButton(tr("表示"));
	connect(button1, SIGNAL(clicked()), this, SLOT(clickDispImage()));

	QHBoxLayout *hbox = new QHBoxLayout;
	hbox->addWidget(label);
	hbox->addWidget(editImageFile);
	hbox->addWidget(button1);

	QString str = tr("プロパティの画像ファイル表示（PNG/JPG/GIF)");
	QPushButton *button2 = new QPushButton(str);
	connect(button2, SIGNAL(clicked()), this, SLOT(clickDispPropertyImage()));

	comboScale = new QComboBox;
	comboScale->setFixedWidth(90);
	comboScale->addItem(tr("伸縮なし"), QVariant(1));
	comboScale->addItem(tr("横基準"), QVariant(2));
	comboScale->addItem(tr("縦基準"), QVariant(3));
	comboScale->addItem(tr("横縦伸縮"), QVariant(4));
	comboScale->setCurrentIndex(0);
	connect(comboScale, SIGNAL(activated(int)), SLOT(activeScaleCombo(int)));

	QHBoxLayout *hbox2 = new QHBoxLayout;
	hbox2->addWidget(button2);
	hbox2->addWidget(comboScale);

	labelImage = new QLabel;
	labelImage->setScaledContents(false);
	labelImage->setAlignment(Qt::AlignLeft | Qt::AlignTop);
	pixmapOriginal = QApplication::style()->standardPixmap(QStyle::SP_TitleBarMenuButton);
	labelImage->setPixmap(pixmapOriginal);

	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addLayout(hbox);
	vbox->addLayout(hbox2);
	vbox->addWidget(labelImage);

	QWidget *widget = new QWidget;
	widget->setLayout(vbox);

	return(widget);
}

void Qt_toolBox::clickDispImage()
{
	QString fnm = editImageFile->text();
	readImageFile(fnm);
	activeScaleCombo(-1);
}
//プロパティ側からファイルを開く
void Qt_toolBox::clickDispPropertyImage()
{
	QString fnm = labelPath->text() + "/" + labelFile->text();
	editImageFile->setText(fnm);
	readImageFile(fnm);
	activeScaleCombo(-1);
}

void Qt_toolBox::readImageFile(QString fnm)
{
	QFileInfo fileInfo(fnm);
	QString suffix = "PNG/JPG/GIF";
	QString sfx = (fileInfo.suffix()).toUpper();

	if (suffix.indexOf(sfx) >= 0) {
		if (fileInfo.exists() && fileInfo.isFile()) {
			pixmapOriginal = QPixmap(fnm);
			return;
		}
	}
	pixmapOriginal = QApplication::style()->standardPixmap(QStyle::SP_TitleBarMenuButton);
}

void Qt_toolBox::activeScaleCombo(int)
{
	int comboVal = comboScale->itemData(comboScale->currentIndex()).toInt();

	if (comboVal == 1) {
		QPixmap pixmap = pixmapOriginal;
		int wd = 500;
		int ht = 350;
		labelImage->setPixmap(pixmap.scaled(wd, ht));
	}
	else {
		QPixmap pixmap = pixmapOriginal;
		int wd = 300;
		int ht = 250;

		switch (comboVal) {
		case 2: labelImage->setPixmap(pixmap.scaledToWidth(wd));  break;
		case 3: labelImage->setPixmap(pixmap.scaledToHeight(ht)); break;
		case 4: labelImage->setPixmap(pixmap.scaled(wd, ht));     break;
		}
	}
}

/////////////////////////////////////////////////////////
//術具位置検出機能部分
/////////////////////////////////////////////////////////
//術具位置検出のタブの中身
QWidget *Qt_toolBox::makePositionTab()
{
	int label[3] = { 1,2,3 };
	QLabel *label_mri = new QLabel(tr("ファイル：MRI"));
	QLabel *label_log = new QLabel(tr("ファイル：術具ログ"));
	QLabel *label_sulci = new QLabel(tr("ファイル:脳溝画像"));
	editMRIFile = new QLineEdit;
	editLogFile = new QLineEdit;
	editSulciFile = new QLineEdit;
	QPushButton *button_mri = new QPushButton(tr("MRIの選択"));
	QPushButton *button_log = new QPushButton(tr("術具ログの選択"));
	QPushButton *button_sulci = new QPushButton(tr("脳溝画像の選択"));
	connect(button_mri, SIGNAL(clicked()), this, SLOT(clickDispPropertyMRISet()));
	connect(button_log, SIGNAL(clicked()), this, SLOT(clickDispPropertyLogSet()));
	connect(button_sulci, SIGNAL(clicked()),this, SLOT(clickDispPropertySulciSet()));

	QHBoxLayout *hbox_mri = new QHBoxLayout;
	hbox_mri->addWidget(label_mri);
	hbox_mri->addWidget(editMRIFile);
	hbox_mri->addWidget(button_mri);

	QHBoxLayout *hbox_log = new QHBoxLayout;
	hbox_log->addWidget(label_log);
	hbox_log->addWidget(editLogFile);
	hbox_log->addWidget(button_log);

	QHBoxLayout *hbox_sulci = new QHBoxLayout;
	hbox_sulci->addWidget(label_sulci);
	hbox_sulci->addWidget(editSulciFile);
	hbox_sulci->addWidget(button_sulci);

	check_slicer = new QCheckBox("Slicer", this);
	QPushButton *button_start = new QPushButton(tr("実行"));
	connect(button_start, SIGNAL(clicked()), this, SLOT(clickSurgicalPosition()));

	QHBoxLayout *hbox_start = new QHBoxLayout;
	hbox_start->addWidget(check_slicer);
	hbox_start->addWidget(button_start);

	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addLayout(hbox_mri);
	vbox->addLayout(hbox_log);
	vbox->addLayout(hbox_sulci);
	vbox->addLayout(hbox_start);
	
	QWidget *widget = new QWidget;
	widget->setLayout(vbox);

	return(widget);
}
//MRI画像をセット（術具位置検出slot）
void Qt_toolBox::clickDispPropertyMRISet()
{
	QString fnm = labelPath->text() + "/" + labelFile->text();
	editMRIFile->setText(fnm);
}
//log情報をセット（術具位置検出slot）
void Qt_toolBox::clickDispPropertyLogSet()
{
	QString fnm = labelPath->text() + "/" + labelFile->text();
	editLogFile->setText(fnm);
}
//sulci情報をセット（術具位置検出slot）
void Qt_toolBox::clickDispPropertySulciSet()
{
	QString fnm = labelPath->text() + "/" + labelFile->text();
	editSulciFile->setText(fnm);
}
//術具検出の詳細プログラムを起動させるスロット（術具位置検出slot）
void Qt_toolBox::clickSurgicalPosition()
{
	QString fnm_mri = editMRIFile->text();
	string fn_mri = fnm_mri.toStdString();
	QString fnm_log = editLogFile->text();
	string fn_log = fnm_log.toStdString();
	QString fnm_sulci = editSulciFile->text();
	string fn_sulci = fnm_sulci.toStdString();
	bool slicer_check = false;

	if (check_slicer->checkState() == Qt::Checked) {
		slicer_check = true;
	}
	SurgicalPosition surgicalposition;
	surgicalposition.Detect_Position(fn_mri, fn_log, fn_sulci, slicer_check);
	dialogMsgOK();
	//cout << "hoge" << endl;
}
///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
//術具種類検出機能部分
/////////////////////////////////////////////////////////
//術具種類検出のタブの中身
QWidget *Qt_toolBox::makeDetectionTab()
{
	QLabel *label = new QLabel(tr("ファイル名"));
	editSurgicalVideoFile = new QLineEdit;
	QPushButton *button_DetectionStart = new QPushButton(tr("実行"));
	connect(button_DetectionStart, SIGNAL(clicked()), this, SLOT(clickSurgicalDetection()));

	QHBoxLayout *hbox = new QHBoxLayout;
	hbox->addWidget(label);
	hbox->addWidget(editSurgicalVideoFile);
	hbox->addWidget(button_DetectionStart);

	QString str = tr("プロパティの動画ファイルを実行");
	QPushButton *button_DetectionStart2 = new QPushButton(str);
	connect(button_DetectionStart2, SIGNAL(clicked()), this, SLOT(clickPropertySurgicalDetection()));

	//QProgressBar *progress = new QProgressBar;
	//progress->setMinimum(0);
	//progress->setMaximum(100);
	//progress->setValue(0);

	//QLabel *label_progress = new QLabel(tr("動画処理経過[実行前]"));
	QHBoxLayout *hbox2 = new QHBoxLayout;
	hbox2->addWidget(button_DetectionStart2);
	//hbox2->addWidget(label_progress);
	//hbox2->addWidget(progress);


	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addLayout(hbox);
	vbox->addLayout(hbox2);

	QWidget *widget = new QWidget;
	widget->setLayout(vbox);

	return(widget);
}

void Qt_toolBox::clickSurgicalDetection()
{}

void Qt_toolBox::clickPropertySurgicalDetection()
{}

////手術工程同定部分
//QWidget *Qt_toolBox::makeIdentificationTab()
//{
//	QWidget *widget = new QWidget;
//	return widget;
//}

/////////////////////////////////////////////////////////
QWidget *Qt_toolBox::makeVideoTab()
{
	QLabel *label = new QLabel;
	label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	label->setText(tr("テスト用"));

	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addWidget(label);

	QWidget *widget = new QWidget;
	widget->setLayout(vbox);

	return(widget);
}


//
//QWidget *Qt_toolBox::makeVideoTab()
//{
//	QLabel *label = new QLabel(tr("ファイル名"));
//	editMediaFile = new QLineEdit;
//
//	QPushButton *button1 = new QPushButton(tr("読み込み"));
//	connect(button1, SIGNAL(clicked()), this, SLOT(clickPlayMedia()));
//
//	QHBoxLayout *hbox1 = new QHBoxLayout;
//	hbox1->addWidget(label);
//	hbox1->addWidget(editMediaFile);
//	hbox1->addWidget(button1);
//
//	QPushButton *button2 = new QPushButton(tr("プロパティの動画／音声再生（wmv/avi...)"));
//	connect(button2, SIGNAL(clicked()), this, SLOT(clickPlayPropertyMedia()));
//
//	comboRate = new QComboBox(this);
//	comboRate->setFixedWidth(60);
//	comboRate->addItem("0.5x", QVariant(0.5));
//	comboRate->addItem("1.0x", QVariant(1.0));
//	comboRate->addItem("2.0x", QVariant(2.0));
//	comboRate->setCurrentIndex(1);
//	connect(comboRate, SIGNAL(activated(int)), SLOT(activeRateCombo(int)));
//
//	QHBoxLayout *hbox2 = new QHBoxLayout;
//	hbox2->addWidget(button2);
//	hbox2->addWidget(comboRate);
//
//	videoWidget = new QVideoWidget;
//	videoWidget->setStyleSheet("background:black;");
//
//	QVBoxLayout *vboxDummy = new QVBoxLayout;
//	vboxDummy->addWidget(videoWidget);
//
//	QWidget *widgetDummy = new QLabel;
//	widgetDummy->setStyleSheet("background:black;");
//	widgetDummy->setLayout(vboxDummy);
//
//	sliderTime = new QSlider(Qt::Horizontal, this);
//	connect(sliderTime, SIGNAL(sliderMoved(int)), this, SLOT(moveTimeSlider(int)));
//
//	labelPlayTime = new QLabel;
//	labelPlayTime->setFixedSize(120, 24);
//	labelPlayTime->setAlignment(Qt::AlignCenter);
//	labelPlayTime->setStyleSheet("background:black; color:cyan");
//	labelPlayTime->setFont((QFont)"MS UI Gothic");
//	labelPlayTime->setText("00:00:00 / 00:00:00");
//
//	buttonStop = new QPushButton(this);
//	buttonStop->setFixedSize(27, 27);
//	buttonStop->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
//	buttonStop->setEnabled(false);
//	connect(buttonStop, SIGNAL(clicked()), this, SLOT(clickStopButton()));
//
//	QPushButton *buttonLeft = new QPushButton(this);
//	buttonLeft->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
//	connect(buttonLeft, SIGNAL(clicked()), this, SLOT(clickLeftButton()));
//
//	buttonPlay = new QPushButton(this);
//	buttonPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
//	connect(buttonPlay, SIGNAL(clicked()), this, SLOT(clickPlayButton()));
//
//	QPushButton *buttonRight = new QPushButton(this);
//	buttonRight->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
//	connect(buttonRight, SIGNAL(clicked()), this, SLOT(clickRightButton()));
//
//	sliderVolume = new QSlider(Qt::Horizontal, this);
//	sliderVolume->setMinimumWidth(80);
//	sliderVolume->setMaximumWidth(400);
//	sliderVolume->setRange(0, 100);
//	sliderVolume->setValue(50);
//	connect(sliderVolume, SIGNAL(sliderMoved(int)), this, SLOT(moveVolumeSlider(int)));
//
//	buttonMute = new QToolButton;
//	buttonMute->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
//	connect(buttonMute, SIGNAL(clicked()), this, SLOT(clickMuteButton()));
//
//	QHBoxLayout *hbox3 = new QHBoxLayout;
//	hbox3->addWidget(labelPlayTime);
//	hbox3->addWidget(buttonStop);
//	hbox3->addWidget(buttonLeft);
//	hbox3->addWidget(buttonPlay);
//	hbox3->addWidget(buttonRight);
//	hbox3->addWidget(sliderVolume);
//	hbox3->addWidget(buttonMute);
//
//	QVBoxLayout *vbox = new QVBoxLayout;
//	vbox->addLayout(hbox1);
//	vbox->addLayout(hbox2);
//	vbox->addWidget(widgetDummy);
//	vbox->addWidget(sliderTime);
//	vbox->addLayout(hbox3);
//
//	QWidget *widget = new QWidget;
//	widget->setLayout(vbox);
//
//	player = new QMediaPlayer;
//	player->setVideoOutput(videoWidget);
//
//	connect(player, SIGNAL(durationChanged(qint64)), this, SLOT(changeDuration(qint64)));
//	connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(changePosition(qint64)));
//	connect(player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this,
//		SLOT(changeMediaStatus(QMediaPlayer::MediaStatus)));
//	connect(player, SIGNAL(bufferStatusChanged(int)), this, SLOT(changeBufferStatus(int)));
//	connect(player, SIGNAL(error(QMediaPlayer::Error)), this, SLOT(onError()));
//	connect(player, SIGNAL(stateChanged(QMediaPlayer::State)),
//		this, SLOT(changeState(QMediaPlayer::State)));
//	connect(player, SIGNAL(volumeChanged(int)), this, SLOT(changeVolume(int)));
//	connect(player, SIGNAL(mutedChanged(bool)), this, SLOT(changeMute(bool)));
//
//	if (!player->isAvailable()) {
//		QMessageBox::warning(this, tr("QMediaPlayer"), tr("プラグインを確認してください。"));
//		setEnabled(false);
//	}
//	return(widget);
//}

void Qt_toolBox::clickPlayMedia()
{
	QString fnm = editMediaFile->text();
	setMediaFile(fnm);
}
void Qt_toolBox::clickPlayPropertyMedia()
{
	QString fnm = labelPath->text() + "/" + labelFile->text();
	editMediaFile->setText(fnm);
	setMediaFile(fnm);
}
void Qt_toolBox::setMediaFile(QString fnm)
{
	QFileInfo fileInfo(fnm);
	if (!(fileInfo.exists() && fileInfo.isFile())) {
		QMessageBox::information(this, tr("ファイルエラー"),
			tr("適切なファイルを指定してください"));
		return;
	}
	QList<QString> sList;
	sList << "wma" << "mp3" << "mp2" << "avi" << "mpg" << "wmv";
	if (sList.indexOf(fileInfo.suffix().toLower()) < 0) {
		QMessageBox::information(this, tr("ファイルエラー"), tr("拡張子が対象外です"));
		return;
	}
	videoWidget->setUpdatesEnabled(true);
	player->setMedia(QUrl::fromLocalFile(fnm));
	player->setVolume(sliderVolume->value());
	player->setMuted(false);
	player->play();
}
void Qt_toolBox::activeRateCombo(int comboIx)
{
	QComboBox *combo = static_cast<QComboBox *>(sender());
	qreal rate = combo->itemData(comboIx).toDouble();
	if (rate != player->playbackRate()) {
		player->setPlaybackRate(rate);
	}
}
void Qt_toolBox::moveTimeSlider(int seconds)
{
	player->setPosition(seconds * 1000);
}
void Qt_toolBox::clickStopButton()
{
	player->setPosition(0);
	player->stop();
}
void Qt_toolBox::clickLeftButton()
{
	qint64 pos = player->position() - 10000;
	player->setPosition(pos < 0 ? 0 : pos);
}
void Qt_toolBox::clickPlayButton()
{
	switch (player->state()) {
	case QMediaPlayer::StoppedState:
	case QMediaPlayer::PausedState:
		videoWidget->setUpdatesEnabled(true);
		player->play();
		break;
	case QMediaPlayer::PlayingState:
		videoWidget->setUpdatesEnabled(false);
		player->pause();
		break;
	}
}
void Qt_toolBox::clickRightButton()
{
	qint64 pos = player->position() + 10000;
	qint64 max = player->duration();
	player->setPosition(pos > max ? max : pos);
}
void Qt_toolBox::moveVolumeSlider(int volume)
{
	player->setVolume(volume);
}
void Qt_toolBox::clickMuteButton()
{
	player->setMuted(!player->isMuted());
}
void Qt_toolBox::changeDuration(qint64 durationValue)
{
	if (durationValue) {
		qint64 dsec = durationValue / 1000;
		sliderTime->setMaximum(dsec);
		QTime tmpTime((dsec / 3600) % 24, (dsec / 60) % 60, dsec % 60, 0);
		labelPlayTime->setText("00:00:00 / " + tmpTime.toString("hh:mm:ss"));
	}
}
void Qt_toolBox::changePosition(qint64 progress)
{
	qint64 curPos = progress / 1000;
	qint64 duration = player->duration() / 1000;
	QTime cTime((curPos / 3600) % 24, (curPos / 60) % 60, curPos % 60, 0);
	QTime dTime((duration / 3600) % 24, (duration / 60) % 60, duration % 60, 0);
	QString str = cTime.toString("hh:mm:ss") + " / " + dTime.toString("hh:mm:ss");

	if (!sliderTime->isSliderDown()) {
		sliderTime->setValue(curPos);
	}
	labelPlayTime->setText(str);
}
void Qt_toolBox::changeMediaStatus(QMediaPlayer::MediaStatus status)
{
	if (status == QMediaPlayer::LoadingMedia ||
		status == QMediaPlayer::BufferingMedia ||
		status == QMediaPlayer::StalledMedia) {
		setCursor(QCursor(Qt::BusyCursor));
	}
	else {
		unsetCursor();
	}
	switch (status) {
	case QMediaPlayer::UnknownMediaStatus:
	case QMediaPlayer::NoMedia:
	case QMediaPlayer::LoadedMedia:
	case QMediaPlayer::BufferingMedia:
	case QMediaPlayer::BufferedMedia:
		statusBar()->showMessage(QString());
		break;
	case QMediaPlayer::LoadingMedia:
		statusBar()->showMessage(tr("Loading..."));
		break;
	case QMediaPlayer::StalledMedia:
		statusBar()->showMessage(tr("Media Stalled"));
		break;
	case QMediaPlayer::EndOfMedia:
		QApplication::alert(this);
		statusBar()->showMessage(tr("End of Media"));
		break;
	case QMediaPlayer::InvalidMedia:
		onError();
		break;
	}
}
void Qt_toolBox::changeBufferStatus(int progress)
{
	statusBar()->showMessage(tr("Buffering %1%").arg(progress));
}
void Qt_toolBox::onError()
{
	statusBar()->showMessage(player->errorString());
}
void Qt_toolBox::changeState(QMediaPlayer::State state)
{
	switch (state) {
	case QMediaPlayer::PlayingState:
		buttonStop->setEnabled(true);
		buttonPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
		break;

	case QMediaPlayer::PausedState:
		buttonStop->setEnabled(true);
		buttonPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
		break;

	case QMediaPlayer::StoppedState:
		buttonStop->setEnabled(false);
		buttonPlay->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
		break;
	}
}
void Qt_toolBox::changeVolume(int volume)
{
	sliderVolume->setValue(volume);
}
void Qt_toolBox::changeMute(bool muted)
{
	buttonMute->setIcon(style()->standardIcon(
		muted ? QStyle::SP_MediaVolumeMuted : QStyle::SP_MediaVolume));
}