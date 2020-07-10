#ifndef _GMSHTHREAD_H_
#define _GMSHTHREAD_H_

#include <QThread>
#include <QList>
#include <QMultiHash>
#include <QString>
#include <QProcess>
#include "DataProperty/DataBase.h"
#include <QTextStream>
#include <QTime>

class TopoDS_Compound;

namespace GUI
{
	class MainWindow;
}

namespace MainWidget
{
	class PreWindow;
}

namespace ModuleBase
{
	class ProcessBar;
}

namespace Py
{
	class PythonAagent;
}

namespace Gmsh
{
	class GmshModule;

	class GMshPara
	{
	public:
		int _dim{ -1 };
		QMultiHash<int, int> _solidHash{};
		QMultiHash<int, int> _surfaceHash{};
		QString _elementType{};
		int _elementOrder{ -1 };
		int _method{ -1 };
		double _sizeFactor{ 0.0 };
		double _minSize{ 0.0 };
		double _maxSize{ 0.0 };
		bool _geoclean{ false };
		int _smoothIteration{ 0 };
		bool _isGridCoplanar{ false };
		QString _sizeAtPoints{};
		QString _sizeFields{};
	//	QString _physicals{};
		bool _selectall{ false };
		bool _selectvisible{ false };

	};

	class GmshThread : public DataProperty::DataBase
	{
		Q_OBJECT
	public:
		GmshThread(GUI::MainWindow* mw, MainWidget::PreWindow* pre, GmshModule* mod, int dim);
		~GmshThread();

		//���ò���
		void setPara(GMshPara*  para);

		//׷��ʵ��
		void appendSolid(int id, int index);
		void setSolid(QMultiHash<int, int> s);
		//׷������
		void appendSurface(int geo, int face);
		void setSurface(QMultiHash<int, int> s);
		//���õ�Ԫ����
		void setElementType(QString t);
		//���õ�Ԫ�״�
		void setElementOrder(int order);
		//���������ʷַ���
		void setMethod(int m);
		//���óߴ�����
		void setSizeFactor(double f);
		//������С�ߴ�
		void setMinSize(double min);
		//�������ߴ�
		void setMaxSize(double max);
		//�����Ƿ���м�������
		void isCleanGeo(bool c);
		//���ù⻬��������
		void setSmoothIteration(int it);
		//����Ƿ�������
		void setGridCoplanar(bool gc);
		//���õ������ܶ�
		void setSizeAtPoint(QString ps);
		//�������������ܶ�
		void setSizeFields(QString fs);
		//�����������
		//void setPhysicals(QString ps);

		void setSelectedAll(bool al);
		void setSelectedVisible(bool sv);

		void run();
		void stop();

	signals:
		void threadFinished(GmshThread* t);
		void sendMessage(QString);

	private slots:
	   void processFinished(int, QProcess::ExitStatus);
		void readProcessOutput();

	private:
		void mergeGeometry();
		void initGmshEnvoirment();
		void submitParaToGmsh();
		void generate();
		void readMesh();
		void appendScript(QString path);
		void generalSetting(QTextStream* out);
		void gridCoplanar(QTextStream* out);
		void sizeAtPoints(QTextStream* out);
		void sizeFields(QTextStream* out);
		void boxFieldScript(QTextStream* out, QStringList list, int& index);
		void ballFieldScript(QTextStream* out, QStringList list, int& index);
		void cylinderFieldScript(QTextStream* out, QStringList list, int& index);
	//	void physicalsGroup(QTextStream* out);
	//	void physicalsScript(QTextStream* out,QString type,QMultiHash<QString,int> pHash);
		//��ȡ��������������,����ֵ����0�� ������0��Ϊ������
		//type 1-�� 2-�� 3-�� 4-ʵ��
		int getShapeIndexInCompound(int setid, int index, int type);
		void mergeAllGeo();
		void mergeVisibleGeo();
		void mergeSelectGeo();


		QStringList getGridCoplanarScript();

	private:
		GUI::MainWindow* _mainwindow{};
		MainWidget::PreWindow* _preWindow{};
		GmshModule* _gmshModule{};

		QProcess _process{};
		ModuleBase::ProcessBar* _processBar{};

		int _dim{ -1 };
		TopoDS_Compound* _compounnd{};
		
		QMultiHash<int, int> _solidHash{};
		QMultiHash<int, int> _surfaceHash{};
		QString _elementType{};
		int _elementOrder{ -1 };
		int _method{ -1 };
		double _sizeFactor{ 0.0 };
		double _minSize{ 0.0 };
		double _maxSize{ 0.0 };
		bool _geoclean{ false };
		int _smoothIteration{ 0 };
		bool _isGridCoplanar{ false };
		QString _sizeAtPoints{};
		QString _sizeFields{};
		//QString _physicals{};
		bool _selectall{ false };
		bool _selectvisible{ false };
	};

}


#endif