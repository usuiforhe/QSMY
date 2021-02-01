//
//  DProgressBarLoader.cpp
//  QSMY
//
//  Created by wanghejun on 13-7-1.
//  Copyright (c) 2013年 thedream. All rights reserved.
//
#include "DProgressBarLoader.h"



#define PROPERTY_BGDISPLAYFRAME "BgSpriteFrame"
#define PROPERTY_BARDISPLAYFRAME "BarSpriteFrame"
#define PROPERTY_MASKDISPLAYFRAME "MaskSpriteFrame"

USING_NS_CC;
USING_NS_CC_EXT;


void DProgressBarLoader::onHandlePropTypeSpriteFrame(CCNode * pNode, CCNode * pParent, const char * pPropertyName, CCSpriteFrame * pCCSpriteFrame, CCBReader * pCCBReader) {
    if(strcmp(pPropertyName, PROPERTY_BGDISPLAYFRAME) == 0) {
        if(pCCSpriteFrame != NULL) {
            ((DProgressBar *)pNode)->setBgSpriteFrame(pCCSpriteFrame);
        }
    } else if(strcmp(pPropertyName, PROPERTY_BARDISPLAYFRAME) == 0) {
        if(pCCSpriteFrame != NULL) {
            ((DProgressBar *)pNode)->setBarSpriteFrame(pCCSpriteFrame);
        }
    } else if(strcmp(pPropertyName, PROPERTY_MASKDISPLAYFRAME) == 0) {
        if(pCCSpriteFrame != NULL) {
            ((DProgressBar *)pNode)->setMaskSpriteFrame(pCCSpriteFrame);
        }
    } else {
        CCNodeLoader::onHandlePropTypeSpriteFrame(pNode, pParent, pPropertyName, pCCSpriteFrame, pCCBReader);
    }
}

