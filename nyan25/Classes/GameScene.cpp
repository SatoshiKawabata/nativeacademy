#include "GameScene.h"

// 名前空間定義
using namespace cocos2d;// "cocos2d::"を使わなくてもいい
using namespace std;

/**
 * このクラスのインスタンスを作るファクトリーメソッド
 */
CCScene* GameScene::scene()
{
    CCScene* scene = CCScene::create();
    GameScene* layer = GameScene::create();
    scene->addChild(layer);
    
    return scene;
}

/**
 * 初期化
 */
bool GameScene::init()
{
    // スーパークラスの初期化
    if (!CCLayer::init())
    {
        return false;
    }
    
    // 初期化関数
    initialize();
    
    // 初期化成功
    return true;
}

/**
 * ゲームに関する初期化処理
 */
void GameScene::initialize()
{
    // 変数のシードを設定する
    srand((unsigned)time(NULL));
    nextNumber = 1;
    // ゲームタイムの初期化
    gametime = 0.f;
    
    // タップイベント取得する
    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);
    
    // 背景を設置する
    makeBackground();
    // 1~25のカードを配置する
    makeCards();
    
    // ゲーム時間を表示する
    showGametimeLabel(gametime);
    
    // ゲーム時間をカウントアップする関数を毎フレーム呼び出す
    this->schedule(schedule_selector(GameScene::measureGametime));
}

/**
 * ゲーム終了処理
 */
void GameScene::endGame()
{
    // ゲーム時間の計測を停止する
    this->unschedule(schedule_selector(GameScene::measureGametime));
    // リトライボタンを作成する
    makeRetryButton();
    
    // ハイスコアを表示する
    showHighScoreLabel();
}

/**
 * 背景を作成する
 */
void GameScene::makeBackground()
{
    // 画面サイズ取得
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    // 背景を生成
    CCSprite* pBG = CCSprite::create("background.png");
    // デフォルトでは画像の中心がアンカーポイントなので画面の中央の座標を設定する
    pBG->setPosition(ccp(winSize.width * 0.5, winSize.height * 0.5));
    this->addChild(pBG);
    /** pBGはGameSceneが解放されるときに解放される **/
}

/**
 * カードを生成して配置する
 */
void GameScene::makeCards()
{
    // 数値配列を初期化する
    CCArray* numbers = CCArray::create();
    for (int i = 1; i <= 25; i++)
        numbers->addObject(CCInteger::create(i));
    
    // 画面サイズを取得
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    // カードを5*5に配置する
    for (int x = 0; x < 5; x++) {
        for (int y = 0; y < 5; y++) {
            // ランダムで一つの値を取得する
            int index = rand() % numbers->count();

            int number = ((CCInteger*)numbers->objectAtIndex(index))->getValue();
            // カードを生成する
            CCSprite* pCard = CCSprite::create("card_frontside.png");
            CCSize cardSize = pCard->getContentSize();
            pCard->setPosition(ccp(winSize.width * 0.5 + (x - 2) * cardSize.width, winSize.height * 0.5 + (y - 2) * cardSize.height));
            pCard->setTag(number);
            this->addChild(pCard);
            
            // カード番号を表示
            CCString* fileName = CCString::createWithFormat("%d.png", number);
            // C言語の文字列を取得する
            CCSprite* pNumber = CCSprite::create(fileName->getCString());
            pNumber->setPosition(ccp(cardSize.width * 0.5, cardSize.height * 0.5));
            pCard->addChild(pNumber);
            
            // 数値配列から値を削除する
            numbers->removeObjectAtIndex(index);
            
            //CCLog(CCString::createWithFormat("%d", rand())->getCString());
        }
    }
}

/**
 * タッチ開始処理（おまじない的な）
 */
bool GameScene::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    return true;
}

/**
 * タッチ終了処理
 */
void GameScene::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    // タップポイント取得
    CCDirector *pDirector = CCDirector::sharedDirector();
    // openGLの座標系に変換
    CCPoint touchPoint = pDirector->convertToGL(pTouch->getLocationInView());
    // 出力
    CCLog("x: %f, y: %f", touchPoint.x, touchPoint.y);
    
    //
    CCNode* pCard = this->getChildByTag(nextNumber);
    if (!pCard)
        return;
    
    
    CCRect cardRect = pCard->boundingBox();
    if (cardRect.containsPoint(touchPoint))
    {
        // 裏カードを作成する
        CCSprite* pNewCard = CCSprite::create("card_backside.png");
        pNewCard->setPosition(pCard->getPosition());
        this->addChild(pNewCard);
        // 表カードを削除する
        pCard->removeFromParentAndCleanup(true);
        
        // ゲーム終了
        if (nextNumber >= 25)
        {
            // ゲーム終了処理
            endGame();
            return;
        }
        
        nextNumber++;
    }
    
}

/**
 * スケジューラのイベントハンドラ
 */
void GameScene::measureGametime(float fDelta)
{
    gametime += fDelta;
    CCLog("gametime: %f", gametime);
    
    showGametimeLabel(gametime);
}

/**
 * ゲーム時間を表示する
 */
void GameScene::showGametimeLabel(float _gametime)
{
    // ゲーム時間ラベル用タグ
    const int tagGameLabel = 100;
    
    // ゲーム時間を文字列に変換する
    CCString* timeString = CCString::createWithFormat("%8.1fs", _gametime);
    
    // ゲーム時間ラベルを取得
    CCLabelTTF* timerLabel = (CCLabelTTF*)this->getChildByTag(tagGameLabel);
    if (timerLabel)
    {
        // ゲーム時間を更新する
        timerLabel->setString(timeString->getCString());
    }
    else
    {
        // 画面サイズを取得する
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        
        // ゲーム時間ラベルを生成する
        timerLabel = CCLabelTTF::create(timeString->getCString(), "Arial", 24.0);
        timerLabel->setPosition(ccp(winSize.width * 0.9, winSize.height * 0.9));
        timerLabel->setTag(tagGameLabel);
        this->addChild(timerLabel);
    }
}

/**
 * リトライボタンを生成する
 */
void GameScene::makeRetryButton()
{
    // 画面サイズを取得する
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    // リトライボタンを作成する
    CCLabelTTF* retryLabel = CCLabelTTF::create("リトライ", "Arial", 24.0);
    CCMenuItemLabel* retryItem = CCMenuItemLabel::create(retryLabel, this, menu_selector(GameScene::tapRetryButton));
    retryItem->setPosition(ccp(winSize.width * 0.9, winSize.height * 0.2));
    
    // メニューを作成する
    CCMenu* menu = CCMenu::create(retryItem, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu);
}

/**
 * リトライボタンタップハンドラ
 */
void GameScene::tapRetryButton(cocos2d::CCNode *node)
{
    // 新しくゲームのシーンを用意する
    CCScene* gameScene = (CCScene*)GameScene::create();
    CCDirector::sharedDirector()->replaceScene(gameScene);
}

/**
 * ハイスコアを表示する
 */
void GameScene::showHighScoreLabel()
{
    // CCUserDefaultクラスのインスタンスを取得する（データの永続化に関するクラス）
    CCUserDefault* userDefault = CCUserDefault::sharedUserDefault();
    
    // ハイスコアのキー取得する
    const char* highScoreKey = "highscore";
    
    // 以前のハイスコアの取得する
    float highScore = userDefault->getFloatForKey(highScoreKey, 99.9);
    if (gametime != 0)
    {
        if (gametime > highScore)
        {
            // ハイスコアが更新されていない場合は抜ける
            return;
        }
        else
        {
            // ハイスコアを更新する
            highScore = gametime;
            // ハイスコアを記録する
            userDefault->setFloatForKey(highScoreKey, highScore);
            userDefault->flush();   // 実際に記録する
        }
    }
    
    // ハイスコアラベル用タグ
    const int tagHighScoreLabel = 200;
    // ハイスコアを表示する文字列を生成する
    CCString* highScoreString = CCString::createWithFormat("%8.1fs", highScore);
    
    // ハイスコアラベルを取得する
    CCLabelTTF* highScoreLabel = (CCLabelTTF*)this->getChildByTag(tagHighScoreLabel);
    if (highScoreLabel)
    {
        // ハイスコアラベルを更新する
        highScoreLabel->setString(highScoreString->getCString());
    }
    else
    {
        // 画面サイズを取得する
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        
        // ハイスコアラベルを生成する
        highScoreLabel = CCLabelTTF::create(highScoreString->getCString(), "Arial", 24.0);
        highScoreLabel->setPosition(ccp(winSize.width * 0.9, winSize.height * 0.7));
        highScoreLabel->setTag(tagHighScoreLabel);
        this->addChild(highScoreLabel);
    }
}


















