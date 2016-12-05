#pragma once
#include <iostream>

namespace Math
{
	class Vec2
	{
	public:
		Vec2();
		Vec2(float xi, float yi);

		float x;
		float y;

		/// <summary>
		/// Applies compontents pairwise.
		/// </summary>
		bool operator==(const Vec2& other) const;

		Vec2 operator+(const Vec2& other) const;
		Vec2 operator-(const Vec2& other) const;
		Vec2 operator*(const Vec2& other) const;
		Vec2 operator/(const Vec2& other) const;

		Vec2& operator+=(const Vec2& other);
		Vec2& operator-=(const Vec2& other);
		Vec2& operator*=(const Vec2& other);
		Vec2& operator/=(const Vec2& other);

		/// <summary>
		/// Applies constant to each component.
		/// </summary>
		Vec2 operator+(float c) const;
		Vec2 operator-(float c) const;
		Vec2 operator*(float c) const;
		Vec2 operator/(float c) const;

		Vec2& operator+=(float c);
		Vec2& operator-=(float c);
		Vec2& operator*=(float c);
		Vec2& operator/=(float c);


		/// <summary>
		/// Returns vector inversed.
		/// </summary>
		Vec2 inv() const;
		/// <summary>
		/// Returns vector from this vector to another.
		/// </summary>
		Vec2 to(const Vec2& other) const;
		/// <summary>
		/// Returns the vector rotated CCW the specified number of radians.
		/// </summary>
		Vec2 rotate(float radiansCCW) const;
		/// <summary>
		/// Find length of this vector.
		/// </summary>
		float len() const;
		/// <summary>
		/// Return unit vector of this Vec2.
		/// </summary>
		Vec2 unit() const;
		/// <summary>
		/// Gets a bounced ray off of a surface normal vector.
		/// </summary>
		Vec2 bounce(const Vec2& normal) const;
		/// <summary>
		/// Compute dot product of two vectors.
		/// </summary>
		float dot(const Vec2& other) const;
		/// <summary>
		/// This vector to an angle.
		/// </summary>
		float toAngle() const;

		/// <summary>
		/// Print this vector.
		/// </summary>
		inline friend std::ostream& operator<<(std::ostream &os, const Vec2& vec)
		{
			return os << "(" << vec.x << ", " << vec.y << ")";
		}
	};
}
