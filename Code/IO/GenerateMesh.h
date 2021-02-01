#include "IOAPI.h"
#include "Gmsh/GmshThread.h"

class QDomDocument;
class QDomElement;
class vtkDataSet;
class QTimer;

namespace GUI
{
	class MainWindow;
}

namespace Gmsh
{
	class GmshThread;
}

namespace Geometry
{
	class GeoComponent;
}

namespace IO
{
	class IOAPI GenerateMesh : public QObject
	{
		Q_OBJECT

	public:
		GenerateMesh() = default;
		~GenerateMesh() = default;
		void iniGenerateMesh(GUI::MainWindow* mw, QString path);
		void startGenerateMesh();

	private slots:
		void writeToSolveFileSlot(vtkDataSet* data);

	private:
		void iniParameter();
		void writeKernelToSolveFile(QDomDocument* doc, QDomElement* kernel, vtkDataSet* data);
		void writeComponentToSolveFile(QDomDocument* doc, QDomElement* set, vtkDataSet* data);
		QString getCellIndexs(QList<Gmsh::itemInfo>& infos, Geometry::GeoComponent*);

	private:
		GUI::MainWindow* _mw;
		QString _filePath;
		Gmsh::GmshThread* _thread;
	};
}
