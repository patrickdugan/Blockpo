SRC=/home/ale/Desktop/BlockPo/omnicore-bin/omnicore-master/src
DATADIR=/home/ale/.bitcoin
CONT=2147483651
COL=2147483652
AMOUNT=1000
bitcoins=5
rm -r -f $DATADIR/regtest
# $SRC/omnicore-cli --regtest --datadir=$DATADIR stop &>/dev/null
# sleep 1
printf "Preparing a test environment...\n"
$SRC/omnicored --cleancache --startclear --regtest --server --daemon
$SRC/omnicore-cli -datadir=$DATADIR --regtest  -rpcwait generate 101 >/dev/null
printf "Creating some addresses ...\n"
ADDR=$($SRC/omnicore-cli -datadir=$DATADIR --regtest  getnewaddress OMNIAccount)
ADDR2=$($SRC/omnicore-cli -datadir=$DATADIR --regtest  getnewaddress OMNIAccount)
printf "${ADDR}\n"
printf "${ADDR2}\n"
JSON="{\""${ADDR}"\":${bitcoins},\""${ADDR2}"\":${bitcoins}}"
#Sending bitcoins to all
$SRC/omnicore-cli -datadir=$DATADIR --regtest sendmany "" $JSON > /dev/null
$SRC/omnicore-cli -datadir=$DATADIR --regtest  generate 1 > /dev/null

# Default inputs in create contract RPC
par1="Future Contract 1"
par2=1
par3=10 # Notional Size
par5=2 #Margin Requirement
#Creo el futuro
T=$($SRC/omnicore-cli -datadir=$DATADIR --regtest omni_createcontract ${ADDR} 2 3 1 "Derivaties" "Futures Contracts" "$par1" "www.tradelayer.org" "Futures Contracts Exchange on OmniLayer" 3 "1" 120 30 2 $par2 $par3 $COL $par5)
$SRC/omnicore-cli -datadir=$DATADIR --regtest generate 1 >/dev/null

# Creating Divisible tokens ( Synthetic USD)
#printf "Creating an Divisible Token:\n"   # TODO: see the indivisible/divisible troubles (amountToReserve < nBalance) in logicMath_Contra$
$SRC/omnicore-cli --regtest omni_sendissuancemanaged ${ADDR} 2 2 0 "Tether" "Tether" "Tether" "" "" >/dev/null
$SRC/omnicore-cli --regtest generate 1 >/dev/null
#$SRC/omnicore-cli --regtest omni_gettransaction $TRA9

#Sending synthetic USDs to all
$SRC/omnicore-cli -datadir=$DATADIR --regtest omni_sendgrant ${ADDR} ${ADDR} ${COL} 1000000 > /dev/null
$SRC/omnicore-cli -datadir=$DATADIR --regtest omni_sendgrant ${ADDR} ${ADDR2} ${COL} 1000000 > /dev/null
$SRC/omnicore-cli -datadir=$DATADIR --regtest  generate 1 >/dev/null

# Checking the balances
printf "Balance of USDT for ADDR:\n"
$SRC/omnicore-cli -datadir=$DATADIR --regtest  omni_getbalance ${ADDR} 2147483652
printf "Balance of USDT for ADDR2:\n"
$SRC/omnicore-cli -datadir=$DATADIR --regtest  omni_getbalance ${ADDR2} 2147483652

printf "Making some tradings ...\n"
TRA1=$($SRC/omnicore-cli -datadir=$DATADIR --regtest omni_tradecontract ${ADDR2} 2147483651 100 1 1 100 1) #>/dev/null
$SRC/omnicore-cli -datadir=$DATADIR --regtest  generate 1 #>/dev/null
$SRC/omnicore-cli -datadir=$DATADIR --regtest omni_gettransaction ${TRA1}
TRA2=$($SRC/omnicore-cli -datadir=$DATADIR --regtest omni_tradecontract ${ADDR} 2147483651 100 1 1 100 2) #>/dev/null
$SRC/omnicore-cli -datadir=$DATADIR --regtest  generate 1 #>/dev/null
$SRC/omnicore-cli -datadir=$DATADIR --regtest omni_gettransaction ${TRA2}
printf "Position in Contracts for ADDR: \n"
$SRC/omnicore-cli -datadir=$DATADIR --regtest  omni_getposition ${ADDR} 2147483651
printf "Testing the RPC for  pegged currency createpayload\n"
$SRC/omnicore-cli --regtest --datadir=$DATADIR omni_createpayload_issuance_pegged 2 2 0 "Currency" "Pegged Currency" "Lihki Coin" "Lihki" "Lihki" ${COL} ${CONT} ${AMOUNT}
printf "Testing the RPC for  pegged currency Tx\n"
TRA=$($SRC/omnicore-cli --regtest --datadir=$DATADIR omni_sendissuance_pegged ${ADDR} 2 2 0 "Currency" "Pegged Currency" "Lihki Coin" "Lihki" "Lihki" ${COL} ${CONT} ${AMOUNT})
$SRC/omnicore-cli -datadir=$DATADIR --regtest  generate 1 > /dev/null
$SRC/omnicore-cli -datadir=$DATADIR --regtest omni_gettransaction ${TRA}
$SRC/omnicore-cli -datadir=$DATADIR --regtest  omni_listproperties
# Checking the balances
printf "Balance of USDT for ADDR:\n"
$SRC/omnicore-cli -datadir=$DATADIR --regtest  omni_getbalance ${ADDR} 2147483652
#$SRC/omnicore-cli -datadir=$DATADIR --regtest  omni_getbalance ${ADDR2} 2147483652
printf "Balance of Pegged Currency in ADDR: \n"
$SRC/omnicore-cli -datadir=$DATADIR --regtest  omni_getbalance ${ADDR} 2147483653
printf "Position in Contracts for ADDR: \n"
$SRC/omnicore-cli -datadir=$DATADIR --regtest  omni_getposition ${ADDR} 2147483651
printf "Checking Payload for Redemption of Peggeds : \n"
$SRC/omnicore-cli --regtest --datadir=$DATADIR omni_createpayload_redemption_pegged 2147483651 1000 ${CONT}
printf " Sending pegged currency from ADDR to ADDR2 : \n"
TRA3=$($SRC/omnicore-cli --regtest --datadir=$DATADIR omni_send ${ADDR} ${ADDR2} 2147483653 1200)
$SRC/omnicore-cli -datadir=$DATADIR --regtest  generate 1 >/dev/null
# printf "Checking RPC for Redemption of Peggeds : \n"
# TRA3=$($SRC/omnicore-cli --regtest --datadir=$DATADIR omni_redemption_pegged ${ADDR} 2147483653 1000 ${CONT})
# $SRC/omnicore-cli -datadir=$DATADIR --regtest  generate 1 > /dev/null
$SRC/omnicore-cli -datadir=$DATADIR --regtest omni_gettransaction ${TRA3}
printf "Balance of Pegged Currency in ADDR: \n"
$SRC/omnicore-cli -datadir=$DATADIR --regtest  omni_getbalance ${ADDR} 2147483653
printf "Balance of Pegged Currency in ADDR2: \n"
$SRC/omnicore-cli -datadir=$DATADIR --regtest  omni_getbalance ${ADDR2} 2147483653
# printf "Position in Contracts for ADDR: \n"
# $SRC/omnicore-cli -datadir=$DATADIR --regtest  omni_getposition ${ADDR} 2147483651
$SRC/omnicore-cli --regtest --datadir=$DATADIR stop
