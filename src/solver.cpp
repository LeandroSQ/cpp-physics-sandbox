#include "solver.hpp"

const float TRANSFER_CONTACT_TEMPERATURE = 0.025f;
const float AIR_TEMPERATURE = 4000.0f;
const float TRANSFER_AIR_TEMPERATURE = 0.01f;
const float GROUND_TEMPERATURE = 10000.0f;
const float TRANSFER_GROUND_TEMPERATURE = 0.3f;
const float UPWARDS_TEMPERATURE = 5.0f;

void updatePosition(std::shared_ptr<Object> object, float deltaTime) {
	if (object == nullptr) return;

	object->update(deltaTime);
}

void applyGravity(std::shared_ptr<Object> object, float deltaTime) {
	if (object == nullptr) return;

	object->acceleration.y += 9.80f * 100.0f;

// The more temperature, make it float upwards
#ifdef CENTER_CIRCLE
    // Create a pull force to the center of the circle, the more temperature, the more pull
    const raylib::Vector2 center = raylib::Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
    const float radius = 400.0f;

    const raylib::Vector2 direction = object->position - center;
    float distance = direction.Length();
    if (distance > radius - object->radius) {
        auto normal = direction / distance;
        object->acceleration -= normal * (object->temperature * UPWARDS_TEMPERATURE) * 100;
    }
#else
	if (object->temperature > 5000.0f) object->acceleration.y -= object->temperature * UPWARDS_TEMPERATURE;
#endif
}

void applyConstraints(std::shared_ptr<Object> object, float deltaTime) {
	if (object == nullptr) return;

#ifdef CENTER_CIRCLE
	const raylib::Vector2 center = raylib::Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
	const float radius = 400.0f;

	const raylib::Vector2 direction = object->position - center;
	float distance = direction.Length();

         // Transfer heat from the border of the circle
    if (distance >= radius - object->radius * 4) {
        auto deltaTemperature = (object->temperature - AIR_TEMPERATURE) * TRANSFER_AIR_TEMPERATURE;
        object->temperature = GROUND_TEMPERATURE;
    }

	if (distance > radius - object->radius) {
		auto normal = direction / distance;
		object->position = center + normal * (radius - object->radius);
	}


#else
	const raylib::Rectangle bounds(0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight());
	if (object->position.x - object->radius < bounds.x) {
		object->position.x = bounds.x + object->radius;
	} else if (object->position.x + object->radius > bounds.x + bounds.width) {
		object->position.x = bounds.x + bounds.width - object->radius;
	}

	if (object->position.y - object->radius < bounds.y) {
		object->position.y = bounds.y + object->radius;
	} else if (object->position.y + object->radius > bounds.y + bounds.height) {
		object->position.y = bounds.y + bounds.height - object->radius;
	}

	if (object->position.y - object->radius >= bounds.y + bounds.height - object->radius * 2 && rand() % 100 <= 50) {
		// Transfer temperature from the ground
		auto deltaTemperature = (object->temperature - GROUND_TEMPERATURE) * TRANSFER_GROUND_TEMPERATURE;
		object->temperature -= deltaTemperature;
	}
#endif
}

uint32_t solveCollisions(Quadtree &quadtree, std::shared_ptr<Object> object) {
	if (object == nullptr) return 0;

	auto p = 3.0f * object->radius;
	auto others = quadtree.query(raylib::Rectangle{ object->position.x - p, object->position.y - p, p * 2.0f, p * 2.0f });
	bool collision = false;

	// Use private scope and parallelize for loop
	for (auto other : others) {
		if (other == object) continue;

		const raylib::Vector2 direction = object->position - other->position;
		const float distance = direction.Length();
		if (distance < object->radius + other->radius) {
			collision = true;
			auto normal = direction / distance;
			auto delta = (object->radius + other->radius) - distance;
			object->position += normal * 0.5f * delta;
			other->position -= normal * 0.5f * delta;

			// Transfer temperature between objects slightly 10% of the time
			auto deltaTemperature = (object->temperature - other->temperature) * TRANSFER_CONTACT_TEMPERATURE;
			object->temperature -= deltaTemperature;
			other->temperature += deltaTemperature;
		}
	}

	if (!collision) {
		// Cool temperature transfering to air
		auto deltaTemperature = (object->temperature - AIR_TEMPERATURE) * TRANSFER_AIR_TEMPERATURE;
		object->temperature -= deltaTemperature;
	}

	return others.size();
}

void Solver::solve(Quadtree &quadtree, float deltaTime) {
	uint32_t iterations = 0;

#ifdef MULTI_THREADING
	std::vector<std::thread> threads;
	uint32_t maxThreads = std::thread::hardware_concurrency();
	auto objects = quadtree.getAll();
	uint32_t objectsPerThread = objects.size() / maxThreads;
	uint32_t remainder = objects.size() % maxThreads;

	for (uint32_t i = 0; i < maxThreads; i++) {
		uint32_t start = i * objectsPerThread;
		uint32_t end = start + objectsPerThread;
		if (i == maxThreads - 1) end += remainder;

		threads.push_back(std::thread([&, start, end]() {
			for (uint32_t j = start; j < end; j++) {
				auto object = objects[j];
				applyGravity(object, deltaTime);
				solveCollisions(quadtree, object);
				applyConstraints(object, deltaTime);
				updatePosition(object, deltaTime);
				quadtree.update(object);
			}
		}));
	}

	for (auto &thread : threads) {
		thread.join();
	}
#else
	for (auto object : quadtree.getAll()) {
		applyGravity(object, deltaTime);
		iterations += solveCollisions(quadtree, object);
		applyConstraints(object, deltaTime);
		updatePosition(object, deltaTime);
		quadtree.update(object);
	}
#endif

	if (iterationCount >= 1000) {
		iterationCount = 0;
		iterationSum = 0;
	}

	iterationSum += iterations;
	iterationCount++;
}