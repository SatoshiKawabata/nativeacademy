#ifndef b2test_GamePhysicsContactListener_h
#define b2test_GamePhysicsContactListener_h

#include <math.h>
#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "PhysicsSprite.h"
#include "Config.h"

USING_NS_CC;

class GamePhysicsContactListener : public b2ContactListener
{
public:
    // 衝突時の処理
    void BeginContact(b2Contact* contact)
    {
        // 衝突した双方の物体を取得
        b2Body* bodyA = contact->GetFixtureA()->GetBody();
        b2Body* bodyB = contact->GetFixtureB()->GetBody();

        // 物体にひもづくSpriteを取得
        PhysicsSprite* spriteA = (PhysicsSprite*)bodyA->GetUserData();
        PhysicsSprite* spriteB = (PhysicsSprite*)bodyB->GetUserData();

        // 地面との衝突は無視する
        if (spriteA->getTag() == Config::kTag_Ground ||
            spriteB->getTag() == Config::kTag_Ground)
        {
            return;
        }

        // 衝突時の加速度を取得
        b2Vec2 velocityA = bodyA->GetLinearVelocity();
        b2Vec2 velocityB = bodyB->GetLinearVelocity();
        CCLOG("[BeginContact] A(%f, %f) B(%f, %f)", velocityA.x, velocityA.y, velocityB.x, velocityB.y);

        // 加速度が一定上の大きさだったら、ぶつかられた方を削除する
        float threshold = 3;
        if (pow(velocityA.x, 2) + pow(velocityA.y, 2) > pow(threshold, 2)) {
            spriteB->setDeleteFlag(true);
        }
        if (pow(velocityB.x, 2) + pow(velocityB.y, 2) > pow(threshold, 2)) {
            spriteA->setDeleteFlag(true);
        }
    }
};

#endif
