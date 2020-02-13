#include "ReportProcessingDialog.h"
#include "ui_ReportProcessingDialog.h"
#include "qpainter.h"
#include <QMessageBox>
#include <QBitmap>
#include <QMovie>
#include <QSpacerItem>
ReportProcessingDialog::ReportProcessingDialog(QString hintMsg, QString iconPath, bool isVisible, QWidget *parent) :
QDialog(parent),
ui(new Ui::ReportProcessingDialog)
{
	this->isVisibleCancle = isVisible;
	miconPath = iconPath;
	mhintMsg = hintMsg;
	ui->setupUi(this);
//	setBackgroundColor();
	setWindowFlags(Qt::FramelessWindowHint);
//	changeFormStyle();
	setCancelBtnText();

	ui->cancelButton->setText(cancelBtnText);
//	changeLayout();
	_Title = tr("Generateing Mesh");
	setWindowTitle(_Title);
	ui->hintMsgLabel->setText(mhintMsg);
	connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(on_cancelBtn_clicked()));
	setWindowFlags(Qt::FramelessWindowHint);//无边框
	setAttribute(Qt::WA_TranslucentBackground);//背景透明
	//   设置关于窗体为圆角
	setFixedSize(this->width(), this->height());
	setButtonPosition();

}

ReportProcessingDialog::~ReportProcessingDialog()
{
	delete ui;
}

void ReportProcessingDialog::startMovie()
{
	movie = new QMovie(miconPath);
	ui->movieLable->setMovie(movie);
	movie->start();
}

void ReportProcessingDialog::setTitle(QString title)
{
	_Title = title;
	setWindowTitle(title);
}

void ReportProcessingDialog::setBackgroundColor(QString color)
{
	backgroundColor = color;
}

void ReportProcessingDialog::changeFormStyle()
{
/*	QBitmap bmp(this->size());

	bmp.fill();

	QPainter p(&bmp);

	p.setPen(Qt::NoPen);

	p.setBrush(Qt::black);

	p.drawRoundedRect(bmp.rect(), 20, 20);
	setMask(bmp);
	//设置圆形窗口及背景色
	setStyleSheet(backgroundColor);*/
}
void ReportProcessingDialog::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
}

void ReportProcessingDialog::setIconPath(QString path)
{
	miconPath = path;
}

void ReportProcessingDialog::changeLayout()
{
	/*hintMsgLabel = new QLabel();
	hintMsgLabel->setText(mhintMsg);

	if (isVisibleCancle)
	{
		cancelButton = new QPushButton();
		cancelButton->setFlat(true);
		cancelButton->setStyleSheet("QPushButton{ font-family:'Microsoft YaHei';font-size:12px;color:#000000;min-width: 50px; color: white;}");
		connect(cancelButton, SIGNAL(clicked()), this, SLOT(on_cancelBtn_clicked()));
		cancelButton->setText(cancelBtnText);
	}

	vertialLayout = new QVBoxLayout;
	bottomLayout = new QHBoxLayout;
	middleLayout = new QHBoxLayout;
	
	middleLayout->addStretch();
	movieLabel = new QLabel();
	movieLabel->setScaledContents(false);
	middleLayout->addWidget(movieLabel);
	middleLayout->addStretch();

	bottomLayout->addStretch();
	bottomLayout->addWidget(hintMsgLabel);
	bottomLayout->addWidget(cancelButton);
	bottomLayout->addStretch();
	//vertialLayout->addLayout(topLayout);
	vertialLayout->addLayout(middleLayout);
	vertialLayout->addLayout(bottomLayout);
	bottomLayout->setSpacing(7);
	this->setLayout(vertialLayout);*/
}
QString ReportProcessingDialog::getCancelBtnText() const
{
	return cancelBtnText;
}

void ReportProcessingDialog::setCancelBtnText(const QString &value)
{
	cancelBtnText = value;
}

void ReportProcessingDialog::setButtonPosition()
{
	ui->movieLable->setGeometry(0, 0, this->width(), this->height());
	double dlgWidth = this->width();
	double dlgHeight = this->height();
	double hintMsgLength = ui->hintMsgLabel->width();
	double buttonLength = ui->cancelButton->width();
	double lefthValue = this->width() - hintMsgLength - buttonLength;
	lefthValue = lefthValue / 2;
	double y = this->height() - 50;
	double buttonLeftValue = lefthValue + hintMsgLength + 10;
	ui->hintMsgLabel->setGeometry(lefthValue, y, hintMsgLength, ui->hintMsgLabel->height());
	ui->cancelButton->setGeometry(buttonLeftValue, y, buttonLength, ui->cancelButton->height());
}

void ReportProcessingDialog::on_cancelBtn_clicked()
{
	emit sig_windowClose();
	close();
}

QString ReportProcessingDialog::getHintMsg() const
{
	return mhintMsg;
}

void ReportProcessingDialog::setHintMsg(const QString &value)
{
	mhintMsg = value;
}
