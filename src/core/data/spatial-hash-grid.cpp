#include "spatial-hash-grid.hpp"
#include "../settings.hpp"
#include "../models/asteroid.hpp"
#include "Color.hpp"
#include "Rectangle.hpp"
#include "icontainer.hpp"
#include "raylib.h"
#include <mutex>

void SpatialHashGrid::clear() {
    for (auto &cell : cells) {
        cell.asteroids.resize(0);
    }
    cache.clear();
}

bool SpatialHashGrid::isEmpty() {
    return cells.empty() || cache.empty();
}

bool SpatialHashGrid::isInitialized() {
    return !cells.empty();
}

uint32_t SpatialHashGrid::size() {
    return cache.size();
}

uint16_t SpatialHashGrid::getCellIndex(raylib::Vector2 position) {
    uint16_t row = position.y / cellSize;
    uint16_t col = position.x / cellSize;
    if (row < 0) row = 0;
    else if (row >= rows) row = rows - 1;
    if (col < 0) col = 0;
    else if (col >= cols) col = cols - 1;
    return row * cols + col;
}

raylib::Vector2 SpatialHashGrid::getCellPosition(uint16_t index) {
    return raylib::Vector2((index % cols) * cellSize, ((float)index / cols) * cellSize);
}

void SpatialHashGrid::resize(uint16_t rows, uint16_t cols) {
    std::lock_guard<std::mutex> lock(mutex);

    const auto backup = cache;
    cache.clear();
    cells.clear();
    cells.resize(rows * cols);

    this->rows = rows;
    this->cols = cols;

    cellSize = std::min(ceilf(HEIGHT / (float)rows), ceilf(WIDTH / (float)cols));

    for (auto &asteroid : backup) insert(asteroid);

    // Assign color palette
    for (uint16_t row = 0; row < rows; row++) {
        for (uint16_t col = 0; col < cols; col++) {
            const float index = (row * cols + col) / (float)(rows * cols) * 240.0f;
            cells[index].color = raylib::Color::FromHSV(index, 0.5f, 0.5f);
        }
    }
}

void SpatialHashGrid::insert(std::shared_ptr<Asteroid> asteroid) {
    std::lock_guard<std::mutex> lock(mutex);

    auto index = getCellIndex(asteroid->position);
    asteroid->gridCellIndex = index;
    asteroid->color = cells[index].color;
    cells[index].asteroids.push_back(asteroid);
    cache.push_back(asteroid);
}

void SpatialHashGrid::remove(std::shared_ptr<Asteroid> asteroid) {
    std::lock_guard<std::mutex> lock(mutex);

    auto index = getCellIndex(asteroid->position);
    asteroid->gridCellIndex = -1;
    cells[index].asteroids.remove(asteroid);
    cache.erase(std::remove(cache.begin(), cache.end(), asteroid), cache.end());
}

void SpatialHashGrid::update() {
    std::lock_guard<std::mutex> lock(mutex);

    for (auto &cell : cells) {
        auto it = cell.asteroids.begin();
        while (it != cell.asteroids.end()) {
            const auto &asteroid = *it;
            const uint16_t currentIndex = asteroid->gridCellIndex;
            const uint16_t newIndex = getCellIndex(asteroid->position);
            if (currentIndex != newIndex) {
                asteroid->gridCellIndex = newIndex;
                asteroid->color = cells[newIndex].color;
                cells[newIndex].asteroids.push_back(asteroid);
                it = cell.asteroids.erase(it);
            } else {
                it++;
            }
        }
    }
}

std::vector<std::shared_ptr<Asteroid>> SpatialHashGrid::retrieve(raylib::Vector2 position, uint16_t radius) {
    std::lock_guard<std::mutex> lock(mutex);

    // Get the adjacent cells based on the radius
    uint16_t startRow = std::max(0, (int)(position.y - radius) / cellSize);
    uint16_t endRow = std::min(rows - 1, (int)(position.y + radius) / cellSize);
    uint16_t startCol = std::max(0, (int)(position.x - radius) / cellSize);
    uint16_t endCol = std::min(cols - 1, (int)(position.x + radius) / cellSize);

    std::vector<std::shared_ptr<Asteroid>> result;
    for (uint16_t row = startRow; row <= endRow; row++) {
        for (uint16_t col = startCol; col <= endCol; col++) {
            uint16_t index = row * cols + col;
            if (index >= cells.size() || index < 0) continue;

            for (auto &asteroid : cells[index].asteroids) {
                if (asteroid && position.Distance(asteroid->position) <= radius) {
                    result.push_back(asteroid);
                }
            }
        }
    }

    return result;
}

std::vector<std::shared_ptr<Asteroid>> SpatialHashGrid::getAll() {
    return cache;
}

void SpatialHashGrid::render() {
    for (uint16_t row = 0; row < rows; row++) {
        for (uint16_t col = 0; col < cols; col++) {
            const uint16_t index = row * cols + col;
            const raylib::Rectangle rect(col * cellSize, row * cellSize, cellSize, cellSize);
            DrawRectangleLinesEx(rect, 1.0f, cells[index].color);
            cells[index].color.SetA(50);
            DrawRectangleRec(rect, cells[index].color);
            cells[index].color.SetA(255);
            DrawText(TextFormat("%d", index), col * cellSize + 5, row * cellSize + 5, 10, WHITE);
        }
    }
}
