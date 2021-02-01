//
//  CCRichLabelTTFLoader.h
//  QSMY
//
//  Created by Eci on 14-4-25.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#ifndef __QSMY__CCRichLabelTTFLoader__
#define __QSMY__CCRichLabelTTFLoader__

#include "AppInclude.h"
#include "../label/CCRichLabelTTF.h"

USING_NS_CC;
USING_NS_CC_EXT;

class CCRichLabelTTFLoader : public cocos2d::extension::CCNodeLoader
{
public:
	CCRichLabelTTFLoader();
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CCRichLabelTTFLoader, loader);
    
protected:

	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CCRichLabelTTF);
	virtual void parseProperties(CCNode * pNode, CCNode * pParent, CCBReader * pCCBReader);
	virtual void onHandlePropTypeCheck(cocos2d::CCNode * pNode, cocos2d::CCNode * pParent, const char* pPropertyName, bool pCheck, cocos2d::extension::CCBReader * pCCBReader);
	virtual void onHandlePropTypeSize(CCNode * pNode, CCNode * pParent, const char * pPropertyName, CCSize pSize, CCBReader * pCCBReader);
	virtual void onHandlePropTypeInteger(CCNode * pNode, CCNode * pParent, const char* pPropertyName, int pInteger, CCBReader * pCCBReader);
	virtual void onHandlePropTypeFloat(CCNode * pNode, CCNode * pParent, const char* pPropertyName, float pFloat, CCBReader * pCCBReader);
	virtual void onHandlePropTypeColor3(CCNode * pNode, CCNode * pParent, const char * pPropertyName, ccColor3B pCCColor3B, CCBReader * pCCBReader);



	virtual void onHandlePropTypeByte(CCNode * pNode, CCNode * pParent, const char * pPropertyName, unsigned char pByte, CCBReader * pCCBReader);
	virtual void onHandlePropTypeBlendFunc(CCNode * pNode, CCNode * pParent, const char * pPropertyName, ccBlendFunc pCCBlendFunc, CCBReader * pCCBReader);
	virtual void onHandlePropTypeFontTTF(CCNode * pNode, CCNode * pParent, const char * pPropertyName, const char * pFontTTF, CCBReader * pCCBReader);
	virtual void onHandlePropTypeText(CCNode * pNode, CCNode * pParent, const char * pPropertyName, const char * pText, CCBReader * pCCBReader);
	virtual void onHandlePropTypeFloatScale(CCNode * pNode, CCNode * pParent, const char * pPropertyName, float pFloatScale, CCBReader * pCCBReader);
	virtual void onHandlePropTypeIntegerLabeled(CCNode * pNode, CCNode * pParent, const char * pPropertyName, int pIntegerLabeled, CCBReader * pCCBReader);
private:
	bool m_bEnableShadow;
	CCSize m_szShadowOffset;
	int m_iShadowColor;
	float m_fShadowBlur;
	bool m_bEnableStroke;
	bool m_bStrokeUsingTextColor;
	cocos2d::ccColor3B m_crStrokeColor;
	float m_fStrokeSize;
};


#endif /* defined(__QSMY__CCRichLabelTTFLoader__) */
