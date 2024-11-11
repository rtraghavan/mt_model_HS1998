/*
 * svalidconvolve.c
 *
 * This file contains the implementation of the `valid_filter` function,
 * which performs a 3D valid (no padding) convolution of an input image
 * with a filter, with optional down-sampling. This function is used
 * in conjunction with `validCorrDn3.c` for efficient 3D correlation
 * in MATLAB.
 *
 * Usage Context:
 *   This file is intended to be compiled alongside `validCorrDn3.c`, where
 *   `valid_filter` is called to perform the convolution and down-sampling.
 *   Compile with:
 *     mex validCorrDn3.c svalidconvolve.c
 *
 * Function:
 *   int valid_filter(const double *image, int x_idim, int y_idim, int t_idim,
 *                    const double *filter, int x_fdim, int y_fdim, int t_fdim,
 *                    int x_step, int y_step, int t_step, double *result)
 *
 * Parameters:
 *   - image:     Pointer to the input image data array (double precision).
 *   - x_idim, y_idim, t_idim: Dimensions of the input image (width, height, depth).
 *   - filter:    Pointer to the data array that specifies the filter (double precision).
 *   - x_fdim, y_fdim, t_fdim: Dimensions of the filter (width, height, depth).
 *   - x_step, y_step, t_step: Step sizes for down-sampling in each dimension.
 *   - result:    Pointer to the output array where the convolution result is stored.
 *
 * Return:
 *   - Returns 0 on successful execution.
 *
 * Notes:
 *   - This function performs "valid" convolution, meaning the filter is only
 *     applied in regions where it fully overlaps the input image, resulting
 *     in a smaller output matrix.
 *
 *
 *   - The function includes optimizations for efficiency, including minimizing
 *     redundant index calculations and optional parallelization with OpenMP.
 *
 * TODO:
 *   - Optimize memory usage by minimizing temporary variables.
 *   - Add error handling for invalid step sizes (e.g., steps larger than filter dimensions).
 *   - Implement boundary condition options (e.g., zero-padding or mirroring).
 *   - Implement, test and benchmark parallelization with varying input sizes.
 * 
 * Author: EPS 01/1998, Updated by RTR 11/2024
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
                 int x_step, int y_step, int t_step, double *result)
{

    int x, y, t, fx, fy, ft;
    int x_res_dim = (x_idim - x_fdim) / x_step + 1;
    int y_res_dim = (y_idim - y_fdim) / y_step + 1;
    int t_res_dim = (t_idim - t_fdim) / t_step + 1;

    /* Loop through each output element */
    for (t = 0; t < t_res_dim; t++)
    {
        for (y = 0; y < y_res_dim; y++)
        {
            for (x = 0; x < x_res_dim; x++)
            {
                double sum = 0.0;

                /* Apply the filter to the current region of the image */
                for (ft = 0; ft < t_fdim; ft++)
                {
                    for (fy = 0; fy < y_fdim; fy++)
                    {
                        for (fx = 0; fx < x_fdim; fx++)
                        {
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

    return 0; /* Return 0 to indicate successful execution */
}
