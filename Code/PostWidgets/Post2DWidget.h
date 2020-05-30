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
		//窗口绘制
		void replot();
		//打开文件
		bool openFile(QString file);
		//从文件添加曲线
		void addCurve(QString tep_filename, int tep_column_index);
		//不调用文件，直接用2组浮点数画曲线
		void addCurve(QVector<double> data_x, QVector<double> data_y, QString tep_filename);
		//添加曲线 x默认为索引值（0，1，2）
		void addCurve(QVector<double> data_y, QString fileName);
		//更新曲线
		void updateCurve(QVector<double> data_x, QVector<double> data_y, QString tep_filename);
		//更新曲线
		void updateCurve(QVector<double> data_y, QString tep_filename);
		//删除曲线
		void delCurve(QString fileName, int colum_index);
		//清除全部曲线
		void delAllCurves();
		//设置标题名称
		void setTitle(QString title);
		//设置标题字体颜色 0-宋体 1-黑体 2-Arial
		void setTitleFont(int font);
		//设置标题字体大小
		void setTitleFontSize(int size);
		//设置标题字体颜色
		void setTitleFontColor(QColor color);
		//设置绘图区的背景颜色
		void setBackGround(QColor color);
		//设置绘图区域网格化
		void setPlotGrid(bool isgrid);
		//设置Legend位置 0-none 1-右上 2-右下 3-左上 4-左下
		void setLegendPosition(int p);
		//设置坐标轴个数
		void setAxisNum(int num);
		//设置坐标轴名称
		void setAxisName(QString axis, QString name);
		//设置坐标轴字体颜色
		void setAxisFontColor(QString axis, QColor color);
		//设置坐标轴字体大小
		void setAxisFontSize(QString axis, int size);
		//设置坐标轴字体 0-宋体 1-黑体 2-Arial
		void setAxisFont(QString axis, int font);
		//设置曲线颜色
		void setCurveColor(QString fileName, int colIndex, QColor color);
		//设置线型
		void setCurveStyle(QString fileName, int colIndex, Qt::PenStyle style);
		//设置线宽
		void setCurveWidth(QString fileName, int colindex, int width);
		//是否显示数据点
		void setCurveShowPoint(QString fileName, int colindex, bool show);
		//设置曲线名称
		void setCurveName(QString fileName, int colindex, QString name);
		//设置曲线坐标轴索引
		void setCurveAxisIndex(QString fileName, int colindex, int axisIndex);
		//获取模型树窗口
		QWidget* getTreeWidget();
		//获取属性窗口
		QWidget* getPropWidget();
		///设置背景颜色
		void setBackGround(QColor color1, QColor color2) override;
		///保存图片
		void saveImage(QString fileName, int width, int heigh, bool showDlg) override;
		///设置坐标轴范围  axis-x y x2 y2  reange[0]-min range[1]-max
		void setAxisRange(QString axis, double range[2]);
		///获取属性列名称
		QStringList getColumnNameList(QString fileName);
		//语言切换
		void reTranslate() override;
		//获取曲线属性接口
		curve_line_data getCurveProp(QString tepFileName, int colIndex);
		//开始动画
		bool startAnimate();
		//结束动画
		bool stopAnimate();
		//设置动点颜色
		void setAniPointColor(QString fileName, int colum, QColor color);
		//设置动点形状
		void setAniPointType(QString fileName, int colum, aniPointType type);


	private:
		Ui::Post2DWidget* _ui{};
		curve_MainWindow* _curveMainWindow{};

	};

}


#endif