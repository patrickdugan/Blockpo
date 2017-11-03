#ifndef OMNICORE_TRADECONTRACT_H
#define OMNICORE_TRADECONTRACT_H

#include "omnicore/tx.h"

#include "uint256.h"

#include <boost/lexical_cast.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/rational.hpp>

#include <openssl/sha.h>

#include <stdint.h>

#include <fstream>
#include <map>
#include <set>
#include <string>

typedef boost::rational<boost::multiprecision::checked_int128_t> rational_t;

// ContractDEx trade statuses
#define TRADE_INVALID                 -1
#define TRADE_OPEN                    1
#define TRADE_OPEN_PART_FILLED        2
#define TRADE_FILLED                  3
#define TRADE_CANCELLED               4
#define TRADE_CANCELLED_PART_FILLED   5

/** Converts price to string. */
std::string xToString(const rational_t &value);

/** A trade on the distributed exchange.*/
class CMPContractDEx 
{
	private:
		uint32_t balance
		uint32_t margin
		uint32_t propertyId;

	public:
		int32_t contractBalanceLogic() const { return balance; }
		int32_t marginLogic() const { return margin; } /*Information about the contract*/
		bool marginCall() const { return true; } 
		int cancelByPrice(const uint256 &, ...);
		int cancelByContract(const uint256 &, uint32_t, const std::string &, ...);
		int cancelAllContract(const uint256 &txid, uint32_t block, ...);

	CMPContractDEx()
      : balance(0), margin(0) propertyId(0) {}

    CMPContractDEx(int32_t n_balance, int32_t n_margin, uint32_t idx_propertyId)
      : balance(n_balance), margin(n_margin) propertyId(idx_propertyId) {}

    CMPContractDEx(const CMPContract &tx) /*CMPContractDEx is friend class from CMPContract*/
      : balance(tx.balance), margin(tx.margin) propertyId(tx.propertyId) {}


    std::string ToString() const;
    rational_t unitPrice() const;
    rational_t inversePrice() const;

    /** Used for display of unit prices to 8 decimal places at UI layer. */
    std::string displayUnitPrice() const;
    /** Used for display of unit prices with 50 decimal places at RPC layer. */
    std::string displayFullUnitPrice() const;

    void saveOffer(std::ofstream& file, SHA256_CTX* shaCtx) const;
};

namespace mastercore
{
struct ContractDEx_compare
{
	bool operator()(const CMPContractDEx &lhs, const CMPContractDEx &rhs) const;
};

// ---------------
//! Set of objects sorted by block+idx
typedef std::set<CMPContractDEx, ContractDEx_compare> md_Set; 
//! Map of prices; there is a set of sorted objects for each price
typedef std::map<rational_t, md_Set> md_PricesMap;
//! Map of properties; there is a map of prices for each property
typedef std::map<uint32_t, md_PricesMap> md_PropertiesMap;

//! Global map for price and order data
extern md_PropertiesMap ContractDEx;

// TODO: explore a property-pair, instead of a single property as map's key........
md_PricesMap* get_Prices(uint32_t prop);
md_Set* get_Indexes(md_PricesMap* p, rational_t price);
// ---------------

int ContractDEx_ADD(const std::string& sender_addr, uint32_t, int64_t, int block, uint32_t property_desired, int64_t amount_desired, const uint256& txid, unsigned int idx);
int ContractDEx_CANCEL_AT_PRICE(const uint256&, uint32_t, const std::string&, uint32_t, int64_t, uint32_t, int64_t);
int ContractDEx_CANCEL_ALL_FOR_PAIR(const uint256&, uint32_t, const std::string&, uint32_t, uint32_t);
int ContractDEx_CANCEL_EVERYTHING(const uint256& txid, uint32_t block, const std::string& sender_addr, unsigned char ecosystem);
int ContractDEx_SHUTDOWN();
int ContractDEx_SHUTDOWN_ALLPAIR();
bool ContractDEx_INSERT(const CMPContractDEx& objContractDEx);
void ContractDEx_debug_print(bool bShowPriceLevel = false, bool bDisplay = false);
bool ContractDEx_isOpen(const uint256& txid, uint32_t propertyIdForSale = 0);
int ContractDEx_getStatus(const uint256& txid, uint32_t propertyIdForSale, int64_t amountForSale, int64_t totalSold = -1);
std::string ContractDEx_getStatusText(int tradeStatus);

// Locates a trade in the ContractDEx maps via txid and returns the trade object
const CMPContractDEx* ContractDEx_RetrieveTrade(const uint256& txid);

}

#endif 