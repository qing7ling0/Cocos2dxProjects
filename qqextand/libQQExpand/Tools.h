#pragma once

/** @def RANDOM_0_1
 returns a random float between 0 and 1
 */
#define RANDOM_0_1() ((float)rand()/RAND_MAX)

/** @def RANDOM_0_TO
 returns a random int between 0 and _max_
 */
#define RANDOM_0_TO(_max_) ((int) (((float)rand()/RAND_MAX )*_max_))