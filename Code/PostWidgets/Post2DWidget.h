#ifndef _POST2DWINDOW_H_
#define _POST2DWINDOW_H_

#include "PostWidgetsAPI.h"
#include "moduleBase/graphWindowBase.h"
#include "CurveAnalyse/curve_struct.h"

namespace GUI
{
	class MainWindow;
}
namespace Ui
{
	class Post2DWidget;
}

class curve_MainWindow;

namespace Post
{

	class POSTAPI Post2DWindow : public ModuleBase::GraphWindowBase
	{
		Q_OBJECT
	public:
		Post2DWindow(GUI::MainWindow* mw, int proID);
		~Post2DWindow();
		//���ڻ���
		void replot();
		//���ļ�
		bool openFile(QString file);
		//���ļ��������
		void addCurve(QString tep_filename, int tep_column_index);
		//�������ļ���ֱ����2�鸡����������
		void addCurve(QVector<double> data_x, QVector<double> data_y, QString tep_filename);
		//������� xĬ��Ϊ����ֵ��0��1��2��
		void addCurve(QVector<double> data_y, QString fileName);
		//��������
		void updateCurve(QVector<double> data_x, QVector<double> data_y, QString tep_filename);
		//��������
		void updateCurve(QVector<double> data_y, QString tep_filename);
		//ɾ������
		void delCurve(QString fileName, int colum_index);
		//���ȫ������
		void delAllCurves();
		//���ñ�������
		void setTitle(QString title);
		//���ñ���������ɫ 0-���� 1-���� 2-Arial
		void setTitleFont(int font);
		//���ñ��������С
		void setTitleFontSize(int size);
		//���ñ���������ɫ
		void setTitleFontColor(QColor color);
		//���û�ͼ���ı�����ɫ
		void setBackGround(QColor color);
		//���û�ͼ��������
		void setPlotGrid(bool isgrid);
		//����Legendλ�� 0-none 1-���� 2-���� 3-���� 4-����
		void setLegendPosition(int p);
		//�������������
		void setAxisNum(int num);
		//��������������
		void setAxisName(QString axis, QString name);
		//����������������ɫ
		void setAxisFontColor(QString axis, QColor color);
		//���������������С
		void setAxisFontSize(QString axis, int size);
		//�������������� 0-���� 1-���� 2-Arial
		void setAxisFont(QString axis, int font);
		//����������ɫ
		void setCurveColor(QString fileName, int colIndex, QColor color);
		//��������
		void setCurveStyle(QString fileName, int colIndex, Qt::PenStyle style);
		//�����߿�
		void setCurveWidth(QString fileName, int colindex, int width);
		//�Ƿ���ʾ���ݵ�
		void setCurveShowPoint(QString fileName, int colindex, bool show);
		//������������
		void setCurveName(QString fileName, int colindex, QString name);
		//������������������
		void setCurveAxisIndex(QString fileName, int colindex, int axisIndex);
		//��ȡģ��������
		QWidget* getTreeWidget();
		//��ȡ���Դ���
		QWidget* getPropWidget();
		///���ñ�����ɫ
		void setBackGround(QColor color1, QColor color2) override;
		///����ͼƬ
		void saveImage(QString fileName, int width, int heigh, bool showDlg) override;
		///���������᷶Χ  axis-x y x2 y2  reange[0]-min range[1]-max
		void setAxisRange(QString axis, double range[2]);
		///��ȡ����������
		QStringList getColumnNameList(QString fileName);
		//�����л�
		void reTranslate() override;
		//��ȡ�������Խӿ�
		curve_line_data getCurveProp(QString tepFileName, int colIndex);
		//��ʼ����
		bool startAnimate();
		//��������
		bool stopAnimate();
		//���ö�����ɫ
		void setAniPointColor(QString fileName, int colum, QColor color);
		//���ö�����״
		void setAniPointType(QString fileName, int colum, aniPointType type);


	private:
		Ui::Post2DWidget* _ui{};
		curve_MainWindow* _curveMainWindow{};

	};

}


#endif