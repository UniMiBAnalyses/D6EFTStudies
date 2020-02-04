ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
c      written by the UFO converter
ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc

      SUBROUTINE COUP1()

      IMPLICIT NONE
      INCLUDE 'model_functions.inc'

      DOUBLE PRECISION PI, ZERO
      PARAMETER  (PI=3.141592653589793D0)
      PARAMETER  (ZERO=0D0)
      INCLUDE 'input.inc'
      INCLUDE 'coupl.inc'
      GC_233 = -((MDL_CKM3X3*MDL_EE*MDL_COMPLEXI)/(MDL_STH*MDL_SQRT__2)
     $ )
      GC_234 = -(MDL_CTH*MDL_EE*MDL_COMPLEXI)/(2.000000D+00*MDL_STH)
      GC_236 = -((MDL_CTH*MDL_EE*MDL_COMPLEXI)/MDL_STH)
      GC_248 = (MDL_EE*MDL_COMPLEXI*MDL_STH)/(6.000000D+00*MDL_CTH)
      GC_249 = -(MDL_EE*MDL_COMPLEXI*MDL_STH)/(2.000000D+00*MDL_CTH)
      GC_254 = -(MDL_CTH*MDL_EE*MDL_COMPLEXI)/(2.000000D+00*MDL_STH)
     $ -(MDL_EE*MDL_COMPLEXI*MDL_STH)/(2.000000D+00*MDL_CTH)
      GC_336 = (MDL_CHE*MDL_CTH*MDL_EE*MDL_COMPLEXI*MDL_VEVHAT__EXP__2)
     $ /(2.000000D+00*MDL_LAMBDASMEFT__EXP__2*MDL_STH)+(MDL_CHE*MDL_EE
     $ *MDL_COMPLEXI*MDL_STH*MDL_VEVHAT__EXP__2)/(2.000000D+00*MDL_CTH
     $ *MDL_LAMBDASMEFT__EXP__2)
      END
