#ifndef _CONNECTION_POOL_
#define _CONNECTION_POOL_

#include <list>
#include <mysql/mysql.h>
#include <string>
#include "../lock/locker.h"

class connection_pool
{
public:
    MYSQL *GetConnection();
    bool ReleaseConnection(MYSQL *conn);
    int GetFreeConn() const;
    void DestroyPool();

    static connection_pool *GetInstance();

    void init(std::string url, std::string User, std::string PassWord, std::string DataBaseName,
        int Port, int MaxConn, int close_log);

private:
    connection_pool();
    ~connection_pool();

    int m_MaxConn;
    int m_CurConn;
    int m_FreeConn;
    locker lock;
    std::list<MYSQL *> connList;
    sem reserve;

public:
    std::string m_url;
    std::string m_Port;
    std::string m_User;
    std::string m_PassWord;
    std::string m_DatabaseName;
    int m_close_log;
};

class connectionRAII
{
public:
    connectionRAII(MYSQL **con, connection_pool *connPool);
    ~connectionRAII();

private:
    MYSQL *conRAII;
    connection_pool *poolRAII;
};

#endif



