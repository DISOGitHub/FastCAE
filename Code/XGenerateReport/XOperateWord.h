#ifndef XOPERATEWORD_H
#define XOPERATEWORD_H

#include <QObject>
#include <QAxObject>
#include "xgeneratereport_global.h"

namespace XReport{
	class XGENERATEREPORT_EXPORT XOperateWord : public QObject
	{
		Q_OBJECT
	public:
		typedef enum tag_SaveFileFormat
		{
			WordFile = 0,
			PDFFile,
		}SaveFileFormat;

		typedef enum tag_ReplaceType
		{
			TextType = 0,
			PicType,
		}ReplaceType;

	public:
		explicit XOperateWord(const QString & filePath, bool visable);
		~XOperateWord();

		bool open();

		void close();

		void save(const QString & savePath , SaveFileFormat format);

		bool replace(const QString & label , ReplaceType type , const QString & content);

		///<MG implement about table
		QAxObject *insertTable(QString sLabel, int row, int column);
		QAxObject *insertTableNoBorder(QString sLabel, int row, int column, QString title);
		QAxObject *insertTable(QString sLabel, int row, int column, QStringList headList, QString title);

		///<MG set the width of the column
		void setColumnWidth(QAxObject *table, int column, int width);

		///<MG set the height of the row
		void setRowHeight(QAxObject *table, int row, int height);

		///<MG set the cell element of the table ' s string 
		void setTableCellString(QAxObject *table, int row, int column, QString text);

		///<MG set the cell element of the table 's picture
		void setTableCellPicture(QAxObject *table, int row, int column, QString sFile);

	private:
		///<MG pointer to the word process
		QAxObject * m_pWord{};

		///<MG pointer to the work word document
		QAxObject * m_pDoc{};

		///<MG visable
		bool m_visable{ false };

		///<MG template file path
		QString m_templateFilePath;

		///<MG is word file open 
		bool m_bOpen{ false };
	};
}


#endif // XOPERATEWORD_H
