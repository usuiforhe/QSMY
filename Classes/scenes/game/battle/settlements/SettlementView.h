//
//  SettlementView.h
//  pve结算界面
//
//  Created by wanghejun on 13-3-27.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__SettlementView__
#define __QSMY__SettlementView__

#include "AppInclude.h"
#include "components/components.h"
#include "../../../../model/SettlementModel.h"
#include "../Battle.h"

class SettlementView :public DLayer
{
public:
    SettlementView(){};
    virtual ~SettlementView(){};
    static SettlementView* create(SettlementDelegate *delegate,settlement::SettlementInfo *pInfo);
    virtual bool init(SettlementDelegate *delegate,settlement::SettlementInfo *pInfo) = 0;

	virtual void closeView() = 0;		//for android back key
};


#endif /* defined(__QSMY__SettlementView__) */
