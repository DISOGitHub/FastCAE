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
#include <vtkCellType.h>
#include "GmshModuleAPI.h"

class TopoDS_Compound;
class TopoDS_Shape;
class gp_Pnt;

class vtkDataSet;

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

namespace MeshData{
	class MeshKernal;
}

namespace Gmsh
{
	typedef struct
	{
		int geoSetID;
		int itemIndex;
		QList<int> cellIndexs;
	}itemInfo;

	class GmshModule;
	class FluidMeshPreProcess;
	class GmshScriptWriter;
	//class GmshSettingData;
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
		int _meshID{ -1 };
		QList<double*> _fluidField{};
		bool _fluidMesh{ false };
		QString _cells{};
	};

	class GMSHAPI GmshThread : public DataProperty::DataBase
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
		//����mesh����
		void setMeshID(int id);
		//��������ȫѡ
		void setSelectedAll(bool al);
		//����ѡ������ɼ���
		void setSelectedVisible(bool sv);
		//����Ϊ�����������ʷ�ģʽ
		void setFluidMesh(bool fm);
		//�������������ʷֵ�����
//		void setFluidField(QList<double*> coors);
		//����ָ���������ͣ����棩
		void setCellTypeList(QString cells);


		void run();
		void stop();

		//�Ƿ񱣴�vtkData
		void isSaveDataToKernal(bool save);
		/*���ؽڵ�/��Ԫ�ڼ����ϵ�id*/
		QList<itemInfo> generateGeoIds(vtkDataSet* dataset);

	signals:
		void threadFinished(GmshThread* t);
		void sendMessage(QString);
		void writeToSolveFileSig(vtkDataSet*);
		void updateMeshActor();

	private slots:
	   void processFinished(int, QProcess::ExitStatus);
		void readProcessOutput();

	private:
		void mergeGeometry();
		void initGmshEnvoirment();
		void submitParaToGmsh();
		void generate();
		void readMesh();
	
		void mergeAllGeo();
		void mergeVisibleGeo();
		void mergeSelectGeo();

		//����meshkernal�����ʷֲ���
		void setGmshSettingData(MeshData::MeshKernal* k);
		//���ýű���д����
		void setGmshScriptData();
		//ɾ��ָ����Ԫ
		vtkDataSet* deleteSpecifiedCells(vtkDataSet* dataset);
		//�ж��Ƿ�Ϊָ����Ԫ
		bool isSpecifiedCell(VTKCellType type);

	private:
		GUI::MainWindow* _mainwindow{};
		MainWidget::PreWindow* _preWindow{};
		GmshModule* _gmshModule{};
		FluidMeshPreProcess* _fluidMeshProcess{};

		QProcess _process{};
		ModuleBase::ProcessBar* _processBar{};

		int _dim{ -1 };
		TopoDS_Compound* _compounnd{};
		//TopoDS_Shape* _faushape{};

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
		bool _selectall{ false };
		bool _selectvisible{ false };
		bool _isSaveToKernal{ true };
		int _meshID{ -1 };

		bool _fluidMesh{ false };

		QList<int> _cellTypeList{};

		GmshScriptWriter* _scriptWriter{};
	};

}


#endif