//
//  Animation.cpp
//  nyanshuffle
//
//  Created by cocos2d-x on 2014/01/13.
//
//

#include "Animation.h"

CCFiniteTimeAction* Animation::catStartAnimation()
{
    CCSkewTo* skew1 = CCSkewTo::create(0.2f, 1.f, 0.f);
    CCSkewTo* skew2 = CCSkewTo::create(0.2f, -1.f, 0.f);
    CCSequence* seq = CCSequence::createWithTwoActions(skew1, skew2);
    CCRepeat* repeat = CCRepeat::create(seq, 10);
    
    return repeat;
}

CCFiniteTimeAction* Animation::vibrationAction()
{
    CCMoveBy* move1 = CCMoveBy::create(0.03f, ccp(-5.f, 0.f));
    CCMoveBy* move2 = CCMoveBy::create(0.06f, ccp(10.f, 0.f));
    CCMoveBy* move3 = CCMoveBy::create(0.03f, ccp(-5.f, 0.f));
    CCSequence* moves = CCSequence::create(move1, move2, move3, NULL);
    
    return CCRepeat::create(moves, 2);
}

CCFiniteTimeAction* Animation::boxiesStartAction(CCObject* target, SEL_CallFunc selector)
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    int random = rand();
    
    float delayTime = 1.f;
    
    switch (random % 5)
    {
        case 0:
            delayTime = delayTime * 0.9f;
            break;
        case 1:
            delayTime = delayTime * 0.95f;
        case 2:
            delayTime = delayTime * 1.05f;
        case 3:
            delayTime = delayTime * 1.1f;
        default:
            break;
    }
    CCDelayTime* delay1 = CCDelayTime::create(delayTime);
    CCMoveBy* move1 = CCMoveBy::create(1.f, ccp(0.f, -size.height));
    CCEaseIn* ease1 = CCEaseIn::create(move1, 10.f);
    
    
    CCDelayTime* delay2 = CCDelayTime::create(0.9f);
    CCCallFunc* func = CCCallFunc::create(target, selector);
    CCFiniteTimeAction* seq = CCSequence::createWithTwoActions(delay2, func);
    
    CCSpawn* spawn = CCSpawn::createWithTwoActions(ease1, seq);
    
    return CCSequence::createWithTwoActions(delay1, spawn);
}

CCFiniteTimeAction* Animation::boxiesStartActionWithVibration(CCObject* target, SEL_CallFunc selector)
{
    CCFiniteTimeAction* action1 = boxiesStartAction(target, selector);
    CCFiniteTimeAction* action2 = Animation::vibrationAction();
    return CCSequence::createWithTwoActions(action1, action2);
}

CCFiniteTimeAction* Animation::num3Animation()
{
    CCDelayTime* delay1 = CCDelayTime::create(0.5f);
    CCScaleTo* scaleUp = CCScaleTo::create(0.1f, 1.f);
    CCDelayTime* delay2 = CCDelayTime::create(0.3f);
    CCScaleTo* scaleDown = CCScaleTo::create(0.1f, 0.f);
    
    return CCSequence::create(delay1, scaleUp, delay2, scaleDown, NULL);
}

CCFiniteTimeAction* Animation::num2Animation()
{
    CCDelayTime* delay = CCDelayTime::create(1.f);
    
    return CCSequence::create(delay, Animation::num3Animation(), NULL);
}

CCFiniteTimeAction* Animation::num1Animation()
{
    CCDelayTime* delay = CCDelayTime::create(2.f);
    
    return CCSequence::create(delay, Animation::num3Animation(), NULL);
}

CCFiniteTimeAction* Animation::num0Animation(cocos2d::CCObject *target, SEL_CallFunc selector)
{
    CCDelayTime* delay = CCDelayTime::create(3.f);
    CCCallFunc* func = CCCallFunc::create(target, selector);
    
    return CCSequence::create(delay, Animation::num3Animation(), func, NULL);
}

















