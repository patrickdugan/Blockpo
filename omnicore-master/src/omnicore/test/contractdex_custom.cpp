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

    CMPContractDex *pt_objContractDEx;
    pt_objContractDEx = &objContractDEx;
    BOOST_CHECK_EQUAL(TRADED, x_Trade(pt_objContractDEx));

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

    CMPContractDex objContractDEx1(objTrans);
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

    if (direction){
        BOOST_TEST_MESSAGE("The seller is inserted in priceMap, the buyer in x_Trade");
        BOOST_CHECK(ContractDex_INSERT(seller));
        BOOST_CHECK_EQUAL(TRADED, x_Trade(b));   // the buyer wants 10 contracts at  price of 5! // There's  match!!!!
    } else {
        BOOST_TEST_MESSAGE("The buyer is inserted in priceMap, the seller in x_Trade");
        BOOST_CHECK(ContractDex_INSERT(buyer));
        BOOST_CHECK_EQUAL(TRADED, x_Trade(s));
    }
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

 // if (msc_debug_metadex1) PrintToLog("%s(); buyer obj: %s\n", __FUNCTION__, new_cdex.ToString());

 // Ensure this is not a badly priced trade (for example due to zero amounts)
 // if (0 >= new_cdex.getEffectivePrice()) return METADEX_ERROR -66;

 // Match against existing trades, remainder of the order will be put into the order book
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

 BOOST_CHECK_EQUAL(1250000000,amountToReserve);

     BOOST_CHECK(mastercore::update_tally_map(object2.getAddr(), object2.getProperty(), 10, BALANCE)); // putting some money here
     BOOST_CHECK(mastercore::update_tally_map(object.getAddr(), object.getProperty(), 10, BALANCE));
     BOOST_CHECK_EQUAL(10, getMPbalance(object2.getAddr(), object2.getProperty(), BALANCE)); // checking balance of sender
     BOOST_CHECK_EQUAL(10, getMPbalance(object.getAddr(), object.getProperty(), BALANCE));

if (nBalance <= (int64_t) amountToReserve) {

    BOOST_TEST_MESSAGE("rejected: sender has insufficient balance for contracts");
    BOOST_TEST_MESSAGE("nBalance: "<< nBalance);
    BOOST_TEST_MESSAGE("amountToReserve: "<< amountToReserve);
} else {


  BOOST_TEST_MESSAGE("nBalance: "<< nBalance);
  BOOST_TEST_MESSAGE("amountToReserve: "<< amountToReserve);
 }

}


// BOOST_AUTO_TEST_CASE(object_default)
// {
//     const uint256 tx; // empty txid
//     CMPContractDex object2(
//             "1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", // address
//             1,  // block
//             1,  // property for sale
//             0,  // amount for sale <-- is this correct?
//             1,  // desired property
//             8,  // amount desired
//             tx, // txid
//             1,  // position in block
//             1,  // subaction
//             1,  // amount remaining
//             5,  // desired price
//             5   // for sale price
//     ); // the buyer

//     CMPContractDex object(
//             "1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", // address
//             1,  // block
//             1,  // property for sale
//             3,  // amount for sale
//             1,  // desired property
//             0,  // amount desired <-- is this correct?
//             tx, // txid
//             2,  // position in block
//             1,  // subaction
//             3,  // amount remaining
//             5,  // desired price
//             25  // for sale price
//     ); // the seller

//     BOOST_CHECK_EQUAL(1, object2.getProperty()); // buyer
//     BOOST_CHECK_EQUAL(1, object2.getDesProperty());
//     BOOST_CHECK_EQUAL("1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", object2.getAddr());
//     BOOST_CHECK_EQUAL(8, object2.getAmountDesired());
//     BOOST_CHECK_EQUAL(1, object2.getAmountRemaining());
//     BOOST_CHECK_EQUAL(5, object2.getEffectivePrice());
//     BOOST_CHECK_EQUAL(5, object2.getEffectivePrice());
//     BOOST_CHECK_EQUAL(1 , object2.getBlock());

//     BOOST_CHECK_EQUAL(1, object.getProperty()); // seller
//     BOOST_CHECK_EQUAL(1, object.getDesProperty());
//     BOOST_CHECK_EQUAL("1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", object.getAddr());
//     BOOST_CHECK_EQUAL(3, object.getAmountForSale());
//     BOOST_CHECK_EQUAL(0, object.getAmountDesired());
//     BOOST_CHECK_EQUAL(3, object.getAmountRemaining());
//     BOOST_CHECK_EQUAL(5, object.getEffectivePrice());
//     BOOST_CHECK_EQUAL(25, object.getEffectivePrice());
//     BOOST_CHECK_EQUAL(1, object.getBlock());

//     BOOST_CHECK(mastercore::update_tally_map(object2.getAddr(), object2.getProperty(), 10, BALANCE)); // putting some money here
//     BOOST_CHECK(mastercore::update_tally_map(object.getAddr(), object.getProperty(), 10, BALANCE));
//     BOOST_CHECK_EQUAL(10, getMPbalance(object2.getAddr(), object2.getProperty(), BALANCE)); // checking balance of sender
//     BOOST_CHECK_EQUAL(10, getMPbalance(object.getAddr(), object.getProperty(), BALANCE));

//     BOOST_CHECK(mastercore::MetaDEx_INSERT(object)); // the seller is inserted
//     BOOST_CHECK_EQUAL(TRADED, x_Trade(&object2)); // the buyer wants 6 contracts at price of 5! // TODO!

//     BOOST_CHECK_EQUAL(10, getMPbalance(object2.getAddr(), object2.getProperty(), BALANCE)); // checking balance of sender
//     BOOST_CHECK_EQUAL(10, getMPbalance(object.getAddr(), object.getProperty(), BALANCE));
// }

// BOOST_AUTO_TEST_CASE(equal_amount_metadex)
// {
//     CMPTally tally;  // the tally map object
//     const uint256 tx;   // address,block,property,amount for sale, desired property, amount desired,uint256 tx,idx, suba, amount remaining,desire price, for sale price
//     CMPMetaDEx seller(
//                     "1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", // address
//                     1,  // block
//                     3,  // property for sale
//                     10,  // amount of contracts for sale
//                     0,  // desired property
//                     0,
//                     tx, // txid
//                     1,  // position in block
//                     1,  // subaction
//                     0,  // amount remaining
//     );

    // CMPMetaDEx buyer(
    //                 "1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", // address
    //                 1,  // block
    //                 3,  // property for sale
    //                 10,  // amount of contracts for trade
    //                 0,   // desired property
    //                 0,
    //                 tx, // txid
    //                 2,  // position in block
    //                 1,  // subaction
    //                 0,  // amount remaining
    // );

    // CMPMetaDEx *s;
    // s = &seller;
    // CMPMetaDEx *b;
    // b = &buyer;

    // BOOST_CHECK_EQUAL(3, seller.getProperty());
    // BOOST_CHECK_EQUAL("1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", seller.getAddr());
    // BOOST_CHECK_EQUAL(10, seller.getAmountForSale());
    // BOOST_CHECK_EQUAL(2, seller.getTradingAction());  // seller
    // BOOST_CHECK_EQUAL(5, seller.getEffectivePrice());

    // BOOST_CHECK_EQUAL(3, buyer.getProperty());
    // BOOST_CHECK_EQUAL("1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", buyer.getAddr());
    // BOOST_CHECK_EQUAL(10, buyer.getAmountForSale());
    // BOOST_CHECK_EQUAL(1, buyer.getTradingAction()); //buyer
    // BOOST_CHECK_EQUAL(5, buyer.getEffectivePrice());

    // BOOST_CHECK_EQUAL(0, getMPbalance(seller.getAddr(), seller.getProperty(), POSSITIVE_BALANCE));
    // BOOST_CHECK_EQUAL(0, getMPbalance(seller.getAddr(), seller.getProperty(), NEGATIVE_BALANCE));
    // BOOST_CHECK_EQUAL(0, getMPbalance(buyer.getAddr(), buyer.getProperty(), POSSITIVE_BALANCE));
    // BOOST_CHECK_EQUAL(0, getMPbalance(buyer.getAddr(), buyer.getProperty(), NEGATIVE_BALANCE));

    // if (direction){
    //     BOOST_TEST_MESSAGE("The seller is inserted in priceMap, the buyer in x_Trade");
    //     BOOST_CHECK(MetaDEx_INSERT(seller));
    //     BOOST_CHECK_EQUAL(TRADED, x_Trade(b));   // the buyer wants 10 contracts at  price of 5! // There's  match!!!!
    // } else {
    //     BOOST_TEST_MESSAGE("The buyer is inserted in priceMap, the seller in x_Trade");
    //     BOOST_CHECK(MetaDEx_INSERT(buyer));
    //     BOOST_CHECK_EQUAL(TRADED, x_Trade(s));
    // }

    // BOOST_CHECK_EQUAL(0, getMPbalance(seller.getAddr(), seller.getProperty(),POSSITIVE_BALANCE));
    // BOOST_CHECK_EQUAL(10, getMPbalance(seller.getAddr(), seller.getProperty(),NEGATIVE_BALANCE));
    // BOOST_CHECK_EQUAL(10, getMPbalance(buyer.getAddr(), buyer.getProperty(),POSSITIVE_BALANCE));
    // BOOST_CHECK_EQUAL(0, getMPbalance(buyer.getAddr(), buyer.getProperty(),NEGATIVE_BALANCE));

// }


// BOOST_AUTO_TEST_CASE(test1)
// {
//   // see getSpinfo rpc
//     CMPSPInfo *_my_sps;
//     _my_sps = new CMPSPInfo(GetDataDir() / "MP_spinfo", fReindex);
//     LOCK(cs_tally);
//     CMPSPInfo::Entry contract;
//     contract.issuer = "Address";
//     contract.prop_type = MSC_PROPERTY_TYPE_INDIVISIBLE;
//     contract.num_tokens = 8952421;
//     contract.category = "Contracts";
//     contract.subcategory = "Futures";
//     contract.name = "Futures Contracts A";
//     contract.url = "http://www.blockpo.com";
//     contract.data = "Futures Contracts are now created on the Omni Layer. Don't get out of this";
//     contract.deadline = 88;
//     contract.blocks_until_expiration=1008;
//     contract.notional_size=100;
//     contract.collateral_currency=1;
//     contract.margin_requirement=2500;

//     uint8_t ecosystem = 1;

//     BOOST_CHECK_EQUAL(_my_sps->putSP(ecosystem, contract),3);

//     _my_sps->printAll();

//     CMPSPInfo::Entry info2;
//     for (uint32_t propertyId = 0; propertyId < 4; propertyId++) {

//         BOOST_CHECK(_my_sps->getSP(propertyId, info2));

//      }
//      CMPSPInfo::Entry info;

//      info.issuer = "Address 2";
//      info.prop_type = MSC_PROPERTY_TYPE_DIVISIBLE;
//      info.num_tokens = 999999;
//      info.category = "Contracts";
//      info.subcategory = "Futures";
//      info.name = "Futures Contracts B";
//      info.url = "http://www.blockpo.com";
//      info.data = "Futures Contracts Weekly Settlement";
//      info.blocks_until_expiration=1008;
//      info.notional_size=100;
//      info.collateral_currency=1;
//      info.margin_requirement=2500;
//      _my_sps->putSP(ecosystem,info);

//     for (uint32_t propertyId = 0; propertyId < 5; propertyId++) {

//         BOOST_CHECK(_my_sps->getSP(propertyId, contract));

//      }

//     _my_sps->printAll();
//     _my_sps->Clear();
// }

// BOOST_AUTO_TEST_CASE(match_trade)
// {
//   const uint256 tx,tx1;
//   CMPContractDex object(
//           "1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz", // address
//           1,  // block
//           3,  // property for sale
//           8,  // amount for sale
//           3,  // desired property
//           0,  // amount desired <-- is this correct?
//           tx, // txid
//           2,  // position in block
//           1,  // subaction
//           1,  // amount remaining that we don't going to use
//           5,  // desired price
//           5  // for sale price
//   ); // the seller
//   CMPContractDex object2(
//                     "1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", // address
//                     1,  // block
//                     3,  // property for sale
//                     0,  // amount for sale <-- is this correct?
//                     3,  // desired property
//                     8,  // amount desired
//                     tx1, // txid
//                     1,  // position in block
//                     1,  // subaction
//                     1,  // amount remaining that we don't going to use
//                     5,  // desired price
//                     5   // for sale price
//     ); // the buyer

//     std::string t_status = "Netted";
//     int64_t tradingFee = 0;
//     CMPContractDex *pnew, *pold;
//     pnew = &object;
//     pold = &object2;
//     CMPTradeList *t_tradelistdb1;
//     std::vector<uint256> vec;
//     vec.push_back(uint256S("2c9a055899147b03b2c5240a020c1f94d243a834ecc06ab8cfa504ee29d07b7d"));
//     vec.push_back(uint256S("3c9a055899147b03b2c5240a030c1f94d243a834ecc06ab8cfa504ee29d07b7f"));
//     t_tradelistdb1 = new CMPTradeList(GetDataDir() / "MP_tradelist", fReindex);
//     t_tradelistdb1->recordMatchedTrade(pold->getHash(), pnew->getHash(),pold->getAddr(), pnew->getAddr(), pold->getDesProperty(), pnew->getDesProperty(), pold->getAmountForSale(), pnew->getAmountDesired(), pnew->getBlock(), tradingFee, t_status,vec);
//     t_tradelistdb1->printAll();
// }

// BOOST_AUTO_TEST_SUITE_END()



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// #include "omnicore/createpayload.h"
// #include "omnicore/encoding.h"
// #include "base58.h"
// #include "coins.h"
// #include "script/script.h"
// #include "script/standard.h"
// #include "utilstrencodings.h"

// #include "omnicore/consensushash.h"
// #include "omnicore/test/utils_tx.h"
// #include "omnicore/tally.h"
// #include "omnicore/mdex.h"
// #include "omnicore/mdex.cpp"
// #include "omnicore/errors.h"
// #include "omnicore/fees.h"
// #include "omnicore/log.h"
// #include "omnicore/omnicore.h"
// #include "omnicore/rules.h"
// #include "omnicore/sp.h"
// #include "omnicore/tx.h"
// #include "omnicore/uint256_extensions.h"
// #include "omnicore/script.h"
// #include "primitives/transaction.h"
// #include "test/test_bitcoin.h"
// #include "arith_uint256.h"
// #include "chain.h"
// #include "main.h"
// #include "tinyformat.h"

// #include "arith_uint256.h"
// #include "sync.h"
// #include "uint256.h"

// #include "omnicore/convert.h"

// #include "base58.h"
// #include "coins.h"
// #include "core_io.h"
// #include "main.h"
// #include "primitives/transaction.h"
// #include "script/script.h"
// #include "script/standard.h"
// #include "test/test_bitcoin.h"
// #include "utilstrencodings.h"

// #include <univalue.h>

// #include <boost/multiprecision/cpp_int.hpp>
// #include <boost/rational.hpp>

// #include <openssl/sha.h>

// #include <assert.h>
// #include <stdint.h>

// #include <fstream>
// #include <limits>
// #include <map>
// #include <set>
// #include <string>
// #include <boost/test/unit_test.hpp>
// #include <limits>
// #include <vector>
// #include <sstream>

// #define PACKET_SIZE         31
// #define MAX_PACKETS        255
// #define PACKET_SIZE_CLASS_A 19

// /*Remember: This new function was defined to build the new class CMPContractDex*/
// namespace mastercore
// {
// 	extern std::string GenerateConsensusString(const CMPContractDex &tradeObj);
// }
// /** Creates a dummy transaction with the given inputs and outputs. */
// static CTransaction TxClassC(const std::vector<CTxOut>& txInputs, const std::vector<CTxOut>& txOuts)
// {
// 		CMutableTransaction mutableTx;

// 		// Inputs:
// 		for (std::vector<CTxOut>::const_iterator it = txInputs.begin(); it != txInputs.end(); ++it)
// 		{
// 				const CTxOut& txOut = *it;

// 				// Create transaction for input:
// 				CMutableTransaction inputTx;
// 				unsigned int nOut = 0;
// 				inputTx.vout.push_back(txOut);
// 				CTransaction tx(inputTx);

// 				// Populate transaction cache:
// 				CCoinsModifier coins = view.ModifyCoins(tx.GetHash());

// 				if (nOut >= coins->vout.size()) {
// 						coins->vout.resize(nOut+1);
// 				}
// 				coins->vout[nOut].scriptPubKey = txOut.scriptPubKey;
// 				coins->vout[nOut].nValue = txOut.nValue;

// 				// Add input:
// 				CTxIn txIn(tx.GetHash(), nOut);
// 				mutableTx.vin.push_back(txIn);
// 		}

// 		for (std::vector<CTxOut>::const_iterator it = txOuts.begin(); it != txOuts.end(); ++it)
// 		{
// 				const CTxOut& txOut = *it;
// 				mutableTx.vout.push_back(txOut);
// 		}

// 		return CTransaction(mutableTx);
// }

// /** Helper to create a CTxOut object. */
// static CTxOut createTxOut(int64_t amount, const std::string& dest)
// {
// 		return CTxOut(amount, GetScriptForDestination(CBitcoinAddress(dest).Get()));
// }


// BOOST_FIXTURE_TEST_SUITE(omnicore_contractdex_object, BasicTestingSetup)

// BOOST_AUTO_TEST_CASE(object_default)
// {

//   CMPTally tally;  // the tally map object
//   const uint256 tx;   // address,block,property,amount for sale, desired property, amount desired,uint256 tx,idx, suba, amount remaining,desire price, for sale price
//   CMPContractDex object2("1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH",1, 1, 0, 1, 8,tx, 1, 1, 1, 5, 5); //the buyer
// 	CMPContractDex object("1NNQKWM8mC35pBNPxV1noWFZEw7A5X6zXz",1, 1, 3, 1, 0,tx, 2, 1, 3, 5, 25); // the seller
// 	CMPContractDex *pobjContractDex;
// 	pobjContractDex = &object;
// 	CMPContractDex *q;
// 	q = &object2;

// 	BOOST_CHECK_EQUAL(0, object2.getProperty());  // buyer
// 	BOOST_CHECK_EQUAL(0, object2.getDesProperty());
// 	BOOST_CHECK_EQUAL("", object2.getAddr());
// 	BOOST_CHECK_EQUAL(0,object2.getAmountDesired());
// 	BOOST_CHECK_EQUAL(0,object2.getAmountRemaining());
// 	BOOST_CHECK_EQUAL(0, object2.getEffectivePrice());
// 	BOOST_CHECK_EQUAL(0, object2.getEffectivePrice());
// 	BOOST_CHECK_EQUAL(0, object2.getBlock());

//   BOOST_CHECK_EQUAL(0, pobjContractDex->getProperty());  // seller
//   BOOST_CHECK_EQUAL(0, pobjContractDex->getDesProperty());
//   BOOST_CHECK_EQUAL("", pobjContractDex->getAddr());
//   BOOST_CHECK_EQUAL(0,pobjContractDex->getAmountForSale());
//   BOOST_CHECK_EQUAL(0,pobjContractDex->getAmountDesired());
//   BOOST_CHECK_EQUAL(0,pobjContractDex->getAmountRemaining());
//   BOOST_CHECK_EQUAL(0, pobjContractDex->getEffectivePrice());
//   BOOST_CHECK_EQUAL(0, pobjContractDex->getEffectivePrice());
//   BOOST_CHECK_EQUAL(0, pobjContractDex->getBlock());

// 	BOOST_CHECK(mastercore::update_tally_map(object2.getAddr(), object2.getProperty(), 10, BALANCE));  // putting some money here
// 	BOOST_CHECK(mastercore::update_tally_map(pobjContractDex->getAddr(), pobjContractDex->getProperty(), 10, BALANCE));
// 	BOOST_CHECK_EQUAL(10,getMPbalance(object2.getAddr(), object2.getProperty(), BALANCE)); //checking balance of sender
// 	BOOST_CHECK_EQUAL(10,getMPbalance(pobjContractDex->getAddr(), pobjContractDex->getProperty(), BALANCE));

// 	BOOST_CHECK(mastercore::MetaDEx_INSERT(object)); // the seller is inserted
//   BOOST_CHECK_EQUAL(TRADED, x_Trade(q));   // the buyer wants 6 contracts at  price of 5!

// 	BOOST_CHECK_EQUAL(10,getMPbalance(object2.getAddr(), object2.getProperty(), BALANCE)); //checking balance of sender
// 	BOOST_CHECK_EQUAL(10,getMPbalance(pobjContractDex->getAddr(), pobjContractDex->getProperty(), BALANCE));

// }

// BOOST_AUTO_TEST_CASE(object_checkpkt_metadex)
// {
// 		    std::vector<unsigned char> vch = CreatePayload_MetaDExTrade(
// 		        static_cast<uint32_t>(1),          /*property*/
// 		        static_cast<uint64_t>(45),         /*amount_forsale*/
// 		        static_cast<uint32_t>(1),          /*property_desired*/
// 		        static_cast<uint64_t>(254));       /*amount_desired*/

// 		    BOOST_CHECK_EQUAL(HexStr(vch),"000000190000000100000000054c56380000002c0000000005f5e0ff");

//         std::vector<unsigned char> strScriptData = ParseHex("0000001900000001000000000000002d0000000100000000000000fe");

//         BOOST_TEST_MESSAGE("sizeof(strScriptData):" << sizeof(strScriptData));

//         int packet_size = sizeof(strScriptData) + sizeof(uint32_t);
//         for(int i = 0; i < packet_size; ++i)
// 				{
// 					int j = strScriptData[i];
//   				BOOST_TEST_MESSAGE("strScriptData[i]:" << j);
//    			}

//         unsigned char single_pkt[packet_size];
//         memcpy(single_pkt, &strScriptData[0], packet_size);

//         for (int i = 0; i < packet_size; ++i)
//         {
//           int z = single_pkt[i];
//           BOOST_TEST_MESSAGE("single_pkt[i]:" << z);
//         }

//         const uint256 tx;
//         CMPTransaction objCMPTran;
//         objCMPTran.Set("1PxejjeWZc9ZHph7A3SYDo2sk2Up4AcysH", "1zAtHRASgdHvZDfHs6xJquMghga4eG7gy", 4000000, tx, 395000, 1,
//                       (unsigned char *)&single_pkt, 33, 31, 32);

//         BOOST_CHECK_EQUAL(objCMPTran.getSender(), "1PxejjeWZc9ZHph7A3SYDo2sk2Up4AcysH");
//         BOOST_CHECK_EQUAL(objCMPTran.getReceiver(), "1zAtHRASgdHvZDfHs6xJquMghga4eG7gy");
//         BOOST_CHECK_EQUAL(objCMPTran.getAmount(), 4000000);
//         BOOST_CHECK_EQUAL(objCMPTran.getNewAmount(), 4000000);
//         BOOST_CHECK_EQUAL(objCMPTran.getIndexInBlock(), 1);
//         BOOST_CHECK_EQUAL(objCMPTran.getEncodingClass(), 31);
//         BOOST_CHECK_EQUAL(objCMPTran.getFeePaid(), 32);
//         BOOST_CHECK_EQUAL(objCMPTran.getProperty(), 1);

//         BOOST_CHECK_EQUAL(objCMPTran.interpret_Transaction(), true);
//         CMPContractDex objCMPCont(objCMPTran);
//         BOOST_CHECK_EQUAL(objCMPCont.getProperty(), 1);
//         BOOST_CHECK_EQUAL(objCMPCont.getDesProperty(), 1);
//         BOOST_CHECK_EQUAL(objCMPCont.getAmountForSale(), 45);
//         BOOST_CHECK_EQUAL(objCMPCont.getAmountDesired(), 254);
// }

// BOOST_AUTO_TEST_CASE(object_contractdex_payload)
// {
//   std::vector<unsigned char> vch = CreatePayload_ContractDexTrade(
//     static_cast<uint32_t>(1),          /*property*/
//     static_cast<uint64_t>(50),         /*amount_forsale*/
//     static_cast<uint32_t>(1),          /*property_desired*/
//     static_cast<uint64_t>(40),         /*amount_desired*/
//     static_cast<uint64_t>(20),         /*price_desired*/
//     static_cast<uint64_t>(30));        /*price_forsale*/

//     BOOST_CHECK_EQUAL(HexStr(vch),"000000190000000100000000054c56380000002c0000000005f5e0ff");
//     std::vector<unsigned char> strScriptData = ParseHex("0000001d0000000100000000000000320000000100000000000000280000000000000014000000000000001e");

//     BOOST_TEST_MESSAGE("sizeof(strScriptData):" << sizeof(strScriptData));

//     int packet_size = sizeof(strScriptData) + sizeof(uint32_t) + 2*sizeof(uint64_t);
//     for(int i = 0; i < packet_size; ++i)
//     {
//       int j = strScriptData[i];
//       BOOST_TEST_MESSAGE("strScriptData[i]:" << j);
//     }

//     unsigned char single_pkt[packet_size];
//     memcpy(single_pkt, &strScriptData[0], packet_size);

//     for (int i = 0; i < packet_size; ++i)
//     {
//       int z = single_pkt[i];
//       BOOST_TEST_MESSAGE("single_pkt[i]:" << z);
//     }

//     const uint256 tx;
//     CMPTransaction objCMPTran;
//     objCMPTran.Set("1PxejjeWZc9ZHph7A3SYDo2sk2Up4AcysH", "1zAtHRASgdHvZDfHs6xJquMghga4eG7gy", 4000000, tx, 395000, 1,
//                    (unsigned char *)&single_pkt, 33, 31, 32);

//     BOOST_CHECK_EQUAL(objCMPTran.getSender(), "1PxejjeWZc9ZHph7A3SYDo2sk2Up4AcysH");
//     BOOST_CHECK_EQUAL(objCMPTran.getReceiver(), "1zAtHRASgdHvZDfHs6xJquMghga4eG7gy");
//     BOOST_CHECK_EQUAL(objCMPTran.getAmount(), 4000000);
//     BOOST_CHECK_EQUAL(objCMPTran.getNewAmount(), 4000000);
//     BOOST_CHECK_EQUAL(objCMPTran.getIndexInBlock(), 1);
//     BOOST_CHECK_EQUAL(objCMPTran.getEncodingClass(), 31);
//     BOOST_CHECK_EQUAL(objCMPTran.getFeePaid(), 32);

//     BOOST_CHECK_EQUAL(objCMPTran.interpret_Transaction(), true);
//     CMPContractDex objCMPCont(objCMPTran);
//     BOOST_CHECK_EQUAL(objCMPCont.getProperty(), 1);
//     BOOST_CHECK_EQUAL(objCMPCont.getAmountForSale(), 50);
//     BOOST_CHECK_EQUAL(objCMPCont.getDesProperty(), 1);
//     BOOST_CHECK_EQUAL(objCMPCont.getAmountDesired(), 40);
//     BOOST_CHECK_EQUAL(objCMPCont.getEffectivePrice(), 20);

//     BOOST_TEST_MESSAGE("Size of single_pkt: " << sizeof(single_pkt));
// }

BOOST_AUTO_TEST_SUITE_END()
