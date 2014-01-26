#ifndef __b2test__GameScene__
#define __b2test__GameScene__

#include <iostream>
#include <list>

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "Config.h"
#include "PhysicsSprite.h"

USING_NS_CC;

class GameScene : public cocos2d::CCLayer
{
private:
	// 物理エンジンワールド
    b2World* world;

    CCSpriteBatchNode* spriteBatchNode;
    list<PhysicsSprite*> pSpriteList;

    // スワイプ用
    CCPoint touchStartPoint;
    PhysicsSprite* targetSprite;	// 作業用参照なので解放不要

public:
    GameScene();
    virtual ~GameScene();

    virtual bool init();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(GameScene);

    void initPhysics();
    virtual void update(float delta);

    void createGround();

    void initSprite();
    void createSprite(CCPoint p);
    void deleteSprite();

    virtual bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent);
    virtual void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent);
};

#endif /* defined(__b2test__GameScene__) */
