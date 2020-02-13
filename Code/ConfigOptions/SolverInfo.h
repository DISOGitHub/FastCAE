#ifndef _SOLVERINFO_H_
#define _SOLVERINFO_H_

#include "ConfigOptionsAPI.h"
#include <QString>
#include <QStringList>

class QDomElement;
class QDomDocument;

namespace ConfigOption
{
	enum SolverType
	{
		None,
		SelfDevelop,
		ThirdParty,
	};

	class CONFIGOPTIONSAPI SolverInfo
	{
	public:
		SolverInfo() = default;
		~SolverInfo() = default;

		void setType(SolverType t);
		SolverType getType();
		void setName(QString n);
		QString getName();
		void setEXEPath(QString path);
		QString getExePath();
		void setParameter(QString p);
		QString getParameter();
		void setProcessKeyWord(QString p);
		QString getProcessKeyWord();

		void setTemplate(QString tem);
		QString getTemplate();
		void setInputFormat(QString f);
		QString getInputFormat();
		void setTransfer(QString t);
		QString getTransfer();

		bool isWriteTemplate();

		void setDependences(QStringList de);
		QStringList getDependences();


		//∂¡»°≈‰÷√Œƒº˛xml
		void readXML(QDomElement *ele);
		void writeXML(QDomDocument* doc, QDomElement *parent);

	private:
		SolverType _type{ None };
		QString _name{};
		QString _exePath{};
		QString _parameter{};
		QString _process{};

		QString _template{};
		QString _inputFormat{};
		QString _transfer{};

		QStringList _dependences{};
	};
}

#endif