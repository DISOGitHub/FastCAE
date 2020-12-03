#include "ParameterList.h"
#include <QDomElement>
#include <QDomNodeList>
#include <QDomDocument>
#include "ParameterInt.h"
#include "ParameterDouble.h"
#include "ParameterBool.h"
#include "ParameterSelectable.h"
#include "ParameterTable.h"
#include "ParameterString.h"
#include "ParameterPath.h"
#include "DataBase.h"
#include <QDebug>
#include <QDataStream>

namespace DataProperty
{
	ParameterList::~ParameterList()
	{
		const int n = _paraList.size();
		for (int i = 0; i < n; ++i)
		{
			ParameterBase* p = _paraList.at(i);
			delete p;
		}
		_paraList.clear();
	}
	void ParameterList::appendParameter(ParameterBase* para)
	{
		_paraList.append(para);
	}
	ParameterBase* ParameterList::getParameterAt(const int i)
	{
		return _paraList.at(i);
	}
	int ParameterList::getParameterCount()
	{
		return _paraList.size();
	}
	ParameterBase* ParameterList::appendParameter(ParaType t)
	{
		ParameterBase* pbase = this->createParameterByType(t);
		if (pbase != nullptr) _paraList.append(pbase);
		return pbase;

	}

	void ParameterList::writeParameters(QDomDocument* doc, QDomElement* parent)
	{
		const int n = this->getParameterCount();
		for (int i = 0; i < n; ++i)
		{
			ParameterBase* p = _paraList.at(i);
			if (p == nullptr) continue;
			QDomElement ele = doc->createElement("Parameter");
			p->writeParameter(doc, &ele);
			parent->appendChild(ele);
		}

	}

	void ParameterList::readParameters(QDomElement* ele)
	{
		QDomNodeList paraList = ele->elementsByTagName("Parameter");
		const int n = paraList.size();
		for (int i = 0; i < n; ++i)
		{
			QDomElement ele = paraList.at(i).toElement();
			QString stype = ele.attribute("Type");
			DataProperty::ParaType type = ParameterBase::StringToParaType(stype);
			DataProperty::ParameterBase* para = this->createParameterByType(type);

			if (para == nullptr) continue;
			para->readParameter(&ele);
			this->appendParameter(para);
		}
	}
	ParameterBase* ParameterList::createParameterByType(QString stype)
	{
		DataProperty::ParaType type = ParameterBase::StringToParaType(stype);
		return createParameterByType(type);
	}
	ParameterBase* ParameterList::createParameterByType(ParaType t)
	{
		ParameterBase* p = nullptr;

		switch (t)
		{
		case DataProperty::Para_Int:
			p = new DataProperty::ParameterInt;
			break;
		case DataProperty::Para_Double:
			p = new DataProperty::ParameterDouble;
			break;
		case DataProperty::Para_String:
			p = new DataProperty::ParameterString;
			break;
		case DataProperty::Para_Bool:
			p = new DataProperty::ParameterBool;
			break;
		case DataProperty::Para_Selectable:
			p = new DataProperty::ParameterSelectable;
			break;
		case DataProperty::Para_Path:
			p = new DataProperty::ParameterPath;
			break;
		case DataProperty::Para_Table:
			p = new DataProperty::ParameterTable;
			break;
		default:
			break;
		}

		return p;
	}
	void ParameterList::copy(ParameterList* data)
	{
		if (data == nullptr) return;
		const int n = data->getParameterCount();
		for (int i = 0; i < n; ++i)
		{
			ParameterBase* pb = data->getParameterAt(i);
			ParameterBase* para = this->createParameterByType(pb->getParaType());
			if (para != nullptr)
			{
				para->copy(pb);
				this->appendParameter(para);
			}
		}
	}
	int ParameterList::getVisibleParaCount()
	{
		int count = 0;
		for (int i = 0; i < _paraList.size(); ++i)
		{
			ParameterBase* b = _paraList.at(i);
			if (b->isVisible())
				count++;
		}
		return count;
	}

	ParameterBase* ParameterList::getVisibleParameterAt(const int i)
	{
		QList<ParameterBase*> list;
		for (int i = 0; i < _paraList.size(); ++i)
		{
			ParameterBase* b = _paraList.at(i);
			if (b->isVisible())
				list.append(b);
		}
		ParameterBase* b = nullptr;
		if (i >= 0 && i < list.size()) b = list.at(i);
		return b;
	}

	ParameterBase* ParameterList::getParameterByName(QString name)
	{
		ParameterBase* p = nullptr;
		for (int i = 0; i < _paraList.size(); ++i)
		{
			auto pa = _paraList.at(i);
			if (name == pa->getDescribe())
			{
				p = pa;
				break;;
			}
		}
		return p;
	}

	ParameterBase* ParameterList::copyParameter(ParameterBase* p)
	{
		ParameterBase* cp = nullptr;
		if (p != nullptr)
		{
			ParaType t = p->getParaType();
			cp = createParameterByType(t);
			cp->copy(p);
		}
		return cp;

	}

	QList<ParameterBase*> ParameterList::getParaList(){
		return _paraList;
	}

	void ParameterList::dataToStream(QDataStream* datas)
	{
		int nparaList = getParameterCount();
		for (int i = 0; i < nparaList; i++){
			DataProperty::ParaType paratype = _paraList[i]->getParaType();
			switch (paratype)
			{
			case DataProperty::Para_Int:
			{
				DataProperty::ParameterInt *para_int = dynamic_cast<DataProperty::ParameterInt*>(getParameterAt(i));
				*datas << QString::number(para_int->getValue());
//				qDebug() << para_int->getDescribe() << para_int->getValue();
				break;
			}
			case DataProperty::Para_Double:
			{
				DataProperty::ParameterDouble *para_double = dynamic_cast<DataProperty::ParameterDouble*>(getParameterAt(i));
				*datas << QString::number(para_double->getValue());
//				qDebug() << para_double->getDescribe() << para_double->getValue();
				break;
			}
			case DataProperty::Para_Bool:
			{
				DataProperty::ParameterBool *para_bool = dynamic_cast<DataProperty::ParameterBool*>(_paraList[i]);
				//*datas << QString::number(para_bool->getValue());
				*datas << para_bool->getValue();
//				qDebug() << para_bool->getDescribe() << QString::number(para_bool->getValue());
				break;
			}
			case DataProperty::Para_Selectable:
			{
				DataProperty::ParameterSelectable *para_selectable = dynamic_cast<DataProperty::ParameterSelectable*>(_paraList[i]);
				//*datas << QString::number(para_selectable->getCurrentIndex());
				*datas << para_selectable->getCurrentIndex();
//				qDebug() << para_selectable->getDescribe() << para_selectable->getCurrentIndex();
				break;
			}
			case DataProperty::Para_String:
			{
				DataProperty::ParameterString *para_string = dynamic_cast<DataProperty::ParameterString*>(_paraList[i]);
				*datas << para_string->getValue();
//				qDebug() << para_string->getDescribe() << para_string->getValue();
				break;
			}
			case DataProperty::Para_Path:
			{
				DataProperty::ParameterPath *para_path = dynamic_cast<DataProperty::ParameterPath*>(_paraList[i]);
				*datas << para_path->getFile();
//				qDebug() << para_path->getDescribe() << para_path->getFile();
				break;
			}
			case DataProperty::Para_Table:
			{
				DataProperty::ParameterTable *para_table = dynamic_cast<DataProperty::ParameterTable*>(_paraList[i]);
				for (int k = 0; k < para_table->getRowCount(); k++){
					for (int j = 0; j < para_table->getColumnCount(); j++){
						//*datas << QString::number(para_table->getValue(k, j));
						*datas << para_table->getValue(k, j);
					}
				}
				break;
			}
			default:
				break;
			}
		}
	}

	void ParameterList::removeParameter(ParameterBase* p)
	{
		if (_paraList.contains(p))
		{
			_paraList.removeOne(p);
			delete p;
		}
	}

	void ParameterList::removeParameterAt(int i)
	{
		if (i < 0 || i >= _paraList.size()) return;

		ParameterBase* p = _paraList.at(i);
		this->removeParameter(p);
	}

}