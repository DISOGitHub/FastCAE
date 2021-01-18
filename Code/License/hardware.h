#ifndef HARDWARE_H
#define HARDWARE_H

#include <QStringList>
#include <QObject>

class Hardware : public QObject
{
	Q_OBJECT

public:
	Hardware();
	~Hardware();

	///<MG get network card 's mac address
	QString getMacAddress();

	///<MG get disk serial id
	QStringList getDiskSerialId();

	///<MG get central processing unit id
	QString getCentralProcessingUnitId();


private:
	///<MG restore the central processor unit id
	QString m_cpuID;

	///<MG restore the disk serial id
	QStringList m_diskSerialID;

	///<MG restore the mac address
	QString m_macAddress;
};

#endif // HARDWARE_H
