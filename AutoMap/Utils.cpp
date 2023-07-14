#include "Utils.h"

// fast_expansion_sum_zeroelim routine from oritinal code
 int sum(int elen, double e[],int flen,double f[],double h[]) {
    double Q, Qnew, hh, bvirt;
    double enow = e[0];
    double fnow = f[0];
    int eindex = 0;
    int findex = 0;
    if ((fnow > enow) == (fnow > -enow)) {
        Q = enow;
        enow = e[++eindex];
    }
    else {
        Q = fnow;
        fnow = f[++findex];
    }
    int hindex = 0;
    if (eindex < elen && findex < flen) {
        if ((fnow > enow) == (fnow > -enow)) {
            Qnew = enow + Q;
            hh = Q - (Qnew - enow);
            enow = e[++eindex];
        }
        else {
            Qnew = fnow + Q;
            hh = Q - (Qnew - fnow);
            fnow = f[++findex];
        }
        Q = Qnew;
        if (hh != 0) {
            h[hindex++] = hh;
        }
        while (eindex < elen && findex < flen) {
            if ((fnow > enow) ==  (fnow > -enow)) {
                Qnew = Q + enow;
                bvirt = Qnew - Q;
                hh = Q - (Qnew - bvirt) + (enow - bvirt);
                enow = e[++eindex];
            }
            else {
                Qnew = Q + fnow;
                bvirt = Qnew - Q;
                hh = Q - (Qnew - bvirt) + (fnow - bvirt);
                fnow = f[++findex];
            }
            Q = Qnew;
            if (hh != 0) {
                h[hindex++] = hh;
            }
        }
    }
    while (eindex < elen) {
        Qnew = Q + enow;
        bvirt = Qnew - Q;
        hh = Q - (Qnew - bvirt) + (enow - bvirt);
        enow = e[++eindex];
        Q = Qnew;
        if (hh != 0) {
            h[hindex++] = hh;
        }
    }
    while (findex < flen) {
        Qnew = Q + fnow;
        bvirt = Qnew - Q;
        hh = Q - (Qnew - bvirt) + (fnow - bvirt);
        fnow = f[++findex];
        Q = Qnew;
        if (hh != 0) {
            h[hindex++] = hh;
        }
    }
    if (Q != 0 || hindex == 0) {
        h[hindex++] = Q;
    }
    return hindex;
}

 double sum_three(int alen, double a[], int blen, double b[], int clen, double c[], double tmp[], double out[]) {
    return sum(sum(alen, a, blen, b, tmp), tmp, clen, c, out);
}

// scale_expansion_zeroelim routine from oritinal code
double scale(int elen,double e[],double b,double h[]) {
    double Q, sum, hh, product1, product0;
    double bvirt, c, ahi, alo, bhi, blo;

    c = splitter * b;
    bhi = c - (c - b);
    blo = b - bhi;
    double enow = e[0];
    Q = enow * b;
    c = splitter * enow;
    ahi = c - (c - enow);
    alo = enow - ahi;
    hh = alo * blo - (Q - ahi * bhi - alo * bhi - ahi * blo);
    int hindex = 0;
    if (hh != 0) {
        h[hindex++] = hh;
    }
    for (int i = 1; i < elen; i++) {
        enow = e[i];
        product1 = enow * b;
        c = splitter * enow;
        ahi = c - (c - enow);
        alo = enow - ahi;
        product0 = alo * blo - (product1 - ahi * bhi - alo * bhi - ahi * blo);
        sum = Q + product0;
        bvirt = sum - Q;
        hh = Q - (sum - bvirt) + (product0 - bvirt);
        if (hh != 0) {
            h[hindex++] = hh;
        }
        Q = product1 + sum;
        hh = sum - (Q - product1);
        if (hh != 0) {
            h[hindex++] = hh;
        }
    }
    if (Q != 0 || hindex == 0) {
        h[hindex++] = Q;
    }
    return hindex;
}

void negate(int length, double e[]) {
    for (int i = 0; i < length; i++) e[i] = -e[i];
}

double estimate(int length, double e[]) {
    double Q = 0;
    for (int i = 0; i < length; i++) Q += e[i];
    return Q;
}