#ifndef __SQLITEDATABASE_HPP__
#define __SQLITEDATABASE_HPP__

#if defined(ENABLE_DATABASE_SQLITE)

#ifdef _WIN32
#include <sqlite/sqlite3.h>
#else
#include "sqlite3.h"
#endif

struct SQLiteDatabaseConnection : public DatabaseConnection
{
	sqlite3 * handle;

	char ** tabledata;
	int rows;
	int cols;
};

class SQLiteQueryResult : public QueryResult
{
public:
	SQLiteQueryResult(char **tableData, uint32 rowCount, uint32 fieldCount)
        : QueryResult(fieldCount, rowCount)
        , mTableData(tableData), mTableIndex(0)
    {
        mCurrentRow = new Field[mFieldCount];
    }

    ~SQLiteQueryResult()
    {
        if (mCurrentRow)
        {
            delete [] mCurrentRow;
            mCurrentRow = NULL;
        }
        if (mTableData)
        {
            sqlite3_free_table(mTableData);
            mTableData = NULL;
        }
    }

	bool NextRow()
    {
        int startIndex;
        uint32 i;

        if (!mTableData)
            return false;

        if (mTableIndex >= mRowCount)
        {
            return false;
        }

        startIndex = (mTableIndex + 1) * mFieldCount;
        for (i = 0; i < mFieldCount; i++)
        {
            mCurrentRow[i].SetValue(mTableData[startIndex + i]);
        }

        ++mTableIndex;
        return true;
    }

protected:
        char **mTableData;
        uint32 mTableIndex;
};

class SQLiteDatabase : public Database
{
public:
	SQLiteDatabase() {}
	~SQLiteDatabase()
    {
        for(int32 i = 0; i < mConnectionCount; ++i) {
            sqlite3_close( static_cast<SQLiteDatabaseConnection*>(Connections[i])->handle );
            delete Connections[i];
        }
        delete [] Connections;
    }

	bool Initialize(const char* Hostname, unsigned int port,
		const char* Username, const char* Password, const char* DatabaseName,
		uint32 ConnectionCount, uint32 BufferSize)
    {
        int result;
        SQLiteDatabaseConnection * con;
        mConnectionCount = 1;
        Connections = (DatabaseConnection**)(new SQLiteDatabaseConnection*[mConnectionCount]);
        Connections[0] = new SQLiteDatabaseConnection;
        con = static_cast<SQLiteDatabaseConnection*>(Connections[0]);
        
        result = sqlite3_open(DatabaseName, &con->handle );
        if( con->handle == NULL || result != SQLITE_OK )
        {
            printf("SQLiteDatabase: Could not open %s.\n", Hostname);
            return false;
        }

        return true;
    }

	///void Shutdown();

	string EscapeString(string Escape)
    {
        char a2[16384] = {0};
        sqlite3_snprintf( 16384, a2, Escape.c_str() );
        return string(a2);
    }

	void EscapeLongString(const char * str, uint32 len, stringstream& out)
    {
        char a2[65536*3] = {0};
        sqlite3_snprintf( 65536*3, a2, str );
    }

	string EscapeString(const char * esc, DatabaseConnection * con)
    {
        char a2[16384] = {0};
        sqlite3_snprintf( 16384, a2, esc );
        return string(a2);
    }
	
protected:

	bool _SendQuery(DatabaseConnection *con, const char* Sql, bool Self = false, bool doseSave = true)
    {
        SQLiteDatabaseConnection * db = static_cast<SQLiteDatabaseConnection*>( con );

        char * err = NULL;
        if (doseSave) {
            int result = sqlite3_get_table( db->handle, Sql, &db->tabledata, &db->rows, &db->cols, &err );

            if( result != SQLITE_OK ) {
                printf("SQLite: Query failed with error message: %s.\n", err ? err : "NULL");
                return false;
            }
        } else {
            if(sqlite3_exec(db->handle, Sql, NULL, NULL, &err) != SQLITE_OK)
                return false;
        }

        return true;
    }

	void _BeginTransaction(DatabaseConnection * conn)
    {
        _SendQuery( conn, "BEGIN TRANSACTION", false );
    }

	void _EndTransaction(DatabaseConnection * conn)
    {
        _SendQuery( conn, "COMMIT TRANSACTION", false );
    }

	QueryResult * _StoreQueryResult(DatabaseConnection * con)
    {
        SQLiteDatabaseConnection * db = static_cast<SQLiteDatabaseConnection*>( con );

        if( db->rows == 0 || db->cols == 0 || db->tabledata == 0 )
        {
            if( db->tabledata != NULL ) {
                sqlite3_free_table(db->tabledata);
                db->tabledata = NULL;
            }
            return NULL;
        }

        SQLiteQueryResult *queryResult = new SQLiteQueryResult(db->tabledata, db->rows, db->cols);
        if(!queryResult) {
            return 0;
        }

        queryResult->NextRow();
        return queryResult;
    }
};

#endif

#endif      // __SQLITEDATABASE_HPP__
