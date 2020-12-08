#ifndef READDATA_EXODUSII_H_
#define READDATA_EXODUSII_H_

#include "ReadData_Base.h"
#include <QString>

class vtkExodusIIReader;

class ReadData_ExodusII : public ReadData_Base
{
public:
    ReadData_ExodusII();
    ~ReadData_ExodusII();

    bool Read(QString tep_filename) override;

private:
    vtkExodusIIReader* reader{};

};


#endif
