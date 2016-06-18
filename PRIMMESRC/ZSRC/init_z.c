/*******************************************************************************
 *   PRIMME PReconditioned Iterative MultiMethod Eigensolver
 *   Copyright (C) 2015 College of William & Mary,
 *   James R. McCombs, Eloy Romero Alcalde, Andreas Stathopoulos, Lingfei Wu
 *
 *   This file is part of PRIMME.
 *
 *   PRIMME is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) any later version.
 *
 *   PRIMME is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *******************************************************************************
 * File: init.c
 *
 * Purpose - Generate the basis that will be used during the first
 *           iteration of the method.
 *  
 ******************************************************************************/

#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "primme.h"
#include "const.h"
#include "init_z.h"
#include "init_private_z.h"
#include "update_projection_z.h"
#include "update_W_z.h"
#include "ortho_z.h"
#include "factorize_z.h"
#include "numerical_z.h"
#include "wtime.h"                       /* Needed for CostModel */


/*******************************************************************************
 * subroutine init_basis - This subroutine is used to 
 *    initialize the basis V.
 *
 * Here we initialize the basis V as well as the array of locked
 * vectors if locking has been enabled.  The cases handled are:
 * 
 *    I. No initial vectors are provided.  BlockSize orthonormal initial
 *       vectors are created and used to form an orthonormal block Krylov
 *       basis.  The size of the basis will be minRestartSize.
 * 
 *   II. Initial vectors are provided.
 *
 *       1.a if locking is disabled, minRestartSize or greater number of initial
 *          vectors are provided and they are orthonormalized by calling ortho.
 * 
 *       1.b if locking is enable, up to minRestartSize of initial vectors are
 *          copied to V and orthonormalized. 
 *       2. There are fewer than minRestartSize initial vectors provided.
 *          A Krylov subspace of dimension restartSize - initSize vectors
 *          is created so that restartSize initial vectors will be available.
 * 
 *
 * INPUT ARRAYS AND PARAMETERS
 * ---------------------------
 * evecs      Array containing the orthogonalization constraints and initial 
 *            guesses. Holds as many as primme->numOrthoConst + primme->numEvals
 *            vectors.
 *
 * machEps    double machine precision
 * 
 * rwork      Double precision work array needed by other subroutines called
 *            by initialize_basis
 *
 * rworkSize  At most the maximum of (maximum size required by the 
 *            orthogonalization routine, maximum worksize by UDUDecompose)
 *
 * primme       Structure containing various solver parameters
 * 
 * 
 * 
 * OUTPUT PARAMETERS
 * -----------------
 * *basisSize   The size of the resulting basis V
 *
 * *nextGuess   The index of the next initial guess stored in the evecs array
 *
 * *numGuesses  When locking is enabled, the number of remaining initial guesses
 * 
 * *timeForMV   Time estimate for applying the matvec operator.
 *              Measured only if primme.dynamicMethodSwitch is on.
 *
 * INPUT/OUTPUT ARRAYS AND PARAMETERS
 * ----------------------------------
 * V            The orthonormal basis
 *
 * W            A*V
 *
 * evecsHat     K^{-1}*evecs, given a preconditioner K
 *
 * M            evecs'*evecsHat.  Its dimension is as large as 
 *              (primme->numOrthoConst + primme->numEvals).
 *
 * UDU          The factorization of M
 *
 * ipivot       The pivots of the UDU factorization
 *
 * Return value
 * ------------
 *  0 - Successful return
 * -1 - Orthogonalization failure
 * -2 - Failure to initialize block Krylov subspace
 * -3 - Failure to initialize Krylov subspace
 * -4 - Failure to UDU decompose M
 *
 ******************************************************************************/

int init_basis_zprimme(Complex_Z *V, int nLocal, int ldV, Complex_Z *W, int ldW,
   Complex_Z *evecs, int ldevecs, Complex_Z *evecsHat, int ldevecsHat, Complex_Z *M,
   int ldM, Complex_Z *UDU, int ldUDU, int *ipivot, double machEps, Complex_Z *rwork,
   int rworkSize, int *basisSize, int *nextGuess, int *numGuesses,
   double *timeForMV, primme_params *primme) {

   int ret;          /* Return value                              */
   int i;
   int initSize;
   int random;

   /* Return memory requirement */

   if (V == NULL) {
      return max(max(max(
            update_projection_zprimme(NULL, 0, NULL, 0, NULL, 0, nLocal,
               0, primme->numOrthoConst, NULL, 0, 1/*symmetric*/, primme),
            UDUDecompose_zprimme(NULL, 0, NULL, 0, NULL,
               primme->numOrthoConst, NULL, 0, primme)),
            ortho_zprimme(NULL, 0, NULL, 0, 0, 
               primme->numOrthoConst-1, NULL, 0, 0, nLocal, 
               NULL, 0.0, NULL, 0, primme)),
            ortho_zprimme(NULL, 0, NULL, 0, 0, *basisSize-1, 
               NULL, 0, primme->numOrthoConst, nLocal, 
               NULL, 0.0, NULL, 0, primme));
   }

   /*-----------------------------------------------------------------------*/
   /* Orthogonalize the orthogonalization constraints provided by the user. */
   /* If a preconditioner is given and inner iterations are to be           */
   /* performed, then initialize M.                                         */
   /*-----------------------------------------------------------------------*/

   if (primme->numOrthoConst > 0) {
   /* lingfei: primme_svds. change ortho function for returning Q and R */
      ret = ortho_zprimme(evecs, ldevecs, NULL, 0, 0, 
        primme->numOrthoConst - 1, NULL, 0, 0, nLocal, 
        primme->iseed, machEps, rwork, rworkSize, primme);

      /* Push an error message onto the stack trace if an error occured */
      if (ret < 0) {
         primme_PushErrorMessage(Primme_init_basis, Primme_ortho, ret, 
                         __FILE__, __LINE__, primme);
         return ORTHO_FAILURE;
      }

      /* Initialize evecsHat, M, and its factorization UDU,ipivot. This   */
      /* allows the orthogonalization constraints to be included in the   */
      /* projector (I-QQ'). Only needed if there is preconditioning, and  */
      /* JDqmr inner iterations with a right, skew projector. Only in     */
      /* that case, is UDU not NULL                                       */

      if (UDU != NULL) {

         primme->applyPreconditioner(evecs, evecsHat, &primme->numOrthoConst, primme); 
         primme->stats.numPreconds += primme->numOrthoConst;

         update_projection_zprimme(evecs, ldevecs, evecsHat, ldevecsHat, M,
            ldM, nLocal, 0, primme->numOrthoConst, rwork, rworkSize, 1/*symmetric*/, primme);

         ret = UDUDecompose_zprimme(M, ldM, UDU, ldUDU, ipivot,
            primme->numOrthoConst, rwork, rworkSize, primme);

         if (ret != 0) {
            primme_PushErrorMessage(Primme_init_basis, Primme_ududecompose, ret,
               __FILE__, __LINE__, primme);
            return UDUDECOMPOSE_FAILURE;
         }

      }  /* if evecsHat and M=evecs'evecsHat, UDU are needed */

   }  /* if numOrthoCont >0 */


   /* Handle case when some or all initial guesses are provided by */ 
   /* the user                                                     */
   if (!primme->locking) {
      initSize = primme->initSize;
   }
   else {
      initSize = min(primme->minRestartSize, primme->initSize);
   }
   *numGuesses = primme->initSize - initSize;
   *nextGuess = primme->numOrthoConst + initSize;

   /* Copy over the initial guesses provided by the user */
   Num_copy_matrix_zprimme(&evecs[primme->numOrthoConst*ldevecs],
         nLocal, initSize, ldevecs, V, ldV);

   switch(primme->initBasisMode) {
   case primme_init_krylov:
      random = 0;
      break;
   case primme_init_random:
      random = max(0,primme->minRestartSize-initSize);
      break;
   case primme_init_user:
      random = max(primme->maxBlockSize-initSize, 0);
      break;
   default:
      assert(0);
   }
   for (i=0; i<random; i++) {
      Num_larnv_zprimme(2, primme->iseed, nLocal,
            &V[ldV*(initSize+i)]);
   }
   *basisSize = initSize + random;

   /* Orthonormalize the guesses provided by the user */ 
   ret = ortho_zprimme(V, ldV, NULL, 0, 0, *basisSize-1, 
         evecs, ldevecs, primme->numOrthoConst, nLocal, 
         primme->iseed, machEps, rwork, rworkSize, primme);

   /* Push an error message onto the stack trace if an error occurred */
   if (ret < 0) {
      primme_PushErrorMessage(Primme_init_basis, Primme_ortho, ret, 
            __FILE__, __LINE__, primme);
      return ORTHO_FAILURE;
   }

   matrixMatvec_zprimme(V, nLocal, ldV, W, ldW, 0, *basisSize, primme);

   if (primme->initBasisMode == primme_init_krylov) {
      ret = init_block_krylov(V, nLocal, ldV, W, ldW, *basisSize,
            primme->minRestartSize-1, evecs, ldevecs, primme->numOrthoConst,
            machEps, rwork, rworkSize, primme); 

      /* Push an error message onto the stack trace if an error occurred */
      if (ret < 0) {
         primme_PushErrorMessage(Primme_init_basis, Primme_init_block_krylov,
               ret, __FILE__, __LINE__, primme);
         return INIT_BLOCK_KRYLOV_FAILURE;
      }

      *basisSize = primme->minRestartSize;
   }

   /* ----------------------------------------------------------- */
   /* If time measurements are needed, waste one MV + one Precond */
   /* Put dummy results in the first open space of W (*basisSize) */
   /* ----------------------------------------------------------- */
   if (primme->dynamicMethodSwitch && *basisSize < primme->maxBasisSize) {
      *timeForMV = primme_wTimer(0);
      matrixMatvec_zprimme(V, nLocal, ldV, &W[ldW*(*basisSize)], ldV,
            0, 1, primme);
      *timeForMV = primme_wTimer(0) - *timeForMV;
   }
      
   return 0;
}


/*******************************************************************************
 * Subroutine init_block_krylov - Initializes the basis as an orthonormal 
 *    block Krylov subspace.  
 *
 * INPUT ARRAYS AND PARAMETERS
 * ---------------------------
 * dv1, dv2    Range of indices over which the basis will be generated
 * 
 * locked      The array of locked Ritz vectors
 * 
 * numLocked   The number of vectors in the locked array
 *
 * machEps     machine precision needed in ortho()
 *
 * rwork       Real work array
 *
 * rworkSize   Size of rwork array
 *
 * 
 * INPUT/OUTPUT ARRAYS
 * -------------------
 * V  The orthonormal basis
 * 
 * W  A*V
 *
 * Return value
 * ------------
 * int -  0 upon success
 *       -1 if orthogonalization failed
 * 
 ******************************************************************************/

static int init_block_krylov(Complex_Z *V, int nLocal, int ldV, Complex_Z *W,
   int ldW, int dv1, int dv2, Complex_Z *locked, int ldlocked, int numLocked,
   double machEps, Complex_Z *rwork, int rworkSize, primme_params *primme) {

   int i;               /* Loop variables */
   int numNewVectors;   /* Number of vectors to be generated */
   int ret;             /* Return code.                      */  
   int blockSize;       /* blockSize used in practice */
   
   numNewVectors = dv2 - dv1 + 1;

   /*----------------------------------------------------------------------*/
   /* Generate a single Krylov space if there are only a few vectors to be */
   /* generated, else generate a block Krylov space with                   */
   /* primme->maxBlockSize as the block Size.                              */ 
   /*----------------------------------------------------------------------*/

   blockSize = numNewVectors <= primme->maxBlockSize ? 1 : primme->maxBlockSize;

   /*----------------------------------------------------------------------*/
   /* Generate the initial vectors.                                        */
   /*----------------------------------------------------------------------*/

   if (dv1+blockSize-1 <= dv2) {
      for (i=dv1; i<dv1+blockSize; i++) {
         Num_larnv_zprimme(2, primme->iseed, nLocal, &V[ldV*i]);
      }
   }
   ret = ortho_zprimme(V, ldV, NULL, 0, dv1, 
      dv1+blockSize-1, locked, ldlocked, numLocked, 
      nLocal, primme->iseed, machEps, rwork, rworkSize, primme);

   /* Generate the remaining vectors in the sequence */

   for (i = dv1+blockSize; i <= dv2; i++) {
      matrixMatvec_zprimme(&V[ldV*(i-blockSize)], nLocal, ldV, &V[ldV*i],
            ldV, 0, 1, primme);
      Num_zcopy_zprimme(nLocal, &V[ldV*i], 1,
         &W[ldW*(i-blockSize)], 1);

       ret = ortho_zprimme(V, ldV, NULL, 0, i, i, locked, 
         ldlocked, numLocked, nLocal, primme->iseed, machEps,
         rwork, rworkSize, primme);

      if (ret < 0) {
         primme_PushErrorMessage(Primme_init_block_krylov, Primme_ortho, 
                         ret, __FILE__, __LINE__, primme);
         return ORTHO_FAILURE;
      }
   }

   matrixMatvec_zprimme(V, nLocal, ldV, W, ldW, dv2-blockSize+1,
         blockSize, primme);

   return 0;
}
