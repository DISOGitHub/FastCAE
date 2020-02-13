#include "XOperateWord.h"
#include <windows.h>
#include <QDir>

namespace XReport{

	XOperateWord::XOperateWord(const QString & filePath, bool visable)
		: m_visable(visable)
		, m_templateFilePath(filePath)
	{
		HRESULT result = OleInitialize(0);
		if (result != S_OK && result != S_FALSE)
			qWarning("Qt: could not initialize OLE (error %x)", (unsigned int)result);
	}

	XOperateWord::~XOperateWord()
	{
		OleUninitialize();
	}

	bool XOperateWord::open()
	{
		m_pWord = new QAxObject();
		if (!m_pWord){ return false; }

		if (!m_pWord->setControl("word.Application"))
		{
			if (!m_pWord->setControl("kwps.Application"))
				return false;
		}

		m_pWord->setProperty("Visible", m_visable);

		QAxObject * pDoc = m_pWord->querySubObject("Documents");
		if (!pDoc){ return false; }

		pDoc->dynamicCall("Add(QString)", m_templateFilePath);

		m_pDoc = m_pWord->querySubObject("ActiveDocument");
		if (m_pDoc)
			m_bOpen = true;
		return m_bOpen;
	}

	void XOperateWord::close()
	{
		if (m_pWord)
			m_pWord->setProperty("DisplayAlerts", false);

		if (m_pDoc)
			m_pDoc->dynamicCall("Close(bool)", false);

		if (m_pWord)
			delete m_pWord;

		m_pWord = NULL;
		m_pDoc = NULL;
		m_bOpen = false;
	}
	
	void XOperateWord::save(const QString & savePath, SaveFileFormat format)
	{
		if (!m_bOpen || !m_pDoc){ return; }

		QAxObject *fields = m_pDoc->querySubObject("Fields");
		if (!fields){ return; }
		fields->dynamicCall("Update(void)");

		if (format == WordFile)
		{
			m_pDoc->dynamicCall("SaveAs (const QString&)", savePath);
		}
		else if (format == PDFFile)
		{
			QList<QVariant>qList = QList<QVariant>{savePath, 17, false, 0, 0, 1, 1, 0, true, true, 0, true, true, false};
			m_pDoc->dynamicCall("ExportAsFixedFormat(OutputFileName, ExportFormat)", savePath, 17);
		}
	}

	bool XOperateWord::replace(const QString & label, ReplaceType type, const QString & content)
	{
		if (!m_pDoc){ return false; }
		QAxObject * pBookMark = m_pDoc->querySubObject("Bookmarks(QString)", label);
		int iError = GetLastError();
		if (pBookMark)
		{
			pBookMark->dynamicCall("Select(void)");
			if (type == TextType)
			{
				pBookMark->querySubObject("Range")->setProperty("Text", content);
			}
			else if (type == PicType)
			{
				QList<QVariant> list = QList<QVariant>{content, false, true, pBookMark->querySubObject("Range")->asVariant()};
				QAxObject *inlineShapes = m_pDoc->querySubObject("InlineShapes");
				inlineShapes->dynamicCall("AddPicture(const QString&, QVariant, QVariant ,QVariant)", list);
				delete inlineShapes;
			}
			delete pBookMark;
			return true;
		}
		return false;
	}

 	QAxObject * XOperateWord::insertTable(QString sLabel, int row, int column)
 	{
 		if (!m_pDoc){ return NULL; }
 		QAxObject *bookmark = m_pDoc->querySubObject("Bookmarks(QVariant)", sLabel);
 		if (!bookmark){ return NULL; }
 
 		bookmark->dynamicCall("Select(void)");
 		QAxObject *selection = m_pWord->querySubObject("Selection");
 
 		selection->dynamicCall("InsertAfter(QString&)", "\n");
 		selection->querySubObject("ParagraphFormat")->dynamicCall("Alignment", "wdAlignParagraphCenter");
 
 		QAxObject *range = selection->querySubObject("Range");
 		QAxObject *tables = m_pDoc->querySubObject("Tables");
 		QAxObject *table = tables->querySubObject("Add(QVariant,int,int)", range->asVariant(), row, column);
 
 		for (int i = 1; i <= 7; i++)
 		{
 			QString str = QString("Borders(-%1)").arg(i);
 			QAxObject *borders = table->querySubObject(str.toLatin1().constData());
 			borders->dynamicCall("SetLineStyle(int)", 1);
 		}
 		return table;
 	}
 
 	QAxObject * XOperateWord::insertTable(QString sLabel, int row, int column, QStringList headList, QString title)
 	{
 		if (!m_pDoc){ return NULL; }
 		QAxObject *bookmark = m_pDoc->querySubObject("Bookmarks(QVariant)", sLabel);
 		if (!bookmark){ return NULL; }
 		if (headList.size() != column * 2){ return NULL; }
 
 		bookmark->dynamicCall("Select(void)");
 		QAxObject *selection = m_pWord->querySubObject("Selection");
 		selection->dynamicCall("MoveUp(Unit,Count,Extend)", 5, 1, 0);//上移一行，在书签前写入表 //Const wdLine = 5  Const wdMove = 0  Const wdExtend = 1
 		selection->dynamicCall("TypeParagraph(void)");//TypeParagraph 方法与 Enter（回车键）的功能相同
 		selection->dynamicCall("MoveUp(Unit,Count,Extend)", 5, 1, 0);//
 		selection->querySubObject("ParagraphFormat")->dynamicCall("Alignment", "wdAlignParagraphCenter");
 		//Selection.Range.HighlightColorIndex = wdYellow
 		selection->querySubObject("Range")->dynamicCall("HighlightColorIndex", "wdYellow");
 		//设置标题
 		//selection->dynamicCall("TypeText(QString&)", "\r\n" + title);//////表上标题
 		selection->dynamicCall("TypeText(QString&)", title);//////表上标题
 
 		QAxObject *range = selection->querySubObject("Range");
 		range->dynamicCall("HighlightColorIndex", "wdNoHighlight");
 		QAxObject *tables = m_pDoc->querySubObject("Tables");
 		QAxObject *table = tables->querySubObject("Add(QVariant,int,int)", range->asVariant(), row * 2, column);
 		//表格自动拉伸列 0固定  1根据内容调整  2 根据窗口调整
 		table->dynamicCall("AutoFitBehavior(WdAutoFitBehavior)", 2);
 
 		//设置表头
 		for (int i = 1; i < 8; i++)
 		{
 			table->querySubObject("Cell(int,int)", 1, i)->querySubObject("Range")->dynamicCall("SetText(QString)", headList.at(i - 1));
 			//加粗
 			table->querySubObject("Cell(int,int)", 1, i)->querySubObject("Range")->dynamicCall("SetBold(int)", true);
 
 			table->querySubObject("Cell(int,int)", row + 1, i)->querySubObject("Range")->dynamicCall("SetText(QString)", headList.at(i + 7 - 1));
 			//加粗
 			table->querySubObject("Cell(int,int)", row + 1, i)->querySubObject("Range")->dynamicCall("SetBold(int)", true);
 		}
 
 		for (int i = 1; i < 7; i++)
 		{
 			QString str = QString("Borders(-%1)").arg(i);
 			QAxObject *borders = table->querySubObject(str.toLatin1().constData());
 			borders->dynamicCall("SetLineStyle(int)", 1);
 		}
 		return table;
 	}
 
 	void XOperateWord::setColumnWidth(QAxObject *table, int column, int width)
 	{
 		if (!table){ return; }
 		table->querySubObject("Columns(int)", column)->setProperty("Width", width);
 	}
 
 	void XOperateWord::setRowHeight(QAxObject *table, int row, int height)
 	{
 		if (!table){ return; }
 		table->querySubObject("Rows(int)", row)->setProperty("Height", height);
 	}
 
	void XOperateWord::setTableCellString(QAxObject *table, int row, int column, QString text)
	{
		if (!table)
			return;
		QAxObject *cell = table->querySubObject("Cell(int,int)", row, column);
		if (!cell)
			return;
		cell->dynamicCall("Select(void)");
		cell->querySubObject("Range")->setProperty("Text", text);
	}

	void XOperateWord::setTableCellPicture(QAxObject *table, int row, int column, QString sFile)
 	{
 		if (!table)
 			return;
 
 		QAxObject *cell = table->querySubObject("Cell(int,int)", row, column);
 		if (!cell){ return; }
 
 		QDir dir(sFile);
 		QString filePath = dir.absolutePath();
 		filePath = QDir::toNativeSeparators(filePath);
 
 		cell->dynamicCall("Select(void)");
 		QVariant tmp = cell->querySubObject("Range")->asVariant();
 
 		QAxObject *Inlineshapes = cell->querySubObject("Range")->querySubObject("InlineShapes");
 		if (!Inlineshapes){ return; }
 		Inlineshapes->dynamicCall("AddPicture(QString&,bool,bool,QVariant)", filePath, false, true, tmp);
 	}
 
 
 
 	QAxObject * XOperateWord::insertTableNoBorder(QString sLabel, int row, int column, QString title)
 	{
 		if (!m_pDoc){ return NULL; }
 		QAxObject *bookmark = m_pDoc->querySubObject("Bookmarks(QVariant)", sLabel);
 		if (!bookmark){ return NULL; }
 
 		bookmark->dynamicCall("Select(void)");
 		QAxObject *selection = m_pWord->querySubObject("Selection");
 
 		selection->dynamicCall("MoveUp(Unit,Count,Extend)", 5, 1, 0);//上移一行，在书签前写入表 //Const wdLine = 5  Const wdMove = 0  Const wdExtend = 1
 		selection->dynamicCall("TypeParagraph(void)");//TypeParagraph 方法与 Enter（回车键）的功能相同
 		//selection->dynamicCall("MoveUp(Unit,Count,Extend)", 5, 1, 0);//
 
 		QAxObject *range = selection->querySubObject("Range");
 		QAxObject *tables = m_pDoc->querySubObject("Tables");
 		QAxObject *table = tables->querySubObject("Add(QVariant,int,int)", range->asVariant(), row + 1, column);
 
 		//ActiveDocument.Tables(1).Cell(3, 1).Select
 		QAxObject *cell = table->querySubObject("Cell(int,int)", row + 1, 1); //选择单元格
 		cell->dynamicCall("Select(void)");                                    //选中选择单元格
 		//Selection.SelectRow
 		selection->dynamicCall("SelectRow(void)");                            //选中单元格所在行
 		QAxObject *cells = selection->querySubObject("Cells");                //选中一行单元格
 		//Selection.Cells.Merge
 		cells->dynamicCall("Merge(void)");                                    //选中一行单元格执行合并
 		//Selection.SelectCell
 		selection->dynamicCall("SelectCell(void)");                           //选中合并后的单元格
 		//Selection.ParagraphFormat.Alignment = wdAlignParagraphCenter
 		selection->querySubObject("ParagraphFormat")->setProperty("Alignment", "wdAlignParagraphCenter");
 		//Selection.Cells.VerticalAlignment = wdCellAlignVerticalCenter
 		cells->setProperty("VerticalAlignment", "wdCellAlignVerticalCenter");
 		selection->querySubObject("Range")->dynamicCall("HighlightColorIndex", "wdYellow");
 		//Selection.TypeText Text : = "abc"
 		selection->dynamicCall("TypeText(Text)", title);
 		selection->querySubObject("Range")->dynamicCall("HighlightColorIndex", "wdNoHighlight");
 
 		return table;
 	}

}
