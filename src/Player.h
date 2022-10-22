#ifndef HOVERGAME_PLAYER_H
#define HOVERGAME_PLAYER_H

#include <Includes.h>
#include "resources/Paths.h"

class Player {
public:
	Vec2 pos;

	RayTexture texture = { Paths::Image("player.png") };

    void Update();
	void Render();
private:
    float angle = 0.0f;
    float scale = 100.0f;
};


#endif //HOVERGAME_PLAYER_H
