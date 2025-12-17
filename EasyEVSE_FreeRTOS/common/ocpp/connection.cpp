
#include <everest/database/sqlite/connection.hpp>
#include "FreeRTOSConfig.h"

namespace everest::db::sqlite {

	Connection::Connection(const fs::path& database_file_path) noexcept :
			db(nullptr), database_file_path(database_file_path), open_count(0) {
	}


	Connection::~Connection() {
	     close_connection_internal(true);
	}


	bool Connection::close_connection_internal(bool force_close) {
	    if (db && (force_close || open_count == 0)) {
	        //sqlite3_close(db);
	        db = nullptr;
	        return true;
	    }
	    return false;
	}

    bool Connection::open_connection() {
    	return false;
    }

    bool Connection::close_connection() {
    	return close_connection_internal(false);;
    }

    std::unique_ptr<TransactionInterface> Connection::begin_transaction() {
    	return nullptr;
    }

    bool Connection::execute_statement(const std::string& statement) {
    	return false;
    }

    std::unique_ptr<StatementInterface> Connection::new_statement(const std::string& sql) {
    	return nullptr;
    }

    const char* Connection::get_error_message() {
    	return "NotImplemented";
    }

    bool Connection::clear_table(const std::string& table) {
    	return false;
    }

    int64_t Connection::get_last_inserted_rowid() {
    	return 0;
    }

    uint32_t Connection::get_user_version() {
    	return 0;
    }

    void Connection::set_user_version(uint32_t version) {
    	configPRINTF(("set_user_version not implememnted"));
    }
}
