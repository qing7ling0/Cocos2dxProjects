#pragma once
#include "QConfig.h"

#define ADD_IMAGE_ASYNC(_PATH_, _COUNT_) \
	Director::getInstance()->getTextureCache()->addImageAsync(_PATH_, this, callfuncO_selector(BattleRes::loadCallback)); \
	_COUNT_++; \

extern const char s_pathBattleVs[];
extern const char s_pathBattleRoundEnd1[];
extern const char s_pathBattleRoundEnd2[];
extern const char s_pathBattleBg[];
extern const char s_pathBattleYourTurn[];
extern const char s_pathBattleNormal_1_1[];
extern const char s_pathBattleNormal_1_2[];
extern const char s_pathBattleNormal_1_3[];
extern const char s_pathBattleNumBg[];


class BattleRes : public Object
{
public:
	void load();

	void loadCallback(Object *obj);

	float loadProgress();

protected:
	int totalCount;
	int loadCount;
};