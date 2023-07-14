#pragma once

constexpr double epsilon = 1.1102230246251565e-16;
constexpr double splitter = 134217729;
constexpr double resulterrbound = (3 + 8 * epsilon) * epsilon;

extern int sum(int elen, double e[], int flen, double f[], double h[]);
double sum_three(int alen, double a[], int blen, double b[], int clen, double c[], double tmp[], double out[]);
double scale(int elen, double e[], double b, double h[]);
void negate(int length, double e[]);
double estimate(int length, double e[]);
