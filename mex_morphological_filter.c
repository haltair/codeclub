/*------------------------------This is a MATLAB MEX function----------------------------------------
 
  [IMG_OUT] = MEX_MORPHOLOGICAL_FILTER(IMG_IN,FLAG,FIXED_PARAMS)
  
  Description:
  This function performs one iteration of  erosion (dilation) at each pixel based on the 
  sign (related to shock filtering)
  
  Input :  IMG_IN        --> 2D array image (LENGTH X WIDTH)
           FIXED_PARAMS  --> 1D array of parameters [LENGTH WIDTH h_x h_y tau]
	   FLAG          --> 1D array of signs at each pixel (LENGTH*WIDTH)  
                                      -1 (erosion), 1 (dilation)                   
         
  Output:  IMG_OUT       --> 2D array image eroded (dilated) (LENGTH X WIDTH) 
 --------------------------------------------------------------------------------------------------*/
#include<stdio.h>
#include<math.h>

/*Change the following line based on the PATH/TO/MEX.H on your system*/
#include"mex.h"

#include"helper.h"

/*Input Arguments*/
#define IMG_IN          prhs[0]
#define FLAG            prhs[1]
#define FIXED_PARAMS    prhs[2]

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
  double *img_in_ptr;double *flag_ptr;double *params_ptr; 
  double *temp_img_ptr;
  double *img_out_ptr;
  double tau, h_x, h_y;
  int  i, j, count;
  double temp1, temp2, temp3, temp4;

  /*Get the dimensions*/
  params_ptr = mxGetPr(FIXED_PARAMS);
  LENGTH   = (int)*(params_ptr);
  WIDTH    = (int)*(params_ptr+1);
  h_x      = *(params_ptr+2);
  h_y      = *(params_ptr+3);
  tau      = *(params_ptr+4);
  
  /*get IMG_IN*/
  img_in_ptr = mxGetPr(IMG_IN);
  
  /*get FLAG*/
  flag_ptr = mxGetPr(FLAG);

  /*create the Output matrix*/
  IMG_OUT = mxCreateDoubleMatrix(LENGTH,WIDTH,mxREAL);
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
      *(temp_img_ptr + j*(LENGTH+2) + 0) =        *(temp_img_ptr + j*(LENGTH+2) + 1);
      *(temp_img_ptr + j*(LENGTH+2) + LENGTH+1) = *(temp_img_ptr + j*(LENGTH+2) + LENGTH);
     }
  
  for (i = 1;i<=LENGTH;i++)
    {
      *(temp_img_ptr + 0*(LENGTH+2) + i) = *(temp_img_ptr + 1*(LENGTH+2) + i); 
      *(temp_img_ptr + (WIDTH+1)*(LENGTH+2) + i) = *(temp_img_ptr + (WIDTH)*(LENGTH+2) + i); 
    }
  
  count = 0;
  
  for (j = 1;j<WIDTH+1;j++)
    {
      for (i = 1;i<LENGTH+1;i++)
	{
	  temp1 =  (*(temp_img_ptr + j*(LENGTH+2) + i) - *(temp_img_ptr + j*(LENGTH+2) + i-1))/h_x;
	  temp2 =  (*(temp_img_ptr + j*(LENGTH+2) + i+1) - *(temp_img_ptr + j*(LENGTH+2) + i))/h_x;
	  temp3 =  (*(temp_img_ptr + j*(LENGTH+2) + i) - *(temp_img_ptr + (j-1)*(LENGTH+2) + i))/h_y;
	  temp4 =  (*(temp_img_ptr + (j+1)*(LENGTH+2) + i) - *(temp_img_ptr + j*(LENGTH+2) + i))/h_y;
	  	  
	  switch ((int)(*(flag_ptr + count))){
	    
	  case -1:  /*Erosion*/
	    
	    *(img_out_ptr + (j-1)*LENGTH + (i-1)) = *(temp_img_ptr + j*(LENGTH+2) + i) - tau * 
	      pow((pow(max(temp1,0),2.0) +
		   pow(min(temp2,0),2.0) +
		   pow(max(temp3,0),2.0) +
		   pow(min(temp4,0),2.0)
		   ),0.5);  
	    
	    break;
	    
	  case 1:   /*Dilation*/
	    
	    *(img_out_ptr + (j-1)*LENGTH + (i-1)) = *(temp_img_ptr + j*(LENGTH+2) + i) + tau * 
	      pow((pow(min(temp1,0),2.0) +
		   pow(max(temp2,0),2.0) +
		   pow(min(temp3,0),2.0) +
		   pow(max(temp4,0),2.0)
		   ),0.5);  
	    break;

	  case 0:
	    *(img_out_ptr + (j-1)*LENGTH + (i-1)) =  *(img_in_ptr + (j-1)*LENGTH + (i-1));
	    break;
	  }
	  
	  count = count + 1;
	}
    }
}
