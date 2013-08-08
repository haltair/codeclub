/*------------------------------This is a MATLAB MEX function----------------------------------------
 
  [IMG_OUT] = MEX_COMPUTE_IMGHESSIAN(IMG_IN,FIXED_PARAMS)
  
  Description:
  This function returns the hessian at each point in an image
  
  Input :  IMG_IN        --> 2D array image (LENGTH X WIDTH)
           FIXED_PARAMS  --> 1D array of parameters [LENGTH WIDTH h_x h_y]
	
  Output:  IMG_OUT  --> 2D array image hessian ((LENGTH*WIDTH) X 3) 
 --------------------------------------------------------------------------------------------------*/
#include<stdio.h>
#include<math.h>

/*Change the following line based on the PATH/TO/MEX.H on your system*/
#include"mex.h"

#include"helper.h"

/*Input Arguments*/
#define IMG_IN          prhs[0]
#define FIXED_PARAMS    prhs[1]

/*Output Arguments*/
#define IMG_OUT         plhs[0]

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif


void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
  int LENGTH; int WIDTH;
  double *img_in_ptr;double *params_ptr; 
  double *temp_img_ptr;
  double *img_out_ptr;
  int  i, j, count, N;
  double h_x, h_y;

  /*Get the dimensions*/
  params_ptr = mxGetPr(FIXED_PARAMS);
  LENGTH   = (int)*(params_ptr);
  WIDTH    = (int)*(params_ptr+1);
  h_x      = *(params_ptr+2);
  h_y      = *(params_ptr+3);

  N = LENGTH*WIDTH;
  
  /*get IMG_IN*/
  img_in_ptr = mxGetPr(IMG_IN);
  
  /*create the Output matrix*/
  IMG_OUT = mxCreateDoubleMatrix(N,3,mxREAL);
  img_out_ptr = mxGetPr(IMG_OUT);


  /*create image with ghost pixels*/
  temp_img_ptr = mxGetPr(mxCreateDoubleMatrix(LENGTH+2,WIDTH+2,mxREAL));
  

  for (i = 0;i<LENGTH+2;i++)
    {
      for (j = 0;j<WIDTH+2;j++)
	{
	  *(temp_img_ptr + j*(LENGTH+2) + i) = 0;
	}
    }

  
  for (i = 1;i<=LENGTH;i++)
    {
      for (j = 1;j<=WIDTH;j++)
	{
	  *(temp_img_ptr + j*(LENGTH+2) + i) = *(img_in_ptr + (j-1)*LENGTH + (i-1));
	}
    }
  
  for (j = 1;j<=WIDTH;j++)
    {
      *(temp_img_ptr + j*(LENGTH+2) + 0)        =  *(temp_img_ptr + j*(LENGTH+2) + 1);
      *(temp_img_ptr + j*(LENGTH+2) + LENGTH+1) =  *(temp_img_ptr + j*(LENGTH+2) + LENGTH);
    }
  
  for (i = 1;i<=LENGTH;i++)
    {
      *(temp_img_ptr + 0*(LENGTH+2) + i)         =  *(temp_img_ptr + 1*(LENGTH+2) + i);
      *(temp_img_ptr + (WIDTH+1)*(LENGTH+2) + i) =   *(temp_img_ptr + (WIDTH)*(LENGTH+2) + i);
    }
  
  /*compute hessian components*/
  count = 0;  
  for (j = 1;j < WIDTH+1;j++)
    {
      for (i = 1;i < LENGTH+1;i++)
	{
	  *(img_out_ptr + N*0 + count) = (*(temp_img_ptr + j*(LENGTH+2) + i+1) - 2*(*(temp_img_ptr + j*(LENGTH+2) + i)) 
                                          + *(temp_img_ptr + j*(LENGTH+2) + i-1))/(h_x*h_x);
	  
	  *(img_out_ptr + N*1 + count) = 0.5*(((*(temp_img_ptr + j*(LENGTH+2) + i+1) 
                                              - *(temp_img_ptr + j*(LENGTH+2) + i)
					      - *(temp_img_ptr + (j-1)*(LENGTH+2) + i+1) 
                                              + *(temp_img_ptr + (j-1)*(LENGTH+2) + i))/(h_x*h_y)) 
                                              + ((*(temp_img_ptr + (j+1)*(LENGTH+2) + i) 
                                              - *(temp_img_ptr + j*(LENGTH+2) + i)
					      - *(temp_img_ptr + (j+1)*(LENGTH+2) + i-1) 
                                              + *(temp_img_ptr + j*(LENGTH+2) + i-1))/(h_x*h_y)));
	  
	  *(img_out_ptr + N*2 + count) = (*(temp_img_ptr + (j+1)*(LENGTH+2) + i) - 2*(*(temp_img_ptr + j*(LENGTH+2) + i))                                               + *(temp_img_ptr + (j-1)*(LENGTH+2) + i))/(h_y*h_y);
	  
	  count = count + 1;
	}
    }
}
