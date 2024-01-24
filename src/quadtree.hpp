#pragma once

#include "object.hpp"
#include "precomp.hpp"

class Object;
class Quadtree;

class Quadtree {
  private:
	raylib::Rectangle bounds;
	uint8_t depth;
	raylib::Color color;
	std::mutex mutex;

#ifdef QUADTREE_GRID
	std::unordered_map<int, std::vector<std::shared_ptr<Object>>> grid;
#else
	std::vector<std::shared_ptr<Object>> children;
	std::vector<std::shared_ptr<Quadtree>> quadrants;
#endif

	bool isLeaf();
	void split();
	void merge();

  public:
	Quadtree(raylib::Rectangle bounds, uint8_t depth = 0);

	bool add(std::shared_ptr<Object> object);
	bool remove(std::shared_ptr<Object> object);
	void update(std::shared_ptr<Object> object);
	std::vector<std::shared_ptr<Object>> getAll();
	std::vector<std::shared_ptr<Quadtree>> getQuadrants();
	unsigned int size();
	unsigned int getSubdivisions();
	std::vector<std::shared_ptr<Object>> query(raylib::Rectangle range);
	void render();
};