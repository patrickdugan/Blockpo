NUL=/dev/null
rm -r ~/.bitcoin/regtest
printf "Preparing a test environment...\n"
./omnicored --cleancache --startclear --regtest --server --daemon
sleep 3
printf "Preparing some mature regtest BTC\n"
./omnicore-cli --regtest  -rpcwait generate 101 > $NUL # Es importante agregar el rpcwait que espera que el nodo e$
# ./omnicore-cli --regtest generate 101 >$NUL # generamos 200 bloques, ganando 50 BTCs
printf "\n The balance of block:\n"
./omnicore-cli --regtest  getbalance  # balance del bloque (50BTCs)
printf "   * Obtaining one address to work with\n"
ADDR=$(./omnicore-cli --regtest  getnewaddress OMNIAccount)
ADDR2=$(./omnicore-cli --regtest  getnewaddress OMNIAccount)
printf $ADDR
printf $ADDR2
printf "   * Funding the address with some testnet BTC for fees\n"
./omnicore-cli --regtest  sendtoaddress $ADDR 20  # enviamos 10 BTC a ADDR
./omnicore-cli --regtest  generate 1
printf "   * Participating in the Exodus crowdsale to obtain 1000 OMNIs\n"
JSON="{\"moneyqMan7uh8FqdCA2BV5yZ8qVrc9ikLP\":10,\""$ADDR"\":4,\""$ADDR2"\":4}"
./omnicore-cli --regtest sendmany "" $JSON
./omnicore-cli --regtest  generate 1
./omnicore-cli --regtest omni_send $ADDR $ADDR2 1 "400"
./omnicore-cli --regtest  generate 1
printf "   * Checking OMNI balances ADDR:\n"
./omnicore-cli --regtest omni_getbalance $ADDR 1
printf "   * Checking OMNI balances ADDR2:\n"
./omnicore-cli --regtest omni_getbalance $ADDR2 1
printf "   * Creating Future Contract 1:\n"
TRA=$(./omnicore-cli --regtest omni_createcontract $ADDR2 2 3 1 "Derivaties" "Futures Contracts" "Future Contract 1" "www.tradelayer.org" "Futures Contracts Exchange on OmniLayer" 3 "1" 120 30 2 200 100 1 2)
./omnicore-cli --regtest generate 1
printf "   * Checking confirmation of transaction:\n"
./omnicore-cli --regtest omni_gettransaction $TRA
./omnicore-cli --regrest omni_listproperties
printf "   * Buying 10 contracts:\n"
TRA2=$(./omnicore-cli --regtest omni_tradecontract $ADDR 2147483651 10.0 1 1 3 1)
./omnicore-cli --regtest generate 1
printf "   * Checking confirmation of 2nd transaction:\n"
./omnicore-cli --regtest omni_gettransaction $TRA2
printf "   * Checking OMNI balances:\n"
./omnicore-cli --regtest omni_getbalance $ADDR 1
printf "   * Selling 3 contracts:\n"
TRA3=$(./omnicore-cli --regtest omni_tradecontract $ADDR2 2147483651 3.0 1 1 3 2)
./omnicore-cli --regtest generate 1
printf "   * Checking confirmation of 3nd transaction:\n"
./omnicore-cli --regtest omni_gettransaction $TRA3
printf "   * Checking the orderbook for Future Contract 1 (buying side):\n"
./omnicore-cli --regtest omni_getcontract_orderbook 2147483651 1
printf "   * Checking the orderbook for Future Contract 1 (selling side):\n"
./omnicore-cli --regtest omni_getcontract_orderbook 2147483651 2
printf "   * Checking OMNI balances ADDR:\n"
./omnicore-cli --regtest omni_getbalance $ADDR 1
printf "   * Checking OMNI balances ADDR2:\n"
./omnicore-cli --regtest omni_getbalance $ADDR2 1
printf "   * Checking position of ADDR:\n"
./omnicore-cli --regtest omni_getposition $ADDR 2147483651
printf "   * Checking position of ADDR2:\n"
./omnicore-cli --regtest omni_getposition $ADDR2 2147483651
./omnicore-cli --regtest stop
