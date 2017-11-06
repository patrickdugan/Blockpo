#ifndef OMNICORE_CONTRACT_H
#define OMNICORE_CONTRACT_H

#include <stdint.h>
#include <map>

//! Balance record types
enum TallyType {
    BALANCE = 0,
    SELLOFFER_RESERVE = 1, // Ofering Omni for Bicoin
    ACCEPT_RESERVE = 2, // Accept the offer
    PENDING = 3, 
    METADEX_RESERVE = 4,

    /*New Things for Contract*/
    CONTRACT_DEX_RESERVE = 5, //Included margin for oustanding contract and reserve margin for pending orders
    REALIZED_PROFIT = 6, //Includes profit every time margin logic is call
    REALIZED_LOSSES = 7, //Includes losses every time margin logic is call
    TALLY_TYPE_COUNT
};

/*Balance records of a single entity.*/
class CMPContract
{
private:
    typedef struct {
        int64_t balance[TALLY_TYPE_COUNT];
    } BalanceRecord;

    //! Map of balance records
    typedef std::map<uint32_t, BalanceRecord> TokenMap;
    //! Balance records for different tokens
    TokenMap mp_token;
    //! Internal iterator pointing to a balance record
    TokenMap::iterator my_it;

public:
    /** Creates an empty tally. */
    CMPContract();

    /** Resets the internal iterator. */
    uint32_t init();

    /** Advances the internal iterator. */
    uint32_t next();

    /** Updates the number of tokens for the given tally type. */
    bool updateMoney(uint32_t propertyId, int64_t amount, TallyType ttype);

    /** Returns the number of tokens for the given tally type. */
    int64_t getMoney(uint32_t propertyId, TallyType ttype) const;

    /** Returns the number of available tokens. */
    int64_t getMoneyAvailable(uint32_t propertyId) const;

    /** Returns the number of reserved tokens. */
    int64_t getMoneyReserved(uint32_t propertyId) const;

    /** Compares the tally with another tally and returns true, if they are equal. */
    bool operator==(const CMPContract& rhs) const;

    /** Compares the tally with another tally and returns true, if they are not equal. */
    bool operator!=(const CMPContract& rhs) const;

    /** Prints a balance record to the console. */
    int64_t print(uint32_t propertyId = 1, bool bDivisible = true) const;
};

#endif // OMNICORE_TALLY_H