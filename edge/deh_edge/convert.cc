//------------------------------------------------------------------------
//  Patch Conversion tables
//------------------------------------------------------------------------
//
//  DEH_EDGE  Copyright (C) 2004  The EDGE Team
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License (in COPYING.txt) for more details.
//
//------------------------------------------------------------------------
//
//  DEH_EDGE is based on:
//
//  +  DeHackEd source code, by Greg Lewis.
//  -  DOOM source code (C) 1993-1996 id Software, Inc.
//  -  Linux DOOM Hack Editor, by Sam Lantinga.
//  -  PrBoom's DEH/BEX code, by Ty Halderman, TeamTNT.
//
//------------------------------------------------------------------------

#include "i_defs.h"
#include "convert.h"


// Thing conversion array from 1.2 to 1.666
short thing12to166[THINGS_1_2] =
{
	 0,  11,   1,   2,  12,  13,  14,  18,  15,  19,
    21,  30,  31,  32,  16,  33,  34,  35,  37,  38,
    39,  41,  42,  43,  44,  45,  46,  47,  48,  49,
    50,  51,  52,  53,  54,  55,  56,  57,  58,  59,
    60,  61,  63,  64,  65,  66,  67,  68,  69,  70,
    71,  72,  73,  74,  75,  76,  77,  81,  82,  83,
    84,  85,  86,  87,  88,  89,  90,  91,  92,  93,
    94,  95,  96,  97,  98,  99, 100, 101, 102, 103,
    104, 105, 106, 107, 108, 109, 110, 111, 112, 113,
    114, 115, 116, 117, 118, 119, 120, 121, 122, 123,
    124, 125, 126, 137
};

// Frame conversion array from 1.2 to 1.666
short frame12to166[FRAMES_1_2] =
{
	 0,   1,   2,   3,   4,   5,   6,   7,   8,   9,
    10,  11,  12,  13,  14,  15,  16,  17,  18,  19,
    20,  21,  22,  23,  24,  25,  26,  27,  28,  29,
    30,  31,  49,  50,  51,  52,  53,  54,  55,  56,
    57,  58,  59,  60,  61,  62,  63,  64,  65,  66,
    67,  68,  69,  70,  71,  72,  73,  74,  75,  76,
    77,  78,  79,  80,  81,  82,  83,  84,  85,  86,
    87,  88,  89,  90,  91,  92,  93,  94,  95,  96,
    97,  98,  99, 100, 101, 102, 103, 104, 105, 106,
    522, 523, 524, 525, 526, 107, 108, 109, 110, 111,
/* 100 */
    112, 113, 114, 115, 116, 117, 118, 119, 120, 121,
    122, 123, 124, 125, 126, 127, 128, 129, 149, 150,
    151, 152, 153, 154, 155, 156, 157, 158, 159, 160,
    161, 162, 163, 164, 165, 166, 167, 168, 169, 170,
    171, 172, 173, 174, 175, 176, 177, 178, 179, 180,
    181, 182, 183, 184, 185, 186, 187, 188, 189, 190,
    191, 192, 193, 194, 195, 196, 197, 198, 199, 200,
    201, 202, 207, 208, 209, 210, 211, 212, 213, 214,
    215, 216, 217, 218, 219, 220, 221, 222, 223, 224,
    225, 226, 227, 228, 229, 230, 231, 232, 233, 234,
/* 200 */
    235, 442, 443, 444, 445, 446, 447, 448, 449, 450,
    451, 452, 453, 454, 455, 456, 457, 458, 459, 460,
    461, 462, 463, 464, 465, 466, 467, 468, 469, 475,
    476, 477, 478, 479, 480, 481, 482, 483, 484, 485,
    486, 487, 488, 489, 490, 491, 492, 493, 494, 495,
    502, 503, 504, 505, 506, 507, 508, 509, 510, 511,
    512, 513, 514, 515, 527, 528, 529, 530, 531, 532,
    533, 534, 535, 536, 537, 538, 539, 540, 541, 542,
    543, 544, 545, 546, 547, 548, 585, 586, 587, 588,
    589, 590, 591, 592, 593, 594, 595, 596, 597, 598,
/* 300 */
    599, 600, 601, 602, 603, 604, 605, 606, 607, 608,
    609, 610, 611, 612, 613, 614, 615, 616, 617, 618,
    619, 620, 621, 622, 623, 624, 625, 626, 627, 628,
    629, 630, 631, 674, 675, 676, 677, 678, 679, 680,
    681, 682, 683, 684, 685, 686, 687, 688, 689, 690,
    691, 692, 693, 694, 695, 696, 697, 698, 699, 700,
    130, 131, 132, 133, 134, 135, 136, 137, 138, 139,
    140, 141, 802, 803, 804, 805, 806, 807, 808, 809,
    810, 811, 812, 816, 817, 818, 819, 820, 821, 822,
    823, 824, 825, 826, 827, 828, 829, 830, 831, 832,
/* 400 */
    833, 834, 835, 836, 837, 838, 839, 840, 841, 842,
    843, 844, 845, 846, 847, 848, 849, 850, 851, 852,
    853, 854, 855, 856, 861, 862, 863, 864, 865, 866,
    867, 868, 869, 870, 871, 872, 873, 874, 875, 876,
    877, 878, 879, 880, 881, 882, 883, 884, 886, 887,
    888, 889, 890, 891, 892, 893, 894, 895, 896, 897,
    898, 899, 900, 901, 902, 903, 904, 905, 906, 907,
    908, 909, 910, 911, 912, 913, 914, 915, 916, 917,
    918, 919, 920, 921, 922, 923, 924, 925, 926, 927,
    928, 929, 930, 931, 932, 933, 934, 935, 936, 937,
/* 500 */
    938, 939, 940, 941, 942, 943, 944, 945, 946, 947,
    948, 949};

// Sound conversion array from 1.2 to 1.666
short sound12to166[SOUNDS_1_2] =
{
	 0,  1,  2,  3,  8,  9, 10, 11, 12, 13,
    14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
    24, 25, 26, 27, 31, 32, 33, 34, 35, 36,
    37, 38, 39, 40, 41, 42, 43, 44, 45, 51,
    52, 55, 57, 59, 60, 61, 62, 63, 64, 65,
    66, 67, 68, 69, 75, 76, 77, 81, 82, 83,
    84, 85, 86
};

// Sprite conversion array from 1.2 to 1.666
short sprite12to166[SPRITES_1_2] =
{
	 0,   1,   2,   3,   4,   5,   7,   8,   9,  10,
    11,  12,  13,  14,  15,  16,  17,  18,  19,  41,
    20,  21,  22,  23,  24,  25,  28,  29,  30,  39,
    40,  42,  44,  45,  49,  26,  55,  56,  57,  58,
    60,  61,  62,  63,  64,  65,  66,  67,  68,  69,
    70,  71,  72,  73,  75,  76,  77,  78,  79,  80,
    81,  82,  83,  84,  85,  86,  87,  88,  89,  90,
    91,  92,  94,  95,  96,  97,  98,  99, 100, 101,
    102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
    112, 113, 114, 115, 116, 117, 118, 119, 120, 121,
    122, 123, 124, 125, 126
};

// code pointer conversion (Pointer number to Frame number)
short pointerToFrame[POINTER_NUM] =
{
  /* 0 */  1, 2, 3, 4, 6, 9, 10, 11, 12, 14, 
 /* 10 */  16, 17, 18, 19, 20, 22, 29, 30, 31, 32, 
 /* 20 */  33, 34, 36, 38, 39, 41, 43, 44, 47, 48, 
 /* 30 */  49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 
 /* 40 */  59, 60, 61, 62, 63, 65, 66, 67, 68, 69, 
 /* 50 */  70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 
 /* 60 */  80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 
 /* 70 */  119, 127, 157, 159, 160, 166, 167, 174, 175, 176, 
 /* 80 */  177, 178, 179, 180, 181, 182, 183, 184, 185, 188, 
 /* 90 */  190, 191, 195, 196, 207, 208, 209, 210, 211, 212, 
/* 100 */  213, 214, 215, 216, 217, 218, 221, 223, 224, 228, 
/* 110 */  229, 241, 242, 243, 244, 245, 246, 247, 248, 249, 
/* 120 */  250, 251, 252, 253, 254, 255, 256, 257, 258, 259, 
/* 130 */  260, 261, 262, 263, 264, 270, 272, 273, 281, 282, 
/* 140 */  283, 284, 285, 286, 287, 288, 289, 290, 291, 292, 
/* 150 */  293, 294, 295, 296, 297, 298, 299, 300, 301, 302, 
/* 160 */  303, 304, 305, 306, 307, 308, 309, 310, 316, 317, 
/* 170 */  321, 322, 323, 324, 325, 326, 327, 328, 329, 330, 
/* 180 */  331, 332, 333, 334, 335, 336, 337, 338, 339, 340, 
/* 190 */  341, 342, 344, 347, 348, 362, 363, 364, 365, 366, 
/* 200 */  367, 368, 369, 370, 371, 372, 373, 374, 375, 376, 
/* 210 */  377, 378, 379, 380, 381, 382, 383, 384, 385, 387, 
/* 220 */  389, 390, 397, 406, 407, 408, 409, 410, 411, 412, 
/* 230 */  413, 414, 415, 416, 417, 418, 419, 421, 423, 424, 
/* 240 */  430, 431, 442, 443, 444, 445, 446, 447, 448, 449, 
/* 250 */  450, 451, 452, 453, 454, 456, 458, 460, 463, 465, 
/* 260 */  475, 476, 477, 478, 479, 480, 481, 482, 483, 484, 
/* 270 */  485, 486, 487, 489, 491, 493, 502, 503, 504, 505, 
/* 280 */  506, 508, 511, 514, 527, 528, 529, 530, 531, 532, 
/* 290 */  533, 534, 535, 536, 537, 538, 539, 541, 543, 545, 
/* 300 */  548, 556, 557, 558, 559, 560, 561, 562, 563, 564, 
/* 310 */  565, 566, 567, 568, 570, 572, 574, 585, 586, 587, 
/* 320 */  588, 589, 590, 594, 596, 598, 601, 602, 603, 604, 
/* 330 */  605, 606, 607, 608, 609, 610, 611, 612, 613, 614, 
/* 340 */  615, 616, 617, 618, 620, 621, 622, 631, 632, 633, 
/* 350 */  635, 636, 637, 638, 639, 640, 641, 642, 643, 644, 
/* 360 */  645, 646, 647, 648, 650, 652, 653, 654, 659, 674, 
/* 370 */  675, 676, 677, 678, 679, 680, 681, 682, 683, 684, 
/* 380 */  685, 686, 687, 688, 689, 690, 692, 696, 700, 701, 
/* 390 */  702, 703, 704, 705, 706, 707, 708, 709, 710, 711, 
/* 400 */  713, 715, 718, 726, 727, 728, 729, 730, 731, 732, 
/* 410 */  733, 734, 735, 736, 737, 738, 739, 740, 741, 743, 
/* 420 */  745, 746, 750, 751, 766, 774, 777, 779, 780, 783, 
/* 430 */  784, 785, 786, 787, 788, 789, 790, 791, 792, 793, 
/* 440 */  794, 795, 796, 797, 798, 801, 809, 811
};

