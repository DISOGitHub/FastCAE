#ifndef COMMON_H
#define COMMON_H

#include "QString"
#include "QMap"
#include "QStringList"
#include "QVector"
#include "QTreeWidget"
#include <assert.h>

//�������鳤��
#define CountArray(Array) (sizeof(Array)/sizeof(Array[0]))
//ɾ��ָ��
#define SafeDelete(pData) { try { delete pData; } catch (...) { ASSERT(FALSE); } pData=NULL; } 
//ɾ������
#define SafeDeleteArray(pData) { try { delete [] pData; } catch (...) { ASSERT(FALSE); } pData=NULL; } 


#define _APP_NAME_							"FastCAEDesigner"
#define _APP_NUMBER_						3

#define _TREE_ITEM_STATE_ROLE_				Qt::UserRole + 0x0100
#define _TREE_SELF_ITEM_EN_NAME_			Qt::UserRole + 0x0101
#define _LIST_ITEM_TYPE_					Qt::UserRole + 0x0102
#define _TREE_DEFAULT_ITEM_TYPE_			Qt::UserRole + 0x0103

#define _CONFIG_FILES_						"ConfigFiles"


#define _BASE_SET_							QString::fromLocal8Bit("��������")
#define _GEOMETRY_SET_						QString::fromLocal8Bit("��������")
#define _GRID_SET_							QString::fromLocal8Bit("��������")
#define _SOLVE_SET_							QString::fromLocal8Bit("�������")
#define _POST_SET_							QString::fromLocal8Bit("��������")
#define _FUNCTION_SET_						QString::fromLocal8Bit("���ܲ���")
#define _PRE_VIEW_							QString::fromLocal8Bit("Ԥ��")

#define _PARA_RELEVANT_						QString::fromLocal8Bit("��������")
#define _MATERIAL_SET_						QString::fromLocal8Bit("��������")

#define _INTRODUCE_COMMON_					QString::fromLocal8Bit("��Ϣ")

#define _USER_MANUAL_PDF_					"UserManual.pdf"

#define Image_Mesh         ":/QUI/icon/mesh.png"
#define Image_Simulation   ":/QUI/icon/solumationsetting.png"
#define Image_BC           ":/QUI/icon/bc.png"
#define Image_Solve        ":/QUI/icon/solve.png"
#define Image_Monitor      ":/QUI/icon/monitor.png"
#define Image_Post         ":/QUI/icon/post.png"
#define Image_Curve        ":/QUI/icon/curve.png"
#define Image_Vector       ":/QUI/icon/vector.png"
#define Image_Physics      ":/QUI/icon/physics.png"
#define Image_Material     ":/QUI/icon/material.png"

#define TChinese 0
#define TEnglish 1

#define TSimulationData 0
#define TSolverData     1



#define _ALL_ITEM_SETTED_					"All Setted"



//�߽��������������ݽṹ

#define _PROJECT_PATH_					"/Designer/"
#define _LOGO_IMAGE_NAME_				"logo_image"
#define _WELCOME_IMAGE_NAME_			"welcome_image"

#define _CAE_NAME_						"FastCAE.exe"


//////////////////////////////////////////////////////////////////////////
#define _CONFIG_BC_						"BCConfig.config"
#define _CONFIG_DATA_					"DataConfig.config"
#define _CONFIG_TREE_					"treeConfig.config"
#define _CONFIG_DESIGNER_				"DESIGNER.XML"
#define _CONFIG_GLOBAL                  "GlobalConfig.config"
#define _CONFIG_MATERIAL_               "MaterialConfig.config"


///<MG �������������ļ�
#define _CONFIG_RELEVANT_				"ObserverConfig.config"

///<MG ���������ļ�
#define _CONFIG_MATERIAL_				"MaterialConfig.config"

#define _DEFAULT_PROJECT_ROOT_			QString::fromLocal8Bit("����")
#define _DEFAULT_MESH_PART_				QString::fromLocal8Bit("�������")
#define _DEFAULT_SIM_PARA_				QString::fromLocal8Bit("�����������")
#define _DEFAULT_BOUNDARY_CONDITION_	QString::fromLocal8Bit("�߽�����")
#define _DEFAULT_SOLVER_SETTING_		QString::fromLocal8Bit("����������")
#define _DEFAULT_MONITOR_				QString::fromLocal8Bit("�����")
#define _DEFAULT_POST_					QString::fromLocal8Bit("����")
#define _DEFAULT_2D_PLOT_				QString::fromLocal8Bit("��ά����")
#define _DEFAULT_3D_GRAPH_				QString::fromLocal8Bit("��ά����")

#define _DEFAULT_PROJECT_ROOT_EN_		"Case"
#define _DEFAULT_MESH_PART_EN_			"Mesh Set"
#define _DEFAULT_SIM_PARA_EN_			"Simulation Setting"
#define _DEFAULT_BOUNDARY_CONDITION_EN_	"Boundary Condition"
#define _DEFAULT_SOLVER_SETTING_EN_		"Solver Setting"
#define _DEFAULT_MONITOR_EN_			"Monitor"
#define _DEFAULT_POST_EN_				"Post"
#define _DEFAULT_2D_PLOT_EN_			"2D Plot"
#define _DEFAULT_3D_GRAPH_EN_			"3D Graph"

#define _BOOL_PARA_NAME_				QString::fromLocal8Bit("��������")
#define _INT_PARA_NAME_					QString::fromLocal8Bit("���β���")
#define _DOUBLE_PARA_NAME_				QString::fromLocal8Bit("�������")
#define _ENUM_PARA_NAME_				QString::fromLocal8Bit("ö�ٲ���")
#define _STRING_PARA_NAME_				QString::fromLocal8Bit("�ַ�������")
#define _TABLE_PARA_NAME_				QString::fromLocal8Bit("�����Ͳ���")
#define _PATH_PARA_NAME_				QString::fromLocal8Bit("·������")
#define _TIME_PARA_NAME_				QString::fromLocal8Bit("ʱ�����")
#define _2D_POINT_PARA_					QString::fromLocal8Bit("��ά�����")
#define _3D_POINT_PARA_					QString::fromLocal8Bit("��ά�����")


#define _TREE_CONFIG_FILE_				"treeConfig.config"
#define _DATA_CONFIG_FILE_				"DataConfig.config"
#define _GLOBAL_CONFIG_FILE_			"GlobalConfig.config"
#define _BC_CONFIG_FILE_				"BCConfig.config"
#define _SOLVER_CONFIG_FILE_			"SolverConfig.config"

#define _CONFIG_SIMLUTATION_SETTING_					"SimulationSetting"
#define _CONFIG_SIMLUTATION_SETTING_CHILD_				"SimulationSettingChild"
#define _CONFIG_SIMLUTATION_SETTING_GRANDSON_			"SimulationSettingGrandSon"

#define _CONFIG_PROJECT_ROOT_							"ProjectRoot"
#define _CONFIG_PROJECT_CHILD_							"ProjectChild"
#define _CONFIG_PROJECT_GRANDSON_						"ProjectGrandSon"

#define _CONFIG_SOLVER_SETTING_							"SolverSetting"
#define _CONFIG_SOLVER_SETTING_CHILD_					"SolverChild"
#define _CONFIG_SOLVER_SETTING_GRNADSON_				"SolverSettingGrandSon"

///<MG ����Parent
#define _CONFIG_SOLVER_SETTING_SUB_PARENT_				"Solver Setting"
#define _CONFIG_SIMULATION_SETTING_SUB_PARENT_			"Simulation Setting"

#define _CONFIG_POST_2D_CHILD_							"Post2DChild"
#define _CONFIG_POST_3D_CHILD_							"Post3DChild"


#define _ICON_PATH_										"icon/"


#define _MENU_ITEM_USE_									"1"




#endif