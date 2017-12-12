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

#include "omnicore/convert.h"

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
namespace mastercore {
    extern std::string GenerateConsensusString(const CMPContractDex &tradeObj);
}

/** Creates a dummy transaction with the given inputs and outputs. */
static CTransaction TxClassC(const std::vector<CTxOut>& txInputs, const std::vector<CTxOut>& txOuts) {
    CMutableTransaction mutableTx;

    // Inputs:
    for (std::vector<CTxOut>::const_iterator it = txInputs.begin(); it != txInputs.end(); ++it) {
        const CTxOut& txOut = *it;

        // Create transaction for input:
        CMutableTransaction inputTx;
        unsigned int nOut = 0;
        inputTx.vout.push_back(txOut);
        CTransaction tx(inputTx);

        // Populate transaction cache:
        CCoinsModifier coins = view.ModifyCoins(tx.GetHash());

        if (nOut >= coins->vout.size()) {
            coins->vout.resize(nOut + 1);
        }
        coins->vout[nOut].scriptPubKey = txOut.scriptPubKey;
        coins->vout[nOut].nValue = txOut.nValue;

        // Add input:
        CTxIn txIn(tx.GetHash(), nOut);
        mutableTx.vin.push_back(txIn);
    }

    for (std::vector<CTxOut>::const_iterator it = txOuts.begin(); it != txOuts.end(); ++it) {
        const CTxOut& txOut = *it;
        mutableTx.vout.push_back(txOut);
    }

    return CTransaction(mutableTx);
}

/** Helper to create a CTxOut object. */
static CTxOut createTxOut(int64_t amount, const std::string& dest) {
    return CTxOut(amount, GetScriptForDestination(CBitcoinAddress(dest).Get()));
}

BOOST_FIXTURE_TEST_SUITE(omnicore_contractdex_object, BasicTestingSetup)

BOOST_AUTO_TEST_CASE(object_default)
{
    const uint256 tx; // empty txid
    CMPContractDex object2(
            "1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", // address
            1,  // block
            1,  // property for sale
            0,  // amount for sale <-- is this correct?
            1,  // desired property
            8,  // amount desired
            tx, // txid
            1,  // position in block
            1,  // subaction
            1,  // amount remaining
            5,  // desired price
            5   // for sale price
    ); // the buyer

    CMPContractDex object(
            "1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", // address
            1,  // block
            1,  // property for sale
            3,  // amount for sale
            1,  // desired property
            0,  // amount desired <-- is this correct?
            tx, // txid
            2,  // position in block
            1,  // subaction
            3,  // amount remaining
            5,  // desired price
            25  // for sale price
    ); // the seller

    BOOST_CHECK_EQUAL(1, object2.getProperty()); // buyer
    BOOST_CHECK_EQUAL(1, object2.getDesProperty());
    BOOST_CHECK_EQUAL("1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", object2.getAddr());
    BOOST_CHECK_EQUAL(8, object2.getAmountDesired());
    BOOST_CHECK_EQUAL(1, object2.getAmountRemaining());
    BOOST_CHECK_EQUAL(5, object2.getDesiredPrice());
    BOOST_CHECK_EQUAL(5, object2.getForsalePrice());
    BOOST_CHECK_EQUAL(1 , object2.getBlock());

    BOOST_CHECK_EQUAL(1, object.getProperty()); // seller
    BOOST_CHECK_EQUAL(1, object.getDesProperty());
    BOOST_CHECK_EQUAL("1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", object.getAddr());
    BOOST_CHECK_EQUAL(3, object.getAmountForSale());
    BOOST_CHECK_EQUAL(0, object.getAmountDesired());
    BOOST_CHECK_EQUAL(3, object.getAmountRemaining());
    BOOST_CHECK_EQUAL(5, object.getDesiredPrice());
    BOOST_CHECK_EQUAL(25, object.getForsalePrice());
    BOOST_CHECK_EQUAL(1, object.getBlock());

    BOOST_CHECK(mastercore::update_tally_map(object2.getAddr(), object2.getProperty(), 10, BALANCE)); // putting some money here
    BOOST_CHECK(mastercore::update_tally_map(object.getAddr(), object.getProperty(), 10, BALANCE));
    BOOST_CHECK_EQUAL(10, getMPbalance(object2.getAddr(), object2.getProperty(), BALANCE)); // checking balance of sender
    BOOST_CHECK_EQUAL(10, getMPbalance(object.getAddr(), object.getProperty(), BALANCE));

    BOOST_CHECK(mastercore::MetaDEx_INSERT(object)); // the seller is inserted
    BOOST_CHECK_EQUAL(TRADED, x_Trade(&object2)); // the buyer wants 6 contracts at price of 5!

    BOOST_CHECK_EQUAL(10, getMPbalance(object2.getAddr(), object2.getProperty(), BALANCE)); // checking balance of sender
    BOOST_CHECK_EQUAL(10, getMPbalance(object.getAddr(), object.getProperty(), BALANCE));
}

BOOST_AUTO_TEST_CASE(object_checkpkt_metadex) {
    std::vector<unsigned char> vch = CreatePayload_MetaDExTrade(
            static_cast<uint32_t> (1), /*property*/
            static_cast<uint64_t> (45), /*amount_forsale*/
            static_cast<uint32_t> (1), /*property_desired*/
            static_cast<uint64_t> (254)); /*amount_desired*/

    BOOST_CHECK_EQUAL(HexStr(vch), "000000190000000100000000054c56380000002c0000000005f5e0ff");

    std::vector<unsigned char> strScriptData = ParseHex(
        "0000001900000001000000000000002d0000000100000000000000fe");

    BOOST_TEST_MESSAGE("sizeof(strScriptData):" << sizeof (strScriptData));

    int packet_size = sizeof (strScriptData) + sizeof (uint32_t);
    for (int i = 0; i < packet_size; ++i) {
        int j = strScriptData[i];
        BOOST_TEST_MESSAGE("strScriptData[i]:" << j);
    }

    unsigned char single_pkt[packet_size];
    memcpy(single_pkt, &strScriptData[0], packet_size);

    for (int i = 0; i < packet_size; ++i) {
        int z = single_pkt[i];
        BOOST_TEST_MESSAGE("single_pkt[i]:" << z);
    }

    const uint256 tx;
    CMPTransaction objCMPTran;
    objCMPTran.Set("1PxejjeWZc9ZHph7A3SYDo2sk2Up4AcysH", "1zAtHRASgdHvZDfHs6xJquMghga4eG7gy", 4000000, tx, 395000, 1,
            (unsigned char *) &single_pkt, 33, 31, 32);

    BOOST_CHECK_EQUAL(objCMPTran.getSender(), "1PxejjeWZc9ZHph7A3SYDo2sk2Up4AcysH");
    BOOST_CHECK_EQUAL(objCMPTran.getReceiver(), "1zAtHRASgdHvZDfHs6xJquMghga4eG7gy");
    BOOST_CHECK_EQUAL(objCMPTran.getAmount(), 4000000);
    BOOST_CHECK_EQUAL(objCMPTran.getNewAmount(), 4000000);
    BOOST_CHECK_EQUAL(objCMPTran.getIndexInBlock(), 1);
    BOOST_CHECK_EQUAL(objCMPTran.getEncodingClass(), 31);
    BOOST_CHECK_EQUAL(objCMPTran.getFeePaid(), 32);
    BOOST_CHECK_EQUAL(objCMPTran.getProperty(), 1);

    BOOST_CHECK_EQUAL(objCMPTran.interpret_Transaction(), true);
    CMPContractDex objCMPCont(objCMPTran);
    BOOST_CHECK_EQUAL(objCMPCont.getProperty(), 1);
    BOOST_CHECK_EQUAL(objCMPCont.getDesProperty(), 1);
    BOOST_CHECK_EQUAL(objCMPCont.getAmountForSale(), 45);
    BOOST_CHECK_EQUAL(objCMPCont.getAmountDesired(), 254);
}

BOOST_AUTO_TEST_CASE(object_contractdex_payload)
{
    std::vector<unsigned char> vch = CreatePayload_ContractDexTrade(
            static_cast<uint32_t> (1), /*property*/
            static_cast<uint64_t> (50), /*amount_forsale*/
            static_cast<uint32_t> (1), /*property_desired*/
            static_cast<uint64_t> (40), /*amount_desired*/
            static_cast<uint64_t> (20), /*price_desired*/
            static_cast<uint64_t> (30)); /*price_forsale*/

    BOOST_CHECK_EQUAL(HexStr(vch), "000000190000000100000000054c56380000002c0000000005f5e0ff");
    std::vector<unsigned char> strScriptData = ParseHex(
            "0000001d0000000100000000000000320000000100000000000000280000000000000014000000000000001e");

    BOOST_TEST_MESSAGE("sizeof(strScriptData):" << sizeof (strScriptData));

    int packet_size = sizeof (strScriptData) + sizeof (uint32_t) + 2 * sizeof (uint64_t);
    for (int i = 0; i < packet_size; ++i) {
        int j = strScriptData[i];
        BOOST_TEST_MESSAGE("strScriptData[i]:" << j);
    }

    unsigned char single_pkt[packet_size];
    memcpy(single_pkt, &strScriptData[0], packet_size);

    for (int i = 0; i < packet_size; ++i) {
        int z = single_pkt[i];
        BOOST_TEST_MESSAGE("single_pkt[i]:" << z);
    }

    const uint256 tx;
    CMPTransaction objCMPTran;
    objCMPTran.Set("1PxejjeWZc9ZHph7A3SYDo2sk2Up4AcysH", "1zAtHRASgdHvZDfHs6xJquMghga4eG7gy", 4000000, tx, 395000, 1,
            (unsigned char *) &single_pkt, 33, 31, 32);

    BOOST_CHECK_EQUAL(objCMPTran.getSender(), "1PxejjeWZc9ZHph7A3SYDo2sk2Up4AcysH");
    BOOST_CHECK_EQUAL(objCMPTran.getReceiver(), "1zAtHRASgdHvZDfHs6xJquMghga4eG7gy");
    BOOST_CHECK_EQUAL(objCMPTran.getAmount(), 4000000);
    BOOST_CHECK_EQUAL(objCMPTran.getNewAmount(), 4000000);
    BOOST_CHECK_EQUAL(objCMPTran.getIndexInBlock(), 1);
    BOOST_CHECK_EQUAL(objCMPTran.getEncodingClass(), 31);
    BOOST_CHECK_EQUAL(objCMPTran.getFeePaid(), 32);

    BOOST_CHECK_EQUAL(objCMPTran.interpret_Transaction(), true);
    // CMPContractDex objCMPCont(objCMPTran);
    // BOOST_CHECK_EQUAL(objCMPCont.getProperty(), 1);
    // BOOST_CHECK_EQUAL(objCMPCont.getAmountForSale(), 50);
    // BOOST_CHECK_EQUAL(objCMPCont.getDesProperty(), 1);
    // BOOST_CHECK_EQUAL(objCMPCont.getAmountDesired(), 40);
    // BOOST_CHECK_EQUAL(objCMPCont.getDesiredPrice(), 20);    

    BOOST_TEST_MESSAGE("Size of single_pkt: " << sizeof (single_pkt));
}

BOOST_AUTO_TEST_SUITE_END()