#include <stdio.h>
#include <math.h>

#define RECURS_LEVEL_MAX  10
#define N_MAX             10

// pointer to function of one variable
typedef double (*Func1vFp)(double);

// example functions of one variable
double f_poly(double x) { // polynomial a[0] + a[1]x + ... + a[n]x^n
	return 2 * pow(x, 5) - 4 * pow(x, 4) + 3.5 * pow(x, 2) + 1.35 * x - 6.25;
}

double f_rat(double x) {
	return 1 / (pow(x-0.5, 2) + 0.01);
}

double f_exp(double x) {
	return 2 * x * exp(-1.5 * x) - 1;
}

double f_trig(double x) {
	return x * tan(x);
}

// Quadratures
double quad_rect(Func1vFp f1, double a, double b, double c) {
	return (b - a) * f1(c);
}

// rectangle rule, leftpoint
double quad_rect_left(Func1vFp f1, double a, double b, int n) {
	double sum = 0;
	double xi = a;
	double h = (b-a)/n; 
	for (int i=0; i<n; i++) {
		sum += quad_rect(f1, xi, xi+h, xi);
		xi += h;
	}
	return sum;
}

// rectangle rule, rightpoint
double quad_rect_right(Func1vFp f1, double a, double b, int n) {
	double sum = 0;
	double xi = a;
	double h = (b-a)/n; 
	for (int i=0; i<n; i++) {
		sum += quad_rect(f1, xi, xi+h, xi+h);
		xi += h;
	}
	return sum;
}

// rectangle rule, midpoint
double quad_rect_mid(Func1vFp f1, double a, double b, int n) {
	double sum = 0;
	double xi = a;
	double h = (b-a)/n; 
	for (int i=0; i<n; i++) {
		sum += quad_rect(f1, xi, xi+h, (xi+xi+h)/2);
		xi += h;
	}
	return sum;
}

double quad_trap_sum(Func1vFp f1, double a, double b) {
	return ((b-a)/2)*(f1(a)+f1(b));
}
// trapezoidal rule
double quad_trap(Func1vFp f1, double a, double b, int n) {
	double sum = 0;
	double xi = a;
	double h = (b-a)/n; 
	for (int i=0; i<n; i++) {
		sum += quad_trap_sum(f1, xi, xi+h);
		xi += h;
	}
	return sum;
}

double quad_simp_sum(Func1vFp f1, double a, double b) {
	return ((b-a)/6)*(f1(a)+f1(b)+4*f1((a+b)/2));
}
// Simpson's rule
double quad_simpson(Func1vFp f1, double a, double b, int n) {
	double sum = 0;
	double xi = a;
	double h = (b-a)/n; 
	for (int i=0; i<n; i++) {
		sum += quad_simp_sum(f1, xi, xi+h);
		xi += h;
	}
	return sum;
}

// pointer to quadrature function
typedef double (*QuadratureFp)(Func1vFp, double, double, int);

// array of pointers to integrand functions
Func1vFp func_tab[] = { f_poly, f_rat, f_trig, f_exp };

// array of pointers to quadrature functions
QuadratureFp quad_tab[] = {
	quad_rect_left, quad_rect_right, quad_rect_mid, quad_trap, quad_simpson };

// calls 'quad_no' quadrature function for 'fun_no' integrand function
// on interval [a, b] and n subintervals
double quad_select(int fun_no, int quad_no, double a, double b, int n) {
	return quad_tab[quad_no](func_tab[fun_no], a, b, n);
}

// adaptive algorithm
double recurs(Func1vFp f, double a, double b, double S, double delta, QuadratureFp quad, int level) {
	if (level>RECURS_LEVEL_MAX)
		return NAN;

	double sum1 = quad(f, a, (a+b)/2, 1);
	double sum2 = quad(f, (a+b)/2, b, 1);
	if (fabs(S-(sum1+sum2))<=delta)
		return sum1+sum2;

	return recurs(f, a, (a+b)/2, sum1, delta/2, quad, level+1) + recurs(f, (a+b)/2, b, sum2, delta/2, quad, level+1);
}

// initialization for adaptive algorithm
double init_recurs(Func1vFp f, double a, double b, double delta, QuadratureFp quad) {
	return recurs(f, a, b, quad(f, a, b, 1), delta, quad, 0);
}

// double integrals

// pointer to function of two variables
typedef double (*Func2vFp)(double, double);

double func2v_2(double x, double y) {
	return 2 - x*x - y*y*y;
}

// sample functions to define the normal domain

double lower_bound2(double x) {
	return 0.7*exp(-2*x*x);
}
double upper_bound2(double x) {
	return sin(10*x);
}

// rectangle rule (leftpoint) - double integral over rectangular domain
double dbl_integr(Func2vFp f, double x1, double x2, int nx, double y1, double y2, int ny) {
	double sum = 0;
	double xi = x1;
	
	double hx = (x2-x1)/nx;
	double hy = (y2-y1)/ny;
	for (int i=0; i<nx; i++) {
		double yi = y1;
		for (int j=0; j<ny; j++) {
			sum += f(xi, yi);
			yi += hy;
		}
		xi += hx;
	}
	return sum*hx*hy;

}

// rectangle rule (midpoint) - double integral over normal domain with respect to the x-axis
double dbl_integr_normal_1(Func2vFp f, double x1, double x2, int nx, double hy,
						   Func1vFp fg, Func1vFp fh) {
	double sum = 0;
	double hx = (x2-x1)/nx;
	double xi, yi;
	for (int i=0; i<nx; i++) {
		xi = x1+(i+0.5)*hx;
		double hi = fh(xi);
		double gi = fg(xi);
		int ny = ceil((hi-gi)/hy);
		double step = (hi-gi)/ny;

		for (int j=0; j<ny; j++) {
			yi = gi+(j+0.5)*step;
			sum += f(xi, yi)*step;
		}
	}
	return sum*hx;
}

// rectangle rule (leftpoint) - double integral over multiple normal
// domains with respect to the x-axis
double dbl_integr_normal_n(Func2vFp f, double x1, double x2, int nx, double y1, double y2,
		int ny, Func1vFp fg, Func1vFp fh)  {
	double hx = (x2-x1)/nx;
	double hy = (y2-y1)/ny;
	double sum = 0; 
	double xi = x1;

    for(int i=0; i<nx; i++) {
       	double gi = fg(xi);
        double hi = fh(xi);
        double up = fmin(y2, hi);
        double down = fmax(y1, gi);

        int currny = ceil((up-down)/hy);
        double step = (up-down)/currny;

        double yi = fmax(y1, gi);

        for (int j=0; j<currny; j++) {
            sum += f(xi, yi)*step;
            yi += step;
        }
        xi += hx;
    }
    return sum*hx;
}

// multiple quadratures

typedef double (*FuncNvFp)(const double*, int);
typedef int (*BoundNvFp)(const double*, int);

// sample function of three variables
double func3v(const double v[], int n) {
	return v[0] - v[1] + 2*v[2];
}

// sample predicate in 3D
int bound3v(const double v[], int n) { // a cylinder
	return v[0] > 0 && v[0] < 0.5 && v[1]*v[1] + (v[2]-1)*(v[2]-1) < 1;
}

// sample function of n variables
double funcNv(const double v[], int n) {
	double fv = 1.;
	for (int i = 1; i < n; ++i) {
		fv += sin(i*v[i]);
	}
	return fv;
}
// sample n-dimensional predicate (n-dim hypersphere)
int boundNv(const double v[], int n) {
	double r = 0.0;
	for (int i = 0; i < n; ++i) r += (v[i]-1)*(v[i]-1);
	return r <= 1.;
}

void calculate_integral(FuncNvFp f, BoundNvFp boundary, double *sum, double variable_lim[][2], double dx[],double x[],
                        double position, const int intervals[], int level, int variable_no) {

    x[level] = variable_lim[level][0] + (dx[level] * position);
    for(int i=0;i<intervals[level];i++) {
        if(level==variable_no-1 && (boundary==NULL||boundary(x, variable_no)))
            *sum += f(x, variable_no);
		else if(level < variable_no-1)
            calculate_integral(f, boundary,sum, variable_lim, dx, x, position, intervals, level+1, variable_no);
        x[level] += dx[level];
    }
}
// multiple integrals over a n-dim hypercuboid with predicate (if boundary != NULL)
// rectangular rule (midpoint)
void recur_quad_rect_mid(double *p_sum, FuncNvFp f, int variable_no, double t_variable[],
         double variable_lim[][2], const int tn[], int level, double position, BoundNvFp boundary) {
	double x[variable_no];
    *p_sum = 0;
    for(int i=0;i<variable_no; i++){
        t_variable[i] = (variable_lim[i][1]-variable_lim[i][0])/tn[i];
    }
    calculate_integral(f, boundary, p_sum, variable_lim, t_variable,x, position, tn, level, variable_no);
    for(int j=0;j<variable_no;j++){
        *p_sum *= t_variable[j];
    }
}
// multiple integrals over a cuboid with predicate (if boundary != NULL)
// rectangular rule (rightpoint)
double trpl_quad_rect(FuncNvFp f, double variable_lim[][2], const int tn[], BoundNvFp boundary) {
	double sum = 0;
    double dx[3];
    for(int i=0;i<3;i++){
        dx[i] = (variable_lim[i][1] - variable_lim[i][0])/tn[i];
    }
    recur_quad_rect_mid(&sum, f, 3, dx, variable_lim, tn, 0, 1, boundary);
    return sum;
}

int main(void) {
	int to_do, n, nx, ny, integrand_fun_no, method_no, flag;
	int no_funcs = sizeof(func_tab) / sizeof(Func1vFp);
	int no_quads = sizeof(quad_tab) / sizeof(QuadratureFp);
	double a, b, x1, x2, y1, y2, hy, sum, delta;
	double t_variable[N_MAX], variable_lim[N_MAX][2];
	int tn[N_MAX];

	scanf("%d", &to_do);
	switch (to_do) {
		case 1: // loop over quadratures and integrands
			scanf("%lf %lf %d", &a, &b, &n);
			for(int q = 0; q < no_quads; ++q) {
				for(int f = 0; f < no_funcs; ++f) {
					printf("%.5f ",quad_select(f, q, a, b, n));
				}
				printf("\n");
			}
			break;
		case 2: // adaptive algorithm
			scanf("%d %d",&integrand_fun_no,&method_no);
			scanf("%lf %lf %lf", &a, &b, &delta);
			printf("%.5f\n", init_recurs(func_tab[integrand_fun_no],a,b,delta,quad_tab[method_no]));
			break;
		case 3: // double integral over a rectangle
			scanf("%lf %lf %d", &x1, &x2, &nx);
			scanf("%lf %lf %d", &y1, &y2, &ny);
			printf("%.5f\n", dbl_integr(func2v_2, x1, x2, nx, y1, y2, ny));
			break;
		case 4: // double integral over normal domain
			scanf("%lf %lf %d", &x1, &x2, &nx);
			scanf("%lf", &hy);
			printf("%.5f\n", dbl_integr_normal_1(func2v_2, x1, x2, nx, hy, lower_bound2, upper_bound2));
			break;
		case 5: // double integral over multiple normal domains
			scanf("%lf %lf %d", &x1, &x2, &nx);
			scanf("%lf %lf %d", &y1, &y2, &ny);
			printf("%.5f\n",dbl_integr_normal_n(func2v_2, x1, x2, nx, y1, y2, ny, lower_bound2, upper_bound2));
			break;
		case 6: // triple integral over a cuboid
			scanf("%lf %lf %d", &variable_lim[0][0], &variable_lim[0][1], tn);
			scanf("%lf %lf %d", &variable_lim[1][0], &variable_lim[1][1], tn+1);
			scanf("%lf %lf %d", &variable_lim[2][0], &variable_lim[2][1], tn+2);
			scanf("%d", &flag);
			printf("%.5f\n", trpl_quad_rect(func3v, variable_lim, tn, flag ? bound3v : NULL));
			break;
		case 7: // multiple integral over hyper-cuboid
			scanf("%d", &n);
			if(n > N_MAX) break;
			for(int i = 0; i < n; ++i) {
				scanf("%lf %lf %d", &variable_lim[i][0], &variable_lim[i][1], tn+i);
			}
			scanf("%d", &flag);
			sum = 0.;
			recur_quad_rect_mid(&sum, funcNv, n, t_variable, variable_lim, tn, 0, 0.5, flag ? boundNv : NULL);
			printf("%.5f\n", sum);
			break;
		default:
			printf("Nothing to do for %d\n", to_do);
			break;
	}
	return 0;
}

