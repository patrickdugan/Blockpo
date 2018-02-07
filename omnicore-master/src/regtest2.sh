NUL=/dev/null

printf "////////////////////////////////\n"
printf "Cleaning the regtest folder\n"
rm -r ~/.bitcoin/regtest

printf "////////////////////////////////\n"
printf "Preparing a test environment...\n"
./omnicored -datadir=/home/lihki/.bitcoin --cleancache --startclear --regtest --server --daemon

printf "////////////////////////////////\n"
printf "Waiting three seconds to start the client...\n"
sleep 3

printf "////////////////////////////////\n"
printf "Preparing some mature regtest BTC: Mining the first 101 blocks getting 50 Bitcoins\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  -rpcwait generate 101 > $NUL # Es importante agregar el rpcwait que espera que el nodo e$

printf "////////////////////////////////\n"
printf "\n Checking the balance of block:\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  getbalance  # balance del bloque (50BTCs)

printf "////////////////////////////////\n"
printf "   * Obtaining three one address to work with\n"
ADDR=$(./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  getnewaddress OMNIAccount)
ADDR2=$(./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  getnewaddress OMNIAccount)
ADDR3=$(./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  getnewaddress OMNIAccount)

printf "////////////////////////////////\n"
printf "Printing this addresses:\n"
printf $ADDR
printf $ADDR2
printf $ADDR3

printf "////////////////////////////////\n"
printf "   * Funding the address with some testnet BTC for fees: 40 BTC to this address\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  sendtoaddress $ADDR 40  # enviamos 10 BTC a ADDR

printf "////////////////////////////////\n"
printf "Generating one block\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  generate 1

printf "////////////////////////////////\n"
printf "   * Participating in the Exodus crowdsale to obtain 1000 OMNIs: To get OMNIs in the first address ADDR\n"
JSON="{\"moneyqMan7uh8FqdCA2BV5yZ8qVrc9ikLP\":10,\""$ADDR"\":4,\""$ADDR2"\":4,\""$ADDR3"\":4}"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest sendmany "" $JSON #Sending Bitcoin to every address
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  generate 1 # Generating one block
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_send $ADDR $ADDR2 1 "200" # OMNI = Ecosystem 1
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  generate 1 # Generating one block
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_send $ADDR $ADDR3 1 "200" # OMNI = Ecosystem 1
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  generate 1 # Generating one block

printf "////////////////////////////////\n"
printf "   * Checking OMNI balances ADDR:\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getbalance $ADDR 1
printf "   * Checking OMNI balances ADDR2:\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getbalance $ADDR2 1
printf "   * Checking OMNI balances ADDR3:\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getbalance $ADDR3 1

printf "////////////////////////////////\n"
printf "   * Creating Future Contract 1:\n"
TRA=$(./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_createcontract $ADDR 2 3 1 "Derivaties" "Futures Contracts" "Future Contract 1" "www.tradelayer.org" "Futures Contracts Exchange on OmniLayer" 3 "1" 120 30 2 200 100 1 2)
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest generate 1 ## Generating one block

printf "////////////////////////////////\n"
printf "   * Checking confirmation of transaction:\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_gettransaction $TRA
./omnicore-cli -datadir=/home/lihki/.bitcoin --regrest omni_listproperties

printf "////////////////////////////////\n"
printf "   * Buying 10 contracts:\n"
TRA2=$(./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_tradecontract $ADDR 2147483651 10 1 1 3 1)
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest generate 1

printf "////////////////////////////////\n"
printf "   * Checking confirmation of 2nd transaction: confirmation: 1 and valid: true\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_gettransaction $TRA2

printf "////////////////////////////////\n"
printf "   * Checking OMNI balances Buyer:\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getbalance $ADDR 1

printf "////////////////////////////////\n"
printf "   * Selling 3 contracts:\n"
TRA3=$(./omnicore-cli --regtest omni_tradecontract $ADDR2 2147483651 3 1 1 3 2)
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest generate 1

printf "////////////////////////////////\n"
printf "   * Checking confirmation of 3nd transaction:\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_gettransaction $TRA3

printf "////////////////////////////////\n"
printf "   * Checking OMNI balances Seller:\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getbalance $ADDR 2

printf "////////////////////////////////\n"
printf "   * Checking the orderbook for Future Contract 1 (buying side):\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getcontract_orderbook 2147483651 1

printf "////////////////////////////////\n"
printf "   * Checking the orderbook for Future Contract 1 (selling side):\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getcontract_orderbook 2147483651 2

printf "////////////////////////////////\n"
printf "   * Checking OMNI balances ADDR:\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getbalance $ADDR 1

printf "////////////////////////////////\n"
printf "   * Checking OMNI balances ADDR2:\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getbalance $ADDR2 1

printf "////////////////////////////////\n"
printf "   * Checking position in Future Contract 1 for ADDR2:\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getposition $ADDR 2147483651

printf "////////////////////////////////\n"
printf "Stoping omnicored and omnicore-cli:\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest stop

# NUL=/dev/null
# rm -r ~/.bitcoin/regtest
# printf "Preparing a test environment...\n"
# ./omnicored -datadir=/home/lihki/.bitcoin --cleancache --startclear --regtest --server --daemon
# sleep 3
# printf "Preparing some mature testnet BTC\n"
# ./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  -rpcwait generate 101 > $NUL # Es importante agregar el rpcwait que espera que el nodo e$
# # ./omnicore-cli --regtest generate 101 >$NUL # generamos 200 bloques, ganando 50 BTCs
# printf "\n The balance of block:\n"
# ./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  getbalance  # balance del bloque (50BTCs)
# printf "   * Obtaining one address to work with\n"
# ADDR=$(./omnicore-cli --regtest  getnewaddress OMNIAccount)
# printf "Address OMNIAccoun\n"
# printf $ADDR 
# printf "   * Funding the address with some testnet BTC for fees\n"
# ./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  sendtoaddress $ADDR 20  # enviamos 20 BTC a ADDR
# ./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  generate 1
# printf "   * Participating in the Exodus crowdsale to obtain some OMNI\n"
# JSON="{\"moneyqMan7uh8FqdCA2BV5yZ8qVrc9ikLP\":10,\""$ADDR"\":8}"
# ./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest sendmany "" $JSON
# ./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest generate 1
# printf "   * Creating Future Contract 1:\n"
# TRA=$(./omnicore-cli --regtest omni_createcontract $ADDR 2 3 1 "Derivaties" "Futures Contracts" "Future Contract 1" "www.tradelayer.org" "Futures Contracts Exchange on OmniLayer" 3 "1" 120 30 2 200 10 1 2)
# ./omnicore-cli --regtest generate 1
# printf "   * Checking confirmation of transaction:\n"
# ./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_gettransaction $TRA
# ./omnicore-cli -datadir=/home/lihki/.bitcoin --regrest omni_listproperties
# printf "   * Checking OMNI balances:\n"
# ./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getbalance $ADDR 1
# ./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getbalance $ADDR 2
# printf "   * Buying 10 contracts:\n"
# TRA2=$(./omnicore-cli --regtest omni_tradecontract $ADDR 2147483651 10 1 1 3 1)
# ./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest generate 1
# printf "   * Checking confirmation of 2nd transaction:\n"
# ./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_gettransaction $TRA2
# printf "   * Checking the orderbook for Future Contract 1:\n"
# ./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getcontract_orderbook 2147483651 1
# printf "   * Selling 3 contracts:\n"
# TRA3=$(./omnicore-cli --regtest omni_tradecontract $ADDR 2147483651 3 1 1 3 2)
# ./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest generate 1
# printf "   * Checking confirmation of 3nd transaction:\n"
# ./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_gettransaction $TRA3
# printf "   * Checking the orderbook for Future Contract 1:\n"
# ./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getcontract_orderbook 2147483651 2
# ./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest stop