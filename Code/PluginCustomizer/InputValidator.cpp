#include "InputValidator.h"
#include "ModelBase.h"
#include <QObject>
#include <QWidget>
#include <QString>

namespace FastCAEDesigner
{
	InputValidator* InputValidator::_instance = nullptr;
	
	InputValidator::InputValidator()
	{
		InitErrorList();
	}

	InputValidator::~InputValidator()
	{

	}
	InputValidator* InputValidator::getInstance()
	{
		if (_instance == nullptr)
			_instance = new InputValidator;
		return _instance;
	}

	int InputValidator::ValidateName(QString nameChn, QString nameEng, QList<QString> usedChnNameList, QList<QString> usedEngNameList, QString &msg)
	{
		InputErrorCode errorCode = NoError;

		if (nameChn.isEmpty())
			errorCode = ChnNameIsEmpty;
		else if (nameEng.isEmpty())
			errorCode = EngNameIsEmpty;
		else if (!FileNameIsAllow(nameChn))
			errorCode = ChnNameNotAllow;
		else if (!FileNameIsAllow(nameEng))
			errorCode = EngNameNotAllow;
		if (usedChnNameList.contains(nameChn))
			errorCode = TheChnNameInUse;
		else if (usedEngNameList.contains(nameEng))
			errorCode = TheEngNameInUse;
		
		msg = _errorList[errorCode];
		return errorCode;
	}

	int InputValidator::ValidateName(QString name, QList<QString> usedNameList, QString &msg)
	{
		InputErrorCode errorCode = NoError;

		if (name.isEmpty())
			errorCode = NameIsEmpty;
		else if (!FileNameIsAllow(name))
			errorCode = NameNotAllow;
		if (usedNameList.contains(name))
			errorCode = TheChnNameInUse;
		
		msg = _errorList[errorCode];
		return errorCode;
	}

	int InputValidator::ValidateModeEditName(ModelBase* model, QString nameChn,QString nameEng, bool isEditState)
	{
		if (nullptr == model)
			return NoError;

		QString oldChnName = model->GetChnName();
		QString oldEngName = model->GetEngName();

		if (nameChn.isEmpty())
			return ChnNameIsEmpty;

		if (nameEng.isEmpty())
			return EngNameIsEmpty;

		ModelBase* parentModel = model->GetParentModelBase();
		
		if (nullptr == parentModel)
			return NoError;

		QList<QString> usedChnNameList = parentModel->GetUsedNameList(TChinese);
		if (isEditState)
			usedChnNameList.removeOne(oldChnName);

		QList<QString> usedEngNameList = parentModel->GetUsedNameList(TEnglish);
		if (isEditState)
			usedEngNameList.removeOne(oldEngName);

		if (usedChnNameList.contains(nameChn))
			return TheChnNameInUse;

		if (usedEngNameList.contains(nameEng))
			return TheEngNameInUse;

		return NoError;
	}

	void  InputValidator::InitErrorList()
	{
		_errorList.insert(ChnNameIsEmpty, tr("Chinese name is empty."));
		_errorList.insert(EngNameIsEmpty, tr("English name is empty."));
		_errorList.insert(NameIsEmpty, tr("Name is empty."));
		_errorList.insert(TheChnNameInUse, tr("Chinese name is already in use"));
		_errorList.insert(TheEngNameInUse, tr("English name is already in use"));
		_errorList.insert(NameNotAllow, tr("Special characters in name"));
		_errorList.insert(ChnNameNotAllow, tr("Special characters in  chinese name"));
		_errorList.insert(EngNameNotAllow, tr("Special characters in  english name"));
	}

	QString InputValidator::GetErrorMsg(int errorCode)
	{
		return _errorList[errorCode];
	}

	bool InputValidator::FileNameIsAllow(QString fileName)
	{
		if (fileName.isEmpty())
			return false;

		bool isOk = true;
		QString pattern = "[\\\\/:*?\"<>., ;'{|`~!@#$%&*()=+\\^}\\[\\]]";

		QRegExp rx(pattern);
		int match = fileName.indexOf(rx);

		if (match >= 0)
			isOk = false;

		return isOk;
	}

}