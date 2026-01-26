#include "game.hpp"
#include <cstdint>
#include <memory>
#include <random>
gore::g_engine_2d g_eng("Box Dodge 3", 1280, 720, PRIMITIVE_COMPONENT | IMAGE_COMPONENT | FONT_COMPONENT, 
gore::LogType::CONSOLE
);

gore::drawpass dr(1280, 720, GL_COLOR_ATTACHMENT0);

Entity player(500.0f, 350.0f, 32.0f, 32.0f);
std::vector<std::shared_ptr<Entity>> enemies;
uint32_t globalWidth, globalHeight;
uint64_t playerScore = 0;
gore::font roboto;
enum game_mode { DEATH, GAMEPLAY };
game_mode mode = GAMEPLAY;

void renderFunction () {
    dr.clear();
    dr.bind();
    switch (mode) {
        case GAMEPLAY:
            g_eng.prim_r->setColor({ 0.0f, 1.0f, 0.0f,1.0f});
            g_eng.prim_r->drawQuad(player.pos, player.dimen.x, player.dimen.y);
            g_eng.prim_r->setColor({1.0f, 0.0f, 0.0f, 1.0f});
            for (auto& i : enemies) {
                g_eng.prim_r->drawQuad(i->pos, i->dimen.x, i->dimen.y);
            }
            g_eng.font_renderer->setColor({1.0f, 1.0f, 1.0f, 1.0f});
            g_eng.font_renderer->drawText("Score: " + std::to_string(playerScore), &roboto, 0.0f, 64.0f, 48, g_eng.getDPI());
        break;
        case DEATH:
            g_eng.prim_r->setColor({ 0.0f, 1.0f, 0.0f,1.0f});
            g_eng.prim_r->drawQuad(player.pos, player.dimen.x, player.dimen.y);
            g_eng.prim_r->setColor({1.0f, 0.0f, 0.0f, 1.0f});
            for (auto& i : enemies) {
                g_eng.prim_r->drawQuad(i->pos, i->dimen.x, i->dimen.y);
            }
            g_eng.font_renderer->setColor({1.0f, 1.0f, 1.0f, 1.0f});
            g_eng.font_renderer->drawText("Score: " + std::to_string(playerScore), &roboto, 100.0f, 350.0f, 48, g_eng.getDPI());
            g_eng.font_renderer->drawText("Press space to try again!", &roboto, 100.0f, 420.0f, 48, g_eng.getDPI());
        break;
    }
    dr.unbind();
    g_eng.img_r->drawTexture(dr.getTexture(), { 0.0f, 0.0f }, {(float)globalWidth, (float)globalHeight}, {0.0f, 1.0f, 1.0f, -1.0f});
}

float randomFloat(float min, float max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}
int randomInt (int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(min, max);
    return dis(gen);
}

int vpW, vpH;

void resizeFunction (uint32_t width, uint32_t height) {
    // dr.resize(width, height);
    globalWidth = width;
    globalHeight = height;
}

void playerHit () {
    mode = DEATH;
}
uint64_t enemy_spawn_count = 0;
double enemy_spawn_max = 1.0f;
void playerDeath () {
    player.pos.x = 500.0f;
    player.pos.y = 350.0f;
    enemies.clear();
    playerScore = 0;
    mode = GAMEPLAY;
    enemy_spawn_max = 1.0f;
    enemy_spawn_count = 0;
}

// TODO
// fix resolution by forcing the renderers into a smaller resolution??
// or continue trying to get framebuffer to render correctly?

int main () {
    g_eng.setRenderFunction(renderFunction);
    g_eng.setWindowResize(resizeFunction);
    g_eng.toggleMaintainViewport();
    g_eng.toggleRendererViewportResizing();
    roboto = gore::fontloader::loadFont("RobotoCondensed-Regular.ttf", 0, 1321);
    double player_move_delay = 0.0f;
    double player_score_delay = 0.0f;
    double player_score_speedup = 0.001f;
    double player_score_delay_s = 0.0f;
    double enemy_spawn_delay = 0.0f;
    double enemy_move_delay = 0.0f;
    double fullscreen_toggle_delay = 0.0f;
    bool cont = true;
    while (g_eng.updateWindow() && cont) {
        double del = g_eng.getDelta();
		g_eng.updateInputState();
        player_move_delay += del;
        enemy_spawn_delay += del;
        enemy_move_delay += del;
        player_score_delay += del;
        player_score_delay_s += del;
        fullscreen_toggle_delay += del;
        switch (mode) {
            case GAMEPLAY:
                if ( player_move_delay >= 0.001f ) {
                    if ( g_eng.getKeyDown(g_a) && player.pos.x >= 0.0f) {
                        player.pos.x -= 1.0f;
                        player_move_delay = 0;
                    } else if ( g_eng.getKeyDown(g_d) && player.pos.x + player.dimen.x < 1024) {
                        player.pos.x += 1.0f;
                        player_move_delay = 0;
                    }
                    if ( g_eng.getKeyDown(g_w) && player.pos.y >= 0.0f) {
                        player.pos.y -= 1.0f;
                        player_move_delay = 0;
                    } else if ( g_eng.getKeyDown(g_s) && player.pos.y + player.dimen.y < 768) {
                        player.pos.y += 1.0f;
                        player_move_delay = 0;
                    }
                }
                if (fullscreen_toggle_delay > 0.2f) {
                    if ( g_eng.getKeyDown(g_f)) {
                        g_eng.toggleFullscreen();
                        fullscreen_toggle_delay = 0.0;
                    } else if ( g_eng.getKeyDown(g_Escape) ) {
                        cont = false;
                    }
                } 
                if (player_score_delay >= player_score_speedup) {
                    playerScore += 1;
                    player_score_delay = 0;
                }
                if (player_score_delay_s > 2.0f) {
                    player_score_speedup -= 0.0001f;
                    if (player_score_speedup <= 0) {
                        player_score_speedup = 0.0001f;
                    }
                }
                if (enemy_spawn_delay >= enemy_spawn_max) {
                    float randx = randomFloat(0.0f, 800.0f);
                    float randy = randomFloat(0.0f, 768);
                    std::shared_ptr<Entity> enem;
                    int max = 0;
                    if (enemy_spawn_count > 10) {
                        max = 1;
                    } else if (enemy_spawn_count > 20) {
                        max = 2;
                    } else if (enemy_spawn_count > 30) {
                        max = 3;
                    }
                    int randtype = randomInt(0, 1);
                    switch (randtype) {
                        case 0:
                            enem = std::make_shared<VerticalDropEnemy>(randx, 0.0f, 32.0f, 32.0f);
                        break;
                        case 1:
                            enem = std::make_shared<HorizontalDropEnemy>(0.0f, randy, 32.0f, 32.0f);
                        break;
                        case 2:
                            enem = std::make_shared<VerticalDropEnemy>(player.pos.x, 0.0f, 32.0f, 32.0f);
                        break;
                        case 3:
                            enem = std::make_shared<HorizontalDropEnemy>(0.0f, player.pos.y, 32.0f, 32.0f);
                        break;
                    }
                    enemies.push_back(enem);
                    enemy_spawn_delay = 0.0f;
                    enemy_spawn_count++;
                    if (enemy_spawn_count % 3 == 0) {
                        enemy_spawn_max -= 0.05f;
                        if (enemy_spawn_max <= 0) {
                            enemy_spawn_max = 0.05f;
                        }
                    }
                }
                if (enemy_move_delay >= 0.001f) {
                    for (size_t i = 0; i < enemies.size();) {
                        enemies[i]->update();
                        if (enemies[i]->collision(player)) {
                            playerHit();
                            break;
                        }
                        if (enemies[i]->pos.y >= 768 + enemies[i]->dimen.y || enemies[i]->pos.x >= 800) {
                            enemies.erase(enemies.begin() + i);
                        } else {
                            i++;
                        }
                    }
                    enemy_move_delay = 0.0f;
                }
            break;
            case DEATH:
                if ( g_eng.getKeyDown(g_Space)) {
                    playerDeath();
                }
            break;
        }
       
    }
}