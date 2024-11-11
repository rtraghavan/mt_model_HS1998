/*
 * validCorrDn3.c
 *
 * This MEX function performs a valid (no padding) 3D correlation of an input 
 * image with a filter, with optional down-sampling. The function returns the 
 * correlation result in a smaller output matrix, based on the filter size 
 * and specified step sizes.
 *
 * Usage in MATLAB:
 *   mex validCorrDn3.c svalidconvolve.c              % Compile the MEX function
 *   result = validCorrDn3(image, filter, step)
 *
 * Inputs:
 *   - image:    A real, non-sparse, double-precision 3D matrix representing the input image.
 *   - filter:   A real, non-sparse, double-precision 3D matrix serving as the correlation filter.
 *   - step:     (Optional) A 1x3 vector of integers specifying the step size for down-sampling
 *               in each dimension (e.g., [stepX, stepY, stepT]). Defaults to [1, 1, 1].
 *
 * Output:
 *   - result:   A matrix of the valid correlation output, down-sampled based on the step size.
 *               Its dimensions depend on the input image size, filter size, and step size.
 *
 * Notes:
 *   - This function performs "valid" correlation, meaning only regions where the filter 
 *     fully overlaps with the image are computed, resulting in a smaller output matrix.
 *   - The function directly calls 'valid_filter' from 'svalidconvolve.c' to compute the 
 *     correlation and down-sampling.
 *
 * Author: [Your Name], [Date]
 */


#include <matrix.h>
#include <mex.h>
#include "convolve.h"  // Include convolve header for valid_filter function

#define isValidDoubleMatrix(matrix) (mxIsNumeric(matrix) && mxIsDouble(matrix) && !mxIsSparse(matrix) && !mxIsComplex(matrix))

void validateInputs(int nrhs, const mxArray *prhs[], double **image, double **filter, mwSize *image_dims, mwSize *filter_dims, int *step);
void calculateOutputDimensions(mwSize *image_dims, mwSize *filter_dims, int *step, mwSize *result_dims);
void performValidCorrelation(double *image, double *filter, double *result, mwSize *image_dims, mwSize *filter_dims, int *step, mwSize *result_dims);

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    double *image, *filter, *result;
    mwSize image_dims[4] = {1, 1, 1, 1};  // Use mwSize instead of int
    mwSize filter_dims[4] = {1, 1, 1, 1}; // Use mwSize instead of int
    int step[3] = {1, 1, 1};
    mwSize result_dims[4];  // Output dimensions as mwSize

    validateInputs(nrhs, prhs, &image, &filter, image_dims, filter_dims, step);
    calculateOutputDimensions(image_dims, filter_dims, step, result_dims);


    plhs[0] = mxCreateNumericArray(4, result_dims, mxDOUBLE_CLASS, mxREAL);
    result = mxGetPr(plhs[0]);

    performValidCorrelation(image, filter, result, image_dims, filter_dims, step, result_dims);
}

void validateInputs(int nrhs, const mxArray *prhs[], double **image, double **filter, mwSize *image_dims, mwSize *filter_dims, int *step) {
    const mwSize *pdim;  // Updated to mwSize for compatibility

    if (nrhs < 2) mexErrMsgIdAndTxt("MATLAB:validCorrDn3:invalidNumInputs", "Requires at least two input arguments: image and filter.");

    // Validate and retrieve dimensions for image and filter
    const mxArray *arg = prhs[0];
    if (!mxIsDouble(arg) || mxIsSparse(arg) || mxIsComplex(arg)) {
        mexErrMsgIdAndTxt("MATLAB:validCorrDn3:invalidImage", "IMAGE must be a non-sparse, double-precision matrix.");
    }
    *image = mxGetPr(arg);
    pdim = mxGetDimensions(arg);
    for (int i = 0; i < mxGetNumberOfDimensions(arg) && i < 4; i++) {
        image_dims[i] = pdim[i];
    }

    arg = prhs[1];
    if (!mxIsDouble(arg) || mxIsSparse(arg) || mxIsComplex(arg)) {
        mexErrMsgIdAndTxt("MATLAB:validCorrDn3:invalidFilter", "FILTER must be a non-sparse, double-precision matrix.");
    }
    *filter = mxGetPr(arg);
    pdim = mxGetDimensions(arg);
    for (int i = 0; i < mxGetNumberOfDimensions(arg) && i < 4; i++) {
        filter_dims[i] = pdim[i];
    }

    if (nrhs > 2) {
        arg = prhs[2];
        if (!mxIsDouble(arg) || mxGetNumberOfElements(arg) != 3) {
            mexErrMsgIdAndTxt("MATLAB:validCorrDn3:invalidStep", "STEP must be a 1x3 non-sparse double array.");
        }
        double *step_data = mxGetPr(arg);
        for (int i = 0; i < 3; i++) {
            step[i] = (int)step_data[i];
        }
    }
}

void calculateOutputDimensions(mwSize *image_dims, mwSize *filter_dims, int *step, mwSize *result_dims) {
    result_dims[0] = (image_dims[0] - filter_dims[0]) / step[0] + 1;
    result_dims[1] = (image_dims[1] - filter_dims[1]) / step[1] + 1;
    result_dims[2] = (image_dims[2] - filter_dims[2]) / step[2] + 1;
    result_dims[3] = image_dims[3];
}

void performValidCorrelation(double *image, double *filter, double *result, mwSize *image_dims, mwSize *filter_dims, int *step, mwSize *result_dims) {
    valid_filter(image, image_dims[0], image_dims[1], image_dims[2],
                 filter, filter_dims[0], filter_dims[1], filter_dims[2],
                 step[0], step[1], step[2], result);
}
