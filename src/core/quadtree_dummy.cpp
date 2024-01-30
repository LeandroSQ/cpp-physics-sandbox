/**
 * @file quadtree_dummy.cpp
 * @author LeandroSQ
 * @brief Dummy quadtree implementation for testing purposes
 * @note This file is only compiled if QUADTREE_DUMMY is defined
 *
 * This emulates the behavior of a vector but exposes the same interface as the quadtree
 * so it can be easily compared with the actual quadtree implementation
 */

#include "constants.hpp"

#ifdef QUADTREE_DUMMY

#include "quadtree.hpp"

Quadtree::Quadtree(raylib::Rectangle bounds, uint8_t depth) : bounds(bounds), depth(depth) { }

bool Quadtree::isLeaf() {
	return false;
}

void Quadtree::split() {

}

void Quadtree::merge() {

}

bool Quadtree::add(std::shared_ptr<Object> object) {
    children.push_back(object);

	return true;
}

bool Quadtree::remove(std::shared_ptr<Object> object) {
    for (auto it = children.begin(); it != children.end(); it++) {
        if (*it == object) {
            children.erase(it);
            return true;
        }
    }

	return true;
}

void Quadtree::update(std::shared_ptr<Object> object) {

}

std::vector<std::shared_ptr<Object>> Quadtree::getAll() {
	return children;
}

unsigned int Quadtree::size() {
	return children.size();
}

unsigned int Quadtree::getSubdivisions() {
	return 0;
}

std::vector<std::shared_ptr<Object>> Quadtree::query(raylib::Rectangle range) {
	std::vector<std::shared_ptr<Object>> objects;

	for (auto node : children) {
        objects.push_back(node);
    }

	return objects;
}

void Quadtree::render() {
    for (auto node : children) {
        DrawCircleV(node->position, node->radius, WHITE);
    }
}

void Quadtree::clear() {
    children.clear();
}

std::vector<std::shared_ptr<Quadtree>> Quadtree::getQuadrants() {
    return quadrants;
}

#endif