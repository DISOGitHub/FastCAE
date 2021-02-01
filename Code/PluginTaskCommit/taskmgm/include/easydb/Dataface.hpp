// This is a database abstraction layer
// Written by zhaodx (zhao.dexiang@foxmail.com)

#include "Database.hpp"

#if 0
#define ENABLE_DATABASE_SQLITE      1
#define ENABLE_DATABASE_MYSQL       1
#define ENABLE_DATABASE_POSTGRES    1
#ifdef _WIN32
#define ENABLE_DATABASE_MSSQL       1
#endif
#endif

#if defined(ENABLE_DATABASE_MYSQL)
#include "MySQLDatabase.hpp"
#endif

#if defined(ENABLE_DATABASE_POSTGRES)
#include "PostgresDatabase.hpp"
#endif

#if defined(ENABLE_DATABASE_SQLITE)
#include "SQLiteDatabase.hpp"
#endif

#if defined(ENABLE_DATABASE_MSSQL)
#include "MsAdoDatabase.hpp"
#endif

namespace easydb 
{

enum {
    DB_SQLITE,
    DB_MYSQL,
    DB_POSTGRES, 
    DB_MSSQL,
};

inline void CleanupLibs()
{
#if defined(ENABLE_DATABASE_MYSQL)
		mysql_library_end();
#endif
}


inline Database* CreateDatabaseInterface(uint32 uType)
{
	switch(uType)
	{
#if defined(ENABLE_DATABASE_MYSQL)
	case DB_MYSQL:		// MYSQL
		return new MySQLDatabase();
		break;
#endif
	
#if defined(ENABLE_DATABASE_POSTGRES)
	case DB_POSTGRES:		// POSTGRES
		return new PostgresDatabase();
		break;
#endif

#if defined(ENABLE_DATABASE_SQLITE)
	case DB_SQLITE:		// SQLITE
		return new SQLiteDatabase();
		break;
#endif

#if defined(ENABLE_DATABASE_MSSQL)
	case DB_MSSQL:		
		return new MsAdoDatabase();
#endif
	}

	perror("You have attempted to connect to a database that is unsupported or nonexistant.\nCheck your config and try again.");
	///abort();
	return NULL;
}

};
