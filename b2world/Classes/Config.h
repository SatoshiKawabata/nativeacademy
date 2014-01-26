#ifndef b2test_Config_h
#define b2test_Config_h

// Pixel To Meter : nピクセルが1メートルという意味
#define PTM_RATIO 32

class Config
{
public:
    enum kTag {			// タグ(主に物理衝突判定用)
        kTag_Ground,	// 床
        kTag_Sprite		// スプライト
    };
};

#endif
