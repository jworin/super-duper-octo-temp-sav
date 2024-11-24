#include "webserver.h"

WebServer::WebServer(){

  users = new http_conn[MAX_FD];

  char server_path[200];
  getcwd(server_path, 200);
  char root[6] = "/root";
  m_root = (char *)malloc(strlen(server_path) + strlen(root) + 1);
  strcpy(m_root, server_path);
  strcat(m_root, root);

  users_timer = new client_data[MAX_FD];

}

WebServer::~WebServer(){
  close(m_epollfd);
  close(m_listenfd);
  close(m_pipefd[1]);
  close(m_pipefd[0]);
  delete[] users;
  delete[] users_timer;
  delete m_pool;
}

void WebServer::init(int port, std::string user, std:string passWord, std::string databaseName, int log_write,
                     int opt_linger, int trigmode, int sql_num, int thread_num, int close_log, int actor_model){
    m_port = port;
    m_user = user;
    m_passWord = passWord;
    m_databaseName = databaseName;
    m_sql_num = sql_num;
    m_thread_num = thread_num;
    m_log_write = log_write;
    m_OPT_LINGER = opt_linger;
    m_TRIGMode = trigmode;
    m_close_log = close_log;
    m_actormodel = actor_model;
}

void WebServer::trig_mode(){

}



