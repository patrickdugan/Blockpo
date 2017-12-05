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


BOOST_FIXTURE_TEST_SUITE(omnicore_contractdex_object, BasicTestingSetup)

BOOST_AUTO_TEST_CASE(object_default)
{

  CMPTally tally;  // the tally map object
  const uint256 tx;   // address,block,property,amount for sale, desired property, amount desired,uint256 tx,idx, suba, amount remaining,desire price, for sale price
  CMPContractDex object2("1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH",1, 1, 0, 1, 8,tx, 1, 1, 1, 5, 5); //the buyer
	CMPContractDex object("1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz",1, 1, 3, 1, 0,tx, 2, 1, 3, 5, 25); // the seller
	CMPContractDex *pobjContractDex;
	pobjContractDex = &object;
	CMPContractDex *q;
	q = &object2;

	BOOST_CHECK_EQUAL(0, object2.getProperty());  // buyer
	BOOST_CHECK_EQUAL(0, object2.getDesProperty());
	BOOST_CHECK_EQUAL("", object2.getAddr());
	BOOST_CHECK_EQUAL(0,object2.getAmountDesired());
	BOOST_CHECK_EQUAL(0,object2.getAmountRemaining());
	BOOST_CHECK_EQUAL(0, object2.getDesiredPrice());
	BOOST_CHECK_EQUAL(0, object2.getForsalePrice());
	BOOST_CHECK_EQUAL(0, object2.getBlock());

  BOOST_CHECK_EQUAL(0, pobjContractDex->getProperty());  // seller
  BOOST_CHECK_EQUAL(0, pobjContractDex->getDesProperty());
  BOOST_CHECK_EQUAL("", pobjContractDex->getAddr());
  BOOST_CHECK_EQUAL(0,pobjContractDex->getAmountForSale());
  BOOST_CHECK_EQUAL(0,pobjContractDex->getAmountDesired());
  BOOST_CHECK_EQUAL(0,pobjContractDex->getAmountRemaining());
  BOOST_CHECK_EQUAL(0, pobjContractDex->getDesiredPrice());
  BOOST_CHECK_EQUAL(0, pobjContractDex->getForsalePrice());
  BOOST_CHECK_EQUAL(0, pobjContractDex->getBlock());

	BOOST_CHECK(mastercore::update_tally_map(object2.getAddr(), object2.getProperty(), 10, BALANCE));  // putting some money here
	BOOST_CHECK(mastercore::update_tally_map(pobjContractDex->getAddr(), pobjContractDex->getProperty(), 10, BALANCE));
	BOOST_CHECK_EQUAL(10,getMPbalance(object2.getAddr(), object2.getProperty(), BALANCE)); //checking balance of sender
	BOOST_CHECK_EQUAL(10,getMPbalance(pobjContractDex->getAddr(), pobjContractDex->getProperty(), BALANCE));


	BOOST_CHECK(mastercore::MetaDEx_INSERT(object)); // the seller is inserted
  BOOST_CHECK_EQUAL(TRADED, x_Trade(q));   // the buyer wants 6 contracts at  price of 5!
                                            // the seller has 10 contracts at price of 5!

	BOOST_CHECK_EQUAL(10,getMPbalance(object2.getAddr(), object2.getProperty(), BALANCE)); //checking balance of sender
	BOOST_CHECK_EQUAL(10,getMPbalance(pobjContractDex->getAddr(), pobjContractDex->getProperty(), BALANCE));





}




BOOST_AUTO_TEST_CASE(object_matching)
{



        // BOOST_CHECK(mastercore::MetaDEx_INSERT(objCDex));
        // BOOST_CHECK_EQUAL(NOTHING, x_Trade(pB));

				// Trade tokens for tokens [type 25, version 0]
		    std::vector<unsigned char> vch = CreatePayload_MetaDExTrade(
		        static_cast<uint32_t>(1),          // property: MSC
		        static_cast<int64_t>(2),   // amount for sale: 2.5 MSC
		        static_cast<uint32_t>(999),         // property desired: TetherUS
		        static_cast<int64_t>(254)); // amount desired: 50.0 TetherUS

		    BOOST_CHECK_EQUAL(HexStr(vch),"000000190000000100000000054c56380000002c0000000005f5e0ff");

        std::vector<unsigned char> v1 = ParseHex("00000019000000010000000000000002000003e700000000000000fe");

				std::ostringstream s;
				for(int i=0;i<28;i++)    // v1 is the pkt vector, the pkt size is in function of the tx.cpp specifications!!
				{                        // ae: for tx=25 the size is 28 excel in the v1 array(metadextrade).v1[4]=25 = tx type
					                       //   v1[8] = 1  (property)
					int j = v1[i];         //   v1[16] = 2 (property desired)
					                       //   v1[20] = 3 (property desired)
												  			 //   v1[28] = 4  (amount desired)
				BOOST_TEST_MESSAGE("vch:" << j);
			  }


      //  BOOST_TEST_MESSAGE( "v1[1]:" << v1[1] );
			//  BOOST_TEST_MESSAGE( "v1[2]:" << v1[2] );

}

BOOST_AUTO_TEST_SUITE_END()
