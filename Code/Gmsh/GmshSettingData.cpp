#include "GmshSettingData.h"
#include "GmshThread.h"
#include "GmshDialogBase.h"
#include "LocalField.h"

namespace Gmsh{
	GmshSettingData::GmshSettingData()
	{
	}


	GmshSettingData::~GmshSettingData()
	{
		for (LocalDensity* ld : _locals)
		{
			delete ld;
		}


		_locals.clear();
	}

	void GmshSettingData::copy(DataBase* data)
	{
		GmshSettingData* d = dynamic_cast<GmshSettingData*>(data);
		if (d == nullptr)return;

		DataProperty::DataBase::copy(data);

		this->setSolidHash(d->getSolidHash());
		this->setSurfaceHash(d->getSurfaceHash());
		this->setSelectAll(d->getSelectAll());
		this->setSelectVisiable(d->getSelectVisiable());
		this->setElementOrder(d->getElementOrder());
		this->setElementType(d->getElementType());
		this->setMethod(d->getMethod());
		this->setSizeFactor(d->getSizeFactor());
		this->setMinSize(d->getMinSize());
		this->setMaxSize(d->getMaxSize());
		this->setGeoClean(d->getGeoClean());
		this->setGridCoplanar(d->getGridCoplanar());
		this->setSmoothIteration(d->getSmoothIteration());
		this->setLocalDesities(d->getLocalDesities());
	}

	void GmshSettingData::setSolidHash(QMultiHash<int, int> sh)
	{
		_solidHash = sh;
	}

	QMultiHash<int, int> GmshSettingData::getSolidHash()
	{
		return _solidHash;
	}

	void GmshSettingData::setSurfaceHash(QMultiHash<int, int> sh)
	{
		_surfaceHash = sh;
	}

	QMultiHash<int, int> GmshSettingData::getSurfaceHash()
	{
		return _surfaceHash;
	}

	void GmshSettingData::setElementType(QString type)
	{
		_elementType = type;
	}

	QString GmshSettingData::getElementType()
	{
		return _elementType;
	}

	void GmshSettingData::setElementOrder(int order)
	{
		_elementOrder = order;
	}

	int GmshSettingData::getElementOrder()
	{
		return _elementOrder;
	}

	void GmshSettingData::setMethod(int m)
	{
		_method = m;
	}

	int GmshSettingData::getMethod()
	{
		return _method;
	}

	void GmshSettingData::setSizeFactor(double sf)
	{
		_sizeFactor = sf;
	}

	double GmshSettingData::getSizeFactor()
	{
		return _sizeFactor;
	}

	void GmshSettingData::setMinSize(double min)
	{
		_minSize = min;
	}

	double GmshSettingData::getMinSize()
	{
		return _minSize;
	}

	void GmshSettingData::setMaxSize(double max)
	{
		_maxSize = max;
	}

	double GmshSettingData::getMaxSize()
	{
		return _maxSize;
	}

	void GmshSettingData::setGeoClean(bool c)
	{
		_geoclean = c;
	}

	bool GmshSettingData::getGeoClean()
	{
		return _geoclean;
	}

	void GmshSettingData::setSmoothIteration(int smooth)
	{
		_smoothIteration = smooth;
	}

	int GmshSettingData::getSmoothIteration()
	{
		return _smoothIteration;
	}

	void GmshSettingData::setGridCoplanar(bool gc)
	{
		_gridCoplanar = gc;
	}

	bool GmshSettingData::getGridCoplanar()
	{
		return _gridCoplanar;
	}

	void GmshSettingData::setSizeAtPoints(QString ps)
	{
		QStringList points = ps.split(";");
		for (QString s : points)
		{
			QStringList coors = s.split(",");
			if (coors.size() != 4) continue;

			bool ok = false;
			double x = coors.at(0).toDouble(&ok);
			if (!ok)continue;
			double y = coors.at(1).toDouble(&ok);
			if (!ok)continue;
			double z = coors.at(2).toDouble(&ok);
			if (!ok)continue;
			double v = coors.at(3).toDouble(&ok);
			if (!ok)continue;

			LocalPoint* lp = new LocalPoint;
			lp->_type = LocalType::PointSize;
			lp->_x = x; lp->_y = y; lp->_z = z; lp->_value = v;

			_locals.append(lp);
		}
	}


	void GmshSettingData::setSizeFields(QString fs)
	{
		QStringList solids{};
		QStringList fields = fs.split(";");
		for (QString s : fields)
		{
			QStringList field = s.split(",");

			if (field.size() <= 0) continue;

			bool ok = false;
			int t = field.at(0).toInt(&ok);
			if (!ok) continue;

			switch (t)
			{
			case 1:
				appendBoxField(field);
				break;
			case 2:
				appendBallField(field);
				break;
			case 3:
				appendCylinderField(field);
				break;
			case 4:
				solids.append(s);
				break;
			case 5:
				appendFrustumField(field);
				break;
			default:
				break;
			}
		}

		if (solids.size() > 0)
			appendSolidsField(solids);
	}

	void GmshSettingData::setLocalDesities(QList<LocalDensity*> list)
	{
		_locals = list;
	}


	QList<LocalDensity*> GmshSettingData::getLocalDesities()
	{
		return _locals;
	}

	void GmshSettingData::setSelectAll(bool b)
	{
		_selectall = b;
	}

	bool GmshSettingData::getSelectAll()
	{
		return _selectall;
	}

	void GmshSettingData::setSelectVisiable(bool b)
	{
		_selectvisible = b;
	}

	bool GmshSettingData::getSelectVisiable()
	{
		return _selectvisible;
	}

	void GmshSettingData::setMeshID(int id)
	{
		_meshID = id;
	}

	int GmshSettingData::getMeshID()
	{
		return _meshID;
	}

	void GmshSettingData::setCells(QList<int> cells)
	{
		_cells = cells;
	}

	QList<int> GmshSettingData::getCells()
	{
		return _cells;
	}

	void GmshSettingData::appendBoxField(QStringList list)
	{
		//QStringList list = s.split(",");
		const int n = list.size();
		if (n != 11)return;

		double val[9] = { 0 };
		bool ok = false;
		for (int i = 1; i < 10; i++)
		{
			double v = list.at(i).toDouble(&ok);
			if (!ok) return;
			val[i - 1] = v;
		}

		Box* b = new Box;
		b->_type = LocalType::BoxField;
		b->_thickness = val[0];
		b->_vIn = val[1];
		b->_vOut = val[2];
		b->_x = val[3];
		b->_y = val[4];
		b->_z = val[5];
		b->_length = val[6];
		b->_width = val[7];
		b->_height = val[8];
		if (list.at(10).toInt() == 1)
			b->_backgroundField = true;

		_locals.append(b);
	}

	void GmshSettingData::appendBallField(QStringList list)
	{
		const int n = list.size();
		if (n != 9)return;

		double val[7] = { 0 };
		bool ok = false;
		for (int i = 1; i < 8; i++)
		{
			double v = list.at(i).toDouble(&ok);
			if (!ok) return;
			val[i - 1] = v;
		}

		Ball* b = new Ball;
		b->_type = LocalType::BallField;
		b->_radius = val[0];
		b->_thickness = val[1];
		b->_vIn = val[2];
		b->_vOut = val[3];
		b->_x = val[4];
		b->_y = val[5];
		b->_z = val[6];

		if (list.at(8).toInt() == 1)
			b->_backgroundField = true;

		_locals.append(b);
		
	}

	void GmshSettingData::appendCylinderField(QStringList list)
	{
		const int n = list.size();
		if (n != 12)return;

		double val[10] = { 0 };
		bool ok = false;
		for (int i = 1; i < 11; i++)
		{
			double v = list.at(i).toDouble(&ok);
			if (!ok) return;
			val[i - 1] = v;
		}

		Cylinder* c = new Cylinder;
		c->_type = LocalType::CylinderField;
		c->_radius = val[0];
		c->_length = val[1];
		c->_x = val[2];
		c->_y = val[3];
		c->_z = val[4];
		c->_xAxis = val[5];
		c->_yAxis = val[6];
		c->_zAxis = val[7];
		c->_vIn = val[8];
		c->_vOut = val[9];

		if (list.at(11).toInt() == 1)
			c->_backgroundField = true;

		_locals.append(c);
	}

	void GmshSettingData::appendSolidsField(QStringList list)
	{
		QMultiHash<int, int> solidHash;
		double val[3] = { 0 };
		bool back{ false };
		for (QString s : list)
		{
			QStringList solid = s.split(",");
			const int n = solid.size();
			if (n != 7)continue;

			bool ok = false;
			val[0] = solid.at(1).toDouble(&ok);
			if (!ok)continue;
			val[1] = solid.at(2).toDouble(&ok);
			if (!ok)continue;
			val[2] = solid.at(3).toDouble(&ok);
			if (!ok)continue;
			int setid = solid.at(4).toDouble(&ok);
			if (!ok)continue;
			int sindex = solid.at(5).toDouble(&ok);
			if (!ok)continue;

			solidHash.insert(setid, sindex);

			if (solid.at(6).toInt() == 1)
				back = true;
		}

		SolidFields* solid = new SolidFields;
		solid->_type = LocalType::SolidField;
		solid->_vIn = val[0];
		solid->_vOut = val[1];
		solid->_thickness = val[2];
		solid->_solidHash = solidHash;
		solid->_backgroundField = back;

		_locals.append(solid);
	}

	void GmshSettingData::appendFrustumField(QStringList list)
	{

		const int n = list.size();
		if (n != 15)return;

		double val[13] = { 0 };
		bool ok = false;
		for (int i = 1; i < 14; i++)
		{
			double v = list.at(i).toDouble(&ok);
			if (!ok) return;
			val[i - 1] = v;
		}

		Frustum* f = new Frustum;
		f->_type = LocalType::FrustumField;
		f->_iRadius = val[0];
		f->_oRadius = val[1];
		f->_iTopRadius = val[2];
		f->_oTopRadius = val[3];
		f->_length = val[4];
		f->_x = val[5];
		f->_y = val[6];
		f->_z = val[7];
		f->_xAxis = val[8];
		f->_yAxis = val[9];
		f->_zAxis = val[10];
		f->_vIn = val[11];
		f->_vOut = val[12];

		if (list.at(14).toInt() == 1)
			f->_backgroundField = true;

		_locals.append(f);
	}

}