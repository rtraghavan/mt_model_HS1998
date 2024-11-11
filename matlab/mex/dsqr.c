/*
 * dsqr.c
 *
 * This MEX function takes a single MATLAB matrix as input and squares 
 * each of its elements in place. It directly modifies the input matrix.
 *
 * Usage in MATLAB:
 *   mex dsqr.c          % Compile the MEX function
 *   dsqr(matrix)        % Squares each element in 'matrix'
 *
 * Input:
 *   - A real, non-sparse, double-precision matrix.
 *
 * Output:
 *   - The input matrix itself is modified to contain its squared values.
 *
 * Author:   EPS [5/98]. Updated by RTR [11/2024]
 */

#include <matrix.h>  /* MATLAB matrix library */
#include <mex.h>     /* MATLAB MEX functions */
#include <stddef.h>  /* Standard definitions like NULL */

/* Macro to check if the input is a valid matrix for squaring:
   It must be a real, non-sparse, double-precision matrix */
#define notDblMtx(matrix) (!mxIsNumeric(matrix) || !mxIsDouble(matrix) || mxIsSparse(matrix) || mxIsComplex(matrix))

/* Main function for MEX file - Entry point called from MATLAB */
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    
    /* Variable declarations */
    double *matrix_data;         /* Pointer to the data in the input matrix */
    const mxArray *input_matrix; /* Pointer to the input matrix (const for safety) */
    size_t num_elements;         /* Total number of elements in the input matrix */
    size_t i;                    /* Loop variable */

    /* Input validation: Check for exactly 1 input argument */
    if (nrhs != 1) {
        mexErrMsgTxt("This function requires exactly 1 input argument.");
    }

    /* Retrieve the input matrix */
    input_matrix = prhs[0];

    /* Validate the input matrix type using the notDblMtx macro */
    if (notDblMtx(input_matrix)) {
        mexErrMsgTxt("Input must be a real, non-sparse, double-precision matrix.");
    }

    /* Get a pointer to the real data of the input matrix */
    matrix_data = mxGetPr(input_matrix);

    /* Calculate the number of elements in the matrix */
    num_elements = mxGetM(input_matrix) * mxGetN(input_matrix);

    /* Square each element of the matrix in place */
    for (i = 0; i < num_elements; i++) {
        matrix_data[i] *= matrix_data[i];
    }

    /* Explicit return for clarity in a void function */
    return;
}
