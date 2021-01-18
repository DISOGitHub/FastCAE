#ifndef INPUTVALIDATOR_H
#define INPUTVALIDATOR_H

#include <QHash>
#include <QString>
#include <QObject>

namespace FastCAEDesigner
{
	class ModelBase;
	
	class InputValidator : public QObject
	{
		Q_OBJECT
	public:
		enum InputErrorCode
		{
			NoError = 0,
			ChnNameIsEmpty,
			EngNameIsEmpty,
			NameIsEmpty,
			TheChnNameInUse,
			TheEngNameInUse,
			NameNotAllow,
			ChnNameNotAllow,
			EngNameNotAllow,
		};

	public:
		static InputValidator* getInstance();
		int ValidateModeEditName(ModelBase* model, QString nameChn, QString nameEng, bool isEditState);
		int ValidateName(QString nameChn, QString nameEng, QList<QString> usedChnNameList, QList<QString> usedEngNameList, QString &msg);
		int ValidateName(QString name, QList<QString> usedNameList, QString &msg);

		QString GetErrorMsg(int errorCode);
		bool FileNameIsAllow(QString fileName);
		bool FileEngNameIsAllow(QString fileName);
		bool FileChnNameIsAllow(QString fileName);

	private:
		InputValidator();
		~InputValidator();
		void InitErrorList();
		

	private:
		static InputValidator* _instance;
		QHash<int, QString> _errorList{};
	};
}

#endif