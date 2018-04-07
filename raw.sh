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
$SRC/omnicore-cli -datadir=$DATADIR --regtest getbalance
printf "Creating some addresses ...\n"
ADDR=$($SRC/omnicore-cli -datadir=$DATADIR --regtest  getnewaddress OMNIAccount)
ADDR2=$($SRC/omnicore-cli -datadir=$DATADIR --regtest  getnewaddress OMNIAccount)
ADDR3=$($SRC/omnicore-cli -datadir=$DATADIR --regtest  getnewaddress OMNIAccount)
ADDR4=$($SRC/omnicore-cli -datadir=$DATADIR --regtest  getnewaddress OMNIAccount)
printf "${ADDR}\n"
printf "${ADDR2}\n"
$SRC/omnicore-cli --regtest  sendtoaddress ${ADDR} 40   # enviamos 10 BTC a ADDR
$SRC/omnicore-cli --regtest  generate 1 >/dev/null

# $SRC/omnicore-cli -datadir=$DATADIR --regtest gettransaction ${TRA}
JSON="{\"moneyqMan7uh8FqdCA2BV5yZ8qVrc9ikLP\":10,\""$ADDR"\":8,\""$ADDR2"\":8,\""$ADDR3"\":8,\""$ADDR4"\":8}"

#Sending bitcoins to all
$SRC/omnicore-cli -datadir=$DATADIR --regtest sendmany "" $JSON > /dev/null
$SRC/omnicore-cli -datadir=$DATADIR --regtest  generate 1 > /dev/null

# checking omni balance:
# $SRC/omnicore-cli -datadir=$DATADIR --regtest omni_getbalance ${ADDR} 1

# testing raw transaction:

# checking the unspending of ADDR
idx=0
printf "the unspending of ADDR:\n"
$SRC/omnicore-cli -datadir=$DATADIR --regtest listunspent 1 99999 "[\""${ADDR}"\"]"
TRA=$($SRC/omnicore-cli -datadir=$DATADIR --regtest listunspent 1 99999 "[\""${ADDR}"\"]")
txid=`echo "$TRA" | jq '.['"$idx"']' | jq .'"txid"'`
vout=`echo "$TRA" | jq '.['"$idx"']' | jq .'"vout"'`
scriptPubKey=`echo "$TRA" | jq '.['"$idx"']' | jq .'"scriptPubKey"'`
amount=`echo "$TRA" | jq '.['"$idx"']' | jq .'"amount"'`
printf "the txid:\n"
printf "${txid}\n"
printf "the vout:\n"
printf "${vout}\n"
printf "the scriptPubKey:\n"
printf "${scriptPubKey}\n"
printf "amount:\n"
printf "${amount}\n"
# creating payload for simple send
PAYLOAD=$($SRC/omnicore-cli -datadir=$DATADIR --regtest omni_createpayload_simplesend 2 "0.1")
printf "Payload of simple send for 1 OMNI:\n"
printf "${PAYLOAD}\n"
# creating the base raw bitcoin transaction
TRA1=$($SRC/omnicore-cli -datadir=$DATADIR --regtest createrawtransaction '[{"txid": '${txid}',"vout":'${vout}'}]' '{}')
# $SRC/omnicore-cli -datadir=$DATADIR --regtest  generate 1 > /dev/null
# atacching the payload to the OP_RETURN
TRA2=$($SRC/omnicore-cli -datadir=$DATADIR --regtest omni_createrawtx_opreturn ${TRA1} ${PAYLOAD})
TRA3=$($SRC/omnicore-cli -datadir=$DATADIR --regtest omni_createrawtx_reference ${TRA2} ${ADDR2})
TRA4=$($SRC/omnicore-cli -datadir=$DATADIR --regtest omni_createrawtx_change ${TRA3} '[{"txid":'${txid}',"vout":'${vout}',"scriptPubKey":'${scriptPubKey}',"value":'${amount}'}]' "${ADDR}" 0.0006)
TRA5=$($SRC/omnicore-cli -datadir=$DATADIR --regtest signrawtransaction ${TRA4})
hex=`echo "$TRA5" | jq .'"hex"'`
hex1=`echo ${hex//[^a-zA-Z0-9]/}`
TRA6=$($SRC/omnicore-cli -datadir=$DATADIR --regtest sendrawtransaction ${hex1})
$SRC/omnicore-cli -datadir=$DATADIR --regtest  generate 1 > /dev/null
$SRC/omnicore-cli -datadir=$DATADIR --regtest omni_gettransaction ${TRA6}
$SRC/omnicore-cli --regtest --datadir=$DATADIR stop
