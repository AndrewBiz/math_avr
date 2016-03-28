/*
    Math functionality test on Arduino Uno
   Andrew Bizyaev (ANB) github.com/andrewbiz
*/
#include "math.h"

#define ARRAY_COUNT(a) (sizeof(a)/(sizeof(a[0])))

const uint32_t clock = 125000000LL;

void test_math1(void);
void test_math2(uint32_t num_test_steps);
void test_math3(uint32_t num_test_steps);
void test_math4(void);


void setup()
{
    Log.Init(LOGLEVEL, 38400L, LOG_PRINT_TS, LOG_AUTO_LN);
} // setup

void loop()
{
    delay(3000);
    // test_math2(100);
    test_math3(300);


} // function loop

void print_bin64(const char * vname, uint64_t v)
{
    Log.Info(F("%s: %y-%y-%y-%y-%y-%y-%y-%y"), vname,\
        Byte8(v),Byte7(v),Byte6(v),Byte5(v),\
        Byte4(v),Byte3(v),Byte2(v),Byte1(v));
}

void print_bin32(const char * vname, uint32_t v)
{
    Log.Info(F("%s: %y-%y-%y-%y"), vname,\
        Byte4(v),Byte3(v),Byte2(v),Byte1(v));
}

// test scenarios
void test_math1() {
    Fp::Fp32s fp_test1 = Fp::Fp32s(9, 2935, 10000, 14);
    float f_test1 = (float)fp_test1;
    int32_t i_test1 = 92935L;
    Fp::Fp32s fp_test2 = Fp::Fp32s(0, 2935, 10000, 14);
    float f_test2 = (float)fp_test2;
    int32_t i_test2 = 2935L;
    Fp::Fp32s fp_test3 = fp_test1 * fp_test2;
    float f_test3 = (float)fp_test3;
    int32_t i_test3 = i_test1 * i_test2;

    print_bin64("UINT64_MAX", UINT64_MAX);
    print_bin64("INT64_MAX", INT64_MAX);
    print_bin64("INT64_MIN", INT64_MIN);
    print_bin32("UINT32_MAX", UINT32_MAX);
    print_bin32("INT32_MAX", INT32_MAX);
    print_bin32("INT32_MIN", INT32_MIN);
    Log.Info(F("UINT32_MAX: %u, INT32_MAX: %l, INT32_MIN: %l"), UINT32_MAX, INT32_MAX, INT32_MIN);

    Log.Info(F("!!!*******************************************!!!"));
    Serial.println(f_test1);
    print_bin32("fp_test1", fp_test1.rawVal);
    Log.Info(F("fp_test1.rawVal: %u, q: %d"), fp_test1.rawVal, fp_test1.q);
    Log.Info(F("fp_test1 to int32_t: %u"), int32_t(fp_test1));
    Log.Info(F("fp_test1.ipart: %u"), fp_test1.ipart());
    print_bin32("fp_test1.ipart", fp_test1.ipart());
    Log.Info(F("fp_test1.fpart: %u"), fp_test1.fpart());
    print_bin32("fp_test1.fpart", fp_test1.fpart());
    Log.Info(F("fp_test1 in 2 parts: %u+%u/%u"), fp_test1.ipart(), fp_test1.fpart(), fp_test1.fpart_divider());

    Log.Info(F("*************************************************"));
    Serial.println(f_test2);
    print_bin32("fp_test2", fp_test2.rawVal);
    Log.Info(F("fp_test2.rawVal: %u, q: %d"), fp_test2.rawVal, fp_test2.q);
    Log.Info(F("fp_test2 to int32_t: %u"), int32_t(fp_test2));
    Log.Info(F("fp_test2.ipart: %u"), fp_test2.ipart());
    print_bin32("fp_test2.ipart", fp_test2.ipart());
    Log.Info(F("fp_test2.fpart: %u"), fp_test2.fpart());
    print_bin32("fp_test2.fpart", fp_test2.fpart());
    Log.Info(F("fp_test2 in 2 parts: %u+%u/%u"), fp_test2.ipart(), fp_test2.fpart(), fp_test2.fpart_divider());

    Log.Info(F("*************************************************"));
    Serial.println(f_test3);
    print_bin32("fp_test3", fp_test3.rawVal);
    Log.Info(F("fp_test3.rawVal: %u, q: %d"), fp_test3.rawVal, fp_test3.q);
    Log.Info(F("fp_test3 to int32_t: %u"), int32_t(fp_test3));
    Log.Info(F("fp_test3.ipart: %u"), fp_test3.ipart());
    print_bin32("fp_test3.ipart", fp_test3.ipart());
    Log.Info(F("fp_test3.fpart: %u"), fp_test3.fpart());
    print_bin32("fp_test3.fpart", fp_test3.fpart());
    Log.Info(F("fp_test3 in 2 parts: %u+%u/%u"), fp_test3.ipart(), fp_test3.fpart(), fp_test3.fpart_divider());

    Serial.println(f_test1 * f_test2);
    Serial.println(i_test1);
    Serial.println(i_test2);
    Serial.println(i_test3);
}

void test_math2(uint32_t num_test_steps) {
    int32_t freq_test[] = {0, 1000, 9995, 50000, 100000, 150995, 1000000, 1499995, 10000000, 19999995};
    Log.Info(F("ITERATION: STEP: FREQ(FL): TWORD(INT): FREQ(100INT): TWORD(INT/100): FREQ(FP): FREQ(FP->F)"));
    float freqf;
    uint32_t tword_freqf;
    uint32_t freq100i;
    uint32_t tword_freq100i;
    //Fp::Fp32s fp2pow32 = Fp::Fp32s(UINT32_MAX, 0);
    Fp::Fp32s freqfp_step = Fp::Fp32s(0, 1, 100, 7); // 0.01, 7bits precision
    //Fp::Fp32s tword_freqfp = Fp::Fp32s(0, 2);  // init as 0, 2 bits presision tword = (0; ~1030792151)

    char fbuf1[13];
    char fbuf2[13];
    char fbuf3[13];
    int count = ARRAY_COUNT(freq_test);
    for (int t = 0;  t < count; t++) {
        freqf = (float)freq_test[t];
        freq100i = 100 * freq_test[t];
        Fp::Fp32s freqfp = Fp::Fp32s(freq_test[t], 7); //7 bits precizion ~ 2 decimal places after point
        for (size_t i = 0; i < num_test_steps; i++) {
            // float calc
            tword_freqf = freqf * UINT32_MAX / clock;
            dtostrf(freqf, 0, 2, fbuf1);
            // int calc
            tword_freq100i = (((uint64_t)freq100i << 32) / clock) / 100L;
            // fixed point calc
            //Fp::Fp32s tword_freqfp = Fp::Fp32s(0, 2935, 10000, 14);
            sprintf(fbuf2, "%02lu", freqfp.fpart());
            dtostrf((float)freqfp, 0, 2, fbuf3);

            Log.Info(F("%u: \t%d: \t%s: \t%u: \t%u: \t%u: \t%u.%s: \t%s"), \
                    freq_test[t], i, fbuf1, tword_freqf, freq100i, tword_freq100i,\
                    freqfp.ipart(), fbuf2, fbuf3);
            freqf += 0.01F;
            freq100i += 1;
            freqfp += freqfp_step;
        }
    }
}

void test_math3(uint32_t num_test_steps) {
    int32_t tword_test[] = {0, 34300, 343600, 1717900, 3435900, 5188300, 34359700, 51539600, 343597300, 687194700};
    Log.Info(F("ITERATION: STEP: TWORD(INT): FREQ(F): FREQ(INT100)"));
    uint32_t tword;
    float freqf;
    uint32_t freq100i;

    char fbuf1[13];
    int count = ARRAY_COUNT(tword_test);
    for (int t = 0;  t < count; t++) {
        tword = tword_test[t];
        for (size_t i = 0; i < num_test_steps; i++) {
            // float calc
            freqf = (float)tword * (float)clock / UINT32_MAX;
            dtostrf(freqf, 0, 2, fbuf1);
            // int calc
            freq100i = ((uint64_t)tword * (uint64_t)clock * 100) >> 32;

            Log.Info(F("%u: \t%d: \t%u: \t%s: \t%u"),\
                    tword_test[t], i, tword, fbuf1, freq100i);
            tword += 1;
        }
    }
}
