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

		//ǰ������������ɫ
		void setBackgroundTopColor(QColor c);
		QColor getBackgroundTopColor();
		//ǰ����Ͳ�������ɫ
		void setBackgroundBottomColor(QColor c);
		QColor getBackgroundBottomColor();
		//������ɫ
		void setHighLightColor(QColor c);
		QColor getHighLightColor();
		//Ԥ������ɫ
		void sePretHighLightColor(QColor c);
		QColor getPreHighLightColor();
		//��������ɫ
		void setGeometrySurfaceColor(QColor c);
		QColor getGeometrySurfaceColor();
		//��������ɫ
		void setGeometryCurveColor(QColor c);
		QColor getGeometryCurveColor();
		//���ε���ɫ
		void setGeometryPointColor(QColor c);
		QColor getGeometryPointColor();
		//��������ɫ
		void setMeshFaceColor(QColor c);
		QColor getMeshFaceColor();
		//�������ɫ
		void setMeshEdgeColor(QColor c);
		QColor getMeshEdgeColor();
		//����ڵ���ɫ
		void setMeshNodeColor(QColor c);
		QColor getMeshNodeColor();
		//����ڵ��С
		void setMeshNodeSize(float s);
		float getMeshNodeSize();
		//����߿��
		void setMeshEdgeWidth(float s);
		float getMeshEdgeWidth();
		//���ε��С
		void setGeoPointSize(float s);
		float getGeoPointSize();
		//���α߿��
		void setGeoCurveWidth(float s);
		float getGeoCurveWidth();
		//͸����
		void setTransparency(int t);
		int getTransparency();

		//���ε���ʾ
		void isShowGeoPoint(bool show);
		bool isShowGeoPoint();
		//���α���ʾ
		void isShowGeoEdge(bool show);
		bool isShowGeoEdge();
		//��������ʾ
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
