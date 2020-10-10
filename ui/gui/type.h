#pragma once

namespace ui
{
    typedef unsigned int unit;
    // point
    template <typename T>
    struct Point
    {
        T x;
        T y;
    };
    // rect
    template <typename T>
    struct Rect
    {
        T left;
        T top;
        T right;
        T bottom;
    };
    // rect w/h model
    template <typename T>
    struct RectWH
    {
        T left;
        T top;
        T width;
        T height;
    };
    // size
    template <typename T>
    struct Size
    {
        T width;
        T height;
    };
    // Point2U
    using Point2U = Point<uint32_t>;
    // Point2F
    using Point2F = Point<float>;
    // Point2L
    using Point2L = Point<std::int32_t>;
    // Vector2F
    struct Vector2F
    {
        float x;
        float y;
    };
    // Vector3F
    struct Vector3F
    {
        float x;
        float y;
        float z;
    };
    // Vector4F
    struct Vector4F
    {
        float x;
        float y;
        float z;
        float w;
    };
    // RectF
    using RectF = Rect<float>;
    // RectU
    using RectU = Rect<uint32_t>;
    // RectL
    using RectL = Rect<std::int32_t>;
    // RectWHF
    using RectWHF = RectWH<float>;
    // RectWHU
    using RectWHU = RectWH<uint32_t>;
    // RectWHL
    using RectWHL = RectWH<std::int32_t>;
    // Size2F
    using Size2F = Size<float>;
    // Size2U
    using Size2U = Size<uint32_t>;
    // Size2U
    using Size2L = Size<int32_t>;
    // Matrix3X2F
    struct Matrix3X2F
    {
        float _11, _12, _21, _22, _31, _32;
    };
    // Matrix4X4F
    struct Matrix4X4F
    {
        float m[4][4];
    };
#ifndef NDEBUG
    // debug display format float: x.xx
    struct DDFFloat2
    {
        float f;
    };
    // debug display format float: x.xxx
    struct DDFFloat3
    {
        float f;
    };
    // debug display format float: x.xxxx
    struct DDFFloat4
    {
        float f;
    };
    // get float1
    static inline auto Get2() noexcept -> DDFFloat2 { return {0.0f}; }
    // get float1
    static inline auto Get3() noexcept -> DDFFloat3 { return {0.0f}; }
    // get float1
    static inline auto Get4() noexcept -> DDFFloat4 { return {0.0f}; }
#endif
    // Ellipse
    struct Ellipse
    {
        Point2F point;
        float radius_x;
        float radius_y;
    };
    // transform point
    auto TransformPoint(const Matrix3X2F& matrix, Point2F point) noexcept
        -> Point2F;
    // transform point - inverse
    auto TransformPointInverse(const Matrix3X2F& matrix, Point2F point) noexcept
        -> Point2F;
    // is overlap?
    bool IsOverlap(const RectF& a, const RectF& b) noexcept;
    // is include?
    bool IsInclude(const RectF& a, const RectF& b) noexcept;
    // is include?
    bool IsInclude(const RectF& a, const Point2F& b) noexcept;
    // is same?
    bool IsSame(const RectF& a, const RectF& b) noexcept;
    // gui float same
    bool IsSameInGuiLevel(float a, float b) noexcept;
    // gui point same
    bool IsSameInGuiLevel(Point2F a, Point2F b) noexcept;
    // get area
    auto GetArea(const RectF& rect) noexcept -> float;
    // mix point
    auto Mix(Point2F from, Point2F to, float progress) noexcept -> Point2F;
    // mix rect
    auto Mix(const RectF& from, const RectF& to, float progress) noexcept;
    // round in gui level
    inline auto RoundInGuiLevel(float a) noexcept
    {
        return float(long(a + 0.5f));
    }
    // round in gui level
    inline auto RoundInGuiLevel(double a) noexcept
    {
        return double(long(a + 0.5));
    }

    // gui size same
    inline bool IsSameInGuiLevel(const Size2F& a, const Size2F& b) noexcept
    {
        return IsSameInGuiLevel(reinterpret_cast<const Point2F&>(a),
                                reinterpret_cast<const Point2F&>(b));
    }
    // point + oparator
    template <typename T>
    auto operator+(Point<T> a, Point<T> b) noexcept
    {
        return Point<T>{a.x + b.x, a.y + b.y};
    }
    // point - oparator
    template <typename T>
    auto operator-(Point<T> a, Point<T> b) noexcept
    {
        return Point<T>{a.x - b.x, a.y - b.y};
    }

    
}  // namespace gui