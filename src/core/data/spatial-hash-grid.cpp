#include "spatial-hash-grid.hpp"
#include "../settings.hpp"
#include "../models/asteroid.hpp"

void SpatialHashGrid::clear() {
    for (auto &cell : cells) {
        cell.asteroids.resize(0);
    }
    count = 0;
}

bool SpatialHashGrid::isEmpty() {
    return cells.empty() || count <= 0;
}

uint32_t SpatialHashGrid::size() {
    return count;
}

uint16_t SpatialHashGrid::getCellIndex(raylib::Vector2 position) {
    uint16_t index = (uint16_t)(position.y / cellSize) * cols + (uint16_t)(position.x / cellSize);
    if (index < 0) index = 0;
    if (index >= cells.size()) index = cells.size() - 1;
    return index;
}

raylib::Vector2 SpatialHashGrid::getCellPosition(uint16_t index) {
    return raylib::Vector2((index % cols) * cellSize, ((float)index / cols) * cellSize);
}

void SpatialHashGrid::resize(uint16_t rows, uint16_t cols) {
    const auto backup = all();
    cells.clear();
    count = 0;
    cells.resize(rows * cols);

    this->rows = rows;
    this->cols = cols;

    cellSize = std::min(ceilf(HEIGHT / (float)rows), ceilf(WIDTH / (float)cols));

    for (auto &asteroid : backup) {
        insert(asteroid);
    }
}

void SpatialHashGrid::insert(std::shared_ptr<Asteroid> asteroid) {
    auto index = getCellIndex(asteroid->position);
    asteroid->index = index;
    asteroid->color = cells[index].color;
    cells[index].asteroids.push_back(asteroid);
    count++;
}

void SpatialHashGrid::remove(std::shared_ptr<Asteroid> asteroid) {
    auto index = getCellIndex(asteroid->position);
    asteroid->index = -1;
    cells[index].asteroids.remove(asteroid);
    count--;
}

void SpatialHashGrid::update() {
    for (auto &cell : cells) {
        auto it = cell.asteroids.begin();
        while (it != cell.asteroids.end()) {
            uint16_t index = getCellIndex(it->get()->position);
            if (index != it->get()->index) {
                it->get()->index = index;
                it->get()->color = cell.color;
                cells[index].asteroids.push_back(*it);
                it = cells[it->get()->index].asteroids.erase(it);
            } else {
                it++;
            }
        }
    }
}

std::vector<std::shared_ptr<Asteroid>> SpatialHashGrid::retrieve(raylib::Vector2 position, uint16_t radius) {
    // Get the adjacent cells based on the radius
    uint16_t startRow = std::max(0, (int)(position.y - radius) / cellSize);
    uint16_t endRow = std::min(rows - 1, (int)(position.y + radius) / cellSize);
    uint16_t startCol = std::max(0, (int)(position.x - radius) / cellSize);
    uint16_t endCol = std::min(cols - 1, (int)(position.x + radius) / cellSize);

    std::vector<std::shared_ptr<Asteroid>> result;
    for (uint16_t row = startRow; row <= endRow; row++) {
        for (uint16_t col = startCol; col <= endCol; col++) {
            uint16_t index = row * cols + col;
            for (auto &asteroid : cells[index].asteroids) {
                if (position.Distance(asteroid->position) <= radius) {
                    result.push_back(asteroid);
                }
            }
        }
    }

    return result;
}

std::vector<std::shared_ptr<Asteroid>> SpatialHashGrid::all() {
    std::vector<std::shared_ptr<Asteroid>> result;
    for (auto &cell : cells) {
        result.insert(result.end(), cell.asteroids.begin(), cell.asteroids.end());
    }
    return result;
}

void SpatialHashGrid::render() {
    for (uint16_t row = 0; row < rows; row++) {
        for (uint16_t col = 0; col < cols; col++) {
            uint16_t index = row * cols + col;
            DrawRectangleLines(col * cellSize, row * cellSize, cellSize, cellSize, cells[index].color);
            DrawText(TextFormat("%d", index), col * cellSize + 5, row * cellSize + 5, 10, WHITE);
        }
    }
}
