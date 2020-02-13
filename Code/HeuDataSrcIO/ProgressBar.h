#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include "heudatasrcio_global.h"

class  ProgressBar : public QWidget
{
	Q_OBJECT
public:
	int mRangeSize=100;
	ProgressBar(QWidget *parent = 0);
	~ProgressBar();

	void SetRange(int range);

	void SetMessage(const QString& msg);
	void SetProgress(int pos);
	bool IsTaskAborted() { return mAbortJob; }

private slots:
	void OnAbort();

private:
	QLabel *mMsgLabel;
	QProgressBar *mProgBar;
	QPushButton *mAbortBtn;
	bool mAbortJob;
};

#endif // PROGRESSBAR_H
