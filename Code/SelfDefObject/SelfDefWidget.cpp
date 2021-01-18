#include "SelfDefWidget.h"
#include "DataProperty/DataBase.h"
#include "SelfDefObjectBase.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

namespace SelfDefObj
{
	SelfDefWidget::SelfDefWidget(DataProperty::DataBase* data, bool isnew,/* WidgetType type, */QWidget* parent /* = 0 */, Qt::WindowFlags f /* = 0 */)
		:/*_type(type),*/ _data(data), _isNew(isnew), QWidget(parent, f)
	{
// 		this->setFocusPolicy(Qt::ClickFocus);
// 		this->grabKeyboard();
	}
	void SelfDefWidget::init()
	{
	}

	SelfDefWidget::~SelfDefWidget()
	{
//		if (_boxLayout != nullptr) delete _boxLayout;
		_checkObjList.clear();
	}

	void SelfDefWidget::appendCheckObj(SelfDefObjBase* checkObj)
	{
		_checkObjList.append(checkObj);
	}
	bool SelfDefWidget::checkObjs()
	{
		const int n = _checkObjList.size();
		for (int i = 0; i < n; ++i)
		{
			SelfDefObjBase* obj = _checkObjList.at(i);
			if (!obj->applyCheckable()) continue;
			if (!obj->check()) return false;
		}
		return true;
	}
	void SelfDefWidget::apply()
	{

	}
	int SelfDefWidget::getCheckObjCount()
	{
		return _checkObjList.size();
	}
	SelfDefObjBase* SelfDefWidget::getCheckObjAt(const int i)
	{
		if (i >= 0 && i < _checkObjList.size())
			return _checkObjList.at(i);
		return nullptr;
	}
	QList<SelfDefObjBase*> SelfDefWidget::getCheckObjList()
	{
		return _checkObjList;
	}
}