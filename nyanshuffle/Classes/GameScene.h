#ifndef __GMAE_SCENEH__
#define __GAME_SCENE__

#include "cocos2d.h"

USING_NS_CC;

class GameScene : public cocos2d::CCLayer
{
public:
    virtual bool init();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(GameScene);
protected:
    CCSprite* m_pCat;
    CCMenuItemImage* m_pBox1;
    CCMenuItemImage* m_pBox2;
    CCMenuItemImage* m_pBox3;
private:
    void initGameScene();
    void playDroppingSound();
    
    void showCountDown();
    void playCountDownSound();
    void startShuffle();
};

#endif /* defined(__GAME_SCENE__) */
