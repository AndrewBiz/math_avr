//!
//! @file 				Fp64sCompoundArithmetic.cpp
//! @author 			Geoffrey Hunter <gbmhunter@gmail.com> (www.mbedded.ninja)
//! @edited 			n/a
//! @created			2013-07-24
//! @last-modified		2014-09-15
//! @brief 				Performs unit tests on the slow 64-bit fixed point compound arithmetic.
//! @details
//!		See README.rst in root fir for more info.

//===== SYSTEM LIBRARIES =====//
// none

//===== USER LIBRARIES =====//
#include "MUnitTest/api/MUnitTestApi.hpp"

//===== USER SOURCE =====//
#include "../api/MFixedPointApi.hpp"

using namespace Fp;

MTEST_GROUP(Fp64sCompoundArithmeticTests)
{
	MTEST(CompoundAdditionTest)
	{
		Fp64s fp1 = Fp64s(3.2, 12);
		Fp64s fp2 = Fp64s(0.6, 12);
		
		fp1 += fp2;

		CHECK_CLOSE(3.8, (double)fp1, 0.1);
	}

	MTEST(CompoundSubtractionTest)
	{
		Fp64s fp1 = Fp64s(3.2, 12);
		Fp64s fp2 = Fp64s(0.6, 12);
		
		fp1 -= fp2;

		CHECK_CLOSE(2.6, (double)fp1, 0.1);
	}

	MTEST(CompoundMultiplicationTest)
	{
		Fp64s fp1 = Fp64s(3.2, 12);
		Fp64s fp2 = Fp64s(0.6, 12);
		
		fp1 *= fp2;

		CHECK_CLOSE(1.92, (double)fp1, 0.1);
	}

	MTEST(CompoundDivisionTest)
	{
		Fp64s fp1 = Fp64s(3.2, 12);
		Fp64s fp2 = Fp64s(0.6, 12);
		
		fp1 /= fp2;

		CHECK_CLOSE(5.33, (double)fp1, 0.1);
	}
	
	MTEST(CompoundModuloTest)
	{
		Fp64s fp1 = Fp64s(20.2, 12);
		Fp64s fp2 = Fp64s(1.5, 12);
		
		fp1 %= fp2;
		//20.2 % 1.5 = 0.7
		
		CHECK_CLOSE(0.7, (double)fp1, 0.1);
	}
}
