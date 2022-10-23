//
// Created by Tobiathan on 10/23/22.
//

#ifndef HOVERGAME_RAGDOLL_H
#define HOVERGAME_RAGDOLL_H


#include "Includes.h"
#include "resources/Paths.h"
#include "RigidBody.h"
#include "Random.h"

struct RagDollSegment {
    shared_ptr<RayTexture> texture;
    Vec2 pivotNormBody;
    Vec2 pivotNorm;
    float angle;
    float angularVel;
};

class RagDoll : RigidBody {
public:
    RagDoll (Vec2 pos, Vec2 vel, float angle, float angularVel, float scale) : RigidBody(), scale(scale) {
        this->pos = pos;
        this->vel = vel;
        this->angle = angle;
        this->angularVel = angularVel;

        bounceConstant = -0.5f;

        bodyTexture = { Paths::Image("body") };
        dimens = Vec2((float) bodyTexture.width, (float) bodyTexture.height) * scale;

        RagDollSegment head = {
                std::make_shared<RayTexture>(Paths::Image("head")),
                { 0.5f, 0.0f },
                { 0.5f, 1.0f },
                0.0f, Random::RandPN(30)
        };

        RagDollSegment leftArm = {
                std::make_shared<RayTexture>(Paths::Image("arm")),
                { 0.0f, 0.2f },
                { 0.0f, 0.5f },
                PI, Random::RandPN(30)
        };

        RagDollSegment rightArm = {
                std::make_shared<RayTexture>(Paths::Image("arm")),
                { 1.0f, 0.2f },
                { 0.0f, 0.5f },
                0.0f, Random::RandPN(30)
        };

        RagDollSegment leftLeg = {
                std::make_shared<RayTexture>(Paths::Image("leg")),
                { 0.0f, 1.0f },
                { 0.0f, 0.5f },
                PI / 2.0f, Random::RandPN(30)
        };

        RagDollSegment rightLeg = {
                std::make_shared<RayTexture>(Paths::Image("leg")),
                { 1.0f, 1.0f },
                { 0.0f, 0.5f },
                PI / 2.0f, Random::RandPN(30)
        };

        segments = { head, leftArm, rightArm, leftLeg, rightLeg };
    }

    void Render();
    void Update(float deltaTime, const vector<TerrainSegment>& terrainSegments);

private:
    float scale;


    RayTexture bodyTexture;
    vector<RagDollSegment> segments;
};


#endif //HOVERGAME_RAGDOLL_H
