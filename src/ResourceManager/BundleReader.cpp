#include "BundleReader.hpp"

#include <sqlite3.h>

std::map<std::string, std::string>
BundleReader::readBundle(const std::string& viewName, const std::string& resourceName, int& resourceId)
{
    std::map<std::string, std::string> data;
    try
    {
        sqlite3* db;
        int rc = sqlite3_open_v2(m_bundleFile.string().c_str(), &db, SQLITE_OPEN_READONLY, nullptr);
        if (rc == SQLITE_OK)
        {
            std::string query = "SELECT * FROM " + viewName + " WHERE Name = ?";
            sqlite3_stmt* stmt;
            rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
            if (rc == SQLITE_OK)
            {
                rc = sqlite3_bind_text(stmt, 1, resourceName.c_str(), resourceName.size(), nullptr);
                if (rc == SQLITE_OK)
                {
                    rc = sqlite3_step(stmt);
                    if (rc == SQLITE_ROW)
                    {
                        resourceId = sqlite3_column_int(stmt, 0);
                        for (int i = 2; i < sqlite3_column_count(stmt); ++i)
                        {
                            const char* key = sqlite3_column_name(stmt, i);
                            const void* value = sqlite3_column_blob(stmt, i);
                            int size = sqlite3_column_bytes(stmt, i);
                            data[key] = std::string(static_cast<const char*>(value), size);
                        }
                        rc = SQLITE_OK;
                    }
                    else if (rc == SQLITE_DONE)
                    {
                        sqlite3_finalize(stmt);
                        sqlite3_close(db);
                        throw std::runtime_error("Not found");
                    }
                }
                sqlite3_finalize(stmt);
            }
            sqlite3_close(db);
        }

        if (rc != SQLITE_OK)
        {
            throw std::runtime_error(sqlite3_errmsg(db));
        }
    }
    catch (std::exception& e)
    {
        const std::string message = "Resource Load Error [" + resourceName + "]: " + e.what();
        throw std::runtime_error(message);
    }
    return data;
}
