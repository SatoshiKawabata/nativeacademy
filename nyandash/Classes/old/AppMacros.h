#ifndef __APPMACROS_H__
#define __APPMACROS_H__

#include "cocos2d.h"

typedef struct tagResource
{
    cocos2d::CCSize size;
    char directory[100];
} Resource;

static Resource smallResource = {cocos2d::CCSizeMake(512, 384), "resources-small"};
static Resource largeResource = {cocos2d::CCSizeMake(1024, 768), "resources-large"};
static Resource xlargeResource = {cocos2d::CCSizeMake(2048, 1536), "resources-xlarge"};

static cocos2d::CCSize designResolutionSize = cocos2d::CCSizeMake(1024, 577);

#endif /* __APPMACROS_H__ */