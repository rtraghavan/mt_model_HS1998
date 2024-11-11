/*
 * destructiveMatrixWriteAtIndices.c
 *
 * This MEX function updates an input MATLAB matrix in place, overwriting
 * a specified number of elements starting from a given index with values
 * from another input matrix. The function requires three arguments:
 * the target matrix, a matrix of new values, and the starting index.
 *
 * Usage in MATLAB:
 *   mex destructiveMatrixWriteAtIndices.c % Compile the MEX function
 *   destructiveMatrixWriteAtIndices(targetMatrix, newValues, startIndex)
 *
 * Inputs:
 *   - targetMatrix: A real, non-sparse, double-precision matrix that will
 *                   be updated in place.
 *   - newValues:    A real, non-sparse, double-precision matrix containing
 *                   the values to write into the target matrix.
 *   - startIndex:   A scalar specifying the starting index (1-based in MATLAB)
 *                   at which to begin overwriting values in targetMatrix.
 *
 * Behavior:
 *   - The function overwrites values in the target matrix starting from
 *     startIndex up to startIndex + numValues - 1, where numValues is the
 *     number of elements in newValues.
 *
 * Notes:
 *   - This function modifies the target matrix directly, so it does not return
 *     a separate output.
 *   - The function performs bounds checking to ensure startIndex and
 *     startIndex + numValues do not exceed the size of targetMatrix.
 *
 * Example:
 *   A = [1, 2, 3, 4, 5];
 *   newValues = [10, 20];
 *   destructiveMatrixWriteAtIndices(A, newValues, 2);     % A becomes [1, 10, 20, 4, 5]
 *
 * Author: EPS, 5/98 updated by RTR 11/2024
 */

#include <matrix.h> /* MATLAB matrix library */
#include <mex.h>    /* MATLAB MEX functions */
#include <stddef.h> /* Standard definitions like NULL */

/* Macro to check if an input is a valid, real, non-sparse double matrix */
#define notDblMtx(matrix) (!mxIsNumeric(matrix) || !mxIsDouble(matrix) || mxIsSparse(matrix) || mxIsComplex(matrix))

/* Main MEX function - Entry point called from MATLAB */
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{

  /* Variable declarations */
  double *mtx;                 /* Pointer to the data in the target matrix */
  double *newValues;           /* Pointer to the new values to be written */
  const double *startIndexPtr; /* Pointer to the starting index */
  size_t i, startIndex, numValues;
  const mxArray *arg; /* Temporary pointer for input validation */

  /* Check for the required number of input arguments */
  if (nrhs != 3)
  {
    mexErrMsgTxt("This function requires exactly 3 input arguments.");
  }

  /* Argument 1: Target MATRIX */
  arg = prhs[0];
  if (notDblMtx(arg))
  {
    mexErrMsgTxt("Argument 1 (MATRIX) must be a real, non-sparse double-precision matrix.");
  }
  mtx = mxGetPr(arg);

  /* Argument 2: New values */
  arg = prhs[1];
  if (notDblMtx(arg))
  {
    mexErrMsgTxt("Argument 2 (newValues) must be a real, non-sparse double-precision matrix.");
  }
  newValues = mxGetPr(arg);
  numValues = mxGetM(arg) * mxGetN(arg); /* Number of elements in newValues */

  /* Argument 3: Start index */
  arg = prhs[2];
  if (notDblMtx(arg) || mxGetNumberOfElements(arg) != 1)
  {
    mexErrMsgTxt("Argument 3 (startIndex) must be a real scalar.");
  }
  startIndexPtr = mxGetPr(arg);
  startIndex = (size_t)(*startIndexPtr);

  /* Validate the starting index and ensure bounds for writing */
  size_t mtxSize = mxGetM(prhs[0]) * mxGetN(prhs[0]);
  if (startIndex >= mtxSize || startIndex + numValues > mtxSize)
  {
    mexErrMsgTxt("Starting index and number of values exceed matrix bounds.");
  }

  /* Overwrite values in the target matrix starting from startIndex */
  for (i = 0; i < numValues; i++)
  {
    mtx[startIndex + i] = newValues[i];
  }

  return;
}
