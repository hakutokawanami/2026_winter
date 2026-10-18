#pragma once
#include"Shape.h"

namespace Collision
{
	//Shapeの組み合わせ
	Contact Intersect(const Sphere& a, const Sphere& b);
	Contact Intersect(const Capsule& a, const Sphere& b);
	Contact Intersect(const Sphere& a, const Capsule& b);
	Contact Intersect(const Capsule& a, const Capsule& b);

	//形の組み合わせをみて自動で選ぶ
	inline Contact Intersect(const Shape& a, const Shape& b)
	{
		return std::visit([](const auto& x, const auto& y) {return Intersect(x, y); },
			a, b);
	}
}

