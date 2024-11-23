#include "m/mat/dmatn.h"
#include "m/vec/dvecn.h"


double dmatn_det(const double *m, int n)
{
    assert(n>=2 && n<=4);
    
    if(n==2) {
        // cast in matrix[2][2]
        const double (*mm)[2] = (const double (*)[2])m;
        
        // from cglm/mat2.h/glm_mat2_det
        return mm[0][0] * mm[1][1] - mm[1][0] * mm[0][1];
    }
    
    if(n==3) {
        // cast in matrix[3][3]
        const double (*mm)[3] = (const double (*)[3])m;
        
        // from cglm/mat3.h/glm_mat3_det
        double a = mm[0][0], b = mm[0][1], c = mm[0][2];
        double d = mm[1][0], e = mm[1][1], f = mm[1][2];
        double g = mm[2][0], h = mm[2][1], i = mm[2][2];

        return a * (e * i - h * f) - d * (b * i - c * h) + g * (b * f - c * e);
    }
    
    if(n == 4) {
        // cast in matrix[4][4]
        const double (*mm)[4] = (const double (*)[4])m;

        // from cglm/mat4.h/glm_mat4_det
        /* [square] det(A) = det(At) */
        double t[6];
        double a = mm[0][0], b = mm[0][1], c = mm[0][2], d = mm[0][3],
                e = mm[1][0], f = mm[1][1], g = mm[1][2], h = mm[1][3],
                i = mm[2][0], j = mm[2][1], k = mm[2][2], l = mm[2][3],
                m = mm[3][0], n = mm[3][1], o = mm[3][2], p = mm[3][3];

        t[0] = k * p - o * l;
        t[1] = j * p - n * l;
        t[2] = j * o - n * k;
        t[3] = i * p - m * l;
        t[4] = i * o - m * k;
        t[5] = i * n - m * j;

        return a * (f * t[0] - g * t[1] + h * t[2])
               - b * (e * t[0] - g * t[3] + h * t[4])
               + c * (e * t[1] - f * t[3] + h * t[5])
               - d * (e * t[2] - f * t[4] + g * t[5]);
    }

    assert(0);
    return 0;
}


void dmatn_inv(double *out_inv, const double * restrict m, int n)
{
    assert(n>=2 && n<=4);
    
    if(n == 2) {
        // cast in matrix[2][2]
        const double (*mm)[2] = (const double (*)[2])m;
        double (*res)[2] = (double (*)[2])out_inv;
        
        // from cglm/mat2.h/glm_mat2_inv
        double a = mm[0][0], b = mm[0][1];
        double c = mm[1][0], d = mm[1][1];

        res[0][0] = d;
        res[0][1] = -b;
        res[1][0] = -c;
        res[1][1] = a;

        double det = a * d - b * c;
        dvecn_div(out_inv, out_inv, det, 4);

        return;
    }
    
    if(n == 3) {
        // cast in matrix[3][3]
        const double (*mm)[3] = (const double (*)[3])m;
        double (*res)[3] = (double (*)[3])out_inv;

        // from cglm/mat3.h/glm_mat3_inv
        double a = mm[0][0], b = mm[0][1], c = mm[0][2];
        double d = mm[1][0], e = mm[1][1], f = mm[1][2];
        double g = mm[2][0], h = mm[2][1], i = mm[2][2];

        res[0][0] = e * i - f * h;
        res[0][1] = -(b * i - h * c);
        res[0][2] = b * f - e * c;
        res[1][0] = -(d * i - g * f);
        res[1][1] = a * i - c * g;
        res[1][2] = -(a * f - d * c);
        res[2][0] = d * h - g * e;
        res[2][1] = -(a * h - g * b);
        res[2][2] = a * e - b * d;

        double det = a * res[0][0] + b * res[1][0] + c * res[2][0];
        dvecn_div(out_inv, out_inv, det, 9);

        return;
    }

    if(n == 4) {
        // cast in matrix[4][4]
        const double (*mm)[4] = (const double (*)[4])m;
        double (*res)[4] = (double (*)[4])out_inv;

        // from cglm/mat4.h/glm_mat4_inv
        double t[6];
        double a = mm[0][0], b = mm[0][1], c = mm[0][2], d = mm[0][3];
        double e = mm[1][0], f = mm[1][1], g = mm[1][2], h = mm[1][3];
        double i = mm[2][0], j = mm[2][1], k = mm[2][2], l = mm[2][3];
        double m = mm[3][0], n = mm[3][1], o = mm[3][2], p = mm[3][3];

        t[0] = k * p - o * l;
        t[1] = j * p - n * l;
        t[2] = j * o - n * k;
        t[3] = i * p - m * l;
        t[4] = i * o - m * k;
        t[5] = i * n - m * j;

        res[0][0] = f * t[0] - g * t[1] + h * t[2];
        res[1][0] = -(e * t[0] - g * t[3] + h * t[4]);
        res[2][0] = e * t[1] - f * t[3] + h * t[5];
        res[3][0] = -(e * t[2] - f * t[4] + g * t[5]);

        res[0][1] = -(b * t[0] - c * t[1] + d * t[2]);
        res[1][1] = a * t[0] - c * t[3] + d * t[4];
        res[2][1] = -(a * t[1] - b * t[3] + d * t[5]);
        res[3][1] = a * t[2] - b * t[4] + c * t[5];

        t[0] = g * p - o * h;
        t[1] = f * p - n * h;
        t[2] = f * o - n * g;
        t[3] = e * p - m * h;
        t[4] = e * o - m * g;
        t[5] = e * n - m * f;

        res[0][2] = b * t[0] - c * t[1] + d * t[2];
        res[1][2] = -(a * t[0] - c * t[3] + d * t[4]);
        res[2][2] = a * t[1] - b * t[3] + d * t[5];
        res[3][2] = -(a * t[2] - b * t[4] + c * t[5]);

        t[0] = g * l - k * h;
        t[1] = f * l - j * h;
        t[2] = f * k - j * g;
        t[3] = e * l - i * h;
        t[4] = e * k - i * g;
        t[5] = e * j - i * f;

        res[0][3] = -(b * t[0] - c * t[1] + d * t[2]);
        res[1][3] = a * t[0] - c * t[3] + d * t[4];
        res[2][3] = -(a * t[1] - b * t[3] + d * t[5]);
        res[3][3] = a * t[2] - b * t[4] + c * t[5];

        double det = a * res[0][0] + b * res[1][0] + c * res[2][0] + d * res[3][0];
        dvecn_div(out_inv, out_inv, det, 16);

        return;
    }
}
