#include "GameScene.h"
#include "GamePhysicsContactListener.h"

using namespace cocos2d;
using namespace std;

// コンストラクタ
GameScene::GameScene() :
		world(NULL), spriteBatchNode(NULL), targetSprite(NULL) {
}

// デストラクタ
GameScene::~GameScene() {

	// 残っているスプライトの削除
	// イテレータの取得
	list<PhysicsSprite*>::iterator it = pSpriteList.begin();
	while (it != pSpriteList.end()) {
		// spriteの取得
		PhysicsSprite* pSprite = *it;
		// Box2d上の要素の削除
		world->DestroyBody(pSprite->getPhysicsBody());
		// spriteの削除
		pSprite->removeFromParentAndCleanup(true);
		// イテレータを進める
		it++;
	}

	// box2d worldの削除
	CC_SAFE_DELETE(world);
}

// シーンの作成
CCScene* GameScene::scene() {
	CCScene* scene = CCScene::create();
	GameScene* layer = GameScene::create();
	scene->addChild(layer);

	return scene;
}

// シーンの初期化
bool GameScene::init() {
	if (!CCLayer::init()) {
		return false;
	}

	// 物理エンジン空間の初期化
	initPhysics();

	// 地面の初期化
	createGround();

	// 物質の初期化
	initSprite();
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCPoint p = ccp(size.width * 0.5, size.height * 0.5);
	createSprite(p);

	// 物理エンジン空間の更新
	scheduleUpdate();

	// タッチ有効化
	setTouchEnabled(true);
	setTouchMode(kCCTouchesOneByOne);

	return true;
}

// 物理エンジンの初期化
void GameScene::initPhysics() {
	// 重力の設定
	b2Vec2 gravity;
	gravity.Set(0.0, -10.0);

	// worldを作成
	world = new b2World(gravity);

	// 衝突判定の初期化
	GamePhysicsContactListener* contactListener =
			new GamePhysicsContactListener();
	world->SetContactListener(contactListener);
}

void GameScene::update(float delta) {
	// 画面外のspriteの削除
	deleteSprite();

	// 物理シミュレーションの正確さを決定するパラメータ
	int velocityIterations = 8;
	int positionIterations = 1;

	// worldを更新する
	world->Step(delta, velocityIterations, positionIterations);
}

void GameScene::createGround() {
	CCSize size = CCDirector::sharedDirector()->getWinSize();

	/*
	 物理エンジン空間上に地面を設定する
	 */

	// 地面ノードの作成
	CCNode* pGround = CCNode::create();
	pGround->setTag(Config::kTag_Ground);
	this->addChild(pGround);

	// 地面の定義
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0, 0);
	groundBodyDef.userData = pGround;

	// 地面作成
	b2Body* groundBody = world->CreateBody(&groundBodyDef);

	// 地面の形と大きさの定義
	float groundHeight = size.height * 0.2;
	b2EdgeShape groundBox;
	groundBox.Set(b2Vec2(0, groundHeight / PTM_RATIO),
			b2Vec2(size.width / PTM_RATIO, groundHeight / PTM_RATIO));
	groundBody->CreateFixture(&groundBox, 0);
}

// スプライトの初期化
void GameScene::initSprite() {
	spriteBatchNode = CCSpriteBatchNode::create("CloseNormal.png");
	this->addChild(spriteBatchNode);
}

// スプライトの生成
void GameScene::createSprite(CCPoint p) {
	/* 画像の表示 */
	PhysicsSprite* pSprite = new PhysicsSprite();
	pSprite->autorelease();
	pSprite->initWithTexture(spriteBatchNode->getTexture());
    pSprite->setPosition(p);
	pSprite->setTag(Config::kTag_Sprite);
	this->addChild(pSprite);

	// 複数個のスプライトに対応するためListで管理
	pSpriteList.push_back(pSprite);

	/*
	 物理エンジン上の物理構造設定
	 */

	/* 物理エンジン上の物質の定義 */
	b2BodyDef spriteBodyDef;
	/*
	 物体の力に対する属性
	 b2_dynamicBody: 速度があり力に反応する
	 b2_staticBody: 速度が無く力に反応しない
	 b2_kinematicBody: 速度があり力に反応しない
	 */
	spriteBodyDef.type = b2_dynamicBody;
	/* 物理エンジンの空間上の座標 */
	spriteBodyDef.position.Set(pSprite->getPositionX() / PTM_RATIO,
			pSprite->getPositionY() / PTM_RATIO);
	spriteBodyDef.userData = pSprite;
	/* 物理エンジン上の物質作成 */
	b2Body* spriteBody = world->CreateBody(&spriteBodyDef);

	/* 物理エンジン上の物質の形と大きさ */
	b2CircleShape spriteShape;
	spriteShape.m_radius = pSprite->getContentSize().width * 0.3 / PTM_RATIO;

	/* 物質の性質定義（形、密度、摩擦） */
	b2FixtureDef spriteFixturedef;
	spriteFixturedef.shape = &spriteShape;
	spriteFixturedef.density = 1;
	spriteFixturedef.friction = 0.9;

	/* 物質の性質定義適用 */
	spriteBody->CreateFixture(&spriteFixturedef);

	/* 画像と物理エンジン上の物質の関連付け */
	pSprite->setPhysicsBody(spriteBody);
}

// 画面外に出たspriteの削除
void GameScene::deleteSprite() {
	// 画面サイズの取得
	CCSize size = CCDirector::sharedDirector()->getWinSize();

	// イテレータの取得
	list<PhysicsSprite*>::iterator it = pSpriteList.begin();
	while (it != pSpriteList.end()) {
		// spriteの取得
		PhysicsSprite* pSprite = *it;
		// spriteの位置の取得
		b2Vec2 position = pSprite->getPhysicsBody()->GetPosition();
		float x = position.x * PTM_RATIO;
		float y = position.y * PTM_RATIO;
		// 画面外に出たか、削除フラグが立っていたら、削除
		if (x < 0 || size.width < x || y < 0 || size.height < y
				|| pSprite->getDeleteFlag()) { // Box2d上の要素の削除
			world->DestroyBody(pSprite->getPhysicsBody());
			// spriteの削除
			pSprite->removeFromParentAndCleanup(true);
			// spriteのリストから削除＆削除した要素以降のイテレータの取得
			it = pSpriteList.erase(it);
		} else {
			// イテレータを進める
			it++;
		}
	}
}

bool GameScene::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent) {
	// タッチ開始時の座標を保存
	CCDirector* pDirector = CCDirector::sharedDirector();
	touchStartPoint = pDirector->convertToGL(pTouch->getLocationInView());

    CCLOG("[ccTouchBegan] s(%f, %f)", touchStartPoint.x, touchStartPoint.y);

    // 作成しているスプライトの座標と一致しているかどうかを調べる
    targetSprite = NULL;
	list<PhysicsSprite*>::iterator it = pSpriteList.begin();
	while (it != pSpriteList.end()) {
		PhysicsSprite* pSprite = *it;
		CCRect rect = pSprite->boundingBox();
		if (rect.containsPoint(touchStartPoint)) {
			targetSprite = pSprite; // 見つかったので座標を覚えておく
			break;
		}
		it++;
	}

	return true;
}

void GameScene::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent) {
	// タッチ終了時の座標を取得
	CCDirector* pDirector = CCDirector::sharedDirector();
	CCPoint touchEndPoint = pDirector->convertToGL(pTouch->getLocationInView());

	// 物体をタッチしていなければ新規作成
	if (targetSprite == NULL) {
        CCLOG("create");
		createSprite(touchEndPoint);
	}
	// 物体をタッチしていれば、タッチ開始から終了の座標を計算して、物体に力を与える
	else {
		b2Body* body = targetSprite->getPhysicsBody();
		float x = touchEndPoint.x - touchStartPoint.x;
		float y = touchEndPoint.y - touchStartPoint.y;
		CCLOG(
				"[ccTouchEnded] s(%f, %f) e(%f, %f) d(%f, %f)", touchStartPoint.x, touchStartPoint.y, touchEndPoint.x, touchEndPoint.y, x, y);
		body->ApplyForce(b2Vec2(x, y), body->GetWorldCenter());
	}
}
