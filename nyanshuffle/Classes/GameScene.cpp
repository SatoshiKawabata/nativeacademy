//
//  GameScene.cpp
//  nyanshuffle
//
//  Created by cocos2d-x on 2014/01/13.
//
//

#include "GameScene.h"
#include "Animation.h"
#include "SimpleAudioEngine.h"

// name space define
using namespace cocos2d;
using namespace std;
using namespace CocosDenshion;

CCScene* GameScene::scene()
{
    CCScene* scene = CCScene::create();
    GameScene* layer = GameScene::create();
    scene->addChild(layer);
    
    return scene;
}

bool GameScene::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    
    initGameScene();
    
    return true;
}

void GameScene::initGameScene()
{
    // 初期化処理
    CCLog("initialize GameScene");
    // 乱数初期化
    srand((unsigned int)time(NULL));
    int random = rand();
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    // 背景の表示
    CCSprite* pBG = CCSprite::create("game_bg.png");
    pBG->setPosition(ccp(size.width * 0.5, size.height * 0.5));
    this->addChild(pBG);
    
    CCSize bgSize = pBG->getContentSize();
    
    // 雲のアクション作成
    CCMoveBy* cloud1Action = CCMoveBy::create(80, ccp(bgSize.width, 0));
    CCMoveBy* cloud2Action = CCMoveBy::create(160, ccp(bgSize.width, 0));
    
    // 雲の表示
    CCSprite* pCloud1 = CCSprite::create("cloud.png");
    pCloud1->setPosition(ccp(bgSize.width * 0.4, bgSize.height * 0.83));
    pCloud1->runAction(cloud1Action);
    pBG->addChild(pCloud1);
        CCSprite* pCloud2 = CCSprite::create("cloud.png");
    pCloud2->setPosition(ccp(bgSize.width * 0.4, bgSize.height * 0.9));
    pCloud2->runAction(cloud2Action);
    pCloud2->setScale(0.6);
    pBG->addChild(pCloud2);
    
    // ネコの表示
    CCString* catInagePath = CCString::createWithFormat("cat%d.png", random % 5 + 1);
    m_pCat = CCSprite::create(catInagePath->getCString());
    m_pCat->setPosition(ccp(size.width * 0.5, size.height * 0.5));
    m_pCat->runAction(Animation::catStartAnimation());
    this->addChild(m_pCat);
    
    // 箱の表示
    CCString* boxFileName = CCString::createWithFormat("box%d.png", random % 2 + 1);
    m_pBox1 = CCMenuItemImage::create(boxFileName->getCString(), boxFileName->getCString(), NULL, NULL);
    m_pBox1->setPosition(ccp(size.width * 0.5, size.height * 0.5 + size.height));
    m_pBox1->runAction(Animation::boxiesStartAction(this, callfunc_selector(GameScene::playDroppingSound)));
    
    m_pBox2 = CCMenuItemImage::create(boxFileName->getCString(), boxFileName->getCString(), NULL, NULL);
    m_pBox2->setPosition(ccp(size.width * 0.2, size.height * 0.5 + size.height));
    m_pBox2->runAction(Animation::boxiesStartAction(this, callfunc_selector(GameScene::playDroppingSound)));
    
    m_pBox3 = CCMenuItemImage::create(boxFileName->getCString(), boxFileName->getCString(), NULL, NULL);
    m_pBox3->setPosition(ccp(size.width * 0.8, size.height * 0.5 + size.height));
    m_pBox3->runAction(Animation::boxiesStartAction(this, callfunc_selector(GameScene::playDroppingSound)));
    
    // メニューを表示させる
    CCMenu* menu = CCMenu::create(m_pBox1, m_pBox2, m_pBox3, NULL);
    menu->setPosition(CCPointZero);
    
    this->addChild(menu);
}

void GameScene::playDroppingSound()
{
    SimpleAudioEngine::sharedEngine()->playEffect("rakka.mp3");
}

void GameScene::showCountDown()
{
    this->scheduleOnce(schedule_selector(GameScene::playCountDownSound), 0.5f);
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    // "3"の表示
    CCSprite* pNum3 = CCSprite::create("3.png");
    pNum3->setPosition(ccp(size.width * 0.5f, size.height * 0.5f));
    pNum3->setScale(0.f);
    pNum3->setScale(0.f);
    pNum3->runAction(Animation::num3Animation());
    this->addChild(pNum3, 1);
    
    // "2"の表示
    CCSprite* pNum2 = CCSprite::create("2.png");
    pNum2->setPosition(pNum3->getPosition());
    pNum2->setScale(0.f);
    pNum2->runAction(Animation::num2Animation());
    this->addChild(pNum2, 2);
}









