#include "SolverConfig.h"
#include "SolverInfo.h"
#include "MesherInfo.h"
#include <QFile>
#include <QDomElement>
#include <QDomDocument>
#include <QTextStream>

namespace ConfigOption
{
	void SolverOption::appendSolver(SolverInfo* s)
	{
		_solverlist.append(s);
	}
	SolverInfo* SolverOption::getSolverAt(int index)
	{
		SolverInfo* info = nullptr;
		if (index >= 0 && index < getSolverCount())
			info = _solverlist.at(index);
		return info;
	}
	int SolverOption::getSolverCount()
	{
		return _solverlist.count();
	}
	void SolverOption::clearData()
	{
		for (int i = 0; i < _solverlist.size(); ++i)
		{
			SolverInfo* d = _solverlist.at(i);
			delete d;
		}
		_solverlist.clear();

		for (int i = 0; i < _mesherList.size(); ++i)
		{
			MesherInfo* d = _mesherList.at(i);
			delete d;
		}
		_mesherList.clear();
	}
	bool SolverOption::read(QString f)
	{
		QFile file(f);
		if (!file.open(QFile::ReadOnly | QFile::Text)) return false;
		QDomDocument doc;
		if (!doc.setContent(&file))
		{
			file.close();
			return false;
		}
		QDomNodeList solvernodelist = doc.elementsByTagName("Solver");
		for (int i = 0; i < solvernodelist.size(); ++i)
		{
			QDomElement ele = solvernodelist.at(i).toElement();
			SolverInfo* info = new SolverInfo;
			info->readXML(&ele);
			this->appendSolver(info);
		}
		QDomNodeList meshernodelist = doc.elementsByTagName("Mesher");
		for (int i = 0; i < meshernodelist.size(); ++i)
		{
			QDomElement ele = meshernodelist.at(i).toElement();
			MesherInfo* info = new MesherInfo;
			info->readParameters(&ele);
			this->appendMesher(info);
		}
		return true;
	}


	void SolverOption::write(QString f)
	{
		QDomDocument *doc = new QDomDocument;
		QDomProcessingInstruction instruction = doc->createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
		doc->appendChild(instruction);
		QDomElement root = doc->createElement("DISO_CONFIG_FILE_1.0");
		doc->appendChild(root);

		const int n = _solverlist.size();
		for (int i = 0; i < n; ++i)
		{
			auto s = _solverlist.at(i);
			s->writeXML(doc,&root);
		}

		const int nm = _mesherList.size();
		for (int i = 0; i < nm; ++i)
		{
			auto m = _mesherList.at(i);
			m->writeParameters(doc, &root);
		}

		QFile file(f);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) return;
		QTextStream* stream = new QTextStream(&file);
		doc->save(*stream, 4);
		file.close();

		delete stream;
		delete doc;
	}



	void SolverOption::appendMesher(MesherInfo* m)
	{
		m->setIndex(_mesherList.size());
		_mesherList.append(m);
		m->generateParaInfo();
	}

	MesherInfo* SolverOption::getMesherAt(int index)
	{
		MesherInfo* m = nullptr;
		if (index >= 0 && index < _mesherList.size())
			m = _mesherList.at(index);
		return m;
	}

	int SolverOption::getMesherCount()
	{
		return _mesherList.size();
	}

	MesherInfo* SolverOption::getMesherByName(QString name)
	{
		MesherInfo* mesher = nullptr;
		for (int i = 0; i < _mesherList.size(); ++i)
		{
			MesherInfo* f = _mesherList.at(i);
			if (name == f->getMesherName())
			{
				mesher = f;
				break;
			}
		}
		return mesher;
	}

	void SolverOption::removeSolver(int index)
	{
		if (index < 0 || index >= _solverlist.size()) return;
		auto s = _solverlist.at(index);
		_solverlist.removeAt(index);
		delete s;
	}


}