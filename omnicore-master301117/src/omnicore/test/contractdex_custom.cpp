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
#include "test/test_bitcoin.h"
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

BOOST_AUTO_TEST_CASE(object_matching)
{
		// CMPTransaction objCMPTransactionB;
		// objCMPTransactionB.Set("1PxejjeWZc9ZHph7A3SYDo2sk2Up4AcysH", "1zAtHRASgdHvZDfHs6xJquMghga4eG7gy", 4000000, 
		// 	uint256S("2c9a055899147b03b2c5240a020c1f94d243a834ecc06ab8cfa504ee29d07b7d"), 395000, 1, 
		// 	unsigned char *p, 33, 1, 0)

		/*Remember: Here amount_desired = 0 "seller"*/
		CMPContractDex tradeB("1PxejjeWZc9ZHph7A3SYDo2sk2Up4AcysH", 395000, 31, 4000000, 1, 0,
        uint256S("2c9a055899147b03b2c5240a020c1f94d243a834ecc06ab8cfa504ee29d07b7d"), 1, 1, 2000000, 0, 100000);
    	BOOST_CHECK_EQUAL("2c9a055899147b03b2c5240a020c1f94d243a834ecc06ab8cfa504ee29d07b7d|1PxejjeWZc9ZHph7A3SYDo2sk2Up4AcysH|31|4000000|1|0|2000000|0|100000|",
        GenerateConsensusString(tradeB));
		CMPContractDex *pobjContractDexB;        
		pobjContractDexB = &tradeB;

		/*Remember: Here amount_forsale = 0 "buyer"*/
		CMPContractDex tradeC("1zAtHRASgdHvZDfHs6xJquMghga4eG7gy", 395000, 31, 0, 1, 2000000,
        uint256S("2c9a055899147b03b2c5240a020c1f94d243a834ecc06ab8cfa504ee29d07b7d"), 1, 1, 0, 100000, 0);
    	BOOST_CHECK_EQUAL("2c9a055899147b03b2c5240a020c1f94d243a834ecc06ab8cfa504ee29d07b7d|1zAtHRASgdHvZDfHs6xJquMghga4eG7gy|31|0|1|2000000|0|100000|0|",
        GenerateConsensusString(tradeC));
		CMPContractDex *pobjContractDexC;        
		pobjContractDexC = &tradeC;

		/*Seller*/
        BOOST_CHECK_EQUAL("1PxejjeWZc9ZHph7A3SYDo2sk2Up4AcysH", pobjContractDexB->getAddr());  
        BOOST_CHECK_EQUAL(395000, pobjContractDexB->getBlock());
        BOOST_CHECK_EQUAL(31, pobjContractDexB->getProperty());
        BOOST_CHECK_EQUAL(4000000, pobjContractDexB->getAmountForSale());
        BOOST_CHECK_EQUAL(1, pobjContractDexB->getDesProperty());
        BOOST_CHECK_EQUAL(0, pobjContractDexB->getAmountDesired());
		BOOST_CHECK_EQUAL(1, pobjContractDexB->getIdx());
		BOOST_CHECK_EQUAL(1, pobjContractDexB->getAction());
		BOOST_CHECK_EQUAL(2000000, pobjContractDexB->getAmountRemaining());
		BOOST_CHECK_EQUAL(0, pobjContractDexB->getDesiredPrice());
		BOOST_CHECK_EQUAL(100000, pobjContractDexB->getForsalePrice());

		/*Buyer*/
		BOOST_CHECK_EQUAL("1zAtHRASgdHvZDfHs6xJquMghga4eG7gy", pobjContractDexC->getAddr());  
        BOOST_CHECK_EQUAL(395000, pobjContractDexC->getBlock());
        BOOST_CHECK_EQUAL(31, pobjContractDexC->getProperty());
        BOOST_CHECK_EQUAL(0, pobjContractDexC->getAmountForSale());
        BOOST_CHECK_EQUAL(1, pobjContractDexC->getDesProperty());
        BOOST_CHECK_EQUAL(2000000, pobjContractDexC->getAmountDesired());
		BOOST_CHECK_EQUAL(1, pobjContractDexC->getIdx());
		BOOST_CHECK_EQUAL(1, pobjContractDexC->getAction());
		BOOST_CHECK_EQUAL(0, pobjContractDexC->getAmountRemaining());
		BOOST_CHECK_EQUAL(100000, pobjContractDexC->getDesiredPrice());
		BOOST_CHECK_EQUAL(0, pobjContractDexC->getForsalePrice());


		BOOST_CHECK(mastercore::MetaDEx_INSERT(tradeB));
        BOOST_CHECK_EQUAL(NOTHING, x_Trade(pobjContractDexC));
}


BOOST_AUTO_TEST_SUITE_END()