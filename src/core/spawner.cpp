#include "spawner.hpp"

Spawner::Spawner(Quadtree &quadtree) : quadtree(quadtree) { }

void Spawner::spawn() {
    for (int i = 0; i < 4; i++) {
        raylib::Vector2 position;
        raylib::Vector2 acceleration;
        if (isSpawning) {
            position = raylib::Vector2(WIDTH / 2.0f, CENTER_CIRCLE_RADIUS / 2.0f);
            acceleration = raylib::Vector2(500.0f * cos(angle), 0.0f);
        } else {
            position = raylib::Vector2(GetMouseX(), GetMouseY());
        }

        const float mass = 100.0f;
        const float radius = 10.0f;
        const float temperature = 0.0f;

        position.x += i * radius * 2.0f;

        raylib::Color color = PALETTE_BLUE;//raylib::Color(raylib::Vector3((quadtree.size() % 361 / 360.0f) * 360.0f, 1.0f, 0.75f));

        auto object = std::make_shared<Object>(
            position,
            acceleration,
            mass,
            radius,
            temperature,
            color
        );
        quadtree.add(object);
    }
}

void Spawner::update(float deltaTime, bool consumeInput) {
    if (isSpawning) {
        angle = sin(GetTime()) * 0.25f * PI + 0.5f * PI;
        timer += deltaTime;
        if (timer >= SPAWN_INTERVAL) {
            timer -= SPAWN_INTERVAL;
            spawn();

            if (quadtree.size() >= SPAWN_COUNT) {
                isSpawning = false;
                timer = 0;
            }
        }

        return;
    }

    if (!consumeInput) return;

    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        spawn();
        timer = 0;
    } else if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        timer += deltaTime;
        if (timer >= MANUAL_SPAWN_INTERVAL) {
            timer -= MANUAL_SPAWN_INTERVAL;
            spawn();
        }
    }

    if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {// Dragging

        auto position = raylib::Vector2(GetMouseX(), GetMouseY());

        if (draggingObjects.size() == 0) {
            constexpr float radius = 50.0f;
            auto objects = quadtree.query(raylib::Rectangle{ position.x - radius, position.y - radius, radius * 2.0f, radius * 2.0f });

            for (auto object : objects) {
                draggingObjects.push_back(object);
            }
        }

        for (auto object : draggingObjects) {
            object->acceleration = (position - object->position).Normalize() * 35000.0f;
        }

        // Explosion
        /* */
    } else {
        draggingObjects.clear();
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_MIDDLE)) {// Explosions
        const float force = 1500000.0f;
        constexpr float radius = 50.0f;
        auto position = raylib::Vector2(GetMouseX(), GetMouseY());
        auto objects = quadtree.query(raylib::Rectangle{ position.x - radius, position.y - radius, radius * 2.0f, radius * 2.0f });
        for (auto object : objects) {
            auto direction = object->position - position;
            auto distance = direction.Length();
            if (distance < radius) {
                auto normal = direction / distance;
                object->acceleration += normal * force;
            }
        }
    }
}