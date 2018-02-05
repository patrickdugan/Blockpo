sudo rm -r ~/.bitcoin/regtest
printf "Preparing a test environment...\n"
./omnicored --cleancache --startclear --regtest --server --daemon
sleep 3
printf "Preparing some mature testnet BTC\n"
./omnicore-cli --regtest  -rpcwait generate 101  # Es importante agregar el rpcwait que espera que el nodo e$
# ./omnicore-cli --regtest generate 101 >$NUL # generamos 200 bloques, ganando 50 BTCs
printf "\n The balance of block:\n"
./omnicore-cli --regtest  getbalance  # balance del bloque (50BTCs)
printf "   * Obtaining one address to work with\n"
ADDR=$(./omnicore-cli --regtest getnewaddress OMNIAccount)
printf "   * Funding the address with some testnet BTC for fees\n"
./omnicore-cli --regtest  $ADDR 20  # enviamos 20 BTC a ADDR
./omnicore-cli --regtest  generate 1
printf "   * Participating in the Exodus crowdsale to obtain some OMNI\n"
JSON="{\"moneyqMan7uh8FqdCA2BV5yZ8qVrc9ikLP\":10,\""$ADDR"\":8}"
./omnicore-cli --regtest  sendmany "" $JSON
./omnicore-cli --regtest  generate 1
printf "   * Creating an indivisible test property\n"
./omnicore-cli --regtest  omni_getbalance $ADDR 1
./omnicore-cli --regtest  omni_getbalance $ADDR 2
./omnicore-cli --regtest  stop
