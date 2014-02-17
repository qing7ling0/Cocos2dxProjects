#include "BattleRes.h"

const char s_pathBattleVs[]						= "images/battle/vs.png";
const char s_pathBattleRoundEnd1[]				= "images/battle/btn_turn_end.png";
const char s_pathBattleRoundEnd2[]				= "images/battle/btn_turn_end_on.png";
const char s_pathBattleBg[]						= "images/battle/game_playing_bg2.png";
const char s_pathBattleYourTurn[]				= "images/battle/battle_your_turn.png";
const char s_pathBattleNormal_1_1[]				= "images/battle/battle_normal_1_1.png";
const char s_pathBattleNormal_1_2[]				= "images/battle/battle_normal_1_2.png";
const char s_pathBattleNormal_1_3[]				= "images/battle/battle_normal_1_3.png";
const char s_pathBattleNumBg[]					= "images/battle/battle_num_bg.png";

void BattleRes::load(void)
{
	loadCount = 0;
	totalCount = 0;
	ADD_IMAGE_ASYNC(s_pathBattleVs, totalCount);
	ADD_IMAGE_ASYNC(s_pathBattleRoundEnd1, totalCount);
	ADD_IMAGE_ASYNC(s_pathBattleRoundEnd2, totalCount);
	ADD_IMAGE_ASYNC(s_pathBattleBg, totalCount);
	ADD_IMAGE_ASYNC(s_pathBattleYourTurn, totalCount);
	ADD_IMAGE_ASYNC(s_pathBattleNormal_1_1, totalCount);
	ADD_IMAGE_ASYNC(s_pathBattleNormal_1_2, totalCount);
	ADD_IMAGE_ASYNC(s_pathBattleNormal_1_3, totalCount);
	ADD_IMAGE_ASYNC(s_pathBattleNumBg, totalCount);
}

void BattleRes::loadCallback(Object *obj)
{
	loadCount++;
}

float BattleRes::loadProgress()
{
	return loadCount *1.0f / totalCount;
}

