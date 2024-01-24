#include "constants.hpp"

#ifdef QUADTREE_GRID
#include "quadtree.hpp"

// Although it says "Quadtree" this is but a spatial grid

// Define the grid size

int hash(int x, int y) {
    auto z = x * GRID_SIZE + y;
    if (z < 0) z = -z;
    if (z > GRID_SIZE * GRID_SIZE) z = z % (GRID_SIZE * GRID_SIZE);
    return z;
}

Quadtree::Quadtree(raylib::Rectangle bounds, uint8_t depth) : bounds(bounds), depth(depth) {
    // Initialize grid
    for (int i = 0; i < GRID_SIZE * GRID_SIZE; i++) {
        grid[i] = std::vector<std::shared_ptr<Object>>();
    }
}

bool Quadtree::isLeaf() {
	return false;
}

void Quadtree::split() {

}

void Quadtree::merge() {

}

bool Quadtree::add(std::shared_ptr<Object> object) {
    auto x = (int) (object->position.x / GRID_SIZE);
    auto y = (int) (object->position.y / GRID_SIZE);

    grid[hash(x, y)].push_back(object);
	return true;
}

bool Quadtree::remove(std::shared_ptr<Object> object) {
    auto x = (int) (object->position.x / GRID_SIZE);
    auto y = (int) (object->position.y / GRID_SIZE);

    auto &cell = grid[hash(x, y)];
    auto it = std::remove_if(cell.begin(), cell.end(), [object](std::shared_ptr<Object> other) { return other == object; });
    cell.erase(it, cell.end());

	return true;
}

void Quadtree::update(std::shared_ptr<Object> object) {
    remove(object);
    add(object);
}

std::vector<std::shared_ptr<Object>> Quadtree::getAll() {
    std::vector<std::shared_ptr<Object>> objects;

    for (int i = 0; i < GRID_SIZE * GRID_SIZE; i++) {
        auto &cell = grid[i];
        objects.insert(objects.end(), cell.begin(), cell.end());
    }

    return objects;
}

unsigned int Quadtree::size() {
	return getAll().size();
}

unsigned int Quadtree::getSubdivisions() {
	return GRID_SIZE * GRID_SIZE;
}

std::vector<std::shared_ptr<Object>> Quadtree::query(raylib::Rectangle range) {
    std::vector<std::shared_ptr<Object>> objects;

    auto startX = (int) (range.x / GRID_SIZE);
    auto startY = (int) (range.y / GRID_SIZE);
    auto endX = (int) ((range.x + range.width) / GRID_SIZE);
    auto endY = (int) ((range.y + range.height) / GRID_SIZE);

    for (int x = startX; x <= endX; x++) {
        for (int y = startY; y <= endY; y++) {
            auto &cell = grid[hash(x, y)];
            if (cell.empty()) continue;
            objects.insert(objects.end(), cell.begin(), cell.end());
        }
    }

    return objects;
}

void Quadtree::render() {
    // Draw lines, grid size is actually the amount of cols and rows, not the size of the grid
    for (int i = 0; i < GRID_SIZE; i++) {
        DrawLineV(raylib::Vector2{ (float) i * GRID_SIZE, 0.0f }, raylib::Vector2{ (float) i * GRID_SIZE, (float) GRID_SIZE * GRID_SIZE }, WHITE);
        DrawLineV(raylib::Vector2{ 0.0f, (float) i * GRID_SIZE }, raylib::Vector2{ (float) GRID_SIZE * GRID_SIZE, (float) i * GRID_SIZE }, WHITE);
    }

    for (int i = 0; i < GRID_SIZE * GRID_SIZE; i++) {
        auto &cell = grid[i];
        for (auto object : cell) {
            DrawCircleV(object->position, object->radius, WHITE);
        }
    }
}

std::vector<std::shared_ptr<Quadtree>> Quadtree::getQuadrants() {
    return std::vector<std::shared_ptr<Quadtree>>();
}

#endif