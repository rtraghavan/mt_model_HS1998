/*
  convolve.h

  Header file for convolution functions.
  Defines the prototype for the valid_filter function used in convolution and down-sampling.
  
  Author: Modernized Version
*/

#ifndef CONVOLVE_H
#define CONVOLVE_H

#include <stddef.h>

/* 
 * Performs a valid convolution on a 3D image with a filter, 
 * down-sampling the result by the specified step values.
 * 
 * Parameters:
 *  - image: Pointer to the input image data.
 *  - x_idim, y_idim, t_idim: Dimensions of the input image (width, height, depth).
 *  - filt: Pointer to the filter data.
 *  - x_fdim, y_fdim, t_fdim: Dimensions of the filter (width, height, depth).
 *  - x_step, y_step, t_step: Step sizes for down-sampling in each dimension.
 *  - result: Pointer to the output array for storing the convolution result.
 * 
 * Returns:
 *  - 0 on success, or a non-zero error code on failure.
 */
int valid_filter(const double *image, int x_idim, int y_idim, int t_idim,
                 const double *filt, int x_fdim, int y_fdim, int t_fdim,
                 int x_step, int y_step, int t_step, double *result);

#endif /* CONVOLVE_H */
