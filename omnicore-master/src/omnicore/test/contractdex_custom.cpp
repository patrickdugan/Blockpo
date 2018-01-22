#include "omnicore/createpayload.h"

#include "omnicore/mdex.h"
#include "omnicore/omnicore.h"

#include "test/test_bitcoin.h"
#include "uint256.h"
#include "utilstrencodings.h"

#include <boost/test/unit_test.hpp>

#include <stddef.h>
#include <stdint.h>
#include <vector>
#include <string>
#include <map>
#include <univalue.h>


using namespace mastercore;
using namespace std;
using namespace boost;


BOOST_FIXTURE_TEST_SUITE(omnicore_contractdex_object, BasicTestingSetup)


BOOST_AUTO_TEST_CASE(object_checkpkt_metadex) {

    std::vector<unsigned char> vch = CreatePayload_MetaDExTrade(
            static_cast<uint32_t>(1),    // property
            static_cast<uint64_t>(45),   // amount_forsale
            static_cast<uint32_t>(3),    // property_desired
            static_cast<uint64_t>(254)); // amount_desired

    BOOST_CHECK_EQUAL(HexStr(vch), "0000001900000001000000000000002d0000000300000000000000fe");

    size_t packet_size = vch.size();
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

    /////////////////////////////////////////////

    std::vector<unsigned char> vch1 = CreatePayload_MetaDExTrade(
            static_cast<uint32_t>(1),    // property
            static_cast<uint64_t>(45),   // amount_forsale
            static_cast<uint32_t>(3),    // property_desired
            static_cast<uint64_t>(254)); // amount_desired

    BOOST_CHECK_EQUAL(HexStr(vch1), "0000001900000001000000000000002d0000000300000000000000fe");

    size_t packet_size1 = vch1.size();
    unsigned char packet1[packet_size1];
    memcpy(packet1, &vch1[0], packet_size1);

    const uint256 tx1;
    CMPTransaction objTrans1;
    objTrans1.Set(
            "1PxejjeWZc9ZHph7A3SYDo2sk2Up4AcysH", // sender
            "1zAtHRASgdHvZDfHs6xJquMghga4eG7gy",  // receiver
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
    BOOST_CHECK_EQUAL(objTrans1.getPayloadSize(), 28);
    BOOST_CHECK_EQUAL(objTrans1.getPayload(), HexStr(vch1));

    CMPMetaDEx objMetaDEx1(objTrans1);
    BOOST_CHECK_EQUAL(objMetaDEx1.getProperty(), 1);
    BOOST_CHECK_EQUAL(objMetaDEx1.getAmountForSale(), 45);
    BOOST_CHECK_EQUAL(objMetaDEx1.getDesProperty(), 3);
    BOOST_CHECK_EQUAL(objMetaDEx1.getAmountDesired(), 254);

    CMPMetaDEx *pt_objMetaDEx1;
    pt_objMetaDEx1 = &objMetaDEx1;
    BOOST_CHECK(MetaDEx_INSERT(objMetaDEx));
    BOOST_CHECK_EQUAL(TRADED, x_Trade(pt_objMetaDEx1));

}

BOOST_AUTO_TEST_CASE(test1)  // seller_amount = 10, buyer_amount = 10;
{
    CMPTally tally;  // the tally map object
    const uint256 tx;
    CMPContractDex seller(
                    "1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", // address
                    1,  // block
                    3,  // property for sale
                    10,  // amount of contracts for sale
                    1,  // desired property
                    0,
                    tx, // txid
                    1,  // position in block
                    1,  // subaction
                    5,  // amount remaining
                    2,  // effective_price
                    2 // trading_action
    );

    BOOST_CHECK_EQUAL(seller.getProperty(), 3);
    BOOST_CHECK_EQUAL(seller.getAmountForSale(), 10);
    BOOST_CHECK_EQUAL(seller.getEffectivePrice(), 5);
    BOOST_CHECK_EQUAL(seller.getTradingAction(), 2);

    CMPContractDex buyer(
                    "1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", // address
                    1,  // block
                    3,  // property for sale
                    5,  // amount of contracts for trade
                    1,   // desired property
                    0,
                    tx, // txid
                    2,  // position in block
                    1,  // subaction
                    5,  // amount remaining
                    2,  // effective_price
                    1 // trading_action
    );

    BOOST_CHECK_EQUAL(buyer.getProperty(), 3);
    BOOST_CHECK_EQUAL(buyer.getAmountForSale(), 5);
    BOOST_CHECK_EQUAL(buyer.getEffectivePrice(), 2);
    BOOST_CHECK_EQUAL(buyer.getTradingAction(), 1);


    CMPContractDex *pt_buyer;
    pt_buyer = &buyer;
    BOOST_CHECK(ContractDex_INSERT(seller));
    BOOST_CHECK_EQUAL(TRADED, x_Trade(pt_buyer));
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

    size_t packet_size = vch.size();
    unsigned char packet[packet_size];
    memcpy(packet, &vch[0], packet_size);

    for (int i = 0; i < packet_size; ++i)
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

    size_t packet_size1 = vch1.size();
    unsigned char packet1[packet_size1];
    memcpy(packet1, &vch1[0], packet_size1);

    for (int i = 0; i < packet_size1; ++i)
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
    BOOST_CHECK(ContractDex_INSERT(objContractDEx));
    BOOST_CHECK_EQUAL(TRADED, x_Trade(pt_objContractDEx1));

    //////////////////////////////////////////////
}

bool direction = true;

BOOST_AUTO_TEST_CASE(equal_amount)
{
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

    BOOST_CHECK_EQUAL(0, getMPbalance(seller.getAddr(), seller.getProperty(), POSSITIVE_BALANCE));
    BOOST_CHECK_EQUAL(0, getMPbalance(seller.getAddr(), seller.getProperty(), NEGATIVE_BALANCE));
    BOOST_CHECK_EQUAL(0, getMPbalance(buyer.getAddr(), buyer.getProperty(), POSSITIVE_BALANCE));
    BOOST_CHECK_EQUAL(0, getMPbalance(buyer.getAddr(), buyer.getProperty(), NEGATIVE_BALANCE));

    mastercore_init();

    if (direction){
        BOOST_TEST_MESSAGE("The seller is inserted in priceMap, the buyer in x_Trade");
        BOOST_CHECK(ContractDex_INSERT(seller));
        BOOST_CHECK_EQUAL(TRADED, x_Trade(b));   // the buyer wants 10 contracts at  price of 5! // There's  match!!!!
    } else {
        BOOST_TEST_MESSAGE("The buyer is inserted in priceMap, the seller in x_Trade");
        BOOST_CHECK(ContractDex_INSERT(buyer));
        BOOST_CHECK_EQUAL(TRADED, x_Trade(s));
    }

    mastercore_shutdown();
}

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

BOOST_AUTO_TEST_CASE(multiplication)
{
///////////////////////////////
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

BOOST_AUTO_TEST_CASE(PNL)
{
    string addr = "1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH";
    int64_t amountToReserve = 0;
    int64_t nBalance = 0;
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

// BOOST_AUTO_TEST_CASE(univalue_typecheck)
// {
//     UniValue v4;
//     BOOST_CHECK(v4.setNumStr("2147483648"));
//     BOOST_CHECK_EQUAL(v4.get_int64(), 2147483648);
//     BOOST_CHECK_THROW(v4.get_int(), runtime_error);
//     BOOST_CHECK_THROW(v4.get_str(), runtime_error);
//     BOOST_CHECK_EQUAL(v4.getValStr(), 2147483648);
//     BOOST_CHECK(v4.setNumStr("1000"));
//     BOOST_CHECK_EQUAL(v4.get_int(), 1000);
//     BOOST_CHECK_THROW(v4.get_str(), runtime_error);
//     BOOST_CHECK_EQUAL(v4.get_real(), 1000);
//     BOOST_CHECK_THROW(v4.get_array(), runtime_error);
//     BOOST_CHECK_THROW(v4.getKeys(), runtime_error);
//     BOOST_CHECK_THROW(v4.getValues(), runtime_error);
//     BOOST_CHECK_THROW(v4.get_obj(), runtime_error);
// }


BOOST_AUTO_TEST_SUITE_END()