#include "object.hpp"

Object::Object(raylib::Vector2 position, raylib::Vector2 acceleration, float mass, float radius, float temperature, raylib::Color color) {
    this->position = position;
    previousPosition = position;
    this->acceleration = acceleration;
    this->mass = mass;
    this->radius = radius;
    this->temperature = temperature;
    this->color = color;
}

Object::Object() {
    position = raylib::Vector2::Zero();
    acceleration = raylib::Vector2::Zero();
}

void Object::update(float deltaTime) {
    auto displacement = position - previousPosition;
    previousPosition = position;
    position += displacement + acceleration * deltaTime * deltaTime;

    acceleration = raylib::Vector2::Zero();
}

void Object::render() {
	DrawCircleV(position, radius, color);
}

raylib::Rectangle Object::getBounds() {
    auto r = radius;
	return raylib::Rectangle {
        position.x - r,
        position.y - r,
        r * 2,
        r * 2
    };
}
