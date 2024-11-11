/*
 * pointOp.c
 *
 * This MEX function performs a point operation on an input image matrix 
 * using a lookup table (LUT). Each pixel value in the image is transformed 
 * based on a specified lookup table, origin, and increment, with optional 
 * warnings for extrapolated values.
 *
 * Usage in MATLAB:
 *   mex pointOp.c                                 % Compile the MEX function
 *   result = pointOp(image, lut, origin, increment, warnings)
 *
 * Inputs:
 *   - image:      A real, non-sparse, double-precision matrix representing the input image.
 *   - lut:        A real, non-sparse, double-precision vector serving as the lookup table.
 *   - origin:     A real scalar that defines the origin for the LUT.
 *   - increment:  A real scalar specifying the increment step for the LUT.
 *   - warnings:   (Optional) A real scalar (1 for warnings, 0 to suppress warnings).
 *
 * Output:
 *   - result:     A matrix of the same size as 'image', where each element has been transformed
 *                 according to the LUT and input parameters.
 *
 * Notes:
 *   - If a pixel value falls outside the LUT range, the nearest LUT boundary value is used.
 *   - If 'warnings' is enabled, a message is displayed for each extrapolated value.
 *
 * Author: EPS, ported from OBVIUS, 7/96. Updated by RTR 2024
 */



#include <matrix.h>  /* MATLAB matrix library */
#include <mex.h>     /* MATLAB MEX functions */
#include <stddef.h>  /* Standard definitions like NULL */

/* Macro to check if the input is a valid double matrix */
#define notDblMtx(matrix) (!mxIsNumeric(matrix) || !mxIsDouble(matrix) || mxIsSparse(matrix) || mxIsComplex(matrix))

/* Function prototype for internal point operation */
void internal_pointop(const double *im, double *res, size_t size, const double *lut, size_t lutsize, 
                      double origin, double increment, int warnings);

/* Main MEX function - Entry point called from MATLAB */
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

    /* Variable declarations */
    const double *image, *lut;
    double *res;
    double origin, increment;
    size_t x_dim, y_dim, lut_size;
    int warnings = 1;
    const mxArray *arg;
    const double *mxMat;

    /* Check for minimum number of arguments */
    if (nrhs < 4) {
        mexErrMsgTxt("This function requires at least 4 input arguments.");
    }

    /* Argument 1: IMAGE */
    arg = prhs[0];
    if (notDblMtx(arg)) {
        mexErrMsgTxt("IMAGE must be a real, non-sparse double-precision matrix.");
    }
    image = mxGetPr(arg);
    x_dim = (size_t)mxGetM(arg);  /* X is the inner index */
    y_dim = (size_t)mxGetN(arg);

    /* Argument 2: Lookup Table (LUT) */
    arg = prhs[1];
    if (notDblMtx(arg)) {
        mexErrMsgTxt("LUT must be a real,   EPS, ported from OBVIUS, 7/96.non-sparse double-precision vector.");
    }
    lut = mxGetPr(arg);
    lut_size = (size_t)(mxGetM(arg) * mxGetN(arg));
    if (mxGetM(arg) != 1 && mxGetN(arg) != 1) {
        mexErrMsgTxt("Lookup table must be a row or column vector.");
    }

    /* Argument 3: ORIGIN */
    arg = prhs[2];
    if (notDblMtx(arg) || mxGetNumberOfElements(arg) != 1) {
        mexErrMsgTxt("ORIGIN must be a real scalar.");
    }
    origin = *mxGetPr(arg);

    /* Argument 4: INCREMENT */
    arg = prhs[3];
    if (notDblMtx(arg) || mxGetNumberOfElements(arg) != 1) {
        mexErrMsgTxt("INCREMENT must be a real scalar.");
    }
    increment = *mxGetPr(arg);

    /* Optional Argument 5: WARNINGS */
    if (nrhs > 4) {
        arg = prhs[4];
        if (notDblMtx(arg) || mxGetNumberOfElements(arg) != 1) {
            mexErrMsgTxt("WARNINGS must be a real scalar.");
        }
        warnings = (int)*mxGetPr(arg);
    }

    /* Create output matrix */
    plhs[0] = mxCreateDoubleMatrix(x_dim, y_dim, mxREAL);
    if (plhs[0] == NULL) {
        mexErrMsgTxt("Cannot allocate result matrix.");
    }
    res = mxGetPr(plhs[0]);
      
    /* Perform the point operation */
    internal_pointop(image, res, x_dim * y_dim, lut, lut_size, origin, increment, warnings);
}

/* Perform linear interpolation using a lookup table */
void internal_pointop(const double *im, double *res, size_t size, const double *lut, size_t lutsize, 
                      double origin, double increment, int warnings) {
    size_t i, index;
    double pos;
    int l_unwarned = warnings;
    int r_unwarned = warnings;

    lutsize -= 2; /* Maximum index value for safe lookup */

    if (increment > 0) {
        /* Iterate over each pixel */
        for (i = 0; i < size; i++) {
            pos = (im[i] - origin) / increment;
            index = (size_t)pos;  /* Floor of the position */

            /* Check for extrapolation and issue warnings if needed */
            if (index < 0) {
                index = 0;
                if (l_unwarned) {
                    mexPrintf("Warning: Extrapolating to left of lookup table...\n");
                    l_unwarned = 0;
                }
/* 

 * 
 */

                if (r_unwarned) {
                    mexPrintf("Warning: Extrapolating to right of lookup table...\n");
                    r_unwarned = 0;
                }
            }

            /* Linear interpolation between adjacent LUT values */
            res[i] = lut[index] + (lut[index + 1] - lut[index]) * (pos - index);
        }
    } else {
        /* If increment is zero, set all results to the first LUT value */
        for (i = 0; i < size; i++) {
            res[i] = *lut;
        }
    }
}
