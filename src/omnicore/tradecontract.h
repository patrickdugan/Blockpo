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
        int block;
        uint256 txid;
        unsigned int idx; //Index within block
        uint32_t property;
        /*New things Contract*/
        bool order_type; //Yes: Buy, No: Sell 
        
        uint32_t desired_property;
        int64_t amount_desired;
        int64_t amount_remaining;
        std::string addr;
        uint8_t subaction; 

	public:

	    uint256 getHash() const { return txid; }
        uint32_t getProperty() const { return property; }
        uint32_t getDesProperty() const { return desired_property; }
	    uint32_t getOrderType() const { return order_type; }
        int64_t getAmountForSale() const { return amount_forsale; }
        int64_t getAmountDesired() const { return amount_desired; }
        int64_t getAmountRemaining() const { return amount_remaining; }
        int64_t getAmountToFill() const;
        uint8_t getAction() const { return subaction; }
        void setAmountRemaining(int64_t nValue, const std::string &label = "");
        const std::string& getAddr() const { return addr; }
        int getBlock() const { return block; }
        unsigned int getIdx() const { return idx; }
        int64_t getBlockTime() const;

		CMPContractDEx()
			: block(0), idx(0), property(0), desired_property(0), amount_desired(0), amount_remaining(0), 
			subaction(0) {}

	    CMPContractDEx(const std::string &addr, int n_block, uint32_t n_property, int64_t nValue, 
	    	uint32_t n_desired_property, int64_t n_amount_desired, const uint256 &n_txid, uint32_t n_idx, 
	    	uint8_t n_subaction, const bool &order_type)
        	: block(n_block), txid(n_txid), idx(n_idx), property(n_property), desired_property(n_desired_property), 
        	amount_desired(n_amount_desired), amount_remaining(nValue), subaction(n_subaction), 
        	addr(addr), order_type(order_type) {}

	    CMPContractDEx(const CMPContract &tx) /*CMPContractDEx is friend class from CMPContract*/
        	: block(tx.block), txid(tx.txid), idx(tx.idx), property(tx.property), desired_property(tx.desired_property), 
        	amount_desired(tx.amount_desired), amount_remaining(tx.nValue), subaction(tx.subaction), 
        	addr(tx.sender) {}

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
/*New things for Contract*/
int ContractDEx_CANCEL_BY_PRICE(const uint256&, uint32_t, const std::string&, uint32_t, int64_t, uint32_t, int64_t);
int ContractDEx_CANCEL_BY_CONTRACT(const uint256&, uint32_t, const std::string&, uint32_t, int64_t, uint32_t, int64_t);
int ContractDEx_ALL_CONTRACT_ORDERS(const uint256&, uint32_t, const std::string&, uint32_t, int64_t, uint32_t, int64_t);

// Locates a trade in the ContractDEx maps via txid and returns the trade object
const CMPContractDEx* ContractDEx_RetrieveTrade(const uint256& txid);

}

#endif 