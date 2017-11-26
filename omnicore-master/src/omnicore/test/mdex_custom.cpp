#include "omnicore/test/utils_tx.h"
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

typedef boost::multiprecision::cpp_dec_float_100 dec_float;
typedef boost::multiprecision::checked_int128_t int128_t;


using namespace mastercore;

BOOST_FIXTURE_TEST_SUITE(omnicore_mdexobject, BasicTestingSetup)

BOOST_AUTO_TEST_CASE(test1)
{
        CMPTransaction x;   // transaction object 1
        const uint256 t;
        x.SetNull();
        uint64_t n1 = 20000;
        uint64_t txf1 = 2;
        unsigned int idx1= 1;
        //int64_t bt1 = 3;
        int b1 = 1;
        const std::string s1 = "WorldHello";
        const std::string r1 = "";
        unsigned int size1 = 10;
        unsigned char c1;
        unsigned char *d1;
        d1 = &c1;
        int encodingClassIn = 2;
        x.Set(s1, r1, n1, t,b1, idx1, d1, size1, encodingClassIn, txf1);

        CMPTransaction tr;  // transaction object 2
        tr.SetNull();
        uint64_t n=10000;
        uint64_t txf = 2;
        unsigned int idx= 1;
      //  int64_t bt = 3;
        int b = 1;
        const std::string s = "HelloWorld";
        const std::string r = "";
        unsigned int size = 10;
        unsigned char c;
        unsigned char *d;
        d = &c;

        //tr.Set(t,b,idx,bt);
        tr.Set(s, r, n, t,b, idx, d, size, encodingClassIn, txf);
        CMPMetaDEx object(tr);
        CMPMetaDEx object2(x);
        CMPMetaDEx *p,*q;
        q = &object2;
        p = &object; // using pointers!


        BOOST_CHECK_EQUAL(0, p->getProperty());  // testing some properties of the object
        BOOST_CHECK_EQUAL(0, p->getDesProperty());
        BOOST_CHECK_EQUAL("HelloWorld", p->getAddr());
        BOOST_CHECK_EQUAL(0,p->getAmountForSale());
        BOOST_CHECK_EQUAL(0,p->getAmountDesired());
        BOOST_CHECK_EQUAL(0,p->getAmountRemaining());



        BOOST_CHECK_EQUAL(0, q->getProperty());  // testing some properties of the object
        BOOST_CHECK_EQUAL(0, q->getDesProperty());
        BOOST_CHECK_EQUAL("WorldHello", q->getAddr());
        BOOST_CHECK_EQUAL(0,q->getAmountForSale());
        BOOST_CHECK_EQUAL(0,q->getAmountDesired());
        BOOST_CHECK_EQUAL(0,q->getAmountRemaining());


      //  BOOST_CHECK(mastercore::MetaDEx_INSERT(q));


      //  BOOST_CHECK_EQUAL(TRADED,x_Trade(p));  // is true if the match happens









}




BOOST_AUTO_TEST_SUITE_END()
