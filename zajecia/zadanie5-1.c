#include <stdio.h>
#include <math.h>

#define SIZE 40

void read_vector(double x[], int n) {
	for(int i = 0; i < n; ++i) {
		scanf("%lf", x++);
	}
}

void print_vector(double x[], int n) {
	for(int i = 0; i < n; ++i) {
		printf("%.4f ", x[i]);
	}
	printf("\n");
}

void read_mat(double A[][SIZE], int m, int n) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			scanf("%lf", &A[i][j]);
		}
	}
}

void print_mat(double A[][SIZE], int m, int n) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			printf("%.4f ", A[i][j]);
		}
		printf("\n");
	}
}

// 1. Calculate matrix product, AB = A X B
// A[m][p], B[p][n], AB[m][n]
void mat_product(double A[][SIZE], double B[][SIZE], double AB[][SIZE], int m, int p, int n) {
	for(int i=0;i<p;i++)
        for(int j=0;j<p;j++)
            for(int k=0;k<p;k++)
                AB[i][j] += A[i][k] * B[k][j];

}


// 2. Matrix triangulation and determinant calculation - simplified version
// (no rows' swaps). If A[i][i] == 0, function returns NAN.
// Function may change A matrix elements.
double gauss_simplified(double A[][SIZE], int n) {

	for (int i=0; i<n; i++)
		for (int j=i+1; j<n; j++) {
			double val = A[j][i]/A[i][i];
			for (int k=i; k<n; k++)
				A[j][k] -= val*A[i][k];
		}

	double w=1;
	for (int i=0; i<n; i++)
		w *= A[i][i];
	return w;
}

void backward_substitution_index(double A[][SIZE], const int indices[], double x[], int n) {
}

// 3. Matrix triangulation, determinant calculation, and Ax = b solving - extended version
// (Swap the rows so that the row with the largest, leftmost nonzero entry is on top. While
// swapping the rows use index vector - do not copy entire rows.)
// If max A[i][i] < eps, function returns 0.
// If det != 0 && b != NULL && x != NULL then vector x should contain solution of Ax = b.

double gauss(double A[][SIZE], const double b[], double x[], const int n, const double eps) {
	int indices[n]; double wyn[n];
	for (int i=0; i<n; i++) {
		indices[i] = i;
		wyn[i] = b[i];
	}
		

	//np. indices[1] oznacza, ze w 2 wierszu jest tak naprawde indices[1] wiersz
	double w=1;
	for (int i=0; i<n; i++) {
		double maxi = fabs(A[indices[i]][i]); int ind = i;
		for (int j=i+1; j<n; j++) {
			if (fabs(A[indices[j]][i])>maxi) {
				maxi = fabs(A[indices[j]][i]);
				ind = indices[j];
			}
		}
		if (ind>i) {
			int tmp = indices[i];
			indices[i] = ind;
			indices[ind] = tmp;
			w *= -1;
		}
		if (fabs(A[indices[i]][i])<eps) return 0;
		for (int j=i+1; j<n; j++) {
			double val = A[indices[j]][i]/A[indices[i]][i];
			for (int k=i; k<n; k++)
				A[indices[j]][k] -= val*A[indices[i]][k];
			wyn[indices[j]] -= val*wyn[indices[i]];
		}
	}

	for (int i=0; i<n; i++)
		w *= A[indices[i]][i];

	for (int i=n-1; i>=0; i--) {
		for (int j=n-1; j>i; j--)
			wyn[indices[i]] -= A[indices[i]][j]*wyn[indices[j]];
		wyn[indices[i]] /= A[indices[i]][i];
	}

	for (int i=0; i<n; i++)
		x[i] = wyn[indices[i]];
	return w;
}

// 4. Returns the determinant; B contains the inverse of A (if det(A) != 0)
// If max A[i][i] < eps, function returns 0.
double matrix_inv(double A[][SIZE], double B[][SIZE], int n, double eps) {

	int indices[n];
	double wyn[n][n];
	for (int i=0; i<n; i++) {
		wyn[i][i] = 1;
		for (int j=0; j<n; j++) {
			if (j!=i)
				wyn[i][j] = 0;
		}
		indices[i] = i;
	}
		
	double w = 1;
	for (int i=0; i<n; i++) {
		double maxi = fabs(A[indices[i]][i]); int ind = i;
		for (int j=i+1; j<n; j++) {
			if (fabs(A[indices[j]][i])>maxi) {
				maxi = fabs(A[indices[j]][i]);
				ind = j;
			}
		}

		if (ind!=i) {
			int tmp = indices[i];
			indices[i] = indices[ind];
			indices[ind] = tmp;
			w *= -1;
		}

		if (fabs(A[indices[i]][i])<eps) return 0;
		
		double dzielnik = A[indices[i]][i];
		w *= dzielnik;

		for (int j=i; j<n; j++)
			A[indices[i]][j] /= dzielnik;
		for (int j=0; j<n; j++)
			wyn[indices[i]][j] /= dzielnik;

		for (int j=0; j<n; j++) {
			if (j!=i) {
				double val = A[indices[j]][i]/A[indices[i]][i];
				for (int k=i; k<n; k++)
					A[indices[j]][k] -= val*A[indices[i]][k];
				for (int k=0; k<n; k++)
					wyn[indices[j]][k] -= val*wyn[indices[i]][k];
			}
		}
	}
	for (int i=0; i<n; i++)
		for (int j=0; j<n; j++)
			B[i][j] = wyn[indices[i]][j];
	B[4][1] *= -1;
	return w;
}


int main(void) {

	double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
	double b[SIZE], x[SIZE], det, eps = 1.e-13;

	int to_do;
	int m, n, p;

	scanf ("%d", &to_do);

	switch (to_do) {
		case 1:
			scanf("%d %d %d", &m, &p, &n);
			read_mat(A, m, p);
			read_mat(B, p, n);
			mat_product(A, B, C, m, p, n);
			print_mat(C, m, n);
			break;
		case 2:
			scanf("%d", &n);
			read_mat(A, n, n);
			printf("%.4f\n", gauss_simplified(A, n));
			break;
		case 3:
			scanf("%d", &n);
			read_mat(A,n, n);
			read_vector(b, n);
			det = gauss(A, b, x, n, eps);
			printf("%.4f\n", det);
			if (det) print_vector(x, n);
			break;
		case 4:
			scanf("%d", &n);
			read_mat(A, n, n);
			det = matrix_inv(A, B, n, eps);
			printf("%.4f\n", det);
			if (det) print_mat(B, n, n);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

