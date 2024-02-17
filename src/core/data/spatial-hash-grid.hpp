#pragma once

#include "../precomp.hpp"
#include "icontainer.hpp"
#include "../utils.hpp"
#include <vector>

class SpatialHashGrid : public IContainer {
  private:
	struct Cell {
		std::list<std::shared_ptr<Asteroid>> asteroids;
        raylib::Color color = randomColor();
	};
    std::vector<std::shared_ptr<Asteroid>> cache;
	std::vector<Cell> cells;
    std::mutex mutex;

  public:
    uint16_t rows;
	uint16_t cols;
	uint16_t cellSize;

	SpatialHashGrid() = default;
	~SpatialHashGrid() = default;

	void clear() override;
    bool isEmpty() override;
    bool isInitialized() override;
    uint32_t size() override;
	uint16_t getCellIndex(raylib::Vector2 position) override;
	raylib::Vector2 getCellPosition(uint16_t index) override;
	void resize(uint16_t rows, uint16_t cols) override;
	void insert(std::shared_ptr<Asteroid> asteroid) override;
	void remove(std::shared_ptr<Asteroid> asteroid) override;
	void update();
	std::vector<std::shared_ptr<Asteroid>> retrieve(raylib::Vector2 position, uint16_t radius) override;
    std::vector<std::shared_ptr<Asteroid>> getAll() override;
    void render();
};