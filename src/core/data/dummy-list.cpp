#include "dummy-list.hpp"
#include "../models/asteroid.hpp"

void DummyList::clear() {
    asteroids.clear();
}

bool DummyList::isEmpty() {
    return asteroids.empty();
}

uint32_t DummyList::size() {
    return asteroids.size();
}

uint16_t DummyList::getCellIndex(raylib::Vector2 position) {
    return 0;
}

raylib::Vector2 DummyList::getCellPosition(uint16_t index) {
    return raylib::Vector2::Zero();
}

void DummyList::resize(uint16_t rows, uint16_t cols) {
    // Do nothing
}

void DummyList::insert(std::shared_ptr<Asteroid> asteroid) {
    asteroids.push_back(asteroid);
}

void DummyList::remove(std::shared_ptr<Asteroid> asteroid) {
    asteroids.erase(std::remove(asteroids.begin(), asteroids.end(), asteroid), asteroids.end());
}

std::vector<std::shared_ptr<Asteroid>> DummyList::retrieve(raylib::Vector2 position, uint16_t radius) {
    return asteroids;
}

std::vector<std::shared_ptr<Asteroid>> DummyList::all() {
    return asteroids;
}