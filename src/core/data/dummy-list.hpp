#pragma once

#include "../precomp.hpp"
#include "icontainer.hpp"

class Asteroid;

class DummyList : public IContainer {
  private:
	std::vector<std::shared_ptr<Asteroid>> asteroids;

  public:
	DummyList() = default;
	~DummyList() = default;

	void clear() override;
    bool isEmpty() override;
    uint32_t size() override;
	uint16_t getCellIndex(raylib::Vector2 position) override;
	raylib::Vector2 getCellPosition(uint16_t index) override;
	void resize(uint16_t rows, uint16_t cols) override;
	void insert(std::shared_ptr<Asteroid> asteroid) override;
	void remove(std::shared_ptr<Asteroid> asteroid) override;
	std::vector<std::shared_ptr<Asteroid>> retrieve(raylib::Vector2 position, uint16_t radius) override;
    std::vector<std::shared_ptr<Asteroid>> getAll() override;
};