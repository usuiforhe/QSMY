//
//  get_items_test.h
//  cache
//
//  Created by 汪燕军 on 13-9-4.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef __cache__get_items_test__
#define __cache__get_items_test__

#include "cache_prefix.h"
#include "protocol/item.pb.h"
#include "net/tcp_client.h"
#include "net/message.h"
#include "net/protobuf_codec.h"
#include "thread/thread_pool.h"
#include "util.h"

namespace cache_test {
    
    class message_sender
    {
    public:
        message_sender();
        
        ~message_sender();
        
        latte::header_protobuf_message_ptr send_and_wait_response(latte::header_protobuf_message_ptr message);
        
    private:
        boost::shared_ptr<latte::protobuf_codec> codec_;
        boost::shared_ptr<latte::tcp_client> client_;
    };
    
    class item_test
    {
    public:
        item_test();
        ~item_test();
        
        void start();
        
        void stop();
        
        void get_items(uint32_t user_id);
        
    private:
        latte::thread_pool thread_pool_;
        message_sender sender_;
    };
}

#endif /* defined(__cache__get_items_test__) */
