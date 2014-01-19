#ifndef __GAME_LAYER_LOADER__
#define __GAME_LAYER_LOADER__

#include "GameLayer.h"

class GameLayerLoader : public cocos2d::extension::CCLayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(GameLayerLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(GameLayer);
};

#endif /* __GAME_LAYER_LOADER__ */