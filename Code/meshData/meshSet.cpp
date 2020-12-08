#include "meshSet.h"
//#include "setMember.h"
#include "meshSingleton.h"
#include "meshKernal.h"
#include <QDomElement>
#include <QDataStream>
#include <vtkIdTypeArray.h>
#include <vtkAppendFilter.h>
#include <vtkUnstructuredGrid.h>
#include <vtkDataSet.h>
#include <vtkSelectionNode.h>
#include <vtkSmartPointer.h>
#include <vtkSelection.h>
#include <vtkExtractSelection.h>
#include <vtkAppendFilter.h>
#include <QDebug>

namespace MeshData
{
	//int MeshSet::maxID = 0;

	MeshSet::MeshSet(QString name, SetType t) : ComponentBase(DataProperty::ComponentType::MESH)
	{
//		maxID++;
//		setID(maxID);
		setName(name);
		setType(t);
//		_member = new SetMember;
	}

	MeshSet::MeshSet() : ComponentBase(DataProperty::ComponentType::MESH)
	{
//		maxID++;
//		setID(maxID);
//		setName(name);
//		_member = new SetMember;
	}

	MeshSet::~MeshSet()
	{
//		if (_member != nullptr) delete _member;
		if (_displayDataSet != nullptr) _displayDataSet->Delete();
	}

// 	void MeshSet::setID(int id)
// 	{
// 		DataBase::setID(id);
// 		if (maxID < id)
// 			maxID = id;
// 	}

	void MeshSet::setType(SetType t)
	{
		_type = t;
		QString stype = "Node";
		switch (t)
		{
		case Element: stype = "Element"; break;
		case Family: stype = "Family"; break;
		case BCZone: stype = "BCZone"; break;
		default: break;
		}
		this->appendProperty("Type", stype);
	}

	SetType MeshSet::getSetType()
	{
		return _type;
	}

// 	int MeshSet::getMaxID()
// 	{
// 		return maxID;
// 	}
// 
// 	void MeshSet::resetMaxID()
// 	{
// 		maxID = 0;
// 	}

	void MeshSet::appendMember(int ker, int id)
	{
//		if (_members.contains(ker, id)) return;
		_members.insert(ker, id);
	}

	QList<int> MeshSet::getKernals()
	{
		return _members.uniqueKeys();
	}

	QList<int> MeshSet::getKernalMembers(int k)
	{
		if (_members.contains(k))
			return _members.values(k);
		return QList<int>();
	}

	int MeshSet::getAllCount()
	{
		return _members.size();
	}

	QDomElement& MeshSet::writeToProjectFile(QDomDocument* doc, QDomElement* parent)
	{
		QDomElement setEle = doc->createElement("MeshSet");

		QDomAttr idAttr = doc->createAttribute("ID");
		idAttr.setValue(QString::number(_id));
		setEle.setAttributeNode(idAttr);
		QDomAttr name = doc->createAttribute("Name");
		name.setValue(_name);
		setEle.setAttributeNode(name);
		QString stype = "Node";
		if (_type == Element) stype = "Element";
		QDomAttr typeAttr = doc->createAttribute("Type");
		typeAttr.setValue(stype);
		setEle.setAttributeNode(typeAttr);

		QList<int> kernalids = _members.uniqueKeys();
		for (int kid : kernalids)
		{
			QDomElement kele = doc->createElement("Kernal");
			kele.setAttribute("ID", kid);
			QDomElement memle = doc->createElement("Member");
			QList<int> memids = _members.values(kid);
			QStringList text;
			for (int id : memids) 
				text.append(QString::number(id));
			QDomText memText = doc->createTextNode(text.join(","));
			memle.appendChild(memText);

			kele.appendChild(memle);
			setEle.appendChild(kele);
		}

		parent->appendChild(setEle);
		return setEle;
	}
	
	void MeshSet::readDataFromProjectFile(QDomElement* setEle)
	{
		QString name = setEle->attribute("Name");
		QString stype = setEle->attribute("Type");
		QString sID = setEle->attribute("ID");
		SetType type = None;
		if (stype.toLower() == "node") type = Node;
		else if (stype.toLower() == "element") type = Element;
		this->setID(sID.toInt());
		this->setName(name);
		this->setType(type);

		QDomNodeList kerList = setEle->elementsByTagName("Kernal");
		for (int i = 0; i < kerList.size(); ++i)
		{
			QDomElement kerele = kerList.at(i).toElement();
			QString skid = kerele.attribute("ID");
			bool ok = false;
			const int kid = skid.toInt(&ok);
			if (!ok) continue;
			QDomNodeList memList = kerele.elementsByTagName("Member");
			for (int j = 0; j < memList.size(); ++j)
			{
				QDomElement memele = memList.at(j).toElement();
				QString text = memele.text();
				QStringList sids = text.split(",");
				for (QString s : sids)
				{
					int mid = s.toInt(&ok);
					if (!ok) continue;
					this->appendMember(kid, mid);
				}
			}
		}
	}

	void MeshSet::generateDisplayDataSet()
	{
		if (_displayDataSet != nullptr) return;
		if (_members.isEmpty()) return;
		appendProperty("Count", _members.values().size());

		MeshData* meshdata = MeshData::getInstance();
		QList<int> kids = _members.uniqueKeys();


		vtkSmartPointer<vtkAppendFilter> appendFliter = vtkSmartPointer<vtkAppendFilter>::New();
		for (int kid : kids)
		{
			QList<int> member = _members.values(kid);
			auto k = meshdata->getKernalByID(kid);
			if (member.isEmpty() || k ==nullptr) continue;
			vtkDataSet* dataset = k->getMeshData();
			if (dataset  == nullptr) continue;

			vtkSmartPointer<vtkIdTypeArray> idArray = vtkSmartPointer<vtkIdTypeArray>::New();
			for (int id : member) idArray->InsertNextValue(id);

			vtkSmartPointer<vtkSelectionNode> selectionNode = vtkSmartPointer<vtkSelectionNode>::New();
			if (_type == Element)
			{
				selectionNode->SetFieldType(vtkSelectionNode::CELL);
			}
			else if (_type == Node)
			{
				selectionNode->SetFieldType(vtkSelectionNode::POINT);
			}
			selectionNode->SetContentType(vtkSelectionNode::INDICES);
			selectionNode->SetSelectionList(idArray);

			vtkSmartPointer<vtkSelection> selection = vtkSmartPointer<vtkSelection>::New();
			selection->AddNode(selectionNode);

			vtkSmartPointer<vtkExtractSelection> extractionSelection = vtkSmartPointer<vtkExtractSelection>::New();
			extractionSelection->SetInputData(0, dataset);
			extractionSelection->SetInputData(1, selection);
			extractionSelection->Update();
			appendFliter->AddInputData(extractionSelection->GetOutput());
		}
		appendFliter->Update();
		
		_displayDataSet = vtkUnstructuredGrid::New();
		_displayDataSet->DeepCopy(appendFliter->GetOutput());
	}

	vtkDataSet* MeshSet::getDisplayDataSet()
	{
		return _displayDataSet;
	}

	SetType MeshSet::stringToSettype(QString s)
	{
		SetType t = None;
		if (s == "Node") t = Node;
		else if (s == "Element") t = Element;
		else if (s == "Family") t = Family;
		else if (s == "BCZone") t = BCZone;
		
		return t;
	}
	//写set部分的二进制
	void MeshSet::writeBinaryFile(QDataStream* dataStream)
	{
		QList<int> kids = _members.uniqueKeys();
		const int nk = kids.size();
		*dataStream << (int)_type << _id << _name << nk;    //setID,setType,setName,KernalIDs

		for (int kid : kids)
		{
			QList<int> memids = _members.values(kid);
			const int n = memids.size();
			*dataStream << kid << n;
			for (int mem : memids)
				*dataStream << mem;
		}
		
	}
	//读取set部分的二进制
	void MeshSet::readBinaryFile(QDataStream* dataStream)
	{
		int SetID{ 0 }, nKernal{ 0 }, nMember{0};         //setID,KernalID,MemberNumber
		QString setName;
		
		*dataStream >> SetID >> setName >> nKernal;
		this->setID(SetID);
		this->setName(setName);

		for (int nk = 0; nk < nKernal; ++nk)
		{
			int kid = 0, ncount = 0 , mid = 0;
			*dataStream >> kid >>ncount;
			for (int i = 0; i < ncount; ++i)
			{
				*dataStream >> mid;
				appendMember(kid, mid);
			}
		}

	}

	void MeshSet::appendTempMem(int m)
	{
		_tempMemberID.append(m);
	}

	void MeshSet::setKeneralID(int id)
	{
		for (int m : _tempMemberID)
			this->appendMember(id, m);
		_tempMemberID.clear();
	}

	bool MeshSet::isContainsKernal(int id)
	{
		return _members.contains(id);
	}

	void MeshSet::dataToStream(QDataStream* s)
	{
		*s << _id << _name << _members.size();
	}

	void MeshSet::isVisible(bool v)
	{
		_visible = v;
	}

	bool MeshSet::isVisible()
	{
		return _visible;
	}

	void MeshSet::merge(MeshSet* set)
	{
		if (set->getSetType() != _type) return;
		QList<int> ks = set->getKernals();
		for (int k : ks)
		{
			QList<int> mem = set->getKernalMembers(k);
			for (int m : mem)
				this->appendMember(k, m);
		}
	}

	void MeshSet::cut(MeshSet* set)
	{
		if (set->getSetType() != _type) return;
		QList<int> ks = set->getKernals();
		for (int k : ks)
		{
			if (!_members.contains(k)) continue;
			QList<int> mem = set->getKernalMembers(k);
			for (int m : mem)
				_members.remove(k, m);
		}
	}

	QString MeshSet::setTypeToString(SetType type)
	{
		QString qtype{};
		switch (type)
		{
			case Node : qtype = "Node"; break;
			case Element : qtype = "Element"; break;
			case Family : qtype = "Family"; break;
			case BCZone : qtype = "BCZone"; break;
			default : break;
		}
		return qtype;
	}
}