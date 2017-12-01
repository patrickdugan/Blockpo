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

#define PACKET_SIZE         31
#define MAX_PACKETS        255
#define PACKET_SIZE_CLASS_C 19

/*Remember: This new function was defined to build the new class CMPContractDex*/
namespace mastercore
{
	extern std::string GenerateConsensusString(const CMPContractDex &tradeObj);
}

/*Remember: Check in Boost library BasicTestingSetup option*/
BOOST_FIXTURE_TEST_SUITE(omnicore_contractdex_object, BasicTestingSetup)

BOOST_AUTO_TEST_CASE(object_default)
{
    	CMPContractDex tradeA;
   		BOOST_CHECK_EQUAL("0000000000000000000000000000000000000000000000000000000000000000||0|0|0|0|0|0|0|",
        GenerateConsensusString(tradeA));

		CMPContractDex tradeB("1PxejjeWZc9ZHph7A3SYDo2sk2Up4AcysH", 395000, 31, 1000000, 1, 2000000,
        uint256S("2c9a055899147b03b2c5240a020c1f94d243a834ecc06ab8cfa504ee29d07b7d"), 1, 1, 900000, 500000, 400000);
    	BOOST_CHECK_EQUAL("2c9a055899147b03b2c5240a020c1f94d243a834ecc06ab8cfa504ee29d07b7d|1PxejjeWZc9ZHph7A3SYDo2sk2Up4AcysH|31|1000000|1|2000000|900000|500000|400000|",
        GenerateConsensusString(tradeB));
		CMPContractDex *pobjContractDex;        
		pobjContractDex = &tradeB;

        BOOST_CHECK_EQUAL(0, pobjContractDex->getProperty());  // testing some properties of the object
        BOOST_CHECK_EQUAL(0, pobjContractDex->getDesProperty());
        BOOST_CHECK_EQUAL("CASH", pobjContractDex->getAddr());
        BOOST_CHECK_EQUAL(0,pobjContractDex->getAmountForSale());
        BOOST_CHECK_EQUAL(0,pobjContractDex->getAmountDesired());
        BOOST_CHECK_EQUAL(0,pobjContractDex->getAmountRemaining());
        BOOST_CHECK_EQUAL(0, pobjContractDex->getDesiredPrice());
        BOOST_CHECK_EQUAL(0, pobjContractDex->getForsalePrice());
        BOOST_CHECK_EQUAL(0, pobjContractDex->getBlock());

        BOOST_CHECK_EQUAL(NOTHING, x_Trade(pobjContractDex));
}

//////////////////////////////

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

//////////////////////////////


BOOST_AUTO_TEST_CASE(object_matching)
{
	// {
 //        int nBlock = ConsensusParams().NULLDATA_BLOCK;

 //        std::vector<CTxOut> txInputs;
 //        txInputs.push_back(createTxOut(5000000, "1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz"));

 //        std::vector<CTxOut> txOutputs;
 //        txOutputs.push_back(OpReturn_MultiSimpleSend());
 //        txOutputs.push_back(createTxOut(2700000, ExodusAddress().ToString()));

 //        CTransaction dummyTx = TxClassC(txInputs, txOutputs);

 //        CMPTransaction metaTx;
 //        BOOST_CHECK(ParseTransaction(dummyTx, nBlock, 1, metaTx) == 0);
 //        BOOST_CHECK(metaTx.getReceiver().empty());
 //        BOOST_CHECK_EQUAL(metaTx.getFeePaid(), 2300000);
 //        BOOST_CHECK_EQUAL(metaTx.getSender(), "1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz");
 //        BOOST_CHECK_EQUAL(metaTx.getPayload(), "00000000000000070000000006dac2c0");
 //        BOOST_CHECK_EQUAL(metaTx.getTypeString(), "Simple Send");

 //        CMPContractDex objCDex(metaTx);

 //        /////////////////////////////////
 //        std::vector<CTxOut> txInputsB;
 //        txInputs.push_back(createTxOut(5000000, "1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz"));

 //        std::vector<CTxOut> txOutputsB;
 //        txOutputs.push_back(OpReturn_MultiSimpleSend());
 //        txOutputs.push_back(createTxOut(2700000, ExodusAddress().ToString()));

 //        CTransaction dummyTxB = TxClassC(txInputsB, txOutputsB);

 //        CMPTransaction metaTxB;
 //        BOOST_CHECK(ParseTransaction(dummyTxB, nBlock, 1, metaTxB) == 0);
 //        BOOST_CHECK(metaTxB.getReceiver().empty());
 //        BOOST_CHECK_EQUAL(metaTxB.getFeePaid(), 2300000);
 //        BOOST_CHECK_EQUAL(metaTxB.getSender(), "1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz");
 //        BOOST_CHECK_EQUAL(metaTxB.getPayload(), "00000000000000070000000006dac2c0");
 //        BOOST_CHECK_EQUAL(metaTxB.getTypeString(), "Simple Send");

 //        CMPContractDex objCDexB(metaTxB);
 //        CMPContractDex *pB;
 //        pB = &objCDexB;

 //        BOOST_CHECK(mastercore::MetaDEx_INSERT(objCDex));
 //        BOOST_CHECK_EQUAL(NOTHING, x_Trade(pB));
 //  }

  {
        unsigned int packet_size = 0;
        ////////////////////////////////////////////////////////////////////////////////
        // Objeto #A
        unsigned char single_pktA[MAX_PACKETS * PACKET_SIZE];
        packet_size = PACKET_SIZE_CLASS_C;
        memcpy(single_pktA, &ParseHex("0000001900000001000000000ee6b2800000001f000000012a05f200")[0], packet_size);

        CMPTransaction objCMPTA;
        objCMPTA.Set("1PxejjeWZc9ZHph7A3SYDo2sk2Up4AcysH", "1zAtHRASgdHvZDfHs6xJquMghga4eG7gy", 4000000, 
         uint256S("2c9a055899147b03b2c5240a020c1f94d243a834ecc06ab8cfa504ee29d07b7d"), 395000, 1, 
         (unsigned char *)&single_pktA, 33, 3, 0);

        CMPContractDex objCMPCA(objCMPTA);
        CMPContractDex *pA;
        pA = &objCMPCA;

        BOOST_CHECK_EQUAL(objCMPCA.getAmountForSale(), 4000000);
        BOOST_CHECK_EQUAL(objCMPCA.getAmountDesired(), 0);
        BOOST_CHECK_EQUAL(objCMPCA.getAmountRemaining(), 4000000);
        BOOST_CHECK_EQUAL(objCMPCA.getDesiredPrice(), 0);
        BOOST_CHECK_EQUAL(objCMPCA.getForsalePrice(), 0);

        ////////////////////////////////////////////////////////////////////////////////
        // Objeto #B
        unsigned char single_pktB[MAX_PACKETS * PACKET_SIZE];
        packet_size = PACKET_SIZE_CLASS_C;
        memcpy(single_pktB, &ParseHex("00010014000000010000000005f5e1000000000001312d000a000000000000271001")[0], packet_size);

        CMPTransaction objCMPTB;
        objCMPTB.Set("1PxejjeWZc9ZHph7A3SYDo2sk2Up4AcysH", "1zAtHRASgdHvZDfHs6xJquMghga4eG7gy", 2000000, 
         uint256S("2c9a055899147b03b2c5240a020c1f94d243a834ecc06ab8cfa504ee29d07b7d"), 395000, 1, 
         (unsigned char *)&single_pktB, 33, 3, 0);

        CMPContractDex objCMPCB(objCMPTB);

        BOOST_CHECK(mastercore::MetaDEx_INSERT(objCMPCB));
        BOOST_CHECK_EQUAL(NOTHING, x_Trade(pA));
        ////////////////////////////////////////////////////////////////////////////////
  }

        //  // Sell tokens for bitcoins [type 20, version 1]
        // std::vector<unsigned char> vch = CreatePayload_DExSell(
        // static_cast<uint32_t>(1),         // property: MSC
        // static_cast<int64_t>(100000000),  // amount to transfer: 1.0 MSC (in willets)
        // static_cast<int64_t>(20000000),   // amount desired: 0.2 BTC (in satoshis)
        // static_cast<uint8_t>(10),         // payment window in blocks
        // static_cast<int64_t>(10000),      // commitment fee in satoshis
        // static_cast<uint8_t>(1));         // sub-action: new offer



		// CMPTransaction objCMPTransactionB;
		// objCMPTransactionB.Set("1PxejjeWZc9ZHph7A3SYDo2sk2Up4AcysH", "1zAtHRASgdHvZDfHs6xJquMghga4eG7gy", 4000000, 
		// 	uint256S("2c9a055899147b03b2c5240a020c1f94d243a834ecc06ab8cfa504ee29d07b7d"), 395000, 1, 
		// 	(unsigned char *)&single_pkt, 33, 1, 0);


		// /*Remember: Here amount_desired = 0 "seller"*/
		// CMPContractDex tradeB("1PxejjeWZc9ZHph7A3SYDo2sk2Up4AcysH", 395000, 31, 4000000, 1, 0,
  //       uint256S("2c9a055899147b03b2c5240a020c1f94d243a834ecc06ab8cfa504ee29d07b7d"), 1, 1, 2000000, 0, 100000);
  //   	BOOST_CHECK_EQUAL("2c9a055899147b03b2c5240a020c1f94d243a834ecc06ab8cfa504ee29d07b7d|1PxejjeWZc9ZHph7A3SYDo2sk2Up4AcysH|31|4000000|1|0|2000000|0|100000|",
  //       GenerateConsensusString(tradeB));
		// CMPContractDex *pobjContractDexB;        
		// pobjContractDexB = &tradeB;

		// /*Remember: Here amount_forsale = 0 "buyer"*/
		// CMPContractDex tradeC("1zAtHRASgdHvZDfHs6xJquMghga4eG7gy", 395000, 31, 0, 1, 2000000,
  //       uint256S("2c9a055899147b03b2c5240a020c1f94d243a834ecc06ab8cfa504ee29d07b7d"), 1, 1, 0, 100000, 0);
  //   	BOOST_CHECK_EQUAL("2c9a055899147b03b2c5240a020c1f94d243a834ecc06ab8cfa504ee29d07b7d|1zAtHRASgdHvZDfHs6xJquMghga4eG7gy|31|0|1|2000000|0|100000|0|",
  //       GenerateConsensusString(tradeC));
		// CMPContractDex *pobjContractDexC;        
		// pobjContractDexC = &tradeC;

		// /*Seller*/
  //       BOOST_CHECK_EQUAL("1PxejjeWZc9ZHph7A3SYDo2sk2Up4AcysH", pobjContractDexB->getAddr());  
  //       BOOST_CHECK_EQUAL(395000, pobjContractDexB->getBlock());
  //       BOOST_CHECK_EQUAL(31, pobjContractDexB->getProperty());
  //       BOOST_CHECK_EQUAL(4000000, pobjContractDexB->getAmountForSale());
  //       BOOST_CHECK_EQUAL(1, pobjContractDexB->getDesProperty());
  //       BOOST_CHECK_EQUAL(0, pobjContractDexB->getAmountDesired());
		// BOOST_CHECK_EQUAL(1, pobjContractDexB->getIdx());
		// BOOST_CHECK_EQUAL(1, pobjContractDexB->getAction());
		// BOOST_CHECK_EQUAL(2000000, pobjContractDexB->getAmountRemaining());
		// BOOST_CHECK_EQUAL(0, pobjContractDexB->getDesiredPrice());
		// BOOST_CHECK_EQUAL(100000, pobjContractDexB->getForsalePrice());

		// /*Buyer*/
		// BOOST_CHECK_EQUAL("1zAtHRASgdHvZDfHs6xJquMghga4eG7gy", pobjContractDexC->getAddr());  
  //       BOOST_CHECK_EQUAL(395000, pobjContractDexC->getBlock());
  //       BOOST_CHECK_EQUAL(31, pobjContractDexC->getProperty());
  //       BOOST_CHECK_EQUAL(0, pobjContractDexC->getAmountForSale());
  //       BOOST_CHECK_EQUAL(1, pobjContractDexC->getDesProperty());
  //       BOOST_CHECK_EQUAL(2000000, pobjContractDexC->getAmountDesired());
		// BOOST_CHECK_EQUAL(1, pobjContractDexC->getIdx());
		// BOOST_CHECK_EQUAL(1, pobjContractDexC->getAction());
		// BOOST_CHECK_EQUAL(0, pobjContractDexC->getAmountRemaining());
		// BOOST_CHECK_EQUAL(100000, pobjContractDexC->getDesiredPrice());
		// BOOST_CHECK_EQUAL(0, pobjContractDexC->getForsalePrice());


		// BOOST_CHECK(mastercore::MetaDEx_INSERT(tradeB));
  //       BOOST_CHECK_EQUAL(NOTHING, x_Trade(pobjContractDexC));
}

BOOST_AUTO_TEST_SUITE_END()