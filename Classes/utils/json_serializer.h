//
//  json_serializer.h
//  QSMY
//
//  Created by wanghejun on 13-4-2.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef QSMY_json_serializer_h
#define QSMY_json_serializer_h

#include "json/json.h"

class json_serializer{
public:
    friend json_serializer & operator <<(json_serializer & s, uint32_t value)
    {
        s.root[s.last_key_] = value;
        s.last_key_ = "";
        return s;
    }
    friend json_serializer & operator <<(json_serializer & s, const char* value)
    {
        if(s.last_key_ == ""){
            s.last_key_ = value;
        }
        else{
            s.root[s.last_key_] = std::string(value);
            s.last_key_ = "";
        }
        return s;
    }
    friend json_serializer & operator <<(json_serializer & s, Json::Value & value)
    {
        s.root[s.last_key_] = value;
        s.last_key_ = "";
        return s;
    }
    
    friend json_serializer & operator <<(json_serializer & s, json_serializer & value)
    {
        s.root[s.last_key_] = value.root;
        s.last_key_ = "";
        return s;
    }
    std::string toString()
    {
        Json::FastWriter writer;
        std::string str = writer.write(root);
        if(str.compare("null\n")==0) return "{}";
        return str;
    }
    const Json::Value &value(){return root;};
private:
    Json::Value root;
    std::string last_key_;
};

#endif
