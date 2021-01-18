#ifndef SELFDEFINELINEEDIT_H
#define SELFDEFINELINEEDIT_H

/* ***********************************
自定义LineEdit
功能：int double sttring 类型限定
检查范围
限定小数点后面位数
数据非法背景变红
Author： libaojun
*********************************** */

#include <QWidget>
#include "SelfDefObjectAPI.h"
#include "SelfDefObjectBase.h"

enum LineEdieDataType
{
	DATATYPENONE = 0,
	DATATYPEINT,
	DATATYPEDOUBLE,
	DATATYPESTRING,
};

namespace Ui
{
	class SelfDefLineEdit;
}

class SELFDEFINEOBJAPI SelfDefLineEdit : public QWidget, public SelfDefObjBase
{
	Q_OBJECT

public:
	SelfDefLineEdit(QWidget *parent = 0);
	~SelfDefLineEdit();

	void setDataType(LineEdieDataType t);
	LineEdieDataType getDataType();
	/*设置int数据，需要首先设置类型为DATATYPEINT 否则不生效 */
	void setData(int data);
	/*设置double数据，需要首先设置类型为DATATYPEDOUBLE 否则不生效 */
	void setData(double data);
	/*设置string数据，需要首先设置类型为DATATYPEString 否则不生效 */
	void setText(const QString &text);
	/*打开/关闭范围检测 */
	void enableRangeCheck(bool on);
	/*设置合法的数据范围 */
	void setRange(double min, double max);
	/*合法情况下获取INT数据，否则返回-1e-10 */
	int getIntData();
	/*合法情况下获取double数据，否则返回-1e-66 */
	double getDoubleData();
	/*合法情况下获取String数据，否则返回FFFFFFFFFFF */
	QString getText();
	/*设置提示字符*/
	void setRemindText(QString text);
	/*设置精确度,参数小于0不检测精确度,仅对Double生效 */
	void setAccuracy(int acc);
	/*获取精确度 */
	int getAccuracy();
	///设置是否可以编辑
	void setEditable(bool editable);
	///设置apply时是否检查
	void setApplyCheckable(bool checkable);
	///apply时是否检查
	bool applyCheckable();

	bool check() override;

signals:
	/*数据改变即时发送信号 */
	void dataChanged();

private:
	bool event(QEvent* e) override;

private slots:
	void soltCheck();

private:
	Ui::SelfDefLineEdit* _ui{};
	LineEdieDataType _type{ DATATYPESTRING };
	bool _checkable{ false };
	double _range[2];
	int _accuracy{ -1 };

	bool _applyCheckable{ true };

	QString _toolTip{};

};

#endif