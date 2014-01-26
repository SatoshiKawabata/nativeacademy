#include "HelloWorldScene.h"

#include "spine/Json.h"

USING_NS_CC;
USING_NS_CC_EXT;

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
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - pLabel->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(pLabel, 1);

    // add "HelloWorld" splash screen"
    CCSprite* pSprite = CCSprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(pSprite, 0);

    // ================================================
    // ここから
    // 元ネタ：
    //      http://www.cocos2d-x.org/wiki/How_to_use_CCHttpClient
    //      http://tks2.net/memo/?p=401
    //      http://www.slideshare.net/doraemonsss/cocos2dx-26341255
    //      https://gist.github.com/tks2shimizu/a1a03bc42bcad87f6c57
    // ================================================
    // リクエスト生成
    CCHttpRequest* request = new CCHttpRequest();
    request->setUrl("http://httpbin.org/post");
    request->setRequestType(CCHttpRequest::kHttpPost);
    request->setResponseCallback(this,
    		httpresponse_selector(HelloWorld::onHttpRequestCompleted));
    request->setTag("POST test1");

    // POSTデータ生成
    std::string postData = "visitor=cocos2d&TestSuite=Extensions Test/NetworkTest";
    // C言語文字列を渡す
    request->setRequestData(postData.c_str(), postData.length());

    // リクエスト送信
    CCHttpClient::getInstance()->send(request);
    
    // 解放処理
    request->release();

    return true;
}

void HelloWorld::onHttpRequestCompleted(CCHttpClient *sender, CCHttpResponse *response)
{
    if (!response)
        return;

    // タグ設定時は、タグをログに出力
    if (0 != strlen(response->getHttpRequest()->getTag()))
    {
        CCLOG("%s completed", response->getHttpRequest()->getTag());
    }

    // ステータスコード取得
    int statusCode = response->getResponseCode();
    CCLOG("response code: %d", statusCode);

    // ステータスコードの取得(このサンプルでは未使用)
    CCString* statusString = CCString::createWithFormat("HTTP Status Code: %d¥ntag = %s",
                                                    statusCode,
                                                    response->getHttpRequest()->getTag());

    // レスポンスが不正な場合は、エラーを出力して終了
    if (!response->isSucceed())
    {
        CCLOG("response failed");
        CCLOG("error buffer: %s", response->getErrorBuffer());
        return;
    }

    // 取得データの出力
    std::vector<char> *buffer = response->getResponseData();
    std::string bufferString(buffer->begin(), buffer->end());
    CCLOG("%s", bufferString.c_str());

    // 文字列の取得
    std::string output = getOutputString(bufferString);
    
    // ラベルに表示表示
    CCSize size = CCDirector::sharedDirector()->getVisibleSize();
    CCLabelTTF* label = CCLabelTTF::create(output.c_str(), "Arial", 24);
    label->setPosition(ccp(size.width / 2, size.height / 2));
    this->addChild(label, 1);
}

std::string HelloWorld::getOutputString(std::string data)
{
    // JSONのパース
    Json* json = Json_create(data.c_str());
    
    // 文字列の取得
    std::string url = Json_getString(json, "url", "");
    CCLOG("url: %s", url.c_str());
    
    // オブジェクト内の値の取得
    Json* headers = Json_getItem(json, "headers");
    std::string  length = Json_getString(headers, "Content-Length", "");
    
    CCString* retString = CCString::createWithFormat("%s len=%s", url.c_str(), length.c_str());
    
    return retString->getCString();
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}
