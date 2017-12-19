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

BOOST_FIXTURE_TEST_SUITE(omnicore_x_trade, BasicTestingSetup)

BOOST_AUTO_TEST_CASE(test1)
{

CMPTally tally;

enum MatchReturnType
{
    NOTHING = 0,
    TRADED = 1,
    TRADED_MOREINSELLER,
    TRADED_MOREINBUYER,
    ADDED,
    CANCELLED,
};

  CMPContractDex tradeB("1PxejjeWZc9ZHph7A3SYDo2sk2Up4AcysH", 395000, 31, 1000000, 1, 2000000,
  uint256S("2c9a055899147b03b2c5240a020c1f94d243a834ecc06ab8cfa504ee29d07b7d"), 1, 1, 900000, 500000, 400000);

  BOOST_CHECK(mastercore::MetaDEx_INSERT(tradeB));

  CMPContractDex tradeC("1dexX7zmPen1yBz2H9ZF62AK5TGGqGTZH", 395000, 31, 1000000, 1, 2000000,
  uint256S("2c9a055899147b03b2c5240a020c1f94d243a834ecc06ab8cfa504ee29d07b7d"), 1, 1, 900000, 500000, 400000);

    CMPContractDex *pnew;
    pnew = &tradeC;
  //  enum MatchReturnType result;


    const uint32_t propertyForSale = pnew->getProperty();
    const uint32_t propertyDesired = pnew->getDesProperty();

    BOOST_TEST_MESSAGE("propertyForSale: " << propertyForSale);
    BOOST_TEST_MESSAGE("propertyDesired: " << propertyDesired);


    //MatchReturnType NewReturn = NOTHING;
    bool bBuyerSatisfied = false;


    md_PricesMap* const ppriceMap = get_Prices(propertyDesired);

    // nothing for the desired property exists in the market, sorry!
    if (!ppriceMap) {
          BOOST_TEST_MESSAGE("No property desired exist in the market, sorry!");
        //return NewReturn;
    }

    // within the desired property map (given one property) iterate over the items looking at prices
    for (md_PricesMap::iterator priceIt = ppriceMap->begin(); priceIt != ppriceMap->end(); ++priceIt) { // check all prices

        const uint64_t sellersPrice = priceIt->first;

        // if (msc_debug_metadex2) PrintToLog("comparing prices: desprice %s needs to be GREATER THAN OR EQUAL TO %s\n",
        //     xToString(pnew->getDesiredPrice()), xToString(sellersPrice));

        // Is the desired price check satisfied? The buyer's inverse price must be larger than that of the seller.

        /*Remember: Here we choose just "desiredCPrice() >= sellersPrice".In this case go to the next line.*/
        if (pnew->getDesiredPrice() < sellersPrice) {
            continue;
        }

        md_Set* const pofferSet = &(priceIt->second);

        // At good (single) price level and property iterate over offers looking at all parameters to find the match
        md_Set::iterator offerIt = pofferSet->begin();
        while (offerIt != pofferSet->end()) { // specific price, check all properties

            const CMPContractDex* const pold = &(*offerIt);
            /*Remeber: Here we check if price for sale = sellersPrice outside*/
            assert(pold->getForsalePrice() == sellersPrice);

            // if (msc_debug_metadex1) PrintToLog("Looking at existing: %s (its prop= %d, its des prop= %d) = %s\n",
            //     xToString(sellersPrice), pold->getProperty(), pold->getDesProperty(), pold->ToString());

            // does the desired property match?
            /*Remember: Here we make a match betewen Criptocurrencies*/
            if (pold->getDesProperty() != propertyForSale) {
                ++offerIt;
                continue;
            }

            // if (msc_debug_metadex1) PrintToLog("MATCH FOUND, Trade: %s = %s\n", xToString(sellersPrice), pold->ToString());

            // Match found, execute trade now!
            const int64_t seller_amountForSale = pold->getAmountRemaining();
            // const int64_t buyer_amountOffered = pnew->getAmountRemaining();

            // if (msc_debug_metadex1) PrintToLog("$$ trading using price: %s; seller: forsale=%d, desired=%d, remaining=%d, buyer amount offered=%d\n",
            //     xToString(sellersPrice), pold->getAmountForSale(), pold->getAmountDesired(), pold->getAmountRemaining(), pnew->getAmountRemaining());
            //
            // if (msc_debug_metadex1) PrintToLog("$$ old: %s\n", pold->ToString());
            // if (msc_debug_metadex1) PrintToLog("$$ new: %s\n", pnew->ToString());

            ///////////////////////////

            // Preconditions
            /*Remember: New preconditions for Contract*/
            assert(0 < pold->getAmountRemaining());
            assert(0 < pnew->getAmountRemaining());

            //assert(pnew->getProperty() != pnew->getDesProperty());
            assert(pnew->getProperty() == pold->getDesProperty());
            assert(pold->getProperty() == pnew->getDesProperty());

            assert(pold->getForsalePrice() <= pnew->getDesiredPrice());
            //assert(pnew->contractPrice() <= pold->desiredCPrice());

            ///////////////////////////
            /*New things for Contract: We compare the amounts desired for the buyer with the remaining of the seller*/
            int64_t nCouldBuy = 0;

            if ( pnew->getAmountDesired() < pold->getAmountRemaining() ) {
                nCouldBuy = pnew->getAmountDesired();
            } else {
                nCouldBuy = pold->getAmountRemaining();
            }

            if (nCouldBuy == 0) {
                // if (msc_debug_metadex1) PrintToLog(
                //         "-- The buyer has not enough contracts for sale!\n");
                ++offerIt;
                continue;
            }

            if (pold->getForsalePrice() > pnew->getDesiredPrice()) {
                // if (msc_debug_metadex1) PrintToLog(
                //         "-- The contract price is too expensive: %s\n", xToString(pold->getForsalePrice()));
                ++offerIt;
                continue;
            }

            /*Remember: "buyer_amountGot" Desired Amounts*/
            const int64_t buyer_amountGot = nCouldBuy;
            const int64_t seller_amountLeft = pold->getAmountRemaining() - buyer_amountGot;

            // if (msc_debug_metadex1) PrintToLog("$$ buyer_amountGot= %d, seller_amountLeft= %d\n"
            //                                         , buyer_amountGot, seller_amountLeft);

            ///////////////////////////
            // Postconditions
            assert(pold->getForsalePrice() <= pnew->getDesiredPrice());
            assert(seller_amountForSale == seller_amountLeft + buyer_amountGot);

            // Transfer the payment property from buyer to seller
            /*Remember: Here we are working just with the amount of contracts "short" or "long"*/
            assert(update_tally_map(pnew->getAddr(), pnew->getProperty(), buyer_amountGot, BALANCE));
            assert(update_tally_map(pold->getAddr(), pold->getDesProperty(), -buyer_amountGot, BALANCE));

          //  NewReturn = TRADED;

            CMPContractDex seller_replacement = *pold; // < can be moved into last if block

            seller_replacement.setAmountRemaining(seller_amountLeft, "seller_replacement");
            pnew->setAmountRemaining(buyer_amountGot, "buyer");

            if (seller_amountLeft < 0) {
              //  NewReturn = TRADED_MOREINBUYER;
            }

            if (0 == seller_amountLeft) {
                bBuyerSatisfied = true;
            }

            if (0 < seller_amountLeft) {
              //  NewReturn = TRADED_MOREINSELLER;
            }

            // if (msc_debug_metadex1) PrintToLog("==== TRADED !!! %u=%s\n", NewReturn, getTradeReturnType(NewReturn));

            // record the trade in MPTradeList
            /*Remember: We don't need for now fees*/
            int64_t tradingFee = 0;

            t_tradelistdb->recordMatchedTrade(pold->getHash(), pnew->getHash(), // < might just pass pold, pnew
                pold->getAddr(), pnew->getAddr(), pold->getDesProperty(), pnew->getDesProperty()
                , pold->getAmountForSale(), pnew->getAmountDesired(), pnew->getBlock(), tradingFee);/*Remember: Check buyer_amountGot here in omnicore.cpp*/

            if (msc_debug_metadex1) PrintToLog("++ erased old: %s\n", offerIt->ToString());
            // erase the old seller element
            pofferSet->erase(offerIt++);

            // insert the updated one in place of the old
            if (0 < seller_replacement.getAmountRemaining()) {
                PrintToLog("++ inserting seller_replacement: %s\n", seller_replacement.ToString());
                pofferSet->insert(seller_replacement);
            }
        } // specific price, check all properties
        if (bBuyerSatisfied) break;
    } // check all prices

    // PrintToLog("%s()=%d:%s\n", __FUNCTION__, NewReturn, getTradeReturnType(NewReturn));

    //return NewReturn;
}


BOOST_AUTO_TEST_SUITE_END()
