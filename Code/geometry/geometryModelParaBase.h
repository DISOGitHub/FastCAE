#ifndef GEOMETRYMODELBASE_H_
#define GEOMETRYMODELBASE_H_

#include "geometryAPI.h"
#include "DataProperty/DataBase.h"

namespace Geometry
{
	enum GeometryParaType
	{
		GeometryParaNone = 0,
		GeometryParaCreateBox,
		GeometryParaCreateCylinder,
		GeometryParaCreateCone,
		GeometryParaCreateFillet,
		GeometryParaCreateSphere,
		GeometryParaCreatePoint,
		GeometryParaCreateLine,
		GeometryParaCreateFace,
		GeometryParaMakeChamfer,
		GeometryParaMakeVariableFillet,
		GeometryParaMakeBoolOperation,
		GeometryParaMakeMirrorFeature,
		GeometryParaMakeRotateFeature,
		GeometryParaMakeMoveFeature,
		GeometryParaMakeMatrix,
		GeometryParaMakeExtrusion,
		GeometryParaMakeLoft,
		GeometryParaMakeSweep,
		GeometryParaMakeRevol,
		GeometryParaSplitter,
		GeometryParaFillHole,
		GeometryParaRemoveSurface,
		GeometryParaFillGap,

	};

	class GeometryData;

	class GEOMETRYAPI GeometryModelParaBase : public DataProperty::DataBase
	{
	public:
		//���캯��
		GeometryModelParaBase();
		~GeometryModelParaBase() = default;
		//��ȡ����
		GeometryParaType getParaType();
		//ͨ���ַ���������������
		static GeometryModelParaBase* createParaByString(QString s);

	protected:
		QString typeToString();
	
	protected:
		GeometryParaType _type{ GeometryParaNone };
		GeometryData* _geoData{};
	};




}


#endif