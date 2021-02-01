//
//  BusyLayer.h
//  QSMY
//
//  Created by wanghejun on 13-8-23.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__BusyLayer__
#define __QSMY__BusyLayer__

#include "AppInclude.h"
#include "components/components.h"
#include "DragonBone.h"

class BusyLayer
:public DLayerModal
{
public:
    BusyLayer();
    virtual ~BusyLayer();
    CREATE_FUNC(BusyLayer);
    virtual bool init();
    D_TOUCH_REGISTER_DEFAULT( DTOUCH_DISABLE_TOUCH_PRIORITY );
    
    void showBusy();
    void hideBusy();
    void updateBusyLayer();
	bool isShowing() {return busyCount > 0;}
private:
    uint16_t    busyCount;
    CCNode*     m_pLoading;
};

#endif /* defined(__QSMY__BusyLayer__) */
