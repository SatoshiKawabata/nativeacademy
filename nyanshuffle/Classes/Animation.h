//
//  Animation.h
//  nyanshuffle
//
//  Created by cocos2d-x on 2014/01/13.
//
//

#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "cocos2d.h"

USING_NS_CC;

class Animation
{
public:
    static CCFiniteTimeAction* catStartAnimation();
    
    static CCFiniteTimeAction* vibrationAction();
    static CCFiniteTimeAction* boxiesStartAction(CCObject* target, SEL_CallFunc selector);
    static CCFiniteTimeAction* boxiesStartActionWithVibration(CCObject* target, SEL_CallFunc selector);

    static CCFiniteTimeAction* num3Animation();
    static CCFiniteTimeAction* num2Animation();
    static CCFiniteTimeAction* num1Animation();
    static CCFiniteTimeAction* num0Animation(CCObject* target, SEL_CallFunc selector);
};

#endif /* defined(__nyanshuffle__Animation__) */
