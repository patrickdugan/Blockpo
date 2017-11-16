#ifndef OMNICORE_CONTRACTDEX_H
#define OMNICORE_CONTRACTDEX_H

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

typedef uint64_t uinteger_t;

// ContractDEx trade statuses
#define TRADE_INVALID                 -1
#define TRADE_OPEN                    1 
#define TRADE_OPEN_PART_FILLED        2
#define TRADE_FILLED                  3
#define TRADE_CANCELLED               4
#define TRADE_CANCELLED_PART_FILLED   5

/** Converts price to string. */
std::string xToString(const int32_t &value);

/** A trade on the distributed exchange.*/
class CMPContractDEx 
{
    private:

        /*Rememeber: Index for block and index within block (base address of a block within the blockchain)*/
        int block;
        unsigned int idx; 
        /*Remember: These are for to know the kind of criptocurrency TMSC or MSC*/
        uint32_t property; /*->tx.cpp*/
        uint32_t desired_property; /*->tx.cpp*/
        /*Remember: These are for to know the amount of contracts for sale and desired*/ 
        int64_t amount_forsale;
        int64_t amount_desired; /*->tx.cpp*/
        /*New things for Contract: These are contract prices*/
        int64_t price_forsale;
        int64_t price_desired; 
        /*Remember: This is the remaining amount still up for sale that need to be update*/
        int64_t amount_remaining;
        /*Remember: This is the addres of the sender*/
        std::string addr;
        /*Remember: This index is used to identify 'send' and 'receive' category of transactions*/
        uint256 txid;
        /*Remember: Every private variable is defined in the input_mp_mdexorder_string(.) inside omnicore.cpp*/

	public:

        int getBlock() const { return block; }
        unsigned int getIdx() const { return idx; }
        uint32_t getProperty() const { return property; }
        uint32_t getDesProperty() const { return desired_property; }
        int64_t getAmountForSale() const { return amount_forsale; }
        int64_t getAmountDesired() const { return amount_desired; }
        int64_t getAmountRemaining() const { return amount_remaining; }
        void setAmountRemaining(int64_t nValue, const std::string &label = "");
        const std::string &getAddr() const { return addr; }
        int64_t getBlockTime() const;
        uint256 getHash() const { return txid; }

        /*Remember: Contract tx parameters are: propertyId, propertyType, amount, price - e.g. 3423, 'sell', 5, 69002*/

		CMPContractDEx()
			: block(0), idx(0), property(0) desired_property(0), amount_remaining(0), amount_forsale(0), amount_desired(0) 
            , price_forsale(0), price_desired(0) {}

        CMPContractDEx(const std::string& addr, int b, uint32_t c, uint32_t cd, const uint256 &tx, uint32_t i, 
            int64_t ar, int64_t afs, int64_t pfs, int64_t ad, int64_t pd)
            : addr(addr), block(b), property(c), desired_property(cd), txid(tx), idx(i), amount_remaining(ar)
            , amount_forsale(afs), price_forsale(pfs), amount_desired(ad), price_desired(pd) {}

        CMPContractDEx(const CMPTransaction &tx)
            : addr(tx.sender), block(tx.block), property(tx.property), desired_property(tx.desired_property)
            , txid(tx.txid), idx(tx.tx_idx), amount_remaining(tx.ar), amount_forsale(afs), price_forsale(tx.price_desired)
            , amount_desired(ad), price_desired(tx.price_desired) {}
            /*Remember: Some private variables are coming from the class CMPTransaction in tx.h and it does the copy by 
            mean its Set() funtion Set*/
    
    std::string ToString() const;
    /** Used for display of unit prices to 8 decimal places at UI layer. */
    std::string displaycontractPrice() const;
    /** Used for display of unit prices with 50 decimal places at RPC layer. */
    std::string displayFullcontractPrice() const;

    /*New things for Contract: Functions to get Contract Price and Desired Price*/
    uinteger_t contractPrice() const;
    uinteger_t desiredCPrice() const;

    void saveOffer(std::ofstream& file, SHA256_CTX* shaCtx) const;
};

namespace mastercore
{
    struct ContractDEx_compare
    {
	   bool operator()(const CMPContractDEx &lhs, const CMPContractDEx &rhs) const;
    };

    //! Set of objects sorted by block+idx
    typedef std::set<CMPContractDEx, ContractDEx_compare> md_Set; 

    //! Map of prices; there is a set of sorted objects for each price
    /*Remember: int32_t =  prices; md_Set = Set for the Contract class*/
    typedef std::map<price_forsale, md_Set> md_PricesMap;

    //! Map of properties; there is a map of prices for each property
    /*Remember: uint32_t =  property; md_PricesMap = Price Map*/
    typedef std::map<uint32_t, md_PricesMap> md_PropertiesMap;

    //! Global map for price and order data
    extern md_PropertiesMap ContractDEx;

    // TODO: explore a property-pair, instead of a single property as map's key........
    md_PricesMap* get_Prices(uint32_t prop);
    md_Set* get_Indexes(md_PricesMap* p, int32_t price);
    // ---------------

    int ContractDEx_ADD(const std::string& sender_addr, uint32_t, int64_t, int block, uint32_t property_desired, int64_t amount_desired, const uint256& txid, unsigned int idx);
    int ContractDEx_CANCEL_AT_PRICE(const uint256&, uint32_t, const std::string&, uint32_t, int64_t, uint32_t, int64_t);
    int ContractDEx_CANCEL_ALL_FOR_PAIR(const uint256&, uint32_t, const std::string&, uint32_t, uint32_t);
    int ContractDEx_CANCEL_EVERYTHING(const uint256& txid, uint32_t block, const std::string& sender_addr, unsigned char ecosystem);
    int ContractDEx_SHUTDOWN();
    int ContractDEx_SHUTDOWN_ALLPAIR();
    bool ContractDEx_INSERT(const CMPContractDEx &objContractDEx);
    void ContractDEx_debug_print(bool bShowPriceLevel = false, bool bDisplay = false);
    bool ContractDEx_isOpen(const uint256& txid, uint32_t propertyIdForSale = 0);
    int ContractDEx_getStatus(const uint256& txid, uint32_t propertyIdForSale, int64_t amountForSale, int64_t totalSold = -1);
    std::string ContractDEx_getStatusText(int tradeStatus);
    /*Added RPC-info-related functions for getTradeInfo*/

    // Locates a trade in the ContractDEx maps via txid and returns the trade object
    const CMPContractDEx* ContractDEx_RetrieveTrade(const uint256& txid); 
    /*Observe that: ContractDEx_RetrieveTrade return pointer to an object of type CMPContractDEx*/
}

   /*New things for Contract*/
    int ContractDEx_CANCEL_BY_PRICE(const uint256&, uint32_t, const std::string&, uint32_t, int64_t, uint32_t, int64_t);
    int ContractDEx_CANCEL_BY_CONTRACT(const uint256&, uint32_t, const std::string&, uint32_t, int64_t, uint32_t, int64_t);
    int ContractDEx_ALL_CONTRACT_ORDERS(const uint256&, uint32_t, const std::string&, uint32_t, int64_t, uint32_t, int64_t);
    bool ContractDEx_GET_ORDER_TYPE(const uint256& txid, const std::string&, uint32_t propertyIdForSale,  int64_t amountForSale, int64_t amount_desired);
    int ContractDEx_BALANCE_LOGIC(uint32_t propertyId, TallyType ttype);
    int ContractDEx_MARGIN_LOGIC(int ContractDExExecuted, CMPContractDEx* objCMPContractDEx, CMPContract &objCMPContract);

#endif 