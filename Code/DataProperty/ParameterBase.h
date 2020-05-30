#ifndef _EDITABLEPROPERTYBASE_H_
#define _EDITABLEPROPERTYBASE_H_

#include "DataProperty/DataPropertyAPI.h"
#include <QString>
#include <QObject>
#include <QList>

class QDomDocument;
class QDomElement;

namespace ConfigOption
{
	class ParameterObserver;
}

namespace DataProperty
{
	enum  ParaType
	{
		Para_Blank = 0,
		Para_Int,
		Para_Double,
		Para_Color,
		Para_String,
		Para_Bool,
		Para_Selectable,
		Para_Path,
		Para_Table,
	};

	enum ModuleType
	{
		Module_None = 0,
		Module_Model,
		Module_BC,
		Module_Material,
		Module_Mesher,
	};
	
	class DATAPROPERTYAPI ParameterBase : public QObject
	{
		Q_OBJECT

	public:
		ParameterBase(ParaType t);
		ParameterBase() = default;
		~ParameterBase() = default;
		//��ori�п�������
		virtual void copy(ParameterBase* ori, bool valueOnly = false);
		//��ȡ����
		ParaType getParaType();
		//��������
		void setParaType(ParaType type);
		//��ȡ����
		QString getDescribe();
		//��������
		void setDescribe(QString s);
		//�����Ƿ�ɼ�
		void setVisible(bool v);
		//��ȡ�ɼ�״̬
		bool isVisible();
		//�����Ƿ�ɱ༭
		void setEditable(bool e);
		//��ȡ�ɱ༭״̬
		bool isEditable();
		//�����������ƣ���setDescribe()��Ӧ
		void setChinese(QString chinese);
		//��ȡ��������
		QString getChinese();
		//��ֵת��Ϊ�ַ���
		virtual QString valueToString();
		//���ַ�����ȡ��������
		virtual void setValueFromString(QString v);
		//����ģ������
		void setModuleType(ModuleType t);
		//��ȡģ������
		ModuleType getModuleType();
		//��ȡ����ID(DataBase)
		int getDataID();
		//��������ID
		void setDataID(int id);
		//��ȡ��������
		int getDataIndex();
		//������������
		void setDataIndex(int index);
		//��ȡ���ڵ�������
		QString getGroupName();
		//�����������
		void setGroupName(QString group);
		//����������/��������
		QString genAbsoluteName();
		//��¼�۲���
		void appendObserver(ConfigOption::ParameterObserver* obs);
		//��ȡ�۲����б�
		QList<ConfigOption::ParameterObserver*> getObserverList();
		//�ж���ֵ�Ƿ�һ��
		virtual bool isSameValueWith(ParameterBase* p);
		//״̬����
		virtual void copyStatus(ParameterBase* p);
		///����д�빤���ļ�
		virtual void writeParameter(QDomDocument* doc, QDomElement* parent);
		///�ӹ����ļ���������
		virtual void readParameter(QDomElement* e);
		//����ת��Ϊ�ַ���
		static QString ParaTypeToString(ParaType t);
		//�ַ���ת��Ϊ��������
		static ParaType StringToParaType(QString stype);

	signals:
		//�źţ�����ֵ�����仯
		void dataChanged();


	protected:
		ParaType _type{ Para_Blank };
		QString _describe{};
		QString _chinese{};
		bool _editable{ true };
		bool _visible{ true };
		QList<ConfigOption::ParameterObserver*> _observerList{};

		//****�����������ģ�������Ϣ,��DataBase��ͬ********
		ModuleType _moduleType{ Module_None };
		int _dataID{ -1 };
		int _dataIndex{ -1 };
		QString _groupName{};
		//**************************************************
	};

}


#endif
