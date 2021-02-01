#ifndef EDITOR3DFILEVALUE_H
#define EDITOR3DFILEVALUE_H

#include <QDialog>

namespace Ui {
class Editor3DFileValue;
}
namespace FastCAEDesigner{
	class Editor3DFileValue : public QDialog
	{
		Q_OBJECT

	public:
		explicit Editor3DFileValue(QWidget *parent = 0);
		Editor3DFileValue(QString, QString, QString, QWidget *parent = 0);
		~Editor3DFileValue();

		void Init();
		void InitErrorList();
		QString getName();
		QString getType();
		QString getDataType();
		int IsDataSure();
		void setNameUsedList(QList<QString> list);

	private slots:
		void OnOkPBtnClicked();
		void OnTypeChanged();
		void OnDataTypeChanged();
		void OnTimeout();

	private:
		QString _name;
		QString _type;
		QString _dataType;
		QList<QString> _nameUsedList;
		QHash<int, QString> _errorList{};
		enum ErrorText
		{
			NameIsEmpty = 1,
			TheNameInuse,
			TypeIsEmpty,
			DataTypeIsEmpty,
			IllegalName,
		};

	private:
		Ui::Editor3DFileValue *ui;
	};
}
#endif // EDITOR3DFILEVALUE_H
