#pragma once
#include <QWidget>
#include <QTreeWidgetItem>
#include "ui_Geant4PropertyWidget.h"
#include "Geant4DataModel.h"

namespace Geant4 {

	class Geant4PropertyWidget : public QWidget
	{
		Q_OBJECT
	public:
		Geant4PropertyWidget(QWidget* parent = nullptr);
		~Geant4PropertyWidget();

		void updateWith(_G4SourceType sourceType);
		void updateWith(_G4AngleDistribution angleDistribution);
		void updateWith(_G4SourceParameters sourceParameters);

		void updateWith(_G4PhysicalPart physicalPart);

		void updateWith(_G4GeometryEnergyThreshold enTh);

		void updateWith(int type, QTreeWidgetItem * item);

		void updateWith(_G4AnalysisItem analysisItem, const QString belong);

	private slots:
		void updateProperty(int type);
		void updateProperty(int type, QTreeWidgetItem * item);
		void slotItemValueChanged();
		void slotUpdateSelf();
		void cleanWidget();
//		void updateGeometrySoildNode();
	private:
		void resizeEvent(QResizeEvent *);

	private:
		Ui::PropTable _ui;
		void init();
		void addEnumLine(QString name, QStringList items, int currentIndex, QString sign, bool updateSelf = false, QString belong = QString());
		void addIntLine(QString name, int val, QString sign, int min = 0, int max = 0x0FFFFFFF, QString belong = QString());
		void addDoubleLine(QString name, double val, QString sign,int accuracy = 2, double min = 0, double max = 0xFFFFFFFF, QString belong = QString());
		void addPathLine(QString name, QString val, QString sign, QString belong = QString());
		void addRelationLine(const QString& objA, const QString& objB);
	};

}