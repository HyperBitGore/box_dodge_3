#include "game.hpp"
#include <random>
gore::g_engine_2d g_eng("Box Dodge 3", 1024, 768, PRIMITIVE_COMPONENT | IMAGE_COMPONENT | FONT_COMPONENT, 
gore::LogType::CONSOLE
);

gore::drawpass dr(1024, 768, GL_COLOR_ATTACHMENT0);

Entity player(500.0f, 350.0f, 32.0f, 32.0f);
std::vector<Entity> enemies;
uint32_t globalWidth, globalHeight;

void renderFunction () {
    //dr.bind();
    //dr.clear();
    g_eng.prim_r->setColor({ 0.0f, 255.0f, 0.0f, 255.0f});
    g_eng.prim_r->drawQuad(player.pos, player.dimen.x, player.dimen.y);
    g_eng.prim_r->setColor({255.0f, 0.0f, 0.0f, 255.0f});
    for (auto& i : enemies) {
        g_eng.prim_r->drawQuad(i.pos, i.dimen.x, i.dimen.y);
    }
    //dr.unbind();
    //g_eng.img_r->drawTexture(dr.getTexture(), { 0.0f, 0.0f }, {(float)globalWidth, (float)globalHeight}, {0.0f, 1.0f, 1.0f, -1.0f});
}

float randomFloat(float min, float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}

int vpW, vpH;

void resizeFunction (uint32_t width, uint32_t height) {
    dr.resize(width, height);
    globalWidth = width;
    globalHeight = height;
    float targetAspect = 1024.0f / 768.0f;
    float windowAspect = (float)width / (float)height;
    int vpX = 0, vpY = 0;
    vpW = width;
    vpH = height;

    if (windowAspect > targetAspect) {
        vpW = (int)(width * targetAspect);
        vpX = (width - vpW) / 2;
    } else {
        vpH = (int)(width / targetAspect);
        vpY = (height - vpH) / 2;
    }
    glViewport(vpX, vpY, vpW, vpH);
}

int main () {
    g_eng.setRenderFunction(renderFunction);
    g_eng.setWindowResize(resizeFunction);
    g_eng.toggleMaintainViewport();
    double player_move_delay = 0.0f;
    double enemy_spawn_delay = 0.0f;
    double enemy_spawn_max = 1.0f;
    double enemy_move_delay = 0.0f;
    while (g_eng.updateWindow()) {
        double del = g_eng.getDelta();
		g_eng.updateInputState();
        player_move_delay += del;
        enemy_spawn_delay += del;
        enemy_move_delay += del;
        if ( player_move_delay >= 0.001f ) {
            if ( g_eng.getKeyDown(g_a) && player.pos.x >= 0.0f) {
                player.pos.x -= 1.0f;
                player_move_delay = 0;
            } else if ( g_eng.getKeyDown(g_d) && player.pos.x + player.dimen.x < vpW) {
                player.pos.x += 1.0f;
                player_move_delay = 0;
            }
            if ( g_eng.getKeyDown(g_w) && player.pos.y >= 0.0f) {
                player.pos.y -= 1.0f;
                player_move_delay = 0;
            } else if ( g_eng.getKeyDown(g_s) && player.pos.y + player.dimen.y < vpH) {
                player.pos.y += 1.0f;
                player_move_delay = 0;
            }
        }
        if (enemy_spawn_delay >= enemy_spawn_max) {
            float randx = randomFloat(0.0f, 800.0f);
            Entity enem(randx, 0.0f, 32.0f, 32.0f);
            enemies.push_back(enem);
            enemy_spawn_delay = 0.0f;
        }
        if (enemy_move_delay >= 0.001f) {
            for (size_t i = 0; i < enemies.size();) {
                enemies[i].pos.y += 1.0f;
                if (enemies[i].pos.y >= vpH + enemies[i].dimen.y) {
                    enemies.erase(enemies.begin() + i);
                } else {
                    i++;
                }
            }
            enemy_move_delay = 0.0f;
        }
    }
}