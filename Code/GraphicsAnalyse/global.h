#ifndef GLOBAL_H//
#define GLOBAL_H
#include "qcolor.h"
#include "qstring.h"
#include "qstringlist.h"
#include "qcolor.h"
#include "qwidget.h"
#include "qvector.h"
#include "vtkSmartPointer.h"
#include "vtkRenderer.h"
#include "vtkPlanes.h"
#include "PipelineObject.h"
//typedef struct FrameDisplace_struct
//{
//	float time;
//	float Surge_GX;
//	float Sway_GY;
//	float Heave_GZ;
//	float Roll_Gxb;
//	float Pitch_Gyb;
//	float Yaw_Gzb;
//} FrameDisplace;
typedef struct refrence_point1
{
	vtkIdType id;
	double x;
	double y;
	double z;
} refrence_point;

typedef struct font_prop1
    {
        int font_type;
        QColor font_color;
        int font_size;
    } font_prop;
typedef struct remark_prop1
{
	QColor fontColor;
	int fontSize;
	bool flag_remark;
	QString text;
	double position1[2];
	double position2[2];
} remark_prop;
typedef struct renderview_global_prop1
    {
        int back_color_type;//0,1,2
        QColor back_color1;
        QColor back_color2;
        QString image_file;
		bool flag_axesVisible;
		bool flag_viewInteraction;
		bool flag_camera_parallel;
		remark_prop remark_para;
    } renderview_global_prop;
typedef struct saveImage_prop1
{
	int image_wid;
	int image_hei;
	bool flag_saveImage;
	QString saveImage_fileName;
	bool flag_savePov;
	QString savePov_fileName;
} saveImage_prop;
typedef struct saveScript_prop1
{
	int image_wid;
	int image_hei;
	bool flag_saveImage;
	QString saveImage_fileName;
	bool flag_saveAvi;
	QString saveAvi_fileName;
	QString saveScript_fileName;
	bool flag_autoExit;
	int timeMS_autoExit;
	bool flag_plotSize;
	int plot_wid;
	int plot_hei;
	int tree_wid;
	int tree_hei;
	int prop_wid;
	int prop_hei;
	int console_wid;
	int console_hei;
	int mainwindow_wid;
	int mainwindow_hei;
	bool flag_adapt;
} saveScript_prop;
typedef struct saveAni_prop1
{
	bool flag_save;
	double frame_rate;
	int frame_num;
	int frame_step;
	int frame_start_xh;
	int frame_end_xh;
	int image_wid;
	int image_hei;
	int total_frame_num;
	QString avi_filename;

	bool flag_saveImage;
	QString saveImage_format;
	QString saveImage_path;

	bool flag_savePov;
	QString savePov_path;

	int file_interval;
} saveAni_prop;

enum EMsgType
{
	kMsg_Error = 0,
	kMsg_Warning,
	kMsg_Info
};
enum ActorOperateType
{
	kActor_Move = 0,
	kActor_Rotate,
	kActor_RotateX,
	kActor_RotateY,
	kActor_RotateZ
};
typedef struct Camera_Para1
{
	double position[3];
	double focalPoint[3];
	double viewUp[3];
	double viewAngle;
	double zoom;
	double clippingRange[2];
	bool flag_parallel;
	double parallel_scale;
} Camera_Para;



struct pick_pipelineObj_propData
{
	int flag_pickMode;//-1无拾取，0点，1单元，2区域点，3区域单元
	vtkActor *pick_actor;
	vtkIdType pick_id;
	//vtkPlanes *frustum;
	PipelineObject* pick_pipelineObj;
	int *pickArea_objId;
	vtkIdList* pick_areaPoint;
	vtkIdList* pick_areaCell;
	QVector<double> pick_curve_data;
	bool flag_draw_curve;
	int *objId;
};
struct colorTransForm_val
{
	double r;
	double g;
	double b;
	double val;
};
struct opacityTransForm_val
{
	double opa;
	double val;
};
struct exitSoftWareScript_struct
{
	bool flag_exit;
	int ms;
};
typedef struct pov_prop1
{
	float mOrigin[3];
	float disAndang[9];
	QVector<vtkSmartPointer<vtkActor>> mVecActors;
	double pos[3];
} pov_prop;
#endif // GLOBAL_H

