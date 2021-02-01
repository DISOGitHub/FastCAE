#include "ParameterTable.h"
#include <QDomText>
#include <QDomElement>
#include <QDomDocument>
#include <QDomNodeList>
#include <assert.h>
#include <math.h>
#include <QDebug>

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
		{
			_data[i].resize(_columnCount);
		}
	}

	QList<double> ParameterTable::getRow(int index)
	{
		QList<double> row;
		if (index < _rowCount)
		{
			std::vector<double> r = _data.at(index);
			for (int i = 0; i < (int)r.size(); ++i)
				row.append(r.at(i));
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
			if (fabs(c - v) > 0.0000001)
			{
				_data[row][col] = v;
				emit dataChanged();
			}
		}


	}

	double ParameterTable::getValue(int row, int col)
	{
		double v = 0.0;
		if (row < _rowCount && col < _columnCount)
			v = _data[row][col];
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
			QList<double> data = this->getRow(i);
			QString sdata;
			for (int i = 0; i < data.size(); ++i)
			{
				sdata.append(QString::number(data.at(i)));
				sdata.append(",");
			}
			sdata.resize(sdata.size() - 1);
			QDomElement dataele = doc->createElement("Data");
			QDomText domtext = doc->createTextNode(sdata);
			dataele.appendChild(domtext);
			parent->appendChild(dataele);
		}

	}
	void ParameterTable::readParameter(QDomElement* e)
	{
		ParameterBase::readParameter(e);
		//       qDebug() << _describe;
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
				double d = datalist.at(j).toDouble();
				setValue(i, j, d);
			}
		}
	}
	std::vector<std::vector<double>> ParameterTable::getData()
	{
		return _data;
	}

	void ParameterTable::copy(ParameterBase* ori, bool valueOnly)
	{
		ParameterBase::copy(ori, valueOnly);
		ParameterTable* p = dynamic_cast<ParameterTable*>(ori);

		_rowCount = p->getRowCount();
		_columnCount = p->getColumnCount();
		_title = p->getTitle();

		this->resizeData();

		_data = p->getData();
	}

	void ParameterTable::setData(std::vector<std::vector<double>> data)
	{
		_data = data;
		_rowCount = (int)data.size();
		if (_rowCount > 0)
		{
			_columnCount = (int)_data[0].size();
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
				double d = rowtext.at(j).toDouble();
				this->setValue(i, j, d);
			}

		}


	}

}
