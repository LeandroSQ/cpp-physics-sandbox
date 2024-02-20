#include "interation_handler.hpp"
#include "../settings.hpp"
#include "../interface/palette.hpp"

InterationHandler::InterationHandler(Quadtree &quadtree) : quadtree(quadtree) { }

void InterationHandler::spawn(raylib::Vector2 mouse, uint8_t substeps) {
    const int count = 4;
    const float mass = 100.0f;
    const float temperature = 0.0f;
    raylib::Vector2 position;
    raylib::Vector2 acceleration;

    for (int i = 0; i < count; i++) {
        if (isSpawning) {
            position = raylib::Vector2(WIDTH / 2.0f, CENTER_CIRCLE_RADIUS / 2.0f);
            acceleration = (raylib::Vector2(cosf(angle), sinf(angle) * 0.5f) * 500.0f * mass) * (float)substeps;
        } else {
            position = mouse;
        }

        position.x += i * (OBJECT_RADIUS * 2.5f) - (count - 1) * (OBJECT_RADIUS * 1.5f);

        raylib::Color color;
        if  (ENABLE_RAINBOW_COLORS) {
            color = raylib::Color(raylib::Vector3((quadtree.size() % 361 / 360.0f) * 360.0f, 1.0f, 1.0f));
        } else {
            color = PALETTE_BLUE;
        }

        auto object = std::make_shared<Object>(position, acceleration, mass, OBJECT_RADIUS, temperature, color);
        quadtree.add(object);
    }
}

void InterationHandler::update(raylib::Vector2 mouse, uint8_t substeps, float deltaTime, bool consumeInput) {
    if (isSpawning) {
        timer += deltaTime;
        if (timer >= SPAWN_INTERVAL) {
            timer -= SPAWN_INTERVAL;
            if  (ENABLE_SPAWN_SPREAD)
                angle = Lerp(angle, sinf(GetTime()) * 0.25f * PI + 0.5f * PI, deltaTime * 10);
            else
                angle = Lerp(angle, 0.5f * PI, deltaTime * 10);

            spawn(raylib::Vector2::Zero(), substeps);

            if (quadtree.size() >= SPAWN_COUNT) {
                isSpawning = false;
                timer = 0;
            }
        }

        return;
    }

    if (!consumeInput) return;

    // Manual spawn
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {// Click
        spawn(mouse, substeps);
        timer = 0;
    } else if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {// Hold
        timer += deltaTime;
        if (timer >= MANUAL_SPAWN_INTERVAL) {
            timer -= MANUAL_SPAWN_INTERVAL;
            spawn(mouse, substeps);
        }
    }

    if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {// Dragging
        if (draggingObjects.size() == 0) {
             float OBJECT_RADIUS = 50.0f;
            auto objects = quadtree.query(raylib::Rectangle{ mouse.x - OBJECT_RADIUS, mouse.y - OBJECT_RADIUS, OBJECT_RADIUS * 2.0f, OBJECT_RADIUS * 2.0f });

            for (auto object : objects) {
                draggingObjects.push_back(object);
            }
        }

        for (auto object : draggingObjects) {
            object->acceleration = (mouse - object->position).Normalize() * DRAGGING_ACCELERATION;
        }
    } else {
        draggingObjects.clear();
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_MIDDLE)) {// Explosion
        auto objects = quadtree.query(raylib::Rectangle{ mouse.x - EXPLOSION_RADIUS, mouse.y - EXPLOSION_RADIUS, EXPLOSION_RADIUS * 2.0f, EXPLOSION_RADIUS * 2.0f });
        for (auto object : objects) {
            auto direction = object->position - mouse;
            auto distance = direction.Length();
            if (distance < EXPLOSION_RADIUS) {
                auto normal = direction / distance;
                object->acceleration += normal * EXPLOSION_FORCE;
            }
        }
    }
}