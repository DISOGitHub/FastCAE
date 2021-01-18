/*****************************
自定义widget
所有的widget都必须继承自这个类
*****************************/

#ifndef SELFDEFWIDGHTS_H
#define SELFDEFWIDGHTS_H

#include "SelfDefObjectAPI.h"
#include <QWidget>
#include <QList>

class QBoxLayout;
class SelfDefObjBase;

namespace DataProperty
{
	class DataBase;
}

namespace SelfDefObj
{
	class SELFDEFINEOBJAPI SelfDefWidget : public QWidget
	{
		Q_OBJECT

	public:
		SelfDefWidget(DataProperty::DataBase *data, bool isNew= false, QWidget* parent = 0, Qt::WindowFlags f = 0);
		~SelfDefWidget();

		/*存储数据*/
		virtual void apply() = 0;
		/*检测全部数据的合法性，合法返回True，存在不合法返回False*/
		bool checkObjs();
		///获取检测控件个数
		int getCheckObjCount();
		///获取第i个检测控件
		SelfDefObjBase* getCheckObjAt(const int i);
		///获取检测控件列表
		QList<SelfDefObjBase*> getCheckObjList();

	protected:
		/*将数据填充到UI*/
		virtual void init() = 0;
		/*添加需要检测的控件对象*/
		void appendCheckObj(SelfDefObjBase* checkObj);

	protected:
		bool _isNew{ false }; //新建为false，否则为true；
		DataProperty::DataBase* _data{};
		QList<SelfDefObjBase*> _checkObjList{};

	};

}


#endif
