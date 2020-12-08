#ifndef CURVESTRUCT_H
#define CURVESTRUCT_H
#include "qcolor.h"
#include "qstring.h"
#include "qstringlist.h"
#include "qcolor.h"
#include "qwidget.h"
#include "qvector.h"
#include "qcustomplot.h"
#define MAX_COL_NUM 100
#define MAX_AXIS_NUM 2

enum aniPointType
{
	dot,
	cross,
	plus,
	circle,
	disc,
	square,
	diamond,
	star,
	triangle,
};
enum plotProp_legendPostion
{
	none,
	right_top,
	rignt_bottom,
	left_top,
	left_bottom,
};
enum plotProp_dataSource
{
	Index,
	File,
};
enum plotProp_fontType
{
	Song,
	Hei,
	Arial,
};
    typedef struct curve_font_prop1
    {
        int font_type;
        QColor font_color;
        int font_size;
    } curve_font_prop;
    typedef struct curve_line_data1
    {
		int axis_index;//x轴索引,坐标轴1还是2
        QString yAxis_name;
        QString yAxis_column_name;//数据源
        int yAxis_column;//数据源
        double yAxis_coor_min;
        double yAxis_coor_max;
        QColor curve_color;
		Qt::PenStyle curve_lineType;
        int curve_lineWidth;
        bool flag_showPoint;

		aniPointType aniPoint_type;
		QColor aniPoint_color;
    } curve_line_data;
	typedef struct curve_file_prop1
	{
        QString Axis_filename;
		QStringList columns_name_list;
		double val_max[MAX_COL_NUM];
		double val_min[MAX_COL_NUM];
		QVector<double> column_val_data[MAX_COL_NUM];
		int num_column;
		int num_point;
	} curve_file_prop;
	typedef struct global_plot_axis_prop1
	{
		QString axis_name;
		plotProp_dataSource axis_dataSource;//data,file
		int axis_fileIndex;//QString axis_fileName;
		int axis_fileColumn;
		curve_font_prop axis_font;
		double axis_range[2];
	} global_plot_axis_prop;
    typedef struct curves_data1
    {
        QString title;
        curve_font_prop title_font;
        int file_num;
        QColor backColor;
		plotProp_legendPostion flag_legend;
        bool flag_grid;
		int num_axis;
		global_plot_axis_prop glo_xAxis_prop_data[MAX_AXIS_NUM];
		global_plot_axis_prop glo_yAxis_prop_data[MAX_AXIS_NUM];

        QStringList curve_fileNameList;
    } global_curves_data;
    
    typedef struct curve_file_data1
    {
        curve_file_prop file_prop;
        QVector<curve_line_data> line_data;
    } curve_file_data;
    typedef struct curve_ini_data1
    {
        global_curves_data glo_curve_data;
        QVector<curve_file_data> curve_file_list;
		QMap<QString, QCPCurve*> curveList;
    } curve_ini_data;
	typedef struct curve_saveImage_prop1
	{
		int image_wid;
		int image_hei;
		QString saveImage_fileName;
	} curve_saveImage_prop;
	typedef struct curve_aniPoint_struct1
	{
		QVector<QPointF> aniPoint_CoorList;
		QVector<QColor> aniPoint_ColorList;
		QVector<aniPointType> aniPoint_TypeList;
		QVector<QString> aniPoint_IdList;
		QVector<QString> aniPoint_CurveNameList;
		QVector<int> axis_indexList;//x轴索引,坐标轴1还是2 
	} curve_aniPoint_struct;
	
#endif // GLOBAL_H

