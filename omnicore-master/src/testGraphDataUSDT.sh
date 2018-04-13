rm graphInfo*

NUL=/dev/null

printf "\n//////////////////////////////////////////\n"
printf "Cleaning the regtest folder\n"
rm -r ~/.bitcoin/regtest

printf "\n________________________________________\n"
printf "Preparing a test environment...\n"
./omnicored -datadir=/home/lihki/.bitcoin --cleancache --startclear --regtest --server --daemon

printf "\n________________________________________\n"
printf "Waiting three seconds to start the client...\n"

sleep 3

printf "\n________________________________________\n"
printf "Preparing some mature regtest BTC: Mining the first 101 blocks getting 50 Bitcoins\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  -rpcwait generate 202 > $NUL # Es importante agregar el rpcwait que espera que el nodo e$

printf "\n________________________________________\n"
printf "\n Checking the balance of block:\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  getbalance  # balance del bloque (50BTCs)

# ##################################################################
printf "\n________________________________________\n"
printf "Obtaining the base address to work with\n"
ADDRBase=$(./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  getnewaddress OMNIAccount)
printf "\n________________________________________\n"
printf "Printing the base address to work with\n"
printf $ADDRBase

N=100

amountbitcoin_baseaddr=40
amountbitcoin_manyaddr=0.02
amountbitcoin_moneyaddr=30
notional_size=1
margin_requirement=1
amountusdts_manyaddr=100000
# amountomnies_manyaddr=20
collateral=2147483652

ADDRess=()
for (( i=1; i<=${N}; i++ ))
do
	ADDRess[$i]=$(./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  getnewaddress OMNIAccount)
	echo ${ADDRess[$i]} >> graphInfoAddresses.txt
done

##################################################################
printf "\n________________________________________\n"
printf " Funding the address with some testnet BTC for fees: 40 BTC to this address\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  sendtoaddress ${ADDRBase} ${amountbitcoin_baseaddr}  # enviamos 10 BTC a ADDR
printf "Generating one block\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  generate 1

##################################################################
printf "\n________________________________________\n"
printf "   * Participating in the Exodus crowdsale to obtain 1000 OMNIs: To get OMNIs in the first address ADDR\n"

for (( i=1; i<=${N}; i++ ))
do 
	StringJSON+=",\""${ADDRess[$i]}"\":${amountbitcoin_manyaddr}"	
done

printf "\n________________________________________\n"
printf "Checking JSON String\n"
JSON="{\"moneyqMan7uh8FqdCA2BV5yZ8qVrc9ikLP\":${amountbitcoin_moneyaddr},\""${ADDRBase}"\":${amountbitcoin_manyaddr}$StringJSON}"
printf "${JSON}"

printf "\n________________________________________\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest sendmany "" $JSON #Sending Bitcoin to every address
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest generate 1 # Generating one block

##################################################################
printf "\n________________________________________\n"
printf "Creating Future Contract #1 with the base address:\n"
TRACreate=$(./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_createcontract $ADDRBase 2 3 1 "Derivaties" "Futures Contracts" "Future Contract 1" "www.tradelayer.org" "Futures Contracts Exchange on OmniLayer" 3 "1" 120 30 2 200 ${notional_size} ${collateral} ${margin_requirement})
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest generate 1 ## Generating one block

printf "\n________________________________________\n"
printf "Checking confirmation of Creating Future Contract #1:\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_gettransaction $TRACreate
./omnicore-cli -datadir=/home/lihki/.bitcoin --regrest omni_listproperties

##################################################################
printf "\n________________________________________\n"
printf "Creating an Divisible Token USDT:\n"   # TODO: see the indivisible/divisible troubles (amountToReserve < nBalance) in logicMath_ContractDexTrade function on tx
TRAUSDT=$(./omnicore-cli --regtest omni_sendissuancemanaged $ADDRBase 2 2 0 "Tether" "Tether" "Tether" "" "")
./omnicore-cli --regtest generate 1

printf "\n________________________________________\n"
printf "Checking confirmation of transaction Token USDT:\n"
./omnicore-cli --regtest omni_gettransaction $TRAUSDT
./omnicore-cli --regrest omni_listproperties

##################################################################
for (( i=1; i<=${N}; i++ ))
do
	printf "\n////////////////////////////////////////\n"
	printf "Sending USDTs from base address to the addresses #$i\n"
	./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_sendgrant ${ADDRBase} ${ADDRess[$i]} 2147483652 "${amountusdts_manyaddr}" # OMNI = Ecosystem 1
	./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest generate 1 # Generating one block
	
	printf "\n________________________________________\n"
	printf "Checking USDT balances for the address #$i:\n"
	./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getbalance ${ADDRess[$i]} 2147483652
done

##################################################################
printf "\n________________________________________\n"
printf "Checking All balances for the id: 2147483652\n"
./omnicore-cli --regtest omni_getallbalancesforid 2147483652

printf "\n________________________________________\n"
printf "Checking grants to addresses:\n"
./omnicore-cli --regtest omni_getgrants 2147483652

printf "\n________________________________________\n"
printf "Looking info of Contracts:\n"
./omnicore-cli --regtest omni_getproperty 2147483651

##################################################################
# Price: 11000-11200, Amount Buy 80 Addrs: 100-5000 | Amount Sell 100 Addrs: 100-5000, Price: 11100-11300 | Amount Buy 100 Addrs: 100-5000, Price: 11000-11200;

TRATradeBuy=()
for (( i=1; i<=${N}/2+30; i++ ))
do
	printf "\n////////////////////////////////////////\n"
	printf "Amount for sale Buyer #$i\n"	
	AMOUNT=$[ ( $RANDOM % 5000 ) + 100 ]
	printf $AMOUNT
	
	printf "\n________________________________________\n"
	printf "Price for sale Buyer #$i\n"	
	PRICE=$[ ( $RANDOM % 11200 )  + 11000 ]
	printf $PRICE

	printf "\n________________________________________\n"
	printf "Buying orders with the address #$i\n"
	#Structure: "omni_tradecontract Adrress|PropertyId1|Amount1|PropertyId2|Amount2|Price|Action\n"
	TRATradeBuy[$i]=$(./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_tradecontract ${ADDRess[$i]} ${collateral} ${AMOUNT} 1 1 ${PRICE} 1)
	./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest generate 1

	printf "\n________________________________________\n"
	printf "Checking confirmation of transaction Buying #$i: confirmation = 1 and valid = true\n"
	./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_gettransaction ${TRATradeBuy[$i]}

done

#-------------------------------------------------------------#

TRATradeSell=()
for (( i=1; i<=${N}; i++ ))
do
	printf "\n////////////////////////////////////////\n"
	printf "Amount for sale Seller #$i\n"	
	AMOUNT=$[ ( $RANDOM % 5000 ) + 100 ]
	printf $AMOUNT
	
	printf "\n________________________________________\n"
	printf "Price for sale Seller #$i\n"	
	PRICE=$[ ( $RANDOM % 11300 )  + 11100 ]
	printf $PRICE

	printf "\n________________________________________\n"
	printf "Selling orders with the address Selling #$i\n"
	#Structure: "omni_tradecontract Adrress|PropertyId1|Amount1|PropertyId2|Amount2|Price|Action\n"
	TRATradeSell[$i]=$(./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_tradecontract ${ADDRess[$i]} ${collateral} ${AMOUNT} 1 1 ${PRICE} 2)
	./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest generate 1

	printf "\n________________________________________\n"
	printf "Checking confirmation of transaction Selling #$i: confirmation = 1 and valid = true\n"
	./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_gettransaction ${TRATradeSell[$i]}

done

#-------------------------------------------------------------#

TRATradeBuyNew=()
for (( i=1; i<=${N}; i++ ))
do
	printf "\n////////////////////////////////////////\n"
	printf "Amount for sale Buyer #$i\n"	
	AMOUNT=$[ ( $RANDOM % 5000 ) + 100 ]
	printf $AMOUNT
	
	printf "\n________________________________________\n"
	printf "Price for sale Buyer #$i\n"	
	PRICE=$[ ( $RANDOM % 11200 )  + 11000 ]
	printf $PRICE

	printf "\n________________________________________\n"
	printf "Buying orders with the address #$i\n"
	#Structure: "omni_tradecontract Adrress|PropertyId1|Amount1|PropertyId2|Amount2|Price|Action\n"
	TRATradeBuyNew[$i]=$(./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_tradecontract ${ADDRess[$i]} ${collateral} ${AMOUNT} 1 1 ${PRICE} 1)
	./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest generate 1

	printf "\n________________________________________\n"
	printf "Checking confirmation of transaction Buying #$i: confirmation = 1 and valid = true\n"
	./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_gettransaction ${TRATradeBuyNew[$i]}

done

##########################################################
for (( i=1; i<=${N}; i++ ))
do
	printf "\n________________________________________\n"
	printf "Checking position Seller address #1:\n"
	./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getposition ${ADDRess[$i]} ${collateral}

done

	printf "\n________________________________________\n"
	printf "   * Checking the orderbook for Future Contract 1 (selling side):\n"
	./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getcontract_orderbook ${collateral} 2

	printf "\n________________________________________\n"
	printf "   * Checking the orderbook for Future Contract 1 (buying side):\n"
	./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getcontract_orderbook ${collateral} 1

##########################################################

printf "\n//////////////////////////////////////////\n"
printf "Stoping omnicored and omnicore-cli:\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest stop