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
		//获取二维绘图窗口
		Post2DWindow* getWindow();
		//打开文件
		void openfile(QString fileName);
		//添加曲线
		void addCurve(QString name, QString f, QString x, QString y);
		//移除曲线
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
