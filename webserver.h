#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <sys/epoll.h>
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <cstring>

#include "CGImysql/sql_connection_pool.h"
#include "log/log.h"

const int MAX_FD = 1020;
const int MAX_EVENT_NUMBER = 10000;
const int TIMESLOT = 5;


class WebServer
{
public:
  WebServer();
  ~WebServer();

  void init(int port, std::string user, std::string passWord, std::string databaseName, int log_write,
            int opt_linger, int trigmode, int sql_num, int thread_num, int close_log, int actor_model);

  void thread_pool();
  void sql_pool();
  void log_write();
  void trig_mode();
  void eventListen();
  void eventLoop();
  void timer(int connfd, struct sockaddr_in client_address);
  void adjust_timer(util_timer *timer);
  void deal_timer(util_timer *timer);
  bool dealclientdata();
  bool dealwithsignal(bool& timeout, bool& stop_server);
  void dealwithread(int sockfd);
  void dealwithwrite(int sockfd);


public:
  int m_port;
  char *m_root;
  int m_log_write;
  int m_close_log;
  int m_actormodel;

  int m_pipefd[2];   // fd 文件描述符
  int m_epollfd;

  http_conn *users;

  connection_pool *m_connPool;
  std::string m_user;
  std::string m_passWord;
  std::string m_databaseName;
  int m_sql_num;

  threadpool<http_conn> *m_pool;
  int m_thread_num;

  epoll_event events[MAX_EVENT_NUMBER];

  int m_listenfd;
  int m_OPT_LINGER;
  int m_TRIGMode;
  int m_LISTENTrigmode;
  int m_CONNTrigmode;

  client_data *users_timer;
  Utils utils;

};


#endif




