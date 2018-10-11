#include "qt_toolbox.h"
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

//visualstudioだと自動的にshiftJISにてビルドします．Qtは標準的にUTFでのみ日本語の使用が可能となっているので，下記のようにQString型の8bitへと変換しています．
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
	QAction *colorAct = new QAction(tr("色選択"), this);
	connect(colorAct, SIGNAL(triggered()), this, SLOT(dialogColor()));
	QAction *fontAct = new QAction(tr("フォント選択"), this);
	connect(fontAct, SIGNAL(triggered()), this, SLOT(dialogFont()));

	QMenu *dialogMenu = menuBar()->addMenu(tr("ダイアログ"));
	dialogMenu->addAction(openAct);
	dialogMenu->addAction(saveAct);
	dialogMenu->addAction(pathAct);
	dialogMenu->addAction(colorAct);
	dialogMenu->addAction(fontAct);
	dialogMenu->addSeparator();
	dialogMenu->addMenu(msgMenu);

	//4.おまけpixmap
	QAction *pixmapAct = new QAction(tr("標準Pixmap一覧"), this);
	connect(pixmapAct, SIGNAL(triggered()), this, SLOT(displayStandardPixmap()));
	QAction *cursorAct = new QAction(tr("標準カーソル"), this);
	connect(cursorAct, SIGNAL(triggered()), this, SLOT(checkStandardCursor()));

	QMenu *refMenu = menuBar()->addMenu(tr("参考"));
	refMenu->addAction(pixmapAct);
	refMenu->addAction(cursorAct);

	//5.ツールバーにメニュー項目を追加
	QToolBar *fileToolBar = addToolBar(tr("File"));
	fileToolBar->addAction(infoAct);
	fileToolBar->addAction(exitAct);

	QToolBar *dialogToolBar = addToolBar(tr("Dialog"));
	dialogToolBar->addAction(colorAct);
	dialogToolBar->addAction(fontAct);
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
		this, tr("フォルダ選択"), tr("C:/DEVEL"), options
		);
	if (!strDir.isEmpty()) {} 
}
/*
void Qt_toolBox::dialogColor()
{
	QColor color = QColorDialog::getColor(Qt::yellow, this);
	if (color.isValid())  qDebug() << color.name();
}
*/
/*
void Qt_toolBox::dialogFont()
{
	bool ok;
	QFont font = QFontDialog::getFont(&ok, QFont("Arial", 18), this, tr("フォント選択"));

	if (ok) {
		qDebug() << font.family();
		qDebug("size=%d,italic=%d,bold=%d", font.pointSize(), font.italic(), font.bold());
	}
}
*/

void Qt_toolBox::dialogMsgWarn()
{
	QMessageBox::warning(this, tr("MessageBoxタイトル"), tr("警告メッセージの練習"));
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
	statusBar()->showMessage(tr("こうていをどーてーするシステム"));
	
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
	QIcon icon = QIcon("C:/img/logo.jpg");
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

//右領域の作成
//QWidget * Qt_toolBox::makeRightArea()
//{
//}


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

void Qt_toolBox::clickPropertyClear()
{
	labelPath->setText("");
	labelFile->setText("");
	labelSize->setText("");
	labelDate->setText("");
	labelType->setText("");
}

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

QTabWidget *Qt_toolBox::createTabWidget()
{
	/*
	QWidget *widgetGroup = makeGroupTab();
	QWidget *widgetText = makeTextTab();

	QWidget *widgetPlayer = makePlayerTab();
	QWidget *widgetCalendar = makeCalendarTab();
	*/
	QWidget *widgetImage = makeImageTab();
	QWidget *widgetEmpty = makeEmptyTab();
	QWidget *widgetVideo = makeVideoTab();
	
	QTabWidget *tab = new QTabWidget;
	/*
	tab->addTab(widgetGroup, "グループ");
	tab->addTab(widgetText, "文書");
	
	tab->addTab(widgetPlayer, "動画");
	tab->addTab(widgetCalendar, "暦");
	*/
	tab->addTab(widgetImage, tr( "画像の表示"));
	tab->addTab(widgetEmpty, tr("テスト画面"));
	tab->addTab(widgetVideo, tr("動画の表示"));
	
	return tab;
}

QWidget *Qt_toolBox::makeVideoTab()
{
	QLabel *label = new QLabel;
	label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	label->setText(tr("video"));

	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addWidget(label);
	QWidget *widget = new QWidget;
	widget->setLayout(vbox);
	return widget;
}
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
		labelImage->setPixmap(pixmapOriginal);
	}
	else {
		QPixmap pixmap = pixmapOriginal;
		int wd = scrollStack->width() - 48;
		int ht = scrollStack->height() - 132;

		switch (comboVal) {
		case 2: labelImage->setPixmap(pixmap.scaledToWidth(wd));  break;
		case 3: labelImage->setPixmap(pixmap.scaledToHeight(ht)); break;
		case 4: labelImage->setPixmap(pixmap.scaled(wd, ht));     break;
		}
	}
}