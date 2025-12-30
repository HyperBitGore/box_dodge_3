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

};