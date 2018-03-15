#include "omnicore/mdex.h"

#include "omnicore/errors.h"
#include "omnicore/fees.h"
#include "omnicore/log.h"
#include "omnicore/omnicore.h"
#include "omnicore/rules.h"
#include "omnicore/sp.h"
#include "omnicore/tx.h"
#include "omnicore/uint256_extensions.h"
#include "omnicore/consensushash.h"
#include "omnicore/test/utils_tx.h"
#include "omnicore/tally.h"
#include "leveldb/db.h"
#include "arith_uint256.h"
#include "chain.h"
#include "main.h"
#include "tinyformat.h"
#include "uint256.h"
#include "test/test_bitcoin.h"

#include <univalue.h>

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/rational.hpp>
#include <boost/test/unit_test.hpp>

#include <openssl/sha.h>

#include <assert.h>
#include <stdint.h>

#include <fstream>
#include <limits>
#include <map>
#include <set>
#include <string>


using namespace mastercore;

BOOST_FIXTURE_TEST_SUITE(db_test, BasicTestingSetup)

BOOST_AUTO_TEST_CASE(simple_trade)
{
  const uint256 tx,tx1;
  CMPContractDex object(
          "1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", // address
          1,  // block
          3,  // property for sale
          8,  // amount for sale
          3,  // desired property
          0,  // amount desired <-- is this correct?
          tx, // txid
          2,  // position in block
          1,  // subaction
          1,  // amount remaining that we don't going to use
          5,  // desired price
          5  // for sale price
  );

	CMPContractDex *q;
	q = &object;
  CMPTradeList *t_tradelistdb;
  std::vector<uint256> vecTransactions;
  t_tradelistdb = new CMPTradeList(GetDataDir() / "MP_tradelist", fReindex);
  t_tradelistdb->recordNewTrade(q->getHash(), q->getAddr(), q->getProperty(), q->getDesProperty(), q->getBlock(), q->getIdx());
  t_tradelistdb->printAll();

}

BOOST_AUTO_TEST_CASE(match_trade)
{
  const uint256 tx,tx1;
  CMPContractDex object(
          "1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", // address
          1,  // block
          3,  // property for sale
          8,  // amount for sale
          3,  // desired property
          0,  // amount desired <-- is this correct?
          tx, // txid
          2,  // position in block
          1,  // subaction
          1,  // amount remaining that we don't going to use
          5,  // desired price
          5  // for sale price
  ); // the seller
  CMPContractDex object2(
					"1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", // address
					1,  // block
					3,  // property for sale
					0,  // amount for sale <-- is this correct?
					3,  // desired property
					8,  // amount desired
					tx1, // txid
					1,  // position in block
					1,  // subaction
					1,  // amount remaining that we don't going to use
					5,  // desired price
					5   // for sale price
	); // the buyer
  std::string t_status = "Netted";
  int64_t tradingFee = 0;
	CMPContractDex *pnew,*pold;
	pnew = &object;
  pold = &object2;
  CMPTradeList *t_tradelistdb1;
  std::vector<uint256> vecTransactions;
  t_tradelistdb1 = new CMPTradeList(GetDataDir() / "MP_tradelist", fReindex);
  t_tradelistdb1->recordMatchedTrade(pold->getHash(), pnew->getHash(),pold->getAddr(), pnew->getAddr(), pold->getDesProperty(), pnew->getDesProperty(), pold->getAmountForSale(), pnew->getAmountDesired(), pnew->getBlock(), tradingFee, t_status);
  t_tradelistdb1->printAll();

}
BOOST_AUTO_TEST_SUITE_END()
