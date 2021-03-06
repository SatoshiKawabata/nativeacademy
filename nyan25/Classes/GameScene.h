#ifndef __GMAE_SCENEH__
#define __GAME_SCENE__

#include "cocos2d.h"

class GameScene : public cocos2d::CCLayer
{
public:
    // 仮想関数
    virtual bool init();
    // staticなファクトリーメソッド
    static cocos2d::CCScene* scene();
    // マクロ（セッターゲッターを自動生成する）cocos特有
    CREATE_FUNC(GameScene);
    
    // タッチ開始
    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    // タッチ終了
    virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
private:
    int nextNumber;
    float gametime;
    
    void initialize();
    void endGame();
    void makeBackground();
    void makeCards();
    void measureGametime(float fDelta);
    void showGametimeLabel(float _gametime);
    void makeRetryButton();
    void tapRetryButton(CCNode *node);
    void showHighScoreLabel();
};

#endif