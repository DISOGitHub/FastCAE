#ifndef MODELDATABASE_H
#define MODELDATABASE_H

#include "modelDataAPI.h"
#include "DataProperty/modelTreeItemType.h"
#include <QString>
#include "DataProperty/DataBase.h"
#include <QList>
#include "moduleBase/messageWindowBase.h"
#include "BCBase/BCType.h"

class QDataStream;
class QTextStream;
class QDomDocument;

namespace BCBase
{
	class BCBase;
}

namespace ModelData
{
	class SimlutationSettingBase;
	class SolverSettingBase;

	class MODELDATAAPI ModelDataBase : public DataProperty::DataBase
	{
	public:
		//���캯��
		ModelDataBase(ProjectTreeType treeType);
		virtual ~ModelDataBase();
		//�������ļ��п���
		virtual void copyFormConfig();
		//��ID��������Ϣ���ݸ�����ĸ�������
		virtual void generateParaInfo() override;
		//��ȡ����ID
		static int getMaxID();
		void setID(int id) override;
		//��ȡ����·��
		QString getPath();
		static void resetMaxID();
		//��������
		void setTreeType(ProjectTreeType type);
		//��ȡ����
		ProjectTreeType getTreeType();
		//������ʱ��
		void setSolveTime(double t);
		//��ȡ������ʱ��
		double getSolveTime();
		//����д������������ļ�����
		void setOutputFileName(QString name);
		QString& getOutputFileName();
		/*��ȡMD5��stream*/
		virtual void dataToStream(QDataStream* datas) override;
		//����д���������ļ� ���������д�ú���
		virtual QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* ele) override;
		//����д���������ļ� ���������д�ú���
		virtual void writeToProjectFile1(QDomDocument* doc, QDomElement* ele);
		///�ӹ����ļ���������ݣ����������д�˺���
		virtual void readDataFromProjectFile(QDomElement* e) override;
		///������д���ı��������
		virtual void writeToSolverText(QTextStream* stream);
		///������д��XML�������
		virtual void writeToSolverXML(QDomDocument* doc, QDomElement* e);
		//��������Ƿ������� ����True-������⣻False-�������
		virtual bool checkSolveableStatus(QVector<ModuleBase::Message> & messages);
		
		//��ȡBC����
		int getBCCount();
		//���BC
		void appeendBC(BCBase::BCBase* bc); 
		//�������ͻ�ȡBC
		QList<BCBase::BCBase*> getBCByType(BCBase::BCType type);

		//��ȡ��index��BC
		BCBase::BCBase* getBCAt(const int index);
		//�Ƴ��߽�����
		void removeBCAt(const int index);
		//ͨ�����IDɾ��BC��ÿ��BC�����һ�����
		void removeBCByComponentID(int);
		//������Ҫ���������ID(������������ͼ������)
		void setComponentIDList(const QList<int>& ids);
		//���һ�����ID
		void addComponentID(int cpId);
		//��ȡ���������ID(������������ͼ������)
		const QList<int>& getComponentIDList();
		//ͨ��IDɾ�����
		bool removeComponentByID(int);

		//��ȡ�������������ID
		QList<int> getMeshSetList();
		//��ȡ�����ļ������ID
		QList<int> getGeoComponentIDList();

		//������Ҫ�������������ID
		//virtual void setMeshSetList(QList<int> ids);
		//������Ҫ�����ļ������ID
		//void setGeoComponentIDList(QList<int> ids);
		
		//�Ƴ���index�����
		virtual void removeComponentAt(int index);
		//��ȡ������Kernal ID
		QList<int>& getMeshKernalList();
		//���ù�����Kernal ID
		void setMeshKernelList(const QList<int>& kids);
		//��ӹ�����Kernal ID
		void addMeshKernalId(const int);
		//��ȡ�����ļ�����״
		QList<int>& getGeometryList();
		//���ù����ļ�����״
		void setGeometryList(QList<int> geo);
		//��ȡ�������
		SimlutationSettingBase* getSimlutationSetting();
		//��ȡ���������
		SolverSettingBase* getSolverSetting();
		//����Ƿ�ʹ��
		bool isComponentUsed(int index);

		virtual DataProperty::ParameterBase* getParameterByName(QString name) override;
		virtual void removeParameter(DataProperty::ParameterBase* p) override;
		virtual DataProperty::ParameterGroup* getParameterGroupByName(QString name) override;
		virtual void removeParameterGroup(DataProperty::ParameterGroup* g) override;


	private:
		ProjectTreeType getTreeTypeByString(const QString& type);
		QString getTreeTypeToSring(ProjectTreeType type);

	protected:
		ProjectTreeType _treeType{ UnDefined };
		static int maxID;
		double _solvetime{ -1.0 };
		QString _outputFileName{};

		QList<BCBase::BCBase*> _bcList{};
//		QMultiHash<int, QString> _ComponentIDType{};
		QList<int> _ComponentIDList{};
		QList<int> _meshKernalIDList{};
		QList<int> _geometryList{};

		SolverSettingBase* _solverSetting{};
		SimlutationSettingBase* _simlutationSetting{};
	};

}


#endif
