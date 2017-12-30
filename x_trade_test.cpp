#include "omnicore/createpayload.h"
#include "omnicore/encoding.h"
#include "base58.h"
#include "coins.h"
#include "script/script.h"
#include "script/standard.h"
#include "utilstrencodings.h"

#include "omnicore/consensushash.h"
#include "omnicore/test/utils_tx.h"
#include "omnicore/tally.h"
#include "omnicore/mdex.h"
#include "omnicore/mdex.cpp"
#include "omnicore/errors.h"
#include "omnicore/fees.h"
#include "omnicore/log.h"
#include "omnicore/omnicore.h"
#include "omnicore/rules.h"
#include "omnicore/sp.h"
#include "omnicore/tx.h"
#include "omnicore/uint256_extensions.h"
#include "omnicore/script.h"
#include "primitives/transaction.h"
#include "test/test_bitcoin.h"
#include "arith_uint256.h"
#include "chain.h"
#include "main.h"
#include "tinyformat.h"

#include "arith_uint256.h"
#include "sync.h"
#include "uint256.h"


#include "base58.h"
#include "coins.h"
#include "core_io.h"
#include "main.h"
#include "primitives/transaction.h"
#include "script/script.h"
#include "script/standard.h"
#include "test/test_bitcoin.h"
#include "utilstrencodings.h"




#include <univalue.h>

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/rational.hpp>

#include <openssl/sha.h>

#include <assert.h>
#include <stdint.h>

#include <fstream>
#include <limits>
#include <map>
#include <set>
#include <string>
#include <boost/test/unit_test.hpp>
#include <limits>
#include <vector>
#include <sstream>

#define PACKET_SIZE         31
#define MAX_PACKETS        255
#define PACKET_SIZE_CLASS_A 19

/*Remember: This new function was defined to build the new class CMPContractDex*/
namespace mastercore
{
	extern std::string GenerateConsensusString(const CMPContractDex &tradeObj);
}
/** Creates a dummy transaction with the given inputs and outputs. */
static CTransaction TxClassC(const std::vector<CTxOut>& txInputs, const std::vector<CTxOut>& txOuts)
{
		CMutableTransaction mutableTx;

		// Inputs:
		for (std::vector<CTxOut>::const_iterator it = txInputs.begin(); it != txInputs.end(); ++it)
		{
				const CTxOut& txOut = *it;

				// Create transaction for input:
				CMutableTransaction inputTx;
				unsigned int nOut = 0;
				inputTx.vout.push_back(txOut);
				CTransaction tx(inputTx);

				// Populate transaction cache:
				CCoinsModifier coins = view.ModifyCoins(tx.GetHash());

				if (nOut >= coins->vout.size()) {
						coins->vout.resize(nOut+1);
				}
				coins->vout[nOut].scriptPubKey = txOut.scriptPubKey;
				coins->vout[nOut].nValue = txOut.nValue;

				// Add input:
				CTxIn txIn(tx.GetHash(), nOut);
				mutableTx.vin.push_back(txIn);
		}

		for (std::vector<CTxOut>::const_iterator it = txOuts.begin(); it != txOuts.end(); ++it)
		{
				const CTxOut& txOut = *it;
				mutableTx.vout.push_back(txOut);
		}

		return CTransaction(mutableTx);
}

/** Helper to create a CTxOut object. */
static CTxOut createTxOut(int64_t amount, const std::string& dest)
{
		return CTxOut(amount, GetScriptForDestination(CBitcoinAddress(dest).Get()));
}

bool direction = false; // true: seller inserted to priceMap, buyer on x_Trade.false: vice-versa.

BOOST_FIXTURE_TEST_SUITE(omnicore_contractdex_object, BasicTestingSetup)

BOOST_AUTO_TEST_CASE(no_match_price)  // seller_amount = 10, buyer_amount = 10;
{
	CMPTally tally;  // the tally map object
	const uint256 tx;
	CMPContractDex seller(
					"1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", // address
					1,  // block
					3,  // property for sale
					10,  // amount of contracts for sale
					0,  // desired property
					0,
					tx, // txid
					1,  // position in block
					1,  // subaction
					0,  // amount remaining
					15,  // effective_price
					2 // trading_action
	);

	CMPContractDex buyer(
					"1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", // address
					1,  // block
					3,  // property for sale
					10,  // amount of contracts for trade
					0,   // desired property
					0,
					tx, // txid
					2,  // position in block
					1,  // subaction
					0,  // amount remaining
					5,  // effective_price
					1 // trading_action
	);
	CMPContractDex *s;
	s = &seller;
	CMPContractDex *b;
	b = &buyer;

	BOOST_CHECK_EQUAL(3, seller.getProperty());
	BOOST_CHECK_EQUAL("1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", seller.getAddr());
	BOOST_CHECK_EQUAL(10, seller.getAmountForSale());
	BOOST_CHECK_EQUAL(2, seller.getTradingAction());  // seller
	BOOST_CHECK_EQUAL(15, seller.getEffectivePrice());

	BOOST_CHECK_EQUAL(3, buyer.getProperty());
	BOOST_CHECK_EQUAL("1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", buyer.getAddr());
	BOOST_CHECK_EQUAL(10, buyer.getAmountForSale());
	BOOST_CHECK_EQUAL(1, buyer.getTradingAction()); //buyer
	BOOST_CHECK_EQUAL(5, buyer.getEffectivePrice());

	BOOST_CHECK(mastercore::update_tally_map(seller.getAddr(), seller.getProperty(),90 ,NEGATIVE_BALANCE)); // putting some money here
	BOOST_CHECK(mastercore::update_tally_map(buyer.getAddr(), buyer.getProperty(),400, POSSITIVE_BALANCE));


	BOOST_CHECK_EQUAL(0,getMPbalance(seller.getAddr(), seller.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(90,getMPbalance(seller.getAddr(), seller.getProperty(),NEGATIVE_BALANCE));
	BOOST_CHECK_EQUAL(400,getMPbalance(buyer.getAddr(), buyer.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(buyer.getAddr(), buyer.getProperty(),NEGATIVE_BALANCE));

	if (direction){
		 BOOST_TEST_MESSAGE("The seller is inserted in priceMap, the buyer in x_Trade");
		 BOOST_CHECK(ContractDex_INSERT(seller));
		 BOOST_CHECK_EQUAL(NOTHING, x_Trade(b));
	}else {
		BOOST_TEST_MESSAGE("The buyer is inserted in priceMap, the seller in x_Trade");
		BOOST_CHECK(ContractDex_INSERT(buyer));
		BOOST_CHECK_EQUAL(TRADED, x_Trade(s));
	}

	BOOST_CHECK_EQUAL(0,getMPbalance(seller.getAddr(), seller.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(90,getMPbalance(seller.getAddr(), seller.getProperty(),NEGATIVE_BALANCE));
	BOOST_CHECK_EQUAL(400,getMPbalance(buyer.getAddr(), buyer.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(buyer.getAddr(), buyer.getProperty(),NEGATIVE_BALANCE));


}
BOOST_AUTO_TEST_CASE(no_match_propertyid)  // seller_amount = 10, buyer_amount = 10;
{
	CMPTally tally;  // the tally map object
	const uint256 tx;
	CMPContractDex seller(
					"1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", // address
					1,  // block
					6,  // property for sale
					10,  // amount of contracts for sale
					0,  // desired property
					0,
					tx, // txid
					1,  // position in block
					1,  // subaction
					0,  // amount remaining
					5,  // effective_price
					2 // trading_action
	);

	CMPContractDex buyer(
					"1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", // address
					1,  // block
					3,  // property for sale
					10,  // amount of contracts for trade
					0,   // desired property
					0,
					tx, // txid
					2,  // position in block
					1,  // subaction
					0,  // amount remaining
					5,  // effective_price
					1 // trading_action
	);
	CMPContractDex *s;
	s = &seller;
	CMPContractDex *b;
	b = &buyer;

	BOOST_CHECK_EQUAL(3, seller.getProperty());
	BOOST_CHECK_EQUAL("1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", seller.getAddr());
	BOOST_CHECK_EQUAL(10, seller.getAmountForSale());
	BOOST_CHECK_EQUAL(2, seller.getTradingAction());  // seller
	BOOST_CHECK_EQUAL(15, seller.getEffectivePrice());

	BOOST_CHECK_EQUAL(3, buyer.getProperty());
	BOOST_CHECK_EQUAL("1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", buyer.getAddr());
	BOOST_CHECK_EQUAL(10, buyer.getAmountForSale());
	BOOST_CHECK_EQUAL(1, buyer.getTradingAction()); //buyer
	BOOST_CHECK_EQUAL(5, buyer.getEffectivePrice());

	BOOST_CHECK(mastercore::update_tally_map(seller.getAddr(), seller.getProperty(),90 ,NEGATIVE_BALANCE)); // putting some money here
	BOOST_CHECK(mastercore::update_tally_map(buyer.getAddr(), buyer.getProperty(),400, POSSITIVE_BALANCE));


	BOOST_CHECK_EQUAL(0,getMPbalance(seller.getAddr(), seller.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(90,getMPbalance(seller.getAddr(), seller.getProperty(),NEGATIVE_BALANCE));
	BOOST_CHECK_EQUAL(400,getMPbalance(buyer.getAddr(), buyer.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(buyer.getAddr(), buyer.getProperty(),NEGATIVE_BALANCE));

	if (direction){
		 BOOST_TEST_MESSAGE("The seller is inserted in priceMap, the buyer in x_Trade");
		 BOOST_CHECK(ContractDex_INSERT(seller));
		 BOOST_CHECK_EQUAL(NOTHING, x_Trade(b));
	}else {
		BOOST_TEST_MESSAGE("The buyer is inserted in priceMap, the seller in x_Trade");
		BOOST_CHECK(ContractDex_INSERT(buyer));
		BOOST_CHECK_EQUAL(TRADED, x_Trade(s));
	}

	BOOST_CHECK_EQUAL(0,getMPbalance(seller.getAddr(), seller.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(90,getMPbalance(seller.getAddr(), seller.getProperty(),NEGATIVE_BALANCE));
	BOOST_CHECK_EQUAL(400,getMPbalance(buyer.getAddr(), buyer.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(buyer.getAddr(), buyer.getProperty(),NEGATIVE_BALANCE));


}
BOOST_AUTO_TEST_CASE(equal_amount)
{                            // seller_amount = buyer_amount = 10

  CMPTally tally;  // the tally map object
  const uint256 tx;   // address,block,property,amount for sale, desired property, amount desired,uint256 tx,idx, suba, amount remaining,desire price, for sale price
	CMPContractDex seller(
					"1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", // address
					1,  // block
					3,  // property for sale
					10,  // amount of contracts for sale
					0,  // desired property
					0,
					tx, // txid
					1,  // position in block
					1,  // subaction
					0,  // amount remaining
					5,  // effective_price
				  2 // trading_action
	);

	CMPContractDex buyer(
					"1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", // address
					1,  // block
					3,  // property for sale
					10,  // amount of contracts for trade
					0,   // desired property
					0,
					tx, // txid
					2,  // position in block
					1,  // subaction
					0,  // amount remaining
					5,  // effective_price
					1 // trading_action
	);
	CMPContractDex *s;
	s = &seller;
	CMPContractDex *b;
	b = &buyer;

	BOOST_CHECK_EQUAL(3, seller.getProperty());
	BOOST_CHECK_EQUAL("1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", seller.getAddr());
	BOOST_CHECK_EQUAL(10, seller.getAmountForSale());
	BOOST_CHECK_EQUAL(2, seller.getTradingAction());  // seller
	BOOST_CHECK_EQUAL(5, seller.getEffectivePrice());

  BOOST_CHECK_EQUAL(3, buyer.getProperty());
  BOOST_CHECK_EQUAL("1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", buyer.getAddr());
  BOOST_CHECK_EQUAL(10, buyer.getAmountForSale());
  BOOST_CHECK_EQUAL(1, buyer.getTradingAction()); //buyer
	BOOST_CHECK_EQUAL(5, buyer.getEffectivePrice());

	// BOOST_CHECK(mastercore::update_tally_map(seller.getAddr(), seller.getProperty(), 1000,POSSITIVE_BALANCE)); // putting some money here
  // BOOST_CHECK(mastercore::update_tally_map(buyer.getAddr(), buyer.getProperty(),2000, POSSITIVE_BALANCE));


	BOOST_CHECK_EQUAL(0,getMPbalance(seller.getAddr(), seller.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(seller.getAddr(), seller.getProperty(),NEGATIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(buyer.getAddr(), buyer.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(buyer.getAddr(), buyer.getProperty(),NEGATIVE_BALANCE));

  if (direction){
		 BOOST_TEST_MESSAGE("The seller is inserted in priceMap, the buyer in x_Trade");
	   BOOST_CHECK(ContractDex_INSERT(seller));
		 BOOST_CHECK_EQUAL(TRADED, x_Trade(b));   // the buyer wants 10 contracts at  price of 5!
																					 // the seller has 10 contracts at price of 5!
																					 // There's  match!!!!
	}else {
		BOOST_TEST_MESSAGE("The buyer is inserted in priceMap, the seller in x_Trade");
		BOOST_CHECK(ContractDex_INSERT(buyer));
	  BOOST_CHECK_EQUAL(TRADED, x_Trade(s));

	}

	BOOST_CHECK_EQUAL(0,getMPbalance(seller.getAddr(), seller.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(10,getMPbalance(seller.getAddr(), seller.getProperty(),NEGATIVE_BALANCE));
	BOOST_CHECK_EQUAL(10,getMPbalance(buyer.getAddr(), buyer.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(buyer.getAddr(), buyer.getProperty(),NEGATIVE_BALANCE));

  // //Check the amounts remainings on the objects
	// //
	// BOOST_CHECK_EQUAL("1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", seller.getAddr());
	// BOOST_CHECK_EQUAL(0, seller.getAmountForSale());
	// BOOST_CHECK_EQUAL(1, seller.getTradingAction());  // seller
	// BOOST_CHECK_EQUAL(5, seller.getEffectivePrice());
	//
	// BOOST_CHECK_EQUAL("1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", buyer.getAddr());
	// BOOST_CHECK_EQUAL(0, buyer.getAmountForSale());
	// BOOST_CHECK_EQUAL(2, buyer.getTradingAction()); //buyer
	// BOOST_CHECK_EQUAL(5, buyer.getEffectivePrice());




}


BOOST_AUTO_TEST_CASE(moreinseller)  // seller_amount = 20, buyer_amount = 15;
{

	CMPTally tally;  // the tally map object
	const uint256 tx;
	CMPContractDex seller(
					"1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", // address
					1,  // block
					3,  // property for sale
					20,  // amount of contracts for sale
					0,  // desired property
					0,
					tx, // txid
					1,  // position in block
					1,  // subaction
					0,  // amount remaining
					5,  // effective_price
					2 // trading_action
	);

	CMPContractDex buyer(
					"1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", // address
					1,  // block
					3,  // property for sale
					15,  // amount of contracts for trade
					0,   // desired property
					0,
					tx, // txid
					2,  // position in block
					1,  // subaction
					0,  // amount remaining
					5,  // effective_price
					1 // trading_action
	);
	CMPContractDex *s;
	s = &seller;
	CMPContractDex *b;
	b = &buyer;

	BOOST_CHECK_EQUAL(3, seller.getProperty());
	BOOST_CHECK_EQUAL("1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", seller.getAddr());
	BOOST_CHECK_EQUAL(20, seller.getAmountForSale());
	BOOST_CHECK_EQUAL(2, seller.getTradingAction());  // seller
	BOOST_CHECK_EQUAL(5, seller.getEffectivePrice());

	BOOST_CHECK_EQUAL(3, buyer.getProperty());
	BOOST_CHECK_EQUAL("1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", buyer.getAddr());
	BOOST_CHECK_EQUAL(15, buyer.getAmountForSale());
	BOOST_CHECK_EQUAL(1, buyer.getTradingAction()); //buyer
	BOOST_CHECK_EQUAL(5, buyer.getEffectivePrice());

	// BOOST_CHECK(mastercore::update_tally_map(seller.getAddr(), seller.getProperty(), 1000,POSSITIVE_BALANCE)); // putting some money here
	// BOOST_CHECK(mastercore::update_tally_map(buyer.getAddr(), buyer.getProperty(),2000, POSSITIVE_BALANCE));


	BOOST_CHECK_EQUAL(0,getMPbalance(seller.getAddr(), seller.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(seller.getAddr(), seller.getProperty(),NEGATIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(buyer.getAddr(), buyer.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(buyer.getAddr(), buyer.getProperty(),NEGATIVE_BALANCE));

	if (direction){
		 BOOST_TEST_MESSAGE("The seller is inserted in priceMap, the buyer in x_Trade");
		 BOOST_CHECK(ContractDex_INSERT(seller));
		 BOOST_CHECK_EQUAL(TRADED, x_Trade(b));
	}else {
		BOOST_TEST_MESSAGE("The buyer is inserted in priceMap, the seller in x_Trade");
		BOOST_CHECK(ContractDex_INSERT(buyer));
		BOOST_CHECK_EQUAL(TRADED, x_Trade(s));
	}

	BOOST_CHECK_EQUAL(0,getMPbalance(seller.getAddr(), seller.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(15,getMPbalance(seller.getAddr(), seller.getProperty(),NEGATIVE_BALANCE));
	BOOST_CHECK_EQUAL(15,getMPbalance(buyer.getAddr(), buyer.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(buyer.getAddr(), buyer.getProperty(),NEGATIVE_BALANCE));




}
BOOST_AUTO_TEST_CASE(moreinbuyer)  // seller_amount = 30, buyer_amount = 60;
{

	CMPTally tally;  // the tally map object
	const uint256 tx;
	CMPContractDex seller(
					"1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", // address
					1,  // block
					3,  // property for sale
					30,  // amount of contracts for sale
					0,  // desired property
					0,
					tx, // txid
					1,  // position in block
					1,  // subaction
					0,  // amount remaining
					5,  // effective_price
					2 // trading_action
	);

	CMPContractDex buyer(
					"1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", // address
					1,  // block
					3,  // property for sale
					60,  // amount of contracts for trade
					0,   // desired property
					0,
					tx, // txid
					2,  // position in block
					1,  // subaction
					0,  // amount remaining
					5,  // effective_price
					1 // trading_action
	);
	CMPContractDex *s;
	s = &seller;
	CMPContractDex *b;
	b = &buyer;

	BOOST_CHECK_EQUAL(3, seller.getProperty());
	BOOST_CHECK_EQUAL("1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", seller.getAddr());
	BOOST_CHECK_EQUAL(30, seller.getAmountForSale());
	BOOST_CHECK_EQUAL(2, seller.getTradingAction());  // seller
	BOOST_CHECK_EQUAL(5, seller.getEffectivePrice());

	BOOST_CHECK_EQUAL(3, buyer.getProperty());
	BOOST_CHECK_EQUAL("1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", buyer.getAddr());
	BOOST_CHECK_EQUAL(60, buyer.getAmountForSale());
	BOOST_CHECK_EQUAL(1, buyer.getTradingAction()); //buyer
	BOOST_CHECK_EQUAL(5, buyer.getEffectivePrice());

	// BOOST_CHECK(mastercore::update_tally_map(seller.getAddr(), seller.getProperty(), 1000,POSSITIVE_BALANCE)); // putting some money here
	// BOOST_CHECK(mastercore::update_tally_map(buyer.getAddr(), buyer.getProperty(),2000, POSSITIVE_BALANCE));


	BOOST_CHECK_EQUAL(0,getMPbalance(seller.getAddr(), seller.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(seller.getAddr(), seller.getProperty(),NEGATIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(buyer.getAddr(), buyer.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(buyer.getAddr(), buyer.getProperty(),NEGATIVE_BALANCE));

	if (direction){
		 BOOST_TEST_MESSAGE("The seller is inserted in priceMap, the buyer in x_Trade");
		 BOOST_CHECK(ContractDex_INSERT(seller));
		 BOOST_CHECK_EQUAL(TRADED, x_Trade(b));
	}else {
		BOOST_TEST_MESSAGE("The buyer is inserted in priceMap, the seller in x_Trade");
		BOOST_CHECK(ContractDex_INSERT(buyer));
		BOOST_CHECK_EQUAL(TRADED, x_Trade(s));
	}

	BOOST_CHECK_EQUAL(0,getMPbalance(seller.getAddr(), seller.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(30,getMPbalance(seller.getAddr(), seller.getProperty(),NEGATIVE_BALANCE));
	BOOST_CHECK_EQUAL(30,getMPbalance(buyer.getAddr(), buyer.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(buyer.getAddr(), buyer.getProperty(),NEGATIVE_BALANCE));



}

BOOST_AUTO_TEST_CASE(equal_seller_long)  // seller_amount = 10, buyer_amount = 10;
{                                         // seller is in long position = 100

	CMPTally tally;  // the tally map object
	const uint256 tx;
	CMPContractDex seller(
					"1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", // address
					1,  // block
					3,  // property for sale
					10,  // amount of contracts for sale
					0,  // desired property
					0,
					tx, // txid
					1,  // position in block
					1,  // subaction
					0,  // amount remaining
					5,  // effective_price
					2 // trading_action
	);

	CMPContractDex buyer(
					"1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", // address
					1,  // block
					3,  // property for sale
					10,  // amount of contracts for trade
					0,   // desired property
					0,
					tx, // txid
					2,  // position in block
					1,  // subaction
					0,  // amount remaining
					5,  // effective_price
					1 // trading_action
	);
	CMPContractDex *s;
	s = &seller;
	CMPContractDex *b;
	b = &buyer;

	BOOST_CHECK_EQUAL(3, seller.getProperty());
	BOOST_CHECK_EQUAL("1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", seller.getAddr());
	BOOST_CHECK_EQUAL(10, seller.getAmountForSale());
	BOOST_CHECK_EQUAL(2, seller.getTradingAction());  // seller
	BOOST_CHECK_EQUAL(5, seller.getEffectivePrice());

	BOOST_CHECK_EQUAL(3, buyer.getProperty());
	BOOST_CHECK_EQUAL("1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", buyer.getAddr());
	BOOST_CHECK_EQUAL(10, buyer.getAmountForSale());
	BOOST_CHECK_EQUAL(1, buyer.getTradingAction()); //buyer
	BOOST_CHECK_EQUAL(5, buyer.getEffectivePrice());

	BOOST_CHECK(mastercore::update_tally_map(seller.getAddr(), seller.getProperty(), 100,POSSITIVE_BALANCE)); // putting some money here
	//BOOST_CHECK(mastercore::update_tally_map(buyer.getAddr(), buyer.getProperty(),2000, POSSITIVE_BALANCE));


	BOOST_CHECK_EQUAL(100,getMPbalance(seller.getAddr(), seller.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(seller.getAddr(), seller.getProperty(),NEGATIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(buyer.getAddr(), buyer.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(buyer.getAddr(), buyer.getProperty(),NEGATIVE_BALANCE));

	if (direction){
		 BOOST_TEST_MESSAGE("The seller is inserted in priceMap, the buyer in x_Trade");
		 BOOST_CHECK(ContractDex_INSERT(seller));
		 BOOST_CHECK_EQUAL(TRADED, x_Trade(b));
	}else {
		BOOST_TEST_MESSAGE("The buyer is inserted in priceMap, the seller in x_Trade");
		BOOST_CHECK(ContractDex_INSERT(buyer));
		BOOST_CHECK_EQUAL(TRADED, x_Trade(s));
	}

	BOOST_CHECK_EQUAL(90,getMPbalance(seller.getAddr(), seller.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(seller.getAddr(), seller.getProperty(),NEGATIVE_BALANCE));
	BOOST_CHECK_EQUAL(10,getMPbalance(buyer.getAddr(), buyer.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(buyer.getAddr(), buyer.getProperty(),NEGATIVE_BALANCE));


}
BOOST_AUTO_TEST_CASE(equal_seller_short)  // seller_amount = 10, buyer_amount = 10;
{                                         // seller is in short position = 200

	CMPTally tally;  // the tally map object
	const uint256 tx;
	CMPContractDex seller(
					"1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", // address
					1,  // block
					3,  // property for sale
					10,  // amount of contracts for sale
					0,  // desired property
					0,
					tx, // txid
					1,  // position in block
					1,  // subaction
					0,  // amount remaining
					5,  // effective_price
					2 // trading_action
	);

	CMPContractDex buyer(
					"1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", // address
					1,  // block
					3,  // property for sale
					10,  // amount of contracts for trade
					0,   // desired property
					0,
					tx, // txid
					2,  // position in block
					1,  // subaction
					0,  // amount remaining
					5,  // effective_price
					1 // trading_action
	);
	CMPContractDex *s;
	s = &seller;
	CMPContractDex *b;
	b = &buyer;

	BOOST_CHECK_EQUAL(3, seller.getProperty());
	BOOST_CHECK_EQUAL("1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", seller.getAddr());
	BOOST_CHECK_EQUAL(10, seller.getAmountForSale());
	BOOST_CHECK_EQUAL(2, seller.getTradingAction());  // seller
	BOOST_CHECK_EQUAL(5, seller.getEffectivePrice());

	BOOST_CHECK_EQUAL(3, buyer.getProperty());
	BOOST_CHECK_EQUAL("1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", buyer.getAddr());
	BOOST_CHECK_EQUAL(10, buyer.getAmountForSale());
	BOOST_CHECK_EQUAL(1, buyer.getTradingAction()); //buyer
	BOOST_CHECK_EQUAL(5, buyer.getEffectivePrice());

	BOOST_CHECK(mastercore::update_tally_map(seller.getAddr(), seller.getProperty(), 200,NEGATIVE_BALANCE)); // putting some money here
	//BOOST_CHECK(mastercore::update_tally_map(buyer.getAddr(), buyer.getProperty(),2000, POSSITIVE_BALANCE));


	BOOST_CHECK_EQUAL(0,getMPbalance(seller.getAddr(), seller.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(200,getMPbalance(seller.getAddr(), seller.getProperty(),NEGATIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(buyer.getAddr(), buyer.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(buyer.getAddr(), buyer.getProperty(),NEGATIVE_BALANCE));

	if (direction){
		 BOOST_TEST_MESSAGE("The seller is inserted in priceMap, the buyer in x_Trade");
		 BOOST_CHECK(ContractDex_INSERT(seller));
		 BOOST_CHECK_EQUAL(TRADED, x_Trade(b));
	}else {
		BOOST_TEST_MESSAGE("The buyer is inserted in priceMap, the seller in x_Trade");
		BOOST_CHECK(ContractDex_INSERT(buyer));
		BOOST_CHECK_EQUAL(TRADED, x_Trade(s));
	}


	BOOST_CHECK_EQUAL(0,getMPbalance(seller.getAddr(), seller.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(210,getMPbalance(seller.getAddr(), seller.getProperty(),NEGATIVE_BALANCE));
	BOOST_CHECK_EQUAL(10,getMPbalance(buyer.getAddr(), buyer.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(buyer.getAddr(), buyer.getProperty(),NEGATIVE_BALANCE));


}
BOOST_AUTO_TEST_CASE(equal_buyer_long)  // seller_amount = 10, buyer_amount = 10;
{                                         // buyer is in long position = 360

	CMPTally tally;  // the tally map object
	const uint256 tx;
	CMPContractDex seller(
					"1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", // address
					1,  // block
					3,  // property for sale
					10,  // amount of contracts for sale
					0,  // desired property
					0,
					tx, // txid
					1,  // position in block
					1,  // subaction
					0,  // amount remaining
					5,  // effective_price
					2 // trading_action
	);

	CMPContractDex buyer(
					"1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", // address
					1,  // block
					3,  // property for sale
					10,  // amount of contracts for trade
					0,   // desired property
					0,
					tx, // txid
					2,  // position in block
					1,  // subaction
					0,  // amount remaining
					5,  // effective_price
					1 // trading_action
	);
	CMPContractDex *s;
	s = &seller;
	CMPContractDex *b;
	b = &buyer;

	BOOST_CHECK_EQUAL(3, seller.getProperty());
	BOOST_CHECK_EQUAL("1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", seller.getAddr());
	BOOST_CHECK_EQUAL(10, seller.getAmountForSale());
	BOOST_CHECK_EQUAL(2, seller.getTradingAction());  // seller
	BOOST_CHECK_EQUAL(5, seller.getEffectivePrice());

	BOOST_CHECK_EQUAL(3, buyer.getProperty());
	BOOST_CHECK_EQUAL("1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", buyer.getAddr());
	BOOST_CHECK_EQUAL(10, buyer.getAmountForSale());
	BOOST_CHECK_EQUAL(1, buyer.getTradingAction()); //buyer
	BOOST_CHECK_EQUAL(5, buyer.getEffectivePrice());

	//BOOST_CHECK(mastercore::update_tally_map(seller.getAddr(), seller.getProperty(), ,NEGATIVE_BALANCE)); // putting some money here
	BOOST_CHECK(mastercore::update_tally_map(buyer.getAddr(), buyer.getProperty(),360, POSSITIVE_BALANCE));


	BOOST_CHECK_EQUAL(0,getMPbalance(seller.getAddr(), seller.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(seller.getAddr(), seller.getProperty(),NEGATIVE_BALANCE));
	BOOST_CHECK_EQUAL(360,getMPbalance(buyer.getAddr(), buyer.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(buyer.getAddr(), buyer.getProperty(),NEGATIVE_BALANCE));

	if (direction){
		 BOOST_TEST_MESSAGE("The seller is inserted in priceMap, the buyer in x_Trade");
		 BOOST_CHECK(ContractDex_INSERT(seller));
		 BOOST_CHECK_EQUAL(TRADED, x_Trade(b));
	}else {
		BOOST_TEST_MESSAGE("The buyer is inserted in priceMap, the seller in x_Trade");
		BOOST_CHECK(ContractDex_INSERT(buyer));
		BOOST_CHECK_EQUAL(TRADED, x_Trade(s));
	}


	BOOST_CHECK_EQUAL(0,getMPbalance(seller.getAddr(), seller.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(10,getMPbalance(seller.getAddr(), seller.getProperty(),NEGATIVE_BALANCE));
	BOOST_CHECK_EQUAL(370,getMPbalance(buyer.getAddr(), buyer.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(buyer.getAddr(), buyer.getProperty(),NEGATIVE_BALANCE));


}
BOOST_AUTO_TEST_CASE(equal_buyer_short)  // seller_amount = 10, buyer_amount = 10;
{                                         // buyer is in short position = 500

	CMPTally tally;  // the tally map object
	const uint256 tx;
	CMPContractDex seller(
					"1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", // address
					1,  // block
					3,  // property for sale
					10,  // amount of contracts for sale
					0,  // desired property
					0,
					tx, // txid
					1,  // position in block
					1,  // subaction
					0,  // amount remaining
					5,  // effective_price
					2 // trading_action
	);

	CMPContractDex buyer(
					"1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", // address
					1,  // block
					3,  // property for sale
					10,  // amount of contracts for trade
					0,   // desired property
					0,
					tx, // txid
					2,  // position in block
					1,  // subaction
					0,  // amount remaining
					5,  // effective_price
					1 // trading_action
	);
	CMPContractDex *s;
	s = &seller;
	CMPContractDex *b;
	b = &buyer;

	BOOST_CHECK_EQUAL(3, seller.getProperty());
	BOOST_CHECK_EQUAL("1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", seller.getAddr());
	BOOST_CHECK_EQUAL(10, seller.getAmountForSale());
	BOOST_CHECK_EQUAL(2, seller.getTradingAction());  // seller
	BOOST_CHECK_EQUAL(5, seller.getEffectivePrice());

	BOOST_CHECK_EQUAL(3, buyer.getProperty());
	BOOST_CHECK_EQUAL("1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", buyer.getAddr());
	BOOST_CHECK_EQUAL(10, buyer.getAmountForSale());
	BOOST_CHECK_EQUAL(1, buyer.getTradingAction()); //buyer
	BOOST_CHECK_EQUAL(5, buyer.getEffectivePrice());

	//BOOST_CHECK(mastercore::update_tally_map(seller.getAddr(), seller.getProperty(), ,NEGATIVE_BALANCE)); // putting some money here
	BOOST_CHECK(mastercore::update_tally_map(buyer.getAddr(), buyer.getProperty(),500, NEGATIVE_BALANCE));


	BOOST_CHECK_EQUAL(0,getMPbalance(seller.getAddr(), seller.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(seller.getAddr(), seller.getProperty(),NEGATIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(buyer.getAddr(), buyer.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(500,getMPbalance(buyer.getAddr(), buyer.getProperty(),NEGATIVE_BALANCE));

	if (direction){
		 BOOST_TEST_MESSAGE("The seller is inserted in priceMap, the buyer in x_Trade");
		 BOOST_CHECK(ContractDex_INSERT(seller));
		 BOOST_CHECK_EQUAL(TRADED, x_Trade(b));
	}else {
		BOOST_TEST_MESSAGE("The buyer is inserted in priceMap, the seller in x_Trade");
		BOOST_CHECK(ContractDex_INSERT(buyer));
		BOOST_CHECK_EQUAL(TRADED, x_Trade(s));
	}


	BOOST_CHECK_EQUAL(0,getMPbalance(seller.getAddr(), seller.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(10,getMPbalance(seller.getAddr(), seller.getProperty(),NEGATIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(buyer.getAddr(), buyer.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(490,getMPbalance(buyer.getAddr(), buyer.getProperty(),NEGATIVE_BALANCE));


}
BOOST_AUTO_TEST_CASE(equal_bl_sl)  // seller_amount = 10, buyer_amount = 10;
{                                         // seller is in long position = 80
                                          // buyer is in long position = 60
	CMPTally tally;  // the tally map object
	const uint256 tx;
	CMPContractDex seller(
					"1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", // address
					1,  // block
					3,  // property for sale
					10,  // amount of contracts for sale
					0,  // desired property
					0,
					tx, // txid
					1,  // position in block
					1,  // subaction
					0,  // amount remaining
					5,  // effective_price
					2 // trading_action
	);

	CMPContractDex buyer(
					"1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", // address
					1,  // block
					3,  // property for sale
					10,  // amount of contracts for trade
					0,   // desired property
					0,
					tx, // txid
					2,  // position in block
					1,  // subaction
					0,  // amount remaining
					5,  // effective_price
					1 // trading_action
	);
	CMPContractDex *s;
	s = &seller;
	CMPContractDex *b;
	b = &buyer;

	BOOST_CHECK_EQUAL(3, seller.getProperty());
	BOOST_CHECK_EQUAL("1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", seller.getAddr());
	BOOST_CHECK_EQUAL(10, seller.getAmountForSale());
	BOOST_CHECK_EQUAL(2, seller.getTradingAction());  // seller
	BOOST_CHECK_EQUAL(5, seller.getEffectivePrice());

	BOOST_CHECK_EQUAL(3, buyer.getProperty());
	BOOST_CHECK_EQUAL("1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", buyer.getAddr());
	BOOST_CHECK_EQUAL(10, buyer.getAmountForSale());
	BOOST_CHECK_EQUAL(1, buyer.getTradingAction()); //buyer
	BOOST_CHECK_EQUAL(5, buyer.getEffectivePrice());

	BOOST_CHECK(mastercore::update_tally_map(seller.getAddr(), seller.getProperty(),80 ,POSSITIVE_BALANCE)); // putting some money here
	BOOST_CHECK(mastercore::update_tally_map(buyer.getAddr(), buyer.getProperty(),60, POSSITIVE_BALANCE));


	BOOST_CHECK_EQUAL(80,getMPbalance(seller.getAddr(), seller.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(seller.getAddr(), seller.getProperty(),NEGATIVE_BALANCE));
	BOOST_CHECK_EQUAL(60,getMPbalance(buyer.getAddr(), buyer.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(buyer.getAddr(), buyer.getProperty(),NEGATIVE_BALANCE));

	if (direction){
		 BOOST_TEST_MESSAGE("The seller is inserted in priceMap, the buyer in x_Trade");
		 BOOST_CHECK(ContractDex_INSERT(seller));
		 BOOST_CHECK_EQUAL(TRADED, x_Trade(b));
	}else {
		BOOST_TEST_MESSAGE("The buyer is inserted in priceMap, the seller in x_Trade");
		BOOST_CHECK(ContractDex_INSERT(buyer));
		BOOST_CHECK_EQUAL(TRADED, x_Trade(s));
	}

	BOOST_CHECK_EQUAL(70,getMPbalance(seller.getAddr(), seller.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(seller.getAddr(), seller.getProperty(),NEGATIVE_BALANCE));
	BOOST_CHECK_EQUAL(70,getMPbalance(buyer.getAddr(), buyer.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(buyer.getAddr(), buyer.getProperty(),NEGATIVE_BALANCE));


}
BOOST_AUTO_TEST_CASE(equal_bs_ss)  // seller_amount = 10, buyer_amount = 10;
{                                         // seller is in short position = 90
                                          // buyer is in short position = 100
	CMPTally tally;  // the tally map object
	const uint256 tx;
	CMPContractDex seller(
					"1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", // address
					1,  // block
					3,  // property for sale
					10,  // amount of contracts for sale
					0,  // desired property
					0,
					tx, // txid
					1,  // position in block
					1,  // subaction
					0,  // amount remaining
					5,  // effective_price
					2 // trading_action
	);

	CMPContractDex buyer(
					"1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", // address
					1,  // block
					3,  // property for sale
					10,  // amount of contracts for trade
					0,   // desired property
					0,
					tx, // txid
					2,  // position in block
					1,  // subaction
					0,  // amount remaining
					5,  // effective_price
					1 // trading_action
	);
	CMPContractDex *s;
	s = &seller;
	CMPContractDex *b;
	b = &buyer;

	BOOST_CHECK_EQUAL(3, seller.getProperty());
	BOOST_CHECK_EQUAL("1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", seller.getAddr());
	BOOST_CHECK_EQUAL(10, seller.getAmountForSale());
	BOOST_CHECK_EQUAL(2, seller.getTradingAction());  // seller
	BOOST_CHECK_EQUAL(5, seller.getEffectivePrice());

	BOOST_CHECK_EQUAL(3, buyer.getProperty());
	BOOST_CHECK_EQUAL("1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", buyer.getAddr());
	BOOST_CHECK_EQUAL(10, buyer.getAmountForSale());
	BOOST_CHECK_EQUAL(1, buyer.getTradingAction()); //buyer
	BOOST_CHECK_EQUAL(5, buyer.getEffectivePrice());

	BOOST_CHECK(mastercore::update_tally_map(seller.getAddr(), seller.getProperty(),90 ,NEGATIVE_BALANCE)); // putting some money here
	BOOST_CHECK(mastercore::update_tally_map(buyer.getAddr(), buyer.getProperty(),100, NEGATIVE_BALANCE));


	BOOST_CHECK_EQUAL(0,getMPbalance(seller.getAddr(), seller.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(90,getMPbalance(seller.getAddr(), seller.getProperty(),NEGATIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(buyer.getAddr(), buyer.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(100,getMPbalance(buyer.getAddr(), buyer.getProperty(),NEGATIVE_BALANCE));

	if (direction){
		 BOOST_TEST_MESSAGE("The seller is inserted in priceMap, the buyer in x_Trade");
		 BOOST_CHECK(ContractDex_INSERT(seller));
		 BOOST_CHECK_EQUAL(TRADED, x_Trade(b));
	}else {
		BOOST_TEST_MESSAGE("The buyer is inserted in priceMap, the seller in x_Trade");
		BOOST_CHECK(ContractDex_INSERT(buyer));
		BOOST_CHECK_EQUAL(TRADED, x_Trade(s));
	}

	BOOST_CHECK_EQUAL(0,getMPbalance(seller.getAddr(), seller.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(100,getMPbalance(seller.getAddr(), seller.getProperty(),NEGATIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(buyer.getAddr(), buyer.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(90,getMPbalance(buyer.getAddr(), buyer.getProperty(),NEGATIVE_BALANCE));


}

BOOST_AUTO_TEST_CASE(equal_bl_ss)  // seller_amount = 10, buyer_amount = 10;
{                                         // seller is in short position = 90
                                          // buyer is in long position = 400
	CMPTally tally;  // the tally map object
	const uint256 tx;
	CMPContractDex seller(
					"1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", // address
					1,  // block
					3,  // property for sale
					10,  // amount of contracts for sale
					0,  // desired property
					0,
					tx, // txid
					1,  // position in block
					1,  // subaction
					0,  // amount remaining
					5,  // effective_price
					2 // trading_action
	);

	CMPContractDex buyer(
					"1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", // address
					1,  // block
					3,  // property for sale
					10,  // amount of contracts for trade
					0,   // desired property
					0,
					tx, // txid
					2,  // position in block
					1,  // subaction
					0,  // amount remaining
					5,  // effective_price
					1 // trading_action
	);
	CMPContractDex *s;
	s = &seller;
	CMPContractDex *b;
	b = &buyer;

	BOOST_CHECK_EQUAL(3, seller.getProperty());
	BOOST_CHECK_EQUAL("1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", seller.getAddr());
	BOOST_CHECK_EQUAL(10, seller.getAmountForSale());
	BOOST_CHECK_EQUAL(2, seller.getTradingAction());  // seller
	BOOST_CHECK_EQUAL(5, seller.getEffectivePrice());

	BOOST_CHECK_EQUAL(3, buyer.getProperty());
	BOOST_CHECK_EQUAL("1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", buyer.getAddr());
	BOOST_CHECK_EQUAL(10, buyer.getAmountForSale());
	BOOST_CHECK_EQUAL(1, buyer.getTradingAction()); //buyer
	BOOST_CHECK_EQUAL(5, buyer.getEffectivePrice());

	BOOST_CHECK(mastercore::update_tally_map(seller.getAddr(), seller.getProperty(),90 ,NEGATIVE_BALANCE)); // putting some money here
	BOOST_CHECK(mastercore::update_tally_map(buyer.getAddr(), buyer.getProperty(),400, POSSITIVE_BALANCE));


	BOOST_CHECK_EQUAL(0,getMPbalance(seller.getAddr(), seller.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(90,getMPbalance(seller.getAddr(), seller.getProperty(),NEGATIVE_BALANCE));
	BOOST_CHECK_EQUAL(400,getMPbalance(buyer.getAddr(), buyer.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(buyer.getAddr(), buyer.getProperty(),NEGATIVE_BALANCE));

	if (direction){
		 BOOST_TEST_MESSAGE("The seller is inserted in priceMap, the buyer in x_Trade");
		 BOOST_CHECK(ContractDex_INSERT(seller));
		 BOOST_CHECK_EQUAL(TRADED, x_Trade(b));
	}else {
		BOOST_TEST_MESSAGE("The buyer is inserted in priceMap, the seller in x_Trade");
		BOOST_CHECK(ContractDex_INSERT(buyer));
		BOOST_CHECK_EQUAL(TRADED, x_Trade(s));
	}

	BOOST_CHECK_EQUAL(0,getMPbalance(seller.getAddr(), seller.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(100,getMPbalance(seller.getAddr(), seller.getProperty(),NEGATIVE_BALANCE));
	BOOST_CHECK_EQUAL(410,getMPbalance(buyer.getAddr(), buyer.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(buyer.getAddr(), buyer.getProperty(),NEGATIVE_BALANCE));


}

BOOST_AUTO_TEST_CASE(equal_bs_sl)  // seller_amount = 10, buyer_amount = 10;
{                                         // seller is in long position = 700
                                          // buyer is in short position = 800
	CMPTally tally;  // the tally map object
	const uint256 tx;
	CMPContractDex seller(
					"1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", // address
					1,  // block
					3,  // property for sale
					10,  // amount of contracts for sale
					0,  // desired property
					0,
					tx, // txid
					1,  // position in block
					1,  // subaction
					0,  // amount remaining
					5,  // effective_price
					2 // trading_action
	);

	CMPContractDex buyer(
					"1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", // address
					1,  // block
					3,  // property for sale
					10,  // amount of contracts for trade
					0,   // desired property
					0,
					tx, // txid
					2,  // position in block
					1,  // subaction
					0,  // amount remaining
					5,  // effective_price
					1 // trading_action
	);
	CMPContractDex *s;
	s = &seller;
	CMPContractDex *b;
	b = &buyer;

	BOOST_CHECK_EQUAL(3, seller.getProperty());
	BOOST_CHECK_EQUAL("1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", seller.getAddr());
	BOOST_CHECK_EQUAL(10, seller.getAmountForSale());
	BOOST_CHECK_EQUAL(2, seller.getTradingAction());  // seller
	BOOST_CHECK_EQUAL(5, seller.getEffectivePrice());

	BOOST_CHECK_EQUAL(3, buyer.getProperty());
	BOOST_CHECK_EQUAL("1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", buyer.getAddr());
	BOOST_CHECK_EQUAL(10, buyer.getAmountForSale());
	BOOST_CHECK_EQUAL(1, buyer.getTradingAction()); //buyer
	BOOST_CHECK_EQUAL(5, buyer.getEffectivePrice());

	BOOST_CHECK(mastercore::update_tally_map(seller.getAddr(), seller.getProperty(),700 ,POSSITIVE_BALANCE)); // putting some money here
	BOOST_CHECK(mastercore::update_tally_map(buyer.getAddr(), buyer.getProperty(),800, NEGATIVE_BALANCE));


	BOOST_CHECK_EQUAL(0,getMPbalance(seller.getAddr(), seller.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(90,getMPbalance(seller.getAddr(), seller.getProperty(),NEGATIVE_BALANCE));
	BOOST_CHECK_EQUAL(400,getMPbalance(buyer.getAddr(), buyer.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(buyer.getAddr(), buyer.getProperty(),NEGATIVE_BALANCE));

	if (direction){
		 BOOST_TEST_MESSAGE("The seller is inserted in priceMap, the buyer in x_Trade");
		 BOOST_CHECK(ContractDex_INSERT(seller));
		 BOOST_CHECK_EQUAL(TRADED, x_Trade(b));
	}else {
		BOOST_TEST_MESSAGE("The buyer is inserted in priceMap, the seller in x_Trade");
		BOOST_CHECK(ContractDex_INSERT(buyer));
		BOOST_CHECK_EQUAL(TRADED, x_Trade(s));
	}

	BOOST_CHECK_EQUAL(0,getMPbalance(seller.getAddr(), seller.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(100,getMPbalance(seller.getAddr(), seller.getProperty(),NEGATIVE_BALANCE));
	BOOST_CHECK_EQUAL(410,getMPbalance(buyer.getAddr(), buyer.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(buyer.getAddr(), buyer.getProperty(),NEGATIVE_BALANCE));


}
BOOST_AUTO_TEST_CASE(moreinseller_bl_sl)  // seller_amount = 20, buyer_amount = 10;
{                                         // seller is in long position = 80
                                          // buyer is in long position = 60
	CMPTally tally;  // the tally map object
	const uint256 tx;
	CMPContractDex seller(
					"1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", // address
					1,  // block
					3,  // property for sale
					20,  // amount of contracts for sale
					0,  // desired property
					0,
					tx, // txid
					1,  // position in block
					1,  // subaction
					0,  // amount remaining
					5,  // effective_price
					2 // trading_action
	);

	CMPContractDex buyer(
					"1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", // address
					1,  // block
					3,  // property for sale
					10,  // amount of contracts for trade
					0,   // desired property
					0,
					tx, // txid
					2,  // position in block
					1,  // subaction
					0,  // amount remaining
					5,  // effective_price
					1 // trading_action
	);
	CMPContractDex *s;
	s = &seller;
	CMPContractDex *b;
	b = &buyer;

	BOOST_CHECK_EQUAL(3, seller.getProperty());
	BOOST_CHECK_EQUAL("1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", seller.getAddr());
	BOOST_CHECK_EQUAL(20, seller.getAmountForSale());
	BOOST_CHECK_EQUAL(2, seller.getTradingAction());  // seller
	BOOST_CHECK_EQUAL(5, seller.getEffectivePrice());

	BOOST_CHECK_EQUAL(3, buyer.getProperty());
	BOOST_CHECK_EQUAL("1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", buyer.getAddr());
	BOOST_CHECK_EQUAL(10, buyer.getAmountForSale());
	BOOST_CHECK_EQUAL(1, buyer.getTradingAction()); //buyer
	BOOST_CHECK_EQUAL(5, buyer.getEffectivePrice());

	BOOST_CHECK(mastercore::update_tally_map(seller.getAddr(), seller.getProperty(),80 ,POSSITIVE_BALANCE)); // putting some money here
	BOOST_CHECK(mastercore::update_tally_map(buyer.getAddr(), buyer.getProperty(),60, POSSITIVE_BALANCE));


	BOOST_CHECK_EQUAL(80,getMPbalance(seller.getAddr(), seller.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(seller.getAddr(), seller.getProperty(),NEGATIVE_BALANCE));
	BOOST_CHECK_EQUAL(60,getMPbalance(buyer.getAddr(), buyer.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(buyer.getAddr(), buyer.getProperty(),NEGATIVE_BALANCE));

	if (direction){
		 BOOST_TEST_MESSAGE("The seller is inserted in priceMap, the buyer in x_Trade");
		 BOOST_CHECK(ContractDex_INSERT(seller));
		 BOOST_CHECK_EQUAL(TRADED, x_Trade(b));
	}else {
		BOOST_TEST_MESSAGE("The buyer is inserted in priceMap, the seller in x_Trade");
		BOOST_CHECK(ContractDex_INSERT(buyer));
		BOOST_CHECK_EQUAL(TRADED, x_Trade(s));
	}

	BOOST_CHECK_EQUAL(70,getMPbalance(seller.getAddr(), seller.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(seller.getAddr(), seller.getProperty(),NEGATIVE_BALANCE));
	BOOST_CHECK_EQUAL(70,getMPbalance(buyer.getAddr(), buyer.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(buyer.getAddr(), buyer.getProperty(),NEGATIVE_BALANCE));


}
BOOST_AUTO_TEST_CASE(moreinseller_bs_ss)  // seller_amount = 23, buyer_amount = 10;
{                                         // seller is in short position = 90
                                          // buyer is in short position = 100
	CMPTally tally;  // the tally map object
	const uint256 tx;
	CMPContractDex seller(
					"1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", // address
					1,  // block
					3,  // property for sale
					23,  // amount of contracts for sale
					0,  // desired property
					0,
					tx, // txid
					1,  // position in block
					1,  // subaction
					0,  // amount remaining
					5,  // effective_price
					2 // trading_action
	);

	CMPContractDex buyer(
					"1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", // address
					1,  // block
					3,  // property for sale
					10,  // amount of contracts for trade
					0,   // desired property
					0,
					tx, // txid
					2,  // position in block
					1,  // subaction
					0,  // amount remaining
					5,  // effective_price
					1 // trading_action
	);
	CMPContractDex *s;
	s = &seller;
	CMPContractDex *b;
	b = &buyer;

	BOOST_CHECK_EQUAL(3, seller.getProperty());
	BOOST_CHECK_EQUAL("1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", seller.getAddr());
	BOOST_CHECK_EQUAL(23, seller.getAmountForSale());
	BOOST_CHECK_EQUAL(2, seller.getTradingAction());  // seller
	BOOST_CHECK_EQUAL(5, seller.getEffectivePrice());

	BOOST_CHECK_EQUAL(3, buyer.getProperty());
	BOOST_CHECK_EQUAL("1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", buyer.getAddr());
	BOOST_CHECK_EQUAL(10, buyer.getAmountForSale());
	BOOST_CHECK_EQUAL(1, buyer.getTradingAction()); //buyer
	BOOST_CHECK_EQUAL(5, buyer.getEffectivePrice());

	BOOST_CHECK(mastercore::update_tally_map(seller.getAddr(), seller.getProperty(),90 ,NEGATIVE_BALANCE)); // putting some money here
	BOOST_CHECK(mastercore::update_tally_map(buyer.getAddr(), buyer.getProperty(),100, NEGATIVE_BALANCE));


	BOOST_CHECK_EQUAL(0,getMPbalance(seller.getAddr(), seller.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(90,getMPbalance(seller.getAddr(), seller.getProperty(),NEGATIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(buyer.getAddr(), buyer.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(100,getMPbalance(buyer.getAddr(), buyer.getProperty(),NEGATIVE_BALANCE));

	if (direction){
		 BOOST_TEST_MESSAGE("The seller is inserted in priceMap, the buyer in x_Trade");
		 BOOST_CHECK(ContractDex_INSERT(seller));
		 BOOST_CHECK_EQUAL(TRADED, x_Trade(b));
	}else {
		BOOST_TEST_MESSAGE("The buyer is inserted in priceMap, the seller in x_Trade");
		BOOST_CHECK(ContractDex_INSERT(buyer));
		BOOST_CHECK_EQUAL(TRADED, x_Trade(s));
	}

	BOOST_CHECK_EQUAL(0,getMPbalance(seller.getAddr(), seller.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(100,getMPbalance(seller.getAddr(), seller.getProperty(),NEGATIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(buyer.getAddr(), buyer.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(90,getMPbalance(buyer.getAddr(), buyer.getProperty(),NEGATIVE_BALANCE));


}

BOOST_AUTO_TEST_CASE(moreinseller_bl_ss)  // seller_amount = 25, buyer_amount = 10;
{                                         // seller is in short position = 90
                                          // buyer is in long position = 400
	CMPTally tally;  // the tally map object
	const uint256 tx;
	CMPContractDex seller(
					"1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", // address
					1,  // block
					3,  // property for sale
					25,  // amount of contracts for sale
					0,  // desired property
					0,
					tx, // txid
					1,  // position in block
					1,  // subaction
					0,  // amount remaining
					5,  // effective_price
					2 // trading_action
	);

	CMPContractDex buyer(
					"1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", // address
					1,  // block
					3,  // property for sale
					10,  // amount of contracts for trade
					0,   // desired property
					0,
					tx, // txid
					2,  // position in block
					1,  // subaction
					0,  // amount remaining
					5,  // effective_price
					1 // trading_action
	);
	CMPContractDex *s;
	s = &seller;
	CMPContractDex *b;
	b = &buyer;

	BOOST_CHECK_EQUAL(3, seller.getProperty());
	BOOST_CHECK_EQUAL("1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", seller.getAddr());
	BOOST_CHECK_EQUAL(25, seller.getAmountForSale());
	BOOST_CHECK_EQUAL(2, seller.getTradingAction());  // seller
	BOOST_CHECK_EQUAL(5, seller.getEffectivePrice());

	BOOST_CHECK_EQUAL(3, buyer.getProperty());
	BOOST_CHECK_EQUAL("1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", buyer.getAddr());
	BOOST_CHECK_EQUAL(10, buyer.getAmountForSale());
	BOOST_CHECK_EQUAL(1, buyer.getTradingAction()); //buyer
	BOOST_CHECK_EQUAL(5, buyer.getEffectivePrice());

	BOOST_CHECK(mastercore::update_tally_map(seller.getAddr(), seller.getProperty(),90 ,NEGATIVE_BALANCE)); // putting some money here
	BOOST_CHECK(mastercore::update_tally_map(buyer.getAddr(), buyer.getProperty(),400, POSSITIVE_BALANCE));


	BOOST_CHECK_EQUAL(0,getMPbalance(seller.getAddr(), seller.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(90,getMPbalance(seller.getAddr(), seller.getProperty(),NEGATIVE_BALANCE));
	BOOST_CHECK_EQUAL(400,getMPbalance(buyer.getAddr(), buyer.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(buyer.getAddr(), buyer.getProperty(),NEGATIVE_BALANCE));

	if (direction){
		 BOOST_TEST_MESSAGE("The seller is inserted in priceMap, the buyer in x_Trade");
		 BOOST_CHECK(ContractDex_INSERT(seller));
		 BOOST_CHECK_EQUAL(TRADED, x_Trade(b));
	}else {
		BOOST_TEST_MESSAGE("The buyer is inserted in priceMap, the seller in x_Trade");
		BOOST_CHECK(ContractDex_INSERT(buyer));
		BOOST_CHECK_EQUAL(TRADED, x_Trade(s));
	}

	BOOST_CHECK_EQUAL(0,getMPbalance(seller.getAddr(), seller.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(100,getMPbalance(seller.getAddr(), seller.getProperty(),NEGATIVE_BALANCE));
	BOOST_CHECK_EQUAL(410,getMPbalance(buyer.getAddr(), buyer.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(buyer.getAddr(), buyer.getProperty(),NEGATIVE_BALANCE));


}

BOOST_AUTO_TEST_CASE(moreinseller_bs_sl)  // seller_amount = 25, buyer_amount = 10;
{                                         // seller is in long position = 700
                                          // buyer is in short position = 800
	CMPTally tally;  // the tally map object
	const uint256 tx;
	CMPContractDex seller(
					"1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", // address
					1,  // block
					3,  // property for sale
					25,  // amount of contracts for sale
					0,  // desired property
					0,
					tx, // txid
					1,  // position in block
					1,  // subaction
					0,  // amount remaining
					5,  // effective_price
					2 // trading_action
	);

	CMPContractDex buyer(
					"1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", // address
					1,  // block
					3,  // property for sale
					10,  // amount of contracts for trade
					0,   // desired property
					0,
					tx, // txid
					2,  // position in block
					1,  // subaction
					0,  // amount remaining
					5,  // effective_price
					1 // trading_action
	);
	CMPContractDex *s;
	s = &seller;
	CMPContractDex *b;
	b = &buyer;

	BOOST_CHECK_EQUAL(3, seller.getProperty());
	BOOST_CHECK_EQUAL("1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", seller.getAddr());
	BOOST_CHECK_EQUAL(25, seller.getAmountForSale());
	BOOST_CHECK_EQUAL(2, seller.getTradingAction());  // seller
	BOOST_CHECK_EQUAL(5, seller.getEffectivePrice());

	BOOST_CHECK_EQUAL(3, buyer.getProperty());
	BOOST_CHECK_EQUAL("1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", buyer.getAddr());
	BOOST_CHECK_EQUAL(10, buyer.getAmountForSale());
	BOOST_CHECK_EQUAL(1, buyer.getTradingAction()); //buyer
	BOOST_CHECK_EQUAL(5, buyer.getEffectivePrice());

	BOOST_CHECK(mastercore::update_tally_map(seller.getAddr(), seller.getProperty(),700 ,POSSITIVE_BALANCE)); // putting some money here
	BOOST_CHECK(mastercore::update_tally_map(buyer.getAddr(), buyer.getProperty(),800, NEGATIVE_BALANCE));


	BOOST_CHECK_EQUAL(700,getMPbalance(seller.getAddr(), seller.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(seller.getAddr(), seller.getProperty(),NEGATIVE_BALANCE));
	BOOST_CHECK_EQUAL(800,getMPbalance(buyer.getAddr(), buyer.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(buyer.getAddr(), buyer.getProperty(),NEGATIVE_BALANCE));

	if (direction){
		 BOOST_TEST_MESSAGE("The seller is inserted in priceMap, the buyer in x_Trade");
		 BOOST_CHECK(ContractDex_INSERT(seller));
		 BOOST_CHECK_EQUAL(TRADED, x_Trade(b));
	}else {
		BOOST_TEST_MESSAGE("The buyer is inserted in priceMap, the seller in x_Trade");
		BOOST_CHECK(ContractDex_INSERT(buyer));
		BOOST_CHECK_EQUAL(TRADED, x_Trade(s));
	}

	BOOST_CHECK_EQUAL(690,getMPbalance(seller.getAddr(), seller.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(seller.getAddr(), seller.getProperty(),NEGATIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(buyer.getAddr(), buyer.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(790,getMPbalance(buyer.getAddr(), buyer.getProperty(),NEGATIVE_BALANCE));


}
BOOST_AUTO_TEST_CASE(moreinbuyer_bs_sl)  // seller_amount = 20, buyer_amount = 30;
{                                         // seller is in long position = 19
                                          // buyer is in short position = 20
	CMPTally tally;  // the tally map object
	const uint256 tx;
	CMPContractDex seller(
					"1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", // address
					1,  // block
					3,  // property for sale
					20,  // amount of contracts for sale
					0,  // desired property
					0,
					tx, // txid
					1,  // position in block
					1,  // subaction
					0,  // amount remaining
					5,  // effective_price
					2 // trading_action
	);

	CMPContractDex buyer(
					"1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", // address
					1,  // block
					3,  // property for sale
					30,  // amount of contracts for trade
					0,   // desired property
					0,
					tx, // txid
					2,  // position in block
					1,  // subaction
					0,  // amount remaining
					5,  // effective_price
					1 // trading_action
	);
	CMPContractDex *s;
	s = &seller;
	CMPContractDex *b;
	b = &buyer;

	BOOST_CHECK_EQUAL(3, seller.getProperty());
	BOOST_CHECK_EQUAL("1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", seller.getAddr());
	BOOST_CHECK_EQUAL(20, seller.getAmountForSale());
	BOOST_CHECK_EQUAL(2, seller.getTradingAction());  // seller
	BOOST_CHECK_EQUAL(5, seller.getEffectivePrice());

	BOOST_CHECK_EQUAL(3, buyer.getProperty());
	BOOST_CHECK_EQUAL("1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", buyer.getAddr());
	BOOST_CHECK_EQUAL(30, buyer.getAmountForSale());
	BOOST_CHECK_EQUAL(1, buyer.getTradingAction()); //buyer
	BOOST_CHECK_EQUAL(5, buyer.getEffectivePrice());

	BOOST_CHECK(mastercore::update_tally_map(seller.getAddr(), seller.getProperty(),19 ,POSSITIVE_BALANCE)); // putting some money here
	BOOST_CHECK(mastercore::update_tally_map(buyer.getAddr(), buyer.getProperty(),20, NEGATIVE_BALANCE));


	BOOST_CHECK_EQUAL(19,getMPbalance(seller.getAddr(), seller.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(seller.getAddr(), seller.getProperty(),NEGATIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(buyer.getAddr(), buyer.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(20,getMPbalance(buyer.getAddr(), buyer.getProperty(),NEGATIVE_BALANCE));

	if (direction){
		 BOOST_TEST_MESSAGE("The seller is inserted in priceMap, the buyer in x_Trade");
		 BOOST_CHECK(ContractDex_INSERT(seller));
		 BOOST_CHECK_EQUAL(TRADED, x_Trade(b));
	}else {
		BOOST_TEST_MESSAGE("The buyer is inserted in priceMap, the seller in x_Trade");
		BOOST_CHECK(ContractDex_INSERT(buyer));
		BOOST_CHECK_EQUAL(TRADED, x_Trade(s));
	}

	BOOST_CHECK_EQUAL(0,getMPbalance(seller.getAddr(), seller.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(1,getMPbalance(seller.getAddr(), seller.getProperty(),NEGATIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(buyer.getAddr(), buyer.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(buyer.getAddr(), buyer.getProperty(),NEGATIVE_BALANCE));


}

BOOST_AUTO_TEST_CASE(moreinbuyer_bl_ss)  // seller_amount = 20, buyer_amount = 30;
{                                         // seller is in short position = 8
                                          // buyer is in long position = 18
	CMPTally tally;  // the tally map object
	const uint256 tx;
	CMPContractDex seller(
					"1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", // address
					1,  // block
					3,  // property for sale
					20,  // amount of contracts for sale
					0,  // desired property
					0,
					tx, // txid
					1,  // position in block
					1,  // subaction
					0,  // amount remaining
					5,  // effective_price
					2 // trading_action
	);

	CMPContractDex buyer(
					"1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", // address
					1,  // block
					3,  // property for sale
					30,  // amount of contracts for trade
					0,   // desired property
					0,
					tx, // txid
					2,  // position in block
					1,  // subaction
					0,  // amount remaining
					5,  // effective_price
					1 // trading_action
	);
	 CMPContractDex *s;
	 s = &seller;
	CMPContractDex *b;
	b = &buyer;

	BOOST_CHECK_EQUAL(3, seller.getProperty());
	BOOST_CHECK_EQUAL("1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", seller.getAddr());
	BOOST_CHECK_EQUAL(20, seller.getAmountForSale());
	BOOST_CHECK_EQUAL(2, seller.getTradingAction());  // seller
	BOOST_CHECK_EQUAL(5, seller.getEffectivePrice());

	BOOST_CHECK_EQUAL(3, buyer.getProperty());
	BOOST_CHECK_EQUAL("1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", buyer.getAddr());
	BOOST_CHECK_EQUAL(30, buyer.getAmountForSale());
	BOOST_CHECK_EQUAL(1, buyer.getTradingAction()); //buyer
	BOOST_CHECK_EQUAL(5, buyer.getEffectivePrice());

	BOOST_CHECK(mastercore::update_tally_map(seller.getAddr(), seller.getProperty(),8 ,NEGATIVE_BALANCE)); // putting some money here
	BOOST_CHECK(mastercore::update_tally_map(buyer.getAddr(), buyer.getProperty(),18, POSSITIVE_BALANCE));


	BOOST_CHECK_EQUAL(0,getMPbalance(seller.getAddr(), seller.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(8,getMPbalance(seller.getAddr(), seller.getProperty(),NEGATIVE_BALANCE));
	BOOST_CHECK_EQUAL(18,getMPbalance(buyer.getAddr(), buyer.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(buyer.getAddr(), buyer.getProperty(),NEGATIVE_BALANCE));

	if (direction){
		 BOOST_TEST_MESSAGE("The seller is inserted in priceMap, the buyer in x_Trade");
		 BOOST_CHECK(ContractDex_INSERT(seller));
		 BOOST_CHECK_EQUAL(TRADED, x_Trade(b));
	}else {
		BOOST_TEST_MESSAGE("The buyer is inserted in priceMap, the seller in x_Trade");
		BOOST_CHECK(ContractDex_INSERT(buyer));
		BOOST_CHECK_EQUAL(TRADED, x_Trade(s));
	}

	BOOST_CHECK_EQUAL(0,getMPbalance(seller.getAddr(), seller.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(28,getMPbalance(seller.getAddr(), seller.getProperty(),NEGATIVE_BALANCE));
	BOOST_CHECK_EQUAL(38,getMPbalance(buyer.getAddr(), buyer.getProperty(),POSSITIVE_BALANCE));
	BOOST_CHECK_EQUAL(0,getMPbalance(buyer.getAddr(), buyer.getProperty(),NEGATIVE_BALANCE));


}


//BOOST_AUTO_TEST_CASE(no_match_different_sp)
// {
//
//   const uint256 tx;   // address,block,property,amount for sale, desired property, amount desired,uint256 tx,idx, suba, amount remaining,desire price, for sale price
//   CMPContractDex object2("1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH",1, 1, 0, 1, 8,tx, 1, 1, 1, 5, 5); //the buyer
// 	CMPContractDex object("1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz",1, 2, 3, 2, 0,tx, 2, 1, 3, 5, 5); // the seller
// 	CMPContractDex *pobjContractDex;
// 	pobjContractDex = &object;
// 	CMPContractDex *q;
// 	q = &object2;
//
// 	BOOST_CHECK_EQUAL(1, object2.getProperty());  // buyer
// 	BOOST_CHECK_EQUAL(1, object2.getDesProperty());
// 	BOOST_CHECK_EQUAL("1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", object2.getAddr());
// 	BOOST_CHECK_EQUAL(8,object2.getAmountDesired());
// 	BOOST_CHECK_EQUAL(1,object2.getAmountRemaining());
// 	BOOST_CHECK_EQUAL(5, object2.getDesiredPrice());
// 	BOOST_CHECK_EQUAL(5, object2.getForsalePrice());
// 	BOOST_CHECK_EQUAL(1, object2.getBlock());
//
// 	BOOST_CHECK_EQUAL(2, pobjContractDex->getProperty());  // seller
// 	BOOST_CHECK_EQUAL(2, pobjContractDex->getDesProperty());
//   BOOST_CHECK_EQUAL("1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", pobjContractDex->getAddr());
// 	BOOST_CHECK_EQUAL(3,pobjContractDex->getAmountForSale());
// 	BOOST_CHECK_EQUAL(0,pobjContractDex->getAmountDesired());
// 	BOOST_CHECK_EQUAL(3,pobjContractDex->getAmountRemaining());
// 	BOOST_CHECK_EQUAL(5, pobjContractDex->getDesiredPrice());
// 	BOOST_CHECK_EQUAL(5, pobjContractDex->getForsalePrice());
// 	BOOST_CHECK_EQUAL(1, pobjContractDex->getBlock());
//
//
// 	BOOST_CHECK(mastercore::update_tally_map(object2.getAddr(), object2.getProperty(), 10, BALANCE));  // putting some money here
// 	BOOST_CHECK(mastercore::update_tally_map(pobjContractDex->getAddr(), pobjContractDex->getProperty(), 10, BALANCE));
// 	BOOST_CHECK_EQUAL(10,getMPbalance(object2.getAddr(), object2.getProperty(), BALANCE)); //checking balance of sender
// 	BOOST_CHECK_EQUAL(10,getMPbalance(pobjContractDex->getAddr(), pobjContractDex->getProperty(), BALANCE));
//
//
// 	BOOST_CHECK(mastercore::MetaDEx_INSERT(object)); // the seller is inserted
//   BOOST_CHECK_EQUAL(NOTHING, x_Trade(q));   // the buyer wants 8 sp at  price of 5!
//                                             // the seller has 3 contracts at price of 5!
//                                              // There's no match!!!!
// 	BOOST_CHECK_EQUAL(10,getMPbalance(object2.getAddr(), object2.getProperty(), BALANCE)); //checking balance of sender
// 	BOOST_CHECK_EQUAL(10,getMPbalance(pobjContractDex->getAddr(), pobjContractDex->getProperty(), BALANCE));
//
//
//
// }
//
// BOOST_AUTO_TEST_CASE(no_amount_remaining)
// {
//
//   const uint256 tx;   // address,block,property,amount for sale, desired property, amount desired,uint256 tx,idx, suba, amount remaining,desire price, for sale price
//   CMPContractDex object2("1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH",1, 1, 0, 1, 8,tx, 1, 1, 1, 5, 5); //the buyer
// 	CMPContractDex object("1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz",1, 1, 3, 1, 0,tx, 2, 1, 0, 5, 5); // the seller
// 	CMPContractDex *pobjContractDex;
// 	pobjContractDex = &object;
// 	CMPContractDex *q;
// 	q = &object2;
//
// 	BOOST_CHECK_EQUAL(1, object2.getProperty());  // buyer
// 	BOOST_CHECK_EQUAL(1, object2.getDesProperty());
// 	BOOST_CHECK_EQUAL("1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", object2.getAddr());
// 	BOOST_CHECK_EQUAL(8,object2.getAmountDesired());
// 	BOOST_CHECK_EQUAL(1,object2.getAmountRemaining());
// 	BOOST_CHECK_EQUAL(5, object2.getDesiredPrice());
// 	BOOST_CHECK_EQUAL(5, object2.getForsalePrice());
// 	BOOST_CHECK_EQUAL(1, object2.getBlock());
//
//   BOOST_CHECK_EQUAL(1, pobjContractDex->getProperty());  // seller
//   BOOST_CHECK_EQUAL(1, pobjContractDex->getDesProperty());
//   BOOST_CHECK_EQUAL("1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", pobjContractDex->getAddr());
//   BOOST_CHECK_EQUAL(3,pobjContractDex->getAmountForSale());
//   BOOST_CHECK_EQUAL(0,pobjContractDex->getAmountDesired());
//   BOOST_CHECK_EQUAL(0,pobjContractDex->getAmountRemaining());
//   BOOST_CHECK_EQUAL(5, pobjContractDex->getDesiredPrice());
//   BOOST_CHECK_EQUAL(5, pobjContractDex->getForsalePrice());
//   BOOST_CHECK_EQUAL(1, pobjContractDex->getBlock());
//
// 	BOOST_CHECK(mastercore::update_tally_map(object2.getAddr(), object2.getProperty(), 10, BALANCE));  // putting some money here
// 	BOOST_CHECK(mastercore::update_tally_map(pobjContractDex->getAddr(), pobjContractDex->getProperty(), 10, BALANCE));
// 	BOOST_CHECK_EQUAL(10,getMPbalance(object2.getAddr(), object2.getProperty(), BALANCE)); //checking balance of sender
// 	BOOST_CHECK_EQUAL(10,getMPbalance(pobjContractDex->getAddr(), pobjContractDex->getProperty(), BALANCE));
//
//
// 	BOOST_CHECK(mastercore::MetaDEx_INSERT(object)); // the seller is inserted
//   BOOST_CHECK_EQUAL(NOTHING, x_Trade(q));   // the buyer wants 8 sp at  price of 5!
//                                             // the seller has 0 contracts remaining!
//                                              // There's no match!!!!
// 	BOOST_CHECK_EQUAL(10,getMPbalance(object2.getAddr(), object2.getProperty(), BALANCE)); //checking balance of sender
// 	BOOST_CHECK_EQUAL(10,getMPbalance(pobjContractDex->getAddr(), pobjContractDex->getProperty(), BALANCE));
//
// }
//
// BOOST_AUTO_TEST_CASE(match_full_amount)
// {
//
//   const uint256 tx;   // address,block,property,amount for sale, desired property, amount desired,uint256 tx,idx, suba, amount remaining,desire price, for sale price
//   CMPContractDex object2("1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH",1, 1, 0, 1, 10,tx, 1, 1, 1, 9, 9); //the buyer
// 	CMPContractDex object("1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz",1, 1, 10, 1, 0,tx, 2, 1, 10, 9, 9); // the seller
// 	CMPContractDex *pobjContractDex;
// 	pobjContractDex = &object;
// 	CMPContractDex *q;
// 	q = &object2;
//
//
// 	BOOST_CHECK_EQUAL(1, object2.getProperty());  // buyer
// 	BOOST_CHECK_EQUAL(1, object2.getDesProperty());
// 	BOOST_CHECK_EQUAL("1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", object2.getAddr());
// 	BOOST_CHECK_EQUAL(10,object2.getAmountDesired());
// 	BOOST_CHECK_EQUAL(1,object2.getAmountRemaining());
// 	BOOST_CHECK_EQUAL(9, object2.getDesiredPrice());
// 	BOOST_CHECK_EQUAL(9, object2.getForsalePrice());
// 	BOOST_CHECK_EQUAL(1, object2.getBlock());
//
// 	BOOST_CHECK_EQUAL(1, pobjContractDex->getProperty());  // seller
// 	BOOST_CHECK_EQUAL(1, pobjContractDex->getDesProperty());
// 	BOOST_CHECK_EQUAL("1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", pobjContractDex->getAddr());
// 	BOOST_CHECK_EQUAL(10,pobjContractDex->getAmountForSale());
// 	BOOST_CHECK_EQUAL(0,pobjContractDex->getAmountDesired());
// 	BOOST_CHECK_EQUAL(10,pobjContractDex->getAmountRemaining());
// 	BOOST_CHECK_EQUAL(9, pobjContractDex->getDesiredPrice());
// 	BOOST_CHECK_EQUAL(9, pobjContractDex->getForsalePrice());
// 	BOOST_CHECK_EQUAL(1, pobjContractDex->getBlock());
//
// 	BOOST_CHECK(mastercore::update_tally_map(object2.getAddr(), object2.getProperty(), 10, BALANCE));  // putting some money here
// 	BOOST_CHECK(mastercore::update_tally_map(pobjContractDex->getAddr(), pobjContractDex->getProperty(), 10, BALANCE));
// 	BOOST_CHECK_EQUAL(10,getMPbalance(object2.getAddr(), object2.getProperty(), BALANCE)); //checking balance of sender
// 	BOOST_CHECK_EQUAL(10,getMPbalance(pobjContractDex->getAddr(), pobjContractDex->getProperty(), BALANCE));
//
//
// 	BOOST_CHECK(mastercore::MetaDEx_INSERT(object)); // the seller is inserted
//   BOOST_CHECK_EQUAL(TRADED, x_Trade(q));   // the buyer wants 7 contracts at  price of 5!
//                                             // the seller has 7 contracts !
//                                              // There is match!!!!
// 	BOOST_CHECK_EQUAL(10,getMPbalance(object2.getAddr(), object2.getProperty(), BALANCE)); //checking balance of sender
// 	BOOST_CHECK_EQUAL(10,getMPbalance(pobjContractDex->getAddr(), pobjContractDex->getProperty(), BALANCE));
//
// }
//
//
// BOOST_AUTO_TEST_CASE(match_and_buyer_rest)
// {
//
//   const uint256 tx;   // address,block,property,amount for sale, desired property, amount desired,uint256 tx,idx, suba, amount remaining,desire price, for sale price
//   CMPContractDex object2("1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH",1, 1, 0, 1, 7,tx, 1, 1, 1, 9, 9); //the buyer
// 	CMPContractDex object("1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz",1, 1, 3, 1, 1,tx, 2, 1, 3, 9, 9); // the seller
// 	CMPContractDex *pobjContractDex;
// 	pobjContractDex = &object;
// 	CMPContractDex *q;
// 	q = &object2;
//
//   BOOST_TEST_PASSPOINT();
// 	BOOST_CHECK_EQUAL(0, object2.getProperty());  // buyer
// 	BOOST_CHECK_EQUAL(0, object2.getDesProperty());
// 	BOOST_CHECK_EQUAL("", object2.getAddr());
// 	BOOST_CHECK_EQUAL(0,object2.getAmountDesired());
// 	BOOST_CHECK_EQUAL(0,object2.getAmountRemaining());
// 	BOOST_CHECK_EQUAL(0, object2.getDesiredPrice());
// 	BOOST_CHECK_EQUAL(0, object2.getForsalePrice());
// 	BOOST_CHECK_EQUAL(0, object2.getBlock());
//
//   BOOST_CHECK_EQUAL(0, pobjContractDex->getProperty());  // seller
//   BOOST_CHECK_EQUAL(0, pobjContractDex->getDesProperty());
//   BOOST_CHECK_EQUAL("", pobjContractDex->getAddr());
//   BOOST_CHECK_EQUAL(0,pobjContractDex->getAmountForSale());
//   BOOST_CHECK_EQUAL(0,pobjContractDex->getAmountDesired());
//   BOOST_CHECK_EQUAL(0,pobjContractDex->getAmountRemaining());
//   BOOST_CHECK_EQUAL(0, pobjContractDex->getDesiredPrice());
//   BOOST_CHECK_EQUAL(0, pobjContractDex->getForsalePrice());
//   BOOST_CHECK_EQUAL(0, pobjContractDex->getBlock());
//
// 	BOOST_CHECK(mastercore::update_tally_map(object2.getAddr(), object2.getProperty(), 10, BALANCE));  // putting some money here
// 	BOOST_CHECK(mastercore::update_tally_map(pobjContractDex->getAddr(), pobjContractDex->getProperty(), 10, BALANCE));
// 	BOOST_CHECK_EQUAL(10,getMPbalance(object2.getAddr(), object2.getProperty(), BALANCE)); //checking balance of sender
// 	BOOST_CHECK_EQUAL(10,getMPbalance(pobjContractDex->getAddr(), pobjContractDex->getProperty(), BALANCE));
//
//
// 	BOOST_CHECK(mastercore::MetaDEx_INSERT(object)); // the seller is inserted
//   BOOST_CHECK_EQUAL(TRADED, x_Trade(q));   // the buyer wants 7 contracts at  price of 5!
//                                             // the seller has only 2 contracts !
//                                              // There is match!!!!
// 	BOOST_CHECK_EQUAL(10,getMPbalance(object2.getAddr(), object2.getProperty(), BALANCE)); //checking balance of sender
// 	BOOST_CHECK_EQUAL(10,getMPbalance(pobjContractDex->getAddr(), pobjContractDex->getProperty(), BALANCE));
//
//
// }
//
//
// BOOST_AUTO_TEST_CASE(match_and_seller_rest)
// {
//
// 	const uint256 tx;   // address,block,property,amount for sale, desired property, amount desired,uint256 tx,idx, suba, amount remaining,desire price, for sale price
// 	CMPContractDex object2("1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH",1, 1, 0, 1, 4,tx, 1, 1, 1, 5, 5); //the buyer
// 	CMPContractDex object("1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz",1, 1, 8, 1, 0,tx, 2, 1, 10, 5, 5); // the seller
// 	CMPContractDex *pobjContractDex;
// 	pobjContractDex = &object;
// 	CMPContractDex *q;
// 	q = &object2;
//
// 	BOOST_CHECK_EQUAL(0, object2.getProperty());  // buyer
// 	BOOST_CHECK_EQUAL(0, object2.getDesProperty());
// 	BOOST_CHECK_EQUAL("", object2.getAddr());
// 	BOOST_CHECK_EQUAL(0,object2.getAmountDesired());
// 	BOOST_CHECK_EQUAL(0,object2.getAmountRemaining());
// 	BOOST_CHECK_EQUAL(0, object2.getDesiredPrice());
// 	BOOST_CHECK_EQUAL(0, object2.getForsalePrice());
// 	BOOST_CHECK_EQUAL(0, object2.getBlock());
//
// 	BOOST_CHECK_EQUAL(0, pobjContractDex->getProperty());  // seller
// 	BOOST_CHECK_EQUAL(0, pobjContractDex->getDesProperty());
// 	BOOST_CHECK_EQUAL("", pobjContractDex->getAddr());
// 	BOOST_CHECK_EQUAL(0,pobjContractDex->getAmountForSale());
// 	BOOST_CHECK_EQUAL(0,pobjContractDex->getAmountDesired());
// 	BOOST_CHECK_EQUAL(0,pobjContractDex->getAmountRemaining());
// 	BOOST_CHECK_EQUAL(0, pobjContractDex->getDesiredPrice());
// 	BOOST_CHECK_EQUAL(0, pobjContractDex->getForsalePrice());
// 	BOOST_CHECK_EQUAL(0, pobjContractDex->getBlock());
//
// 	BOOST_CHECK(mastercore::update_tally_map(object2.getAddr(), object2.getProperty(), 10, BALANCE));  // putting some money here
// 	BOOST_CHECK(mastercore::update_tally_map(pobjContractDex->getAddr(), pobjContractDex->getProperty(), 10, BALANCE));
// 	BOOST_CHECK_EQUAL(10,getMPbalance(object2.getAddr(), object2.getProperty(), BALANCE)); //checking balance of sender
// 	BOOST_CHECK_EQUAL(10,getMPbalance(pobjContractDex->getAddr(), pobjContractDex->getProperty(), BALANCE));
//
//
// 	BOOST_CHECK(mastercore::MetaDEx_INSERT(object)); // the seller is inserted
// 	BOOST_CHECK_EQUAL(TRADED, x_Trade(q));   // the buyer wants 4 contracts at  price of 5!
// 																						// the seller has only 8 contracts at price of 5! ( amount remaining of the seller:10)
//
// 	BOOST_CHECK_EQUAL(10,getMPbalance(object2.getAddr(), object2.getProperty(), BALANCE)); //checking balance of sender
// 	BOOST_CHECK_EQUAL(10,getMPbalance(pobjContractDex->getAddr(), pobjContractDex->getProperty(), BALANCE));
//
//
// }
//


BOOST_AUTO_TEST_SUITE_END()
