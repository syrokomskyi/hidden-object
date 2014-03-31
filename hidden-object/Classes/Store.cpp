#include "stdafx.h"
#include "Store.h"


Store::Store() :
    db( nullptr )
{

    // подключаемся к д-хранилищу
    const std::string path = PATH_STORE + "/store.db";

#if 0
    auto r = sqlite3_open_v2(
        path.c_str(),
        &db,
        SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE,
        nullptr
    );
#else
    auto r = sqlite3_open( path.c_str(), &db );
#endif

    if (r != SQLITE_OK) {
        LOG << "Opening store wrong, " << r << std::endl;
    }
    DASSERT( r == SQLITE_OK );

    // создаём таблицы, если их ещё нет

    // PLAYER_TABLE_STORE
    char* err;
    const std::string s =
        "create table if not exists " + PLAYER_TABLE_STORE + "("
        // # UID послужит время создания записи, мс. См. Player::currentTime().
        " uid INT primary key,"
        " login varchar( 200 ),"
        " energy INT,"
        " lastTimeAddEnergy INT,"
        " room varchar( 200 ),"
        " exploreTime INT,"
        " findItems INT"
        ")";
    r = sqlite3_exec( db, s.c_str(), nullptr, nullptr, &err );
    if (r != SQLITE_OK) {
        LOG << "Create table fail, " << r << ", " << err << std::endl;
    }
    DASSERT( r == SQLITE_OK );
}




Store::~Store() {
    sqlite3_close( db );
}




void
Store::save( const std::string& table,  const std::string& data ) {

    DASSERT( db
        && "Нет подключения к долговременному хранилищу." );

    char* err;
    const std::string s =
        "insert into " + table + " values( " + data + " )";
    const auto r = sqlite3_exec( db, s.c_str(), nullptr, nullptr, &err );
    if (r != SQLITE_OK) {
        LOG << "Insert into table fail, " << r << ", " << err << std::endl;
    }
    DASSERT( r == SQLITE_OK );

#ifdef _DEBUG
    LOG << "Status saved in table '" << table << "'.\nData: " <<
        data << std::endl;
#endif
}
