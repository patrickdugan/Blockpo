#include "omnicore/createpayload.h"

#include "omnicore/mdex.h"
#include "omnicore/omnicore.h"

#include "test/test_bitcoin.h"
#include "uint256.h"
#include "utilstrencodings.h"
#include "leveldb/db.h"



#include "omnicore/activation.h"
#include "omnicore/consensushash.h"
#include "omnicore/convert.h"
#include "omnicore/dex.h"
#include "omnicore/encoding.h"
#include "omnicore/errors.h"
#include "omnicore/fees.h"
#include "omnicore/log.h"
#include "omnicore/notifications.h"
#include "omnicore/pending.h"
#include "omnicore/persistence.h"
#include "omnicore/rules.h"
#include "omnicore/script.h"
#include "omnicore/seedblocks.h"
#include "omnicore/sp.h"
#include "omnicore/tally.h"
#include "omnicore/tx.h"
#include "omnicore/utils.h"
#include "omnicore/utilsbitcoin.h"
#include "omnicore/version.h"
#include "omnicore/walletcache.h"
#include "omnicore/wallettxs.h"

#include "base58.h"
#include "chainparams.h"
#include "coincontrol.h"
#include "coins.h"
#include "core_io.h"
#include "init.h"
#include "main.h"
#include "primitives/block.h"
#include "primitives/transaction.h"
#include "script/script.h"
#include "script/standard.h"
#include "sync.h"
#include "tinyformat.h"
#include "ui_interface.h"
#include "util.h"
#include "utiltime.h"

#include <univalue.h>
#include <boost/test/unit_test.hpp>
#include <stddef.h>
#include <stdint.h>
#include <boost/algorithm/string.hpp>
#include <boost/exception/to_string.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include <openssl/sha.h>


#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#include <fstream>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

using namespace mastercore;
using namespace std;
using namespace boost;

using boost::algorithm::token_compress_on;
using boost::to_string;

using leveldb::Iterator;
using leveldb::Slice;
using leveldb::Status;

using std::endl;
using std::make_pair;
using std::map;
using std::ofstream;
using std::pair;
using std::string;
using std::vector;

BOOST_FIXTURE_TEST_SUITE(omnicore_contractdex_object, BasicTestingSetup)

BOOST_AUTO_TEST_CASE(payload_simple_send)
{
    // Simple send [type 0, version 0]
    std::vector<unsigned char> vch = CreatePayload_SimpleSend(
        static_cast<uint32_t>(1),          // property: MSC
        static_cast<int64_t>(100000000));  // amount to transfer: 1.0 MSC (in willets)

    BOOST_CHECK_EQUAL(HexStr(vch), "00000000000000010000000005f5e100");

    const size_t packet_size = vch.size();
    unsigned char packet[packet_size];
    memcpy(packet, &vch[0], packet_size);

    const uint256 tx;
    CMPTransaction objTrans;
    objTrans.Set(
            "1PxejjeWZc9ZHph7A3SYDo2sk2Up4AcysH", // sender
            "1zAtHRASgdHvZDfHs6xJquMghga4eG7gy",  // receiver
            4000000,                              // nValue, nNewValue
            tx,                                   // txid
            395000,                               // block
            1,                                    // idx
            (unsigned char *) &packet,            // pkt
            packet_size,                          // pkt_size
            31,                                   // encodingClass
            32                                    // tx_fee_paid
    );

    BOOST_CHECK_EQUAL(objTrans.interpret_Transaction(), true);
    BOOST_CHECK_EQUAL(objTrans.interpretPacket(), 0);
}


BOOST_AUTO_TEST_CASE(object_checkpkt_metadex)
{
    std::vector<unsigned char> vch = CreatePayload_MetaDExTrade(
            static_cast<uint32_t>(1),    // property
            static_cast<uint64_t>(45),   // amount_forsale
            static_cast<uint32_t>(3),    // property_desired
            static_cast<uint64_t>(254)); // amount_desired

    BOOST_CHECK_EQUAL(HexStr(vch), "0000001900000001000000000000002d0000000300000000000000fe");

    const size_t packet_size = vch.size();
    unsigned char packet[packet_size];
    memcpy(packet, &vch[0], packet_size);

    const uint256 tx;
    CMPTransaction objTrans;
    objTrans.Set(
            "1PxejjeWZc9ZHph7A3SYDo2sk2Up4AcysH", // sender
            "1zAtHRASgdHvZDfHs6xJquMghga4eG7gy",  // receiver
            4000000,                              // nValue, nNewValue
            tx,                                   // txid
            395000,                               // block
            1,                                    // idx
            (unsigned char *) &packet,            // pkt
            packet_size,                          // pkt_size
            31,                                   // encodingClass
            32                                    // tx_fee_paid
    );

    BOOST_CHECK_EQUAL(objTrans.getSender(), "1PxejjeWZc9ZHph7A3SYDo2sk2Up4AcysH");
    BOOST_CHECK_EQUAL(objTrans.getReceiver(), "1zAtHRASgdHvZDfHs6xJquMghga4eG7gy");
    BOOST_CHECK_EQUAL(objTrans.getAmount(), 4000000);
    BOOST_CHECK_EQUAL(objTrans.getNewAmount(), 4000000);
    BOOST_CHECK_EQUAL(objTrans.getIndexInBlock(), 1);
    BOOST_CHECK_EQUAL(objTrans.getEncodingClass(), 31);
    BOOST_CHECK_EQUAL(objTrans.getFeePaid(), 32);

    BOOST_CHECK_EQUAL(objTrans.interpret_Transaction(), true);
    BOOST_CHECK_EQUAL(objTrans.getPayloadSize(), 28);
    BOOST_CHECK_EQUAL(objTrans.getPayload(), HexStr(vch));

    CMPMetaDEx objMetaDEx(objTrans);
    BOOST_CHECK_EQUAL(objMetaDEx.getProperty(), 1);
    BOOST_CHECK_EQUAL(objMetaDEx.getAmountForSale(), 45);
    BOOST_CHECK_EQUAL(objMetaDEx.getDesProperty(), 3);
    BOOST_CHECK_EQUAL(objMetaDEx.getAmountDesired(), 254);
}

BOOST_AUTO_TEST_CASE(test1)  // seller_amount = 10, buyer_amount = 10;
{
    CMPTally tally;  // the tally map object
    const uint256 tx;
    int64_t amount = 5;
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
                    amount,  // amount remaining
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
                    2,  // amount remaining
                    5,  // effective_price
                    1 // trading_action
    );

    CMPMetaDEx object(
                    "1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", // address
                    1,  // block
                    3,  // property for sale
                    10,  // amount of contracts for sale
                    0,  // desired property
                    0,
                    tx, // txid
                    1,  // position in block
                    1,  // subaction
                    3  // amount remaining
    );

    BOOST_CHECK_EQUAL(3, object.getAmountRemaining());
    CMPMetaDEx *pold;
    pold = &object;
    CMPMetaDEx metadex_replacement = *pold;
}

BOOST_AUTO_TEST_CASE(object_checkpkt_contractdex)
{
    //////////////////////////////////////////////

    std::vector<unsigned char> vch = CreatePayload_ContractDexTrade(
            static_cast<uint32_t>(1),   // property
            static_cast<uint64_t>(30),  // amount_forsale
            static_cast<uint32_t>(3),   // property_desired
            static_cast<uint64_t>(40),  // amount_desired
            static_cast<uint64_t>(20),  // effective_price
            static_cast<uint8_t>(1)); // trading_action

    BOOST_CHECK_EQUAL(HexStr(vch), "0000001d000000010000000000000032000000030000000000000028000000000000001401");

    const size_t packet_size = vch.size();
    unsigned char packet[packet_size];
    memcpy(packet, &vch[0], packet_size);

    for (unsigned int i = 0; i < packet_size; ++i)
    {
        int z = packet[i];
        BOOST_TEST_MESSAGE("packet[i]:" << z);
    }

    const uint256 tx;
    CMPTransaction objTrans;
    objTrans.Set(
            "1PxejjeWZc9ZHph7A3SYDo2sk2Up4AcysH", // sender
            "1zAtHRASgdHvZDfHs6xJquMghga4eG7gy",  // receiver
            4000000,                              // nValue, nNewValue
            tx,                                   // txid
            395000,                               // block
            1,                                    // idx
            (unsigned char *) &packet,            // pkt
            packet_size,                          // pkt_size
            31,                                   // encodingClass
            32                                    // tx_fee_paid
    );

    BOOST_CHECK_EQUAL(objTrans.getSender(), "1PxejjeWZc9ZHph7A3SYDo2sk2Up4AcysH");
    BOOST_CHECK_EQUAL(objTrans.getReceiver(), "1zAtHRASgdHvZDfHs6xJquMghga4eG7gy");
    BOOST_CHECK_EQUAL(objTrans.getAmount(), 4000000);
    BOOST_CHECK_EQUAL(objTrans.getNewAmount(), 4000000);
    BOOST_CHECK_EQUAL(objTrans.getIndexInBlock(), 1);
    BOOST_CHECK_EQUAL(objTrans.getEncodingClass(), 31);
    BOOST_CHECK_EQUAL(objTrans.getFeePaid(), 32);

    BOOST_CHECK_EQUAL(objTrans.interpret_Transaction(), true);
    BOOST_CHECK_EQUAL(objTrans.getPayloadSize(), 37);
    BOOST_CHECK_EQUAL(objTrans.getPayload(), HexStr(vch));

    CMPContractDex objContractDEx(objTrans);
    BOOST_CHECK_EQUAL(objContractDEx.getProperty(), 1);
    BOOST_CHECK_EQUAL(objContractDEx.getAmountForSale(), 50);
    BOOST_CHECK_EQUAL(objContractDEx.getDesProperty(), 3);
    BOOST_CHECK_EQUAL(objContractDEx.getAmountDesired(), 40);
    BOOST_CHECK_EQUAL(objContractDEx.getEffectivePrice(), 20);
    BOOST_CHECK_EQUAL(objContractDEx.getTradingAction(), 1);
    BOOST_CHECK_EQUAL(objContractDEx.getAmountRemaining(), 50);

    //////////////////////////////////////////////

    std::vector<unsigned char> vch1 = CreatePayload_ContractDexTrade(
            static_cast<uint32_t>(1),   // property
            static_cast<uint64_t>(30),  // amount_forsale
            static_cast<uint32_t>(3),   // property_desired
            static_cast<uint64_t>(20),  // amount_desired
            static_cast<uint64_t>(20),  // effective_price
            static_cast<uint8_t>(2)); // trading_action

    BOOST_CHECK_EQUAL(HexStr(vch1), "0000001d000000010000000000000032000000030000000000000028000000000000001401");

    const size_t packet_size1 = vch1.size();
    unsigned char packet1[packet_size1];
    memcpy(packet1, &vch1[0], packet_size1);

    for (unsigned int i = 0; i < packet_size1; ++i)
    {
        int z = packet1[i];
        BOOST_TEST_MESSAGE("packet1[i]:" << z);
    }

    const uint256 tx1;
    CMPTransaction objTrans1;
    objTrans1.Set(
            "1PxejjeWZc9ZHph7A3SYDo2sn2Up4AcysH", // sender
            "1zAtHRASgdHvZDfHs6xJquMgjga4eG7gy",  // receiver
            4000000,                              // nValue, nNewValue
            tx1,                                   // txid
            395000,                               // block
            1,                                    // idx
            (unsigned char *) &packet1,            // pkt
            packet_size1,                          // pkt_size
            31,                                   // encodingClass
            32                                    // tx_fee_paid
    );

    BOOST_CHECK_EQUAL(objTrans1.getSender(), "1PxejjeWZc9ZHph7A3SYDo2sk2Up4AcysH");
    BOOST_CHECK_EQUAL(objTrans1.getReceiver(), "1zAtHRASgdHvZDfHs6xJquMghga4eG7gy");
    BOOST_CHECK_EQUAL(objTrans1.getAmount(), 4000000);
    BOOST_CHECK_EQUAL(objTrans1.getNewAmount(), 4000000);
    BOOST_CHECK_EQUAL(objTrans1.getIndexInBlock(), 1);
    BOOST_CHECK_EQUAL(objTrans1.getEncodingClass(), 31);
    BOOST_CHECK_EQUAL(objTrans1.getFeePaid(), 32);

    BOOST_CHECK_EQUAL(objTrans1.interpret_Transaction(), true);
    BOOST_CHECK_EQUAL(objTrans1.getPayloadSize(), 37);
    BOOST_CHECK_EQUAL(objTrans1.getPayload(), HexStr(vch));

    CMPContractDex objContractDEx1(objTrans1);
    BOOST_CHECK_EQUAL(objContractDEx1.getProperty(), 1);
    BOOST_CHECK_EQUAL(objContractDEx1.getAmountForSale(), 50);
    BOOST_CHECK_EQUAL(objContractDEx1.getDesProperty(), 3);
    BOOST_CHECK_EQUAL(objContractDEx1.getAmountDesired(), 40);
    BOOST_CHECK_EQUAL(objContractDEx1.getEffectivePrice(), 20);
    BOOST_CHECK_EQUAL(objContractDEx1.getTradingAction(), 1);
    BOOST_CHECK_EQUAL(objContractDEx1.getAmountRemaining(), 50);

    CMPContractDex *pt_objContractDEx1;
    pt_objContractDEx1 = &objContractDEx1;

    mastercore_init();
    
    BOOST_CHECK(ContractDex_INSERT(objContractDEx));
    BOOST_CHECK_EQUAL(TRADED, x_Trade(pt_objContractDEx1));

    mastercore_shutdown();

    BOOST_CHECK_EQUAL(objTrans.interpret_Transaction(), true);
    BOOST_CHECK_EQUAL(objTrans.interpretPacket(), 29);

    //////////////////////////////////////////////
}
																																																											
bool direction = true;
																													
BOOST_AUTO_TEST_CASE(equal_amount)
{
    CMPTally tally;  // the tally map object
    CMPContractDex seller(
                    "1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", // address
                    172,  // block
                    1,  // property for sale
                    10,  // amount of contracts for sale
                    0,  // desired property
                    0,
                    uint256S("2c9a055899147b03b2c5240a020c1f94d243a834ecc06ab8cfa504ee29d07b7d"), // txid
                    1,  // position in block
                    1,  // subaction
                    0,  // amount remaining
                    5,  // effective_price
                    2 // trading_action
    );

    CMPContractDex buyer(
                    "1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", // address
                    172,  // block
                    1,  // property for sale
                    10,  // amount of contracts for trade
                    0,   // desired property
                    0,
                    uint256S("3c9a055899147b03b2c5240a030c1f94d243a834ecc06ab8cfa504ee29d07b7f"), // txid
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

    BOOST_CHECK_EQUAL(1, seller.getProperty());
    BOOST_CHECK_EQUAL("1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", seller.getAddr());
    BOOST_CHECK_EQUAL(10, seller.getAmountForSale());
    BOOST_CHECK_EQUAL(2, seller.getTradingAction());  // seller
    BOOST_CHECK_EQUAL(5, seller.getEffectivePrice());

    BOOST_CHECK_EQUAL(1, buyer.getProperty());
    BOOST_CHECK_EQUAL("1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", buyer.getAddr());
    BOOST_CHECK_EQUAL(10, buyer.getAmountForSale());
    BOOST_CHECK_EQUAL(1, buyer.getTradingAction()); //buyer
    BOOST_CHECK_EQUAL(5, buyer.getEffectivePrice());

    mastercore_init();

    // BOOST_CHECK(mastercore::update_tally_map(seller.getAddr(),seller.getProperty(),10,POSSITIVE_BALANCE));
    BOOST_CHECK(mastercore::update_tally_map(seller.getAddr(),seller.getProperty(),10, NEGATIVE_BALANCE));
    BOOST_CHECK(mastercore::update_tally_map(buyer.getAddr(),buyer.getProperty(),10, POSSITIVE_BALANCE));

    BOOST_CHECK_EQUAL(0, getMPbalance(seller.getAddr(), seller.getProperty(), POSSITIVE_BALANCE));
    BOOST_CHECK_EQUAL(10, getMPbalance(seller.getAddr(), seller.getProperty(), NEGATIVE_BALANCE));
    BOOST_CHECK_EQUAL(10, getMPbalance(buyer.getAddr(), buyer.getProperty(), POSSITIVE_BALANCE));
    BOOST_CHECK_EQUAL(0, getMPbalance(buyer.getAddr(), buyer.getProperty(), NEGATIVE_BALANCE));

    if (direction){
        BOOST_TEST_MESSAGE("The seller is inserted in priceMap, the buyer in x_Trade");
        BOOST_CHECK(ContractDex_INSERT(seller));
        BOOST_CHECK_EQUAL(TRADED, x_Trade(b));   // the buyer wants 10 contracts at  price of 5! // There's  match!!!!
    } else {
        BOOST_TEST_MESSAGE("The buyer is inserted in priceMap, the seller in x_Trade");
        BOOST_CHECK(ContractDex_INSERT(buyer));
        BOOST_CHECK_EQUAL(TRADED, x_Trade(s));
    }

    BOOST_CHECK_EQUAL(0, getMPbalance(seller.getAddr(), seller.getProperty(), POSSITIVE_BALANCE));
    BOOST_CHECK_EQUAL(20, getMPbalance(seller.getAddr(), seller.getProperty(), NEGATIVE_BALANCE));
    BOOST_CHECK_EQUAL(20, getMPbalance(buyer.getAddr(), buyer.getProperty(), POSSITIVE_BALANCE));
    BOOST_CHECK_EQUAL(0, getMPbalance(buyer.getAddr(), buyer.getProperty(), NEGATIVE_BALANCE));

    BOOST_CHECK_EQUAL(10, seller.getAmountForSale());  // seller
    BOOST_CHECK_EQUAL(0, buyer.getAmountForSale());

	t_tradelistdb->printAll();
    mastercore_shutdown();
}
////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(freed_reserve)
{
    CMPTally tally;  // the tally map object
    CMPContractDex seller(
                    "1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", // address
                    172,  // block
                    1,  // property for sale
                    10,  // amount of contracts for sale
                    0,  // desired property
                    0,
                    uint256S("2c9a055899147b03b2c5240a020c1f94d243a834ecc06ab8cfa504ee29d07b7d"), // txid
                    1,  // position in block
                    1,  // subaction
                    0,  // amount remaining
                    5,  // effective_price
                    2 // trading_action
    );

    CMPContractDex buyer(
                    "1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", // address
                    172,  // block
                    1,  // property for sale
                    10,  // amount of contracts for trade
                    0,   // desired property
                    0,
                    uint256S("3c9a055899147b03b2c5240a030c1f94d243a834ecc06ab8cfa504ee29d07b7f"), // txid
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

    BOOST_CHECK_EQUAL(1, seller.getProperty());
    BOOST_CHECK_EQUAL("1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", seller.getAddr());
    BOOST_CHECK_EQUAL(10, seller.getAmountForSale());
    BOOST_CHECK_EQUAL(2, seller.getTradingAction());  // seller
    BOOST_CHECK_EQUAL(5, seller.getEffectivePrice());

    BOOST_CHECK_EQUAL(1, buyer.getProperty());
    BOOST_CHECK_EQUAL("1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", buyer.getAddr());
    BOOST_CHECK_EQUAL(10, buyer.getAmountForSale());
    BOOST_CHECK_EQUAL(1, buyer.getTradingAction()); //buyer
    BOOST_CHECK_EQUAL(5, buyer.getEffectivePrice());

    mastercore_init();

    // BOOST_CHECK(mastercore::update_tally_map(seller.getAddr(),seller.getProperty(),10,POSSITIVE_BALANCE));
    BOOST_CHECK(mastercore::update_tally_map(seller.getAddr(),seller.getProperty(), 20, POSSITIVE_BALANCE));
    BOOST_CHECK(mastercore::update_tally_map(buyer.getAddr(),buyer.getProperty(), 20, POSSITIVE_BALANCE));
   
    //putting some money on reserve of seller and buyer
    BOOST_CHECK(mastercore::update_tally_map(seller.getAddr(),seller.getProperty(), 100000, CONTRACTDEX_RESERVE));
    BOOST_CHECK(mastercore::update_tally_map(buyer.getAddr(),buyer.getProperty(), 100000, CONTRACTDEX_RESERVE));
   
    // checking balance and reserve of seller
    BOOST_CHECK_EQUAL(0, getMPbalance(seller.getAddr(), seller.getProperty(), BALANCE));
    BOOST_CHECK_EQUAL(100000, getMPbalance(seller.getAddr(), seller.getProperty(), CONTRACTDEX_RESERVE));

    // checking balance and reserve of buyer
    BOOST_CHECK_EQUAL(0, getMPbalance(buyer.getAddr(), buyer.getProperty(), BALANCE));
    BOOST_CHECK_EQUAL(100000, getMPbalance(buyer.getAddr(), buyer.getProperty(), CONTRACTDEX_RESERVE));

    if (direction){
        BOOST_TEST_MESSAGE("The seller is inserted in priceMap, the buyer in x_Trade");
        BOOST_CHECK(ContractDex_INSERT(seller));
        BOOST_CHECK_EQUAL(TRADED, x_Trade(b));   // the buyer wants 10 contracts at  price of 5! // There's  match!!!!
    } else {
        BOOST_TEST_MESSAGE("The buyer is inserted in priceMap, the seller in x_Trade");
        BOOST_CHECK(ContractDex_INSERT(buyer));
        BOOST_CHECK_EQUAL(TRADED, x_Trade(s));
    }

    // checking balance and reserve of seller
    BOOST_CHECK_EQUAL(250, getMPbalance(seller.getAddr(), seller.getProperty(), BALANCE));
    BOOST_CHECK_EQUAL(99750, getMPbalance(seller.getAddr(), seller.getProperty(), CONTRACTDEX_RESERVE));

    // checking balance and reserve of buyer
    BOOST_CHECK_EQUAL(0, getMPbalance(buyer.getAddr(), buyer.getProperty(), BALANCE));
    BOOST_CHECK_EQUAL(100000, getMPbalance(buyer.getAddr(), buyer.getProperty(), CONTRACTDEX_RESERVE));

	t_tradelistdb->printAll();
    mastercore_shutdown();
}

////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE(balance_reserve)
{
 CMPTally tally;  // the tally map object
 const uint256 tx;
 CMPContractDex new_cdex(
         "1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", // address
         1,  // block
         3,  // property for sale
         100,  // amount of contracts for sale   (100 contracts!!!)
         0,  // desired property
         0,
         tx, // txid
         1,  // position in block
         1,  // subaction
         0,  // amount remaining
         5,  // effective_price
         2 // trading_action
 );
// Showing the balance of Address:
BOOST_CHECK_EQUAL(0,getMPbalance(new_cdex.getAddr(), new_cdex.getProperty(),BALANCE));
BOOST_CHECK_EQUAL(0,getMPbalance(new_cdex.getAddr(), new_cdex.getProperty(),CONTRACTDEX_RESERVE));

// Putting some money in balance: 10000
BOOST_CHECK(mastercore::update_tally_map(new_cdex.getAddr(), new_cdex.getProperty(),13000,BALANCE));

// Showing the balance of Address now:
BOOST_CHECK_EQUAL(13000,getMPbalance(new_cdex.getAddr(), new_cdex.getProperty(),BALANCE));
BOOST_CHECK_EQUAL(0,getMPbalance(new_cdex.getAddr(), new_cdex.getProperty(),CONTRACTDEX_RESERVE));

 // if (msc_debug_metadex3) MetaDEx_debug_print();
 x_Trade(&new_cdex);
 // if (msc_debug_metadex3) MetaDEx_debug_print();
 int64_t margin_requirement = 25; // the margin of contract
 int64_t multiplier = 100; // value of unit contract
 int64_t amount_to_reserve = (margin_requirement+multiplier)*new_cdex.getAmountForSale();
 // Insert the remaining order into the ContractDex maps
 if (0 < new_cdex.getAmountForSale()) { //switch to getAmounForSale() when ready
     if (!MetaDEx_INSERT(new_cdex)) {
         PrintToLog("%s() ERROR: ALREADY EXISTS, line %d, file: %s\n", __FUNCTION__, __LINE__, __FILE__);
        //  return METADEX_ERROR -70;
     } else {
         // move tokens into reserve, the amount of tokens is AmountForSale*margin_requirement
         assert(update_tally_map(new_cdex.getAddr(), new_cdex.getProperty(),-amount_to_reserve , BALANCE));
         assert(update_tally_map(new_cdex.getAddr(), new_cdex.getProperty(),amount_to_reserve, CONTRACTDEX_RESERVE));

     }
 }
 // Checking finally Balance & Reserve (= 125*100)
 BOOST_CHECK_EQUAL(500,getMPbalance(new_cdex.getAddr(), new_cdex.getProperty(),BALANCE));
 BOOST_CHECK_EQUAL(12500,getMPbalance(new_cdex.getAddr(), new_cdex.getProperty(),CONTRACTDEX_RESERVE));

}
////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(multiplication)
{
int64_t nBalance = 20000000000;
int64_t nNewValue = 10000000;
uint32_t marginRequirementValue = 25;
uint32_t notionalSizeValue = 100;
uint32_t sum = notionalSizeValue + marginRequirementValue;
int64_t amountToReserve = nNewValue*sum;
BOOST_CHECK_EQUAL(1250000000,amountToReserve);

if (nBalance <= (int64_t) amountToReserve) {

    BOOST_TEST_MESSAGE("rejected: sender has insufficient balance for contracts");
    BOOST_TEST_MESSAGE("nBalance: "<< nBalance);
    BOOST_TEST_MESSAGE("amountToReserve: "<< amountToReserve);
} else {


  BOOST_TEST_MESSAGE("nBalance: "<< nBalance);
  BOOST_TEST_MESSAGE("amountToReserve: "<< amountToReserve);
 }

}

////////////////////////////////////////////////////////
BOOST_AUTO_TEST_CASE(db_status)
{
  //////////////////////////////////////////////

  std::vector<unsigned char> vch = CreatePayload_ContractDexTrade(
          static_cast<uint32_t>(1),   // property
          static_cast<uint64_t>(60),  // amount_forsale
          static_cast<uint32_t>(3),   // property_desired
          static_cast<uint64_t>(40),  // amount_desired
          static_cast<uint64_t>(20),  // effective_price
          static_cast<uint8_t>(1)); // trading_action

  BOOST_CHECK_EQUAL(HexStr(vch), "0000001d000000010000000000000032000000030000000000000028000000000000001401");

  const size_t packet_size = vch.size();
  unsigned char packet[packet_size];
  memcpy(packet, &vch[0], packet_size);

  for (unsigned int i = 0; i < packet_size; ++i)
  {
      int z = packet[i];
      BOOST_TEST_MESSAGE("packet[i]:" << z);
  }

  const uint256 tx;
  CMPTransaction objTrans;
  objTrans.Set(
          "1PxejjeWZc9ZHph7A3SYDo2sk2Up4AcysH", // sender
          "1zAtHRASgdHvZDfHs6xJquMghga4eG7gy",  // receiver
          0,                              // nValue, nNewValue
          tx,                                   // txid
          395000,                               // block
          1,                                    // idx
          (unsigned char *) &packet,            // pkt
          packet_size,                          // pkt_size
          31,                                   // encodingClass
          32                                    // tx_fee_paid
  );

  BOOST_CHECK_EQUAL(objTrans.getSender(), "1PxejjeWZc9ZHph7A3SYDo2sk2Up4AcysH");
  BOOST_CHECK_EQUAL(objTrans.getReceiver(), "1zAtHRASgdHvZDfHs6xJquMghga4eG7gy");
  BOOST_CHECK_EQUAL(objTrans.getAmount(), 4000000);
  BOOST_CHECK_EQUAL(objTrans.getNewAmount(), 4000000);
  BOOST_CHECK_EQUAL(objTrans.getIndexInBlock(), 1);
  BOOST_CHECK_EQUAL(objTrans.getEncodingClass(), 31);
  BOOST_CHECK_EQUAL(objTrans.getFeePaid(), 32);

  BOOST_CHECK_EQUAL(objTrans.interpret_Transaction(), true);
  BOOST_CHECK_EQUAL(objTrans.getPayloadSize(), 37);
  BOOST_CHECK_EQUAL(objTrans.getPayload(), HexStr(vch));

  CMPContractDex objContractDEx(objTrans);
  BOOST_CHECK_EQUAL(objContractDEx.getProperty(), 1);
  BOOST_CHECK_EQUAL(objContractDEx.getAmountForSale(), 50);
  BOOST_CHECK_EQUAL(objContractDEx.getDesProperty(), 3);
  BOOST_CHECK_EQUAL(objContractDEx.getAmountDesired(), 40);
  BOOST_CHECK_EQUAL(objContractDEx.getEffectivePrice(), 20);
  BOOST_CHECK_EQUAL(objContractDEx.getTradingAction(), 1);
  BOOST_CHECK_EQUAL(objContractDEx.getAmountRemaining(), 50);

  // CMPContractDex *pt_objContractDEx;
  // pt_objContractDEx = &objContractDEx;
  // BOOST_CHECK_EQUAL(TRADED, x_Trade(pt_objContractDEx));

  //////////////////////////////////////////////

  std::vector<unsigned char> vch1 = CreatePayload_ContractDexTrade(
          static_cast<uint32_t>(1),   // property
          static_cast<uint64_t>(60),  // amount_forsale
          static_cast<uint32_t>(3),   // property_desired
          static_cast<uint64_t>(20),  // amount_desired
          static_cast<uint64_t>(20),  // effective_price
          static_cast<uint8_t>(2)); // trading_action

  BOOST_CHECK_EQUAL(HexStr(vch1), "0000001d000000010000000000000032000000030000000000000028000000000000001401");

  const size_t packet_size1 = vch1.size();
  unsigned char packet1[packet_size1];
  memcpy(packet1, &vch1[0], packet_size1);

  for (unsigned int i = 0; i < packet_size1; ++i)
  {
      int z = packet1[i];
      BOOST_TEST_MESSAGE("packet1[i]:" << z);
  }

  const uint256 tx1;
  CMPTransaction objTrans1;
  objTrans1.Set(
          "1PxejjeWZc9ZHph7A3SYDo2sn2Up4AcysH", // sender
          "1zAtHRASgdHvZDfHs6xJquMgjga4eG7gy",  // receiver
          0,                              // nValue, nNewValue
          tx1,                                   // txid
          395000,                               // block
          1,                                    // idx
          (unsigned char *) &packet1,            // pkt
          packet_size1,                          // pkt_size
          31,                                   // encodingClass
          32                                    // tx_fee_paid
  );

  BOOST_CHECK_EQUAL(objTrans1.getSender(), "1PxejjeWZc9ZHph7A3SYDo2sk2Up4AcysH");
  BOOST_CHECK_EQUAL(objTrans1.getReceiver(), "1zAtHRASgdHvZDfHs6xJquMghga4eG7gy");
  BOOST_CHECK_EQUAL(objTrans1.getAmount(), 4000000);
  BOOST_CHECK_EQUAL(objTrans1.getNewAmount(), 4000000);
  BOOST_CHECK_EQUAL(objTrans1.getIndexInBlock(), 1);
  BOOST_CHECK_EQUAL(objTrans1.getEncodingClass(), 31);
  BOOST_CHECK_EQUAL(objTrans1.getFeePaid(), 32);

  BOOST_CHECK_EQUAL(objTrans1.interpret_Transaction(), true);
  BOOST_CHECK_EQUAL(objTrans1.getPayloadSize(), 37);
  BOOST_CHECK_EQUAL(objTrans1.getPayload(), HexStr(vch));

  CMPContractDex objContractDEx1(objTrans1);
  BOOST_CHECK_EQUAL(objContractDEx1.getProperty(), 1);
  BOOST_CHECK_EQUAL(objContractDEx1.getAmountForSale(), 50);
  BOOST_CHECK_EQUAL(objContractDEx1.getDesProperty(), 3);
  BOOST_CHECK_EQUAL(objContractDEx1.getAmountDesired(), 40);
  BOOST_CHECK_EQUAL(objContractDEx1.getEffectivePrice(), 20);
  BOOST_CHECK_EQUAL(objContractDEx1.getTradingAction(), 1);
  BOOST_CHECK_EQUAL(objContractDEx1.getAmountRemaining(), 50);

  CMPContractDex *pt_objContractDEx1;
  pt_objContractDEx1 = &objContractDEx1;
  BOOST_CHECK(ContractDex_INSERT(objContractDEx));
  BOOST_CHECK_EQUAL(TRADED, x_Trade(pt_objContractDEx1));

  BOOST_CHECK_EQUAL(objContractDEx.getAmountForSale(), 0);
  BOOST_CHECK_EQUAL(objContractDEx1.getAmountForSale(), 0);


}

BOOST_AUTO_TEST_SUITE_END()
