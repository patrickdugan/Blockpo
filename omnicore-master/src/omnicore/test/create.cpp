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

BOOST_FIXTURE_TEST_SUITE(create_contract, BasicTestingSetup)

BOOST_AUTO_TEST_CASE(test1)
{
  // see getSpinfo rpc
    CMPSPInfo *_my_sps;
    _my_sps = new CMPSPInfo(GetDataDir() / "MP_spinfo", fReindex);
    LOCK(cs_tally);
    CMPSPInfo::Entry contract;
    // special cases for constant SPs OMNI and TOMNI
    contract.issuer = "Address";
    contract.prop_type = MSC_PROPERTY_TYPE_DIVISIBLE;
    contract.num_tokens = 700000;
    contract.category = "N/A";
    contract.subcategory = "N/A";
    contract.name = "Futures Contract";
    contract.url = "http://www.blockpo.com";
    contract.data = "Futures Contracts are now created on the Omni Layer. Don't get out of this";
    uint8_t ecosystem = 1;
    BOOST_CHECK_EQUAL(_my_sps->putSP(ecosystem, contract),3);
    _my_sps->printAll();

    for (uint32_t propertyId = 0; propertyId < 4; propertyId++) {

        BOOST_CHECK(_my_sps->getSP(propertyId, contract));

     }



}

BOOST_AUTO_TEST_SUITE_END()
