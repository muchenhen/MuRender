#pragma once
#include <cstddef>
#include <stdexcept>
#include <cmath>

template <typename T>
struct Vec3
{
    union
    {
        struct
        {
            T x, y, z;
        };
        T v[3];
    };

    constexpr Vec3() :
        x(0), y(0), z(0)
    {
    }
    constexpr Vec3(T _x, T _y, T _z) :
        x(_x), y(_y), z(_z)
    {
    }

    // unchecked: 与原生数组语义一致，支持 constexpr / noexcept
    constexpr T& operator[](std::size_t i) noexcept
    {
        return v[i];
    }

    constexpr const T& operator[](std::size_t i) const noexcept
    {
        return v[i];
    }

    // 可选：带边界检查的访问器（在需要安全检查时使用）
    T& at(std::size_t i)
    {
        if (i >= 3) throw std::out_of_range("Vec3::at index out of range");
        return v[i];
    }

    const T& at(std::size_t i) const
    {
        if (i >= 3) throw std::out_of_range("Vec3::at index out of range");
        return v[i];
    }

    // 运算符重载
    constexpr Vec3<T> operator+(const Vec3<T>& other) const noexcept
    {
        return Vec3<T>(x + other.x, y + other.y, z + other.z);
    }
    constexpr Vec3<T> operator-(const Vec3<T>& other) const noexcept
    {
        return Vec3<T>(x - other.x, y - other.y, z - other.z);
    }
    constexpr Vec3<T> operator*(T scalar) const noexcept
    {
        return Vec3<T>(x * scalar, y * scalar, z * scalar);
    }
    constexpr Vec3<T> operator/(T scalar) const noexcept
    {
        return Vec3<T>(x / scalar, y / scalar, z / scalar);
    }

    template <typename T>
    T dot(const Vec3<T>& v1, const Vec3<T>& v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    template <typename T>
    T cross(const Vec3<T>& v1, const Vec3<T>& v2)
    {
        return Vec3<T>(
            v1.y * v2.z - v1.z * v2.y,
            v1.z * v2.x - v1.x * v2.z,
            v1.x * v2.y - v1.y * v2.x
        );
    }

    template <typename T>
    T norm(const Vec3<T>& v)
    {
        return std::sqrt(dot(v, v));
    }

    template <typename T>
    Vec3<T> normalize(const Vec3<T>& v)
    {
        T n = norm(v);
        if (n > 1e-6)
        {
            T inv = static_cast<T>(1) / n;
            return v * inv;
        }
        return v; // 返回原向量以避免除以零
    }
};

void drawline(int x0, int y0, int x1, int y1)
{
    // 1. 检查是否陡峭
    bool steep = std::abs(y1 - y0) > std::abs(x1 - x0);
    if (steep)
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }

    // 2. 确保从左往右画
    if (x0 > x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    // 3. 重新计算 Delta（此时 deltaX 永远为正）
    int deltaX = x1 - x0;
    int deltaY = std::abs(y1 - y0);
    int yStep = (y0 < y1) ? 1 : -1;

    int P = 2 * deltaY - deltaX;

    int y = y0;
    for (int x = x0; x <= x1; ++x)
    {
        if (P >= 0)
        {
            y = y + yStep;
            P += 2 * deltaY - 2 * deltaX;
        }
        else
        {
            y = y;
            P += 2 * deltaY;
        }
        /*
         *  if (steep)
         *  draw pixel(y, x); // 注意坐标交换
         * else
         *  draw pixel(x, y);
        */
    }
}