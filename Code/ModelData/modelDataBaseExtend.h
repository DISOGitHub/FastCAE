#ifndef _MODELSATABASEEXTEND_H_
#define _MODELSATABASEEXTEND_H_

#include "modelDataAPI.h"
#include "modelDataBase.h"
#include <QStringList>
#include <QList>
#include <QHash>

class QDomDocument;
class QDomElement;

namespace DataProperty
{
	class DataBase;
	class ParameterBase;
	class ParameterGroup;
}

namespace ConfigOption
{
	class PostCurve;
	class ParameterObserver;
}

namespace Post
{
	class Post2DWindowInterface;
	class Post3DWindowInterface;
}

namespace ModelData
{
	class MODELDATAAPI ModelDataBaseExtend : public ModelDataBase
	{
	public:
		ModelDataBaseExtend(ProjectTreeType type);
		~ModelDataBaseExtend();

		virtual void copyFormConfig() override;
		virtual void generateParaInfo() override;
		//��ӱ���
		void appendReport(QString report);
		//��ȡ��������
		int getReportCount();
		//��ȡ��index������
		QString getReportAt(int index);
		//�Ƴ���index������
		void removeReportAt(int index);
		//���ò��ϣ��������ID�����ID
		void setMaterial(int setID, int materialID);
		//��ȡ����Ĳ���ID
		int getMaterialID(int setid);
		//����Ƿ����ò���
		bool isMaterialSetted(int setid);
		//�������������İ�
		void removeMaterial(int setid);
		
		virtual void setComponentIDList(QList<int> ids);

		virtual void removeComponentAt(int index) override;

		virtual QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* ele) override;
		virtual void writeToProjectFile1(QDomDocument* doc, QDomElement* ele) override;
		virtual void readDataFromProjectFile(QDomElement* e) override;
		virtual void writeToSolverXML(QDomDocument* doc, QDomElement* e) override;

		virtual void dataToStream(QDataStream* datas) override;

		void appendConfigData(int dataID, DataProperty::DataBase* d);
		DataProperty::DataBase* getConfigData(int dataid);
		//��ȡ�ڵ�����
		int getConfigDataCount();
		//��ȡ����ļ�������
		QStringList getMonitorFile();
		//��ȡ����ļ��ľ���·��
		QStringList getAbsoluteMonitorFile();
		//�����ļ���ȡ��صı����б�
		QStringList getMonitorVariables(QString file);
		//��ȡ�������
		QList<ConfigOption::PostCurve*> getMonitorCurves();
		//��ȡ��index�ļ������
		ConfigOption::PostCurve* getMonitorCurveAt(const int index);
		//��ȡ�����ά�����ļ�
		QStringList getPost2DFiles();
		//��ȡ�����ά�����ļ��ľ���·��
		QStringList getAbsolutePost2DFiles();
		//�����ļ���ȡ��ά�����ļ��еı���
		QStringList getPost2DVariables(QString f);
		//��ȡ��ά�����ļ�
		QString getPost3DFile();
		//��ȡ��ά�������
		void get3DScalars(QStringList &node, QStringList &ele);
		//��ȡ��ά��������
		void get3DVector(QStringList &node, QStringList &ele);
		//��Ӷ�ά��������
		void apppendPlotCurve(ConfigOption::PostCurve* c);
		//��ȡ�ѻ�������
		QList<ConfigOption::PostCurve*> getPlotCurves();
		//�Ƴ���ά����
		void removePlotCurve(int index);
		//���ȫ����ά����
		void clearPlotCurve();
		//���������Ƿ����
		bool isPostCurveExist(QString name);
		//��ӱ�����������
		void appendScalarVariable(QString v);
		//��ȡ������������
		QStringList getScalarVariable();
		//�Ƴ���index������
		void removeScalarVariable(int index);
		//���������������
		void appendVectorVariable(QString v);
		//��ȡ������������
		QStringList getVectorVariable();
		//�Ƴ���index����������
		void removeVectorVariable(int index);
		//�����ά����
		void clear3DVariable();
		DataProperty::ParameterBase* getParameterByName(QString name) override;
		DataProperty::ParameterGroup* getParameterGroupByName(QString name) override;
		virtual void removeParameter(DataProperty::ParameterBase* p) override;
		virtual void removeParameterGroup(DataProperty::ParameterGroup* g) override;
		//���ö�ά��ͼ����
		void setPost2DWindow(Post::Post2DWindowInterface* p2d);
		//��ȡ��ά����
		Post::Post2DWindowInterface* getPost2DWindow();
		//������ά��Ⱦ����
		void setPost3DWindow(Post::Post3DWindowInterface* p3d);
		//��ȡ��ά��Ⱦ����
		Post::Post3DWindowInterface* getPost3DWindow();

		void bindInpMaterialIds(const QList<int>&);
		const QList<int>& getInpMaterialIds();

	protected:
		void registerObserver();
		
	protected:
		QList<int> _inpMaterIds;
		QHash<int, int> _setMaterial{};
		QStringList _reportList{};
		QHash<int, DataProperty::DataBase*> _configData{};
		QStringList _monitorFiles{};
		QList<ConfigOption::PostCurve*> _monitorCurves{};
		QList<ConfigOption::PostCurve*> _postCurves{};
//		QStringList _plot2DVariable{};
		QStringList _scalarVariable{};
		QStringList _vectorVariable{};
		QList<ConfigOption::ParameterObserver*> _observerList{};

		Post::Post2DWindowInterface* _post2DWindow{};
		Post::Post3DWindowInterface* _post3DWindow{};
	};


}

#endif
