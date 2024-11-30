#include "sql_connection_pool.h"

#include "../log/log.h"

connection_pool::connection_pool() {
    m_CurConn = 0;
    m_FreeConn = 0;
}

connection_pool* connection_pool::GetInstance() {
    static connection_pool connPool;
    return &connPool;
}

void connection_pool::init(std::string url, std::string User, std::string PassWord, std::string DataBaseName, int Port, int MaxConn, int close_log) {
    m_url = url;
    m_Port = Port;
    m_User = User;
    m_PassWord = PassWord;
    m_DatabaseName = DataBaseName;
    m_close_log = close_log;

    for (int i = 0; i < MaxConn; i++) {
        MYSQL *con = NULL;
        con = mysql_init(con);
        if (con == NULL) {
            LOG_ERROR("MYSQL Error");
            exit(1);
        }
        con = mysql_real_connect(con, url.c_str(), User.c_str(), PassWord.c_str(), DataBaseName.c_str(), Port, NULL, 0);
        if (con == NULL) {
            LOG_ERROR("MYSQL Error");
            exit(1);
        }
        connList.push_back(con);
        ++m_FreeConn;
    }
    reserve = sem(m_FreeConn);
    m_MaxConn = m_FreeConn;
}

MYSQL *connection_pool::GetConnection() {
    MYSQL *con = NULL;

    if (0 == connList.size())
        return NULL;

    reserve.wait();

    lock.lock();

    con = connList.front();
    connList.pop_front();

    --m_FreeConn;
    ++m_CurConn;

    lock.unlock();
    return con;
}

bool connection_pool::ReleaseConnection(MYSQL* conn) {
    if (NULL == conn)
        return false;

    lock.lock();

    connList.push_back(conn); // 因为这个是一个连接池子，里面都是固定数量的，所以这边释放了一个之后就变成新的可用了
    ++m_FreeConn;
    --m_CurConn;

    lock.unlock();

    reserve.post();
    return true;
}

void connection_pool::DestroyPool() {
    lock.lock();
    if (connList.size() > 0) {
        std::list<MYSQL *>::iterator it;
        for (it = connList.begin(); it != connList.end(); ++it) {
            MYSQL *con = *it;
            mysql_close(con);
        }
        m_CurConn = 0;
        m_FreeConn = 0;
        connList.clear();
    }

    lock.unlock();
}

int connection_pool::GetFreeConn() const {
    return this->m_FreeConn;
}

connection_pool::~connection_pool() {
    DestroyPool();
}

connectionRAII::connectionRAII(MYSQL** con, connection_pool *connPool) {
    *con = connPool->GetConnection();
    conRAII = *con;
    poolRAII = connPool;
}

connectionRAII::~connectionRAII() {
    poolRAII->ReleaseConnection(conRAII);
}











