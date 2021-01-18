#ifndef REPORTPROCESSINGDIALOG_H
#define REPORTPROCESSINGDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
namespace Ui
{
    class ReportProcessingDialog;
}

class ReportProcessingDialog : public QDialog
{
	Q_OBJECT

public:
	ReportProcessingDialog(QString hintMsg, QString iconPath, bool isVisible = true, QWidget *parent = 0);
	~ReportProcessingDialog();
	void startMovie();
public:
	void setTitle(QString title);
	void setBackgroundColor(QString color = "background-color: rgb(55, 135,215);");
	QString getHintMsg() const;
	void setHintMsg(const QString &value);

	QString getCancelBtnText() const;
	void setCancelBtnText(const QString &value = tr("Cancel"));
	//设置按钮和提示信息位置
	void setButtonPosition();
signals:
	void sig_windowClose();
	private slots:


	void on_cancelBtn_clicked();

private:
	
	//更改圆形窗口及背景颜色
	void changeFormStyle();
	void setIconPath(QString path);
	//是否显示结束按钮
	bool isVisibleCancle{ true };
	void changeLayout();
private:
	QString _Title;
	//QLabel* movieLabel{};
//	QLabel* hintMsgLabel{};
	//QPushButton* cancelButton{};
	QHBoxLayout* bottomLayout{};
	QHBoxLayout* middleLayout{};
	QVBoxLayout* vertialLayout{};

	QHBoxLayout* topLayout{};
	//结束按钮标题
	QString  cancelBtnText;
	//窗口中等待提示信息
	QString  mhintMsg;
	QString  backgroundColor;
	QString  miconPath;
	QMovie   *movie{};
	Ui::ReportProcessingDialog *ui;
	void paintEvent(QPaintEvent *event) override;
};



#endif // REPORTPROCESSINGDIALOG_H
