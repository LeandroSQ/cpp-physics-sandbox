#include "ship.hpp"
#include "../utils.hpp"

void Ship::updatePhysics() {
    const auto deltaTime = GetFrameTime();

    // Movement
    velocity += acceleration * deltaTime;
    position += velocity * deltaTime;

    acceleration = raylib::Vector2::Zero(); // Reset acceleration
    velocity *= SHIP_DAMPING;

    // Angle
    angle += angularVelocity * deltaTime;
    angularVelocity *= SHIP_ANGULAR_DAMPING;
    if (angle > 2 * PI) angle -= 2 * PI;
    if (angle < 0) angle += 2 * PI;

    // Reset angular velocity if it's too small
    if (fabs(angularVelocity) < 0.1f) angularVelocity = 0;
}

void Ship::updateInput() {
    // Rotation
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) angularVelocity += SHIP_ANGULAR_ACCELERATION;
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) angularVelocity -= SHIP_ANGULAR_ACCELERATION;

    // Translation
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
        acceleration.x += cosf(angle) * SHIP_ACCELERATION;
        acceleration.y += sinf(angle) * SHIP_ACCELERATION;
    }
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
        acceleration.x -= cosf(angle) * SHIP_ACCELERATION;
        acceleration.y -= sinf(angle) * SHIP_ACCELERATION;
    }
}

void Ship::wrapAroundScreen() {
     if (position.x > WIDTH) {
        position.x = 0;
    } else if (position.x < 0) {
        position.x = WIDTH;
    }
    if (position.y > HEIGHT) {
        position.y = 0;
    } else if (position.y < 0) {
        position.y = HEIGHT;
    }
}

void Ship::setup() {
    position = raylib::Vector2(WIDTH / 2.0f, HEIGHT / 2.0f);
    velocity = raylib::Vector2::Zero();
    acceleration = raylib::Vector2::Zero();
    angle = (360.0f - 90.0f) * DEG2RAD;
    angularVelocity = 0.0f;
}

void Ship::update() {
    updatePhysics();
    updateInput();
    wrapAroundScreen();
}

void Ship::render() {
    // Render triangle with the pointy end facing the direction of the ship
    const float size = 10.0f;
    raylib::Vector2 p1 = position + raylib::Vector2(size, 0);
    raylib::Vector2 p2 = position + raylib::Vector2(-size, -size);
    raylib::Vector2 p3 = position + raylib::Vector2(-size, size);

    /* DrawCircleV(position, size, WHITE);
    DrawLineV(position, position + raylib::Vector2(cosf(angle) * size, sinf(angle) * size), RED); */

    DrawTriangleLines(
        rotateAround(p1, position, angle),
        rotateAround(p2, position, angle),
        rotateAround(p3, position, angle),
        WHITE
    );

}
