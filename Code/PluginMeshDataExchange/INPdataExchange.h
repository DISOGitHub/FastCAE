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

namespace Material {
	class Material;
}

namespace DataProperty {
	class ParameterBase;
}

namespace MeshData
{
	class MeshData;
	class MeshKernal;

	class MESHDATAEXCHANGEPLUGINAPI INPdataExchange :public MeshThreadBase
	{
	public:
		INPdataExchange(const QString &filename, MeshOperation operation, GUI::MainWindow *mw ,int modelId = -1);
		~INPdataExchange();

		void run();
		bool read();
		bool write();

		void readLine(QString &line);
		bool readNode(vtkUnstructuredGrid* g, QString &line, QList<int>& inpSetIds,MeshKernal* k);
		bool readNodes(vtkUnstructuredGrid* g, QString &line);
		bool readElement(vtkUnstructuredGrid* g, QString &line, QList<int>& inpSetIds, MeshKernal* k);
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


		bool readMaterial(QString& line);
		void addINPMaterials();
		void addINPMaterialAttribute(Material::Material* m, QString name, QStringList list);

		DataProperty::ParameterBase* getMaterialParameter(QString s);

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

		QHash<QString, QMultiHash<QString,QString>> m_materialHash{};
		/*QMultiHash<QString, double> m_materialAttributeHash{};*/

	};
}
#endif