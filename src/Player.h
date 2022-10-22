#ifndef HOVERGAME_PLAYER_H
#define HOVERGAME_PLAYER_H

#include <Includes.h>
#include "resources/Paths.h"

class Player {
public:
	Vec2 pos;
    float angle = 0.0f;

    explicit Player(Vec2 pos) : pos(pos) {}

    void Update(float deltaTime);
	void Render();
private:
	RayTexture texture = { Paths::Image("HoverCraft") };
    float scale = 25.0f;
};


#endif //HOVERGAME_PLAYER_H
