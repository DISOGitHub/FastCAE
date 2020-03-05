#ifndef GRAPHOPTION_H_
#define GRAPHOPTION_H_

#include "settingAPI.h"
#include <QColor>

class QSettings;

namespace Setting
{
	class SETTINGAPI GraphOption
	{
	public:
		GraphOption() = default;
		~GraphOption() = default;
		void writeINI(QSettings* setting);
		void readINI(QSettings* settings);

		//前处理顶部背景颜色
		void setBackgroundTopColor(QColor c);
		QColor getBackgroundTopColor();
		//前处理低部背景颜色
		void setBackgroundBottomColor(QColor c);
		QColor getBackgroundBottomColor();
		//高亮颜色
		void setHighLightColor(QColor c);
		QColor getHighLightColor();
		//预高亮颜色
		void sePretHighLightColor(QColor c);
		QColor getPreHighLightColor();
		//几何面颜色
		void setGeometrySurfaceColor(QColor c);
		QColor getGeometrySurfaceColor();
		//几何线颜色
		void setGeometryCurveColor(QColor c);
		QColor getGeometryCurveColor();
		//几何点颜色
		void setGeometryPointColor(QColor c);
		QColor getGeometryPointColor();
		//网格面颜色
		void setMeshFaceColor(QColor c);
		QColor getMeshFaceColor();
		//网格边颜色
		void setMeshEdgeColor(QColor c);
		QColor getMeshEdgeColor();
		//网格节点颜色
		void setMeshNodeColor(QColor c);
		QColor getMeshNodeColor();
		//网格节点大小
		void setMeshNodeSize(float s);
		float getMeshNodeSize();
		//网格边宽度
		void setMeshEdgeWidth(float s);
		float getMeshEdgeWidth();
		//几何点大小
		void setGeoPointSize(float s);
		float getGeoPointSize();
		//几何边宽度
		void setGeoCurveWidth(float s);
		float getGeoCurveWidth();
		//透明度
		void setTransparency(int t);
		int getTransparency();

		//几何点显示
		void isShowGeoPoint(bool show);
		bool isShowGeoPoint();
		//几何边显示
		void isShowGeoEdge(bool show);
		bool isShowGeoEdge();
		//几何面显示
		void isShowGeoSurface(bool show);
		bool isShowGeoSurface();


	private:
		QColor _bgTop{};
		QColor _bgBottom{};
		QColor _highLightColor{};
		QColor _preHighLightColor{};
		QColor _geoSurfaceColor{};
		QColor _geoCurveColor{};
		QColor _geoPointColor{};
		QColor _meshFaceColor{};
		QColor _meshEdgeColor{};
		QColor _meshNodeColor{};

		float _meshNodeSize{ 1.0 };
		float _meshEdgeWidth{ 1.0 };
		float _geoPointSize{ 1.0 };
		float _geoCurveWidth{ 1.0 };

		bool _showGeoPoint{ false };
		bool _showGeoEdge{ false };
		bool _showGeoSurface{ false };

		int _transparency{ 0 };


	};


}



#endif
