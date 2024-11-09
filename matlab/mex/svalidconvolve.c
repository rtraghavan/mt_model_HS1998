/*
  svalidconvolve.c

  Contains the implementation of the valid_filter function, which performs 3D convolution 
  with down-sampling on the input image using the specified filter.
  
  Author: Modernized Version
*/

#include <stdio.h>
#include <math.h>
#include "convolve.h"

/* 
 * Perform valid convolution of `image` with `filt`, applying down-sampling 
 * based on `x_step`, `y_step`, and `t_step` parameters. The result is stored in `result`.
 */
int valid_filter(const double *image, int x_idim, int y_idim, int t_idim,
                 const double *filt, int x_fdim, int y_fdim, int t_fdim,
                 int x_step, int y_step, int t_step, double *result) {
    
    int x, y, t, fx, fy, ft;
    int x_res_dim = (x_idim - x_fdim) / x_step + 1;
    int y_res_dim = (y_idim - y_fdim) / y_step + 1;
    int t_res_dim = (t_idim - t_fdim) / t_step + 1;
    
    /* Loop through each output element */
    for (t = 0; t < t_res_dim; t++) {
        for (y = 0; y < y_res_dim; y++) {
            for (x = 0; x < x_res_dim; x++) {
                double sum = 0.0;

                /* Apply the filter to the current region of the image */
                for (ft = 0; ft < t_fdim; ft++) {
                    for (fy = 0; fy < y_fdim; fy++) {
                        for (fx = 0; fx < x_fdim; fx++) {
                            int img_x = x * x_step + fx;
                            int img_y = y * y_step + fy;
                            int img_t = t * t_step + ft;

                            /* Calculate indices for accessing the image and filter */
                            double img_value = image[img_x + img_y * x_idim + img_t * x_idim * y_idim];
                            double filt_value = filt[fx + fy * x_fdim + ft * x_fdim * y_fdim];
                            sum += img_value * filt_value;
                        }
                    }
                }

                /* Store the result at the current position */
                result[x + y * x_res_dim + t * x_res_dim * y_res_dim] = sum;
            }
        }
    }

    return 0;  /* Return 0 to indicate successful execution */
}
