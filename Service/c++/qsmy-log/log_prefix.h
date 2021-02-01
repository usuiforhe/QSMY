//
//  log_prefix.h
//  qsmy-log
//
//  Created by 汪燕军 on 13-5-21.
//  Copyright (c) 2013年 汪燕军. All rights reserved.
//

#ifndef qsmy_log_log_prefix_h
#define qsmy_log_log_prefix_h

#include "latte_prefix.h"
#include <json/json.h>
#include <boost/foreach.hpp>
#include <boost/chrono.hpp>
#include "log/log.h"
#include "util.h"

#define GETTER(type, name)  type name(){ return name##_; }
#define CONST_GETTER(type, name)  type name() const { return name##_; }
#define SETTER(type, name)  void name(type name){ name##_ = name; }

#define SINGLETON(T) latte::singleton<T>::instance()


#endif
