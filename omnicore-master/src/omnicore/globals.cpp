#include <stdio.h>
#include <string>

using namespace std;

volatile uint64_t marketPrice;
uint32_t blocksUntilExpiration;
uint32_t notionalSize;
uint32_t collateralCurrency;
uint32_t marginRequirementContract;
double percentLiqPrice;
volatile uint64_t marketP[10]; // using a simple array to get market prices
