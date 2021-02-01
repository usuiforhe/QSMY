//
//  DPageDot.h
//  QSMY
//
//  Created by OnePiece on 13-6-7.
//  Copyright (c) 2013年 thedream. All rights reserved.
//

#ifndef __QSMY__DPageDot__
#define __QSMY__DPageDot__

#include "AppInclude.h"


class DPageDot : public cocos2d::CCNode
{
public:
    DPageDot();
    virtual ~DPageDot();
    
    static DPageDot * create(const char *selectString, const char *unselectString);
    CREATE_FUNC(DPageDot);
    
    virtual bool init(const char *selectString , const char *unselectString);
    virtual bool init();
    
private:
    CCNode *m_pDotContainer;
    
    std::string m_sSelectString;
    std::string m_sUnelectString;
    
    int         m_nMarginX;
    
    int         m_nPages;
    int         m_nCurrentPage;
    
    int         m_nPageSize;
    
public:
    void setPages(const int &pages);
    void setCurrentPage(const int &currentPage);
    void updateView(const bool &clear=false);
    void setOffsetX(const int &offset);
    void setPageSize(int pageSize);
    int getFixedMargin()const;
};

#endif /* defined(__QSMY__DPageDot__) */
