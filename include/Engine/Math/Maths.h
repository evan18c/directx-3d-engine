#pragma once
#include <algorithm>
#include <float.h>
#include <math.h>

const float PI = 3.1415926535;

struct Vec2 {
    float x, y;
};

struct Vec3 {
    float x, y, z;
};

struct Vec4 {
    float x, y, z, w;
};

struct Mat4 {
    float m[4][4];
};

struct AABB {
    Vec3 min;
    Vec3 max;

    bool Intersects(const AABB &other) {
        return (min.x <= other.max.x && max.x >= other.min.x) &&
               (min.y <= other.max.y && max.y >= other.min.y) &&
               (min.z <= other.max.z && max.z >= other.min.z);
    }
};

inline Vec3 Add(Vec3 a, Vec3 b) {
    return { a.x + b.x, a.y + b.y, a.z + b.z };
}

inline Vec3 Sub(Vec3 a, Vec3 b) {
    return { a.x - b.x, a.y - b.y, a.z - b.z };
}

inline Vec3 Mul(Vec3 a, float s) {
    return { a.x * s, a.y * s, a.z * s };
}

inline Mat4 Identity() {
    Mat4 r = {};
    for (int i=0; i<4; i++) r.m[i][i] = 1.0f;
    return r;
}

inline float Length(const Vec3& v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

inline Vec3 Normalize(const Vec3& v) {
    float len = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    return { v.x / len, v.y / len, v.z / len };
}

inline Vec3 Cross(const Vec3& a, const Vec3& b) {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

inline float Dot(const Vec3& a, const Vec3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline Mat4 Projection(float fov, float aspect, float nearZ, float farZ) {
    Mat4 r = {};
    r.m[0][0] = 1 / (aspect * tan(fov / 2));
    r.m[1][1] = 1 / tan(fov / 2);
    r.m[2][2] = farZ / (farZ - nearZ);
    r.m[2][3] = 1.0f;
    r.m[3][2] = (-nearZ * farZ) / (farZ - nearZ);
    return r;
}

inline Mat4 LookAt(Vec3 eye, Vec3 target, Vec3 up) {
    Vec3 zaxis = Normalize(Sub(target, eye));
    Vec3 xaxis = Normalize(Cross(up, zaxis));
    Vec3 yaxis = Cross(zaxis, xaxis);

    Mat4 r = {};
    r.m[0][0] = xaxis.x;  r.m[0][1] = yaxis.x;  r.m[0][2] = zaxis.x;  r.m[0][3] = 0.0f;
    r.m[1][0] = xaxis.y;  r.m[1][1] = yaxis.y;  r.m[1][2] = zaxis.y;  r.m[1][3] = 0.0f;
    r.m[2][0] = xaxis.z;  r.m[2][1] = yaxis.z;  r.m[2][2] = zaxis.z;  r.m[2][3] = 0.0f;
    r.m[3][0] = -Dot(xaxis, eye);
    r.m[3][1] = -Dot(yaxis, eye);
    r.m[3][2] = -Dot(zaxis, eye);
    r.m[3][3] = 1.0f;
    return r;
}

inline Mat4 Multiply(const Mat4& a, const Mat4& b) {
    Mat4 result = {};
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.m[i][j] =
                a.m[i][0] * b.m[0][j] +
                a.m[i][1] * b.m[1][j] +
                a.m[i][2] * b.m[2][j] +
                a.m[i][3] * b.m[3][j];
        }
    }
    return result;
}

inline Mat4 Scale(Vec3 factor) {
    Mat4 result = {};
    result.m[0][0] = factor.x;
    result.m[1][1] = factor.y;
    result.m[2][2] = factor.z;
    result.m[3][3] = 1.0f;
    return result;
}

inline Mat4 Translate(Vec3 vec) {
    Mat4 result = Identity();
    result.m[3][0] = vec.x;
    result.m[3][1] = vec.y;
    result.m[3][2] = vec.z;
    return result;
}

inline Mat4 Rotate(Vec3 rotation) {
    float cx = cosf(rotation.x);
    float sx = sinf(rotation.x);
    float cy = cosf(rotation.y);
    float sy = sinf(rotation.y);
    float cz = cosf(rotation.z);
    float sz = sinf(rotation.z);

    Mat4 result = {};

    // Combined rotation (Y * X * Z)
    result.m[0][0] = cy * cz + sy * sx * sz;
    result.m[0][1] = cx * sz;
    result.m[0][2] = -sy * cz + cy * sx * sz;
    result.m[0][3] = 0.0f;

    result.m[1][0] = -cy * sz + sy * sx * cz;
    result.m[1][1] = cx * cz;
    result.m[1][2] = sy * sz + cy * sx * cz;
    result.m[1][3] = 0.0f;

    result.m[2][0] = sy * cx;
    result.m[2][1] = -sx;
    result.m[2][2] = cy * cx;
    result.m[2][3] = 0.0f;

    result.m[3][0] = 0.0f;
    result.m[3][1] = 0.0f;
    result.m[3][2] = 0.0f;
    result.m[3][3] = 1.0f;

    return result;
}

// Returns true if AABB intersects triangle (v0, v1, v2)
// Credit to ChatGPT for this one, I have no idea how this works LOL
inline bool AABBvsTriangle(const AABB& box, const Vec3& v0, const Vec3& v1, const Vec3& v2)
{
    // Compute box center and half-size
    Vec3 c = {
        (box.min.x + box.max.x) * 0.5f,
        (box.min.y + box.max.y) * 0.5f,
        (box.min.z + box.max.z) * 0.5f
    };
    Vec3 e = {
        (box.max.x - box.min.x) * 0.5f,
        (box.max.y - box.min.y) * 0.5f,
        (box.max.z - box.min.z) * 0.5f
    };

    // Move triangle to box space
    Vec3 v0b = Sub(v0, c);
    Vec3 v1b = Sub(v1, c);
    Vec3 v2b = Sub(v2, c);

    // Compute triangle edges
    Vec3 f0 = Sub(v1b, v0b);
    Vec3 f1 = Sub(v2b, v1b);
    Vec3 f2 = Sub(v0b, v2b);

    // Define a lambda for projection test on an axis
    auto axisTest = [&](const Vec3& axis) -> bool {
        if (axis.x == 0 && axis.y == 0 && axis.z == 0) return true; // skip degenerate
        float p0 = Dot(v0b, axis);
        float p1 = Dot(v1b, axis);
        float p2 = Dot(v2b, axis);
        float r = e.x * abs(axis.x) + e.y * abs(axis.y) + e.z * abs(axis.z);
        float minP = std::min({p0, p1, p2});
        float maxP = std::max({p0, p1, p2});
        return !(minP > r || maxP < -r);
    };

    // 1. Test axes: box normals (x, y, z)
    if (!axisTest({1, 0, 0})) return false;
    if (!axisTest({0, 1, 0})) return false;
    if (!axisTest({0, 0, 1})) return false;

    // 2. Test axes: 9 cross products (edge × axis)
    Vec3 axes[9] = {
        {0, -f0.z, f0.y}, {0, -f1.z, f1.y}, {0, -f2.z, f2.y},
        {f0.z, 0, -f0.x}, {f1.z, 0, -f1.x}, {f2.z, 0, -f2.x},
        {-f0.y, f0.x, 0}, {-f1.y, f1.x, 0}, {-f2.y, f2.x, 0}
    };
    for (int i = 0; i < 9; i++) {
        if (!axisTest(axes[i])) return false;
    }

    // 3. Test axis: triangle normal
    Vec3 normal = Cross(f0, f1);
    if (!axisTest(normal)) return false;

    // 4. Test overlap in box space
    float minB[3] = {-e.x, -e.y, -e.z};
    float maxB[3] = { e.x,  e.y,  e.z};
    float minT[3] = {
        std::min({v0b.x, v1b.x, v2b.x}),
        std::min({v0b.y, v1b.y, v2b.y}),
        std::min({v0b.z, v1b.z, v2b.z})
    };
    float maxT[3] = {
        std::max({v0b.x, v1b.x, v2b.x}),
        std::max({v0b.y, v1b.y, v2b.y}),
        std::max({v0b.z, v1b.z, v2b.z})
    };

    if (maxT[0] < minB[0] || minT[0] > maxB[0]) return false;
    if (maxT[1] < minB[1] || minT[1] > maxB[1]) return false;
    if (maxT[2] < minB[2] || minT[2] > maxB[2]) return false;

    // No separating axis found → intersection
    return true;
}

// Applies Mat4 transformation to Vec3
inline Vec3 TransformPoint(const Vec3& v, const Mat4& m) {
    Vec3 result;
    result.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0];
    result.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1];
    result.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2];
    return result;
}

// Creates a transform matrix for 2D sprites (position + scale in NDC)
inline Mat4 MakeSpriteTransform(float x, float y, float sx, float sy, float screenW, float screenH)
{
    // Convert pixel coords to NDC (−1 to +1)
    float tx = (x / screenW) * 2.0f - 1.0f;
    float ty = 1.0f - (y / screenH) * 2.0f; // flip Y
    float sxNDC = (sx / screenW) * 2.0f;
    float syNDC = (sy / screenH) * 2.0f;

    Mat4 scale = Identity();
    scale.m[0][0] = sxNDC;
    scale.m[1][1] = syNDC;
    scale.m[2][2] = 1.0f;

    Mat4 translate = Identity();
    translate.m[3][0] = tx;
    translate.m[3][1] = ty;
    translate.m[3][2] = 0.0f;

    return Multiply(scale, translate);
}

inline AABB TransformAABB(const AABB& localAABB, const Mat4& transform) {
    // Extract all 8 corners of the local AABB
    Vec3 corners[8];
    corners[0] = { localAABB.min.x, localAABB.min.y, localAABB.min.z };
    corners[1] = { localAABB.max.x, localAABB.min.y, localAABB.min.z };
    corners[2] = { localAABB.min.x, localAABB.max.y, localAABB.min.z };
    corners[3] = { localAABB.min.x, localAABB.min.y, localAABB.max.z };
    corners[4] = { localAABB.max.x, localAABB.max.y, localAABB.min.z };
    corners[5] = { localAABB.max.x, localAABB.min.y, localAABB.max.z };
    corners[6] = { localAABB.min.x, localAABB.max.y, localAABB.max.z };
    corners[7] = { localAABB.max.x, localAABB.max.y, localAABB.max.z };

    // Transform them
    Vec3 minV = { FLT_MAX, FLT_MAX, FLT_MAX };
    Vec3 maxV = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

    for (int i = 0; i < 8; i++) {
        Vec3 worldPos = TransformPoint(corners[i], transform);
        minV.x = std::min(minV.x, worldPos.x);
        minV.y = std::min(minV.y, worldPos.y);
        minV.z = std::min(minV.z, worldPos.z);
        maxV.x = std::max(maxV.x, worldPos.x);
        maxV.y = std::max(maxV.y, worldPos.y);
        maxV.z = std::max(maxV.z, worldPos.z);
    }

    return { minV, maxV };
}