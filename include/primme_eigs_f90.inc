!*******************************************************************************
!  Copyright (c) 2018, College of William & Mary                                   
!  All rights reserved.                                                            
!                                                                                  
!  Redistribution and use in source and binary forms, with or without
!  modification, are permitted provided that the following conditions are met:     
!      * Redistributions of source code must retain the above copyright
!        notice, this list of conditions and the following disclaimer.             
!      * Redistributions in binary form must reproduce the above copyright         
!        notice, this list of conditions and the following disclaimer in the       
!        documentation and/or other materials provided with the distribution.      
!      * Neither the name of the College of William & Mary nor the
!        names of its contributors may be used to endorse or promote products      
!        derived from this software without specific prior written permission.     
!                                                                                  
!  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
!  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
!  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE          
!  DISCLAIMED. IN NO EVENT SHALL THE COLLEGE OF WILLIAM & MARY BE LIABLE FOR ANY       
!  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES      
!  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
!  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
!  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
!  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
!  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
!  
!  PRIMME: https://github.com/primme/primme
!  Contact: Andreas Stathopoulos, a n d r e a s _at_ c s . w m . e d u
!*******************************************************************************
!  File: primme_eigs_f90.inc
!  
!  Purpose - Main header with the PRIMME EIGS F2008 interface functions.
!  
!******************************************************************************

!-------------------------------------------------------
!     Defining easy to remember labels for setting the 
!     method in primme_set_method from Fortran
!-------------------------------------------------------
integer, parameter :: PRIMME_DEFAULT_METHOD = 0
integer, parameter :: PRIMME_DYNAMIC = 1
integer, parameter :: PRIMME_DEFAULT_MIN_TIME = 2
integer, parameter :: PRIMME_DEFAULT_MIN_MATVECS = 3
integer, parameter :: PRIMME_Arnoldi = 4
integer, parameter :: PRIMME_GD = 5
integer, parameter :: PRIMME_GD_plusK = 6
integer, parameter :: PRIMME_GD_Olsen_plusK = 7
integer, parameter :: PRIMME_JD_Olsen_plusK = 8
integer, parameter :: PRIMME_RQI = 9
integer, parameter :: PRIMME_JDQR = 10
integer, parameter :: PRIMME_JDQMR = 11
integer, parameter :: PRIMME_JDQMR_ETol = 12
integer, parameter :: PRIMME_STEEPEST_DESCENT = 13
integer, parameter :: PRIMME_LOBPCG_OrthoBasis = 14
integer, parameter :: PRIMME_LOBPCG_OrthoBasis_Window = 15

!-------------------------------------------------------
!     Defining easy to remember labels for setting the 
!     members of the primme structure from Fortran
!-------------------------------------------------------
integer, parameter :: PRIMME_n                                      = 1 
integer, parameter :: PRIMME_matrixMatvec                           = 2 
integer, parameter :: PRIMME_matrixMatvec_type                      = 3 
integer, parameter :: PRIMME_applyPreconditioner                    = 4 
integer, parameter :: PRIMME_applyPreconditioner_type               = 5 
integer, parameter :: PRIMME_massMatrixMatvec                       = 6 
integer, parameter :: PRIMME_massMatrixMatvec_type                  = 7 
integer, parameter :: PRIMME_numProcs                               = 8 
integer, parameter :: PRIMME_procID                                 = 9 
integer, parameter :: PRIMME_commInfo                               = 10
integer, parameter :: PRIMME_nLocal                                 = 11
integer, parameter :: PRIMME_globalSumReal                          = 12
integer, parameter :: PRIMME_globalSumReal_type                     = 13
integer, parameter :: PRIMME_broadcastReal                          = 14
integer, parameter :: PRIMME_broadcastReal_type                     = 15
integer, parameter :: PRIMME_numEvals                               = 16
integer, parameter :: PRIMME_target                                 = 17
integer, parameter :: PRIMME_numTargetShifts                        = 18
integer, parameter :: PRIMME_targetShifts                           = 19
integer, parameter :: PRIMME_locking                                = 20
integer, parameter :: PRIMME_initSize                               = 21
integer, parameter :: PRIMME_numOrthoConst                          = 22
integer, parameter :: PRIMME_maxBasisSize                           = 23
integer, parameter :: PRIMME_minRestartSize                         = 24
integer, parameter :: PRIMME_maxBlockSize                           = 25
integer, parameter :: PRIMME_maxMatvecs                             = 26
integer, parameter :: PRIMME_maxOuterIterations                     = 27
integer, parameter :: PRIMME_iseed                                  = 28
integer, parameter :: PRIMME_aNorm                                  = 29
integer, parameter :: PRIMME_BNorm                                  = 30
integer, parameter :: PRIMME_invBNorm                               = 31
integer, parameter :: PRIMME_eps                                    = 32
integer, parameter :: PRIMME_orth                                   = 33
integer, parameter :: PRIMME_internalPrecision                      = 34
integer, parameter :: PRIMME_printLevel                             = 35
integer, parameter :: PRIMME_outputFile                             = 36
integer, parameter :: PRIMME_matrix                                 = 37
integer, parameter :: PRIMME_massMatrix                             = 38
integer, parameter :: PRIMME_preconditioner                         = 39
integer, parameter :: PRIMME_ShiftsForPreconditioner                = 40
integer, parameter :: PRIMME_initBasisMode                          = 41
integer, parameter :: PRIMME_projectionParams_projection            = 42
integer, parameter :: PRIMME_restartingParams_maxPrevRetain         = 43
integer, parameter :: PRIMME_correctionParams_precondition          = 44
integer, parameter :: PRIMME_correctionParams_robustShifts          = 45
integer, parameter :: PRIMME_correctionParams_maxInnerIterations    = 46
integer, parameter :: PRIMME_correctionParams_projectors_LeftQ      = 47
integer, parameter :: PRIMME_correctionParams_projectors_LeftX      = 48
integer, parameter :: PRIMME_correctionParams_projectors_RightQ     = 49
integer, parameter :: PRIMME_correctionParams_projectors_RightX     = 50
integer, parameter :: PRIMME_correctionParams_projectors_SkewQ      = 51
integer, parameter :: PRIMME_correctionParams_projectors_SkewX      = 52
integer, parameter :: PRIMME_correctionParams_convTest              = 53
integer, parameter :: PRIMME_correctionParams_relTolBase            = 54
integer, parameter :: PRIMME_stats_numOuterIterations               = 55
integer, parameter :: PRIMME_stats_numRestarts                      = 56
integer, parameter :: PRIMME_stats_numMatvecs                       = 57
integer, parameter :: PRIMME_stats_numPreconds                      = 58
integer, parameter :: PRIMME_stats_numGlobalSum                     = 59
integer, parameter :: PRIMME_stats_volumeGlobalSum                  = 60
integer, parameter :: PRIMME_stats_numBroadcast                     = 61
integer, parameter :: PRIMME_stats_volumeBroadcast                  = 62
integer, parameter :: PRIMME_stats_numOrthoInnerProds               = 63
integer, parameter :: PRIMME_stats_elapsedTime                      = 64
integer, parameter :: PRIMME_stats_timeMatvec                       = 65
integer, parameter :: PRIMME_stats_timePrecond                      = 66
integer, parameter :: PRIMME_stats_timeOrtho                        = 67
integer, parameter :: PRIMME_stats_timeGlobalSum                    = 68
integer, parameter :: PRIMME_stats_timeBroadcast                    = 69
integer, parameter :: PRIMME_stats_estimateMinEVal                  = 70
integer, parameter :: PRIMME_stats_estimateMaxEVal                  = 71
integer, parameter :: PRIMME_stats_estimateLargestSVal              = 72
integer, parameter :: PRIMME_stats_estimateBNorm                    = 73
integer, parameter :: PRIMME_stats_estimateInvBNorm                 = 74
integer, parameter :: PRIMME_stats_maxConvTol                       = 75
integer, parameter :: PRIMME_stats_lockingIssue                     = 76
integer, parameter :: PRIMME_dynamicMethodSwitch                    = 77
integer, parameter :: PRIMME_convTestFun                            = 78
integer, parameter :: PRIMME_convTestFun_type                       = 79
integer, parameter :: PRIMME_convtest                               = 80
integer, parameter :: PRIMME_ldevecs                                = 81
integer, parameter :: PRIMME_ldOPs                                  = 82
integer, parameter :: PRIMME_monitorFun                             = 83
integer, parameter :: PRIMME_monitorFun_type                        = 84
integer, parameter :: PRIMME_monitor                                = 85
integer, parameter :: PRIMME_queue                                  = 86
integer, parameter :: PRIMME_profile                                = 87  

!-------------------------------------------------------
!    Defining easy to remember labels for setting the 
!    enum members for targeting, restarting and innertest
!-------------------------------------------------------

integer(kind=c_int64_t), parameter :: primme_smallest = 0
integer(kind=c_int64_t), parameter :: primme_largest = 1
integer(kind=c_int64_t), parameter :: primme_closest_geq = 2
integer(kind=c_int64_t), parameter :: primme_closest_leq = 3
integer(kind=c_int64_t), parameter :: primme_closest_abs = 4
integer(kind=c_int64_t), parameter :: primme_largest_abs = 5
integer(kind=c_int64_t), parameter :: primme_proj_default = 0
integer(kind=c_int64_t), parameter :: primme_proj_RR = 1
integer(kind=c_int64_t), parameter :: primme_proj_harmonic = 2
integer(kind=c_int64_t), parameter :: primme_proj_refined = 3
integer(kind=c_int64_t), parameter :: primme_init_default = 0
integer(kind=c_int64_t), parameter :: primme_init_krylov = 1
integer(kind=c_int64_t), parameter :: primme_init_random = 2
integer(kind=c_int64_t), parameter :: primme_init_user = 3
integer(kind=c_int64_t), parameter :: primme_full_LTolerance = 0
integer(kind=c_int64_t), parameter :: primme_decreasing_LTolerance = 1
integer(kind=c_int64_t), parameter :: primme_adaptive_ETolerance = 2
integer(kind=c_int64_t), parameter :: primme_adaptive = 3
integer(kind=c_int64_t), parameter :: primme_event_outer_iteration = 0
integer(kind=c_int64_t), parameter :: primme_event_inner_iteration = 1
integer(kind=c_int64_t), parameter :: primme_event_restart = 2
integer(kind=c_int64_t), parameter :: primme_event_reset = 3
integer(kind=c_int64_t), parameter :: primme_event_converged = 4
integer(kind=c_int64_t), parameter :: primme_event_locked = 5
integer(kind=c_int64_t), parameter :: primme_op_default = 0 
integer(kind=c_int64_t), parameter :: primme_op_half = 1
integer(kind=c_int64_t), parameter :: primme_op_float = 2
integer(kind=c_int64_t), parameter :: primme_op_double = 3
integer(kind=c_int64_t), parameter :: primme_op_quad = 4
integer(kind=c_int64_t), parameter :: primme_op_int = 5

!-------------------------------------------------------
! Declare interface
!-------------------------------------------------------

abstract interface
   subroutine primme_eigs_matvec(x, ldx, y, ldy, blockSize, primme, ierr) bind(c)
      use iso_c_binding, only: c_ptr, c_int, c_int64_t, c_double
      integer(c_int64_t) :: ldx, ldy
      type(*) :: x(ldx,*), y(ldy,*)
      type(c_ptr), value :: primme
      integer(c_int), intent(in) :: blockSize
      integer(c_int) :: ierr
   end subroutine
end interface

interface
   function primme_params_create() bind(c)
      use iso_c_binding, only: c_ptr
      type(c_ptr) :: primme_params_create
   end function

   function primme_params_destroy(primme) bind(c)
      use iso_c_binding, only: c_ptr, c_int
      type(c_ptr), intent(in), value :: primme
      integer(c_int) :: primme_params_destroy
   end function
end interface

interface primme_get_member
   function primme_get_member_int(primme, label, value) bind(c, name="primme_get_member")
      use iso_c_binding, only: c_ptr, c_int, c_int64_t
      type(c_ptr), intent(in), value :: primme
      integer(c_int), intent(in), value :: label
      integer(kind=c_int64_t), intent(out) :: value
      integer(c_int) :: primme_get_member_int
   end function

   function primme_get_member_ints4(primme, label, value) bind(c, name="primme_get_member")
      use iso_c_binding, only: c_ptr, c_int, c_int64_t
      type(c_ptr), intent(in), value :: primme
      integer(c_int), intent(in), value :: label
      integer(kind=c_int64_t), dimension(*) :: value
      integer(c_int) :: primme_get_member_ints4
   end function

   function primme_get_member_double(primme, label, value) bind(c, name="primme_get_member")
      use iso_c_binding, only: c_ptr, c_int, c_int64_t, c_double
      type(c_ptr), value :: primme
      integer(c_int), value :: label
      real(kind=c_double), intent(out) :: value
      integer(c_int) :: primme_get_member_double
   end function

   function primme_get_member_ptr(primme, label, value) bind(c, name="primme_get_member")
      use iso_c_binding, only: c_ptr, c_int, c_int64_t
      type(c_ptr), value :: primme
      integer(c_int), value :: label
      type(c_ptr), intent(out) :: value
      integer(c_int) :: primme_get_member_ptr
   end function
end interface primme_get_member

interface primme_set_member
   function primme_set_member_int(primme, label, value) bind(c, name="primme_set_member")
      use iso_c_binding, only: c_ptr, c_int, c_int64_t
      type(c_ptr), intent(in), value :: primme
      integer(c_int), intent(in), value :: label
      integer(kind=c_int64_t), intent(in) :: value
      integer(c_int) :: primme_set_member_int
   end function

   function primme_set_member_double(primme, label, value) bind(c, name="primme_set_member")
      use iso_c_binding, only: c_ptr, c_int, c_int64_t, c_double
      type(c_ptr), value :: primme
      integer(c_int), value :: label
      real(kind=c_double), intent(in) :: value
      integer(c_int) :: primme_set_member_double
   end function

   function primme_set_member_doubles(primme, label, value) bind(c, name="primme_set_member")
      use iso_c_binding, only: c_ptr, c_int, c_int64_t, c_double
      type(c_ptr), value :: primme
      integer(c_int), value :: label
      real(c_double), dimension(*) :: value
      integer(c_int) :: primme_set_member_doubles
   end function

   function primme_set_member_ptr(primme, label, value) bind(c, name="primme_set_member")
      use iso_c_binding, only: c_ptr, c_int, c_int64_t, c_double
      type(c_ptr), value :: primme
      integer(c_int), value :: label
      type(c_ptr), intent(in), value :: value
      integer(c_int) :: primme_set_member_ptr
   end function

   function primme_set_member_fun(primme, label, value) bind(c, name="primme_set_member")
      use iso_c_binding, only: c_ptr, c_funptr, c_int, c_int64_t, c_double
      type(c_ptr), value :: primme
      integer(c_int), value :: label
      type(c_funptr), intent(in), value :: value
      integer(c_int) :: primme_set_member_fun
   end function

   function primme_set_member_matvec(primme, label, value) bind(c, name="primme_set_member")
      use iso_c_binding, only: c_ptr, c_funptr, c_int, c_int64_t
      type(c_ptr), value :: primme
      integer(c_int), value :: label
      procedure(primme_eigs_matvec) :: value
      integer(c_int) :: primme_set_member_matvec
   end function
end interface primme_set_member

interface
   function primme_set_method(method, primme) bind(c)
      use iso_c_binding, only: c_ptr, c_int
      type(c_ptr), value :: primme
      integer(c_int), value :: method
      integer(c_int) :: primme_set_method
   end function
end interface

interface xprimme
   function dprimme(evals, evecs, rnorms, primme) bind(c)
      use iso_c_binding, only: c_ptr, c_funptr, c_int, c_int64_t, c_double
      real(c_double), dimension(*), intent(out) :: evals, rnorms
      real(c_double), dimension(*) :: evecs
      type(c_ptr), value :: primme
      integer(c_int) :: dprimme
   end function

   function zprimme(evals, evecs, rnorms, primme) bind(c)
      use iso_c_binding, only: c_ptr, c_funptr, c_int, c_int64_t, c_double
      real(c_double), dimension(*), intent(out) :: evals, rnorms
      complex(c_double), dimension(*) :: evecs
      type(c_ptr), value :: primme
      integer(c_int) :: zprimme
   end function

   function sprimme(evals, evecs, rnorms, primme) bind(c)
      use iso_c_binding, only: c_ptr, c_funptr, c_int, c_int64_t, c_float
      real(c_float), dimension(*), intent(out) :: evals, rnorms
      real(c_float), dimension(*) :: evecs
      type(c_ptr), value :: primme
      integer(c_int) :: sprimme
   end function

   function cprimme(evals, evecs, rnorms, primme) bind(c)
      use iso_c_binding, only: c_ptr, c_funptr, c_int, c_int64_t, c_float
      real(c_float), dimension(*), intent(out) :: evals, rnorms
      complex(c_float), dimension(*) :: evecs
      type(c_ptr), value :: primme
      integer(c_int) :: cprimme
   end function
end interface xprimme
