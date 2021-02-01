//
//  DSprite.h
//  QSMY
//
//  Created by wanghejun on 13-6-3.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__DSprite__
#define __QSMY__DSprite__

#include "AppInclude.h"


class DSprite:public cocos2d::CCSprite
{
    CC_SYNTHESIZE(bool, m_bTextureRepeat, TextureRepeat);
public:
    
    
    /**
     * Creates an empty sprite without texture. You can call setTexture method subsequently.
     *
     * @return An empty sprite object that is marked as autoreleased.
     */
    static DSprite* create();
    
    /**
     * Creates a sprite with an image filename.
     *
     * After creation, the rect of sprite will be the size of the image,
     * and the offset will be (0,0).
     *
     * @param   pszFileName The string which indicates a path to image file, e.g., "scene1/monster.png".
     * @return  A valid sprite object that is marked as autoreleased.
     */
    static DSprite* create(const char *pszFileName);
    
    /**
     * Creates a sprite with an image filename and a rect.
     *
     * @param   pszFileName The string wich indicates a path to image file, e.g., "scene1/monster.png"
     * @param   rect        Only the contents inside rect of pszFileName's texture will be applied for this sprite.
     * @return  A valid sprite object that is marked as autoreleased.
     */
    static DSprite* create(const char *pszFileName, const cocos2d::CCRect& rect);
    
    /**
     * Creates a sprite with an exsiting texture contained in a CCTexture2D object
     * After creation, the rect will be the size of the texture, and the offset will be (0,0).
     *
     * @param   pTexture    A pointer to a CCTexture2D object.
     * @return  A valid sprite object that is marked as autoreleased.
     */
    static DSprite* createWithTexture(cocos2d::CCTexture2D *pTexture);
    
    /**
     * Creates a sprite with a texture and a rect.
     *
     * After creation, the offset will be (0,0).
     *
     * @param   pTexture    A pointer to an existing CCTexture2D object.
     *                      You can use a CCTexture2D object for many sprites.
     * @param   rect        Only the contents inside the rect of this texture will be applied for this sprite.
     * @return  A valid sprite object that is marked as autoreleased.
     */
    static DSprite* createWithTexture(cocos2d::CCTexture2D *pTexture, const cocos2d::CCRect& rect);
    
    /**
     * Creates a sprite with an sprite frame.
     *
     * @param   pSpriteFrame    A sprite frame which involves a texture and a rect
     * @return  A valid sprite object that is marked as autoreleased.
     */
    static DSprite* createWithSpriteFrame(cocos2d::CCSpriteFrame *pSpriteFrame);
    
    /**
     * Creates a sprite with an sprite frame name.
     *
     * A CCSpriteFrame will be fetched from the CCSpriteFrameCache by pszSpriteFrameName param.
     * If the CCSpriteFrame doesn't exist it will raise an exception.
     *
     * @param   pszSpriteFrameName A null terminated string which indicates the sprite frame name.
     * @return  A valid sprite object that is marked as autoreleased.
     */
    static DSprite* createWithSpriteFrameName(const char *pszSpriteFrameName);
        
    /**
     *	@brief	设置上下顶点的颜色
     *
     *	@param 	top 	<#start description#>
     *	@param 	bottom 	<#end description#>
     */
    virtual void setColor(const cocos2d::ccColor3B &top, const cocos2d::ccColor3B &bottom);

    
    /**
     *	@brief	设置4个顶点的颜色
     *
     *	@param 	tl 	<#tl description#>
     *	@param 	tr 	<#tr description#>
     *	@param 	bl 	<#bl description#>
     *	@param 	br 	<#br description#>
     */
    virtual void setColor(const cocos2d::ccColor3B &tl, const cocos2d::ccColor3B &tr,const cocos2d::ccColor3B &bl, const cocos2d::ccColor3B &br);
    
    virtual ~DSprite(){};
    DSprite()
    :m_bUseGradient(false)
    ,m_bTextureRepeat(false){};
    
    
    /// @name Functions inherited from CCNodeRGBA
    virtual void setColor(const cocos2d::ccColor3B& color3);
    virtual void updateDisplayedColor(const cocos2d::ccColor3B& parentColor);
    virtual void setOpacity(GLubyte opacity);
    virtual void setOpacityModifyRGB(bool modify);
    virtual bool isOpacityModifyRGB(void);
    virtual void updateDisplayedOpacity(GLubyte parentOpacity);
    /// @}
    
    virtual void draw(void);
    
protected:
    virtual void updateColor(void);
protected:
    cocos2d::ccV3F_C4B_T2F_Quad m_sDQuad;
    bool                        m_bUseGradient;
    
};

#endif /* defined(__QSMY__DSprite__) */
