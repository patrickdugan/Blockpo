#include <string>
#include <cmath>
#include "omnicore/omnicore.h"  // getbalance function and others
#include "omnicore/tally.h"
using namespace mastercore;

int margin_Logic(int contractsExecuted,CMPcontractDex* contractdex, CMPcontract contract)
{
  std::string address =  contractdex.address;  // adrdess for contract in dex
  int propertyId = contractdex.propertyId;
  int contractInfo = contract.getContractInfo(propertyId);
  uint64_t marginId = contract.marginProperty;
  int oldBalance = getMPbalance(address, propertyId,BALANCE);
  //set-up logic about closing gains/losses or not
  int contractsClosed = 0;
  int cont = 0;
  int total = 0;

  //apply clean math from the signed int provided as parameter
  int newBalance = oldBalance + contractsExecuted;

  if((abs(oldBalance) > abs(newBalance)) && (abs(contractsExecuted) < oldBalance)){

         contractsClosed = oldBalance-newBalance;

  }else if((abs(contractsExecuted) > abs(oldBalance)) && (contractsExecuted+oldBalance != 0)){

         contractsClosed = oldBalance;
    }

    //calculate reserve to free pre-loss
  int64_t freedReserveExPNL = (contractInfo.marginRequirement) *contractsClosed;

    //look-up contract prices, take difference of execution price to average
  const trades_lenght = thisTrade.matches.lenght; // this is only the idea

  for( k = 0; k < trades_lenght ; k++ ) { total = total + thisTrade.matches[k]; }

  double avgerageExecution = total / (trades_lenght);

  //calls function to return map //of contracts not netted on this address
  uint64_t myContracts [lenght] = contract.getMyContracts(thisAddress); // we need some wisdom from Pat
  const contracts_lenght = myContracts.lenght; // this is only the idea

  for( j = 0; j < contracts_lenght ; j++ ) { cont = cont + myContracts[j]; }

  double averageCostBasis = cont / (contracts_lenght);
  double PNL = averageExecution + (averageCostBasis * contractsClosed);
  int64_t oldReserve = getMPbalance(marginId, propertyId,SELLOFFER_RESERVE); // selloffer_reserve is fine? or we must use ACCEPT_RESERVE?
  int64_t newReserve = oldReserve - freedReserveExPNL-abs(PNL);
  assert(update_tally_map(address,marginId, newReserve, SELLOFFER_RESERVE));
  oldMarginBalance = TallyMap.retrieveBalance(marginid,thisaddress);
  newMarginBalance = oldMarginBalance+freedReserveExPNL-abs(PNL);

  //modify TallyMap for new contract balances and margin balances
  assert(update_tally_map(address,contract, newBalance, BALANCE));
  assert(update_tally_map(address, marginId, newMarginBalance, BALANCE));



}
