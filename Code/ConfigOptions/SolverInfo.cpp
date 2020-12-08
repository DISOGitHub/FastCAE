#include "SolverInfo.h"
#include <QDomDocument>
#include <QDomElement>
#include <QDomAttr>
#include <QFile>
#include <QCoreApplication>

namespace ConfigOption
{
	void SolverInfo::setName(QString n)
	{
		_name = n;
	}
	QString SolverInfo::getName()
	{
		return _name;
	}
	void SolverInfo::setParameter(QString p)
	{
		_parameter = p;
	}
	QString SolverInfo::getParameter()
	{
		return _parameter;
	}
	void SolverInfo::setProcessKeyWord(QString p)
	{
		_process = p;
	}
	QString SolverInfo::getProcessKeyWord()
	{
		return _process;
	}

	void SolverInfo::setEXEPath(QString path)
	{
		_exePath = path;
	}
	
	QString SolverInfo::getExePath()
	{
		return _exePath;
	}

	void SolverInfo::setType(SolverType t)
	{
		_type = t;
	}

	SolverType SolverInfo::getType()
	{
		return _type;
	}

	void SolverInfo::setTemplate(QString tem)
	{
		_template = tem; 
	}

	QString SolverInfo::getTemplate()
	{
		return _template;
	}

	void SolverInfo::setInputFormat(QString f)
	{
		_inputFormat = f;
	}

	QString SolverInfo::getInputFormat()
	{
		return _inputFormat;
	}

	void SolverInfo::setTransfer(QString t)
	{
		_transfer = t;
	}

	QString SolverInfo::getTransfer()
	{
		return _transfer;
	}

	void SolverInfo::readXML(QDomElement *ele)
	{
		QString stype = ele->attribute("Type");
		_type = SelfDevelop;
		if (stype.toLower() == "thirdpart") _type = ThirdParty;
		_name = ele->attribute("Name");
		_inputFormat = ele->attribute("InpFormat");
		_transfer = ele->attribute("Transfer");

		QDomNodeList patheles = ele->elementsByTagName("Path");
		if (patheles.size() > 1) return;
		QDomElement pathele = patheles.at(0).toElement();
		QString path = pathele.text();
		QFile exefile(path);
		if (exefile.exists())
			_exePath = path;
		else
		{
			QString fielname = exefile.fileName();
			QString appDir = qApp->applicationDirPath();
			QString solvedirFile = appDir + "/../Solver/" + fielname;
			if (QFile::exists(solvedirFile))
				_exePath = solvedirFile;
			else
				_exePath.clear();
		}

		QDomNodeList argeles = ele->elementsByTagName("Argument");
		if (argeles.size() > 1) return;
		QDomElement argele = argeles.at(0).toElement();
		_parameter = argele.text();

		QDomNodeList proeles = ele->elementsByTagName("ProcessKey");
		if (proeles.size() > 1) return;
		QDomElement proele = proeles.at(0).toElement();
		_process = proele.text();

		QDomNodeList temeles = ele->elementsByTagName("Template");
		if (temeles.size() > 1) return;
		QDomElement temele = temeles.at(0).toElement();
		QString temp = temele.text();
		if (!temp.isEmpty())
		{
			QFile tempfile(temp);
			if (tempfile.exists())
				_template = temp;
			else
			{
				QString fielname = tempfile.fileName();
				QString appDir = qApp->applicationDirPath();
				QString solvedirFile = appDir + "/../Solver/" + fielname;
				if (QFile::exists(solvedirFile))
					_template = solvedirFile;
				else
					_template.clear();
			}

		}
		else
			_template.clear();
		QDomNodeList depeles = ele->elementsByTagName("Dependence");
		if (depeles.size() > 1) return;
		QDomElement depele = depeles.at(0).toElement();
		QString temptext = depele.text();
		_dependences = temptext.split(",");
	}

	void SolverInfo::writeXML(QDomDocument* doc, QDomElement *parent)
	{
		QDomElement domele = doc->createElement("Solver");

		QString stype = "SelfDev";
		if (_type == ThirdParty) stype = "ThirdPart";
		domele.setAttribute("Type", stype);
		domele.setAttribute("Name", _name);
		domele.setAttribute("InpFormat", _inputFormat);
		domele.setAttribute("Transfer", _transfer);

		QDomElement pathEle = doc->createElement("Path");
		QDomText pathText = doc->createTextNode(_exePath);
		pathEle.appendChild(pathText);
		domele.appendChild(pathEle);

		QDomElement argEle = doc->createElement("Argument");
		QDomText  argText = doc->createTextNode(_parameter);
		argEle.appendChild(argText);
		domele.appendChild(argEle);

		QDomElement proEle = doc->createElement("ProcessKey");
		QDomText protext = doc->createTextNode(_process);
		proEle.appendChild(protext);
		domele.appendChild(proEle);

		QDomElement temEle = doc->createElement("Template");
		QDomText temText = doc->createTextNode(_template);
		temEle.appendChild(temText);
		domele.appendChild(temEle);

		QDomElement depele = doc->createElement("Dependence");
		QDomText depText = doc->createTextNode(_dependences.join(","));
		depele.appendChild(depText);
		domele.appendChild(depele);

		parent->appendChild(domele);
	}

	bool SolverInfo::isWriteTemplate()
	{
		return  !_template.isEmpty();
	}

	void SolverInfo::setDependences(QStringList de)
	{
		_dependences = de;
	}

	QStringList SolverInfo::getDependences()
	{
		return _dependences;
	}


}