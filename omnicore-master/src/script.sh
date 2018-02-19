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
printf "   * Funding the address with some testnet BTC for fees\n"
./omnicore-cli --regtest  sendtoaddress $ADDR 40  # enviamos 10 BTC a ADDR
./omnicore-cli --regtest  generate 1
printf "   * Participating in the Exodus crowdsale to obtain 600 OMNIs\n"
JSON="{\"moneyqMan7uh8FqdCA2BV5yZ8qVrc9ikLP\":10,\""$ADDR"\":8,\""$ADDR2"\":8}"
./omnicore-cli --regtest sendmany "" $JSON
./omnicore-cli --regtest  generate 1
./omnicore-cli --regtest omni_send $ADDR $ADDR2 1 "400"
./omnicore-cli --regtest  generate 1
printf "   * Creating Future Contract 1:\n"
TRA=$(./omnicore-cli --regtest omni_createcontract $ADDR2 2 3 1 "Derivaties" "Futures Contracts" "Future Contract 1" "www.tradelayer.org" "Futures Contracts Exchange on OmniLayer" 3 "1" 120 30 2 200 100 1 2)
./omnicore-cli  --regtest generate 1
printf "   * Checking confirmation of transaction:\n"
./omnicore-cli  --regtest omni_gettransaction $TRA
./omnicore-cli  --regrest omni_listproperties
printf "* Putting some orders in orderbook :\n"
TRA2=$(./omnicore-cli --regtest omni_tradecontract $ADDR 2147483651 10 1 1 3 1)
./omnicore-cli  --regtest omni_gettransaction $TRA2
TRA3=$(./omnicore-cli --regtest omni_tradecontract $ADDR2 2147483651 7 1 1 6 2)
./omnicore-cli   --regtest generate 1
TRA4=$(./omnicore-cli --regtest omni_tradecontract $ADDR 2147483651 9 1 1 8 1)
./omnicore-cli  --regtest generate 1
TRA5=$(./omnicore-cli --regtest omni_tradecontract $ADDR2 2147483651 2 1 1 9 2)
./omnicore-cli  --regtest generate 1
TRA6=$(./omnicore-cli --regtest omni_tradecontract $ADDR 2147483651 1 1 1 10 1)
./omnicore-cli  --regtest generate 1
TRA7=$(./omnicore-cli --regtest omni_tradecontract $ADDR2 2147483651 5 1 1 7 2)
./omnicore-cli  --regtest generate 1
printf "* Checking the orderbook for Future Contract 1 (buying side):\n"
./omnicore-cli --regtest omni_getcontract_orderbook 2147483651 1
printf "* Checking the orderbook for Future Contract 1 (selling side):\n"
./omnicore-cli --regtest omni_getcontract_orderbook 2147483651 2
printf "* Your Omni Balance is:\n"
./omnicore-cli --regtest omni_getbalance $ADDR 1
printf "* Now you can match with the orders:\n"
printf "* Your Addres is called :ADDR\n"
printf "* Now you can match with the orders:\n"
./omnicore-cli --regtest stop