#pragma once

#include "../precomp.hpp"

class Asteroid;

class IContainer {
  public:
	virtual void clear() = 0;
	virtual bool isEmpty() = 0;
	virtual uint32_t size() = 0;
	virtual uint16_t getCellIndex(raylib::Vector2 position) = 0;
	virtual raylib::Vector2 getCellPosition(uint16_t index) = 0;
	virtual void resize(uint16_t rows, uint16_t cols) = 0;
	virtual void insert(std::shared_ptr<Asteroid> asteroid) = 0;
	virtual void remove(std::shared_ptr<Asteroid> asteroid) = 0;
	virtual std::vector<std::shared_ptr<Asteroid>> retrieve(raylib::Vector2 position, uint16_t radius) = 0;
	virtual std::vector<std::shared_ptr<Asteroid>> all() = 0;
};