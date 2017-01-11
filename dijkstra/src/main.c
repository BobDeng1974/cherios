#include <stdio.h>
#include<stdlib.h>

#define NUM_NODES                          100
#define NONE                               9999

struct _NODE
{
  int iDist;
  int iPrev;
};
typedef struct _NODE NODE;

struct _QITEM
{
  int iNode;
  int iDist;
  int iPrev;
  struct _QITEM *qNext;
};
typedef struct _QITEM QITEM;

QITEM *qHead = NULL;

int AdjMatrix[NUM_NODES][NUM_NODES] = {
{32,32,54,12,52,56,8,30,44,94,44,39,65,19,51,91,1,5,89,34,25,58,20,51,38,65,30,7,20,10,51,18,43,71,97,61,26,5,57,70,65,0,75,29,86,93,87,87,64,75,88,89,100,7,40,37,38,36,44,24,46,95,43,89,32,5,15,58,77,72,95,8,38,69,37,24,27,90,77,92,31,30,80,30,37,86,33,76,21,77,100,68,37,8,22,69,81,38,94,57},
{76,54,65,14,89,69,4,16,24,47,7,21,78,53,17,81,39,50,22,60,93,89,94,30,97,16,65,43,20,24,67,62,78,98,42,67,32,46,49,57,60,56,44,37,75,62,17,13,11,40,40,4,95,100,0,57,82,31,0,1,56,67,30,100,64,72,66,63,18,81,19,44,2,63,81,78,91,64,91,2,70,97,73,64,97,39,21,78,70,21,46,25,54,76,92,84,47,57,46,31},
{38,31,75,40,61,21,84,51,86,41,19,21,37,58,86,100,97,73,44,67,60,90,58,13,31,49,63,44,73,76,76,77,73,16,83,100,4,67,51,56,7,36,77,10,95,28,10,57,0,54,23,60,9,48,39,40,97,69,84,35,44,25,11,83,8,61,83,12,27,100,34,0,35,10,10,96,39,87,53,5,40,42,66,15,90,71,55,87,39,5,88,49,97,100,32,4,60,81,83,53},
{80,16,53,14,94,29,77,99,16,29,3,22,71,35,4,61,6,25,13,11,30,0,27,94,66,25,64,92,5,47,44,85,29,63,65,89,59,41,87,41,36,57,29,7,92,33,34,64,59,47,76,55,13,2,48,46,27,12,37,99,25,48,83,20,77,13,9,35,55,62,76,57,18,72,64,10,4,64,74,63,77,15,18,91,84,32,36,77,10,39,75,35,87,23,22,30,37,31,65,58},
{59,7,14,78,79,45,54,83,8,94,12,86,9,97,42,93,95,44,70,5,83,10,40,36,34,62,66,71,59,97,95,18,3,8,62,48,19,15,98,28,8,9,80,84,72,21,43,66,65,79,71,13,89,78,49,22,5,14,59,65,11,53,49,81,28,77,29,47,92,26,41,66,1,20,50,73,7,59,4,72,37,76,86,25,19,0,14,24,15,73,55,93,93,3,73,87,80,68,100,37},
{94,41,3,61,27,19,33,35,78,38,73,14,80,58,5,99,59,19,22,40,59,78,32,17,47,71,3,94,39,2,97,99,9,66,60,37,85,59,38,28,63,10,8,8,35,81,6,60,100,96,66,24,39,64,41,52,34,10,11,39,80,8,4,89,74,64,92,25,89,29,19,18,6,28,26,7,8,33,67,74,95,32,99,33,96,5,51,96,83,63,35,62,71,39,16,10,69,8,35,23},
{3,55,41,76,49,68,83,23,67,15,97,61,13,61,60,75,33,77,71,15,39,72,43,76,77,59,53,11,33,88,34,37,8,76,79,23,9,62,46,76,43,9,2,57,70,28,31,69,4,68,84,10,39,26,52,82,52,4,93,85,59,94,21,33,35,67,57,44,28,69,86,37,78,54,94,14,48,25,83,18,59,33,28,99,25,81,46,77,51,39,62,9,32,49,43,33,15,100,77,9},
{68,28,47,12,82,6,26,96,98,75,13,57,7,8,55,33,55,0,76,5,5,3,15,3,53,58,36,34,23,79,10,57,6,23,69,54,29,61,49,27,36,63,84,9,71,4,8,25,71,85,97,77,88,11,46,6,35,83,7,24,27,17,82,34,40,16,88,69,44,3,62,46,32,45,55,2,49,64,94,87,14,90,63,68,68,75,75,2,23,82,27,51,65,75,85,71,57,38,39,0},
{7,1,46,39,12,68,41,28,31,0,14,45,91,43,12,58,17,53,26,41,0,19,92,31,60,42,1,17,46,41,84,54,8,97,93,20,64,0,14,61,0,28,72,57,71,50,81,89,70,7,96,70,26,87,1,87,95,69,70,40,9,19,94,84,15,87,71,45,87,85,5,53,13,43,10,50,94,91,38,63,98,33,99,91,86,66,43,80,35,79,20,10,98,80,61,13,66,31,24,18},
{82,97,72,61,39,48,11,99,38,49,27,2,49,26,59,0,58,1,81,59,80,67,70,77,46,97,56,79,27,81,63,75,77,0,36,82,48,47,81,53,62,7,55,77,100,13,78,24,81,24,83,26,91,18,2,2,14,25,47,7,72,10,83,14,10,18,96,25,65,42,78,93,16,32,70,15,11,47,5,58,71,89,84,27,73,86,96,88,77,43,95,48,19,43,62,96,61,24,20,92},
{66,98,85,82,96,20,64,73,67,69,30,3,23,13,97,97,66,58,50,42,0,44,57,86,54,85,82,14,8,1,73,41,66,23,22,61,43,86,0,9,21,30,79,44,44,75,40,76,99,56,17,100,67,40,51,20,25,32,0,100,0,73,40,66,96,29,93,38,81,93,13,1,90,92,46,100,32,52,75,31,8,58,97,75,99,13,61,90,46,61,89,12,34,96,78,96,24,36,34,4},
{96,13,73,85,72,18,50,70,36,24,67,10,82,29,51,80,43,11,35,89,39,24,0,73,86,44,34,9,46,34,80,41,48,52,92,19,36,41,55,39,31,22,49,13,51,67,59,94,44,95,48,83,85,48,21,70,58,56,45,4,90,91,11,3,43,70,89,45,77,44,84,8,66,100,88,83,66,46,77,76,6,24,59,91,39,46,26,97,68,37,0,58,28,79,27,37,48,16,82,24},
{60,66,32,92,65,19,74,97,32,16,72,38,41,97,96,46,43,88,42,77,25,9,34,19,88,28,56,1,44,3,25,70,69,24,27,100,9,0,96,7,84,34,12,91,30,7,36,39,95,78,16,86,53,16,71,6,44,26,7,54,30,100,23,65,23,50,65,99,17,26,73,67,60,85,57,57,92,93,96,52,36,78,4,90,61,75,96,4,68,3,25,64,69,14,28,58,31,59,56,48},
{86,28,81,45,12,37,1,70,29,64,89,31,41,93,20,1,67,83,73,0,52,98,64,20,78,93,78,8,17,100,22,2,95,2,48,6,39,15,43,34,79,31,66,87,23,52,54,56,34,93,57,52,56,87,72,34,79,15,42,63,15,65,65,9,67,79,82,73,95,91,6,39,21,38,92,10,91,46,67,91,38,90,43,95,76,81,28,21,63,70,84,78,0,48,53,68,94,0,40,88},
{92,12,93,12,17,85,23,7,30,56,64,34,45,73,28,87,20,22,7,83,59,91,26,59,5,79,26,99,79,32,52,70,11,44,83,28,95,72,1,91,27,65,25,38,4,19,24,24,8,99,73,67,89,99,25,60,77,18,24,21,16,42,58,27,53,6,55,47,78,56,38,71,88,29,8,58,48,99,48,56,97,20,89,52,18,14,78,61,99,2,48,14,44,5,42,97,11,63,10,55},
{19,48,25,73,77,100,30,91,99,78,13,95,98,1,12,82,82,91,8,80,93,22,61,2,28,2,66,5,65,76,61,50,90,86,22,32,52,52,22,50,96,1,10,59,70,90,40,51,80,14,98,38,37,58,40,31,60,72,2,91,47,63,7,2,15,29,34,67,48,23,83,9,24,59,69,94,48,8,11,27,90,8,31,93,32,38,90,58,9,92,48,23,55,55,25,36,51,60,69,65},
{83,51,74,73,76,42,67,24,17,44,17,73,18,49,65,50,87,54,7,62,11,21,85,32,77,10,68,94,70,36,24,52,53,98,24,96,6,57,86,90,67,2,62,85,17,26,34,70,46,41,32,23,63,16,56,5,26,23,65,62,26,89,80,45,52,71,6,58,27,92,47,61,61,75,45,78,67,46,14,12,53,46,36,82,28,58,87,21,47,17,83,73,72,63,85,24,33,91,48,26},
{49,62,53,9,36,99,53,3,10,67,82,63,79,84,45,7,41,98,95,89,82,43,27,53,5,78,77,4,69,25,98,17,53,16,93,89,81,45,58,91,12,40,54,91,90,65,64,31,62,58,86,43,1,12,63,73,91,39,44,25,30,7,8,83,23,0,38,4,45,96,61,23,1,14,81,92,45,44,89,74,69,74,83,36,52,45,75,8,85,18,100,81,92,7,30,82,74,34,52,86},
{96,12,8,98,94,89,55,38,100,43,11,68,83,95,3,0,39,78,9,90,63,8,37,20,83,67,1,56,67,53,7,62,66,16,25,25,71,80,63,70,89,75,3,37,35,6,38,74,51,47,30,80,21,67,100,3,100,68,26,66,87,33,27,52,15,53,43,53,99,6,22,88,47,26,24,82,99,28,21,15,75,51,95,63,84,61,66,83,28,58,14,14,58,42,33,39,61,76,92,25},
{48,14,79,95,6,70,76,4,98,98,87,39,14,81,1,99,7,33,81,1,92,96,16,15,3,15,54,30,57,12,55,5,93,0,100,99,70,42,69,67,39,21,5,53,2,6,51,76,40,99,78,98,60,60,79,63,75,99,59,98,10,80,2,2,80,69,67,49,10,2,16,49,23,88,68,92,95,86,68,0,84,11,64,43,71,42,72,45,40,97,42,17,76,11,86,56,80,19,4,90},
{88,87,4,77,75,72,69,35,23,2,35,6,80,99,15,50,6,53,61,46,49,69,29,25,80,15,47,25,34,51,14,21,38,85,98,79,57,32,13,46,0,48,53,80,12,34,29,18,54,56,30,2,25,60,94,4,41,40,30,75,58,10,62,62,96,59,40,18,58,53,64,24,67,83,4,79,17,100,63,37,56,93,39,81,18,100,51,59,5,81,100,63,58,61,24,53,87,64,37,10},
{83,67,34,49,50,38,27,33,4,56,70,60,15,75,6,33,40,57,59,46,4,24,75,62,86,100,81,38,29,17,48,79,84,48,27,100,87,21,32,57,77,68,16,92,9,22,92,49,79,16,95,83,40,70,10,25,35,91,29,30,74,43,8,24,92,2,23,44,23,22,0,66,56,16,58,65,4,15,14,49,31,75,32,71,10,8,63,45,100,92,42,73,1,50,97,93,18,87,36,41},
{75,36,7,30,18,31,96,22,12,76,71,43,50,69,80,61,78,42,72,43,0,13,15,68,30,79,60,48,31,62,56,5,98,29,1,82,26,97,3,38,72,40,81,89,76,26,15,53,35,87,96,1,67,77,69,97,21,28,10,18,90,32,23,53,61,25,34,87,88,3,91,26,9,37,81,85,64,96,3,99,82,65,100,48,42,68,10,29,62,88,48,17,19,37,70,47,28,70,100,16},
{73,91,8,82,94,89,33,57,84,36,21,31,1,87,46,9,20,56,4,82,9,52,99,96,56,34,8,84,3,7,66,42,64,74,24,58,28,23,81,11,59,2,9,26,55,55,1,76,77,6,23,87,24,89,82,80,22,90,30,93,63,96,34,27,36,24,51,30,47,98,8,73,100,17,99,21,72,0,97,48,73,86,34,97,74,82,43,63,37,73,55,0,34,55,94,36,80,10,67,93},
{7,75,65,74,92,64,95,63,30,57,77,2,42,11,65,16,59,7,45,97,46,66,63,81,20,56,83,66,32,49,59,39,90,23,12,81,53,73,9,49,29,87,17,72,64,83,54,89,90,65,85,36,30,13,83,16,35,65,83,67,14,7,73,70,97,85,51,16,24,26,65,53,79,83,91,8,65,10,98,20,41,48,22,71,62,4,54,63,36,36,30,16,9,2,86,5,53,36,88,77},
{29,53,97,74,1,53,83,32,30,46,52,71,94,41,42,21,45,62,85,81,98,81,97,73,83,83,44,1,85,32,45,80,85,41,54,52,60,2,84,90,48,1,61,7,42,69,96,54,30,46,0,94,26,64,32,75,46,76,42,97,7,87,43,58,94,97,9,54,99,59,43,12,61,70,19,69,4,14,22,0,26,23,60,52,53,92,93,65,68,35,61,75,88,70,33,82,66,8,35,30},
{68,44,8,95,81,28,63,85,8,52,86,35,41,11,53,94,3,12,58,71,13,85,11,0,55,44,82,87,19,83,84,87,27,92,81,7,86,9,58,61,27,9,62,68,21,81,61,24,93,85,61,72,70,72,73,91,16,20,77,35,3,26,88,97,18,34,3,70,9,27,30,37,37,92,4,24,73,32,48,31,83,8,3,52,80,42,8,62,62,52,63,65,78,16,27,62,50,30,32,26},
{24,62,63,27,20,67,51,59,65,65,90,48,73,93,66,18,0,75,47,63,26,76,94,3,59,21,66,75,17,64,0,41,25,63,68,11,97,85,70,61,49,60,8,88,18,41,6,19,15,19,48,41,61,41,10,19,62,42,95,46,5,95,53,98,58,21,8,20,5,79,81,21,4,56,8,89,97,81,74,11,100,21,18,61,29,95,46,57,37,40,2,42,1,56,5,59,43,14,79,14},
{59,25,35,29,81,44,84,43,24,58,20,91,45,38,17,74,100,63,31,36,3,33,44,71,55,50,96,98,30,40,12,55,65,13,50,12,57,33,55,48,91,42,38,36,46,55,76,45,17,6,81,87,6,25,57,61,41,52,25,37,92,3,92,23,16,7,35,74,40,56,21,98,98,59,100,44,80,75,89,97,82,36,50,54,27,6,14,68,25,5,4,83,8,62,5,25,69,40,65,75},
{63,52,72,60,10,71,70,56,12,59,52,94,95,68,13,21,41,94,55,66,100,25,48,7,53,54,99,88,60,63,62,22,14,34,49,91,71,18,46,83,77,65,42,37,32,55,24,39,15,45,4,14,36,19,21,89,39,87,76,99,49,4,88,64,4,36,54,75,20,67,24,64,31,32,0,29,54,92,69,69,36,39,83,39,58,70,27,63,56,70,28,5,74,15,35,78,17,55,18,37},
{88,8,0,85,41,68,14,95,59,49,63,61,54,11,66,79,81,94,41,3,29,69,75,69,50,9,46,33,30,30,71,18,39,37,2,80,4,83,40,29,98,2,57,52,13,22,30,60,82,71,29,10,6,3,79,22,79,91,56,76,21,26,94,26,63,62,72,34,45,11,29,42,13,86,94,93,75,90,18,56,27,48,33,33,17,78,55,63,69,10,38,56,2,31,48,32,93,19,32,3},
{30,61,46,43,13,5,1,88,96,86,9,89,100,42,21,17,20,42,80,55,19,17,10,88,14,58,19,6,77,17,77,73,79,22,15,58,94,83,45,55,68,20,43,68,63,30,51,49,39,97,3,58,13,80,45,27,3,31,100,80,48,76,52,93,64,33,50,24,82,61,45,15,82,89,49,10,85,100,59,23,96,28,81,75,7,93,68,10,90,34,56,3,76,74,97,6,73,12,30,20},
{40,75,35,88,29,85,64,14,50,22,37,12,16,85,87,23,77,21,100,66,55,21,35,30,95,31,2,33,10,32,53,16,74,54,70,69,38,33,83,55,55,87,67,71,71,19,60,13,40,25,45,61,46,80,58,6,78,60,39,88,93,58,70,32,11,39,0,16,72,50,71,93,36,37,29,6,56,55,19,63,80,64,23,25,43,81,98,87,41,2,40,100,60,9,31,37,14,98,53,86},
{47,90,44,83,26,73,55,49,27,40,11,73,70,0,64,13,82,61,66,89,29,6,88,89,15,85,93,30,82,11,82,96,1,26,78,27,65,100,42,93,39,53,31,9,54,96,89,1,22,54,90,52,60,43,6,42,27,99,72,75,10,19,70,11,45,14,4,10,13,47,69,52,66,100,27,86,61,15,53,84,36,42,35,96,85,41,37,78,40,75,53,16,95,22,94,5,36,98,15,15},
{10,50,34,77,16,61,28,77,43,82,60,79,90,95,74,41,2,78,18,8,18,71,24,12,60,17,85,62,81,66,78,92,16,11,34,32,38,28,75,81,9,1,59,66,62,100,6,64,43,24,72,61,62,62,40,21,79,24,49,26,90,26,84,72,3,84,70,8,11,45,89,88,46,14,53,74,80,59,38,89,83,9,15,10,38,55,31,83,45,81,8,1,73,92,73,43,75,9,51,53},
{54,5,40,66,86,59,39,31,17,43,19,66,19,1,77,57,22,74,39,68,20,14,35,60,5,7,2,47,16,19,66,36,91,5,68,43,30,74,40,47,83,26,79,1,27,21,24,49,96,64,83,82,78,17,41,49,92,9,62,74,28,27,77,86,99,44,95,28,84,34,41,33,60,20,34,87,41,59,36,2,89,85,85,32,2,25,47,94,35,9,67,29,2,43,81,1,54,75,96,3},
{9,37,36,35,23,37,22,30,62,24,33,50,8,84,48,77,8,95,70,9,70,37,5,73,46,86,74,100,27,35,70,2,72,5,37,95,42,25,25,3,49,24,19,24,7,67,0,82,28,71,92,98,74,63,70,86,14,9,52,41,45,21,43,83,93,47,44,35,72,35,4,88,59,91,11,32,57,11,13,51,48,71,49,88,33,85,40,48,61,92,55,5,79,65,54,71,11,98,72,83},
{32,43,70,57,33,47,89,56,25,69,7,73,39,56,27,39,6,67,53,67,24,74,38,2,38,93,73,49,56,11,99,89,54,34,11,87,48,67,42,73,35,49,11,40,71,4,45,78,71,98,10,95,38,49,63,76,41,36,92,97,47,56,51,0,56,63,53,3,29,95,76,30,44,54,70,81,58,82,58,96,45,69,56,83,84,19,59,24,21,16,87,34,72,4,0,27,33,53,31,28},
{47,73,58,57,26,94,38,85,75,62,80,87,97,35,69,80,20,27,3,41,43,57,75,81,27,75,8,60,27,5,88,41,78,11,98,71,71,1,55,12,64,0,99,60,1,67,40,22,61,9,63,70,32,4,51,59,79,25,18,73,30,72,13,7,49,77,78,87,79,99,99,42,65,63,68,67,96,7,55,56,84,84,93,15,88,43,75,33,34,59,72,64,98,85,37,12,27,82,99,5},
{80,63,13,11,92,48,44,88,55,99,9,4,48,1,20,2,10,61,1,44,86,73,74,83,23,11,62,50,93,26,22,38,90,1,15,47,49,59,34,71,23,44,75,38,11,61,40,22,21,41,32,7,13,6,56,36,84,17,52,76,44,74,80,100,42,96,46,91,20,81,27,10,91,2,48,1,29,88,90,51,95,22,58,7,95,13,9,78,31,61,19,41,1,65,40,43,26,86,100,47},
{32,94,23,22,62,71,91,91,58,80,41,18,68,65,25,62,79,0,5,76,27,24,83,28,56,22,37,82,74,3,95,6,97,17,95,24,54,85,14,78,31,56,96,99,20,87,27,65,87,32,6,14,23,89,8,45,77,12,26,51,82,88,23,44,71,17,68,25,69,82,2,100,3,99,64,91,85,91,21,38,90,28,52,79,83,26,23,60,38,49,10,86,2,33,29,74,16,97,65,51},
{45,67,16,48,31,81,4,16,37,26,20,93,20,38,71,2,64,94,62,69,9,72,54,11,71,84,51,54,80,15,4,24,83,88,39,80,68,43,62,71,35,82,64,55,19,0,58,84,95,19,18,3,58,72,81,95,55,32,14,1,47,19,92,96,6,30,76,40,40,37,77,75,19,6,30,38,7,54,88,68,73,5,71,97,78,51,58,99,49,72,66,97,57,58,58,63,54,33,69,60},
{37,12,1,56,18,31,60,92,51,14,59,90,19,29,87,63,47,10,28,96,82,94,58,39,17,16,68,38,15,3,64,52,15,65,74,100,62,0,92,12,14,50,2,33,46,55,63,59,65,91,20,46,50,79,51,34,61,19,72,76,89,35,95,3,67,68,69,28,68,60,41,82,77,43,82,22,98,44,47,28,0,67,74,50,11,92,84,72,77,21,14,65,23,8,34,90,42,2,84,10},
{63,24,58,5,33,5,94,97,15,40,24,15,6,65,32,18,56,82,56,32,70,70,97,93,78,30,48,87,99,31,97,27,22,20,32,55,93,25,52,7,31,42,90,4,6,88,89,62,35,44,60,4,81,56,63,24,52,10,10,17,8,73,44,30,94,77,51,86,68,69,59,66,11,48,70,84,1,58,12,37,68,72,41,48,95,71,73,12,47,83,29,55,56,74,51,15,16,2,67,50},
{71,92,15,82,6,51,66,7,75,44,44,43,15,52,57,9,22,96,89,35,79,17,91,0,57,7,82,73,9,14,90,81,5,4,28,11,22,60,19,97,3,29,5,86,81,63,61,69,58,49,71,2,67,27,69,90,34,50,29,44,64,18,91,36,89,85,47,10,45,32,7,14,62,12,100,8,41,61,44,100,9,14,68,42,41,37,99,75,87,27,85,17,45,75,53,33,26,66,10,71},
{99,84,85,60,62,51,68,3,11,11,69,87,92,36,96,32,39,94,74,93,87,58,9,31,100,28,30,25,94,6,62,92,90,12,17,52,29,86,55,40,63,90,94,21,92,55,53,31,14,93,23,0,17,99,98,16,26,27,7,86,34,35,78,90,13,95,41,43,46,62,49,76,51,42,97,9,63,15,40,77,8,63,43,25,61,40,7,53,68,81,38,68,82,82,57,95,43,65,37,55},
{93,87,30,10,95,93,19,58,75,59,0,83,88,44,74,14,50,47,67,17,94,71,51,75,53,75,69,96,5,73,16,98,59,13,7,19,5,93,43,80,17,44,28,4,54,68,18,3,14,51,88,7,22,4,48,41,45,17,2,50,90,18,14,14,31,88,33,3,81,77,49,98,87,44,2,6,11,87,76,93,4,63,66,26,34,14,33,79,98,35,29,53,19,43,67,51,30,66,20,77},
{8,69,75,61,79,43,33,91,96,9,49,100,38,14,25,72,28,58,51,92,59,46,44,79,55,77,96,51,9,15,28,17,50,69,45,29,11,78,86,6,53,34,73,92,48,98,29,43,22,46,34,47,92,79,25,12,55,87,64,64,68,58,48,18,93,59,13,70,2,99,76,56,32,14,13,46,12,42,89,0,89,23,13,46,1,5,59,22,92,89,53,60,12,67,44,4,92,57,74,94},
{55,15,15,53,30,28,99,8,71,88,75,59,77,88,4,44,93,29,66,51,17,85,10,96,17,54,100,8,77,73,2,31,89,17,50,85,46,48,93,83,35,67,7,11,54,78,21,13,7,88,64,91,38,74,87,56,94,86,64,70,25,32,67,80,50,16,64,62,30,56,10,32,89,17,9,8,95,31,21,68,18,85,59,22,24,11,78,84,97,42,19,88,40,86,67,90,68,30,17,99},
{52,27,30,40,44,5,49,5,36,70,73,20,21,31,43,11,42,20,96,5,28,14,93,69,67,26,24,34,56,8,99,75,35,95,14,46,0,29,51,36,66,23,57,87,21,100,98,29,86,59,0,81,74,60,15,40,86,39,40,7,47,5,82,49,100,63,95,66,92,11,2,57,0,25,9,21,91,74,17,76,32,17,22,72,43,37,78,28,77,18,36,90,90,84,38,89,46,99,21,4},
{9,90,27,10,14,3,98,4,77,14,46,75,99,35,47,41,72,24,70,48,8,72,4,98,55,42,53,68,7,74,72,16,63,99,26,43,1,24,13,44,4,25,19,2,60,32,10,32,22,80,46,98,17,50,95,38,59,13,5,66,87,77,48,15,42,41,58,9,31,71,54,35,97,39,4,56,37,14,88,59,60,0,56,77,50,17,81,75,30,87,6,84,29,55,99,37,96,57,47,26},
{94,67,27,56,5,98,12,8,11,66,67,37,66,90,80,83,6,61,23,2,47,30,86,42,51,51,80,46,74,26,38,67,59,31,23,64,29,1,38,6,33,4,44,100,60,90,48,32,50,71,1,63,67,87,5,17,3,51,29,77,77,33,10,35,65,100,65,60,0,2,32,33,73,42,99,100,32,12,31,48,84,99,11,50,86,83,34,55,33,63,32,76,97,8,77,27,7,7,53,74},
{76,85,73,14,27,72,13,59,50,11,73,33,9,84,50,61,32,84,16,31,12,14,6,8,89,49,1,96,56,54,35,31,39,7,46,32,45,59,57,96,36,29,95,46,80,10,73,11,94,89,9,73,69,15,47,57,31,49,18,87,69,53,18,74,27,30,5,38,55,28,33,92,58,95,3,37,4,76,14,65,31,23,37,66,5,50,23,36,99,41,22,68,61,6,7,88,2,13,92,58},
{41,92,15,65,86,18,1,56,60,83,87,57,5,90,23,10,40,12,12,38,19,35,72,80,7,80,33,10,59,25,34,66,16,49,31,68,33,99,23,59,47,10,16,53,100,5,29,39,17,42,44,2,43,82,49,16,27,82,93,86,73,26,18,55,75,49,89,7,13,79,33,61,55,15,80,20,20,75,60,3,83,70,5,92,17,54,8,45,2,0,30,41,27,14,63,68,29,51,42,43},
{96,75,70,50,90,49,71,9,90,97,79,73,66,50,64,83,4,72,27,73,39,24,80,32,4,42,100,34,60,41,43,55,82,12,5,71,27,42,46,16,38,24,89,3,41,19,52,11,57,46,84,96,36,29,27,40,72,94,40,98,0,83,18,83,95,90,53,88,31,66,71,69,56,59,38,97,44,57,7,1,2,57,97,4,87,91,10,24,84,51,21,84,33,39,66,95,96,86,82,26},
{51,52,96,73,78,33,70,21,90,77,89,58,0,86,28,87,42,39,10,25,56,98,75,89,2,7,49,98,59,98,24,76,15,86,48,59,18,17,81,75,61,69,99,61,20,27,13,62,32,90,53,88,87,95,42,89,1,58,53,60,55,43,1,70,28,49,29,12,33,76,53,60,10,52,87,98,45,100,25,43,89,79,97,41,73,4,96,40,62,48,66,16,91,67,53,85,82,48,98,14},
{90,50,74,66,68,26,63,12,25,89,55,80,33,17,20,72,22,83,11,84,30,77,67,88,9,86,72,91,33,35,72,89,86,11,54,53,38,17,32,29,72,53,76,71,71,62,42,93,44,19,76,41,62,42,28,71,27,66,27,26,1,99,14,87,10,35,5,14,52,37,43,90,91,18,60,27,81,68,19,24,87,95,31,48,3,59,18,97,92,11,90,93,10,70,45,20,4,16,34,22},
{54,43,11,10,62,37,37,8,4,22,99,57,83,30,4,86,55,89,49,46,0,38,38,77,74,49,97,79,66,97,0,86,5,79,62,33,15,65,41,87,87,6,9,35,2,14,21,57,69,36,3,35,40,7,11,13,23,74,92,55,36,93,40,42,37,68,75,18,32,83,71,85,89,81,19,91,61,6,13,29,8,16,65,48,91,76,62,80,16,19,34,52,78,74,94,14,7,69,33,5},
{17,3,56,5,84,41,62,44,48,75,40,56,58,71,71,14,12,99,94,28,17,27,81,96,67,74,76,74,8,75,45,25,79,0,97,28,41,58,39,55,100,45,11,23,15,48,37,27,46,97,56,63,90,36,24,56,76,0,96,85,41,40,9,19,6,6,14,47,30,19,2,96,64,80,18,45,27,21,72,39,17,94,1,6,96,93,28,72,59,90,56,100,96,31,86,1,3,66,15,0},
{85,17,96,14,63,81,59,90,1,97,28,19,57,96,92,52,54,87,23,12,76,45,79,72,43,64,39,46,29,54,12,80,37,8,60,100,89,85,55,56,47,49,75,3,45,33,56,99,19,45,78,61,91,56,99,33,86,4,45,81,58,58,60,96,32,19,61,87,70,16,42,16,65,84,20,76,83,42,41,68,87,18,28,77,40,94,76,25,98,88,5,21,11,31,16,43,16,44,29,86},
{60,37,1,24,20,88,67,69,29,7,36,16,25,65,59,65,24,1,56,21,89,61,42,100,58,25,8,74,69,3,25,95,40,26,85,27,81,51,96,9,58,32,25,49,63,51,80,87,52,35,74,40,62,82,5,19,73,13,59,7,16,84,1,56,77,53,49,57,3,45,66,28,43,58,77,72,8,57,58,60,92,98,66,20,79,71,39,52,84,65,59,100,48,27,21,91,80,71,47,83},
{82,80,10,24,37,54,62,45,10,86,71,68,83,36,88,27,6,94,79,56,58,4,55,72,98,42,63,77,12,9,25,60,89,2,50,92,56,11,2,32,97,73,100,79,75,88,73,47,47,17,2,4,21,23,42,18,66,4,61,44,81,87,71,35,89,20,27,10,32,96,42,95,69,41,40,9,95,12,23,41,29,25,11,17,15,54,1,47,24,63,57,4,49,27,40,3,48,33,13,46},
{95,55,40,29,96,46,39,57,58,62,98,54,53,76,71,68,29,72,81,53,34,38,24,49,65,30,52,79,29,31,24,23,86,31,53,48,77,92,4,1,19,68,55,72,9,92,6,38,63,87,58,64,24,82,79,56,78,98,34,6,28,25,29,81,22,82,28,65,39,99,66,58,32,87,97,42,78,2,46,7,55,3,71,46,51,49,1,28,46,1,34,41,26,30,21,48,11,49,80,17},
{13,45,75,11,99,37,53,76,39,66,83,95,35,19,40,87,69,7,81,81,8,82,21,35,11,42,49,89,57,95,5,36,40,47,14,38,84,33,80,23,99,29,84,34,48,90,87,16,97,67,64,71,48,51,72,59,60,88,48,83,82,53,86,21,66,100,25,50,32,72,39,31,0,22,65,48,78,51,31,40,84,61,10,32,11,83,57,71,70,4,20,51,24,5,39,90,4,30,5,36},
{1,44,33,68,66,64,16,9,81,13,49,65,74,60,97,51,42,19,89,11,24,8,28,14,13,67,70,84,64,76,86,65,19,19,100,52,83,15,61,64,95,10,95,34,70,57,85,78,76,73,55,66,47,83,80,60,16,16,9,80,92,96,10,77,14,9,28,63,91,56,93,85,32,87,18,68,43,70,45,19,42,66,85,56,48,31,82,30,47,92,9,4,87,87,81,67,96,76,29,87},
{31,89,37,63,75,22,97,85,92,41,70,100,73,20,55,20,51,37,17,64,28,93,68,81,79,15,47,75,91,42,27,88,30,64,16,72,52,12,56,43,19,25,43,92,45,64,78,63,0,95,26,95,54,61,75,32,76,88,73,32,30,66,86,26,97,1,98,48,80,19,92,99,10,0,56,56,64,33,85,65,95,77,59,48,3,0,46,45,88,19,77,84,51,62,10,47,29,74,96,8},
{94,53,73,3,53,28,25,16,62,76,47,22,53,73,70,22,73,15,68,60,0,10,44,52,73,54,65,68,94,60,77,53,79,15,23,31,44,48,14,72,91,27,94,9,100,29,31,72,44,99,32,11,9,76,29,48,96,94,15,55,20,58,8,99,40,31,97,84,45,77,55,35,3,14,44,3,43,42,75,87,40,73,64,15,14,93,29,76,53,11,31,73,69,39,37,8,70,100,58,81},
{76,79,16,80,93,26,49,35,68,23,89,75,63,18,56,77,11,86,53,30,97,84,2,31,89,5,6,24,5,64,4,47,43,87,26,1,13,41,3,47,65,92,88,94,9,44,70,87,29,89,16,25,72,85,56,26,57,62,50,62,93,55,8,1,7,1,2,20,42,5,34,73,63,21,66,39,31,2,25,60,91,8,51,29,59,74,55,15,1,5,77,94,26,52,95,33,19,64,20,27},
{35,54,0,99,41,32,37,73,34,28,99,92,2,50,20,62,23,75,77,24,46,20,85,72,38,45,72,57,75,92,84,10,11,50,75,18,83,78,91,83,72,56,74,75,72,60,36,95,1,79,85,47,99,35,19,36,47,91,59,21,48,43,31,59,59,72,77,7,49,34,91,21,56,30,96,27,57,98,88,58,76,38,4,41,74,90,43,20,46,2,7,94,11,39,18,70,77,62,78,26},
{62,34,47,17,30,8,10,87,72,98,44,47,1,15,54,75,4,98,61,17,100,69,10,10,74,96,46,50,23,23,42,85,23,55,68,54,29,44,40,0,41,51,14,42,66,68,84,36,31,10,53,30,45,30,6,85,25,53,1,14,42,43,65,66,65,32,86,94,42,25,95,83,42,8,91,74,42,40,10,74,51,63,70,62,59,77,47,50,96,48,64,3,57,28,35,21,26,20,15,68},
{12,9,16,54,84,74,28,92,13,4,65,30,33,1,93,93,78,5,42,39,53,73,42,9,0,78,98,94,98,12,61,76,88,44,30,37,17,24,28,97,28,60,27,61,27,86,53,4,91,62,9,9,34,17,85,0,61,82,94,25,60,21,0,13,65,30,50,48,54,45,44,48,71,37,9,98,89,62,68,45,23,43,54,23,60,5,24,21,87,17,12,13,4,12,26,69,9,43,83,29},
{88,94,78,24,30,87,21,86,14,55,30,4,98,51,27,57,56,17,44,8,35,56,21,39,69,14,75,44,57,23,73,10,16,50,34,13,2,55,99,17,9,95,21,6,45,14,29,0,32,74,9,33,96,97,38,30,10,79,74,33,2,47,43,85,63,77,98,66,98,62,83,73,57,70,45,68,50,75,69,82,14,44,81,9,6,19,40,84,64,80,16,66,26,60,51,90,36,14,55,34},
{43,3,73,100,73,18,67,89,93,1,37,6,11,17,82,85,2,88,68,67,68,50,99,60,9,15,49,12,30,70,12,73,73,85,38,11,2,71,67,95,39,3,67,16,20,15,0,90,69,34,22,36,85,20,63,94,36,11,72,32,48,84,71,87,69,75,65,37,11,31,99,50,34,31,33,20,46,100,76,15,34,98,17,18,18,80,78,20,58,16,18,72,100,55,58,34,96,89,72,6},
{86,36,23,86,67,56,6,80,21,48,61,55,46,78,39,30,24,84,50,48,100,34,19,65,89,43,100,84,32,37,56,17,73,79,3,5,0,76,85,22,23,45,43,35,23,83,65,13,32,14,61,31,14,46,96,2,89,61,52,87,64,8,4,2,53,74,8,54,15,93,42,38,4,85,40,94,67,4,6,99,86,33,96,100,79,58,69,33,85,20,20,49,95,91,17,14,64,25,68,79},
{85,76,83,89,60,22,82,94,27,54,58,79,87,54,78,31,78,12,64,62,100,84,10,94,74,28,7,37,19,41,82,70,16,31,58,43,19,5,36,12,59,94,91,11,13,69,42,91,81,6,53,80,90,29,40,30,23,13,33,9,21,15,79,3,12,37,46,31,8,48,44,34,42,34,45,21,69,54,12,16,60,65,96,15,60,1,45,84,82,45,93,2,60,71,5,38,74,18,69,49},
{66,12,83,74,47,94,96,15,47,74,31,6,4,94,89,64,61,100,13,42,44,72,44,70,9,16,7,83,34,77,98,66,55,80,40,1,74,1,84,20,41,81,94,45,40,48,8,1,47,89,43,58,60,54,27,69,36,1,18,70,44,15,1,99,96,7,0,35,75,50,21,15,30,14,60,37,62,35,38,76,23,47,33,49,67,60,18,2,27,2,38,71,17,6,70,79,13,36,80,89},
{86,1,3,82,15,30,18,44,31,22,19,54,36,52,69,69,78,53,72,5,55,76,42,73,82,11,17,62,47,98,50,99,99,19,81,80,15,65,23,46,54,8,66,56,60,35,24,4,88,62,76,43,38,17,82,86,29,65,47,42,62,63,41,26,49,88,6,64,18,96,10,72,4,42,94,64,77,18,34,31,80,9,40,84,27,21,70,22,86,83,64,14,46,4,40,61,92,46,24,10},
{42,0,48,12,9,42,76,86,26,77,83,5,86,22,56,79,43,92,0,96,40,65,76,52,35,15,12,94,28,3,3,36,3,17,48,79,25,90,65,51,66,47,23,18,36,79,97,79,36,98,40,76,28,15,28,63,98,40,56,25,43,25,27,13,9,75,92,34,30,22,86,97,36,75,81,72,19,77,16,55,40,23,97,68,4,24,31,1,31,53,93,40,79,19,19,88,60,78,88,91},
{66,39,53,1,13,33,39,32,76,22,53,16,11,16,84,15,40,81,17,37,34,76,44,79,96,63,32,21,6,86,11,73,25,30,40,4,29,46,3,5,68,56,21,79,72,71,60,79,18,77,82,52,53,25,97,14,55,95,35,61,80,13,33,4,9,74,9,39,19,12,10,53,34,98,98,73,68,57,17,52,0,99,3,19,24,66,100,79,60,34,39,40,13,39,44,23,79,19,28,64},
{98,38,16,32,35,80,71,69,36,88,21,2,86,91,21,76,57,87,20,83,21,26,22,0,65,33,90,9,18,17,73,16,55,55,14,56,34,85,92,36,38,79,5,90,35,93,66,58,80,86,41,67,78,29,67,8,62,57,17,47,74,90,63,96,44,43,17,44,27,75,47,65,53,52,54,55,10,86,12,90,38,53,56,15,49,23,24,77,46,41,23,19,98,86,81,7,95,65,18,21},
{39,31,52,59,49,73,13,59,24,25,49,62,45,4,44,60,94,34,36,39,41,60,25,4,11,72,12,6,36,97,94,76,27,12,34,76,85,13,34,75,4,83,3,49,54,47,8,47,47,11,53,88,71,44,59,48,15,71,54,52,67,14,27,94,26,27,69,77,6,69,51,10,52,54,26,72,67,0,85,80,11,37,34,48,81,93,97,97,29,16,14,96,30,7,55,56,34,90,99,6},
{58,50,16,76,70,8,47,3,9,32,49,87,69,83,35,16,75,98,79,3,13,93,65,44,100,86,66,100,75,65,5,33,81,88,75,16,97,22,86,72,54,35,58,89,17,59,71,59,56,49,28,70,41,60,80,40,45,11,5,20,42,10,19,22,99,94,5,61,82,91,32,1,25,90,57,9,49,27,34,71,43,62,40,50,21,86,91,33,98,62,53,39,73,38,28,37,98,33,98,80},
{90,29,47,82,85,3,57,100,98,91,71,40,18,77,90,6,63,46,39,26,8,58,31,47,96,59,84,59,58,47,38,48,76,52,96,26,55,52,26,52,42,63,58,26,5,48,32,68,60,37,60,68,95,92,14,56,16,64,15,75,10,19,89,52,71,84,79,26,1,71,44,43,100,2,35,4,16,68,39,76,4,99,10,100,56,91,21,73,55,36,13,31,56,1,84,93,51,28,85,52},
{65,29,61,64,98,96,68,13,29,73,55,34,38,65,100,94,56,87,32,77,23,45,7,45,12,91,37,29,85,22,47,49,17,74,12,14,70,47,94,65,86,48,99,23,13,64,84,35,51,15,11,40,27,18,51,5,76,88,1,26,76,48,76,59,22,54,73,58,67,32,22,53,81,88,76,60,17,25,95,34,7,5,40,34,90,91,5,31,45,6,58,20,21,33,80,9,53,18,67,20},
{51,55,73,31,42,14,57,26,40,51,60,13,22,0,47,78,91,18,9,1,92,33,22,79,32,68,88,85,86,20,71,2,75,43,100,84,24,56,9,30,6,35,43,95,1,56,73,59,40,48,60,31,81,82,9,12,15,97,63,1,83,34,70,58,43,70,41,67,25,16,63,99,17,5,93,19,27,31,78,68,79,37,99,59,86,75,37,0,37,67,68,20,0,38,78,43,7,85,77,99},
{67,39,97,84,11,90,2,38,20,46,5,100,50,71,24,35,45,28,1,82,95,36,68,61,40,11,70,47,62,46,11,28,52,8,79,63,98,81,67,84,94,39,49,43,9,40,78,20,68,45,68,28,81,36,89,20,47,58,33,9,71,45,37,22,53,82,51,16,29,84,100,22,22,15,65,98,55,8,17,22,19,86,16,0,21,4,87,34,28,20,43,99,31,47,87,50,28,3,66,57},
{88,31,45,76,46,9,74,0,84,91,89,3,42,4,3,63,8,56,98,3,76,6,1,73,53,55,22,48,58,54,71,11,86,16,88,98,92,61,99,76,17,53,79,60,58,48,89,32,3,52,35,46,59,3,18,78,24,7,92,48,61,63,60,12,79,47,10,70,74,75,11,91,27,90,16,51,3,5,84,74,57,85,19,15,54,3,60,44,10,51,93,38,13,52,50,58,65,60,28,38},
{34,39,95,28,96,11,79,99,16,28,38,73,80,57,55,100,27,14,44,3,65,36,41,79,54,92,2,18,17,30,56,18,36,50,46,98,27,24,62,43,19,0,83,99,23,37,98,50,51,41,20,82,43,61,26,97,18,29,14,2,25,36,20,61,53,66,24,80,56,87,90,41,87,72,39,9,8,3,26,25,44,46,73,54,73,100,50,58,95,31,60,19,67,80,47,86,11,71,32,33},
{23,21,75,9,93,80,86,67,83,11,58,94,23,30,47,96,96,63,19,56,94,79,42,27,24,89,12,1,25,44,35,49,65,76,58,23,21,9,90,4,87,13,64,9,10,77,72,72,39,91,28,33,70,70,60,60,24,72,62,49,83,63,64,47,4,89,37,25,98,26,96,85,6,25,94,16,1,31,54,41,22,48,74,58,17,100,17,7,71,45,57,19,74,20,67,78,75,3,70,73},
{96,65,57,68,57,16,50,58,14,4,99,36,52,38,60,36,37,43,43,75,89,66,94,62,53,60,6,27,29,76,100,92,6,22,59,63,5,9,21,19,13,86,21,31,24,47,67,61,90,10,35,44,42,29,73,95,55,79,22,51,54,88,42,26,10,0,56,82,9,77,67,89,28,88,20,52,34,53,80,90,29,14,34,72,9,6,66,65,85,54,82,4,42,23,97,18,23,52,100,100},
{95,66,54,23,19,40,75,19,60,20,8,89,35,42,60,10,48,93,41,99,46,22,69,54,45,66,38,35,17,37,0,12,69,54,35,54,61,76,73,20,97,48,8,98,90,35,7,4,94,15,69,5,37,38,60,83,3,98,84,20,1,84,99,36,3,100,57,64,76,96,50,38,43,25,35,100,60,8,70,53,23,38,58,27,42,84,76,11,48,59,99,15,8,97,51,11,97,7,42,38},
{70,58,76,12,83,77,11,42,51,47,61,75,86,86,68,94,69,43,5,16,1,3,31,9,100,49,87,62,22,95,100,92,53,41,71,35,17,48,44,69,96,4,9,47,56,77,40,25,86,45,7,87,48,5,62,14,20,48,76,8,43,76,67,62,16,37,97,0,85,6,35,80,78,10,26,33,53,33,24,38,78,32,24,93,3,52,6,90,100,48,98,8,90,64,70,6,67,33,73,52},
{39,7,98,16,84,91,16,36,23,40,74,67,38,64,59,41,15,31,97,81,80,61,56,35,24,25,41,92,24,80,9,30,53,6,12,36,97,28,72,86,69,11,53,6,75,78,14,56,76,10,37,55,37,93,56,62,84,98,19,75,43,28,4,97,0,83,32,98,11,71,49,80,82,1,52,23,80,66,45,55,43,48,76,80,40,31,7,91,95,93,31,38,20,1,0,88,84,32,51,95},
{2,100,40,85,1,59,74,47,91,18,68,33,67,9,80,73,6,53,29,1,46,60,5,32,61,5,86,11,3,36,72,6,36,12,57,37,71,97,50,61,14,17,61,47,93,6,20,99,25,15,66,37,76,71,36,2,42,21,80,12,58,52,18,94,30,41,97,67,3,12,94,17,96,54,31,88,26,51,86,18,66,52,55,7,89,91,77,98,79,56,9,36,74,94,96,3,34,92,70,37},
{3,64,20,65,84,51,52,77,68,37,95,0,55,15,7,10,6,50,7,85,73,16,87,46,9,82,50,9,39,86,12,8,49,32,73,100,50,24,76,17,27,70,17,83,51,92,93,23,7,66,74,80,82,60,26,57,41,42,66,80,27,78,88,77,76,26,42,25,50,17,9,78,53,26,26,3,84,85,27,92,50,0,71,31,27,63,88,34,4,19,14,32,97,68,75,72,95,16,64,10},
{100,73,88,52,65,80,21,49,64,14,6,13,15,77,10,8,6,64,42,10,83,22,8,45,91,49,84,51,65,47,27,30,86,82,82,50,61,70,65,92,84,71,71,65,14,82,73,20,11,15,97,61,37,5,72,94,54,55,10,86,68,38,15,53,19,64,70,80,33,34,37,16,72,8,82,86,56,54,5,33,69,1,94,73,73,66,66,27,87,77,79,55,14,94,74,100,57,43,45,90},
{44,83,73,15,91,54,0,46,74,72,79,9,39,39,82,12,71,13,5,57,90,84,11,70,77,52,69,0,95,14,56,38,63,28,19,53,48,19,65,89,57,9,98,97,14,45,8,85,58,80,42,14,63,19,50,5,71,86,72,66,66,28,70,28,56,90,81,71,75,11,59,32,87,56,28,1,67,2,86,91,82,27,71,10,47,21,82,17,6,54,49,38,82,86,66,3,75,12,74,15},
{23,99,47,9,20,75,10,87,43,63,44,91,90,14,0,2,35,83,87,7,2,1,45,84,87,77,53,27,89,94,43,78,92,90,88,12,31,64,65,74,93,8,65,49,23,31,51,24,80,3,99,82,5,9,31,92,87,85,19,41,78,62,19,35,17,73,13,48,2,79,89,96,53,19,44,42,50,61,67,30,65,31,78,36,40,9,94,93,60,12,34,3,40,53,38,24,92,52,72,94},
{97,60,89,15,79,99,58,96,26,91,92,91,21,69,93,27,44,86,20,3,65,54,6,71,73,11,95,64,29,67,23,92,93,79,6,38,77,30,33,2,20,91,59,7,59,51,1,3,3,21,73,68,41,46,4,80,57,100,9,86,32,32,43,24,10,49,28,88,80,27,56,66,17,82,40,77,32,41,46,1,28,85,35,69,30,40,14,53,39,23,4,71,55,47,61,66,97,56,19,42},
{83,41,74,0,22,80,77,21,20,89,22,14,73,58,83,70,98,63,22,2,86,27,39,41,40,66,73,36,21,92,44,4,32,85,4,21,64,47,42,85,1,64,65,40,88,48,9,51,77,99,53,63,92,58,3,31,24,76,34,11,33,44,15,31,28,86,52,93,99,94,43,100,24,7,40,11,21,15,63,99,13,82,61,4,40,30,2,30,72,36,41,71,80,23,1,8,8,20,67,7}
};

int g_qCount = 0;
NODE rgnNodes[NUM_NODES];
int ch;
int iPrev, iNode;
int i, iCost, iDist;


void print_path (NODE *rgnNodes_l, int chNode)
{
  if (rgnNodes_l[chNode].iPrev != NONE)
    {
      print_path(rgnNodes_l, rgnNodes_l[chNode].iPrev);
    }
  printf (" %d", chNode);
}


void enqueue (int iNode_l, int iDist_l, int iPrev_l)
{
  QITEM *qNew = (QITEM *) malloc(sizeof(QITEM));
  QITEM *qLast = qHead;
  
  if (!qNew) 
    {
      printf("Out of memory.\n");
      exit(1);
    }
  qNew->iNode = iNode_l;
  qNew->iDist = iDist_l;
  qNew->iPrev = iPrev_l;
  qNew->qNext = NULL;
  
  if (!qLast) 
    {
      qHead = qNew;
    }
  else
    {
      while (qLast->qNext) qLast = qLast->qNext;
      qLast->qNext = qNew;
    }
  g_qCount++;
  //               ASSERT(g_qCount);
}


void dequeue (int *piNode, int *piDist, int *piPrev)
{
  QITEM *qKill = qHead;
  
  if (qHead)
    {
      //                 ASSERT(g_qCount);
      *piNode = qHead->iNode;
      *piDist = qHead->iDist;
      *piPrev = qHead->iPrev;
      qHead = qHead->qNext;
      free(qKill);
      g_qCount--;
    }
}


int qcount (void)
{
  return(g_qCount);
}

void dijkstra(int chStart, int chEnd) 
{
  

  
  for (ch = 0; ch < NUM_NODES; ch++)
    {
      rgnNodes[ch].iDist = NONE;
      rgnNodes[ch].iPrev = NONE;
    }

  if (chStart == chEnd) 
    {
      printf("Shortest path is 0 in cost. Just stay where you are.\n");
    }
  else
    {
      rgnNodes[chStart].iDist = 0;
      rgnNodes[chStart].iPrev = NONE;
      
      enqueue (chStart, 0, NONE);
      
     while (qcount() > 0)
	{
	  dequeue (&iNode, &iDist, &iPrev);
	  for (i = 0; i < NUM_NODES; i++)
	    {
	      if ((iCost = AdjMatrix[iNode][i]) != NONE)
		{
		  if ((NONE == rgnNodes[i].iDist) || 
		      (rgnNodes[i].iDist > (iCost + iDist)))
		    {
		      rgnNodes[i].iDist = iDist + iCost;
		      rgnNodes[i].iPrev = iNode;
		      enqueue (i, iDist + iCost, iNode);
		    }
		}
	    }
	}
      
      printf("Shortest path is %d in cost. ", rgnNodes[chEnd].iDist);
      printf("Path is: ");
      print_path(rgnNodes, chEnd);
      printf("\n");
    }
}

int main() {
  int i_l,j,k;
  
  /* make a fully connected matrix */
  //for (i=0;i<NUM_NODES;i++) {
  //  for (j=0;j<NUM_NODES;j++) {
  //    /* make it more sparce */
  //    scanf(fp,"%d",&k);
  //  		AdjMatrix[i][j]= k;
  //  }
  //}

  /* finds 10 shortest paths between nodes */
  for (i_l=0,j=NUM_NODES/2;i_l<100;i_l+=2,j+=2) {
			j=j%NUM_NODES;
      dijkstra(i_l,j);
  }
  return 0;
}
