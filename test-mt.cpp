/* 
 * The Mersenne Twister pseudo-random number generator (PRNG)
 *
 * This is an implementation of fast PRNG called MT19937,
 * meaning it has a period of 2^19937-1, which is a Mersenne
 * prime.
 *
 * This PRNG is fast and suitable for non-cryptographic code.
 * For instance, it would be perfect for Monte Carlo simulations,
 * etc.
 *
 * Written by Christian Stigen Larsen
 * 2012-01-11 -- http://csl.sublevel3.org
 *
 * Distributed under the modified BSD license.
 */

#include <stdio.h>

#define __STDC_FORMAT_MACROS
#include <inttypes.h>

#include "mersenne-twister.h"

/*
 * Reference mersenne twister numbers for seed = 1.
 */
uint32_t expected[] = {
  1791095845, 4282876139, 3093770124, 4005303368,     491263,
   550290313, 1298508491, 4290846341,  630311759, 1013994432,
   396591248, 1703301249,  799981516, 1666063943, 1484172013,
  2876537340, 1704103302, 4018109721, 2314200242, 3634877716,
  1800426750, 1345499493, 2942995346, 2252917204,  878115723,
  1904615676, 3771485674,  986026652,  117628829, 2295290254,
  2879636018, 3925436996, 1792310487, 1963679703, 2399554537,
  1849836273,  602957303, 4033523166,  850839392, 3343156310,
  3439171725, 3075069929, 4158651785, 3447817223, 1346146623,
   398576445, 2973502998, 2225448249, 3764062721, 3715233664,
  3842306364, 3561158865,  365262088, 3563119320,  167739021,
  1172740723,  729416111,  254447594, 3771593337, 2879896008,
   422396446, 2547196999, 1808643459, 2884732358, 4114104213,
  1768615473, 2289927481,  848474627, 2971589572, 1243949848,
  1355129329,  610401323, 2948499020, 3364310042, 3584689972,
  1771840848,   78547565,  146764659, 3221845289, 2680188370,
  4247126031, 2837408832, 3213347012, 1282027545, 1204497775,
  1916133090, 3389928919,  954017671,  443352346,  315096729,
  1923688040, 2015364118, 3902387977,  413056707, 1261063143,
  3879945342, 1235985687,  513207677,  558468452, 2253996187,
    83180453,  359158073, 2915576403, 3937889446,  908935816,
  3910346016, 1140514210, 1283895050, 2111290647, 2509932175,
   229190383, 2430573655, 2465816345, 2636844999,  630194419,
  4108289372, 2531048010, 1120896190, 3005439278,  992203680,
   439523032, 2291143831, 1778356919, 4079953217, 2982425969,
  2117674829, 1778886403, 2321861504,  214548472, 3287733501,
  2301657549,  194758406, 2850976308,  601149909, 2211431878,
  3403347458, 4057003596,  127995867, 2519234709, 3792995019,
  3880081671, 2322667597,  590449352, 1924060235,  598187340,
  3831694379, 3467719188, 1621712414, 1708008996, 2312516455,
   710190855, 2801602349, 3983619012, 1551604281, 1493642992,
  2452463100, 3224713426, 2739486816, 3118137613,  542518282,
  3793770775, 2964406140, 2678651729, 2782062471, 3225273209,
  1520156824, 1498506954, 3278061020, 1159331476, 1531292064,
  3847801996, 3233201345, 1838637662, 3785334332, 4143956457,
    50118808, 2849459538, 2139362163, 2670162785,  316934274,
   492830188, 3379930844, 4078025319,  275167074, 1932357898,
  1526046390, 2484164448, 4045158889, 1752934226, 1631242710,
  1018023110, 3276716738, 3879985479, 3313975271, 2463934640,
  1294333494,   12327951, 3318889349, 2650617233,  656828586
};

/*
 * Reference mersenne twister numbers for seed = 1.
 * The index of each number is doubled, so the first index is 0, then 1, 3, 7,
 * 15, 31, ...
 *
 * Numbers were generated using the reference code at 
 * http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/MT2002/CODES/mt19937ar.c
 */
uint32_t doubled_reference_seed1[] = {
  /* n=         0 */ 1791095845,
  /* n=         1 */ 4282876139,
  /* n=         3 */ 4005303368,
  /* n=         7 */ 4290846341,
  /* n=        15 */ 2876537340,
  /* n=        31 */ 3925436996,
  /* n=        63 */ 2884732358,
  /* n=       127 */ 2321861504,
  /* n=       255 */ 1195370327,
  /* n=       511 */  899765072,
  /* n=      1023 */ 1714350790,
  /* n=      2047 */ 3742484479,
  /* n=      4095 */ 3962329154,
  /* n=      8191 */  740139619,
  /* n=     16383 */ 3156554771,
  /* n=     32767 */ 2155441805,
  /* n=     65535 */  181306153,
  /* n=    131071 */ 1493556421,
  /* n=    262143 */ 1963136003,
  /* n=    524287 */ 2991783559,
  /* n=   1048575 */ 1708194087,
  /* n=   2097151 */  712866985,
  /* n=   4194303 */ 2195311408,
  /* n=   8388607 */ 2899694794,
  /* n=  16777215 */ 1460185617,
  /* n=  33554431 */ 1301553711,
  /* n=  67108863 */  669321401,
  /* n= 134217727 */ 2613167558,
  /* n= 268435455 */ 2861867968,
  /* n= 536870911 */  175437983,
  /* n=1073741823 */  382741236,
  /* n=2147483647 */ 3139600069,
  /* n=4294967295 */ 3468780828
};

int main()
{
  uint32_t errors = 0;
  int seed = 1;
  srand(seed);

  printf("Mersenne Twister -- printing the first 200 numbers seed %d\n\n",
    seed);

  for ( int n=0; n<200; ++n ) {
    uint32_t r = rand_u32();

    bool error = r != expected[n];
    if ( error ) ++errors;

    printf("%10u%c%c", r,
      error? '*' : ' ',
      n % 5 == 4 ? '\n' : ' ');
    fflush(stdout);
  }

  printf("\nGenerating 64-bit pseudo-random numbers\n\n");
  for ( int n=0; n<27; ++n )
    printf("%20" PRIu64 "%c", rand_u64(), n % 3 == 2 ? '\n' : ' ');

  printf("\nFloat values in range [0..1]\n\n");
  for ( int n=0; n<40; ++n )
    printf("%f%c", randf_cc(), n % 5 == 4 ? '\n' : ' ');

  printf("\nDouble values in range [0..1]\n\n");
  for ( int n=0; n<40; ++n )
    printf("%f%c", randd_cc(), n % 5 == 4 ? '\n' : ' ');

  printf("\nChecking reference numbers for seed 1 (may take some time)\n\n");
  srand(1);
  for ( size_t n=0, target=1, idx=0; n<=0xffffffff; ++n ) {
    uint32_t r = rand_u32();

    if ( n != (target-1) )
      continue;

    bool error = r != doubled_reference_seed1[idx];
    if ( error ) ++errors;

    printf("%11zu %11u%c %c", n, r,
      error? '*' : ' ',
      idx % 4 == 3 ? '\n' : ' ');
    fflush(stdout);
    target *= 2;
    ++idx;
  }

  printf("\nFound %u incorrect numbers\n\n", errors);
  return errors > 0;
}
