#ifndef _PARAMETERTABLE_H_
#define _PARAMETERTABLE_H_

#include "ParameterBase.h"
#include <vector>
#include <QList>
#include <QStringList>

namespace DataProperty
{
	enum TableDataType
	{
		Unknown = 0,
		DoubleNumber,
		String,
	};

	class DATAPROPERTYAPI ParameterTable : public ParameterBase
	{
	public:
		ParameterTable();
		~ParameterTable() = default;

		void copy(ParameterBase* ori, bool valueOnly = false) override;
		//设置类型
		void setDataType(TableDataType t);
		//获取数据类型
		TableDataType getTableDataType();
		//设置行数
		void setRowCount(const int n);
		//设置列数
		void setColumnCount(const int n);
		//获取行数
		int getRowCount();
		//获取列数
		int getColumnCount();
		//获取第index行的数据
		QStringList getRow(int index);
		//获取第index列数据
		QList<double> getColumn(int index);
		//获取第row行 第col列数据
		QString getValue(int row, int col);
		//将第row行 第col列数据设置为v
		void setValue(int row, int col, double v);
		void setValue(int row, int col, QString v);
		//设置表头
		void setTitle(QStringList t);
		//获取表头
		QStringList getTitle();
		//获取全部数据
		std::vector<std::vector<double>> getData();
		std::vector<std::vector<QString>> getStringData();

		//设置全部数据
		void setData(std::vector<std::vector<double>> data);

		void writeParameter(QDomDocument* doc, QDomElement* parent) override;
		void readParameter(QDomElement* e);

		void setValueFromString(QString v) override;

	protected:
		void resizeData();

	protected:
		TableDataType _dataType{ DoubleNumber };
		int _rowCount{ 0 };
		int _columnCount{ 0 };
		std::vector<std::vector<double>> _data{};
		std::vector<std::vector<QString>> _stringData{};
		QStringList _title{};
		
	};
}

#endif