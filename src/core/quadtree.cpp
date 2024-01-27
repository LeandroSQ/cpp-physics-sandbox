#include "constants.hpp"

#ifndef QUADTREE_DUMMY

#include "quadtree.hpp"

Quadtree::Quadtree(raylib::Rectangle bounds, uint8_t depth) : bounds(bounds), depth(depth) {
    color = raylib::Color {
        (uint8_t) GetRandomValue(128, 255),
        (uint8_t) GetRandomValue(128, 255),
        (uint8_t) GetRandomValue(128, 255),
        255
    };
}

bool Quadtree::isLeaf() {
	return quadrants.empty();
}

void Quadtree::split() {
	auto x = bounds.x, y = bounds.y, w = bounds.width / 2, h = bounds.height / 2;

	// North-West
	quadrants.push_back(std::make_shared<Quadtree>(raylib::Rectangle{ x, y, w, h }, depth + 1));
	// North-East
	quadrants.push_back(std::make_shared<Quadtree>(raylib::Rectangle{ x + w, y, w, h }, depth + 1));
	// South-East
	quadrants.push_back(std::make_shared<Quadtree>(raylib::Rectangle{ x + w, y + h, w, h }, depth + 1));
	// South-West
	quadrants.push_back(std::make_shared<Quadtree>(raylib::Rectangle{ x, y + h, w, h }, depth + 1));

	// Move children to quadrants
	for (auto node : children) {
		// bool found = false;
		for (auto quadrant : quadrants) {
			if (quadrant->add(node)) {
				// found = true;
				break;
			}
		}

		// if (!found) TraceLog(LOG_WARNING, "Failed to add object to quadrant");
	}

	children.clear();
}

void Quadtree::merge() {
	if (isLeaf()) return;

	for (auto quadrant : quadrants) {
		// Merge children
		quadrant->merge();

		// Move children to parent
		for (auto node : quadrant->children) {
			children.push_back(node);
		}
		quadrant->children.clear();
	}

	// Clear quadrants, make it a leaf
	quadrants.clear();
}

bool Quadtree::add(std::shared_ptr<Object> object) {
	if (!bounds.CheckCollision(object->getBounds())) return false;

	if (isLeaf()) {
		if (children.size() < MAX_OBJECTS || depth >= MAX_SUBDIVISIONS) {
			object->quadrant = this;
			children.push_back(object);
			return true;
		}

		split();
	}

	if (!isLeaf()) {
		for (auto quadrant : quadrants) {
			if (quadrant->add(object)) return true;
		}
	}

	return false;
}

bool Quadtree::remove(std::shared_ptr<Object> object) {
	if (isLeaf()) {
		for (auto it = children.begin(); it != children.end(); it++) {
			if (*it == object) {
				object->quadrant = nullptr;
				children.erase(it);
			    return true;
			}
		}

		return false;
	}

	for (auto quadrant : quadrants) {
		if (quadrant->remove(object)) {
			if (size() < MAX_OBJECTS) merge();

			return true;
		}
	}

	return false;
}

void Quadtree::update(std::shared_ptr<Object> object) {
	if (object->quadrant != nullptr && object->quadrant->isLeaf() && object->quadrant->bounds.CheckCollision(object->getBounds())) return;

	if (remove(object)) {
		if (!add(object)) {
			TraceLog(LOG_WARNING, "Failed to add object to quadtree");
		}
	} else {
		TraceLog(LOG_WARNING, "Failed to remove object from quadtree");
	}
}

std::vector<std::shared_ptr<Object>> Quadtree::getAll() {
	std::vector<std::shared_ptr<Object>> objects;

	if (isLeaf()) {
		for (auto node : children) {
			objects.push_back(node);
		}
	} else {
		for (auto quadrant : quadrants) {
			auto quadrantObjects = quadrant->getAll();
			objects.insert(objects.end(), quadrantObjects.begin(), quadrantObjects.end());
		}
	}

	return objects;
}

unsigned int Quadtree::size() {
	unsigned int size = 0;

	if (isLeaf()) {
		size = children.size();
	} else {
		for (auto quadrant : quadrants) {
			size += quadrant->size();
		}
	}

	return size;
}

unsigned int Quadtree::getSubdivisions() {
	unsigned int subdivisions = 0;

	if (isLeaf()) {
		subdivisions = 1;
	} else {
		for (auto quadrant : quadrants) {
			subdivisions += quadrant->getSubdivisions();
		}
	}

	return subdivisions;
}

std::vector<std::shared_ptr<Object>> Quadtree::query(raylib::Rectangle range) {
	std::vector<std::shared_ptr<Object>> objects;

     if (!bounds.CheckCollision(range)) {
        return objects; // No collision with the quadtree bounds
    }

	if (isLeaf()) {
        for (auto node : children) {
            if (range.CheckCollision(node->getBounds())) {
                objects.push_back(node);
            }
        }
    } else {
        for (auto quadrant : quadrants) {
            if (quadrant->bounds.CheckCollision(range)) {
                auto quadrantObjects = quadrant->query(range);
                objects.insert(objects.end(), quadrantObjects.begin(), quadrantObjects.end());
            }
        }
    }


	return objects;
}

void Quadtree::render() {
	DrawRectangleLinesEx(bounds, 1, color);

	if (isLeaf()) {
		for (auto node : children) {
			DrawCircleV(node->position, node->radius, color);
		}
	} else {
		for (auto quadrant : quadrants) {
			quadrant->render();
		}
	}
}

void Quadtree::clear() {
    children.clear();
    quadrants.clear();
}

std::vector<std::shared_ptr<Quadtree>> Quadtree::getQuadrants() {
    return quadrants;
}
#endif
