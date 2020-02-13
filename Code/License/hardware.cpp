#include "hardware.h"

#ifdef Q_OS_WIN
#include <snmp.h>
#pragma comment(lib, "snmpapi.lib")
#pragma comment(lib, "Ws2_32.lib")
#include <windows.h>
#include <stdio.h>
#include <comdef.h>
#include <Wbemidl.h>
# pragma comment(lib, "wbemuuid.lib")
#elif defined Q_OS_LINUX
#include <uuid.h>
#include <stdio.h>
#endif
#include "global.h"

Hardware::Hardware()
{

}

Hardware::~Hardware()
{

}

QString Hardware::getMacAddress()
{
	if (!m_macAddress.isEmpty())
	{
		return m_macAddress;
	}
#ifdef Q_OS_WIN
	QString result;
	WSADATA WinsockData;
	if (WSAStartup(MAKEWORD(2, 0), &WinsockData) != 0)
		return false;

	// Load the SNMP dll and get the addresses of the functions necessary
	const HINSTANCE m_dll = LoadLibraryA("inetmib1.dll");
	if (m_dll < (HINSTANCE)HINSTANCE_ERROR)
		return false;

	const PFNSNMPEXTENSIONINIT f_SnmpExtensionInit = (PFNSNMPEXTENSIONINIT)GetProcAddress(m_dll, "SnmpExtensionInit");
	const PFNSNMPEXTENSIONINITEX f_SnmpExtensionInitEx = (PFNSNMPEXTENSIONINITEX)GetProcAddress(m_dll, "SnmpExtensionInitEx");
	const PFNSNMPEXTENSIONQUERY f_SnmpExtensionQuery = (PFNSNMPEXTENSIONQUERY)GetProcAddress(m_dll, "SnmpExtensionQuery");
	const PFNSNMPEXTENSIONTRAP f_SnmpExtensionTrap = (PFNSNMPEXTENSIONTRAP)GetProcAddress(m_dll, "SnmpExtensionTrap");
	HANDLE pollForTrapEvent;
	AsnObjectIdentifier supportedView;
	f_SnmpExtensionInit(GetTickCount(), &pollForTrapEvent, &supportedView);

	// Initialize the variable list to be retrieved by f_SnmpExtensionQuery
	const AsnObjectIdentifier MIB_NULL = { 0, 0 };

	RFC1157VarBind varBind[2];
	varBind[0].name = MIB_NULL;
	varBind[1].name = MIB_NULL;

	RFC1157VarBindList varBindList;
	varBindList.list = varBind;

	UINT OID_ifEntryType[] = { 1, 3, 6, 1, 2, 1, 2, 2, 1, 3 };
	UINT OID_ifEntryNum[] = { 1, 3, 6, 1, 2, 1, 2, 1 };
	UINT OID_ipMACEntAddr[] = { 1, 3, 6, 1, 2, 1, 2, 2, 1, 6 };
	AsnObjectIdentifier MIB_ifMACEntAddr = { sizeof(OID_ipMACEntAddr) / sizeof(UINT), OID_ipMACEntAddr };
	AsnObjectIdentifier MIB_ifEntryType = { sizeof(OID_ifEntryType) / sizeof(UINT), OID_ifEntryType };
	AsnObjectIdentifier MIB_ifEntryNum = { sizeof(OID_ifEntryNum) / sizeof(UINT), OID_ifEntryNum };

	// Copy in the OID to find the number of entries in the Inteface table
	varBindList.len = 1;        // Only retrieving one item
	SnmpUtilOidCpy(&varBind[0].name, &MIB_ifEntryNum);
	AsnInteger errorStatus;
	AsnInteger errorIndex;
	f_SnmpExtensionQuery(ASN_RFC1157_GETNEXTREQUEST, &varBindList, &errorStatus, &errorIndex);
	varBindList.len = 2;

	// Copy in the OID of ifType, the type of interface
	SnmpUtilOidCpy(&varBind[0].name, &MIB_ifEntryType);

	// Copy in the OID of ifPhysAddress, the address
	SnmpUtilOidCpy(&varBind[1].name, &MIB_ifMACEntAddr);

	for (int j = 0; j < varBind[0].value.asnValue.number; j++)
	{
		// Submit the query.  Responses will be loaded into varBindList.
		// We can expect this call to succeed a # of times corresponding to the # of adapters reported to be in the system
		if (f_SnmpExtensionQuery(ASN_RFC1157_GETNEXTREQUEST, &varBindList, &errorStatus, &errorIndex) == FALSE)
			continue;
		// Confirm that the proper type has been returned
		if (SnmpUtilOidNCmp(&varBind[0].name, &MIB_ifEntryType, MIB_ifEntryType.idLength) != 0)
			continue;
		// Type 6 describes ethernet interfaces
		if (varBind[0].value.asnValue.number != 6)
			continue;
		// Confirm that we have an address here
		if (SnmpUtilOidNCmp(&varBind[1].name, &MIB_ifMACEntAddr, MIB_ifMACEntAddr.idLength) != 0)
			continue;
		if (varBind[1].value.asnValue.address.stream == NULL)
			continue;
		// Ignore all dial-up networking adapters
		if ((varBind[1].value.asnValue.address.stream[0] == 0x44)
			&& (varBind[1].value.asnValue.address.stream[1] == 0x45)
			&& (varBind[1].value.asnValue.address.stream[2] == 0x53)
			&& (varBind[1].value.asnValue.address.stream[3] == 0x54)
			&& (varBind[1].value.asnValue.address.stream[4] == 0x00))
			continue;
		// Ignore NULL addresses returned by other network interfaces
		if ((varBind[1].value.asnValue.address.stream[0] == 0x00)
			&& (varBind[1].value.asnValue.address.stream[1] == 0x00)
			&& (varBind[1].value.asnValue.address.stream[2] == 0x00)
			&& (varBind[1].value.asnValue.address.stream[3] == 0x00)
			&& (varBind[1].value.asnValue.address.stream[4] == 0x00)
			&& (varBind[1].value.asnValue.address.stream[5] == 0x00))
			continue;
		char buf[32];
		result.sprintf("%02X-%02X-%02X-%02X-%02X-%02X",
			varBind[1].value.asnValue.address.stream[0],
			varBind[1].value.asnValue.address.stream[1],
			varBind[1].value.asnValue.address.stream[2],
			varBind[1].value.asnValue.address.stream[3],
			varBind[1].value.asnValue.address.stream[4],
			varBind[1].value.asnValue.address.stream[5]);
		break;
	}

	// Free the bindings
	SnmpUtilVarBindFree(&varBind[0]);
	SnmpUtilVarBindFree(&varBind[1]);
	m_macAddress = result;
	return result;
#elif defined Q_OS_LINUX
	int sockfd;
	struct ifreq ifr;
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd == -1)
	{
		perror("socket error");
		return QString("");
	}
	strncpy(ifr.ifr_name, ETH_NAME, IFNAMSIZ);

	if (ioctl(sockfd, SIOCGIFHWADDR, &fir) == 0)
	{
		QString result;
		result.sprintf("%02x%02x%02x%02x%02x%02x", ifr.ifr_hwaddr.sa_data[0], ifr.ifr_hwaddr.sa_data[1], ifr.ifr_hwaddr.sa_data[2]£¬ifr.ifr_hwaddr.sa_data[3]£¬ifr.ifr_hwaddr.sa_data[4]£¬ifr.ifr_hwaddr.sa_data[5]);
		result.append(ifr.ifr_hwaddr.sa_data);
		m_macAddress = result;
		return result;
	}
#endif /*Q_OS_WIN*/

}

QStringList Hardware::getDiskSerialId()
{
	if (!m_diskSerialID.isEmpty())
	{
		return m_diskSerialID;
	}

#ifdef Q_OS_WIN
	HRESULT hres;

	// Step 1: --------------------------------------------------
	// Initialize COM. ------------------------------------------

	bool bInitialize = false;
	hres = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (FAILED(hres))
	{
		bInitialize = true;
	}

	// Step 2: --------------------------------------------------
	// Set general COM security levels --------------------------
	// Note: If you are using Windows 2000, you need to specify -
	// the default authentication credentials for a user by using
	// a SOLE_AUTHENTICATION_LIST structure in the pAuthList ----
	// parameter of CoInitializeSecurity ------------------------
	if (!bInitialize)
	{
		hres = CoInitializeSecurity(
			NULL,
			-1,                          // COM authentication
			NULL,                        // Authentication services
			NULL,                        // Reserved
			RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication
			RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation  
			NULL,                        // Authentication info
			EOAC_NONE,                   // Additional capabilities
			NULL                         // Reserved
			);

		if (FAILED(hres))
		{
			CoUninitialize();
			return QStringList();                    // Program has failed.
		}
	}


	// Step 3: ---------------------------------------------------
	// Obtain the initial locator to WMI -------------------------

	IWbemLocator *pLoc = NULL;

	hres = CoCreateInstance(
		CLSID_WbemLocator,
		0,
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator, (LPVOID *)&pLoc);

	if (FAILED(hres))
	{
		CoUninitialize();
		return QStringList();
	}

	// Step 4: -----------------------------------------------------
	// Connect to WMI through the IWbemLocator::ConnectServer method

	IWbemServices *pSvc = NULL;

	// Connect to the root\cimv2 namespace with
	// the current user and obtain pointer pSvc
	// to make IWbemServices calls.
	hres = pLoc->ConnectServer(
		_bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
		NULL,                    // User name. NULL = current user
		NULL,                    // User password. NULL = current
		0,                       // Locale. NULL indicates current
		NULL,                    // Security flags.
		0,                       // Authority (e.g. Kerberos)
		0,                       // Context object
		&pSvc                    // pointer to IWbemServices proxy
		);

	if (FAILED(hres))
	{
		CoUninitialize();
		return QStringList();
	}

	// Step 5: --------------------------------------------------
	// Set security levels on the proxy -------------------------

	hres = CoSetProxyBlanket(
		pSvc,                        // Indicates the proxy to set
		RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
		RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
		NULL,                        // Server principal name
		RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx
		RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
		NULL,                        // client identity
		EOAC_NONE                    // proxy capabilities
		);

	if (FAILED(hres))
	{
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return QStringList();               // Program has failed.
	}

	// Step 6: --------------------------------------------------
	// Use the IWbemServices pointer to make requests of WMI ----

	// For example, get the name of the operating system
	IEnumWbemClassObject* pEnumerator = NULL;
	hres = pSvc->ExecQuery(
		bstr_t("WQL"),
		bstr_t("SELECT * FROM Win32_PhysicalMedia"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumerator);

	if (FAILED(hres))
	{
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return QStringList();               // Program has failed.
	}

	// Step 7: -------------------------------------------------
	// Get the data from the query in step 6 -------------------

	IWbemClassObject *pclsObj;
	ULONG uReturn = 0;

	QStringList result;
	while (pEnumerator)
	{
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1,
			&pclsObj, &uReturn);

		if (0 == uReturn)
		{
			break;
		}

		VARIANT vtProp;

		// Get the value of the Name property
		hr = pclsObj->Get(L"SerialNumber", 0, &vtProp, 0, 0);
		if (vtProp.bstrVal)
		{
			QString diskId = QString::fromStdWString(vtProp.bstrVal);
			QStringList disk = diskId.split(32, QString::SkipEmptyParts);

			result.push_back(disk[0]);
		}
		VariantClear(&vtProp);
	}

	// Cleanup
	// ========

	pSvc->Release();
	pLoc->Release();
	pEnumerator->Release();
	pclsObj->Release();
	if (!bInitialize)
	{
		CoUninitialize();
	}
	m_diskSerialID = result;
	return result;   // Program successfully completed.
#elif defined Q_OS_LINUX
	struct hd_driveid dsid;
	int fd = open("/dev/sda", O_RDONLY);
	if (fd < 0)
	{
		return QString("");
	}
	if (ioctl(fd, HDIO_GET_IDENTITY, &hid) < 0)
	{
		close(fd);
		return -1;
	}
	close(fd);
	QString result;
	result.sprintf("%s", hid.serial_no);
	m_diskSerialID = result;
	return result;
#endif
}

QString Hardware::getCentralProcessingUnitId()
{
	if (!m_cpuID.isEmpty())
	{
		return m_cpuID;
	}
#ifdef Q_OS_WIN
	QStringList results = XMG::Global::instance()->runCommand("wmic cpu get processorid").split(' ', QString::SkipEmptyParts);
	if (results.size() >= 2)
	{
		QStringList values = results[1].split("\r\r\n", QString::SkipEmptyParts);
		if (values.size() == 1)
		{
			m_cpuID = values[0];
			return values[0];
		}
	}
	return QString("");
#elif defined Q_OS_LINUX
	unsigned int eax, ebx, ecx, edx;
	native_cpuid(&eax, &ebx, &ecx, &edx);
	QString result;
	result.sprintf("0x%08x%08x", edx, ecx);
	qDebug() << "cpu serial number is:" + result;
#endif
}
