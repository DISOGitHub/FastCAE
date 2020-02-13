#include "ProgressBar.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QApplication>

ProgressBar::ProgressBar(QWidget *parent)
	: QWidget(parent, Qt::Widget), mAbortJob(false)
{
	mMsgLabel = new QLabel(this);
	mMsgLabel->setText("Progress message...");
	mProgBar = new QProgressBar(this);
	mProgBar->setRange(0, 100);
	mAbortBtn = new QPushButton("Abort", this);
	QHBoxLayout *hLayout = new QHBoxLayout;
	hLayout->addWidget(mProgBar);
	hLayout->addWidget(mAbortBtn);
	QVBoxLayout *vLayout = new QVBoxLayout;
	vLayout->addWidget(mMsgLabel);
	vLayout->addLayout(hLayout);
	this->setLayout(vLayout);
	this->setVisible(true);

	QObject::connect(mAbortBtn, SIGNAL(clicked()), this, SLOT(OnAbort()));
}

ProgressBar::~ProgressBar()
{

}

void ProgressBar::SetRange(int range)
{
	mRangeSize = range;
	mProgBar->setRange(0, mRangeSize);
}

void ProgressBar::SetMessage(const QString& msg)
{
	mMsgLabel->setText(msg);
	qApp->processEvents();
}

void ProgressBar::SetProgress(int pos)
{
	mProgBar->setValue(pos);
	qApp->processEvents();
}

void ProgressBar::OnAbort()
{
	mAbortJob = true;
}