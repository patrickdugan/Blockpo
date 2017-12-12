#include "omnicore/createpayload.h"

#include "omnicore/mdex.h"
#include "omnicore/mdex.cpp" // TODO: make x_Trade and MatchReturnType public!
#include "omnicore/omnicore.h"

#include "test/test_bitcoin.h"
#include "uint256.h"
#include "utilstrencodings.h"

#include <boost/test/unit_test.hpp>

#include <stddef.h>
#include <stdint.h>

#include <vector>
#include <string>


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
    BOOST_CHECK_EQUAL(TRADED, x_Trade(&object2)); // the buyer wants 6 contracts at price of 5! // TODO!

    BOOST_CHECK_EQUAL(10, getMPbalance(object2.getAddr(), object2.getProperty(), BALANCE)); // checking balance of sender
    BOOST_CHECK_EQUAL(10, getMPbalance(object.getAddr(), object.getProperty(), BALANCE));
}

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

BOOST_AUTO_TEST_CASE(object_checkpkt_contractdex)
{
    std::vector<unsigned char> vch = CreatePayload_ContractDexTrade(
            static_cast<uint32_t>(1),   // property
            static_cast<uint64_t>(50),  // amount_forsale
            static_cast<uint32_t>(3),   // property_desired
            static_cast<uint64_t>(40),  // amount_desired
            static_cast<uint64_t>(20),  // price_desired
            static_cast<uint64_t>(30)); // price_forsale

    BOOST_CHECK_EQUAL(HexStr(vch), "0000001d0000000100000000000000320000000300000000000000280000000000000014000000000000001e");

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
    BOOST_CHECK_EQUAL(objTrans.getPayloadSize(), 44);
    BOOST_CHECK_EQUAL(objTrans.getPayload(), HexStr(vch));

    CMPContractDex objContractDEx(objTrans);
    BOOST_CHECK_EQUAL(objContractDEx.getProperty(), 1);
    BOOST_CHECK_EQUAL(objContractDEx.getAmountForSale(), 50);
    BOOST_CHECK_EQUAL(objContractDEx.getDesProperty(), 3);
    BOOST_CHECK_EQUAL(objContractDEx.getAmountDesired(), 40);
    BOOST_CHECK_EQUAL(objContractDEx.getAmountDesired(), 40);
    BOOST_CHECK_EQUAL(objContractDEx.getDesiredPrice(), 20);
}

BOOST_AUTO_TEST_SUITE_END()
