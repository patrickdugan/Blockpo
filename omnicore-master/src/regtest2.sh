NUL=/dev/null
rm -r ~/.bitcoin/regtest
printf "Preparing a test environment...\n"
./omnicored -datadir=/home/lihki/.bitcoin --cleancache --startclear --regtest --server --daemon
sleep 3
printf "Preparing some mature regtest BTC\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  -rpcwait generate 101 > $NUL # Es importante agregar el rpcwait que espera que el nodo e$
# ./omnicore-cli --regtest generate 101 >$NUL # generamos 200 bloques, ganando 50 BTCs
printf "\n The balance of block:\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  getbalance  # balance del bloque (50BTCs)
printf "   * Obtaining one address to work with\n"
ADDR=$(./omnicore-cli --regtest  getnewaddress OMNIAccount)
ADDR2=$(./omnicore-cli --regtest  getnewaddress OMNIAccount)
ADDR3=$(./omnicore-cli --regtest  getnewaddress OMNIAccount)
printf $ADDR
printf $ADDR2
printf $ADDR3
printf "   * Funding the address with some testnet BTC for fees\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  sendtoaddress $ADDR 40  # enviamos 10 BTC a ADDR
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  generate 1
printf "   * Participating in the Exodus crowdsale to obtain 1000 OMNIs\n"
JSON="{\"moneyqMan7uh8FqdCA2BV5yZ8qVrc9ikLP\":10,\""$ADDR"\":4,\""$ADDR2"\":4,\""$ADDR3"\":4}"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest sendmany "" $JSON
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  generate 1
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_send $ADDR $ADDR2 1 "200"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  generate 1
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_send $ADDR $ADDR3 1 "200"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  generate 1
printf "   * Checking OMNI balances ADDR:\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getbalance $ADDR 1
printf "   * Checking OMNI balances ADDR2:\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getbalance $ADDR2 1
printf "   * Checking OMNI balances ADDR3:\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getbalance $ADDR3 1

printf "   * Creating Future Contract 1:\n"
TRA=$(./omnicore-cli --regtest omni_createcontract $ADDR 2 3 1 "Derivaties" "Futures Contracts" "Future Contract 1" "www.tradelayer.org" "Futures Contracts Exchange on OmniLayer" 3 "1" 120 30 2 200 100 1 2)
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest generate 1
printf "   * Checking confirmation of transaction:\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_gettransaction $TRA
./omnicore-cli -datadir=/home/lihki/.bitcoin --regrest omni_listproperties

printf "   * Buying 10 contracts:\n"
TRA2=$(./omnicore-cli --regtest omni_tradecontract $ADDR 2147483651 10 1 1 3 1)
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest generate 1
printf "   * Checking confirmation of 2nd transaction:\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_gettransaction $TRA2
printf "   * Checking OMNI balances Buyer:\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getbalance $ADDR 1

printf "   * Selling 3 contracts:\n"
TRA3=$(./omnicore-cli --regtest omni_tradecontract $ADDR2 2147483651 3 1 1 3 2)
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest generate 1
printf "   * Checking confirmation of 3nd transaction:\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_gettransaction $TRA3
printf "   * Checking OMNI balances Seller:\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getbalance $ADDR 2

# printf "   * Selling 4 contracts:\n"
# TRA4=$(./omnicore-cli --regtest omni_tradecontract $ADDR2 2147483651 4 1 1 4 2)
# ./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest generate 1
# printf "   * Checking confirmation of 3nd transaction:\n"
# ./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_gettransaction $TRA4

printf "   * Checking the orderbook for Future Contract 1 (buying side):\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getcontract_orderbook 2147483651 1
printf "   * Checking the orderbook for Future Contract 1 (selling side):\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getcontract_orderbook 2147483651 2

printf "   * Checking OMNI balances ADDR:\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getbalance $ADDR 1
printf "   * Checking OMNI balances ADDR2:\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getbalance $ADDR2 1

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
