
 Omni Core with Futures Contracts (alpha) 
===========================================

[![Build Status](https://travis-ci.org/OmniLayer/omnicore.svg?branch=omnicore-0.0.10)](https://travis-ci.org/OmniLayer/omnicore)

What is the Omni Layer
---------------------
The Omni Layer is a communications protocol that uses the Bitcoin block chain to enable features such as smart contracts, user currencies and decentralized peer-to-peer exchanges.

http://www.omnilayer.org

What is Omni Core
---------------------

Omni Core is a fast, portable Omni Layer implementation that is based off the Bitcoin Core codebase (currently 0.13). This implementation requires no external dependencies extraneous to Bitcoin Core, and is native to the Bitcoin network just like other Bitcoin nodes. It currently supports a wallet mode and is seamlessly available on three platforms: Windows, Linux and Mac OS. Omni Layer extensions are exposed via the JSON-RPC interface. Development has been consolidated on the Omni Core product, and it is the reference client for the Omni Layer.

Futures Contracts
=====================

Notes:
 
Prototype doesn't include settlement yet, so the PNL after closing positions will be locked up in reserve until this part of the prototype is ready.
Margin call logic is also being implemented in short term.
Reserve amounts are locked until settlement.


Now we can create and trade futures contracts (by default the collateral currency is Dollars)!

*For test the new features related to futures contracts on Omni Layer you must run ./omnilayer.sh .

*If you cannot start the omnicored server with the former command you can use the ./stop.sh script to shutdown the server and run the omnilayer.sh script again.
 
*The exchange will be filled with 5 orders for each order book (BUY and SELL) to be able to play with buy and sell orders.

* Starting the script you can create a contract typing:

  - Contract name (e.g. Future Contract 1 ) 
  - Blocks until expiration (number of new bitcoin blocks valid for the contract)
  - Notional size (refers to units of the denominator of the price feed) 
  - Collateral Currency (By default use propertyid =2147483652 -> Synthetic USD ) 

* After contract are created, you can choose any of the following options :

  1 - Trade : You'll be able to Buy or Sell future contracts, the required options are the following
	  - Operation ( buy Contracts = 1, selling =2 ) 
	  - Price (price to put in the orderbook)
	  - Volume (amount of contracts traded)
	  
	  You'll see the updated orderbook in every trade. 

  2 - Get Balance : List you available balance and reserve
   
  3 - Get Positions : List the Short or Long position related to the address 
  
  4 - Get Order Book : Display the actual Buy and Sell Order Book
 
  5 - Fundings : You can put more USDs in your balance account
  
  6 - Exit : Shutdown the omnicored server and exit the script
    
  


Related projects:
-----------------

* https://github.com/OmniLayer/OmniJ

* https://github.com/OmniLayer/omniwallet

* https://github.com/OmniLayer/spec

