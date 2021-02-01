//
//  SettlementView.cpp
//  QSMY
//
//  Created by wanghejun on 13-3-27.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "SettlementView.h"
#include "SettlementViewPve.h"
#include "SettlementViewMatch.h"
#include "SettlementViewPvp.h"

USING_NS_CC;
using namespace settlement;


SettlementView* SettlementView::create(SettlementDelegate *delegate,SettlementInfo *pInfo)
{
    if(!pInfo) return NULL;
    SettlementView * pView = NULL;
    switch (pInfo->m_type) {
        case kSettlementPve:
            pView = new SettlementViewPve();
            break;
        case kSettlementMatch:
            pView = new SettlementViewMatch();
            break;
		case kSettlementPvp:
			pView = new SettlementViewPvp();
			break;
        default:
            break;
    }
    if( pView && pView->init(delegate, pInfo))
    {
        pView->autorelease();
        return pView;
    }
    CC_SAFE_DELETE(pView);
    return NULL;
}