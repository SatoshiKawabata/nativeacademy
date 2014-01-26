/*
 * PhysicsSprite.h
 *
 *  Created on: 2014/01/20
 *      Author: zzaba_000
 */

#ifndef PHYSICSSPRITE_H_
#define PHYSICSSPRITE_H_

#include <iostream>
#include "cocos2d.h"
#include "Box2D/Box2D.h"

USING_NS_CC;

class PhysicsSprite : public CCSprite {
private:
	bool deleteFlag;
    b2Body* m_pBody;

public:
	PhysicsSprite();
    virtual ~PhysicsSprite();
    
    void setPhysicsBody(b2Body* body);
    b2Body* getPhysicsBody();

    virtual bool isDirty(void);
    virtual CCAffineTransform nodeToParentTransform(void);

    // 物体にデリートフラグを追加する
    void setDeleteFlag(bool flag);
    bool getDeleteFlag();

};

#endif /* PHYSICSSPRITE_H_ */
