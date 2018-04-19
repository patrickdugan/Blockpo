#include <stdio.h>
#include <string>

using namespace std;

volatile uint64_t marketPrice;
uint32_t blocksUntilExpiration;
uint32_t notionalSize;
uint32_t collateralCurrency;
uint32_t marginRequirementContract;
int64_t priceIndex = 200; // an index price to calculate the interest in pegg currencies
double percentLiqPrice;
