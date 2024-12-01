cmake_minimum_required(VERSION 3.30)
project(webserv)

set(CMAKE_CXX_STANDARD 11)

add_executable(webserv main.cpp
        webserver.cpp
        config.cpp
        lock/locker.h
        log/log.h
        log/log.cpp
        log/block_queue.h
        CGImysql/sql_connection_pool.h
        CGImysql/sql_connection_pool.cpp
        threadpool/threadpool.h
        http/http_conn.h
        http/http_conn.cpp
        timer/lst_timer.h
        timer/lst_timer.cpp)
