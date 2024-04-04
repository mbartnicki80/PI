#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <math.h>

typedef struct _matrix{
    int rows;
    int cols;
    double**data;
} Matrix;

int get(Matrix *pm, int row, int col, double *pvalue) {
    if (row>=pm->rows||col>=pm->cols||row<0||col<0||pm->data==NULL)
        return 0;
    *pvalue = pm->data[row][col];
    return 1;
}

int set(Matrix *pm, int row, int col, double value) {
    if (row>=pm->rows||col>=pm->cols||row<0||col<0||pm->data==NULL)
        return 0;
    pm->data[row][col] = value;
    return 1;
}

void create_identity_matrix(Matrix*pm, int size) {
    pm->rows = size;
    pm->cols = size;
    pm->data = (double **)calloc((size*size), sizeof(double));
    for (int i=0; i<size; i++)
        pm->data[i][i] = 1;
}

double dot_prod_vect(const double *v1, const double *v2, int size) {
    double suma = 0;
    for (int i=0; i<size; i++)
        suma += v1[i] * v2[i];
    return suma;
}

void mul_vect(double *result, const Matrix *pm, const double *vect) {
    for (int i=0; i<pm->rows; i++)
        result[i] = dot_prod_vect(pm->data[i], vect, pm->cols);
}

void max_element_in_rows(double *maxdata, const Matrix *pm) {
    for (int i=0; i<pm->rows; i++) {
        double maxi = pm->data[i][0];
        for (int j=1; j<pm->cols; j++)
            if (pm->data[i][j]>maxi)
                maxi = pm->data[i][j];
        maxdata[i] = maxi;
    }
}

void print_row(double *row, size_t n) {
    for (int i=0; i<n; i++)
        printf("%d ", row[i]);
}

int compar(const void *a, const void *b) {
    int fa = *(int *)a;
	int fb = *(int *)b;
    if (fabs(fa-fb<=0.01))
        return 0;
	return fa-fb;
}

void find_row(Matrix M, double key) {
    double *ptr = bsearch(&key, M.data, M.rows, sizeof(double), compar);
    if (ptr==NULL)
        return;
    print_row(ptr, M.cols);
}



int main(void) {

}