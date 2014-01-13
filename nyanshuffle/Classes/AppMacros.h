//
//  AppMacro.h
//  nyanshuffle
//
//  Created by cocos2d-x on 2014/01/13.
//
//

#ifndef __APPMACROS_H__
#define __APPMACROS_H__

#include "cocos2d.h"

typedef struct tagResource
{
    cocos2d::CCSize size;
    char directory[100];
} Resource;

static Resource smallResource   = { cocos2d::CCSizeMake( 512,  384), "S"};
static Resource mediumResource  = { cocos2d::CCSizeMake(1024,  768), "M"};
static Resource largeResource   = { cocos2d::CCSizeMake(2048, 1536), "L"};

static cocos2d::CCSize designResolutionSize = cocos2d::CCSizeMake(1024, 768);

#define LEVEL_FONT_SIZE (cocos2d::CCGLView::sharedOpenGLView()->getDesignResolutionSize().width / mediumResource.size.width * 48)

#endif // __APPMACROS_H__
