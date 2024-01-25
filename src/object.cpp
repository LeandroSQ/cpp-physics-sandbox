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

	/* auto a = acceleration.Normalize() * 20.0f;
	DrawLine(
		position.x,
		position.y,
		position.x + a.x,
		position.y + a.y,
		RED
	); */

	acceleration = raylib::Vector2::Zero();
}

raylib::Color getColorFromTemperature(float temperature) {
	// Cool colors are blue
	// Hot colors are red
	// Extreme colors are white
	// Using Kelvin scale
	// https://en.wikipedia.org/wiki/Color_temperature

	// 1000K - 4000K
	if (temperature < 4000.0f) {
		auto t = temperature / 4000.0f;
		return raylib::Color{ (uint8_t)(128.0f * t), 0, 0, 255 };
	}

    // 4000K - 7000K
    if (temperature < 7000.0f) {
        auto t = (temperature - 4000.0f) / 3000.0f;
        return raylib::Color{ (uint8_t)(128.0f + 127.0f * t), (uint8_t)(255.0f * t), 0, 255 };
    }

	// 7000K - 10000K
	if (temperature < 10000.0f) {
		auto t = (temperature - 7000.0f) / 3000.0f;
		return raylib::Color{ 255, 255, (uint8_t)(255.0f * t), 255 };
	}

	return WHITE;
}

void Object::render() {
	if constexpr (ENABLE_TEMPERATURE)
		position.DrawCircle(radius, getColorFromTemperature(temperature));
	else
		DrawCircleV(position, radius, color);
}

raylib::Rectangle Object::getBounds() {
	auto r = radius;
	return raylib::Rectangle{ position.x - r, position.y - r, r * 2, r * 2 };
}
