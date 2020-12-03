#include "ParameterTable.h"
#include <QDomText>
#include <QDomElement>
#include <QDomDocument>
#include <QDomNodeList>
#include <assert.h>
#include <math.h>

namespace DataProperty
{

	ParameterTable::ParameterTable()
		: ParameterBase(Para_Table)
	{
	}

	void ParameterTable::setRowCount(const int n)
	{
		_rowCount = n;
		resizeData();
	}

	void ParameterTable::setColumnCount(const int n)
	{
		_columnCount = n;
		resizeData();
	}

	int ParameterTable::getRowCount()
	{
		return _rowCount;
	}

	int ParameterTable::getColumnCount()
	{
		return _columnCount;
	}

	void ParameterTable::resizeData()
	{
		_data.resize(_rowCount);
		for (int i = 0; i < _rowCount; ++i)
			_data[i].resize(_columnCount);

		_stringData.resize(_rowCount);
		for (int i = 0; i < _rowCount; ++i)
			_stringData[i].resize(_columnCount);
	}

	QStringList ParameterTable::getRow(int index)
	{
		QStringList row;
		if (index < _rowCount)
		{
			if (_dataType == DoubleNumber)
			{
				std::vector<double> r = _data.at(index);
				for (int i = 0; i < r.size(); ++i)
					row.append(QString::number(r.at(i)));
			}
			else if (_dataType == String)
			{
				std::vector<QString> r = _stringData.at(index);
				for (int i = 0; i < r.size(); ++i)
					row.append(r.at(i));
			}
				
			
		}
		return row;
	}

	QList<double> ParameterTable::getColumn(int index)
	{
		QList<double> column;
		if (index < _columnCount)
		{
			for (int i = 0; i < _rowCount; ++i)
			{
				double d = _data[i][index];
				column.append(d);
			}
		}
		return column;
	}
	
	void ParameterTable::setValue(int row, int col, double v)
	{
		if (row < _rowCount && col < _columnCount)
		{
			double  c = _data[row][col];
			if (fabs(c - v) >0.0000001)
			{
				_data[row][col] = v;
				emit dataChanged();
			}
		}
			

	}

	void ParameterTable::setValue(int row, int col, QString v)
	{
		if (row < _rowCount && col < _columnCount)
		{
			QString  c = _stringData[row][col];
			if (c!= v)
			{
				_stringData[row][col] = v;
				emit dataChanged();
			}
		}
	}

	QString ParameterTable::getValue(int row, int col)
	{
		QString v;
		if (row < _rowCount && col < _columnCount)
		{
			if (_dataType == DoubleNumber)
				v = QString::number(_data[row][col]);
			else if (_dataType == String)
				v = _stringData[row][col];
		}
		return v;
	}

	void ParameterTable::setTitle(QStringList t)
	{
		_title = t;
	}

	QStringList ParameterTable::getTitle()
	{
		return _title;
	}
	void ParameterTable::writeParameter(QDomDocument* doc, QDomElement* parent)
	{
		ParameterBase::writeParameter(doc, parent);

		QString stype = "DoubleNumber";
		if (_dataType == String)
			stype = "String";
		QDomElement stypeele = doc->createElement("DataType");
		QDomText tyText = doc->createTextNode(stype);
		stypeele.appendChild(tyText);
		parent->appendChild(stypeele);

		QString stitle;
		for (int i = 0; i < _title.size(); ++i)
		{
			stitle.append(_title.at(i));
			stitle.append(";");
		}
		stitle.resize(stitle.size() - 1);
		QDomElement titele = doc->createElement("Title");
		QDomText tText = doc->createTextNode(stitle);
		titele.appendChild(tText);
		parent->appendChild(titele);

		QDomElement nuele = doc->createElement("Number");
		QString snu = QString("%1,%2").arg(_rowCount).arg(_columnCount);
		QDomText numText = doc->createTextNode(snu);
		nuele.appendChild(numText);
		parent->appendChild(nuele);

		for (int i = 0; i < _rowCount; ++i)
		{
			QStringList data = this->getRow(i);
			QString sdata = data.join(",");
			
			QDomElement dataele = doc->createElement("Data");
			QDomText domtext = doc->createTextNode(sdata);
			dataele.appendChild(domtext);
			parent->appendChild(dataele);
		}

	}
	void ParameterTable::readParameter(QDomElement* e)
	{
		ParameterBase::readParameter(e);
		
		QDomNodeList typeList = e->elementsByTagName("DataType");
		if (typeList.size() == 1)
		{
			QString s = typeList.at(0).toElement().text();
			if (s == "String")
				_dataType = String;
		}

		QDomNodeList titleList = e->elementsByTagName("Title");
		if (titleList.size() == 1)
		{
			QString s = titleList.at(0).toElement().text();
			_title = s.split(";");
		}
		QDomNodeList numList = e->elementsByTagName("Number");
		if (numList.size() == 1)
		{
			QStringList slnum = numList.at(0).toElement().text().split(",");
			if (slnum.size() == 2)
			{
				_rowCount = slnum[0].toInt();
				_columnCount = slnum[1].toInt();
			}
			this->resizeData();
		}
		QDomNodeList datalist = e->elementsByTagName("Data");
		for (int i = 0; i < datalist.size(); ++i)
		{
			QString s = datalist.at(i).toElement().text();
			QStringList datalist = s.split(",");
			for (int j = 0; j < datalist.size(); ++j)
			{
				QString sd = datalist.at(j);
				if (_dataType == String)
					this->setValue(i, j, sd);
				else if (_dataType == DoubleNumber)
					this->setValue(i, j, sd.toDouble());
			}
		}
	}
	std::vector<std::vector<double>> ParameterTable::getData()
	{
		return _data;
	}

	std::vector<std::vector<QString>> ParameterTable::getStringData()
	{
		return _stringData;
	}

	void ParameterTable::copy(ParameterBase* ori, bool valueOnly)
	{
		ParameterBase::copy(ori, valueOnly);
		ParameterTable* p = dynamic_cast<ParameterTable*>(ori);

		_rowCount = p->getRowCount();
		_columnCount = p->getColumnCount();
		_title = p->getTitle();
		_dataType = p->getTableDataType();

		this->resizeData();

		_data = p->getData();
		_stringData = p->getStringData();
	}

	void ParameterTable::setDataType(TableDataType t)
	{
		_dataType = t;
	}

	DataProperty::TableDataType ParameterTable::getTableDataType()
	{
		return _dataType;
	}

	void ParameterTable::setData(std::vector<std::vector<double>> data)
	{
		_data = data;
		_rowCount = data.size();
		if (_rowCount > 0)
		{
			_columnCount = _data[0].size();
		}
	}

	void ParameterTable::setValueFromString(QString v)
	{
		QStringList infoList = v.split(";");
		if (infoList.size() != 3) return;

		QStringList rcInfo = infoList.at(0).split(",");
		if (rcInfo.size() != 2) return;
		_rowCount = rcInfo[0].toInt();
		_columnCount = rcInfo[1].toInt();
		setRowCount(_rowCount);
		setColumnCount(_columnCount);

		_title = infoList.at(1).split(",");

		QStringList dataInfo = infoList.at(2).split("}");
		const int rn = dataInfo.size();
		for (int i = 0; i < rn; ++i)
		{
			QString text = dataInfo.at(i);
			text.remove("{").simplified();
			QStringList rowtext = text.split(",");
			const int cn = rowtext.size();
			for (int j = 0; j < cn; ++j)
			{
				QString sd = rowtext.at(j);
				if(_dataType ==  String)
					this->setValue(i, j, sd);
				else if(_dataType == DoubleNumber)
					this->setValue(i, j, sd.toDouble());
			}

		}


	}

}
