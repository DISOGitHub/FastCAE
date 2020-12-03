#ifndef PIPELINEOBJECT_H
#define PIPELINEOBJECT_H

#include "global_define.h"
#include <QDateTime>
#include <QTime>
#include "qfile.h"
#include <QObject>
#include "qstring.h"
#include "heudatasrcio_global.h"
#include "PipelineObjProp.h"
#include "qvector.h"
#include "vtkActor.h"
#include "vtkMapper.h"
#include "vtkUnstructuredGrid.h"
#include "vtkDataSetMapper.h"
#include "vtkLookupTable.h"
#include <vtkAutoInit.h>
#include "vtkActorCollection.h"
#include "vtkCellArray.h"
#include "vtkDoubleArray.h"
#include "vtkFloatArray.h"
#include "vtkIntArray.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkProperty.h"
#include "vtkCommand.h"
#include "vtkCamera.h"
#include "vtkCellType.h"
#include "vtkCellData.h"
#include "vtkRectilinearGrid.h"
#include "vtkStructuredGrid.h"
#include "vtkDataSet.h"
#include "vtkScalarBarWidget.h"
#include "vtkObject.h"
#include "vtkScalarBarActor.h"
#include "vtkScalarBarWidget.h"
#include "vtkInformation.h"
#include "vtkColorTransferFunction.h"
#include "vtkPiecewiseFunction.h"
#include "vtkMultiBlockDataSet.h"
#include "vtkAppendFilter.h"
#include "vtkAssembly.h"
#include "vtkCubeAxesActor.h"

class PipelineObjProp;

class HEUDATASRCIOSHARED_EXPORT PipelineObject : public QObject
{
	Q_OBJECT
public:
	vtkFloatArray* getColumnData(QString colName);
	vtkDataSet* mPipelineDataSet;
	int numPoints_SeaBottom;
	vtkAssembly* model_ptr;
	vtkFloatArray* componentXYZ_arr;
	vtkFloatArray* func_getColorXYZComponent(vtkFloatArray* tep_arr, QString component);
	void func_ReInitLookupTable(QString tep_colName, QString val);
	void func_ReInitLookupOpacity(QString tep_colName, QString val);
	vtkSmartPointer<vtkLookupTable> MakeLUTFromCTF(vtkColorTransferFunction* colorTransferFunction,vtkPiecewiseFunction* opacityFunction, int tableSize);
	void update_fileList_byFileInterval(int val);
	Scalar_Vector_NameListData ReadVarsList(vtkDataSet* tep_dataSet);
	vtkColorTransferFunction* makeBlocksLUT();
	//////////////
	vtkDataSet* func_merge_blockDataSet(vtkMultiBlockDataSet* tep_blockDataSet);
	vtkDataSet* func_merge_idTypeBlockDataSet(QMap<QString, vtkSmartPointer<vtkIdTypeArray>> tep_idTypeArrayMap);
	bool func_update_mBaseGrid();
	bool func_isVector(QString colName, int flag_PointCell, vtkDataSet* tep_dataSet);//0point,1cell
	////////////////
	
	Def_GridType get_GridType(){ return mGridType; };
	void set_GridType(Def_GridType type){ mGridType = type; };
	bool getcolorColumnRange(QString colName,  double mRange[2]);
	vtkColorTransferFunction* getLookupTable(vtkDataSet* tep_mBaseGrid, QString colName);
	bool get_column_range(vtkDataSet* tep_mBaseGrid, QString colName, double *tep_range);
	QMap<QString, vtkScalarBarWidget*> scalarBar_widgetMap;
	QMap<QString, vtkColorTransferFunction*> scalarBar_lookupTableMap;
	QMap<QString, vtkPiecewiseFunction*> scalarBar_lookupOpacityMap;
	void generate_scalarBar_lookupTableMap(vtkDataSet* tep_mBaseGrid);
	void func_update_componentXYX_lookupTableMap(QString key);
	bool flag_exist_actors;
	Scalar_Vector_NameListData scalar_vector_val;
	PipelineObjProp mPipeLineObjProp;
	QString mdefaultColumnName = "SolidColor";
	PipelineObject(PipelineObject* parent = 0);
	PipelineObject(QString& name, PipelineObject* parent = 0);
	virtual ~PipelineObject();
	QVector<vtkSmartPointer<vtkActor>> mVecActors;  
	vtkSmartPointer<vtkProperty> mBackFaceProp;
	PipelineObject* GetObjParent();
	void SetObjParent(PipelineObject* parent);

	vtkDataSet* mBaseGrid; 
	vtkDataSet* mBaseGridBak;
	vtkMultiBlockDataSet *mBlockDataSet;
	QMap<vtkDataSet*, QMap<QString, vtkSmartPointer<vtkIdTypeArray>>> dataSetMap;
	QMap<vtkDataSet*, QString> dataSetNameMap;
	QMap<QString, vtkSmartPointer<vtkIdTypeArray>> idTypeBlockDataSet;
	QMap<QString, vtkSmartPointer<vtkFloatArray>> blockValMap;
	int numberOfCgnsTimes;

	bool flag_update_mBaseGrid;
	double origin_center[3];
	double origin_center0[3];
	vtkSmartPointer<vtkDataSetMapper> m_glphMapper;
	void SetObjectType(Def_PipelineObjType type);
	Def_PipelineObjType GetObjectType();
	virtual void generate_actors();
	void generate_axesActor();
	void delete_axesActor();
	virtual QString GetName() const { return mName; }
	void SetName(const QString& name) { mName = name; }
	void ShowActor();
	void UpdatePropertyValueData();
	void SetLightingAmbient();
	void SetLightingAmbientColor();
	void SetLightingDiffuse();
	void SetLightingDiffuseColor();
	void SetOpacity();	
	void SetRenderMode();
	void SetEdgeColor();
	void SetOrientation();
	void SetOrigin();
	void SetPointSize();
	void SetLineWidth();
	void SetScale();
	void SetTranslate();	
	void SetLightingSpecular();	
	void SetLightingSpecularPower();//光照高光（亮斑）光洁度，虽然英文是power，但是实际上这个参数应该是phong光照模型中的光洁度，范围0.0~128.0，注意值很低，比如小于5.0时效果会很奇怪
	void SetLightingSpecularCorlor();//光照高光（亮斑）颜色
	void SetSolidCorlor();
	void SetEdgeCorlor();
	void SetLightingInterpolation();
	void SetMooringSeaBottom();
	virtual void  ChangeColumnColor();
	virtual void func_update_xyzColorColumn(vtkDataSet* tep_dataSet);
	virtual vtkDataSet* get_pipelineObj_dataSet() { return mPipelineDataSet; };
	void func_ChangeColumnColor();
	QByteArray  QStringToChar(QString sourceStr);
	void init_subObject_count();
	bool get_prop_val(vtkIdType tep_id,bool flag_point,double* val);
	vtkPiecewiseFunction* getLookupOpacity(vtkDataSet* tep_mBaseGrid, QString colName);
	vtkUnstructuredGrid* getExtractUnstructuredGrid(QString zoneName, vtkSmartPointer<vtkIdTypeArray> ids);
	double model_move_para[6];
	vtkSmartPointer<vtkCubeAxesActor> get_axesActor() { return axesActor; };
	void func_update_displacement(int cur_frame_xh);
	void func_init_displacement();
	//void func_axesVal_lookupTableMap();
	vtkSmartPointer<vtkFloatArray> tep_arrX;
	vtkSmartPointer<vtkFloatArray> tep_arrY;
	vtkSmartPointer<vtkFloatArray> tep_arrZ;
	void func_initGlyphVectorArr(vtkDataSet* tep_dataSet);
	void func_insertGlyphVector_lookupTableMap(vtkFloatArray* tep_arr);
	void func_insertCalculatorResult_lookupTableMap(vtkFloatArray* tep_arr);
	vtkFloatArray* glyphVector_arr;

	QMap<QString, ScalarBarPosition_Para> scalarBar_positionMap;
	reflection_PropData m_reflectionPara;
	vtkDataSet* func_getReflectionDataset(vtkDataSet* tep_dataSet);
	int get_objID() { return mPipeLineObjProp.pipelineObj_base_propData.obj_id; };
private:
	Def_GridType mGridType;
	Def_PipelineObjType mObjectType;
	QString mName;
	PipelineObject* mParent;
	void SetPosition();
	vtkSmartPointer<vtkUnstructuredGrid> UnStructuredGrid;
	vtkSmartPointer<vtkRectilinearGrid> mRectilinearGrid;
	vtkSmartPointer<vtkStructuredGrid>  mStructuredGrid;
protected:
	vtkSmartPointer <vtkActor> selectActor;
	vtkSmartPointer<vtkCubeAxesActor> axesActor;
	
};
#endif


