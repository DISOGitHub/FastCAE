#ifndef _POST2DINTERFACE_H_
#define _POST2DINTERFACE_H_

#include "PostWindowBase.h"
#include <QHash>
#include <QVector>;

class QTextStream;

namespace Ui
{
	class Post2DInterface;
}
namespace GUI
{
	class MainWindow;
}

namespace Post
{
	class Post2DWindow;

	class POSTAPI Post2DWindowInterface : public PostWindowBase
	{
	public:
		Post2DWindowInterface(GUI::MainWindow* m, int proid);
		~Post2DWindowInterface();
		void reTranslate() override;
		//��ȡ��ά��ͼ����
		Post2DWindow* getWindow();
		//���ļ�
		void openfile(QString fileName);
		//�������
		void addCurve(QString name, QString f, QString x, QString y);
		//�Ƴ�����
		void removeCurve(QString name);
		void saveImage(QString fileName, int width, int heigh, bool showDlg) override;
		
	private:
		QString readLine();
		QVector<double> getData(QString f, QString va);

	private:
		Ui::Post2DInterface* _ui{};
		Post2DWindow* _post2DWindow{};

		QTextStream* _stream{};
		QHash<QString, QHash<QString, QVector<double>>> _values{};
	};
}


#endif
