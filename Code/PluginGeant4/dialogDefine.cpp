#include "ui_dialogDefine.h"
#include "dialogDefine.h"
#include "DefineManager.h"
#include "DataManager.h"
#include "Value3D.h"
#include "Position.h"
#include "Rotation.h"
#include "Expression.h"
#include "Quantity.h"
#include "Constant.h"
#include <QStringList>


namespace Geant4
{

	CreateDefineDialog::CreateDefineDialog(PreDefineBase* pd):m_defineData(pd),m_type(m_defineData->getType())
	{
		_ui = new Ui::dialogDefine;
		_ui->setupUi(this);

		Qt::WindowFlags flags = Qt::Dialog;
		flags |= Qt::WindowCloseButtonHint;
		setWindowFlags(flags);

		this->init();
	}

	void CreateDefineDialog::init()
	{
		setTitle();
		displayInterface();
		displayPreDefineData();
	}

	void CreateDefineDialog::closeEvent(QCloseEvent *)
	{
		
	}

	void CreateDefineDialog::reject()
	{
		this->close();
	}

	void CreateDefineDialog::accept()
	{
		updataPreDefineData();		

		QDialog::accept();
		this->close();
	}

	void CreateDefineDialog::setTitle()
	{
		switch (m_type)
		{
		case DataType::ConstantType:
			this->setWindowTitle(tr("Constant")); 
			this->resize(270, 70); break;
		case DataType::ExpressionType:
			this->setWindowTitle(tr("Expression"));
			this->resize(270, 70); break;
		case DataType::QuantityType:
			this->setWindowTitle(tr("Quantity"));
			this->resize(270, 120); break;
		case DataType::PositionType:
			this->setWindowTitle(tr("Position")); break;
		case DataType::RotationType:
			this->setWindowTitle(tr("Rotation")); break;
		default:
			break;
		}
	}

	void CreateDefineDialog::displayInterface()
	{
		switch (m_type)
		{
		case DataType::ConstantType:
		case DataType::ExpressionType:
			_ui->label_5->setVisible(false);
			_ui->unitComboBox->setVisible(false);
		case DataType::QuantityType:
			_ui->label->setText(tr("Value"));
			_ui->label_2->setVisible(false);
			_ui->yValue->setVisible(false);
			_ui->label_3->setVisible(false);
			_ui->zValue->setVisible(false);
// 		case DataType::PositionType:
// 		case DataType::RotationType:
		default:
			break;
		}
	}

	void CreateDefineDialog::displayPreDefineData()
	{
		_ui->name->setText(m_defineData->getName());
		QStringList list;
		if (m_type == DataType::ConstantType){
			Constant* con = dynamic_cast<Constant*>(m_defineData);
			_ui->xValue->setText(con->getValue());
		}
		else if (m_type == DataType::ExpressionType){
			Expression* exp = dynamic_cast<Expression*>(m_defineData);
			_ui->xValue->setText(exp->getExpression());
		}	
		else if (m_type == DataType::QuantityType){
			Quantity* qua = dynamic_cast<Quantity*>(m_defineData);
			_ui->unitComboBox->setCurrentText(qua->getUnit());
			_ui->xValue->setText(qua->getValueString());
		}
		else if (m_type == DataType::PositionType){
			Position* pos = dynamic_cast<Position*>(m_defineData);
			_ui->unitComboBox->setCurrentText(pos->getUnit());
			pos->getValue(list);
		}	
		else if (m_type == DataType::RotationType){
			Rotation* rot = dynamic_cast<Rotation*>(m_defineData);
			_ui->unitComboBox->setCurrentText(rot->getUnit());
			rot->getValue(list);
		}
		
		if (m_defineData->getDim() == 3)
		{
			_ui->xValue->setText(list.at(0));
			_ui->yValue->setText(list.at(1));
			_ui->zValue->setText(list.at(2));
		}
	}

	void CreateDefineDialog::updataPreDefineData()
	{
		QString x = _ui->xValue->text();	   
		QString y = _ui->yValue->text();	   
		QString z = _ui->zValue->text();	   
		if (m_type == DataType::ConstantType) {
			Constant* con = dynamic_cast<Constant*>(m_defineData);
			con->setValue(_ui->xValue->text().trimmed());
		}
		else if (m_type == DataType::ExpressionType) {
			Expression* eps = dynamic_cast<Expression*>(m_defineData);
			eps->setExpression(_ui->xValue->text().trimmed());
		}
		else if (m_type == DataType::QuantityType) {
			Quantity* qua = dynamic_cast<Quantity*>(m_defineData);
			qua->setValue(x); qua->setUnit(_ui->unitComboBox->currentText());
		}
		else if (m_type == DataType::PositionType) {
			Position* pos = dynamic_cast<Position*>(m_defineData);
			pos->setValue(x,y,z); pos->setUnit(_ui->unitComboBox->currentText());
		}
		else if (m_type == DataType::RotationType) {
			Rotation* rot = dynamic_cast<Rotation*>(m_defineData);
			rot->setValue(x,y,z); rot->setUnit(_ui->unitComboBox->currentText());
		}		
	}

}