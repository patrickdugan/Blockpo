SRCDIR=/home/lihki/.bitcoin
NUL=/dev/null
PASS=0
FAIL=0
clear
printf "Preparing a test environment...\n"
printf "   * Starting a fresh regtest daemon\n"
rm -r ~/.bitcoin/regtest
./omnicored -datadir=$SRCDIR --regtest --cleancache --startclear --daemon
sleep 3
printf "   * Preparing some mature testnet BTC\n"
./omnicore-cli -datadir=$SRCDIR --regtest -rpcwait generate 102 >$NUL
printf "   * Obtaining a master address to work with\n"
ADDR=$(./omnicore-cli -datadir=$SRCDIR --regtest getnewaddress OMNIAccount)
printf "   * Funding the address with some testnet BTC for fees\n"
echo $ADDR
TRA=$(./omnicore-cli -datadir=$SRCDIR --regtest sendtoaddress ${ADDR} 20)
./omnicore-cli -datadir=$SRCDIR --regtest generate 1 >$NUL
./omnicore-cli -datadir=$SRCDIR --regtest gettransaction $TRA 
printf "   * Participating in the Exodus crowdsale to obtain some OMNI\n"
JSON="{\"moneyqMan7uh8FqdCA2BV5yZ8qVrc9ikLP\":10,\""$ADDR"\":4}"
./omnicore-cli -datadir=$SRCDIR --regtest sendmany OMNIAccount $JSON >$NUL
./omnicore-cli -datadir=$SRCDIR --regtest generate 1 >$NUL
printf "   * Creating an indivisible test property\n"
./omnicore-cli -datadir=$SRCDIR --regtest omni_sendissuancefixed $ADDR 1 1 0 "Z_TestCat" "Z_TestSubCat" "Z_IndivisTestProperty" "Z_TestURL" "Z_TestData" 10000000 >$NUL
./omnicore-cli -datadir=$SRCDIR --regtest generate 1 >$NUL
#printf "   * Creating a divisible test property\n"
./omnicore-cli -datadir=$SRCDIR --regtest omni_sendissuancefixed $ADDR 1 2 0 "Z_TestCat" "Z_TestSubCat" "Z_DivisTestProperty" "Z_TestURL" "Z_TestData" 10000 >$NUL
./omnicore-cli -datadir=$SRCDIR --regtest generate 1 >$NUL
printf "   * Creating another indivisible test property\n"
./omnicore-cli -datadir=$SRCDIR --regtest omni_sendissuancefixed $ADDR 1 1 0 "Z_TestCat" "Z_TestSubCat" "Z_IndivisTestProperty" "Z_TestURL" "Z_TestData" 10000000 >$NUL
./omnicore-cli -datadir=$SRCDIR --regtest generate 1 >$NUL
printf "\nTesting a trade against self that uses divisible / divisible (10.0 OMNI for 100.0 #4)\n"
printf "   * Executing the trade\n"
TXID=$(./omnicore-cli -datadir=$SRCDIR --regtest omni_sendtrade $ADDR 1 10.0 4 100.0)
./omnicore-cli -datadir=$SRCDIR --regtest generate 1 >$NUL
printf "   * Verifiying the results\n"
printf "      # Checking the unit price was 10.0..."
./omnicore-cli -datadir=$SRCDIR --regtest omni_gettransaction $TXID 
printf "\nChecking the orderbook"
./omnicore-cli -datadir=$SRCDIR --regtest omni_getorderbook 1
printf "Deleting the order..."
./omnicore-cli -datadir=$SRCDIR --regtest omni_sendcancelalltrades ${ADDR} 1
./omnicore-cli -datadir=$SRCDIR --regtest generate 1 >$NUL 
printf "\nChecking the orderbook"
./omnicore-cli -datadir=$SRCDIR --regtest omni_getorderbook 1
./omnicore-cli -datadir=$SRCDIR --regtest stop