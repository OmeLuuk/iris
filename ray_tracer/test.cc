#include "ray_tracer/types.h"
#include "ray_tracer/geometry.h"
#include <cassert>

int main()
{
    // Test case 1: Reflect a downward ray off a horizontal surface
    Vector3 input1{0, -1, 0};   // Ray pointing downwards
    Vector3 normal1{0, 1, 0};   // Upward normal
    Vector3 expected1{0, 1, 0}; // Expected to reflect upwards
    assert(expected1 == reflectedVector(input1, normal1));

    // Test case 2: Reflect a ray at an angle off a vertical surface
    Vector3 input2{1, 0, 0};    // Ray pointing right
    Vector3 normal2{0, 0, 1};   // Normal pointing outwards
    Vector3 expected2{1, 0, 0}; // Expected to reflect in the same direction
    assert(expected2 == reflectedVector(input2, normal2));

    // Test case 3: Diagonal reflection
    Vector3 input3{1, -1, 0};
    Vector3 normal3{0, 1, 0};
    Vector3 expected3{1, 1, 0}; // Should reflect upwards diagonally
    assert(expected3 == reflectedVector(input3, normal3));

    return 0;
}