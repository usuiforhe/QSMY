//
//  CCRichLabelTTFLoader.cpp
//  QSMY
//
//  Created by Eci on 14-4-25.
//  Copyright (c) 2014年 thedream. All rights reserved.
//

#include "CCRichLabelTTFLoader.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define PROPERTY_SHADOW "enableShadow"
#define PROPERTY_SHADOW_OFFSET	"shadowOffset"
#define PROPERTY_SHADOW_COLOR	"shadowColor"
#define PROPERTY_SHADOW_BLUR	"shadowBlur"

#define PROPERTY_STROKE	"enableStroke"
#define PROPERTY_STROKE_TEXT_COLOR	"strokeUsingTextColor"
#define PROPERTY_STROKE_COLOR	"strokeColor"
#define PROPERTY_STROKE_SIZE	"strokeSize"


#define PROPERTY_COLOR "color"
#define PROPERTY_OPACITY "opacity"
#define PROPERTY_BLENDFUNC "blendFunc"
#define PROPERTY_FONTNAME "fontName"
#define PROPERTY_FONTSIZE "fontSize"
#define PROPERTY_HORIZONTALALIGNMENT "horizontalAlignment"
#define PROPERTY_VERTICALALIGNMENT "verticalAlignment"
#define PROPERTY_STRING "string"
#define PROPERTY_DIMENSIONS "dimensions"

CCRichLabelTTFLoader::CCRichLabelTTFLoader()
	: m_bEnableShadow(false)
	, m_szShadowOffset()
	, m_iShadowColor(0)
	, m_fShadowBlur(.0f)
	, m_bEnableStroke(false)
	, m_bStrokeUsingTextColor(false)
	, m_crStrokeColor()
	, m_fStrokeSize(0)
{

}


void CCRichLabelTTFLoader::parseProperties(CCNode * pNode, CCNode * pParent, CCBReader * pCCBReader)
{
	CCNodeLoader::parseProperties(pNode, pParent, pCCBReader);
	CCRichLabelTTF *ttf = static_cast<CCRichLabelTTF *>(pNode);
	ttf->setFontFillColor(ttf->getColor());
	if (m_bEnableShadow)
	{
		ttf->enableShadow(m_szShadowOffset, m_iShadowColor, m_fShadowBlur);
	}
	if (m_bEnableStroke)
	{
		if (m_bStrokeUsingTextColor)
		{
			ttf->enableStroke(ttf->getColor(), m_fStrokeSize);
		}
		else
		{
			ttf->enableStroke(m_crStrokeColor, m_fStrokeSize);
		}
	}
	
}

void CCRichLabelTTFLoader::onHandlePropTypeCheck(CCNode * pNode, CCNode * pParent, const char* pPropertyName, bool pCheck, CCBReader * pCCBReader)
{
	if (strcmp(pPropertyName, PROPERTY_SHADOW) == 0)
	{
		m_bEnableShadow = pCheck;
	}
	else if (strcmp(pPropertyName, PROPERTY_STROKE) == 0)
	{
		m_bEnableStroke = pCheck;
	}
	else if (strcmp(pPropertyName, PROPERTY_STROKE_TEXT_COLOR) == 0)
	{
		m_bStrokeUsingTextColor = pCheck;
	}
	else
	{
		cocos2d::extension::CCNodeLoader::onHandlePropTypeCheck(pNode, pParent, pPropertyName,pCheck,pCCBReader);
	}
}

void CCRichLabelTTFLoader::onHandlePropTypeSize(CCNode * pNode, CCNode * pParent, const char * pPropertyName, CCSize pSize, CCBReader * pCCBReader)
{
	if (strcmp(pPropertyName, PROPERTY_SHADOW_OFFSET) == 0)
	{
		m_szShadowOffset = pSize;
	}
	else if(strcmp(pPropertyName, PROPERTY_DIMENSIONS) == 0) {
		((CCRichLabelTTF *)pNode)->setDimensions(pSize);
	} 
	else
	{
		cocos2d::extension::CCNodeLoader::onHandlePropTypeSize(pNode, pParent, pPropertyName, pSize, pCCBReader);
	}
}

void CCRichLabelTTFLoader::onHandlePropTypeInteger(CCNode * pNode, CCNode * pParent, const char* pPropertyName, int pInteger, CCBReader * pCCBReader)
{

	if (strcmp(pPropertyName, PROPERTY_SHADOW_COLOR) == 0)
	{
		m_iShadowColor = pInteger;
	}
	else
	{
		cocos2d::extension::CCNodeLoader::onHandlePropTypeInteger(pNode, pParent, pPropertyName, pInteger, pCCBReader);
	}
}

void CCRichLabelTTFLoader::onHandlePropTypeFloat(CCNode * pNode, CCNode * pParent, const char* pPropertyName, float pFloat, CCBReader * pCCBReader)
{

	if (strcmp(pPropertyName, PROPERTY_SHADOW_BLUR) == 0)
	{
		m_fShadowBlur = pFloat;
	}
	else if (strcmp(pPropertyName, PROPERTY_STROKE_SIZE) == 0)
	{
		m_fStrokeSize = pFloat;
	}
	else
	{
		cocos2d::extension::CCNodeLoader::onHandlePropTypeFloat(pNode, pParent, pPropertyName, pFloat, pCCBReader);
	}
}

void CCRichLabelTTFLoader::onHandlePropTypeColor3(CCNode * pNode, CCNode * pParent, const char * pPropertyName, ccColor3B pCCColor3B, CCBReader * pCCBReader)
{
	if (strcmp(pPropertyName, PROPERTY_STROKE_COLOR) == 0)
	{
		m_crStrokeColor = pCCColor3B;
	}
	else if(strcmp(pPropertyName, PROPERTY_COLOR) == 0) {
		((CCRichLabelTTF *)pNode)->setColor(pCCColor3B);
	}
	else
	{
		cocos2d::extension::CCNodeLoader::onHandlePropTypeColor3(pNode, pParent, pPropertyName, pCCColor3B, pCCBReader);
	}
}

void CCRichLabelTTFLoader::onHandlePropTypeByte(CCNode * pNode, CCNode * pParent, const char * pPropertyName, unsigned char pByte, CCBReader * pCCBReader) {
	if(strcmp(pPropertyName, PROPERTY_OPACITY) == 0) {
		((CCRichLabelTTF *)pNode)->setOpacity(pByte);
	} else {
		CCNodeLoader::onHandlePropTypeByte(pNode, pParent, pPropertyName, pByte, pCCBReader);
	}
}

void CCRichLabelTTFLoader::onHandlePropTypeBlendFunc(CCNode * pNode, CCNode * pParent, const char * pPropertyName, ccBlendFunc pCCBlendFunc, CCBReader * pCCBReader) {
	if(strcmp(pPropertyName, PROPERTY_BLENDFUNC) == 0) {
		((CCRichLabelTTF *)pNode)->setBlendFunc(pCCBlendFunc);
	} else {
		CCNodeLoader::onHandlePropTypeBlendFunc(pNode, pParent, pPropertyName, pCCBlendFunc, pCCBReader);
	}
}

void CCRichLabelTTFLoader::onHandlePropTypeFontTTF(CCNode * pNode, CCNode * pParent, const char * pPropertyName, const char * pFontTTF, CCBReader * pCCBReader) {
	if(strcmp(pPropertyName, PROPERTY_FONTNAME) == 0) {
		((CCRichLabelTTF *)pNode)->setFontName(pFontTTF);
	} else {
		CCNodeLoader::onHandlePropTypeFontTTF(pNode, pParent, pPropertyName, pFontTTF, pCCBReader);
	}
}

void CCRichLabelTTFLoader::onHandlePropTypeText(CCNode * pNode, CCNode * pParent, const char * pPropertyName, const char * pText, CCBReader * pCCBReader) {
	if(strcmp(pPropertyName, PROPERTY_STRING) == 0) {
		((CCRichLabelTTF *)pNode)->setString(pText);
	} else {
		CCNodeLoader::onHandlePropTypeText(pNode, pParent, pPropertyName, pText, pCCBReader);
	}
}

void CCRichLabelTTFLoader::onHandlePropTypeFloatScale(CCNode * pNode, CCNode * pParent, const char * pPropertyName, float pFloatScale, CCBReader * pCCBReader) {
	if(strcmp(pPropertyName, PROPERTY_FONTSIZE) == 0) {
		((CCRichLabelTTF *)pNode)->setFontSize(pFloatScale);
	} else {
		CCNodeLoader::onHandlePropTypeFloatScale(pNode, pParent, pPropertyName, pFloatScale, pCCBReader);
	}
}

void CCRichLabelTTFLoader::onHandlePropTypeIntegerLabeled(CCNode * pNode, CCNode * pParent, const char * pPropertyName, int pIntegerLabeled, CCBReader * pCCBReader) {
	if(strcmp(pPropertyName, PROPERTY_HORIZONTALALIGNMENT) == 0) {
		((CCRichLabelTTF *)pNode)->setHorizontalAlignment(CCTextAlignment(pIntegerLabeled));
	} else if(strcmp(pPropertyName, PROPERTY_VERTICALALIGNMENT) == 0) {
		((CCRichLabelTTF *)pNode)->setVerticalAlignment(CCVerticalTextAlignment(pIntegerLabeled));
	} else {
		CCNodeLoader::onHandlePropTypeIntegerLabeled(pNode, pParent, pPropertyName, pIntegerLabeled, pCCBReader);
	}
}