#include "ReadData_ExodusII.h"
#include <vtkExodusIIReader.h>


ReadData_ExodusII::ReadData_ExodusII()
{
  
}

ReadData_ExodusII::~ReadData_ExodusII()
{
    if (reader != NULL)
    {
        reader->Delete();
        reader = NULL;
    }
}

bool ReadData_ExodusII::Read(QString tep_filename)
{
    reader = vtkExodusIIReader::New();
    char * fileNamechar;
    QByteArray ba = QStringToChar(tep_filename);
    fileNamechar = ba.data();
    reader->SetFileName(fileNamechar);
    reader->Update();

    blockDataSet = reader->GetOutput();

    const int n = blockDataSet->GetNumberOfBlocks();

    return n > 0;
}

