/*******************************************************************************
 * Copyright (c) 2016, College of William & Mary
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of College of William & Mary nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL COLLEGE OF WILLIAM & MARY BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * PRIMME: https://github.com/primme/primme
 * Contact: Andreas Stathopoulos, a n d r e a s _at_ c s . w m . e d u
 *******************************************************************************
 *   NOTE: THIS FILE IS AUTOMATICALLY GENERATED. PLEASE DON'T MODIFY
 ******************************************************************************/


#ifndef update_projection_H
#define update_projection_H
#if !defined(CHECK_TEMPLATE) && !defined(update_projection_Sprimme)
#  define update_projection_Sprimme CONCAT(update_projection_,SCALAR_SUF)
#endif
#if !defined(CHECK_TEMPLATE) && !defined(update_projection_Rprimme)
#  define update_projection_Rprimme CONCAT(update_projection_,REAL_SUF)
#endif
int update_projection_dprimme(double *X, int64_t ldX, double *Y,
      int64_t ldY, double *Z, int64_t ldZ, int64_t nLocal, int numCols,
      int blockSize, double *rwork, size_t *lrwork, int isSymmetric,
      primme_params *primme);
int update_projection_zprimme(PRIMME_COMPLEX_DOUBLE *X, int64_t ldX, PRIMME_COMPLEX_DOUBLE *Y,
      int64_t ldY, PRIMME_COMPLEX_DOUBLE *Z, int64_t ldZ, int64_t nLocal, int numCols,
      int blockSize, PRIMME_COMPLEX_DOUBLE *rwork, size_t *lrwork, int isSymmetric,
      primme_params *primme);
int update_projection_sprimme(float *X, int64_t ldX, float *Y,
      int64_t ldY, float *Z, int64_t ldZ, int64_t nLocal, int numCols,
      int blockSize, float *rwork, size_t *lrwork, int isSymmetric,
      primme_params *primme);
int update_projection_cprimme(PRIMME_COMPLEX_FLOAT *X, int64_t ldX, PRIMME_COMPLEX_FLOAT *Y,
      int64_t ldY, PRIMME_COMPLEX_FLOAT *Z, int64_t ldZ, int64_t nLocal, int numCols,
      int blockSize, PRIMME_COMPLEX_FLOAT *rwork, size_t *lrwork, int isSymmetric,
      primme_params *primme);
#endif
