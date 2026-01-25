#include "../include/g_engine/g_engine_2d.hpp"


class Entity {
private:
public:
    gore::vec2 pos;
    gore::vec2 dimen;
    Entity () {
        pos = { 0.0f, 0.0f };
        dimen = { 0.0f, 0.0f };
    }
    Entity ( float x, float y, float w, float h) {
        pos = { x, y };
        dimen = { w, h };
    }
    // copy
    Entity (const Entity& e) {
        pos = e.pos;
        dimen = e.dimen;
    }
    bool collision (Entity e) {
        return (pos.x + dimen.x >= e.pos.x // right edge past E left edge
            && pos.x <= e.pos.x + e.dimen.x // left edge not outside of E right edge
            && pos.y + dimen.y >= e.pos.y // bottom edge below E top edge
            && pos.y <= e.pos.y + e.dimen.y
        );
    }
    virtual void update() {

    }
};

class VerticalDropEnemy : public Entity {
    public:
    VerticalDropEnemy() : Entity() {

    }
    VerticalDropEnemy (float x, float y, float w, float h) : Entity(x, y, w, h) {

    }
    void update () {
        pos.y += 1.0f;
    }
};

class HorizontalDropEnemy : public Entity {
    public:
    HorizontalDropEnemy() : Entity() {

    }
    HorizontalDropEnemy (float x, float y, float w, float h) : Entity(x, y, w, h) {

    }
    void update () {
        pos.x += 0.5f;
    }
};