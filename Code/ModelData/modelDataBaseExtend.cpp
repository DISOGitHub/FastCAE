#include "modelDataBaseExtend.h"
#include "BCBase/BCBase.h"
#include <QDomElement>
#include <assert.h>
#include "simulationSettingBase.h"
#include "solverSettingBase.h"
#include "ConfigOptions/ConfigOptions.h"
#include "ConfigOptions/DataConfig.h"
#include "ConfigOptions/PostCurve.h"
#include "ConfigOptions/PostConfig.h"
#include "ConfigOptions/PostConfigInfo.h"
#include "ConfigOptions/ObserverConfig.h"
#include "ConfigOptions/ParameterObserver.h"
#include "DataProperty/PropertyBase.h"
#include "DataProperty/PropertyString.h"
#include "DataProperty/ParameterInt.h"
#include "DataProperty/ParameterBool.h"
#include "DataProperty/ParameterDouble.h"
#include "DataProperty/ParameterSelectable.h"
#include "DataProperty/ParameterString.h"
#include "DataProperty/ParameterPath.h"
#include "DataProperty/ParameterTable.h"
#include <QDebug>

namespace ModelData
{
	ModelDataBaseExtend::ModelDataBaseExtend(ProjectTreeType type)
		: ModelDataBase(type)
	{
		ConfigOption::DataConfig* dataconfig = ConfigOption::ConfigOption::getInstance()->getDataConfig();

		QList<ConfigOption::PostCurve*> monitorCurves = dataconfig->getMonitorCurves(_treeType);
		_monitorFiles = dataconfig->getMonitorFile(_treeType);
		for (int i = 0; i < monitorCurves.size(); ++i)
		{
			ConfigOption::PostCurve* p = new ConfigOption::PostCurve;
			p->copy(monitorCurves.at(i));
			_monitorCurves.append(p);
		}
	}

	ModelDataBaseExtend::~ModelDataBaseExtend()
	{
		QList<DataProperty::DataBase*> datalist = _configData.values();
		for (int i = 0; i < datalist.size(); ++i)
		{
			DataProperty::DataBase* d = datalist.at(i);
			delete d;
		}
		_configData.clear();
		for (int i = 0; i < _observerList.size(); ++i)
		{
			auto obs = _observerList.at(i);
			delete obs;
		}
		_observerList.clear();
	}
	
	void ModelDataBaseExtend::appendReport(QString report)
	{
		_reportList.append(report);
	}

	int ModelDataBaseExtend::getReportCount()
	{
		return _reportList.size();
	}

	void ModelDataBaseExtend::removeReportAt(int index)
	{
		assert(index >= 0 && index < _reportList.size());
		_reportList.removeAt(index);
	}

	QString ModelDataBaseExtend::getReportAt(int index)
	{
		assert(index >= 0 && index < _reportList.size());
		return _reportList.at(index);
	}

	QDomElement& ModelDataBaseExtend::writeToProjectFile(QDomDocument* doc, QDomElement* e)
	{
		QDomElement element = ModelDataBase::writeToProjectFile(doc, e);
		QDomElement materialele = doc->createElement("Material");
		QList<int> setidlist = _setMaterial.keys();
		for (int i = 0; i < setidlist.size(); ++i)
		{
			int setid = setidlist.at(i);
			int materialid = _setMaterial.value(setid);
			QDomElement mc = doc->createElement("MaterialInfo");
			QDomAttr setattr = doc->createAttribute("ComponentID");
			QDomAttr materialattr = doc->createAttribute("MaterialID");
			setattr.setValue(QString::number(setid));
			materialattr.setValue(QString::number(materialid));
			mc.setAttributeNode(setattr);
			mc.setAttributeNode(materialattr);
			materialele.appendChild(mc);
		}
		element.appendChild(materialele);

		QDomElement configele = doc->createElement("ConfigData");
		QList<DataProperty::DataBase*> datalist = _configData.values();
		for (int i = 0; i < datalist.size(); ++i)
		{
			DataProperty::DataBase* d = datalist.at(i);
			const int id = _configData.key(d);
			QDomElement dataele = doc->createElement("Data");
			dataele.setAttribute("ID", id);
// 			QString name = d->getName();
// 			dataele.setAttribute("TreeNode", name);
			d->writeParameters(doc, &dataele);
			configele.appendChild(dataele);
		}
		element.appendChild(configele);
		
		QDomElement ele = doc->createElement("Report");
		for (int i = 0; i < _reportList.size(); ++i)
		{
			QDomElement textele = doc->createElement("Path");
			QString text = _reportList.at(i);
			QDomText textdom = doc->createTextNode(text);
			textele.appendChild(textdom);
			ele.appendChild(textele);
		}
		element.appendChild(ele);

		return element;
	}

	void ModelDataBaseExtend::writeToProjectFile1(QDomDocument* doc, QDomElement* e)
	{
		ModelDataBase::writeToProjectFile1(doc, e);

		QDomElement materialele = doc->createElement("Material");
		QList<int> setidlist = _setMaterial.keys();
		for (int i = 0; i < setidlist.size(); ++i)
		{
			int setid = setidlist.at(i);
			int materialid = _setMaterial.value(setid);
			QDomElement mc = doc->createElement("MaterialInfo");
			QDomAttr setattr = doc->createAttribute("ComponentID");
			QDomAttr materialattr = doc->createAttribute("MaterialID");
			setattr.setValue(QString::number(setid));
			materialattr.setValue(QString::number(materialid));
			mc.setAttributeNode(setattr);
			mc.setAttributeNode(materialattr);
			materialele.appendChild(mc);
		}
		e->appendChild(materialele);

		QDomElement configele = doc->createElement("ConfigData");
		QList<DataProperty::DataBase*> datalist = _configData.values();
		for (int i = 0; i < datalist.size(); ++i)
		{
			DataProperty::DataBase* d = datalist.at(i);
			const int id = _configData.key(d);
			QDomElement dataele = doc->createElement("Data");
			dataele.setAttribute("ID", id);
			d->writeParameters(doc, &dataele);
			configele.appendChild(dataele);
		}
		e->appendChild(configele);

		QDomElement ele = doc->createElement("Report");
		for (int i = 0; i < _reportList.size(); ++i)
		{
			QDomElement textele = doc->createElement("Path");
			QString text = _reportList.at(i);
			QDomText textdom = doc->createTextNode(text);
			textele.appendChild(textdom);
			ele.appendChild(textele);
		}
		e->appendChild(ele);
	}

	void ModelDataBaseExtend::readDataFromProjectFile(QDomElement* e)
	{
		ModelDataBase::readDataFromProjectFile(e);
		QDomNodeList materialList = e->elementsByTagName("MaterialInfo");
		for (int i = 0; i < materialList.size(); ++i)
		{
			QDomElement ele = materialList.at(i).toElement();
			QString ssetid = ele.attribute("ComponentID");
			QString smaterialID = ele.attribute("MaterialID");
			int setid = ssetid.toInt();
			int maid = smaterialID.toInt();
			this->setMaterial(setid, maid);
		}

		QDomNodeList configdata = e->elementsByTagName("ConfigData");
		if (configdata.size() == 1)
		{
			QDomElement cele = configdata.at(0).toElement();
			QDomNodeList datalist = cele.elementsByTagName("Data");
			for (int i = 0; i < datalist.size(); ++i)
			{
				QDomElement ele = datalist.at(i).toElement();
				QString sid = ele.attribute("ID");
//				QString sname = ele.attribute("TreeNode");
				const int id = sid.toInt();
				DataProperty::DataBase* d = new DataProperty::DataBase;
				d->setModuleType(DataProperty::Module_Model);
				d->setID(_id);
//				d->setName(sname);
				d->readParameters(&ele);
				if (_configData.contains(id))
				{
					DataProperty::DataBase* dd = _configData.value(id);
					if (dd != nullptr) delete dd;
				}
				_configData[id] = d;
			}

		}
		QDomNodeList reportNodelist = e->elementsByTagName("Report");
		if (reportNodelist.size() == 1)
		{
			QDomElement element = reportNodelist.at(0).toElement();
			QDomNodeList pathlist = element.elementsByTagName("Path");
			const int n = pathlist.size();
			for (int i = 0; i < n; ++i)
			{
				QDomElement ele = pathlist.at(i).toElement();
				QString p = ele.text();
				_reportList.append(p);
			}
		}
		this->registerObserver();
	}

	void ModelDataBaseExtend::writeToSolverXML(QDomDocument* doc, QDomElement* e)
	{
		this->writeToProjectFile(doc, e);
	}

	void ModelDataBaseExtend::setMaterial(int setID, int materialID)	
	{
		if (!_ComponentIDList.contains(setID) || materialID <= 0) return;
		_setMaterial[setID] = materialID;
	}

	int ModelDataBaseExtend::getMaterialID(int setid)
	{
		int m = -1;
		if (_setMaterial.contains(setid))
		{
			m = _setMaterial.value(setid);
		}
		return m;
	}

	bool ModelDataBaseExtend::isMaterialSetted(int setid)
	{
		bool s = false;
		if (_setMaterial.contains(setid))
		{
			if (_setMaterial.value(setid) >= 0)
				s = true;
		}
		return s;
	}

	void ModelDataBaseExtend::removeMaterial(int setid)
	{
		if (_setMaterial.contains(setid))
		{
			_setMaterial.remove(setid);
		}
	}

	void ModelDataBaseExtend::setComponentIDList(QList<int> ids)
	{
		QList<int> old = _ComponentIDList;
		QList<int> removeid;
		for (auto id : old)
			if (!ids.contains(id)) removeid.append(id);

		for (int id : removeid)
		{
			if (_setMaterial.contains(id))
				_setMaterial.remove(id);
			for (auto bc : _bcList)
			{
				if (bc->getComponentID() == id)
				{
					_bcList.removeOne(bc);
					delete bc;
					break;
				}
			}
		}
		ModelDataBase::setComponentIDList(ids);
	}

	void ModelDataBaseExtend::removeComponentAt(int index)
	{
		assert(index >= 0 && index < _ComponentIDList.size());
		int id = _ComponentIDList.at(index);
		if (isMaterialSetted(id))
		{
			removeMaterial(id);
		}
		ModelDataBase::removeComponentAt(index);
	}

	void ModelDataBaseExtend::copyFormConfig()
	{
		ConfigOption::DataConfig* dataconfig = ConfigOption::ConfigOption::getInstance()->getDataConfig();
		QHash<int, DataProperty::DataBase*> configList = dataconfig->getConfigData(_treeType);
		QList<int> ids = configList.keys();
		for (int i = 0; i < ids.size(); ++i)
		{
			const int id = ids.at(i);
			DataProperty::DataBase* d = configList.value(id);
			DataProperty::DataBase* newdata = new DataProperty::DataBase;
			newdata->setModuleType(DataProperty::Module_Model);
			newdata->copy(d);
			newdata->setID(_id);
			_configData[id] = newdata;
		}
		_monitorFiles = dataconfig->getMonitorFile(_treeType);
//		_monitorCurves = dataconfig->getMonitorCurves(_treeType);
		ModelDataBase::copyFormConfig();
		this->registerObserver();
		
	}

	QList<ConfigOption::PostCurve*> ModelDataBaseExtend::getMonitorCurves()
	{
		return _monitorCurves;
	}

	ConfigOption::PostCurve* ModelDataBaseExtend::getMonitorCurveAt(const int index)
	{
		assert(index >= 0 && index < _monitorCurves.size());
		return _monitorCurves.at(index);
	}
		
	void ModelDataBaseExtend::appendConfigData(int dataID, DataProperty::DataBase* data)
	{
		if (data == nullptr) return;
		if (_configData.contains(dataID))
		{
			DataProperty::DataBase* d = _configData.value(dataID);
			delete d;
			_configData.remove(dataID);
		}
		_configData[dataID] = data;
	}

	DataProperty::DataBase* ModelDataBaseExtend::getConfigData(int dataID)
	{
		return _configData.value(dataID);
	}

	int ModelDataBaseExtend::getConfigDataCount()
	{
		return _configData.count();
	}

// 	QString ModelDataBaseExtend::getMonitorList()
// 	{
// 		return _monitor;
// 	}
	QStringList ModelDataBaseExtend::getMonitorFile()
	{
		return _monitorFiles;
	}

	QStringList ModelDataBaseExtend::getAbsoluteMonitorFile()
	{
		QStringList s;
		QString path = this->getPath();
		for (int i = 0; i < _monitorFiles.size(); ++i)
		{
			QString ss = path + "/MonitorFiles/" + _monitorFiles.at(i);
			s.append(ss);
		}
		return s;
	}

	QStringList ModelDataBaseExtend::getMonitorVariables(QString f)
	{
		QStringList v;
		for (int i = 0; i < _monitorCurves.size(); ++i)
		{
			ConfigOption::PostCurve* c = _monitorCurves.at(i);
			QString file = c->getFile();
			if (!f.endsWith(file)) continue;
			QString x = c->getXVariable();
			QString y = c->getYVariable();
			if (!v.contains(x)) v.append(x);
			if (!v.contains(y)) v.append(y);
		}
		return v;
	}

	QStringList ModelDataBaseExtend::getPost2DFiles()
	{
		ConfigOption::PostConfigInfo* info = ConfigOption::ConfigOption::getInstance()->getPostConfig()->getPostConfigInfo(_treeType);
		QStringList s;
		if (info != nullptr)
			s = info->getPost2DFile();
		return s;
	}

	QStringList ModelDataBaseExtend::getAbsolutePost2DFiles()
	{
		QStringList s = this->getPost2DFiles();
		QStringList f;
		QString path = this->getPath() + "/Result/";
		for (int i = 0; i < s.size(); ++i)
		{
			f.append(path + s.at(i));
		}
		return f;
	}

	QStringList ModelDataBaseExtend::getPost2DVariables(QString f)
	{
		ConfigOption::PostConfigInfo* info = ConfigOption::ConfigOption::getInstance()->getPostConfig()->getPostConfigInfo(_treeType);
		QStringList s;
		if (info != nullptr)
			s = info->get2DVariables(f);
		return s;
	}

	QString ModelDataBaseExtend::getPost3DFile()
	{
		ConfigOption::PostConfigInfo* info = ConfigOption::ConfigOption::getInstance()->getPostConfig()->getPostConfigInfo(_treeType);
		if (info != nullptr)
			return info->getPost3DFile();
		return "";
	}

	void ModelDataBaseExtend::get3DScalars(QStringList &node, QStringList &ele)
	{
		ConfigOption::PostConfigInfo* info = ConfigOption::ConfigOption::getInstance()->getPostConfig()->getPostConfigInfo(_treeType);
		if (info != nullptr)
		{
			node = info->getNodeScalarVariable();
			ele = info->getCellScalarVariable();
		}
	}

	void ModelDataBaseExtend::get3DVector(QStringList &node, QStringList &ele)
	{
		ConfigOption::PostConfigInfo* info = ConfigOption::ConfigOption::getInstance()->getPostConfig()->getPostConfigInfo(_treeType);
		if (info != nullptr)
		{
			node = info->getNodeVectorVariable();
			ele = info->getCellVectorVariable();
		}
	}

	void ModelDataBaseExtend::appendScalarVariable(QString v)
	{
		_scalarVariable.append(v);
	}

	void ModelDataBaseExtend::removeScalarVariable(int index)
	{
		if (index < 0 || index >= _scalarVariable.size()) return;
		_scalarVariable.removeAt(index);
	}

	QStringList ModelDataBaseExtend::getScalarVariable()
	{
		return _scalarVariable;
	}

	void ModelDataBaseExtend::appendVectorVariable(QString v)
	{
		_vectorVariable.append(v);
	}

	void ModelDataBaseExtend::removeVectorVariable(int index)
	{
		if (index < 0 || index >= _vectorVariable.size()) return;
		_vectorVariable.removeAt(index);
	}

	QStringList ModelDataBaseExtend::getVectorVariable()
	{
		return _vectorVariable;
	}

	void ModelDataBaseExtend::apppendPlotCurve(ConfigOption::PostCurve* c)
	{
		_postCurves.append(c);
	}

	QList<ConfigOption::PostCurve*> ModelDataBaseExtend::getPlotCurves()
	{
		return _postCurves;
	}

	void ModelDataBaseExtend::removePlotCurve(int index)
	{
		_postCurves.removeAt(index);
	}

	bool ModelDataBaseExtend::isPostCurveExist(QString name)
	{
		for (int i = 0; i < _postCurves.size(); ++i)
		{
			ConfigOption::PostCurve* c = _postCurves.at(i);
			QString n = c->getDescribe();
			if (n == name) return true;
		}
		return false;
	}

	void ModelDataBaseExtend::clearPlotCurve()
	{
		for (int i = 0; i < _postCurves.size(); ++i)
		{
			ConfigOption::PostCurve* c = _postCurves.at(i);
			delete c;
		}
		_postCurves.clear();
	}

	void ModelDataBaseExtend::clear3DVariable()
	{
		_scalarVariable.clear();
		_vectorVariable.clear();
	}

	DataProperty::ParameterBase* ModelDataBaseExtend::getParameterByName(QString name)
	{
		DataProperty::ParameterBase* P = nullptr;
		P = ModelDataBase::getParameterByName(name);
		if (P != nullptr) return P;
		QList<DataProperty::DataBase*> dataList = _configData.values();
		for (int i = 0; i < dataList.size(); ++i)
		{
			auto d = dataList.at(i);
			P = d->getParameterByName(name);
			if (P != nullptr)
				break;
		}
		return P;
	}

	void ModelDataBaseExtend::removeParameter(DataProperty::ParameterBase* p)
	{
		QList<DataProperty::DataBase*> dataList = _configData.values();
		for (int i = 0; i < dataList.size(); ++i)
		{
			auto d = dataList.at(i);
			d->removeParameter(p);
		}
		ModelDataBase::removeParameter(p);
	}

	void ModelDataBaseExtend::removeParameterGroup(DataProperty::ParameterGroup* g)
	{

		QList<DataProperty::DataBase*> dataList = _configData.values();
		for (int i = 0; i < dataList.size(); ++i)
		{
			auto d = dataList.at(i);
			d->removeParameterGroup(g);
		}
		ModelDataBase::removeParameterGroup(g);
	}

	DataProperty::ParameterGroup* ModelDataBaseExtend::getParameterGroupByName(QString name)
	{
		DataProperty::ParameterGroup* g = nullptr;
		g = ModelDataBase::getParameterGroupByName(name);
		if (g != nullptr) return g;
		QList<DataProperty::DataBase*> dataList = _configData.values();
		for (int i = 0; i < dataList.size(); ++i)
		{
			auto d = dataList.at(i);
			g = d->getParameterGroupByName(name);
			if (g != nullptr)
				break;
		}
		return g;
	}

	void ModelDataBaseExtend::registerObserver()
	{
		QList<ConfigOption::ParameterObserver*> obslist = ConfigOption::ConfigOption::getInstance()->getObseverConfig()->getObserverList(_treeType);
		const int n = obslist.size();
		for (int i = 0; i < n; ++i)
		{
			ConfigOption::ParameterObserver* obs = obslist.at(i)->copy();
			if (obs == nullptr) continue;
			_observerList.append(obs);
			QStringList activePara = obs->getActiveParameterNames();
			for (int i = 0; i < activePara.size(); ++i)
			{
				QString name = activePara.at(i);
//				qDebug() << name;
				auto para = this->getParameterByName(name);
				if (para == nullptr)
				{
					obs->removeConfigActive(name);
				}
				else
				{
					obs->appendDataActive(name, para);
					para->appendObserver(obs);
				}
					
			}
			QStringList followPara = obs->getFollowParameterNames();
			for (int i = 0; i < followPara.size(); ++i)
			{
				QString name = followPara.at(i);
				auto para = this->getParameterByName(name);
				if (para == nullptr)
				{
					obs->removeConfigFollow(name);
				}
				else
					obs->appendDataFollow(name, para);
			}
			QStringList followgroup = obs->getFollowGroupNames();
			for (int i = 0; i < followgroup.size(); ++i)
			{
				QString name = followgroup.at(i);
				auto para = this->getParameterGroupByName(name);
				if (para == nullptr)
				{
					obs->removeConfigFollow(name);
				}
				else
					obs->appendDataFollowGroup(name, para);
			}

			obs->observe();
		}
	}

	void ModelDataBaseExtend::dataToStream(QDataStream* datas){
		*datas << _name << _id << _treeType; 
		QList<DataProperty::DataBase*> datalist = _configData.values();
		for (int i = 0; i < datalist.count();++i)
		{
			DataProperty::DataBase* data = datalist.at(i);
			data->dataToStream(datas);
		}
	}

	void ModelDataBaseExtend::generateParaInfo()
	{
		DataBase::generateParaInfo();
		QList<DataProperty::DataBase*> datalist = _configData.values();
		for (int i = 0; i < datalist.size(); ++i)
		{
			DataProperty::DataBase* data = datalist.at(i);
			data->generateParaInfo();
		}
		ModelDataBase::generateParaInfo();
	}

	void ModelDataBaseExtend::setPost2DWindow(Post::Post2DWindowInterface* p2d)
	{
		_post2DWindow = p2d;
	}

	Post::Post2DWindowInterface* ModelDataBaseExtend::getPost2DWindow()
	{
		return _post2DWindow;
	}

	void ModelDataBaseExtend::setPost3DWindow(Post::Post3DWindowInterface* p3d)
	{
		_post3DWindow = p3d;
	}

	Post::Post3DWindowInterface* ModelDataBaseExtend::getPost3DWindow()
	{
		return _post3DWindow;
	}

	void ModelDataBaseExtend::bindInpMaterialIds(const QList<int>& inpMaterIds)
	{
		_inpMaterIds = inpMaterIds;
	}

	const QList<int>& ModelDataBaseExtend::getInpMaterialIds()
	{
		return _inpMaterIds;
	}
}