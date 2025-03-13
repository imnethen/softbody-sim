#include <SDL3/SDL_stdinc.h>
#include "vector.h"

Vector VectorAdd(Vector a, Vector b) {
    return (Vector){ a.x + b.x, a.y + b.y };
}

Vector VectorSub(Vector a, Vector b) {
    return (Vector){ a.x - b.x, a.y - b.y };
}

Vector VectorMul(Vector a, float scalar) {
    return (Vector){ a.x * scalar, a.y * scalar };
}

Vector VectorDiv(Vector a, float scalar) {
    return (Vector){ a.x / scalar, a.y / scalar };
}

float VectorDot(Vector a, Vector b) {
    return a.x * b.x + a.y * b.y;
}

float VectorLenSq(Vector a) {
    return VectorDot(a, a);
}

float VectorLen(Vector a) {
    return SDL_sqrtf(VectorLenSq(a));
}

// returns nan if zero vector
Vector VectorNormalized(Vector a) {
    return VectorDiv(a, VectorLen(a));
}
