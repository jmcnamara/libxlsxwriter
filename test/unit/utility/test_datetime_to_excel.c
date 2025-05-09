/*
 * Tests for the libxlsxwriter library.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * Copyright 2014-2025, John McNamara, jmcnamara@cpan.org.
 *
 */

#include "../ctest.h"
#include "../helper.h"

#include "../../../include/xlsxwriter/utility.h"


// Test _datetime_to_excel_date().
CTEST(utility, test_datetime_date_and_time) {

    double got;
    lxw_datetime *datetime;

    TEST_DATETIME(1899, 12, 31,  0,  0,  0.000, 0);
    TEST_DATETIME(1982,  8, 25,  0, 15, 20.213, 30188.010650613425);
    TEST_DATETIME(2065,  4, 19,  0, 16, 48.290, 60376.011670023145);
    TEST_DATETIME(2147, 12, 15,  0, 55, 25.446, 90565.038488958337);
    TEST_DATETIME(2230,  8, 10,  1,  2, 46.891, 120753.04359827546);
    TEST_DATETIME(2313,  4,  6,  1,  4, 15.597, 150942.04462496529);
    TEST_DATETIME(2395, 11, 30,  1,  9, 40.889, 181130.04838991899);
    TEST_DATETIME(2478,  7, 25,  1, 11, 32.560, 211318.04968240741);
    TEST_DATETIME(2561,  3, 21,  1, 30, 19.169, 241507.06272186342);
    TEST_DATETIME(2643, 11, 15,  1, 48, 25.580, 271695.07529606484);
    TEST_DATETIME(2726,  7, 12,  2,  3, 31.919, 301884.08578609955);
    TEST_DATETIME(2809,  3,  6,  2, 11, 11.986, 332072.09111094906);
    TEST_DATETIME(2891, 10, 31,  2, 24, 37.095, 362261.10042934027);
    TEST_DATETIME(2974,  6, 26,  2, 35,  7.220, 392449.10772245371);
    TEST_DATETIME(3057,  2, 19,  2, 45, 12.109, 422637.1147234838);
    TEST_DATETIME(3139, 10, 17,  3,  6, 39.990, 452826.12962951389);
    TEST_DATETIME(3222,  6, 11,  3,  8,  8.251, 483014.13065105322);
    TEST_DATETIME(3305,  2,  5,  3, 19, 12.576, 513203.13834);
    TEST_DATETIME(3387, 10,  1,  3, 29, 42.574, 543391.14563164348);
    TEST_DATETIME(3470,  5, 27,  3, 37, 30.813, 573579.15105107636);
    TEST_DATETIME(3553,  1, 21,  4, 14, 38.231, 603768.17683137732);
    TEST_DATETIME(3635,  9, 16,  4, 16, 28.559, 633956.17810832174);
    TEST_DATETIME(3718,  5, 13,  4, 17, 58.222, 664145.17914608796);
    TEST_DATETIME(3801,  1,  6,  4, 21, 41.794, 694333.18173372687);
    TEST_DATETIME(3883,  9,  2,  4, 56, 35.792, 724522.20596981479);
    TEST_DATETIME(3966,  4, 28,  5, 25, 14.885, 754710.2258667245);
    TEST_DATETIME(4048, 12, 21,  5, 26,  5.724, 784898.22645513888);
    TEST_DATETIME(4131,  8, 18,  5, 46, 44.068, 815087.24078782403);
    TEST_DATETIME(4214,  4, 13,  5, 48,  1.141, 845275.24167987274);
    TEST_DATETIME(4296, 12,  7,  5, 53, 52.315, 875464.24574438657);
    TEST_DATETIME(4379,  8,  3,  6, 14, 48.580, 905652.26028449077);
    TEST_DATETIME(4462,  3, 28,  6, 46, 15.738, 935840.28212659725);
    TEST_DATETIME(4544, 11, 22,  7, 31, 20.407, 966029.31343063654);
    TEST_DATETIME(4627,  7, 19,  7, 58, 33.754, 996217.33233511576);
    TEST_DATETIME(4710,  3, 15,  8,  7, 43.130, 1026406.3386936343);
    TEST_DATETIME(4792, 11,  7,  8, 29, 11.091, 1056594.3536005903);
    TEST_DATETIME(4875,  7,  4,  9,  8, 15.328, 1086783.3807329629);
    TEST_DATETIME(4958,  2, 27,  9, 30, 41.781, 1116971.3963169097);
    TEST_DATETIME(5040, 10, 23,  9, 34,  4.462, 1147159.3986627546);
    TEST_DATETIME(5123,  6, 20,  9, 37, 23.945, 1177348.4009715857);
    TEST_DATETIME(5206,  2, 12,  9, 37, 56.655, 1207536.4013501736);
    TEST_DATETIME(5288, 10,  8,  9, 45, 12.230, 1237725.406391551);
    TEST_DATETIME(5371,  6,  4,  9, 54, 14.782, 1267913.412671088);
    TEST_DATETIME(5454,  1, 28,  9, 54, 22.108, 1298101.4127558796);
    TEST_DATETIME(5536,  9, 24, 10,  1, 36.151, 1328290.4177795255);
    TEST_DATETIME(5619,  5, 20, 12,  9, 48.602, 1358478.5068125231);
    TEST_DATETIME(5702,  1, 14, 12, 34,  8.549, 1388667.5237100578);
    TEST_DATETIME(5784,  9,  8, 12, 56,  6.495, 1418855.5389640625);
    TEST_DATETIME(5867,  5,  6, 12, 58, 58.217, 1449044.5409515856);
    TEST_DATETIME(5949, 12, 30, 12, 59, 54.263, 1479232.5416002662);
    TEST_DATETIME(6032,  8, 24, 13, 34, 41.331, 1509420.5657561459);
    TEST_DATETIME(6115,  4, 21, 13, 58, 28.601, 1539609.5822754744);
    TEST_DATETIME(6197, 12, 14, 14,  2, 16.899, 1569797.5849178126);
    TEST_DATETIME(6280,  8, 10, 14, 36, 17.444, 1599986.6085352316);
    TEST_DATETIME(6363,  4,  6, 14, 37, 57.451, 1630174.60969272);
    TEST_DATETIME(6445, 11, 30, 14, 57, 42.757, 1660363.6234115392);
    TEST_DATETIME(6528,  7, 26, 15, 10, 48.307, 1690551.6325035533);
    TEST_DATETIME(6611,  3, 22, 15, 14, 39.890, 1720739.635183912);
    TEST_DATETIME(6693, 11, 15, 15, 19, 47.988, 1750928.6387498612);
    TEST_DATETIME(6776,  7, 11, 16,  4, 24.344, 1781116.6697262037);
    TEST_DATETIME(6859,  3,  7, 16, 22, 23.952, 1811305.6822216667);
    TEST_DATETIME(6941, 10, 31, 16, 29, 55.999, 1841493.6874536921);
    TEST_DATETIME(7024,  6, 26, 16, 58, 20.259, 1871681.7071789235);
    TEST_DATETIME(7107,  2, 21, 17,  4,  2.415, 1901870.7111390624);
    TEST_DATETIME(7189, 10, 16, 17, 18, 29.630, 1932058.7211762732);
    TEST_DATETIME(7272,  6, 11, 17, 47, 21.323, 1962247.7412190163);
    TEST_DATETIME(7355,  2,  5, 17, 53, 29.866, 1992435.7454845603);
    TEST_DATETIME(7437, 10,  2, 17, 53, 41.076, 2022624.7456143056);
    TEST_DATETIME(7520,  5, 28, 17, 55,  6.044, 2052812.7465977315);
    TEST_DATETIME(7603,  1, 21, 18, 14, 49.151, 2083000.7602910995);
    TEST_DATETIME(7685,  9, 16, 18, 17, 45.738, 2113189.7623349307);
    TEST_DATETIME(7768,  5, 12, 18, 29, 59.700, 2143377.7708298611);
    TEST_DATETIME(7851,  1,  7, 18, 33, 21.233, 2173566.773162419);
    TEST_DATETIME(7933,  9,  2, 19, 14, 24.673, 2203754.8016744559);
    TEST_DATETIME(8016,  4, 27, 19, 17, 12.816, 2233942.8036205554);
    TEST_DATETIME(8098, 12, 22, 19, 23, 36.418, 2264131.8080603937);
    TEST_DATETIME(8181,  8, 17, 19, 46, 25.908, 2294319.8239109721);
    TEST_DATETIME(8264,  4, 13, 20,  7, 47.314, 2324508.8387420601);
    TEST_DATETIME(8346, 12,  8, 20, 31, 37.603, 2354696.855296331);
    TEST_DATETIME(8429,  8,  3, 20, 39, 57.770, 2384885.8610853008);
    TEST_DATETIME(8512,  3, 29, 20, 50, 17.067, 2415073.8682530904);
    TEST_DATETIME(8594, 11, 22, 21,  2, 57.827, 2445261.8770581828);
    TEST_DATETIME(8677,  7, 19, 21, 23,  5.519, 2475450.8910360998);
    TEST_DATETIME(8760,  3, 14, 21, 34, 49.572, 2505638.8991848612);
    TEST_DATETIME(8842, 11,  8, 21, 39,  5.944, 2535827.9021521294);
    TEST_DATETIME(8925,  7,  4, 21, 39, 18.426, 2566015.9022965971);
    TEST_DATETIME(9008,  2, 28, 21, 46,  7.769, 2596203.9070343636);
    TEST_DATETIME(9090, 10, 24, 21, 57, 55.662, 2626392.9152275696);
    TEST_DATETIME(9173,  6, 19, 22, 19, 11.732, 2656580.9299968979);
    TEST_DATETIME(9256,  2, 13, 22, 23, 51.376, 2686769.9332335186);
    TEST_DATETIME(9338, 10,  9, 22, 27, 58.771, 2716957.9360968866);
    TEST_DATETIME(9421,  6,  5, 22, 43, 30.392, 2747146.9468795368);
    TEST_DATETIME(9504,  1, 30, 22, 48, 25.834, 2777334.9502990046);
    TEST_DATETIME(9586,  9, 24, 22, 53, 51.727, 2807522.9540709145);
    TEST_DATETIME(9669,  5, 20, 23, 12, 56.536, 2837711.9673210187);
    TEST_DATETIME(9752,  1, 14, 23, 15, 54.109, 2867899.9693762613);
    TEST_DATETIME(9834,  9, 10, 23, 17, 12.632, 2898088.9702850925);
    TEST_DATETIME(9999, 12, 31, 23, 59, 59.000, 2958465.999988426);
}

CTEST(utility, test_datetime_date_only) {

    double got;
    lxw_datetime *datetime;

    // Params = (year, month, day, excel_date).
    TEST_DATETIME_DATE(1899, 12, 31, 0);
    TEST_DATETIME_DATE(1900,  1,  1, 1);
    TEST_DATETIME_DATE(1900,  2, 27, 58);
    TEST_DATETIME_DATE(1900,  2, 28, 59);
    TEST_DATETIME_DATE(1900,  3,  1, 61);
    TEST_DATETIME_DATE(1900,  3,  2, 62);
    TEST_DATETIME_DATE(1900,  3, 11, 71);
    TEST_DATETIME_DATE(1900,  4,  8, 99);
    TEST_DATETIME_DATE(1900,  9, 12, 256);
    TEST_DATETIME_DATE(1901,  5,  3, 489);
    TEST_DATETIME_DATE(1901, 10, 13, 652);
    TEST_DATETIME_DATE(1902,  2, 15, 777);
    TEST_DATETIME_DATE(1902,  6,  6, 888);
    TEST_DATETIME_DATE(1902,  9, 25, 999);
    TEST_DATETIME_DATE(1902,  9, 27, 1001);
    TEST_DATETIME_DATE(1903,  4, 26, 1212);
    TEST_DATETIME_DATE(1903,  8,  5, 1313);
    TEST_DATETIME_DATE(1903, 12, 31, 1461);
    TEST_DATETIME_DATE(1904,  1,  1, 1462);
    TEST_DATETIME_DATE(1904,  2, 28, 1520);
    TEST_DATETIME_DATE(1904,  2, 29, 1521);
    TEST_DATETIME_DATE(1904,  3,  1, 1522);
    TEST_DATETIME_DATE(1907,  2, 27, 2615);
    TEST_DATETIME_DATE(1907,  2, 28, 2616);
    TEST_DATETIME_DATE(1907,  3,  1, 2617);
    TEST_DATETIME_DATE(1907,  3,  2, 2618);
    TEST_DATETIME_DATE(1907,  3,  3, 2619);
    TEST_DATETIME_DATE(1907,  3,  4, 2620);
    TEST_DATETIME_DATE(1907,  3,  5, 2621);
    TEST_DATETIME_DATE(1907,  3,  6, 2622);
    TEST_DATETIME_DATE(1999,  1,  1, 36161);
    TEST_DATETIME_DATE(1999,  1, 31, 36191);
    TEST_DATETIME_DATE(1999,  2,  1, 36192);
    TEST_DATETIME_DATE(1999,  2, 28, 36219);
    TEST_DATETIME_DATE(1999,  3,  1, 36220);
    TEST_DATETIME_DATE(1999,  3, 31, 36250);
    TEST_DATETIME_DATE(1999,  4,  1, 36251);
    TEST_DATETIME_DATE(1999,  4, 30, 36280);
    TEST_DATETIME_DATE(1999,  5,  1, 36281);
    TEST_DATETIME_DATE(1999,  5, 31, 36311);
    TEST_DATETIME_DATE(1999,  6,  1, 36312);
    TEST_DATETIME_DATE(1999,  6, 30, 36341);
    TEST_DATETIME_DATE(1999,  7,  1, 36342);
    TEST_DATETIME_DATE(1999,  7, 31, 36372);
    TEST_DATETIME_DATE(1999,  8,  1, 36373);
    TEST_DATETIME_DATE(1999,  8, 31, 36403);
    TEST_DATETIME_DATE(1999,  9,  1, 36404);
    TEST_DATETIME_DATE(1999,  9, 30, 36433);
    TEST_DATETIME_DATE(1999, 10,  1, 36434);
    TEST_DATETIME_DATE(1999, 10, 31, 36464);
    TEST_DATETIME_DATE(1999, 11,  1, 36465);
    TEST_DATETIME_DATE(1999, 11, 30, 36494);
    TEST_DATETIME_DATE(1999, 12,  1, 36495);
    TEST_DATETIME_DATE(1999, 12, 31, 36525);
    TEST_DATETIME_DATE(2000,  1,  1, 36526);
    TEST_DATETIME_DATE(2000,  1, 31, 36556);
    TEST_DATETIME_DATE(2000,  2,  1, 36557);
    TEST_DATETIME_DATE(2000,  2, 29, 36585);
    TEST_DATETIME_DATE(2000,  3,  1, 36586);
    TEST_DATETIME_DATE(2000,  3, 31, 36616);
    TEST_DATETIME_DATE(2000,  4,  1, 36617);
    TEST_DATETIME_DATE(2000,  4, 30, 36646);
    TEST_DATETIME_DATE(2000,  5,  1, 36647);
    TEST_DATETIME_DATE(2000,  5, 31, 36677);
    TEST_DATETIME_DATE(2000,  6,  1, 36678);
    TEST_DATETIME_DATE(2000,  6, 30, 36707);
    TEST_DATETIME_DATE(2000,  7,  1, 36708);
    TEST_DATETIME_DATE(2000,  7, 31, 36738);
    TEST_DATETIME_DATE(2000,  8,  1, 36739);
    TEST_DATETIME_DATE(2000,  8, 31, 36769);
    TEST_DATETIME_DATE(2000,  9,  1, 36770);
    TEST_DATETIME_DATE(2000,  9, 30, 36799);
    TEST_DATETIME_DATE(2000, 10,  1, 36800);
    TEST_DATETIME_DATE(2000, 10, 31, 36830);
    TEST_DATETIME_DATE(2000, 11,  1, 36831);
    TEST_DATETIME_DATE(2000, 11, 30, 36860);
    TEST_DATETIME_DATE(2000, 12,  1, 36861);
    TEST_DATETIME_DATE(2000, 12, 31, 36891);
    TEST_DATETIME_DATE(2001,  1,  1, 36892);
    TEST_DATETIME_DATE(2001,  1, 31, 36922);
    TEST_DATETIME_DATE(2001,  2,  1, 36923);
    TEST_DATETIME_DATE(2001,  2, 28, 36950);
    TEST_DATETIME_DATE(2001,  3,  1, 36951);
    TEST_DATETIME_DATE(2001,  3, 31, 36981);
    TEST_DATETIME_DATE(2001,  4,  1, 36982);
    TEST_DATETIME_DATE(2001,  4, 30, 37011);
    TEST_DATETIME_DATE(2001,  5,  1, 37012);
    TEST_DATETIME_DATE(2001,  5, 31, 37042);
    TEST_DATETIME_DATE(2001,  6,  1, 37043);
    TEST_DATETIME_DATE(2001,  6, 30, 37072);
    TEST_DATETIME_DATE(2001,  7,  1, 37073);
    TEST_DATETIME_DATE(2001,  7, 31, 37103);
    TEST_DATETIME_DATE(2001,  8,  1, 37104);
    TEST_DATETIME_DATE(2001,  8, 31, 37134);
    TEST_DATETIME_DATE(2001,  9,  1, 37135);
    TEST_DATETIME_DATE(2001,  9, 30, 37164);
    TEST_DATETIME_DATE(2001, 10,  1, 37165);
    TEST_DATETIME_DATE(2001, 10, 31, 37195);
    TEST_DATETIME_DATE(2001, 11,  1, 37196);
    TEST_DATETIME_DATE(2001, 11, 30, 37225);
    TEST_DATETIME_DATE(2001, 12,  1, 37226);
    TEST_DATETIME_DATE(2001, 12, 31, 37256);
    TEST_DATETIME_DATE(2400,  1,  1, 182623);
    TEST_DATETIME_DATE(2400,  1, 31, 182653);
    TEST_DATETIME_DATE(2400,  2,  1, 182654);
    TEST_DATETIME_DATE(2400,  2, 29, 182682);
    TEST_DATETIME_DATE(2400,  3,  1, 182683);
    TEST_DATETIME_DATE(2400,  3, 31, 182713);
    TEST_DATETIME_DATE(2400,  4,  1, 182714);
    TEST_DATETIME_DATE(2400,  4, 30, 182743);
    TEST_DATETIME_DATE(2400,  5,  1, 182744);
    TEST_DATETIME_DATE(2400,  5, 31, 182774);
    TEST_DATETIME_DATE(2400,  6,  1, 182775);
    TEST_DATETIME_DATE(2400,  6, 30, 182804);
    TEST_DATETIME_DATE(2400,  7,  1, 182805);
    TEST_DATETIME_DATE(2400,  7, 31, 182835);
    TEST_DATETIME_DATE(2400,  8,  1, 182836);
    TEST_DATETIME_DATE(2400,  8, 31, 182866);
    TEST_DATETIME_DATE(2400,  9,  1, 182867);
    TEST_DATETIME_DATE(2400,  9, 30, 182896);
    TEST_DATETIME_DATE(2400, 10,  1, 182897);
    TEST_DATETIME_DATE(2400, 10, 31, 182927);
    TEST_DATETIME_DATE(2400, 11,  1, 182928);
    TEST_DATETIME_DATE(2400, 11, 30, 182957);
    TEST_DATETIME_DATE(2400, 12,  1, 182958);
    TEST_DATETIME_DATE(2400, 12, 31, 182988);
    TEST_DATETIME_DATE(4000,  1,  1, 767011);
    TEST_DATETIME_DATE(4000,  1, 31, 767041);
    TEST_DATETIME_DATE(4000,  2,  1, 767042);
    TEST_DATETIME_DATE(4000,  2, 29, 767070);
    TEST_DATETIME_DATE(4000,  3,  1, 767071);
    TEST_DATETIME_DATE(4000,  3, 31, 767101);
    TEST_DATETIME_DATE(4000,  4,  1, 767102);
    TEST_DATETIME_DATE(4000,  4, 30, 767131);
    TEST_DATETIME_DATE(4000,  5,  1, 767132);
    TEST_DATETIME_DATE(4000,  5, 31, 767162);
    TEST_DATETIME_DATE(4000,  6,  1, 767163);
    TEST_DATETIME_DATE(4000,  6, 30, 767192);
    TEST_DATETIME_DATE(4000,  7,  1, 767193);
    TEST_DATETIME_DATE(4000,  7, 31, 767223);
    TEST_DATETIME_DATE(4000,  8,  1, 767224);
    TEST_DATETIME_DATE(4000,  8, 31, 767254);
    TEST_DATETIME_DATE(4000,  9,  1, 767255);
    TEST_DATETIME_DATE(4000,  9, 30, 767284);
    TEST_DATETIME_DATE(4000, 10,  1, 767285);
    TEST_DATETIME_DATE(4000, 10, 31, 767315);
    TEST_DATETIME_DATE(4000, 11,  1, 767316);
    TEST_DATETIME_DATE(4000, 11, 30, 767345);
    TEST_DATETIME_DATE(4000, 12,  1, 767346);
    TEST_DATETIME_DATE(4000, 12, 31, 767376);
    TEST_DATETIME_DATE(4321,  1,  1, 884254);
    TEST_DATETIME_DATE(4321,  1, 31, 884284);
    TEST_DATETIME_DATE(4321,  2,  1, 884285);
    TEST_DATETIME_DATE(4321,  2, 28, 884312);
    TEST_DATETIME_DATE(4321,  3,  1, 884313);
    TEST_DATETIME_DATE(4321,  3, 31, 884343);
    TEST_DATETIME_DATE(4321,  4,  1, 884344);
    TEST_DATETIME_DATE(4321,  4, 30, 884373);
    TEST_DATETIME_DATE(4321,  5,  1, 884374);
    TEST_DATETIME_DATE(4321,  5, 31, 884404);
    TEST_DATETIME_DATE(4321,  6,  1, 884405);
    TEST_DATETIME_DATE(4321,  6, 30, 884434);
    TEST_DATETIME_DATE(4321,  7,  1, 884435);
    TEST_DATETIME_DATE(4321,  7, 31, 884465);
    TEST_DATETIME_DATE(4321,  8,  1, 884466);
    TEST_DATETIME_DATE(4321,  8, 31, 884496);
    TEST_DATETIME_DATE(4321,  9,  1, 884497);
    TEST_DATETIME_DATE(4321,  9, 30, 884526);
    TEST_DATETIME_DATE(4321, 10,  1, 884527);
    TEST_DATETIME_DATE(4321, 10, 31, 884557);
    TEST_DATETIME_DATE(4321, 11,  1, 884558);
    TEST_DATETIME_DATE(4321, 11, 30, 884587);
    TEST_DATETIME_DATE(4321, 12,  1, 884588);
    TEST_DATETIME_DATE(4321, 12, 31, 884618);
    TEST_DATETIME_DATE(9999,  1,  1, 2958101);
    TEST_DATETIME_DATE(9999,  1, 31, 2958131);
    TEST_DATETIME_DATE(9999,  2,  1, 2958132);
    TEST_DATETIME_DATE(9999,  2, 28, 2958159);
    TEST_DATETIME_DATE(9999,  3,  1, 2958160);
    TEST_DATETIME_DATE(9999,  3, 31, 2958190);
    TEST_DATETIME_DATE(9999,  4,  1, 2958191);
    TEST_DATETIME_DATE(9999,  4, 30, 2958220);
    TEST_DATETIME_DATE(9999,  5,  1, 2958221);
    TEST_DATETIME_DATE(9999,  5, 31, 2958251);
    TEST_DATETIME_DATE(9999,  6,  1, 2958252);
    TEST_DATETIME_DATE(9999,  6, 30, 2958281);
    TEST_DATETIME_DATE(9999,  7,  1, 2958282);
    TEST_DATETIME_DATE(9999,  7, 31, 2958312);
    TEST_DATETIME_DATE(9999,  8,  1, 2958313);
    TEST_DATETIME_DATE(9999,  8, 31, 2958343);
    TEST_DATETIME_DATE(9999,  9,  1, 2958344);
    TEST_DATETIME_DATE(9999,  9, 30, 2958373);
    TEST_DATETIME_DATE(9999, 10,  1, 2958374);
    TEST_DATETIME_DATE(9999, 10, 31, 2958404);
    TEST_DATETIME_DATE(9999, 11,  1, 2958405);
    TEST_DATETIME_DATE(9999, 11, 30, 2958434);
    TEST_DATETIME_DATE(9999, 12,  1, 2958435);
    TEST_DATETIME_DATE(9999, 12, 31, 2958465);
}


CTEST(utility, test_datetime_date_only_1904) {

    double got;
    lxw_datetime *datetime;

    // Params = (year, month, day, excel_date).
    TEST_DATETIME_DATE_1904(1904,  1,  1, 0);
    TEST_DATETIME_DATE_1904(1904,  1, 31, 30);
    TEST_DATETIME_DATE_1904(1904,  2,  1, 31);
    TEST_DATETIME_DATE_1904(1904,  2, 29, 59);
    TEST_DATETIME_DATE_1904(1904,  3,  1, 60);
    TEST_DATETIME_DATE_1904(1904,  3, 31, 90);
    TEST_DATETIME_DATE_1904(1904,  4,  1, 91);
    TEST_DATETIME_DATE_1904(1904,  4, 30, 120);
    TEST_DATETIME_DATE_1904(1904,  5,  1, 121);
    TEST_DATETIME_DATE_1904(1904,  5, 31, 151);
    TEST_DATETIME_DATE_1904(1904,  6,  1, 152);
    TEST_DATETIME_DATE_1904(1904,  6, 30, 181);
    TEST_DATETIME_DATE_1904(1904,  7,  1, 182);
    TEST_DATETIME_DATE_1904(1904,  7, 31, 212);
    TEST_DATETIME_DATE_1904(1904,  8,  1, 213);
    TEST_DATETIME_DATE_1904(1904,  8, 31, 243);
    TEST_DATETIME_DATE_1904(1904,  9,  1, 244);
    TEST_DATETIME_DATE_1904(1904,  9, 30, 273);
    TEST_DATETIME_DATE_1904(1904, 10,  1, 274);
    TEST_DATETIME_DATE_1904(1904, 10, 31, 304);
    TEST_DATETIME_DATE_1904(1904, 11,  1, 305);
    TEST_DATETIME_DATE_1904(1904, 11, 30, 334);
    TEST_DATETIME_DATE_1904(1904, 12,  1, 335);
    TEST_DATETIME_DATE_1904(1904, 12, 31, 365);
    TEST_DATETIME_DATE_1904(1907,  2, 27, 1153);
    TEST_DATETIME_DATE_1904(1907,  2, 28, 1154);
    TEST_DATETIME_DATE_1904(1907,  3,  1, 1155);
    TEST_DATETIME_DATE_1904(1907,  3,  2, 1156);
    TEST_DATETIME_DATE_1904(1907,  3,  3, 1157);
    TEST_DATETIME_DATE_1904(1907,  3,  4, 1158);
    TEST_DATETIME_DATE_1904(1907,  3,  5, 1159);
    TEST_DATETIME_DATE_1904(1907,  3,  6, 1160);
    TEST_DATETIME_DATE_1904(1999,  1,  1, 34699);
    TEST_DATETIME_DATE_1904(1999,  1, 31, 34729);
    TEST_DATETIME_DATE_1904(1999,  2,  1, 34730);
    TEST_DATETIME_DATE_1904(1999,  2, 28, 34757);
    TEST_DATETIME_DATE_1904(1999,  3,  1, 34758);
    TEST_DATETIME_DATE_1904(1999,  3, 31, 34788);
    TEST_DATETIME_DATE_1904(1999,  4,  1, 34789);
    TEST_DATETIME_DATE_1904(1999,  4, 30, 34818);
    TEST_DATETIME_DATE_1904(1999,  5,  1, 34819);
    TEST_DATETIME_DATE_1904(1999,  5, 31, 34849);
    TEST_DATETIME_DATE_1904(1999,  6,  1, 34850);
    TEST_DATETIME_DATE_1904(1999,  6, 30, 34879);
    TEST_DATETIME_DATE_1904(1999,  7,  1, 34880);
    TEST_DATETIME_DATE_1904(1999,  7, 31, 34910);
    TEST_DATETIME_DATE_1904(1999,  8,  1, 34911);
    TEST_DATETIME_DATE_1904(1999,  8, 31, 34941);
    TEST_DATETIME_DATE_1904(1999,  9,  1, 34942);
    TEST_DATETIME_DATE_1904(1999,  9, 30, 34971);
    TEST_DATETIME_DATE_1904(1999, 10,  1, 34972);
    TEST_DATETIME_DATE_1904(1999, 10, 31, 35002);
    TEST_DATETIME_DATE_1904(1999, 11,  1, 35003);
    TEST_DATETIME_DATE_1904(1999, 11, 30, 35032);
    TEST_DATETIME_DATE_1904(1999, 12,  1, 35033);
    TEST_DATETIME_DATE_1904(1999, 12, 31, 35063);
    TEST_DATETIME_DATE_1904(2000,  1,  1, 35064);
    TEST_DATETIME_DATE_1904(2000,  1, 31, 35094);
    TEST_DATETIME_DATE_1904(2000,  2,  1, 35095);
    TEST_DATETIME_DATE_1904(2000,  2, 29, 35123);
    TEST_DATETIME_DATE_1904(2000,  3,  1, 35124);
    TEST_DATETIME_DATE_1904(2000,  3, 31, 35154);
    TEST_DATETIME_DATE_1904(2000,  4,  1, 35155);
    TEST_DATETIME_DATE_1904(2000,  4, 30, 35184);
    TEST_DATETIME_DATE_1904(2000,  5,  1, 35185);
    TEST_DATETIME_DATE_1904(2000,  5, 31, 35215);
    TEST_DATETIME_DATE_1904(2000,  6,  1, 35216);
    TEST_DATETIME_DATE_1904(2000,  6, 30, 35245);
    TEST_DATETIME_DATE_1904(2000,  7,  1, 35246);
    TEST_DATETIME_DATE_1904(2000,  7, 31, 35276);
    TEST_DATETIME_DATE_1904(2000,  8,  1, 35277);
    TEST_DATETIME_DATE_1904(2000,  8, 31, 35307);
    TEST_DATETIME_DATE_1904(2000,  9,  1, 35308);
    TEST_DATETIME_DATE_1904(2000,  9, 30, 35337);
    TEST_DATETIME_DATE_1904(2000, 10,  1, 35338);
    TEST_DATETIME_DATE_1904(2000, 10, 31, 35368);
    TEST_DATETIME_DATE_1904(2000, 11,  1, 35369);
    TEST_DATETIME_DATE_1904(2000, 11, 30, 35398);
    TEST_DATETIME_DATE_1904(2000, 12,  1, 35399);
    TEST_DATETIME_DATE_1904(2000, 12, 31, 35429);
    TEST_DATETIME_DATE_1904(2001,  1,  1, 35430);
    TEST_DATETIME_DATE_1904(2001,  1, 31, 35460);
    TEST_DATETIME_DATE_1904(2001,  2,  1, 35461);
    TEST_DATETIME_DATE_1904(2001,  2, 28, 35488);
    TEST_DATETIME_DATE_1904(2001,  3,  1, 35489);
    TEST_DATETIME_DATE_1904(2001,  3, 31, 35519);
    TEST_DATETIME_DATE_1904(2001,  4,  1, 35520);
    TEST_DATETIME_DATE_1904(2001,  4, 30, 35549);
    TEST_DATETIME_DATE_1904(2001,  5,  1, 35550);
    TEST_DATETIME_DATE_1904(2001,  5, 31, 35580);
    TEST_DATETIME_DATE_1904(2001,  6,  1, 35581);
    TEST_DATETIME_DATE_1904(2001,  6, 30, 35610);
    TEST_DATETIME_DATE_1904(2001,  7,  1, 35611);
    TEST_DATETIME_DATE_1904(2001,  7, 31, 35641);
    TEST_DATETIME_DATE_1904(2001,  8,  1, 35642);
    TEST_DATETIME_DATE_1904(2001,  8, 31, 35672);
    TEST_DATETIME_DATE_1904(2001,  9,  1, 35673);
    TEST_DATETIME_DATE_1904(2001,  9, 30, 35702);
    TEST_DATETIME_DATE_1904(2001, 10,  1, 35703);
    TEST_DATETIME_DATE_1904(2001, 10, 31, 35733);
    TEST_DATETIME_DATE_1904(2001, 11,  1, 35734);
    TEST_DATETIME_DATE_1904(2001, 11, 30, 35763);
    TEST_DATETIME_DATE_1904(2001, 12,  1, 35764);
    TEST_DATETIME_DATE_1904(2001, 12, 31, 35794);
    TEST_DATETIME_DATE_1904(2400,  1,  1, 181161);
    TEST_DATETIME_DATE_1904(2400,  1, 31, 181191);
    TEST_DATETIME_DATE_1904(2400,  2,  1, 181192);
    TEST_DATETIME_DATE_1904(2400,  2, 29, 181220);
    TEST_DATETIME_DATE_1904(2400,  3,  1, 181221);
    TEST_DATETIME_DATE_1904(2400,  3, 31, 181251);
    TEST_DATETIME_DATE_1904(2400,  4,  1, 181252);
    TEST_DATETIME_DATE_1904(2400,  4, 30, 181281);
    TEST_DATETIME_DATE_1904(2400,  5,  1, 181282);
    TEST_DATETIME_DATE_1904(2400,  5, 31, 181312);
    TEST_DATETIME_DATE_1904(2400,  6,  1, 181313);
    TEST_DATETIME_DATE_1904(2400,  6, 30, 181342);
    TEST_DATETIME_DATE_1904(2400,  7,  1, 181343);
    TEST_DATETIME_DATE_1904(2400,  7, 31, 181373);
    TEST_DATETIME_DATE_1904(2400,  8,  1, 181374);
    TEST_DATETIME_DATE_1904(2400,  8, 31, 181404);
    TEST_DATETIME_DATE_1904(2400,  9,  1, 181405);
    TEST_DATETIME_DATE_1904(2400,  9, 30, 181434);
    TEST_DATETIME_DATE_1904(2400, 10,  1, 181435);
    TEST_DATETIME_DATE_1904(2400, 10, 31, 181465);
    TEST_DATETIME_DATE_1904(2400, 11,  1, 181466);
    TEST_DATETIME_DATE_1904(2400, 11, 30, 181495);
    TEST_DATETIME_DATE_1904(2400, 12,  1, 181496);
    TEST_DATETIME_DATE_1904(2400, 12, 31, 181526);
    TEST_DATETIME_DATE_1904(4000,  1,  1, 765549);
    TEST_DATETIME_DATE_1904(4000,  1, 31, 765579);
    TEST_DATETIME_DATE_1904(4000,  2,  1, 765580);
    TEST_DATETIME_DATE_1904(4000,  2, 29, 765608);
    TEST_DATETIME_DATE_1904(4000,  3,  1, 765609);
    TEST_DATETIME_DATE_1904(4000,  3, 31, 765639);
    TEST_DATETIME_DATE_1904(4000,  4,  1, 765640);
    TEST_DATETIME_DATE_1904(4000,  4, 30, 765669);
    TEST_DATETIME_DATE_1904(4000,  5,  1, 765670);
    TEST_DATETIME_DATE_1904(4000,  5, 31, 765700);
    TEST_DATETIME_DATE_1904(4000,  6,  1, 765701);
    TEST_DATETIME_DATE_1904(4000,  6, 30, 765730);
    TEST_DATETIME_DATE_1904(4000,  7,  1, 765731);
    TEST_DATETIME_DATE_1904(4000,  7, 31, 765761);
    TEST_DATETIME_DATE_1904(4000,  8,  1, 765762);
    TEST_DATETIME_DATE_1904(4000,  8, 31, 765792);
    TEST_DATETIME_DATE_1904(4000,  9,  1, 765793);
    TEST_DATETIME_DATE_1904(4000,  9, 30, 765822);
    TEST_DATETIME_DATE_1904(4000, 10,  1, 765823);
    TEST_DATETIME_DATE_1904(4000, 10, 31, 765853);
    TEST_DATETIME_DATE_1904(4000, 11,  1, 765854);
    TEST_DATETIME_DATE_1904(4000, 11, 30, 765883);
    TEST_DATETIME_DATE_1904(4000, 12,  1, 765884);
    TEST_DATETIME_DATE_1904(4000, 12, 31, 765914);
    TEST_DATETIME_DATE_1904(4321,  1,  1, 882792);
    TEST_DATETIME_DATE_1904(4321,  1, 31, 882822);
    TEST_DATETIME_DATE_1904(4321,  2,  1, 882823);
    TEST_DATETIME_DATE_1904(4321,  2, 28, 882850);
    TEST_DATETIME_DATE_1904(4321,  3,  1, 882851);
    TEST_DATETIME_DATE_1904(4321,  3, 31, 882881);
    TEST_DATETIME_DATE_1904(4321,  4,  1, 882882);
    TEST_DATETIME_DATE_1904(4321,  4, 30, 882911);
    TEST_DATETIME_DATE_1904(4321,  5,  1, 882912);
    TEST_DATETIME_DATE_1904(4321,  5, 31, 882942);
    TEST_DATETIME_DATE_1904(4321,  6,  1, 882943);
    TEST_DATETIME_DATE_1904(4321,  6, 30, 882972);
    TEST_DATETIME_DATE_1904(4321,  7,  1, 882973);
    TEST_DATETIME_DATE_1904(4321,  7, 31, 883003);
    TEST_DATETIME_DATE_1904(4321,  8,  1, 883004);
    TEST_DATETIME_DATE_1904(4321,  8, 31, 883034);
    TEST_DATETIME_DATE_1904(4321,  9,  1, 883035);
    TEST_DATETIME_DATE_1904(4321,  9, 30, 883064);
    TEST_DATETIME_DATE_1904(4321, 10,  1, 883065);
    TEST_DATETIME_DATE_1904(4321, 10, 31, 883095);
    TEST_DATETIME_DATE_1904(4321, 11,  1, 883096);
    TEST_DATETIME_DATE_1904(4321, 11, 30, 883125);
    TEST_DATETIME_DATE_1904(4321, 12,  1, 883126);
    TEST_DATETIME_DATE_1904(4321, 12, 31, 883156);
    TEST_DATETIME_DATE_1904(9999,  1,  1, 2956639);
    TEST_DATETIME_DATE_1904(9999,  1, 31, 2956669);
    TEST_DATETIME_DATE_1904(9999,  2,  1, 2956670);
    TEST_DATETIME_DATE_1904(9999,  2, 28, 2956697);
    TEST_DATETIME_DATE_1904(9999,  3,  1, 2956698);
    TEST_DATETIME_DATE_1904(9999,  3, 31, 2956728);
    TEST_DATETIME_DATE_1904(9999,  4,  1, 2956729);
    TEST_DATETIME_DATE_1904(9999,  4, 30, 2956758);
    TEST_DATETIME_DATE_1904(9999,  5,  1, 2956759);
    TEST_DATETIME_DATE_1904(9999,  5, 31, 2956789);
    TEST_DATETIME_DATE_1904(9999,  6,  1, 2956790);
    TEST_DATETIME_DATE_1904(9999,  6, 30, 2956819);
    TEST_DATETIME_DATE_1904(9999,  7,  1, 2956820);
    TEST_DATETIME_DATE_1904(9999,  7, 31, 2956850);
    TEST_DATETIME_DATE_1904(9999,  8,  1, 2956851);
    TEST_DATETIME_DATE_1904(9999,  8, 31, 2956881);
    TEST_DATETIME_DATE_1904(9999,  9,  1, 2956882);
    TEST_DATETIME_DATE_1904(9999,  9, 30, 2956911);
    TEST_DATETIME_DATE_1904(9999, 10,  1, 2956912);
    TEST_DATETIME_DATE_1904(9999, 10, 31, 2956942);
    TEST_DATETIME_DATE_1904(9999, 11,  1, 2956943);
    TEST_DATETIME_DATE_1904(9999, 11, 30, 2956972);
    TEST_DATETIME_DATE_1904(9999, 12,  1, 2956973);
    TEST_DATETIME_DATE_1904(9999, 12, 31, 2957003);
}

CTEST(utility, test_datetime_time_only) {

    double got;
    lxw_datetime *datetime;

    TEST_DATETIME_TIME( 0,  0,  0.000, 0);
    TEST_DATETIME_TIME( 0, 15, 20.213, 1.0650613425925924E-2);
    TEST_DATETIME_TIME( 0, 16, 48.290, 1.1670023148148148E-2);
    TEST_DATETIME_TIME( 0, 55, 25.446, 3.8488958333333337E-2);
    TEST_DATETIME_TIME( 1,  2, 46.891, 4.3598275462962965E-2);
    TEST_DATETIME_TIME( 1,  4, 15.597, 4.4624965277777782E-2);
    TEST_DATETIME_TIME( 1,  9, 40.889, 4.8389918981481483E-2);
    TEST_DATETIME_TIME( 1, 11, 32.560, 4.9682407407407404E-2);
    TEST_DATETIME_TIME( 1, 30, 19.169, 6.2721863425925936E-2);
    TEST_DATETIME_TIME( 1, 48, 25.580, 7.5296064814814809E-2);
    TEST_DATETIME_TIME( 2,  3, 31.919, 8.5786099537037031E-2);
    TEST_DATETIME_TIME( 2, 11, 11.986, 9.1110949074074077E-2);
    TEST_DATETIME_TIME( 2, 24, 37.095, 0.10042934027777778);
    TEST_DATETIME_TIME( 2, 35,  7.220, 0.1077224537037037);
    TEST_DATETIME_TIME( 2, 45, 12.109, 0.11472348379629631);
    TEST_DATETIME_TIME( 3,  6, 39.990, 0.12962951388888888);
    TEST_DATETIME_TIME( 3,  8,  8.251, 0.13065105324074075);
    TEST_DATETIME_TIME( 3, 19, 12.576, 0.13833999999999999);
    TEST_DATETIME_TIME( 3, 29, 42.574, 0.14563164351851851);
    TEST_DATETIME_TIME( 3, 37, 30.813, 0.1510510763888889);
    TEST_DATETIME_TIME( 4, 14, 38.231, 0.1768313773148148);
    TEST_DATETIME_TIME( 4, 16, 28.559, 0.17810832175925925);
    TEST_DATETIME_TIME( 4, 17, 58.222, 0.17914608796296297);
    TEST_DATETIME_TIME( 4, 21, 41.794, 0.18173372685185185);
    TEST_DATETIME_TIME( 4, 56, 35.792, 0.2059698148148148);
    TEST_DATETIME_TIME( 5, 25, 14.885, 0.22586672453703704);
    TEST_DATETIME_TIME( 5, 26,  5.724, 0.22645513888888891);
    TEST_DATETIME_TIME( 5, 46, 44.068, 0.24078782407407406);
    TEST_DATETIME_TIME( 5, 48,  1.141, 0.2416798726851852);
    TEST_DATETIME_TIME( 5, 53, 52.315, 0.24574438657407408);
    TEST_DATETIME_TIME( 6, 14, 48.580, 0.26028449074074073);
    TEST_DATETIME_TIME( 6, 46, 15.738, 0.28212659722222222);
    TEST_DATETIME_TIME( 7, 31, 20.407, 0.31343063657407405);
    TEST_DATETIME_TIME( 7, 58, 33.754, 0.33233511574074076);
    TEST_DATETIME_TIME( 8,  7, 43.130, 0.33869363425925925);
    TEST_DATETIME_TIME( 8, 29, 11.091, 0.35360059027777774);
    TEST_DATETIME_TIME( 9,  8, 15.328, 0.380732962962963);
    TEST_DATETIME_TIME( 9, 30, 41.781, 0.39631690972222228);
    TEST_DATETIME_TIME( 9, 34,  4.462, 0.39866275462962958);
    TEST_DATETIME_TIME( 9, 37, 23.945, 0.40097158564814817);
    TEST_DATETIME_TIME( 9, 37, 56.655, 0.40135017361111114);
    TEST_DATETIME_TIME( 9, 45, 12.230, 0.40639155092592594);
    TEST_DATETIME_TIME( 9, 54, 14.782, 0.41267108796296298);
    TEST_DATETIME_TIME( 9, 54, 22.108, 0.41275587962962962);
    TEST_DATETIME_TIME(10,  1, 36.151, 0.41777952546296299);
    TEST_DATETIME_TIME(12,  9, 48.602, 0.50681252314814818);
    TEST_DATETIME_TIME(12, 34,  8.549, 0.52371005787037039);
    TEST_DATETIME_TIME(12, 56,  6.495, 0.53896406249999995);
    TEST_DATETIME_TIME(12, 58, 58.217, 0.54095158564814816);
    TEST_DATETIME_TIME(12, 59, 54.263, 0.54160026620370372);
    TEST_DATETIME_TIME(13, 34, 41.331, 0.56575614583333333);
    TEST_DATETIME_TIME(13, 58, 28.601, 0.58227547453703699);
    TEST_DATETIME_TIME(14,  2, 16.899, 0.58491781249999997);
    TEST_DATETIME_TIME(14, 36, 17.444, 0.60853523148148148);
    TEST_DATETIME_TIME(14, 37, 57.451, 0.60969271990740748);
    TEST_DATETIME_TIME(14, 57, 42.757, 0.6234115393518519);
    TEST_DATETIME_TIME(15, 10, 48.307, 0.6325035532407407);
    TEST_DATETIME_TIME(15, 14, 39.890, 0.63518391203703706);
    TEST_DATETIME_TIME(15, 19, 47.988, 0.63874986111111109);
    TEST_DATETIME_TIME(16,  4, 24.344, 0.66972620370370362);
    TEST_DATETIME_TIME(16, 22, 23.952, 0.68222166666666662);
    TEST_DATETIME_TIME(16, 29, 55.999, 0.6874536921296297);
    TEST_DATETIME_TIME(16, 58, 20.259, 0.70717892361111112);
    TEST_DATETIME_TIME(17,  4,  2.415, 0.71113906250000003);
    TEST_DATETIME_TIME(17, 18, 29.630, 0.72117627314814825);
    TEST_DATETIME_TIME(17, 47, 21.323, 0.74121901620370367);
    TEST_DATETIME_TIME(17, 53, 29.866, 0.74548456018518516);
    TEST_DATETIME_TIME(17, 53, 41.076, 0.74561430555555563);
    TEST_DATETIME_TIME(17, 55,  6.044, 0.74659773148148145);
    TEST_DATETIME_TIME(18, 14, 49.151, 0.760291099537037);
    TEST_DATETIME_TIME(18, 17, 45.738, 0.76233493055555546);
    TEST_DATETIME_TIME(18, 29, 59.700, 0.77082986111111118);
    TEST_DATETIME_TIME(18, 33, 21.233, 0.77316241898148153);
    TEST_DATETIME_TIME(19, 14, 24.673, 0.80167445601851861);
    TEST_DATETIME_TIME(19, 17, 12.816, 0.80362055555555545);
    TEST_DATETIME_TIME(19, 23, 36.418, 0.80806039351851855);
    TEST_DATETIME_TIME(19, 46, 25.908, 0.82391097222222232);
    TEST_DATETIME_TIME(20,  7, 47.314, 0.83874206018518516);
    TEST_DATETIME_TIME(20, 31, 37.603, 0.85529633101851854);
    TEST_DATETIME_TIME(20, 39, 57.770, 0.86108530092592594);
    TEST_DATETIME_TIME(20, 50, 17.067, 0.86825309027777775);
    TEST_DATETIME_TIME(21,  2, 57.827, 0.87705818287037041);
    TEST_DATETIME_TIME(21, 23,  5.519, 0.891036099537037);
    TEST_DATETIME_TIME(21, 34, 49.572, 0.89918486111111118);
    TEST_DATETIME_TIME(21, 39,  5.944, 0.90215212962962965);
    TEST_DATETIME_TIME(21, 39, 18.426, 0.90229659722222222);
    TEST_DATETIME_TIME(21, 46,  7.769, 0.90703436342592603);
    TEST_DATETIME_TIME(21, 57, 55.662, 0.91522756944444439);
    TEST_DATETIME_TIME(22, 19, 11.732, 0.92999689814814823);
    TEST_DATETIME_TIME(22, 23, 51.376, 0.93323351851851843);
    TEST_DATETIME_TIME(22, 27, 58.771, 0.93609688657407408);
    TEST_DATETIME_TIME(22, 43, 30.392, 0.94687953703703709);
    TEST_DATETIME_TIME(22, 48, 25.834, 0.95029900462962968);
    TEST_DATETIME_TIME(22, 53, 51.727, 0.95407091435185187);
    TEST_DATETIME_TIME(23, 12, 56.536, 0.96732101851851848);
    TEST_DATETIME_TIME(23, 15, 54.109, 0.96937626157407408);
    TEST_DATETIME_TIME(23, 17, 12.632, 0.97028509259259266);
    TEST_DATETIME_TIME(23, 59, 59.999, 0.99999998842592586);
}

CTEST(utility, test_unixtime) {

    double got;

    TEST_UNIXTIME(-2209075200LL,  0);         // 1899-12-31 00:00
    TEST_UNIXTIME(-2209032000LL,  0.5);       // 1899-12-31 12:00
    TEST_UNIXTIME(-2208988800LL,  1);         // 1900-01-01 00:00
    TEST_UNIXTIME(-2208945600LL,  1.5);       // 1900-01-01 12:00
    TEST_UNIXTIME(-2203977600LL,  59);        // 1900-02-28 00:00
    TEST_UNIXTIME(-2203934400LL,  59.5);      // 1900-02-28 12:00
    TEST_UNIXTIME(-2203891200LL,  61);        // 1900-03-01 00:00
    TEST_UNIXTIME(-2203848000LL,  61.5);      // 1900-03-01 12:00
    TEST_UNIXTIME(0,              25569);     // 1970-01-01 00:00
    TEST_UNIXTIME(43200,          25569.5);   // 1970-01-01 12:00
    TEST_UNIXTIME(946684800,      36526);     // 2000-01-01 00:00
    TEST_UNIXTIME(946728000,      36526.5);   // 2000-01-01 12:00
    TEST_UNIXTIME(253370764800,   2958101);   // 9999-01-01 00:00
    TEST_UNIXTIME(253370808000,   2958101.5); // 9999-01-01 12:00

}

CTEST(utility, test_unixtime_1904) {

    double got;

    TEST_UNIXTIME_1904(-2082844800, 0);       // 1904-01-01 00:00
    TEST_UNIXTIME_1904(-2082801600, 0.5);     // 1904-01-01 12:00
    TEST_UNIXTIME_1904(0,           24107);   // 1970-01-01 00:00
    TEST_UNIXTIME_1904(43200,       24107.5); // 1970-01-01 12:00
    TEST_UNIXTIME_1904(946684800,   35064);   // 2000-01-01 00:00
    TEST_UNIXTIME_1904(946728000,   35064.5); // 2000-01-01 12:00
}


