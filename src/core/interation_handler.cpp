#include "Vector2.hpp"
#include "interation_handler.hpp"
#include "constants.hpp"
#include "palette.hpp"
#include "raylib.h"

InterationHandler::InterationHandler(Quadtree &quadtree) : quadtree(quadtree) { }

void InterationHandler::spawn(raylib::Vector2 mouse) {
    const int count = 4;
    const float mass = 100.0f;
    const float temperature = 0.0f;
    raylib::Vector2 position;
    raylib::Vector2 acceleration;

    for (int i = 0; i < count; i++) {
        if (isSpawning) {
            position = raylib::Vector2(WIDTH / 2.0f, CENTER_CIRCLE_RADIUS / 2.0f);
            acceleration = raylib::Vector2(cosf(angle), sinf(angle) * 0.5f) * 2000.0f * mass;
        } else {
            position = mouse;
        }


        position.x += i * (OBJECT_RADIUS * 2.5f) - (count - 1) * (OBJECT_RADIUS * 1.5f);

        raylib::Color color;
        if constexpr (ENABLE_RAINBOW) {
            color = raylib::Color(raylib::Vector3((quadtree.size() % 361 / 360.0f) * 360.0f, 1.0f, 1.0f));
        } else {
            color = PALETTE_BLUE;
        }

        auto object = std::make_shared<Object>(position, acceleration, mass, OBJECT_RADIUS, temperature, color);
        quadtree.add(object);
    }
}

void InterationHandler::update(raylib::Vector2 mouse, float deltaTime, bool consumeInput) {
    if (isSpawning) {
        timer += deltaTime;
        if (timer >= SPAWN_INTERVAL) {
            timer -= SPAWN_INTERVAL;
            if constexpr (ENABLE_SPAWN_SPREAD)
                angle = sinf(GetTime()) * 0.25f * PI + 0.5f * PI;
            else
                angle = 0.5f * PI;

            spawn(raylib::Vector2::Zero());

            if (quadtree.size() >= SPAWN_COUNT) {
                isSpawning = false;
                timer = 0;
            }
        }

        return;
    }

    if (!consumeInput) return;

    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        spawn(mouse);
        timer = 0;
    } else if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        timer += deltaTime;
        if (timer >= MANUAL_SPAWN_INTERVAL) {
            timer -= MANUAL_SPAWN_INTERVAL;
            spawn(mouse);
        }
    }

    if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {// Dragging
        if (draggingObjects.size() == 0) {
            constexpr float OBJECT_RADIUS = 50.0f;
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