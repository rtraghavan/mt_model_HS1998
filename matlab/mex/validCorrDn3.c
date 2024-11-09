/*
  Modernized validCorrDn3: RES = validCorrDn3(IM, FILT, STEP)
  
  This function performs a valid correlation on an input image `IM` 
  with a filter `FILT`, using specified step sizes `STEP`.

  Author: EPS, Modernized version
*/

#include <matrix.h>
#include <mex.h>
#include "convolve.h"  // Include convolve header for valid_filter function

#define isValidDoubleMatrix(matrix) (mxIsNumeric(matrix) && mxIsDouble(matrix) && !mxIsSparse(matrix) && !mxIsComplex(matrix))

void validateInputs(int nrhs, const mxArray *prhs[], double **image, double **filter, int *image_dims, int *filter_dims, int *step);
void calculateOutputDimensions(int *image_dims, int *filter_dims, int *step, int *result_dims);
void performValidCorrelation(double *image, double *filter, double *result, int *image_dims, int *filter_dims, int *step, int *result_dims);

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    double *image, *filter, *result;
    int image_dims[4] = {1, 1, 1, 1};
    int filter_dims[4] = {1, 1, 1, 1};
    int step[3] = {1, 1, 1};
    int result_dims[4];

    validateInputs(nrhs, prhs, &image, &filter, image_dims, filter_dims, step);
    calculateOutputDimensions(image_dims, filter_dims, step, result_dims);

    plhs[0] = mxCreateNumericArray(4, result_dims, mxDOUBLE_CLASS, mxREAL);
    result = mxGetPr(plhs[0]);

    performValidCorrelation(image, filter, result, image_dims, filter_dims, step, result_dims);
}

void validateInputs(int nrhs, const mxArray *prhs[], double **image, double **filter, int *image_dims, int *filter_dims, int *step) {
    const mxArray *arg;
    double *step_data;
    const int *pdim;
    int i, ndims;

    if (nrhs < 2) mexErrMsgIdAndTxt("MATLAB:validCorrDn3:invalidNumInputs", "Requires at least two input arguments: image and filter.");

    arg = prhs[0];
    if (!isValidDoubleMatrix(arg)) mexErrMsgIdAndTxt("MATLAB:validCorrDn3:invalidImage", "IMAGE must be a non-sparse, double-precision matrix.");
    *image = mxGetPr(arg);
    pdim = mxGetDimensions(arg);
    ndims = mxGetNumberOfDimensions(arg);
    for (i = 0; i < ndims && i < 4; i++) image_dims[i] = pdim[i];

    arg = prhs[1];
    if (!isValidDoubleMatrix(arg)) mexErrMsgIdAndTxt("MATLAB:validCorrDn3:invalidFilter", "FILTER must be a non-sparse, double-precision matrix.");
    *filter = mxGetPr(arg);
    pdim = mxGetDimensions(arg);
    ndims = mxGetNumberOfDimensions(arg);
    for (i = 0; i < ndims && i < 4; i++) filter_dims[i] = pdim[i];

    if (nrhs > 2) {
        arg = prhs[2];
        if (!isValidDoubleMatrix(arg) || mxGetNumberOfElements(arg) != 3) 
            mexErrMsgIdAndTxt("MATLAB:validCorrDn3:invalidStep", "STEP must be a 1x3 non-sparse double array.");
        step_data = mxGetPr(arg);
        for (i = 0; i < 3; i++) step[i] = (int) step_data[i];
    }
}

void calculateOutputDimensions(int *image_dims, int *filter_dims, int *step, int *result_dims) {
    result_dims[0] = (image_dims[0] - filter_dims[0]) / step[0] + 1;
    result_dims[1] = (image_dims[1] - filter_dims[1]) / step[1] + 1;
    result_dims[2] = (image_dims[2] - filter_dims[2]) / step[2] + 1;
    result_dims[3] = image_dims[3];
}

void performValidCorrelation(double *image, double *filter, double *result, int *image_dims, int *filter_dims, int *step, int *result_dims) {
    /* Call the valid_filter function from svalidconvolve.c for convolution and down-sampling */
    valid_filter(image, image_dims[0], image_dims[1], image_dims[2], image_dims[3],
                 filter, filter_dims[0], filter_dims[1], filter_dims[2],
                 step[0], step[1], step[2], result);
}
