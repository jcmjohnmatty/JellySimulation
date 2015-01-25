#ifndef __JELLY_ANIMATE_H_
#define __JELLY_ANIMATE_H_ /* __JELLY_ANIMATE_H_ */

#include "../scene.h"
#include "jelly_body.h"

/**
 * @brief Anamate a jellyfish one more step past where it currently is.
 * @param j The jellyfish to anamate.
 * @param sp The current properties of the scene.
 * @param t The change in time since the last amimation step.
 */
void
jelly_animate_step (struct jelly* j, struct scene_props* sp, float dt);

#endif /* __JELLY_ANIMATE_H_ */
