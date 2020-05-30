#ifndef _PARAMETERTABLE_H_
#define _PARAMETERTABLE_H_

#include "ParameterBase.h"
#include <vector>
#include <QList>
#include <QStringList>

namespace DataProperty
{

	class DATAPROPERTYAPI ParameterTable : public ParameterBase
	{
	public:
		ParameterTable();
		~ParameterTable() = default;

		void copy(ParameterBase* ori, bool valueOnly = false) override;
		//��������
		void setRowCount(const int n);
		//��������
		void setColumnCount(const int n);
		//��ȡ����
		int getRowCount();
		//��ȡ����
		int getColumnCount();
		//��ȡ��index�е�����
		QList<double> getRow(int index);
		//��ȡ��index������
		QList<double> getColumn(int index);
		//��ȡ��row�� ��col������
		double getValue(int row, int col);
		//����row�� ��col����������Ϊv
		void setValue(int row, int col, double v);
		//���ñ�ͷ
		void setTitle(QStringList t);
		//��ȡ��ͷ
		QStringList getTitle();
		//��ȡȫ������
		std::vector<std::vector<double>> getData();
		//����ȫ������
		void setData(std::vector<std::vector<double>> data);

		void writeParameter(QDomDocument* doc, QDomElement* parent) override;
		void readParameter(QDomElement* e);

		void setValueFromString(QString v) override;

	protected:
		void resizeData();

	protected:
		int _rowCount{ 0 };
		int _columnCount{ 0 };
		std::vector<std::vector<double>> _data{};
		QStringList _title{};
	};
}

#endif