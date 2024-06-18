#include "types.h"
#include "geometry.h"
#include "test_suite.h"
#include <cassert>

int main()
{
    Vector3 input1{0, -1, 0};   // Ray pointing downwards
    Vector3 normal1{0, 1, 0};   // Upward normal
    Vector3 expected1{0, 1, 0}; // Expected to reflect upwards
    EQUALS("Reflect a downward ray off a horizontal surface", expected1, reflectedVector(input1, normal1));

    Vector3 input2{1, 0, 0};    // Ray pointing right
    Vector3 normal2{0, 0, 1};   // Normal pointing outwards
    Vector3 expected2{1, 0, 0}; // Expected to reflect in the same direction
    EQUALS("Reflect a ray at an angle off a vertical surface", expected2, reflectedVector(input2, normal2));

    Vector3 input3{1, -1, 0};
    Vector3 normal3{0, 1, 0};
    Vector3 expected3{1, 1, 0}; // Should reflect upwards diagonally
    EQUALS("Diagonal reflection", expected3, reflectedVector(input3, normal3));

    Vector3 input4{1, 1, 0};     // Ray at a 45-degree angle
    Vector3 normal4{0, 1, 0};    // Upward normal
    Vector3 expected4{1, -1, 0}; // Reflected downwards at 45 degrees
    EQUALS("45-degree angle reflection", expected4, reflectedVector(input4, normal4));

    Vector3 input5{1, 2, 3};
    Vector3 normal5{1, 2, 3};      // Non-unit normal, should still reflect correctly
    Vector3 expected5{-1, -2, -3}; // Reflect directly back
    EQUALS("Reflection against non-unit normal", expected5, reflectedVector(input5, normal5));

    return 0;
}