//
//  GameLayer.cpp
//  nyandash
//
//  Created by cocos2d-x on 2014/01/19.
//
//

#include "GameLayer.h"
#include "GameLayerLoader.h"

GameLayer::GameLayer()
: mBg1(NULL)
, mBg2(NULL)
, mBg3(NULL)
{
}

GameLayer::~GameLayer()
{
    CC_SAFE_RELEASE(mBg1);
    CC_SAFE_RELEASE(mBg2);
    CC_SAFE_RELEASE(mBg3);
}

SEL_MenuHandler GameLayer::onResolveCCBCCMenuItemSelector(CCObject* pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "tappedLeftFoot", GameLayer::tappedLeftFoot);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "tappedRightFoot", GameLayer::tappedRightFoot);
    
    return NULL;
}

SEL_CCControlHandler GameLayer::onResolveCCBCCControlSelector(CCObject* pTarget, const char* pSelectorName)
{
    return NULL;
}

void GameLayer::tappedLeftFoot(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent)
{
    tappedFoot();
}

void GameLayer::tappedRightFoot(cocos2d::CCObject *pSender, CCControlEvent pCCControlEvent)
{
    tappedFoot();
}

void GameLayer::tappedFoot()
{
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

bool GameLayer::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBg1", CCSprite*, this->mBg1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBg2", CCSprite*, this->mBg2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBg3", CCSprite*, this->mBg3);
    
    return false;
}