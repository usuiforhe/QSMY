//
//  TouchLayer.h
//  QSMY
//
//  Created by wanghejun on 13-8-23.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__TouchLayer__
#define __QSMY__TouchLayer__

#include "AppInclude.h"
#include "../../../components/components.h"

class TouchLayer
:public DLayerModal
{
public:
    CREATE_FUNC(TouchLayer);
    D_INIT_DEFAULT(DLayerModal);
    D_TOUCH_REGISTER_DEFAULT( DTOUCH_DISABLE_TOUCH_PRIORITY );
};

#endif /* defined(__QSMY__TouchLayer__) */
