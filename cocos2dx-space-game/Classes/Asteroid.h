//
//  Asteroid.h
//  cocos2dx-space-game
//
//  Created by cocos2d-x on 2014/01/26.
//
//

#ifndef __cocos2dx_space_game__Asteroid__
#define __cocos2dx_space_game__Asteroid__

#include "cocos2d.h"
USING_NS_CC;

class Asteroid : public cocos2d::CCSprite
{
private:
    // レーザーオブジェクト
    CCArray* _enemyLasers;
    // 操作する対象のレーザー
    int _nextEnemyLaser;
public:
    Asteroid();
    virtual ~Asteroid();
};

#endif /* defined(__cocos2dx_space_game__Asteroid__) */
