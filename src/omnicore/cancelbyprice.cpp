#include <string>
#include <cmath>
#include "omnicore/omnicore.h"  // getbalance function and others
using namespace mastercore;

int cancelByPrice(const uint256& txid, unsigned int block, const std::string& sender_addr, uint32_t prop, int64_t amount, uint32_t property_desired, int64_t amount_desired)
{
    int rc = METADEX_ERROR -20;
    CMPcontractDex cdex(sender_addr, 0, prop, amount, property_desired, amount_desired, uint256(), 0, CMPTransaction::CANCEL_AT_PRICE); // inicializamos un objeto CMPcontractDex
    md_PricesMap* prices = get_Prices(prop);
    const CMPcontractDex* p_mdex = NULL;
  //  if (msc_debug_metadex1) PrintToLog("%s():%s\n", __FUNCTION__, mdex.ToString());

  //  if (msc_debug_metadex2) MetaDEx_debug_print();
    if (!prices) {
        PrintToLog("%s() NOTHING FOUND for %s\n", __FUNCTION__, cdex.ToString());
        return rc -1;
    }

    // within the desired property map (given one property) iterate over the items
    for (md_PricesMap::iterator my_it = prices->begin(); my_it != prices->end(); ++my_it) {
        rational_t sellers_price = my_it->first;

        if (cdex.unitPrice() != sellers_price) continue;

        md_Set* indexes = &(my_it->second);

        for (md_Set::iterator iitt = indexes->begin(); iitt != indexes->end();) {
            p_cdex = &(*iitt);

            if (msc_debug_metadex3) PrintToLog("%s(): %s\n", __FUNCTION__, p_cdex->ToString());

            if ((p_cdex->getDesProperty() != property_desired) || (p_cdex->getAddr() != sender_addr)) {
                ++iitt;
                continue;
            }

            rc = 0;
            PrintToLog("%s(): REMOVING %s\n", __FUNCTION__, p_cdex->ToString());

            // move from reserve to main
            assert(update_tally_map(p_cdex->getAddr(), p_cdex->getProperty(), -p_cdex->getAmountRemaining(), METADEX_RESERVE));
            assert(update_tally_map(p_cdex->getAddr(), p_cdex->getProperty(), p_cdex->getAmountRemaining(), BALANCE));

            // record the cancellation
            bool bValid = true;
            p_txlistdb->recordcontractDExCancelTX(txid, p_cdex->getHash(), bValid, block, p_cdex->getProperty(), p_cdex->getAmountRemaining());

            indexes->erase(iitt++);
        }
    }

   //if (msc_debug_metadex2) MetaDEx_debug_print();
    return rc;
}
