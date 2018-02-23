NUL=/dev/null
COL=2147483652
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
ADDR3=$(./omnicore-cli --regtest  getnewaddress OMNIAccount)
ADDR4=$(./omnicore-cli --regtest  getnewaddress OMNIAccount)

# printf "   * Funding the address with some testnet BTC for fees\n"
# ./omnicore-cli --regtest  sendtoaddress $ADDR 10  # enviamos 10 BTC a ADDR
# ./omnicore-cli --regtest  generate 1

printf "   * Funding the address with some testnet BTC for fees\n"
JSON="{\"moneyqMan7uh8FqdCA2BV5yZ8qVrc9ikLP\":10,\""$ADDR"\":8,\""$ADDR2"\":8,\""$ADDR3"\":8}"
./omnicore-cli --regtest sendmany "" $JSON
./omnicore-cli --regtest  generate 1

printf "   * Checking the Bitcoin balance of ADDR:\n"
./omnicore-cli --regtest  getbalance $ADDR
./omnicore-cli --regtest  getbalance $ADDR2
./omnicore-cli --regtest  getbalance $ADDR3

printf "   * Creating Future Contract 1:\n"
TRA=$(./omnicore-cli --regtest omni_createcontract $ADDR 2 3 1 "Derivaties" "Futures Contracts" "Future Contract 1" "www.tradelayer.org" "Futures Contracts Exchange on OmniLayer" 3 "1" 120 30 2 200 100 ${COL} 1)
./omnicore-cli --regtest generate 1

printf "   * Checking confirmation of transaction:\n"
./omnicore-cli --regtest omni_gettransaction $TRA

printf "  * Checking Omni listproperties:\n"
./omnicore-cli --regrest omni_listproperties

printf "   * Creating an Divisible Token:\n"   # TODO: see the indivisible/divisible troubles (amountToReserve < nBalance) in logicMath_ContractDexTrade function on tx
TRA2=$(./omnicore-cli --regtest omni_sendissuancemanaged $ADDR 2 2 0 "Tether" "Tether" "Tether" "" "")
./omnicore-cli --regtest generate 1

printf "  * Checking confirmation of transaction:\n"
./omnicore-cli --regtest omni_gettransaction $TRA2

printf "  * Checking Omni listproperties:\n"
./omnicore-cli --regrest omni_listproperties

printf "   * Sending Tokens!!!:\n"
TRA3=$(./omnicore-cli --regtest omni_sendgrant $ADDR $ADDR2 2147483652 1000000)
./omnicore-cli --regtest generate 1
TRA3=$(./omnicore-cli --regtest omni_sendgrant $ADDR $ADDR3 2147483652 1000000)
./omnicore-cli --regtest generate 1

printf "   * Checking balance of ADDR:\n"
./omnicore-cli --regtest omni_getbalance $ADDR 2147483652
printf "   * Checking balance of ADDR2:\n"
./omnicore-cli --regtest omni_getbalance $ADDR2 2147483652
printf "   * Checking balance of ADDR3:\n"
./omnicore-cli --regtest omni_getbalance $ADDR3 2147483652

printf "   * Checking All balances for the id: 2147483652\n"
./omnicore-cli --regtest omni_getallbalancesforid 2147483652

printf "   * Checking grants to addresses:\n"
./omnicore-cli --regtest omni_getgrants 2147483652

printf "   * Looking info of Contracts:\n"
./omnicore-cli --regtest omni_getproperty 2147483651

printf "* Putting some orders in orderbook :\n"
TRA4=$(./omnicore-cli --regtest omni_tradecontract $ADDR2 2147483651 3 1 1 5 1)
./omnicore-cli --regtest generate 1
TRA5=$(./omnicore-cli --regtest omni_tradecontract $ADDR3 2147483651 3 1 1 5 2)
./omnicore-cli --regtest generate 1
./omnicore-cli --regtest omni_gettransaction $TRA4
./omnicore-cli --regtest omni_gettransaction $TRA5
printf "* Checking the orderbook for Future Contract 1 (buying side):\n"
./omnicore-cli --regtest omni_getcontract_orderbook 2147483651 1

printf "* Checking the orderbook for Future Contract 1 (selling side):\n"
./omnicore-cli --regtest omni_getcontract_orderbook 2147483651 2

printf "   * Checking balance of ADDR3:\n"
./omnicore-cli --regtest omni_getcontract_balance $ADDR2 2147483652
printf "   * Checking balance of ADDR3:\n"
./omnicore-cli --regtest omni_getcontract_balance $ADDR3 2147483652

printf "   * Checking position of ADDR3:\n"
./omnicore-cli --regtest omni_getposition $ADDR2 2147483651
printf "   * Checking position of ADDR3:\n"
./omnicore-cli --regtest omni_getposition $ADDR3 2147483651

printf "   * Sending More Tokens!!!:\n"
TRA3=$(./omnicore-cli --regtest omni_sendgrant $ADDR $ADDR2 2147483652 55555)
./omnicore-cli --regtest generate 1
TRA3=$(./omnicore-cli --regtest omni_sendgrant $ADDR $ADDR3 2147483652 55555)
./omnicore-cli --regtest generate 1

printf "   * Checking balance of ADDR3:\n"
./omnicore-cli --regtest omni_getcontract_balance $ADDR2 2147483652
printf "   * Checking balance of ADDR3:\n"
./omnicore-cli --regtest omni_getcontract_balance $ADDR3 2147483652

./omnicore-cli --regtest stop
