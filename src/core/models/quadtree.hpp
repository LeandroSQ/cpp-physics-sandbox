#pragma once

#include "../precomp.hpp"
#include "object.hpp"

class Object;
class Quadtree;

class Quadtree {
  private:
	std::vector<std::shared_ptr<Object>> children;
	std::vector<std::shared_ptr<Quadtree>> quadrants;
	raylib::Rectangle bounds;
	raylib::Color color;
	uint8_t depth;

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
    void clear();
	void render();
};