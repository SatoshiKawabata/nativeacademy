#include "PhysicsSprite.h"
#include "Config.h"

PhysicsSprite::PhysicsSprite()
:m_pBody(NULL)
,deleteFlag(false)
{
}

PhysicsSprite::~PhysicsSprite()
{
    // m_pBody(b2Body)はこのクラスの利用者側で
    //   　world->DestroyBody(pSprite->getPhysicsBody());
    // とするので、ここでは何もしない。
    // 実はb2Bodyクラスのデストラクタがprivateになっているため
    // 勝手に解放できないようになっている。
}

void PhysicsSprite::setPhysicsBody(b2Body* body)
{
    m_pBody = body;
}

b2Body* PhysicsSprite::getPhysicsBody()
{
    return m_pBody;
}

// CCSpriteクラスの関数をオーバーロード

bool PhysicsSprite::isDirty(void)
{
    return true;
}

// CCNodeクラスの関数をオーバーロード
// 表示されている画像を m_pBody に追従させるコード
// 参考書籍には「図形の変形などを行ったりするアフィン変換を利用した処理」とあり、それ以上詳しい解説は無く画像処理の専門書などを参照するように書いてある。
CCAffineTransform PhysicsSprite::nodeToParentTransform(void)
{
    b2Vec2 pos = m_pBody->GetPosition();

    float x = pos.x * PTM_RATIO;
    float y = pos.y * PTM_RATIO;

    if (isIgnoreAnchorPointForPosition()) {
        x += m_obAnchorPointInPoints.x;
        y += m_obAnchorPointInPoints.y;
    }

    float radians = m_pBody->GetAngle();
    float c = cosf(radians);
    float s = sinf(radians);

    if (!m_obAnchorPointInPoints.equals(CCPointZero)) {
        x += c * -m_obAnchorPointInPoints.x + -s * -m_obAnchorPointInPoints.y;
        y += s * -m_obAnchorPointInPoints.x +  c * -m_obAnchorPointInPoints.y;
    }

    m_sTransform = CCAffineTransformMake(c, s, -s, c, x, y);
    return m_sTransform;
}

// 衝突判定用のフラグの設定
void PhysicsSprite::setDeleteFlag(bool flag)
{
    deleteFlag = flag;
}

// 衝突判定用のフラグの取得
bool PhysicsSprite::getDeleteFlag()
{
    return deleteFlag;
}
