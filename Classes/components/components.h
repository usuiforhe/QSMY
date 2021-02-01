//
//  components.h
//  自定义组件
//
//  Created by wanghejun on 13-3-11.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef QSMY_components_h
#define QSMY_components_h

#include "utils/UIUtil.h"

#include "DLayer.h"
#include "DTouchDelegate.h"
#include "DScrollLabel.h"
#include "DPopup.h"
#include "DTabView.h"
#include "DPageView.h"
#include "DPageDot.h"
#include "DTableView.h"
#include "DSlideView.h"
#include "DPageViewLoop.h"
#include "DSelectorView.h"
#include "DMaskedSprite.h"
#include "DProgressBar.h"
#include "DSprite.h"
#include "DTipsComponets.h"
#include "DMarqueeView.h"
#include "DEffectNode.h"
#include "DDraggingView.h"
#include "./gui/DButton.h"
#include "./gui/DMenuItemSound.h"
#include "./gui/UIAnimate.h"
#include "./gui/PLAdaptiveNode.h"
#include "./gui/PLAdaptiveScale9Sprite.h"
#include "./gui/DButtonSelect.h"
#include "./gui/DClippingNode.h"
#include "./gui/DSwitchButton.h"

#include "./video/VideoPlayer.h"
#include "./label/CCRichLabelTTF.h"
#include "./webview/CCWebView.h"
#include "DMessageBox.h"
#include "DStepTableView.h"
#include "DNumberBandit.h"

#include "PLWheelView.h"



#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "./input/DEditBox.h"
#endif

#include "ccbloaders/CCRichLabelTTFLoader.h"
#include "ccbloaders/DSpriteLoader.h"
#include "ccbloaders/DProgressBarLoader.h"
#include "ccbloaders/DCCBReader.h"
#include "ccbloaders/DCCBFileLoader.h"
#include "ccbloaders/DCCScrollViewLoader.h"



#define __CUSTOM_CCB_LOADER__(____ccNodeLoaderLibrary__) \
____ccNodeLoaderLibrary__->registerCCNodeLoader("DButton", DButtonLoader::loader());\
____ccNodeLoaderLibrary__->registerCCNodeLoader("DButtonSelect", DButtonSelectLoader::loader());\
____ccNodeLoaderLibrary__->registerCCNodeLoader("DMenuItemImageSound", DMenuItemImageSoundLoader::loader());\
____ccNodeLoaderLibrary__->registerCCNodeLoader("DMenuItemSound", DMenuItemImageSoundLoader::loader());\
____ccNodeLoaderLibrary__->registerCCNodeLoader("UIAnimate", UIAnimateLoader::loader());\
____ccNodeLoaderLibrary__->registerCCNodeLoader("DProgressBar", DProgressBarLoader::loader());\
____ccNodeLoaderLibrary__->registerCCNodeLoader("DSprite", DSpriteLoader::loader());\
____ccNodeLoaderLibrary__->unregisterCCNodeLoader("CCBFile");\
____ccNodeLoaderLibrary__->registerCCNodeLoader("CCBFile", DCCBFileLoader::loader());\
____ccNodeLoaderLibrary__->unregisterCCNodeLoader("CCScrollView");\
____ccNodeLoaderLibrary__->registerCCNodeLoader("CCScrollView", DCCScrollViewLoader::loader());\
____ccNodeLoaderLibrary__->registerCCNodeLoader("CCRichLabelTTF", CCRichLabelTTFLoader::loader());\
____ccNodeLoaderLibrary__->registerCCNodeLoader("PLAdaptiveNode", PLAdaptiveNodeLoader::loader());\
____ccNodeLoaderLibrary__->registerCCNodeLoader("PLAdaptiveScale9Sprite", PLAdaptiveScale9SpriteLoader::loader());



#ifndef CUSTOM_CCB_LOADER
#define CUSTOM_CCB_LOADER(____ccNodeLoaderLibrary__) \
__CUSTOM_CCB_LOADER__(____ccNodeLoaderLibrary__)
#endif

#define CCB_READER(__file__,__pNode__) \
{cocos2d::extension::CCNodeLoaderLibrary * ____ccNodeLoaderLibrary__ = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();\
CUSTOM_CCB_LOADER(____ccNodeLoaderLibrary__)    \
cocos2d::extension::CCBReader * ___ccbReader__ = new DCCBReader(____ccNodeLoaderLibrary__);\
__pNode__ = ___ccbReader__->readNodeGraphFromFile(__file__, this);\
___ccbReader__->release();}


#define CCB_READER_AND_ADDCHILD_BY_CUSTOM_LOADER(__file__,____ccNodeLoaderLibrary__) \
{cocos2d::CCNode *__pNode__ = NULL;   \
cocos2d::extension::CCBReader * ___ccbReader__ = new DCCBReader(____ccNodeLoaderLibrary__);\
__pNode__ = ___ccbReader__->readNodeGraphFromFile(__file__, this);\
___ccbReader__->release();\
if(__pNode__) addChild(__pNode__);}


#define CCB_READER_BY_CUSTOM_LOADER(__file__,____ccNodeLoaderLibrary__, __pNode__) \
{	cocos2d::extension::CCBReader * ___ccbReader__ = new DCCBReader(____ccNodeLoaderLibrary__);\
	__pNode__ = ___ccbReader__->readNodeGraphFromFile(__file__, this);\
	___ccbReader__->release();}


#define CCB_READER_AND_ADDCHILD(__file__) \
{cocos2d::CCNode *__pNode__ = NULL;   \
CCB_READER(__file__,__pNode__);           \
if(__pNode__) addChild(__pNode__);}


#define CCB_READER_DATA(__data__,___pNode__) \
{cocos2d::extension::CCNodeLoaderLibrary * ____ccNodeLoaderLibrary__ = cocos2d::extension::CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();\
CUSTOM_CCB_LOADER(____ccNodeLoaderLibrary__)    \
cocos2d::extension::CCBReader * ___ccbReader__ = new DCCBReader(____ccNodeLoaderLibrary__);\
___pNode__ = ___ccbReader__->readNodeGraphFromData(__data__, this, CCDirector::sharedDirector()->getWinSize());\
___ccbReader__->release();}

#define CCB_READER_DATA_BY_CUSTOM_LOADER(__data__,___pNode__,____ccNodeLoaderLibrary__) \
{cocos2d::CCNode *__pNode__ = NULL;\
	cocos2d::extension::CCBReader * ___ccbReader__ = new DCCBReader(____ccNodeLoaderLibrary__);\
	___pNode__ = ___ccbReader__->readNodeGraphFromData(__data__, this, CCDirector::sharedDirector()->getWinSize());\
	___ccbReader__->release();}

#define CCB_READER_DATA_AND_ADDCHILD(__data__) \
{cocos2d::CCNode *__pNode__ = NULL;   \
CCB_READER_DATA(__data__,__pNode__);           \
if(__pNode__) addChild(__pNode__);} \


#define CCB_READER_DATA_AND_ADDCHILD_BY_CUSTOM_LOADER(__data__,____ccNodeLoaderLibrary__) \
{cocos2d::CCNode *__pNode__ = NULL;   \
cocos2d::extension::CCBReader * ___ccbReader__ = new DCCBReader(____ccNodeLoaderLibrary__);\
__pNode__ = ___ccbReader__->readNodeGraphFromData(__data__, this, CCDirector::sharedDirector()->getWinSize());\
___ccbReader__->release();\
if(__pNode__) addChild(__pNode__);}



static cocos2d::extension::CCData* readCCData(const char*pCCBFileName)
{
    std::string strCCBFileName(pCCBFileName);
    std::string strSuffix(CCB_SUFFIX);
 
    // Add ccbi suffix
    if (!cocos2d::extension::CCBReader::endsWith(strCCBFileName.c_str(), strSuffix.c_str()))
    {
        strCCBFileName += strSuffix;
    }
    
    strCCBFileName +=SUFFIX;
    
    std::string strPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(strCCBFileName.c_str());
    unsigned long size = 0;
    
    unsigned char * pBytes = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(strPath.c_str(), "rb", &size);
    
    std::string string((char*)pBytes,size);
#ifdef CRYPT
    string = DecodeAES(UI_PASSWORD, string);
#endif
    cocos2d::extension::CCData *data = new cocos2d::extension::CCData((unsigned char *)string.c_str(), string.length());
    CC_SAFE_DELETE_ARRAY(pBytes);
    if(data) data->autorelease();
    return data;
}


static void replaceLabel(cocos2d::CCLabelTTF* o,cocos2d::CCRichLabelTTF* n)
{
    if(!n || !o)return;
    n->setFontName(o->getFontName());
    n->setFontSize(o->getFontSize());
    n->setOpacity(o->getOpacity());
    n->setFontFillColor(o->getColor());
    n->setDimensions(o->getDimensions());
    n->setHorizontalAlignment(o->getHorizontalAlignment());
    n->setVerticalAlignment(o->getVerticalAlignment());
    UIUtil::duplicate((CCNode *)n, (CCNode *)o);
    CCNode* parent = o->getParent();
    if(parent) parent->addChild(n);
    o->removeFromParent();
}


#endif
