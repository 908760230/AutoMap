#pragma once
#include <qvector.h>


// double seems to be faster than single or int's
// probably because most operations are in double precision
//const grad3 = /*#__PURE__*/ new Float64Array([1, 1, 0,
//    -1, 1, 0,
//    1, -1, 0,
//
//    -1, -1, 0,
//    1, 0, 1,
//    -1, 0, 1,
//
//    1, 0, -1,
//    -1, 0, -1,
//    0, 1, 1,
//
//    0, -1, 1,
//    0, 1, -1,
//    0, -1, -1]);

// double is a bit quicker here as well
//const grad4 = /*#__PURE__*/ new Float64Array([0, 1, 1, 1, 0, 1, 1, -1, 0, 1, -1, 1, 0, 1, -1, -1,
//    0, -1, 1, 1, 0, -1, 1, -1, 0, -1, -1, 1, 0, -1, -1, -1,
//    1, 0, 1, 1, 1, 0, 1, -1, 1, 0, -1, 1, 1, 0, -1, -1,
//    -1, 0, 1, 1, -1, 0, 1, -1, -1, 0, -1, 1, -1, 0, -1, -1,
//    1, 1, 0, 1, 1, 1, 0, -1, 1, -1, 0, 1, 1, -1, 0, -1,
//    -1, 1, 0, 1, -1, 1, 0, -1, -1, -1, 0, 1, -1, -1, 0, -1,
//    1, 1, 1, 0, 1, 1, -1, 0, 1, -1, 1, 0, 1, -1, -1, 0,
//    -1, 1, 1, 0, -1, 1, -1, 0, -1, -1, 1, 0, -1, -1, -1, 0]);

/**
 * A random() function, must return a number in the interval [0,1), just like Math.random().
 */
//export type RandomFn = () = > number;


//export type NoiseFunction2D = (x: number, y : number) = > number;

/**
 * Creates a 2D noise function
 * @param random the random function that will be used to build the permutation table
 * @returns {NoiseFunction2D}
 */

class Noise2D {
public:
    Noise2D(int seed);
    ~Noise2D(){}
    /**
 * Samples the noise field in two dimensions
 *
 * Coordinates should be finite, bigger than -2^31 and smaller than 2^31.
 * @param x
 * @param y
 * @returns a number in the interval [-1, 1]
 */
    double get(int x, int y) const;
private:
    QVector<int> m_permGrad2x;
    QVector<int> m_permGrad2y;
};


 /**
  * Creates a 3D noise function
  * @param random the random function that will be used to build the permutation table
  * @returns {NoiseFunction3D}
  */
class Noise3D {
public:
    Noise3D();
    ~Noise3D() {};
    /**
 * Samples the noise field in three dimensions
 *
 * Coordinates should be finite, bigger than -2^31 and smaller than 2^31.
 * @param x
 * @param y
 * @param z
 * @returns a number in the interval [-1, 1]
 */
    double get(int x, int y, int z);
};

/**
 * Creates a 4D noise function
 * @param random the random function that will be used to build the permutation table
 * @returns {NoiseFunction4D}
 */
class Noise4D
{
public:
    Noise4D();
    ~Noise4D() {};
    /**
 * Samples the noise field in four dimensions
 *
 * Coordinates should be finite, bigger than -2^31 and smaller than 2^31.
 * @param x
 * @param y
 * @param z
 * @param w
 * @returns a number in the interval [-1, 1]
 */
    double get(int x, int y, int z, int w);
};
