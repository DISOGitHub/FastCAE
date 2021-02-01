// This is a generic database access interface
// Written by zhaodx (zhao.dexiang@foxmail.com)

#ifndef __DATABASE_HPP__
#define __DATABASE_HPP__

#include <stdio.h>
#include <stdarg.h>
#include <string>

#ifdef _WIN32
#  define WIN32_LEAN_AND_MEAN
///#  define _WIN32_WINNT 0x0500
#  define NOMINMAX
#  include <windows.h>

#include <winsock2.h>

#else 
#include <stdint.h>

#endif

/* Use correct types for x64 platforms, too */
#ifdef _WIN32
typedef signed __int64 int64;
typedef signed __int32 int32;
typedef signed __int16 int16;
typedef signed __int8 int8;

typedef unsigned __int64 uint64;
typedef unsigned __int32 uint32;
typedef unsigned __int16 uint16;
typedef unsigned __int8 uint8;
#else

typedef int64_t int64;
typedef int32_t int32;
typedef int16_t int16;
typedef int8_t int8;
typedef uint64_t uint64;
typedef uint32_t uint32;
typedef uint16_t uint16;
typedef uint8_t uint8;
typedef uint32_t DWORD;

#endif

#ifdef _WIN32

#define I64FMT "%016I64X"
#define I64FMTD "%I64u"
#define SI64FMTD "%I64d"
#define snprintf _snprintf
#define atoll __atoi64

#else

#define stricmp strcasecmp
#define strnicmp strncasecmp
#define I64FMT "%016llX"
#define I64FMTD "%llu"
#define SI64FMTD "%lld"

#endif

#include "Field.hpp"
#include "FastMutex.hpp"

using namespace std;
class QueryResult;
///class Database;

struct DatabaseConnection
{
	FastMutex Busy;
};

class Database 
{
public:
	Database()
    {
        _counter=0;
        Connections = NULL;
        mConnectionCount = -1;   // Not connected.
        ///ThreadRunning = true;
    }
	virtual ~Database() {}

	/************************************************************************/
	/* Virtual Functions                                                    */
	/************************************************************************/
	virtual bool Initialize(const char* Hostname, unsigned int port,
		const char* Username, const char* Password, const char* DatabaseName,
		uint32 ConnectionCount=5, uint32 BufferSize=16384) = 0;
	
	virtual void Shutdown() {}

	virtual QueryResult* Query(const char* QueryString, ...)
    {
        char sql[16384];
        va_list vlist;
        va_start(vlist, QueryString);
        vsnprintf(sql, 16384, QueryString, vlist);
        va_end(vlist);

        // Send the query
        QueryResult * qResult = NULL;
        DatabaseConnection * con = GetFreeConnection();

        if(_SendQuery(con, sql, false)) {
            qResult = _StoreQueryResult( con );
        }
        
        con->Busy.Release();
        return qResult;
    }

	virtual QueryResult* QueryNA(const char* QueryString)
    {
        // Send the query
        QueryResult * qResult = NULL;
        DatabaseConnection * con = GetFreeConnection();

        if( _SendQuery( con, QueryString, false ) )
            qResult = _StoreQueryResult( con );

        con->Busy.Release();
        return qResult;
    }

	virtual QueryResult * FQuery(const char * QueryString, DatabaseConnection * con)
    {
        // Send the query
        QueryResult * qResult = NULL;
        if( _SendQuery( con, QueryString, false ) )
            qResult = _StoreQueryResult( con );

        return qResult;
    }

	virtual void FWaitExecute(const char * QueryString, DatabaseConnection * con)
    {
        // Send the query
        _SendQuery( con, QueryString, true );
    }

    //Wait For Request Completion
    virtual bool WaitExecute(const char* QueryString, ...)
    {
        char sql[16384];
        va_list vlist;
        va_start(vlist, QueryString);
        vsnprintf(sql, 16384, QueryString, vlist);
        va_end(vlist);

        DatabaseConnection * con = GetFreeConnection();
        bool Result = _SendQuery(con, sql, false, false);
        con->Busy.Release();
        return Result;
    }

    //Wait For Request Completion
    virtual bool WaitExecuteNA(const char* QueryString)
    {
        DatabaseConnection * con = GetFreeConnection();
        bool Result = _SendQuery(con, QueryString, false, false);
        con->Busy.Release();
        return Result;
    }


	inline const string& GetHostName() { return mHostname; }
	inline const string& GetDatabaseName() { return mDatabaseName; }

	virtual string EscapeString(string Escape) {return string(Escape);}
	virtual void EscapeLongString(const char * str, uint32 len, stringstream& out) {}
	virtual string EscapeString(const char * esc, DatabaseConnection * con) {return string(esc);};
	
	void FreeQueryResult(QueryResult * p) { delete p; }

	DatabaseConnection * GetFreeConnection()
    {
        uint32 i = 0;
        for(;;)
        {
            DatabaseConnection * con = Connections[ ((i++) % mConnectionCount) ];
            if(con->Busy.AttemptAcquire())
                return con;
        }

        // shouldn't be reached
        return NULL;
    }
#if 0
    static Database * CreateDatabaseInterface(uint32 uType)
    {
        switch(uType)
        {
    #if defined(ENABLE_DATABASE_MYSQL)
        case 1:		// MYSQL
            return new MySQLDatabase();
            break;
    #endif
        
    #if defined(ENABLE_DATABASE_POSTGRES)
        case 2:		// POSTGRES
            return new PostgresDatabase();
            break;
    #endif

    #if defined(ENABLE_DATABASE_SQLITE)
        case 3:		// SQLITE
            return new SQLiteDatabase();
            break;
    #endif
        }

        perror("You have attempted to connect to a database that is unsupported or nonexistant.\nCheck your config and try again.");
        abort();
        return NULL;
    }

    static void CleanupLibs()
    {
#if defined(ENABLE_DATABASE_MYSQL)
        mysql_library_end();
#endif
    }
#endif
	virtual bool SupportsReplaceInto() {return true;}
	virtual bool SupportsTableLocking() {return true;}

    ////////////////////////////////
    ///bool ThreadRunning;

protected:

	// spawn threads and shizzle
	void _Initialize() {}

	virtual void _BeginTransaction(DatabaseConnection * conn) = 0;
	virtual void _EndTransaction(DatabaseConnection * conn) = 0;

	// actual query function
	virtual bool _SendQuery(DatabaseConnection *con, const char* Sql, bool Self, bool bSave = true) = 0;
	virtual QueryResult * _StoreQueryResult(DatabaseConnection * con) = 0;

    ////////////////////////////////
	DatabaseConnection ** Connections;
	uint32 _counter;

    ///////////////////////////////
	int32 mConnectionCount;

	// For reconnecting a broken connection
	string mHostname;
	string mUsername;
	string mPassword;
	string mDatabaseName;
	uint32 mPort;
};

class QueryResult
{
public:
	QueryResult(uint32 fields, uint32 rows) : mFieldCount(fields), mRowCount(rows), mCurrentRow(NULL) {}
	virtual ~QueryResult() {}

	virtual bool NextRow() = 0;
	void Delete() { delete this; }

	inline Field* Fetch() { return mCurrentRow; }
	inline uint32 GetFieldCount() const { return mFieldCount; }
	inline uint32 GetRowCount() const { return mRowCount; }

protected:
	uint32 mFieldCount;
	uint32 mRowCount;
	Field *mCurrentRow;
};

#endif
