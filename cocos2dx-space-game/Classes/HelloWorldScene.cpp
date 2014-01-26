#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    
    _touchFlag = false;
    _moveShipPos = ccp(0, 0);

    // テクスチャ
    _batchNode = CCSpriteBatchNode::create("Sprites.pvr.ccz");
    this->addChild(_batchNode);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Sprites.plist");

    // プレイヤーの機体
    _ship = CCSprite::createWithSpriteFrameName("SpaceFlier_sm_1.png");
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    _ship->setPosition(ccp(winSize.width * 0.1, winSize.height * 0.5));
    _batchNode->addChild(_ship, 1);
    
    // 1) Create the CCParallaxNode
    _backgroundNode = CCParallaxNodeExtras::node();
    this->addChild(_backgroundNode,-1) ;
    
    // 2) Create the sprites will be added to the CCParallaxNode
    _spacedust1 = CCSprite::create("bg_front_spacedust.png");
    _spacedust2 = CCSprite::create("bg_front_spacedust.png");
    _planetsunrise = CCSprite::create("bg_planetsunrise.png");
    _galaxy = CCSprite::create("bg_galaxy.png");
    _spacialanomaly = CCSprite::create("bg_spacialanomaly.png");
    _spacialanomaly2 = CCSprite::create("bg_spacialanomaly2.png");
    
    // 3) Determine relative movement speeds for space dust and background
    CCPoint dustSpeed = ccp(0.1, 0.1);
    CCPoint bgSpeed = ccp(0.05, 0.05);
    
    // 4) Add children to CCParallaxNode
    _backgroundNode->addChild(_spacedust1, 0, dustSpeed , ccp(0,winSize.height/2) ); // 2
    _backgroundNode->addChild(_spacedust2, 0, dustSpeed , ccp( _spacedust1->getContentSize().width,winSize.height/2));
    _backgroundNode->addChild(_galaxy,-1, bgSpeed, ccp(0,winSize.height * 0.7));
    _backgroundNode->addChild(_planetsunrise, -1 , bgSpeed, ccp(600, winSize.height * 0));
    _backgroundNode->addChild(_spacialanomaly, -1, bgSpeed, ccp(900, winSize.height * 0.3));
    _backgroundNode->addChild(_spacialanomaly2, -1, bgSpeed, ccp(1500, winSize.height * 0.9));
    
    this->scheduleUpdate();

    HelloWorld::addChild(CCParticleSystemQuad::create("Stars1.plist"));
    HelloWorld::addChild(CCParticleSystemQuad::create("Stars2.plist"));
    HelloWorld::addChild(CCParticleSystemQuad::create("Stars3.plist"));
    
    this->setAccelerometerEnabled(true);
    
    // 小惑星
    #define KNUMASTEROIDS 15
    _asteroids = new CCArray();
    for(int i = 0; i < KNUMASTEROIDS; ++i)
    {
        Asteroid *asteroid = Asteroid::createWithSpriteFrameName("asteroid.png");
        asteroid->setVisible(false);
        _batchNode->addChild(asteroid);
        _asteroids->addObject(asteroid);
    }
    
    // プレイヤーのレーザー
    #define KNUMLASERS 100
    _shipLasers = new CCArray();
    for(int i = 0; i < KNUMLASERS; ++i)
    {
        CCSprite *shipLaser = CCSprite::createWithSpriteFrameName("laserbeam_blue.png");
        shipLaser->setVisible(false);
        _batchNode->addChild(shipLaser);
        _shipLasers->addObject(shipLaser);
    }
    // 敵のレーザー
    _enemyLasers = new CCArray();
    for(int i = 0; i < KNUMLASERS; ++i)
    {
        CCSprite *enemyLaser = CCSprite::createWithSpriteFrameName("laserbeam_blue.png");
        enemyLaser->setVisible(false);
        _batchNode->addChild(enemyLaser);
        _enemyLasers->addObject(enemyLaser);
    }
    this->setTouchEnabled(true);
    
    // 残機
    _lives = 3;
    double curTime = getTimeTick();
    _gameOverTime = curTime + 30000;
    
    // 音
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic("SpaceGame.wav",true);
    SimpleAudioEngine::sharedEngine()->preloadEffect("explosion_large.wav");
    SimpleAudioEngine::sharedEngine()->preloadEffect("laser_ship.wav");
    
    return true;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

/**
 * 毎フレーム処理
 */
void HelloWorld::update(float dt)
{
    // パララックスノードの位置を更新
    CCPoint backgroundScrollVert = ccp(-1000,0);
    _backgroundNode->setPosition(ccpAdd(_backgroundNode->getPosition(), ccpMult(backgroundScrollVert, dt)));
    
    CCArray *spaceDusts = CCArray::createWithCapacity(2);
    spaceDusts->addObject(_spacedust1);
    spaceDusts->addObject(_spacedust2);
    for ( int ii = 0; ii <spaceDusts->count(); ii++ )
    {
        CCSprite * spaceDust = (CCSprite *)(spaceDusts->objectAtIndex(ii));
        float xPosition = _backgroundNode->convertToWorldSpace(spaceDust->getPosition()).x;
        float size = spaceDust->getContentSize().width;
        // 画面外に出たら
        if ( xPosition < -size/2 )
        {
            // パララックスノードから消去する
            _backgroundNode->incrementOffset(ccp(spaceDust->getContentSize().width*2,0),spaceDust);
        }
    }
    
    // 
    CCArray *backGrounds = CCArray::createWithCapacity(4);
    backGrounds->addObject(_galaxy);
//    backGrounds->addObject(_planetsunrise);
//    backGrounds->addObject(_spacialanomaly);
//    backGrounds->addObject(_spacialanomaly2);
    for ( int ii = 0; ii <backGrounds->count(); ii++ )
    {
        CCSprite * background = (CCSprite *)(backGrounds->objectAtIndex(ii));
        float xPosition = _backgroundNode->convertToWorldSpace(background->getPosition()).x;
        float size = background->getContentSize().width;
        // 画面外に出たら
        if ( xPosition < -size )
        {
            _backgroundNode->incrementOffset(ccp(2000,0),background);
        }
    }
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    float maxX = winSize.width - _ship->getContentSize().width/2;
    float maxY = winSize.height - _ship->getContentSize().height/2;
    float minX = _ship->getContentSize().width/2;
    float minY = _ship->getContentSize().height/2;
    
    float newX = _ship->getPosition().x + _moveShipPos.x;
    float newY = _ship->getPosition().y + _moveShipPos.y;
    newX = MIN(MAX(newX, minX), maxX);
    newY = MIN(MAX(newY, minY), maxY);
    _ship->setPosition(ccp(newX, newY));
    _moveShipPos = ccp(0, 0);
    
    // おそらくここで小惑星出してる
    float curTimeMillis = getTimeTick();
    if (curTimeMillis > _nextAsteroidSpawn)
    {
        float randMillisecs = randomValueBetween(0.20,1.0) * 1000;
        _nextAsteroidSpawn = randMillisecs + curTimeMillis;
        
        float randY = randomValueBetween(0.0,winSize.height);
        float randDuration = randomValueBetween(2.0,10.0);
        
        // 配列から小惑星取得
        CCSprite *asteroid = (CCSprite *)_asteroids->objectAtIndex(_nextAsteroid);
        _nextAsteroid++;
        
        if (_nextAsteroid >= _asteroids->count())
            _nextAsteroid = 0;
        
        // すべての動きを停止
        asteroid->stopAllActions();
        // 初期座標を設定
        asteroid->setPosition(
                              ccp(winSize.width+asteroid->getContentSize().width/2
                                  , randY)
                              );
        // 表示する
        asteroid->setVisible(true);
        // アニメーション再生
        asteroid->runAction(CCSequence::create(CCMoveBy::create(randDuration, ccp(-winSize.width-asteroid->getContentSize().width, 0)), CCCallFuncN::create(this, callfuncN_selector(HelloWorld::setInvisible)), NULL // DO NOT FORGET TO TERMINATE WITH NULL (unexpected in C++)
                                               ));        
    }
    
    // Asteroids
    CCObject* asteroid;
    CCObject* shipLaser;
    // 小惑星ループ
    CCARRAY_FOREACH(_asteroids, asteroid)
    {
        if (!((CCSprite *) asteroid)->isVisible() )
            continue;
        // レーザーループ
        CCARRAY_FOREACH(_shipLasers, shipLaser)
        {
            if (!((CCSprite *) shipLaser)->isVisible())
                continue;
            // 当たり判定処理
            hitProcess(shipLaser, asteroid);
        }
        // 船と小惑星の当たり判定
        if (_ship->boundingBox().intersectsRect(((CCSprite *)asteroid)->boundingBox()) )
        {
            ((CCSprite *)asteroid)->setVisible(false);
            _ship->runAction( CCBlink::create(1.0, 9));
            _lives--;
        }
    }
    
    // ゲームオーバー処理
    if (_lives <= 0)
    {
        _ship->stopAllActions();
        _ship->setVisible(false);
        this->endScene(KENDREASONLOSE);
    }
    else if (curTimeMillis >= _gameOverTime)
    {
        this->endScene(KENDREASONWIN);
    }
}

/**
 * ヒット処理
 */
void HelloWorld::hitProcess(CCObject* bounding, CCObject* target)
{
    // boundingとtargetとの当たり判定
    if (((CCSprite *) bounding)->boundingBox().intersectsRect(((CCSprite *)target)->boundingBox()) )
    {
        // 音をならす
        SimpleAudioEngine::sharedEngine()->playEffect("explosion_large.wav");
        // 非表示にする処理
        ((CCSprite *)bounding)->setVisible(false);
        ((CCSprite *)target)->setVisible(false);
        return;
    }
}

/*
 * ランダム数値生成
 */
float HelloWorld::randomValueBetween(float low, float high) {
    return (((float) arc4random() / 0xFFFFFFFFu) * (high - low)) + low;
}

float HelloWorld::getTimeTick() {
    timeval time;
    gettimeofday(&time, NULL);
    unsigned long millisecs = (time.tv_sec * 1000) + (time.tv_usec/1000);
    return (float) millisecs;
}

void HelloWorld::setInvisible(CCNode * node) {
    node->setVisible(false);
}

void HelloWorld::restartTapped() {
    CCDirector::sharedDirector()->replaceScene
    (CCTransitionZoomFlipX::create(0.5, this->scene()));
    // reschedule
    this->scheduleUpdate();
}

void HelloWorld::endScene( EndReason endReason ) {
    if (_gameOver)
        return;
    _gameOver = true;
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    char message[10] = "You Win";
    if ( endReason == KENDREASONLOSE)
        strcpy(message,"You Lose");
    CCLabelBMFont * label ;
    label = CCLabelBMFont::create(message, "Arial.fnt");
    label->setScale(0.1);
    label->setPosition(ccp(winSize.width/2 , winSize.height*0.6));
    this->addChild(label);
    
    CCLabelBMFont * restartLabel;
    strcpy(message,"Restart");
    restartLabel = CCLabelBMFont::create(message, "Arial.fnt");
    CCMenuItemLabel *restartItem = CCMenuItemLabel::create(restartLabel, this, menu_selector(HelloWorld::restartTapped) );
    restartItem->setScale(0.1);
    restartItem->setPosition( ccp(winSize.width/2, winSize.height*0.4));
    
    CCMenu *menu = CCMenu::create(restartItem, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu);
    
    // clear label and menu
    restartItem->runAction(CCScaleTo::create(0.5, 1.0));
    label ->runAction(CCScaleTo::create(0.5, 1.0));
    // Terminate update callback
    this->unscheduleUpdate();
}


void HelloWorld::ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
    _touchFlag = true;
    CCTouch *myTouch = (CCTouch*)touches->anyObject();
    CCPoint location = myTouch->getLocationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);
    _startTouchPos = location;
    
    SimpleAudioEngine::sharedEngine()->playEffect("laser_ship.wav");
    
    // レーザー射出
    shotLaser();
    
    _lastTouchPos = location;
}

void HelloWorld::ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
    CCTouch *myTouch = (CCTouch*)touches->anyObject();
    CCPoint location = myTouch->getLocationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);
    
    // プレイヤーの船を動かす
    _moveShipPos = CCPointMake(location.x - _lastTouchPos.x, location.y - _lastTouchPos.y);
    
    // レーザー射出
    shotLaser();
    
    _lastTouchPos = location;
}


void HelloWorld::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
    _touchFlag = false;
}

/**
 * レーザー射出
 */
void HelloWorld::shotLaser()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    // レーザー射出
    CCSprite *shipLaser = (CCSprite *)_shipLasers->objectAtIndex(_nextShipLaser++);
    if ( _nextShipLaser >= _shipLasers->count() )
        _nextShipLaser = 0;
    // レーザーの初期座標設定
    shipLaser->setPosition( ccpAdd( _ship->getPosition(), ccp(shipLaser->getContentSize().width/2, 0)));
    // 表示する
    shipLaser->setVisible(true);
    // すべてのアクションを停止する
    shipLaser->stopAllActions();
    // アクションを再生する
    shipLaser->runAction(CCSequence::create(CCMoveBy::create(0.5,ccp(winSize.width, 0)), CCCallFuncN::create(this, callfuncN_selector(HelloWorld::setInvisible)), NULL  // DO NOT FORGET TO TERMINATE WITH NULL
                                            ));
}






