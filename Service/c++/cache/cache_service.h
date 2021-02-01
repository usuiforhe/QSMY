//
//  cache_service.h
//  cache
//
//  Created by 汪燕军 on 13-5-7.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __cache__cache_service__
#define __cache__cache_service__

#include "cache_prefix.h"
#include "config.h"
#include "net/protobuf_dispatcher.h"
#include "net/protobuf_codec.h"
#include "net/tcp_server.h"
#include "thread/thread_pool.h"

namespace cache{
    class cache_service{
    public:
        cache_service();
        ~cache_service();
        
        void start();
        
        void stop();
        
        void send(latte::tcp_conn_ptr connection, latte::header_protobuf_message_ptr message);
        
        void handle_message(latte::tcp_conn_ptr connection, latte::header_protobuf_message_ptr message);
        
        void wrap_handle(latte::tcp_conn_ptr connection, latte::header_protobuf_message_ptr message);
        
        inline GETTER(boost::shared_ptr<latte::thread_pool>, thread_pool);
        
        inline GETTER(boost::shared_ptr<latte::mysql_pool>, connection_pool);

        inline GETTER(bool, stopped);
        
        void before_thread_exit();
        
        boost::shared_ptr<sql::Connection> get_connection();
        
    private:
        latte::tcp_server * tcp_server_;
        latte::protobuf_codec * codec_;
        latte::protobuf_dispatcher<latte::tcp_conn_ptr> * dispatcher_;
        
        boost::shared_ptr<latte::thread_pool> thread_pool_;
        boost::shared_ptr<latte::mysql_pool> connection_pool_;
        bool stopped_;
        
        void init();
    };
    
#define SCOPED_CONNECT(scoped, conn)    latte::scoped_pooled_connection<sql::Connection> scoped(SINGLETON(cache_service).connection_pool(), SINGLETON(config).db_info);\
    boost::shared_ptr<sql::Connection> conn(scoped.get());
}

#endif /* defined(__cache__cache_service__) */
