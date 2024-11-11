/*
 * range2.c
 *
 * This MEX function calculates the minimum and maximum values of an input 
 * matrix and returns them as separate outputs. The input matrix must be a 
 * real, non-sparse, double-precision matrix.
 *
 * Usage in MATLAB:
 *   mex range2.c                       % Compile the MEX function
 *   [minVal, maxVal] = range2(matrix)  % Compute min and max of 'matrix'
 *
 * Inputs:
 *   - matrix: A real, non-sparse, double-precision matrix.
 *
 * Outputs:
 *   - minVal: The minimum value in the input matrix.
 *   - maxVal: The maximum value in the input matrix.
 *
 * Notes:
 *   - This function returns two separate scalar values: the minimum and maximum.
 *
 * Example:
 *   A = [3, 5; 1, 9];
 *   [minVal, maxVal] = range2(A);  % minVal will be 1, maxVal will be 9
 *
 * Author: EPS, 3/97. Updated by RTR 11/2024
 */

#include <matrix.h>  /* MATLAB matrix library */
#include <mex.h>     /* MATLAB MEX functions */
#include <stddef.h>  /* Standard definitions like NULL */

/* Macro to check if an input is a valid, real, non-sparse double matrix */
#define notDblMtx(matrix) (!mxIsNumeric(matrix) || !mxIsDouble(matrix) || mxIsSparse(matrix) || mxIsComplex(matrix))

/* Main MEX function - Entry point called from MATLAB */
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

    /* Variable declarations */
    const double *mtx;      /* Pointer to the input matrix data */
    double minVal, maxVal;  /* Variables to hold the minimum and maximum values */
    size_t i, numElements;  /* Number of elements in the matrix */
    const mxArray *arg;     /* Temporary pointer for input validation */

    /* Check for exactly 1 input argument */
    if (nrhs != 1) {
        mexErrMsgTxt("This function requires exactly 1 input argument.");
    }

    /* Argument 1: MATRIX */
    arg = prhs[0];
    if (notDblMtx(arg)) {
        mexErrMsgTxt("The input matrix must be a real, non-sparse double-precision matrix.");
    }
    mtx = mxGetPr(arg);
    numElements = mxGetM(arg) * mxGetN(arg);  /* Calculate total number of elements */

    /* Initialize min and max values */
    minVal = mtx[0];
    maxVal = mtx[0];

    /* Find the minimum and maximum values in the matrix */
    for (i = 1; i < numElements; i++) {
        double temp = mtx[i];
        if (temp < minVal) {
            minVal = temp;
        } else if (temp > maxVal) {
            maxVal = temp;
        }
    }

    /* Create output matrices for min and max values */
    plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
    if (plhs[0] == NULL) {
        mexErrMsgTxt("Error allocating result matrix for min value.");
    }
    mxGetPr(plhs[0])[0] = minVal;

    plhs[1] = mxCreateDoubleMatrix(1, 1, mxREAL);
    if (plhs[1] == NULL) {
        mexErrMsgTxt("Error allocating result matrix for max value.");
    }
    mxGetPr(plhs[1])[0] = maxVal;

    return;
}
