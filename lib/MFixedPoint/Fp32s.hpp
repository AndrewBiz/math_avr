//!
//! @file                 Fp32s.hpp
//! @author             Geoffrey Hunter <gbmhunter@gmail.com> (www.mbedded.ninja)
//                      AndrewBiz forked and updated the lib
//! @edited             n/a
//! @created            2013-07-22
//! @last-modified        2014-09-15
//! @brief                 A slower, more powerful 32-bit fixed point library.
//! @details
//!        See README.rst in root dir for more info.

//===============================================================================================//
//====================================== HEADER GUARD ===========================================//
//===============================================================================================//

#ifndef __cplusplus
    #error Please build with C++ compiler
#endif

#ifndef FP32S_H
#define FP32S_H

#define QDEC_MAX 9

#include <stdint.h>

namespace Fp
{
// Luck-up table to calculate number of decimal digits depending on chosen q (fractional binary digits)
// formula used for qd = ROUND(LOG(2^q;10))
static const uint8_t qd_lut[] = {0, 0, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8, 8, 9, 9, 9, 9};
static const uint32_t power10[] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};

//! The template argument p in all of the following functions refers to the
//! fixed point precision (e.g. p = 8 gives 24.8 fixed point functions).
//! Contains mathematical operator overloading. Doesn't have modulus (%) overloading
class Fp32s {
private:
    void _set_q(uint8_t qin)
    {
        q = qin;
        qd = qd_lut[q];
    }

public:

    //! @brief        The fixed-point number is stored in this basic data type.
    int32_t rawVal;

    //! @brief        This stores the number of fractional bits.
    uint8_t q;

    uint8_t qd;  // decimal digits for the current number

    Fp32s()
    {
    }

    Fp32s(int32_t i, uint8_t qin)
    {
        rawVal = i << qin;
        _set_q(qin);
    }

    Fp32s(double dbl, uint8_t qin)
    {
        rawVal = (int32_t)(dbl * (1 << qin));
        _set_q(qin);
    }

    //  To get the FixPoint number (e.g 123.0045)- set separately:
    //  integer part: 123
    //  fractional part integer: 45
    //  fractional part divider: 10000
    //  fractional bits: 13 (13 binary bits ~ 4 decimal digits)
    Fp32s(int32_t ipart, uint32_t fpart_i, uint32_t fpart_d, uint8_t qin)
    {
        rawVal = (ipart << qin) + (fpart_i << qin) / fpart_d;
        _set_q(qin);
    }

    // Compound Arithmetic Operators

    //! @brief        Overload for '+=' operator.
    Fp32s& operator += (Fp32s r)
    {
        // Optimised for when q is the same for both
        // operators (first if statement).
        if(q == r.q)
        {
            // First number smaller
            rawVal = rawVal + r.rawVal;
            // No need to change Q, both are the same
        }
        else if(q > r.q)
        {
            // Second number has smaller Q, so result is in that precision
            rawVal = (rawVal >> (q - r.q)) + r.rawVal;
            // Change Q
            _set_q(r.q);
        }
        else // q < r.q
        {
            // First number has smaller Q, so result is in that precision
            rawVal = rawVal + (r.rawVal >> (r.q - q));
            // No need to change Q
        }
        return *this;
    }

    //! @brief        Overload for '-=' operator.
    Fp32s& operator -= (Fp32s r)
    {
        // Optimised for when q is the same for both
        // operators (first if statement).
        if(q == r.q)
        {
            // Q the same for both numbers
            rawVal = rawVal - r.rawVal;
            // No need to change Q, both are the same
        }
        else if(q > r.q)
        {
            // Second number has smaller Q, so result is in that precision
            rawVal = (rawVal >> (q - r.q)) - r.rawVal;
            // Change Q
            _set_q(r.q);
        }
        else // q < r.q
        {
            // First number has smaller Q, so result is in that precision
            rawVal = rawVal - (r.rawVal >> (r.q - q));
            // No need to change Q
        }
        return *this;
    }

    //! @brief        Overlaod for '*=' operator.
    //! @details    Uses intermediatary casting to int64_t to prevent overflows.
    Fp32s& operator *= (Fp32s r)
    {
        // Optimised for when q is the same for both
        // operators (first if statement).
        if(q == r.q)
        {
            // Q the same for both numbers, shift right by Q
            rawVal = (int32_t)(((int64_t)rawVal * (int64_t)r.rawVal) >> q);
            // No need to change Q, both are the same
        }
        else if(q > r.q)
        {
            // Second number has smaller Q, so result is in that precision
            rawVal = (int32_t)((((int64_t)rawVal >> (q - r.q)) * (int64_t)r.rawVal) >> r.q);
            // Change Q
            _set_q(r.q);
        }
        else // q < r.q
        {
            // First number has smaller Q, so result is in that precision
            rawVal = (int32_t)(((int64_t)rawVal * ((int64_t)r.rawVal >> (r.q - q))) >> q);
            // No need to change Q
        }
        return *this;
    }

    //! @brief        Overlaod for '/=' operator.
    //! @details    Uses intermediatary casting to int64_t to prevent overflows.
    Fp32s& operator /= (Fp32s r)
    {
        // Optimised for when q is the same for both
        // operators (first if statement).
        if(q == r.q)
        {
            // Q the same for both numbers, shift right by Q
            rawVal = (int32_t)((((int64_t)rawVal << q) / (int64_t)r.rawVal));
            // No need to change Q, both are the same
        }
        else if(q > r.q)
        {
            // Second number has smaller Q, so result is in that precision
            rawVal = (int32_t)(((((int64_t)rawVal >> (q - r.q)) << r.q) / (int64_t)r.rawVal));
            // Change Q
            _set_q(r.q);
        }
        else // q < r.q
        {
            // First number has smaller Q, so result is in that precision
            rawVal = (int32_t)(((int64_t)rawVal << q) / ((int64_t)r.rawVal >> (r.q - q)));
            // No need to change Q
        }
        return *this;
    }

    //! @brief        Overlaod for '%=' operator.
    Fp32s& operator %= (Fp32s r)
    {
        // Optimised for when q is the same for both
        // operators (first if statement).
        if(q == r.q)
        {
            // Q the same for both numbers
            rawVal = rawVal % r.rawVal;
            // No need to change Q, both are the same
        }
        else if(q > r.q)
        {
            // Second number has smaller Q, so result is in that precision
            rawVal = (rawVal >> (q - r.q)) % r.rawVal;
            // Change Q
            _set_q(r.q);
        }
        else // q < r.q
        {
            // First number has smaller Q, so result is in that precision
            rawVal = rawVal % (r.rawVal >> (r.q - q));
            // No need to change Q
        }
        return *this;
    }

    // Simple Arithmetic Operators

    //! @brief        Overload for '+' operator.
    //! @details    Uses '+=' operator.
    Fp32s operator + (Fp32s r) const
    {
        Fp32s x = *this;
        x += r;
        return x;
    }

    //! @brief        Overload for '-' operator.
    //! @details    Uses '-=' operator.
    Fp32s operator - (Fp32s r) const
    {
        Fp32s x = *this;
        x -= r;
        return x;
    }

    //! @brief        Overload for '*' operator.
    //! @details    Uses '*=' operator.
    Fp32s operator * (Fp32s r) const
    {
        Fp32s x = *this;
        x *= r;
        return x;
    }

    //! @brief        Overload for '/' operator.
    //! @details    Uses '/=' operator.
    Fp32s operator / (Fp32s r) const
    {
        Fp32s x = *this;
        x /= r;
        return x;
    }

    //! @brief        Overload for '%' operator.
    //! @details    Uses '%=' operator.
    Fp32s operator % (Fp32s r) const
    {
        Fp32s x = *this;
        x %= r;
        return x;
    }

    // Binary Operator Overloads

    //! @brief        Overload for the '==' operator.
    bool operator == (Fp32s r) const
    {
        // Optimised for when q is the same for both
        // operators (first if statement).
        if(q == r.q)
        {
            // Q the same for both numbers
            return rawVal == r.rawVal;
        }
        else if(q > r.q)
        {
            // Second number has smaller Q, so result is in that precision
            return (rawVal >> (q - r.q)) == r.rawVal;
        }
        else // q < r.q
        {
            // First number has smaller Q, so result is in that precision
            return rawVal == (r.rawVal >> (r.q - q));
        }
    }

    //! @brief        Overload for the '!=' operator.
    bool operator != (Fp32s r) const
    {
        // Optimised for when q is the same for both
        // operators (first if statement).
        if(q == r.q)
        {
            // Q the same for both numbers
            return rawVal != r.rawVal;
        }
        else if(q > r.q)
        {
            // Second number has smaller Q, so result is in that precision
            return (rawVal >> (q - r.q)) != r.rawVal;
        }
        else // q < r.q
        {
            // First number has smaller Q, so result is in that precision
            return rawVal != (r.rawVal >> (r.q - q));
        }
    }

    //! @brief        Overload for the '<' operator.
    bool operator < (Fp32s r) const
    {
        // Optimised for when q is the same for both
        // operators (first if statement).
        if(q == r.q)
        {
            // Q the same for both numbers
            return rawVal < r.rawVal;
        }
        else if(q > r.q)
        {
            // Second number has smaller Q, so result is in that precision
            return (rawVal >> (q - r.q)) < r.rawVal;
        }
        else // q < r.q
        {
            // First number has smaller Q, so result is in that precision
            return rawVal < (r.rawVal >> (r.q - q));
        }
    }

    //! @brief        Overload for the '>' operator.
    bool operator > (Fp32s r) const
    {
        // Optimised for when q is the same for both
        // operators (first if statement).
        if(q == r.q)
        {
            // Q the same for both numbers
            return rawVal > r.rawVal;
        }
        else if(q > r.q)
        {
            // Second number has smaller Q, so result is in that precision
            return (rawVal >> (q - r.q)) > r.rawVal;
        }
        else // q < r.q
        {
            // First number has smaller Q, so result is in that precision
            return rawVal > (r.rawVal >> (r.q - q));
        }
    }

    //! @brief        Overload for the '<=' operator.
    bool operator <= (Fp32s r) const
    {
        // Optimised for when q is the same for both
        // operators (first if statement).
        if(q == r.q)
        {
            // Q the same for both numbers
            return rawVal <= r.rawVal;
        }
        else if(q > r.q)
        {
            // Second number has smaller Q, so result is in that precision
            return (rawVal >> (q - r.q)) <= r.rawVal;
        }
        else // q < r.q
        {
            // First number has smaller Q, so result is in that precision
            return rawVal <= (r.rawVal >> (r.q - q));
        }
    }

    //! @brief        Overload for the '>=' operator.
    bool operator >= (Fp32s r) const
    {
        // Optimised for when q is the same for both
        // operators (first if statement).
        if(q == r.q)
        {
            // Q the same for both numbers
            return rawVal >= r.rawVal;
        }
        else if(q > r.q)
        {
            // Second number has smaller Q, so result is in that precision
            return (rawVal >> (q - r.q)) >= r.rawVal;
        }
        else // q < r.q
        {
            // First number has smaller Q, so result is in that precision
            return rawVal >= (r.rawVal >> (r.q - q));
        }
    }

    // Explicit Conversion Operator Overloads (casts)

    //! @brief        Conversion operator from fixed-point to int32_t.
    operator int32_t()
    {
        // Right-shift to get rid of all the decimal bits
        return (rawVal >> q);
    }

    //! @brief        Conversion operator from fixed-point to int64_t.
    operator int64_t()
    {
        // Right-shift to get rid of all the decimal bits
        return (int64_t)(rawVal >> q);
    }

    //! @brief        Conversion operator from fixed-point to float.
    //! @note        Similar to double conversion.
    operator float()
    {
        return (float)rawVal / (float)(1 << q);
    }

    //! @brief        Conversion operator from fixed-point to double.
    //! @note        Similar to float conversion.
    operator double()
    {
        return (double)rawVal / (double)(1 << q);
    }

    // Returns the integer part of the number
    int32_t ipart()
    {
        // Right-shift to get rid of all the decimal bits
        return (rawVal >> q);
    }

    // Returns the fractional part of the number as integer
    uint32_t fpart()
    {
        uint32_t res;
        uint64_t res64 = (uint64_t)(rawVal % (1 << q)) * (uint64_t)fpart_divider();
        if (qd < QDEC_MAX) {
            res64 *= 10; // increase precision - prepare for rounding
            res = (uint32_t)(res64 >> q);
            // rounding the result (minus one decimal digit)
            uint8_t rem = res % 10;
            res = res / 10;
            if (rem >= 5) res += 1;
        } else {
            res = (uint32_t)(res64 >> q);
        }
        return res;
    }

    // Returns the fractional part divider
    uint32_t fpart_divider()
    {
        return power10[qd];
    }
}; // class
} // namespace Fp

#endif // #ifndef FP32S_H
// EOF
