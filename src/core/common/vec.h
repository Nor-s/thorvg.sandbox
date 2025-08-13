#ifndef _CORE_COMMON_VECTOR3_H_
#define _CORE_COMMON_VECTOR3_H_

#include <cmath>
#include <cstring>

namespace core
{
template <typename T>
struct Vector2
{
	using element_type = T;
	union
	{
		struct
		{
			T x, y;
		};
		struct
		{
			T w, h;
		};
		struct
		{
			T r, g;
		};
		T value[2] = {
			0,
		};
	};
	Vector2(const Vector2& v)
		: x(v.x), y(v.y)
	{
	}
	Vector2()
	{
		memset(value, 0, sizeof value);
	}
	Vector2(T x, T y) : x(x), y(y)
	{
	}
	T& operator[](size_t col)
	{
		return value[col];
	}
	const T& operator[](size_t col) const
	{
		return value[col];
	}
	bool operator==(const Vector2& rhs)
	{
		return x == rhs.x &&  y == rhs.y;
	}
	bool operator!=(const Vector2& rhs)
	{
		return !(*this == rhs);
	}
};

template <typename T>
struct Vector3
{
	using element_type = T;
	union
	{
		struct
		{
			T x, y, z;
		};
		struct
		{
			T r, g, b;
		};
		T value[3] = {
			0,
		};
	};
	Vector3()
	{
		memset(value, 0, sizeof value);
	}
	Vector3(T x)
	{
		value[0] = x;
		value[1] = x;
		value[2] = x;
	}
	Vector3(Vector2<T> xy, float z)
	{
		value[0] = xy.x;
		value[1] = xy.y;
		value[2] = z;
	}
	Vector3(T x, T y, T z)
	{
		value[0] = x;
		value[1] = y;
		value[2] = z;
	}
	T& operator[](size_t col)
	{
		return value[col];
	}
	const T& operator[](size_t col) const
	{
		return value[col];
	}
	explicit operator Vector2<T>() const
	{
		return Vector2<T>(x, y);
	}
	explicit operator float() const
	{
		return x;
	}
};

template <typename T>
struct Vector4
{
	using element_type = T;
	union
	{
		struct
		{
			T x, y, z, w;
		};
		struct
		{
			T r, g, b, a;
		};
		T value[4] = {
			0,
		};
	};
	Vector4()
	{
		memset(value, 0, sizeof value);
	}
	Vector4(T x, T y, T z, T w)
	{
		value[0] = x;
		value[1] = y;
		value[2] = z;
		value[3] = w;
	}
	Vector4(Vector3<T> v, T w)
	{
		value[0] = v.x;
		value[1] = v.y;
		value[2] = v.z;
		value[3] = w;
	}
	T& operator[](size_t col)
	{
		return value[col];
	}
	const T& operator[](size_t col) const
	{
		return value[col];
	}
	operator Vector3<T>()
	{
		return Vector3<T>{x, y, z};
	}
	operator const Vector3<T>() const
	{
		return Vector3<T>{x, y, z};
	}
};

using Vec2 = Vector2<float>;
using Vec3 = Vector3<float>;
using Vec4 = Vector4<float>;

// dot product
template <typename T>
T operator*(const Vector2<T>& a, const Vector2<T>& b)
{
	return a.x * b.x + a.y * b.y;
}

template <typename T>
T operator*(const Vector3<T>& a, const Vector3<T>& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

template <typename T>
Vector2<T> operator-(const Vector2<T>& a, const Vector2<T>& b)
{
	return Vector2<T>{a.x - b.x, a.y - b.y};
}
template <typename T>
Vector2<T> operator+(const Vector2<T>& a, const Vector2<T>& b)
{
	return Vector2<T>{a.x + b.x, a.y + b.y};
}

template <typename T>
Vector3<T> operator+(const Vector3<T>& a, const Vector3<T>& b)
{
	return Vector3<T>{a.x + b.x, a.y + b.y, a.z + b.z};
}

template <typename T>
Vector3<T> operator*(const Vector3<T>& a, T b)
{
	return Vector3<T>{a.x * b, a.y * b, a.z * b};
}

template <typename T>
Vector3<T> operator-(const Vector3<T>& a, const Vector3<T>& b)
{
	return Vector3<T>{a.x - b.x, a.y - b.y, a.z - b.z};
}

template <typename T>
Vector3<T> operator/(const Vector3<T>& a, T b)
{
	return Vector3<T>{a.x / b, a.y / b, a.z / b};
}

// dot product
template <typename T>
T operator*(const Vector4<T>& a, const Vector4<T>& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

template <typename T>
Vector4<T> operator+(const Vector4<T>& a, const Vector4<T>& b)
{
	return Vector4<T>{a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}

template <typename T>
Vector4<T> operator*(const Vector4<T>& a, T b)
{
	return Vector4<T>{a.x * b, a.y * b, a.z * b, a.w * b};
}

template <typename T>
Vector4<T> operator-(const Vector4<T>& a, const Vector4<T>& b)
{
	return Vector4<T>{a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}

template <typename T>
Vector4<T> operator/(const Vector4<T>& a, T b)
{
	return Vector4<T>{a.x / b, a.y / b, a.z / b, a.w / b};
}

template <typename T>
float length2(const T& a)
{
	return a * a;
}

template <typename T>
float length(const T& a)
{
	return std::sqrt(length2(a));
}

template <typename T>
const T normalize(const T& a)
{
	return a / length(a);
}
template <typename T>
const Vector3<T> cross(const Vector3<T>& a, const Vector3<T>& b)
{
	Vector3<T> ret;

	ret.x = a.y * b.z - a.z * b.y;
	ret.y = a.z * b.x - a.x * b.z;
	ret.z = a.x * b.y - a.y * b.x;

	return ret;
}

template <typename T>
T cross(const Vector2<T>& lhs, const Vector2<T>& rhs)
{
    return lhs.x*rhs.y - rhs.x*lhs.y;
}

template <typename T>
const Vector2<T> abs(const Vector2<T> a)
{
	Vector2<T> ret; 
	ret.x = std::abs(a.x);
	ret.y = std::abs(a.y);
	return ret;
}

}	 // namespace core

#endif
