#ifndef _BCUSERDEF_H_
#define _BCUSERDEF_H_

#include "BCBase.h"
#include <QString>
#include <QIcon>

class QDomElement;
class QDomDocument;

namespace BCBase
{
	class BCBASEAPI BCUserDef : public BCBase 
	{
	public:
		BCUserDef();
		~BCUserDef() = default;

		void copy(DataBase* data) override;

		QString getName();
		void setName(QString s);
		QString getChinese();
		void setChinese(QString s);
		void setIcon(QString icon);
		QString getIcon();

		QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* parent) override;
		void readDataFromProjectFile(QDomElement* ele) override;

	private:
		QString _name{};
		QString _chinese{};
		QString _icon{};
	};

}


#endif