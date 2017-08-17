#include "math.h"
#include "stdio.h"
#include "stdlib.h"


extern void multigrid(double *, double *, int, int, int, int);
extern void output(double *, double *, int, char *);

double source(double x, double y) {
    return exp(-(pow(2*(x-.5),2) + 10*pow(2*(y-.5),2)));
    //return -.25*pow(-1 + x,2)*x*(9*cos(2 - 3*y) - 8*(1 + 2*y)*cos(2 - 2*y) + cos(2 - y) - 2*cos(y) + 16*sin(2 - 2*y)) - 2*(-2 + 3*x)*(-1 - 2*y + cos(y))*pow(sin(1 - y),2);
}

double potential(double x, double y) {
        return 0.0;
        //return pow(x - 1.,2.)*pow(sin(y - 1.),2.)*x*(2.*y + 1. - cos(y));
    }

int main(int argc, char *argv[]) {
        
        int lmax = atoi(argv[1]);
        int numpre = atoi(argv[2]);

        int n = 1 + (2 << lmax);
        int size = n*n;

        printf("%d points\n",n);
        printf("Using %d smoothing iterations\n",numpre);

        double dx = 1./(n-1);
        double dy = 1./(n-1);

        double *u = (double *)malloc(sizeof(double)*size);
        double *ans = (double *)malloc(sizeof(double)*size);
        double *res = (double *)malloc(sizeof(double)*size);

        double *x = (double *)malloc(sizeof(double)*n);
        double *y = (double *)malloc(sizeof(double)*n);


        int i,j;

        for(i=0;i<n;i++) {
            x[i] = i*dx;
            y[i] = i*dy;
        }


        for(i=0;i<n;i++) {
            for(j=0;j<n;j++) {
                u[j + i*n] = source(x[i],y[j]);
                ans[j + i*n] = 0.0; //potential(x[i],y[j]);
                res[j + i*n] = 0.0;
            }
        }

        FILE *f;

        f = fopen("grid.dat","w");
        fwrite(x,sizeof(double),n,f);
        fwrite(y,sizeof(double),n,f);
        fclose(f);
        f = fopen("source.dat","w");
        fwrite(u,sizeof(double),size,f);
        fclose(f);
        f = fopen("answer.dat","w");
        fwrite(ans,sizeof(double),size,f);
        fclose(f);


        multigrid(ans,u,lmax, 1 ,numpre,numpre);

        /*
    for(i=0;i<1+(2<<lmax);i++) {
        for(j=0;j<1+(2<<lmax);j++) {
            printf("%lg\t",ans[j+i*(1 + (2<<lmax))]);
        }
        printf("\n");
    }
    */
    
    f = fopen("result.dat","w");
    fwrite(ans,sizeof(double),size,f);
    fclose(f);

    double L2err = 0;
    int indx;

    for(i=0;i<n*n;i++) L2err += pow(res[i] - ans[i],2.);

    L2err = sqrt(L2err/(n*n));

    printf("L2error: %.5e\n",L2err);
    

    free(u);
    free(ans);
    free(x);
    free(y);
    free(res);
}
