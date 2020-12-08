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
		//构造函数
		GeometryModelParaBase();
		~GeometryModelParaBase() = default;
		//获取类型
		GeometryParaType getParaType();
		//通过字符串创建操作参数
		static GeometryModelParaBase* createParaByString(QString s);

	protected:
		QString typeToString();
	
	protected:
		GeometryParaType _type{ GeometryParaNone };
		GeometryData* _geoData{};
	};




}


#endif