#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Asteroid.h"
#include "CCParallaxNodeExtras.h"

USING_NS_CC;

typedef enum {
    KENDREASONWIN,
    KENDREASONLOSE
} EndReason;

class HelloWorld : public cocos2d::CCLayer
{
private:
	cocos2d::CCSpriteBatchNode * _batchNode;
    // プレイヤー
	cocos2d::CCSprite * _ship;
    
    CCParallaxNodeExtras *_backgroundNode;
    CCSprite *_spacedust1;
    CCSprite *_spacedust2;
    CCSprite *_planetsunrise;
    CCSprite *_galaxy;
    CCSprite *_spacialanomaly;
    CCSprite *_spacialanomaly2;
    
    // プレイヤーを動かすためのタッチ関連
    float _shipPointsPerSecY;
    bool    _touchFlag;
    CCPoint _startTouchPos;
    CCPoint _lastTouchPos;
    CCPoint _moveShipPos;

    // 小惑星
    CCArray* _asteroids;
    int _nextAsteroid;
    float _nextAsteroidSpawn;
    
    // レーザーオブジェクト
    CCArray* _shipLasers;
    // 操作する対象のレーザー
    int _nextShipLaser;
    
    // レーザーオブジェクト
    CCArray* _enemyLasers;
    // 操作する対象のレーザー
    int _nextEnemyLaser;
    
    // 残機
    int _lives;
    
    // 制限時間
    double _gameOverTime;
    bool _gameOver;
    
    // scheduled Update
    void update(float dt);
    
    void endScene(EndReason endReason);
    void restartTapped();
    
    // ヒット処理
    void hitProcess(CCObject* bounding, CCObject* target);
    void shotLaser();
    
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
    
    float randomValueBetween(float low, float high);
    void setInvisible(CCNode * node);
    float getTimeTick();
    
    // タッチ処理
    virtual void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    virtual void ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
};

#endif // __HELLOWORLD_SCENE_H__
