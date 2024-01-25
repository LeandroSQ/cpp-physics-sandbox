#include "solver.hpp"

const float AIR_TEMPERATURE = 4000.0f;
const float GROUND_TEMPERATURE = 10000.0f;

Solver::Solver(Quadtree &quadtree) : quadtree(quadtree) {
	centerCirclePosition = raylib::Vector2{ WIDTH / 2.0f, HEIGHT / 2.0f };
}

void Solver::applyGravity(std::shared_ptr<Object> object, float deltaTime) {
    object->acceleration += gravity * object->mass;


	if constexpr (!ENABLE_TEMPERATURE) return;

    // The more heat, the more upwards acceleration
    auto temp = (object->temperature - AIR_TEMPERATURE) / (GROUND_TEMPERATURE - AIR_TEMPERATURE);
    object->acceleration -= gravity * object->mass * powf(temp, 2.0f) * temperatureFloatingForce;

}

void Solver::applyConstraints(std::shared_ptr<Object> object, float deltaTime) {
#ifdef CENTER_CIRCLE
	const raylib::Vector2 direction = object->position - centerCirclePosition;
	float distance = direction.Length();

    if constexpr (ENABLE_TEMPERATURE) {
        // Transfer heat from the border of the circle
        /* if (distance >= CENTER_CIRCLE_RADIUS - object->radius * 4) {
            auto deltaTemperature = (object->temperature - GROUND_TEMPERATURE) * temperatureTransferGroundMultiplier;
            object->temperature -= deltaTemperature;
        } */

        // Transfer heat from the bottom of the circle
        auto distanceFromBottom = object->position - raylib::Vector2{ centerCirclePosition.x, centerCirclePosition.y + CENTER_CIRCLE_RADIUS };
        if (distanceFromBottom.Length() <= object->radius * 14) {
            auto deltaTemperature = (object->temperature - GROUND_TEMPERATURE) * temperatureTransferGroundMultiplier;
            object->temperature -= deltaTemperature;
        }
    }

	if (distance > CENTER_CIRCLE_RADIUS - object->radius) {
		auto normal = direction / distance;
		object->position = centerCirclePosition + normal * (CENTER_CIRCLE_RADIUS - object->radius);
	}
#else
    const raylib::Vector2 screen((float) GetScreenWidth(), (float) GetScreenHeight());
	if (object->position.x - object->radius < 0.0f) {
		object->position.x = object->radius;
	} else if (object->position.x + object->radius > screen.x) {
		object->position.x = screen.x - object->radius;
	}

	if (object->position.y - object->radius < 0.0f) {
		object->position.y = object->radius;
	} else if (object->position.y + object->radius > screen.y) {
		object->position.y = screen.y - object->radius;
	}

    if constexpr (!ENABLE_TEMPERATURE) return;

	if (object->position.y + object->radius >= screen.y - object->radius * 4) {
		// Transfer temperature from the ground
		auto deltaTemperature = (object->temperature - GROUND_TEMPERATURE) * temperatureTransferGroundMultiplier;
		object->temperature -= deltaTemperature;
	}
#endif
}

void Solver::solveCollisions(std::shared_ptr<Object> object) {
	auto padding = 3.0f * object->radius;
	auto others = quadtree.query(raylib::Rectangle{ object->position.x - padding, object->position.y - padding, padding * 2.0f, padding * 2.0f });
	bool collision = false;

	// Use private scope and parallelize for loop
	for (auto other : others) {
        iterationSum++;
		if (other == object) continue;

		const raylib::Vector2 direction = object->position - other->position;
		float distance = direction.LengthSqr();
		if (distance < powf(object->radius + other->radius, 2.0f)) {
            distance = sqrtf(distance);
			collision = true;
			auto normal = direction / distance;
			auto delta = (object->radius + other->radius) - distance;
			object->position += normal * 0.5f * delta;
			other->position -= normal * 0.5f * delta;

			// Transfer temperature between objects slightly 10% of the time
			if constexpr (ENABLE_TEMPERATURE) {
				auto deltaTemperature = (object->temperature - other->temperature) * temperatureTransferContactMultiplier;
				object->temperature -= deltaTemperature;
				other->temperature += deltaTemperature;
			}
		}
	}

	if (!collision && ENABLE_TEMPERATURE) {
		// Cool temperature transfering to air
		auto deltaTemperature = (object->temperature - AIR_TEMPERATURE) * temperatureTransferAirMultiplier;
		object->temperature -= deltaTemperature;
	}
}

uint32_t Solver::getAverageIterations() {
	return iterationSum / iterationCount;
}

void Solver::solve(float deltaTime) {
	if (iterationCount >= 100) {
		iterationCount = 0;
		iterationSum = 0;
	}

	float dt = deltaTime / float(substeps);
	for (uint8_t i = 0; i < substeps; i++) {
		for (auto object : quadtree.getAll()) {
			applyGravity(object, dt);
			solveCollisions(object);
			applyConstraints(object, dt);

			object->update(dt);
			quadtree.update(object);
		}
	}

	iterationCount++;
}