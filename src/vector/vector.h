#pragma once

typedef struct Vector {
    float x;
    float y;
} Vector;

Vector VectorAdd(Vector a, Vector b);
Vector VectorSub(Vector a, Vector b);
Vector VectorMul(Vector a, float scalar);
Vector VectorDiv(Vector a, float scalar);
float VectorDot(Vector a, Vector b);
float VectorLenSq(Vector a);
float VectorLen(Vector a);
Vector VectorNormalized(Vector a);
