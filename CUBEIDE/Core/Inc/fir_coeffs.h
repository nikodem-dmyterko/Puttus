/*
 * Filter Coefficients (C Source) generated by the Filter Design and Analysis Tool
 * Generated by MATLAB(R) 9.13 and Signal Processing Toolbox 9.1.
 * Generated on: 21-Jan-2023 11:53:28
 */

/*
 * Discrete-Time FIR Filter (real)
 * -------------------------------
 * Filter Structure  : Direct-Form FIR
 * Filter Length     : 13
 * Stable            : Yes
 * Linear Phase      : Yes (Type 1)
 */

/* General type conversion for MATLAB generated C-code  */
#include "tmwtypes.h"
#include "arm_math.h"
/* 
 * Expected path to tmwtypes.h 
 * C:\Program Files\MATLAB\R2022b\extern\include\tmwtypes.h 
 */
const int BL = 13;
const float32_t B[13] = {
   -0.01122438086569,  0.00468613197333,  0.05697002003886, -0.03005198720482,
   -0.09592119494385,   0.2872964212019,   0.6241293939518,   0.2872964212019,
   -0.09592119494385, -0.03005198720482,  0.05697002003886,  0.00468613197333,
   -0.01122438086569
};
