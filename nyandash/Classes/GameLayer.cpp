//
//  GameLayer.cpp
//  nyandash
//
//  Created by zabaglione on 2014/01/19.
//
//

#include "GameLayer.h"

GameLayer::GameLayer()
: mBg1(NULL)
, mBg2(NULL)
, mBg3(NULL)
, mAnimationManager(NULL)
{
    mCount = 0;
    mBeforeFoot = FOOT_UNKNOWN;
}

GameLayer::~GameLayer()
{
    CC_SAFE_RELEASE(mBg1);
    CC_SAFE_RELEASE(mBg2);
    CC_SAFE_RELEASE(mBg3);
    CC_SAFE_RELEASE(mAnimationManager);
}

SEL_MenuHandler GameLayer::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "tappedLeftFoot", GameLayer::tappedLeftFoot);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "tappedRightFoot", GameLayer::tappedRightFoot);
    
    return NULL;
}

void GameLayer::tappedLeftFoot(CCObject* pSender, CCControlEvent pCCControlEvent)
{
    CCLOG("left button tapped :: mCount = %d", mCount);
    tappedFoot(FOOT_LEFTFOOT);
}

void GameLayer::tappedRightFoot(CCObject* pSender, CCControlEvent pCCControlEvent)
{
    CCLOG("right button tapped :: mCount = %d", mCount);
    tappedFoot(FOOT_RIGHTFOOT);
}

SEL_CCControlHandler GameLayer::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName)
{
    return NULL;
}

bool GameLayer::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBg1", CCSprite*, this->mBg1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBg2", CCSprite*, this->mBg2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBg3", CCSprite*, this->mBg3);
    
    return false;
}

void GameLayer::tappedFoot(EFoot currentFoot)
{
    if (mBeforeFoot == currentFoot)
    {
        mAnimationManager->runAnimationsForSequenceNamedTweenDuration("Tumble", 0);
        mBeforeFoot = FOOT_UNKNOWN;
    }
    else
    {
        mCount++;
        if (mCount >= GOAL_COUNT)
        {
            mAnimationManager->runAnimationsForSequenceNamedTweenDuration("Goal", 0);
            return;
        }
        
        mBeforeFoot = currentFoot;
        switch (currentFoot) {
            case FOOT_LEFTFOOT:
                mAnimationManager->runAnimationsForSequenceNamedTweenDuration("Leftfoot", 0);
                break;
            case FOOT_RIGHTFOOT:
                mAnimationManager->runAnimationsForSequenceNamedTweenDuration("Rightfoot", 0);
                break;
                
            default:
                break;
        }
        const float moveTime = 1.0 / 3;
        float moveDistanceForBG3 = (this->mBg3->getContentSize().width - CCDirector::sharedDirector()->getWinSize().width) / GOAL_COUNT;
        
        CCMoveBy* move1 = CCMoveBy::create(moveTime, ccp(-moveDistanceForBG3 / 4, 0));
        this->mBg1->runAction(move1);
        
        CCMoveBy* move2 = CCMoveBy::create(moveTime, ccp(-moveDistanceForBG3 / 2, 0));
        this->mBg2->runAction(move2);
        
        CCMoveBy* move3 = CCMoveBy::create(moveTime, ccp(-moveDistanceForBG3, 0));
        this->mBg3->runAction(move3);
        
        float moveDistanceForCat = CCDirector::sharedDirector()->getWinSize().width / 2 / GOAL_COUNT;
        
        CCNode* cat = this->getChildByTag(1);
        CCMoveBy* move = CCMoveBy::create(moveTime, ccp(moveDistanceForCat, 0));
        cat->runAction(move);
    }
}

void GameLayer::setAnimationManager(CCBAnimationManager *pAnimationManager)
{
    CC_SAFE_RELEASE_NULL(mAnimationManager);
    mAnimationManager = pAnimationManager;
    CC_SAFE_RETAIN(mAnimationManager);
}