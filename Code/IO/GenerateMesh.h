#include "IOAPI.h"
#include "Gmsh/GmshThread.h"

class QDomDocument;
class QDomElement;
class vtkDataSet;

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
		void writeMeshToSolveFile(QDomDocument* doc, QDomElement* parent, vtkDataSet* data);
		void writeComponentToSolveFile(QDomDocument* doc, QDomElement* parent, vtkDataSet* data);
		QString getCellIndexs(QList<Gmsh::itemInfo>& infos, Geometry::GeoComponent*/*int geoSetID, int itemIndex*/);

	private:
		GUI::MainWindow* _mw;
		QString _filePath;
		Gmsh::GmshThread* _thread;
	};
}
