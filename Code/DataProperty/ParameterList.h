#ifndef _PARAMETERLIST_H_
#define _PARAMETERLIST_H_

#include <QList>
#include "DataPropertyAPI.h"
#include "ParameterBase.h"
// #include "ParameterInt.h"
// #include "ParameterDouble.h"
// #include "ParameterBool.h"
// #include "ParameterSelectable.h"
// #include "ParameterTable.h"
// #include "ParameterString.h"
// #include "ParameterPath.h"

class QDataStream;
class QDomDocument;
class QDomElement;

namespace DataProperty
{
	class ParameterBase;
	class DataBase;

	class DATAPROPERTYAPI ParameterList
	{
	public:
		ParameterList() = default;
		~ParameterList();
		//��data�п��������б�
		void copy(ParameterList* data);
		//��Ӳ���
		virtual void appendParameter(ParameterBase* para);
		//�������ʹ����µĲ������������б�
		virtual ParameterBase* appendParameter(ParaType type);
		//��ȡ��i������
		ParameterBase* getParameterAt(const int i);
		//��ȡ��������
		int getParameterCount();
		//��ȡ�ɼ��Ĳ�������
		int getVisibleParaCount();
		//�Ƴ�����
		virtual void removeParameter(ParameterBase* p);
		//�Ƴ���i������
		void removeParameterAt(int i);
		//��ȡ��i�Ϳɼ��Ĳ���
		ParameterBase* getVisibleParameterAt(const int i);
		//�������ʹ����µĲ����������������б�
		static ParameterBase* createParameterByType(ParaType t);
		//�������ʹ����µĲ����������������б�
		static ParameterBase* createParameterByType(QString stype);
		//���ݴ����²�����������p�����ݣ������������б�
		static ParameterBase* copyParameter(ParameterBase* p);

		///д����������
		virtual void writeParameters(QDomDocument* doc, QDomElement* parent);
		//��������
		virtual void readParameters(QDomElement* ele);
		//�������ƻ�ȡ����
		virtual ParameterBase* getParameterByName(QString name);
		//��ȡmd5
		virtual void dataToStream(QDataStream* datas);
		//��ȡȫ�������б�
		QList<ParameterBase*> getParaList();


	protected:
		QList<ParameterBase*> _paraList{};
	};
}

#endif
