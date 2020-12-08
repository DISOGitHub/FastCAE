#ifndef _INPDATAEXCHANGE_H_
#define _INPDATAEXCHANGE_H_

#include "meshDataExchangePlugin.h"
#include "MeshThreadBase.h"
#include <QHash>

class QTextStream;
class vtkUnstructuredGrid;
class vtkDataSet;

namespace ModelData
{
	class ModelDataBaseExtend;
}

namespace MeshData
{
	class MeshData;

	class MESHDATAEXCHANGEPLUGINAPI INPdataExchange :public MeshThreadBase
	{
	public:
		INPdataExchange(const QString &filename, MeshOperation operation, GUI::MainWindow *mw ,int modelId = -1);
		~INPdataExchange();

		void run();
		bool read();
		bool write();

		void readLine(QString &line);
		bool readNodes(vtkUnstructuredGrid* g, QString &line);
		bool readElements(vtkUnstructuredGrid* g, QString &line);
		bool readNSet(QString &line, QList<int>& inpSetIds);
		bool readElSet(QString &line, QList<int>& inpSetIds);
		bool readMaterial(QString &line, QStringList& materialName, QStringList& density, QStringList& elastic);
		bool readBoundary(QString &line, QStringList& bcSetIds, QStringList& bcName, QStringList& bcType, QList<double>& displacement, QList<double>& rotation);

	private:
		void addINPComponents(const QList<int>&);
		void addINPMaterials(const QStringList&, const QStringList&, const QStringList&);
		void addINPBCs(const QStringList& bcSetIds, const QStringList& bcName, const QStringList& bcType, const QList<double>& displacement, const QList<double>& rotation);

		void writePoint(vtkDataSet* data);
		void writeCell(vtkDataSet* data);
		void writeComponent(int kId);
		void writeMaterial();
		void writeBoundary();
		void endsWithGEN(QString& inpSetName, QString& qSet, int head, int tail);
		void notEndsWithGEN(QString& inpSetName, QString& qSet, int nMember, const QList<int>& members);

	private:
		GUI::MainWindow* _mw;
		QString _fileName{};
		QTextStream* _stream{};

		MeshOperation _operation;
		MeshData* _meshData{};
		QHash<int, int> _nodeIDIndex{};
		QHash<int, int> _elemIDIndex{};
		int _modelId;
		ModelData::ModelDataBaseExtend* _Case{};
	};
}
#endif