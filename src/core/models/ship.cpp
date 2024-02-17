#include "ship.hpp"
#include "../utils.hpp"
#include "Vector2.hpp"
#include "particle.hpp"
#include "raylib.h"
#include "soundfx.hpp"

void Ship::shoot(std::list<Bullet> &bullets) {
    const auto shipDirection = raylib::Vector2(cosf(angle), sinf(angle));
    const auto spawnPosition = position + shipDirection * SHIP_SIZE * 1.5f;

    // Spawn particles
    ParticleSystem::addExplosion(spawnPosition, 5, angle - PI / 4.0f, angle + PI / 4.0f);

    // Spawn bullet
    bullets.emplace_back(
        spawnPosition,
        shipDirection * BULLET_VELOCITY + velocity
    );

    // Play sound
    SoundFX::play(
        SoundFX::shoot,
        0.05f,
        -1.0f,
        1.0f - position.x / (float)WIDTH
    );
}

void Ship::updatePhysics() {
    const auto deltaTime = GetFrameTime();

    const float maxSpeed = BULLET_VELOCITY * 0.75f;

    // Movement
    velocity += acceleration * deltaTime * 0.5f;
    if (velocity.Length() >= maxSpeed) velocity = velocity.Normalize() * maxSpeed;
    position += velocity * deltaTime;
    velocity += acceleration * deltaTime * 0.5f;
    velocity *= powf(SHIP_DAMPING, deltaTime * TARGET_FPS);
    acceleration = raylib::Vector2::Zero(); // Reset acceleration

    // Angle
    angle += angularVelocity * deltaTime;
    if (fabsf(angularVelocity) > SHIP_MAX_ANGULAR_VELOCITY) angularVelocity = SHIP_MAX_ANGULAR_VELOCITY * (angularVelocity > 0 ? 1 : -1);
    angularVelocity *= powf(SHIP_ANGULAR_DAMPING, deltaTime * TARGET_FPS);
    if (angle > 2 * PI) angle -= 2 * PI;
    if (angle < 0) angle += 2 * PI;
    if (fabs(angularVelocity) < 0.1f) angularVelocity = 0;// Prevents the ship from rotating forever
}

void Ship::updateInput(std::list<Bullet> &bullets) {
    // Rotation
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) angularVelocity += SHIP_ANGULAR_ACCELERATION;
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) angularVelocity -= SHIP_ANGULAR_ACCELERATION;

    // Translation
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
        acceleration.x += cosf(angle) * SHIP_ACCELERATION;
        acceleration.y += sinf(angle) * SHIP_ACCELERATION;
        thrust += 0.1f;
    }
    /* if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
        acceleration.x -= cosf(angle) * SHIP_ACCELERATION;
        acceleration.y -= sinf(angle) * SHIP_ACCELERATION;
    } */

    // Shooting
    if (shotCooldown >= BULLET_SHOOT_INTERVAL && IsKeyDown(KEY_SPACE)) {
        shoot(bullets);
        shotCooldown = 0;
    } else shotCooldown += GetFrameTime();
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

void Ship::updateVertices() {
    vertices = {
        rotateAround(position + raylib::Vector2(SHIP_SIZE, 0), position, angle),
        rotateAround(position + raylib::Vector2(-SHIP_SIZE, -SHIP_SIZE), position, angle),
        rotateAround(position + raylib::Vector2(-SHIP_SIZE, SHIP_SIZE), position, angle)
    };
}

void Ship::updateTrail() {
    const auto reflectedAngle = angle + PI;
    const auto shipDirection = raylib::Vector2(cosf(reflectedAngle), sinf(reflectedAngle));
    const auto shipBottom = position + shipDirection * SHIP_SIZE;

    auto baseParticleVelocity = raylib::Vector2::Zero();

    // Small bursts of particles, when the ship is not thrusting
    if (thrust <= 0.1f && getRandomValue(0, 100) < 10) baseParticleVelocity += shipDirection * 50.0f;

    ParticleSystem::addTrail(shipBottom, 10, angle, baseParticleVelocity);
}

void Ship::updateThrustSound() {
    if (!SoundFX::thrust.IsReady()) return;

    // Postion sound
    SoundFX::thrust.SetPan(1.0f - position.x / (float)WIDTH);

    // Decay thrust force
    thrust *= 0.95f;

    // Set volume based on thrust force
    float volume = std::max(thrust, 1.0f) * 0.075f;
    if (thrust <= 0.1) volume = 0.0f;
    SoundFX::thrust.SetVolume(volume);

    // Play sound
    if (SoundFX::thrust.IsPlaying()) {
        if (volume <= 0.1f) SoundFX::thrust.Pause();
    } else {
        if (volume >= 0.1f) SoundFX::thrust.Play();
        else SoundFX::thrust.Pause();
    }

}

void Ship::update(std::list<Bullet> &bullets) {
    updateInput(bullets);
    updatePhysics();
    wrapAroundScreen();
    updateVertices();
    updateTrail();
    updateThrustSound();

    // TODO: Check for collisions with asteroids
}

void Ship::render() {
    drawFilledPolygon(position, vertices.data(), vertices.size(), WHITE);
    // drawPolygonOutline(vertices.data(), vertices.size(), WHITE);
}
