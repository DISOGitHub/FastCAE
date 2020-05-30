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
		//设置行数
		void setRowCount(const int n);
		//设置列数
		void setColumnCount(const int n);
		//获取行数
		int getRowCount();
		//获取列数
		int getColumnCount();
		//获取第index行的数据
		QList<double> getRow(int index);
		//获取第index列数据
		QList<double> getColumn(int index);
		//获取第row行 第col列数据
		double getValue(int row, int col);
		//将第row行 第col列数据设置为v
		void setValue(int row, int col, double v);
		//设置表头
		void setTitle(QStringList t);
		//获取表头
		QStringList getTitle();
		//获取全部数据
		std::vector<std::vector<double>> getData();
		//设置全部数据
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