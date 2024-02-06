// #include "collision-handler.hpp"

// using namespace CollisionHandler;

// struct Projection {
// 	float min;
// 	float max;
// };

// void getAxes(const std::vector<raylib::Vector2> &vertices, std::vector<raylib::Vector2> &axes) {
// 	for (size_t i = 0; i < vertices.size(); i++) {
// 		const auto p1 = vertices[i];
// 		const auto p2 = vertices[(i + 1) % vertices.size()];
// 		const auto edge = p1 - p2;
// 		axes.emplace_back(-edge.y, edge.x); // Perpendicular vector
// 	}
// }

// Projection project(const std::vector<raylib::Vector2> &vertices, const raylib::Vector2 &axis) {
// 	float min = axis.DotProduct(vertices[0]);
// 	float max = min;
// 	for (size_t i = 1; i < vertices.size(); i++) {
// 		const float p = axis.DotProduct(vertices[i]);
// 		if (p < min)
// 			min = p;
// 		else if (p > max)
// 			max = p;
// 	}

// 	return { min, max };
// }

// std::optional<CollisionInfo> CollisionHandler::getCollisionInfo(const Shape &a, const Shape &b) {
// 	auto normal = raylib::Vector2::Zero();
// 	float overlap = std::numeric_limits<float>::max();

// 	// Get the axes of the polygons
// 	std::vector<raylib::Vector2> axes;
//     axes.reserve(a.vertices.size() + b.vertices.size());
// 	getAxes(a.vertices, axes);
// 	getAxes(b.vertices, axes);

// 	// Iterate through the axes
//     for (auto &axis : axes) {
// 		const auto projectionA = project(a.vertices, axis);
// 		const auto projectionB = project(b.vertices, axis);

// 		// Detected a gap, no collision
// 		if (projectionA.max < projectionB.min || projectionB.max < projectionA.min) {
// 			return std::nullopt;
// 		}

// 		// Calculate the overlap
// 		const float depth = std::min(projectionA.max, projectionB.max) - std::max(projectionA.min, projectionB.min);
// 		if (depth < overlap) {
// 			overlap = depth;
// 			normal = axis;
// 		}
// 	}

// 	// Normalize the depth and normal
// 	const auto length = normal.Length();
// 	normal /= length;
// 	overlap /= length;

// 	// If the normal is pointing from A to B, invert it
// 	const auto center = b.position - a.position;
// 	if (center.DotProduct(normal) < 0) {
// 		normal = -normal;
// 	}

// 	return CollisionInfo{ normal, overlap };
// }