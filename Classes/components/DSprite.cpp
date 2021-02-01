//
//  DSprite.cpp
//  QSMY
//
//  Created by wanghejun on 13-6-3.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#include "DSprite.h"

USING_NS_CC;

DSprite* DSprite::createWithTexture(CCTexture2D *pTexture)
{
    DSprite *pobSprite = new DSprite();
    if (pobSprite && pobSprite->initWithTexture(pTexture))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

DSprite* DSprite::createWithTexture(CCTexture2D *pTexture, const CCRect& rect)
{
    DSprite *pobSprite = new DSprite();
    if (pobSprite && pobSprite->initWithTexture(pTexture, rect))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

DSprite* DSprite::create(const char *pszFileName)
{
    DSprite *pobSprite = new DSprite();
    if (pobSprite && pobSprite->initWithFile(pszFileName))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

DSprite* DSprite::create(const char *pszFileName, const CCRect& rect)
{
    DSprite *pobSprite = new DSprite();
    if (pobSprite && pobSprite->initWithFile(pszFileName, rect))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

DSprite* DSprite::createWithSpriteFrame(CCSpriteFrame *pSpriteFrame)
{
    DSprite *pobSprite = new DSprite();
    if (pSpriteFrame && pobSprite && pobSprite->initWithSpriteFrame(pSpriteFrame))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

DSprite* DSprite::createWithSpriteFrameName(const char *pszSpriteFrameName)
{
    CCSpriteFrame *pFrame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pszSpriteFrameName);
    
#if COCOS2D_DEBUG > 0
    char msg[256] = {0};
    sprintf(msg, "Invalid spriteFrameName: %s", pszSpriteFrameName);
    CCAssert(pFrame != NULL, msg);
#endif
    
    return createWithSpriteFrame(pFrame);
}

DSprite* DSprite::create()
{
    DSprite *pSprite = new DSprite();
    if (pSprite && pSprite->init())
    {
        pSprite->autorelease();
        return pSprite;
    }
    CC_SAFE_DELETE(pSprite);
    return NULL;
}

void DSprite::updateColor(void)
{
    
    if(m_bUseGradient)
    {
        float opacityRatio = m_bOpacityModifyRGB?_displayedOpacity/255.0f:1.0f;
        
        
        m_sQuad.tl.colors.r = m_sDQuad.tl.colors.r * opacityRatio;
        m_sQuad.tl.colors.g = m_sDQuad.tl.colors.g * opacityRatio;
        m_sQuad.tl.colors.b = m_sDQuad.tl.colors.b * opacityRatio;
        m_sQuad.tr.colors.a = _displayedOpacity;
        
        m_sQuad.tr.colors.r = m_sDQuad.tr.colors.r * opacityRatio;
        m_sQuad.tr.colors.g = m_sDQuad.tr.colors.g * opacityRatio;
        m_sQuad.tr.colors.b = m_sDQuad.tr.colors.b * opacityRatio;
        m_sQuad.tr.colors.a = _displayedOpacity;
        
        m_sQuad.bl.colors.r = m_sDQuad.bl.colors.r * opacityRatio;
        m_sQuad.bl.colors.g = m_sDQuad.bl.colors.g * opacityRatio;
        m_sQuad.bl.colors.b = m_sDQuad.bl.colors.b * opacityRatio;
        m_sQuad.bl.colors.a = _displayedOpacity;
        
        m_sQuad.br.colors.r = m_sDQuad.br.colors.r * opacityRatio;
        m_sQuad.br.colors.g = m_sDQuad.br.colors.g * opacityRatio;
        m_sQuad.br.colors.b = m_sDQuad.br.colors.b * opacityRatio;
        m_sQuad.br.colors.a = _displayedOpacity;
    }else
    {
        
        ccColor4B color4 = { _displayedColor.r, _displayedColor.g, _displayedColor.b, _displayedOpacity };
        
        // special opacity for premultiplied textures
        if (m_bOpacityModifyRGB)
        {
            color4.r *= _displayedOpacity/255.0f;
            color4.g *= _displayedOpacity/255.0f;
            color4.b *= _displayedOpacity/255.0f;
        }
        
        m_sQuad.bl.colors = color4;
        m_sQuad.br.colors = color4;
        m_sQuad.tl.colors = color4;
        m_sQuad.tr.colors = color4;
    }
    
    
    
    
    // renders using batch node
    if (m_pobBatchNode)
    {
        if (m_uAtlasIndex != CCSpriteIndexNotInitialized)
        {
            m_pobTextureAtlas->updateQuad(&m_sQuad, m_uAtlasIndex);
        }
        else
        {
            // no need to set it recursively
            // update dirty_, don't update recursiveDirty_
            setDirty(true);
        }
    }
    
    // self render
    // do nothing
}

void DSprite::setColor(const ccColor3B &start, const ccColor3B &end)
{
    m_sDQuad.tl.colors.r = start.r;
    m_sDQuad.tl.colors.g = start.g;
    m_sDQuad.tl.colors.b = start.b;
    
    m_sDQuad.tr.colors.r = start.r;
    m_sDQuad.tr.colors.g = start.g;
    m_sDQuad.tr.colors.b = start.b;
    
    m_sDQuad.bl.colors.r = end.r;
    m_sDQuad.bl.colors.g = end.g;
    m_sDQuad.bl.colors.b = end.b;
    
    m_sDQuad.br.colors.r = end.r;
    m_sDQuad.br.colors.g = end.g;
    m_sDQuad.br.colors.b = end.b;
    m_bUseGradient = true;
    updateColor();
}



void DSprite::setColor(const ccColor3B &tl, const ccColor3B &tr,const ccColor3B &bl, const ccColor3B &br)
{
    m_sDQuad.tl.colors.r = tl.r;
    m_sDQuad.tl.colors.g = tl.g;
    m_sDQuad.tl.colors.b = tl.b;
    
    m_sDQuad.tr.colors.r = tr.r;
    m_sDQuad.tr.colors.g = tr.g;
    m_sDQuad.tr.colors.b = tr.b;
    
    m_sDQuad.bl.colors.r = bl.r;
    m_sDQuad.bl.colors.g = bl.g;
    m_sDQuad.bl.colors.b = bl.b;
    
    m_sDQuad.br.colors.r = br.r;
    m_sDQuad.br.colors.g = br.g;
    m_sDQuad.br.colors.b = br.b;
    m_bUseGradient = true;
    updateColor();
}


void DSprite::setOpacity(GLubyte opacity)
{
    CCNodeRGBA::setOpacity(opacity);
    
    updateColor();
}

void DSprite::setColor(const ccColor3B& color3)
{
    CCNodeRGBA::setColor(color3);
    m_bUseGradient = false;
    updateColor();
}

void DSprite::setOpacityModifyRGB(bool modify)
{
    if (m_bOpacityModifyRGB != modify)
    {
        m_bOpacityModifyRGB = modify;
        updateColor();
    }
}

bool DSprite::isOpacityModifyRGB(void)
{
    return m_bOpacityModifyRGB;
}

void DSprite::updateDisplayedColor(const ccColor3B& parentColor)
{
    CCNodeRGBA::updateDisplayedColor(parentColor);
    m_bUseGradient = false;
    updateColor();
}

void DSprite::updateDisplayedOpacity(GLubyte opacity)
{
    CCNodeRGBA::updateDisplayedOpacity(opacity);
    
    updateColor();
}

void DSprite::draw(void)
{
    CC_PROFILER_START_CATEGORY(kCCProfilerCategorySprite, "CCSprite - draw");
    
    CCAssert(!m_pobBatchNode, "If CCSprite is being rendered by CCSpriteBatchNode, CCSprite#draw SHOULD NOT be called");
    
    CC_NODE_DRAW_SETUP();
    
    ccGLBlendFunc( m_sBlendFunc.src, m_sBlendFunc.dst );
    
    if (m_pobTexture != NULL)
    {
        ccGLBindTexture2D( m_pobTexture->getName() );
        if(m_bTextureRepeat)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        }
    }
    else
    {
        ccGLBindTexture2D(0);
    }
    
    //
    // Attributes
    //
    
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_PosColorTex );
    
#define kQuadSize sizeof(m_sQuad.bl)
    long offset = (long)&m_sQuad;
    
    // vertex
    int diff = offsetof( ccV3F_C4B_T2F, vertices);
    glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, kQuadSize, (void*) (offset + diff));
    
    // texCoods
    diff = offsetof( ccV3F_C4B_T2F, texCoords);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));
    
    // color
    diff = offsetof( ccV3F_C4B_T2F, colors);
    glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (void*)(offset + diff));
    
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    if (m_pobTexture != NULL)
    {
        if(m_bTextureRepeat)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        }
    }
    
    CHECK_GL_ERROR_DEBUG();
    
    
    
#if CC_SPRITE_DEBUG_DRAW == 1
    // draw bounding box
    CCPoint vertices[4]={
        ccp(m_sQuad.tl.vertices.x,m_sQuad.tl.vertices.y),
        ccp(m_sQuad.bl.vertices.x,m_sQuad.bl.vertices.y),
        ccp(m_sQuad.br.vertices.x,m_sQuad.br.vertices.y),
        ccp(m_sQuad.tr.vertices.x,m_sQuad.tr.vertices.y),
    };
    ccDrawPoly(vertices, 4, true);
#elif CC_SPRITE_DEBUG_DRAW == 2
    // draw texture box
    CCSize s = this->getTextureRect().size;
    CCPoint offsetPix = this->getOffsetPosition();
    CCPoint vertices[4] = {
        ccp(offsetPix.x,offsetPix.y), ccp(offsetPix.x+s.width,offsetPix.y),
        ccp(offsetPix.x+s.width,offsetPix.y+s.height), ccp(offsetPix.x,offsetPix.y+s.height)
    };
    ccDrawPoly(vertices, 4, true);
#endif // CC_SPRITE_DEBUG_DRAW
    
    CC_INCREMENT_GL_DRAWS(1);
    
    CC_PROFILER_STOP_CATEGORY(kCCProfilerCategorySprite, "CCSprite - draw");
}


