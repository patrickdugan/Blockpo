SRC=/home/ale/Desktop/omnicore-cash/Omnicore-Cash/src
DATADIR=/home/ale/.bitcoincash
CONT=2147483651
COL=2147483652
AMOUNT=1000
bitcoins=5
rm -r -f $DATADIR/regtest
# $SRC/omnicore-cli --regtest --datadir=$DATADIR stop &>/dev/null
# sleep 1
printf "Preparing a test environment...\n"
$SRC/bitcoind -datadir=$DATADIR -daemon -server
$SRC/bitcoin-cli -datadir=$DATADIR -rpcwait getinfo
$SRC/bitcoin-cli -datadir=$DATADIR -rpcwait generate 101 >/dev/null
$SRC/bitcoin-cli -datadir=$DATADIR getbalance
printf "Creating some addresses ...\n"
ADDR=$($SRC/bitcoin-cli -datadir=$DATADIR  getnewaddress "")
ADDR2=$($SRC/bitcoin-cli -datadir=$DATADIR getnewaddress "")
printf "${ADDR}\n"
printf "${ADDR2}\n"
TRA=$($SRC/bitcoin-cli -datadir=$DATADIR sendtoaddress ${ADDR} 40)   # enviamos 10 BTC a ADDR
$SRC/bitcoin-cli -datadir=$DATADIR generate 1 >/dev/null
$SRC/bitcoin-cli -datadir=$DATADIR  gettransaction ${TRA}


# testing raw transaction: ----------------------------------------------------

# checking the unspending of ADDR
idx=0
printf "the unspending of ADDR:\n"
$SRC/bitcoin-cli -datadir=$DATADIR --regtest listunspent 1 99999 "[\""${ADDR}"\"]"
TRA=$($SRC/bitcoin-cli -datadir=$DATADIR --regtest listunspent 1 99999 "[\""${ADDR}"\"]")
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
# the payload for 0.1 OMNI:
printf "the payload for 0.1 OMNI:"
PAYLOAD=00000000000000020000000000989680
printf "${PAYLOAD}\n"
# creating the base raw bitcoin cash transaction
JSON="{\""$ADDR2"\":8,\""$ADDR"\":1}"
JSON3="{\"data\":\"6f6d6e6900000000000000020000000000989680\"}" # Omni Marker and the payload
TRA1=$($SRC/bitcoin-cli -datadir=$DATADIR createrawtransaction '[{"txid": '${txid}',"vout":'${vout}'}]' ${JSON3})
# $SRC/bitcoin-cli -datadir=$DATADIR decoderawtransaction ${TRA1})
# $SRC/bitcoin-cli -datadir=$DATADIR decodescript ${TRA1}
TRA2=$($SRC/bitcoin-cli -datadir=$DATADIR signrawtransaction ${TRA1})
hex=`echo "$TRA2" | jq .'"hex"'`
hex1=`echo ${hex//[^a-zA-Z0-9]/}`
TRA3=$($SRC/bitcoin-cli -datadir=$DATADIR sendrawtransaction ${hex1} true)
# $SRC/bitcoin-cli -datadir=$DATADIR generate 1 > /dev/null
# $SRC/bitcoin-cli -datadir=$DATADIR getrawtransaction ${TRA6} 1
$SRC/bitcoin-cli -datadir=$DATADIR decoderawtransaction ${hex1}
$SRC/bitcoin-cli -datadir=$DATADIR stop
