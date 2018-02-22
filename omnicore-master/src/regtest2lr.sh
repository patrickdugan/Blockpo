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
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  -rpcwait generate 101 > $NUL # Es importante agregar el rpcwait que espera que el nodo e$

printf "\n________________________________________\n"
printf "\n Checking the balance of block:\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  getbalance  # balance del bloque (50BTCs)

##################################################################
printf "\n________________________________________\n"
printf "Obtaining the base address to work with\n"
ADDRBase=$(./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  getnewaddress OMNIAccount)
printf "\n________________________________________\n"
printf "Printing the base address to work with\n"
printf $ADDRBase

N=100

amountbitcoin_baseaddr=40
amountbitcoin_manyaddr=0.2
amountbitcoin_moneyaddr=10
amountomnies_manyaddr=10

ADDRess=()
for (( i=1; i<=${N}; i++ ))
do
	ADDRess[$i]=$(./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  getnewaddress OMNIAccount)
	printf "\n________________________________\n"
	printf "Printing the address #$i to work with\n"
	printf ${ADDRess[$i]}
done
##################################################################

printf "\n________________________________________\n"
printf " Funding the address with some testnet BTC for fees: 40 BTC to this address\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  sendtoaddress ${ADDRBase} ${amountbitcoin_baseaddr}  # enviamos 10 BTC a ADDR
printf "Generating one block\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  generate 1

printf "\n________________________________________\n"
printf "   * Participating in the Exodus crowdsale to obtain 1000 OMNIs: To get OMNIs in the first address ADDR\n"

for (( i=1; i<=${N}; i++ ))
do 
	StringJSON+=",\""${ADDRess[i]}"\":${amountbitcoin_manyaddr}"	
done

printf "\n________________________________________\n"
printf "Checking JSON String\n"
JSON="{\"moneyqMan7uh8FqdCA2BV5yZ8qVrc9ikLP\":${amountbitcoin_moneyaddr},\""${ADDRBase}"\":${amountbitcoin_manyaddr}$StringJSON}"
printf "${JSON}"

printf "\n________________________________________\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest sendmany "" $JSON #Sending Bitcoin to every address
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  generate 1 # Generating one block

for (( i=1; i<=${N}; i++ ))
do
	printf "\n////////////////////////////////////////\n"
	printf "Sending OMNIs from base address to the addresses #$i\n"
	./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_send ${ADDRBase} ${ADDRess[$i]} 1 "${amountomnies_manyaddr}" # OMNI = Ecosystem 1
	./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  generate 1 # Generating one block
	
	printf "\n________________________________________\n"
	printf "Checking OMNI balances for the address #$i:\n"
	./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getbalance ${ADDRess[$i]} 1
done

printf "\n________________________________________\n"
printf "Creating Future Contract #1 with the address #1:\n"
TRACreate=$(./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_createcontract ${ADDRess[1]} 2 3 1 "Derivaties" "Futures Contracts" "Future Contract 1" "www.tradelayer.org" "Futures Contracts Exchange on OmniLayer" 3 "1" 120 30 2 200 10 1 1)
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest generate 1 ## Generating one block

printf "\n________________________________________\n"
printf "Checking confirmation of transaction:\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_gettransaction $TRACreate
./omnicore-cli -datadir=/home/lihki/.bitcoin --regrest omni_listproperties


###Buy 80 adrr, amount 1-5 ## Sell 100 addr, amount 1-5 ## Buy 100 addr amount 1-5

TRATradeSell=()
for (( i=1; i<=${N}/2+30; i++ ))
do
	printf "\n////////////////////////////////////////\n"
	printf "Amount for sale Seller #$i\n"	
	AMOUNT=$[ ( $RANDOM % 5 ) + 1 ]
	printf $AMOUNT
	
	printf "\n________________________________________\n"
	printf "Price for sale Seller #$i\n"	
	PRICE=$[ ( $RANDOM % 5 )  + 1 ]
	printf $PRICE

	printf "\n________________________________________\n"
	printf "Selling orders with the address Selling #$i\n"
	#Structure: "omni_tradecontract Adrress|PropertyId1|Amount1|PropertyId2|Amount2|Price|Action\n"
	# TRATradeSell[$i]=$(./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_tradecontract ${ADDRess[$i]} 2147483651 ${AMOUNT} 1 1 ${PRICE} 2)
	./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest generate 1

	printf "\n________________________________________\n"
	printf "Checking confirmation of transaction Selling #$i: confirmation = 1 and valid = true\n"
	./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_gettransaction ${TRATradeSell[$i]}

	printf "\n________________________________________\n"
	printf "Checking OMNI balances Seller address #$i:\n"
	./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getbalance ${ADDRess[$i]} 2
done


TRATradeBuy=()
for (( i=1; i<=${N}; i++ ))
do
	# printf "\n////////////////////////////////////////\n"
	# printf "Amount for sale Buyer #$i\n"	
	# AMOUNT=$[ ( $RANDOM % 5 ) + 1 ]
	# printf $AMOUNT
	
	# printf "\n________________________________________\n"
	# printf "Price for sale Buyer #$i\n"	
	# PRICE=$[ ( $RANDOM % 2 )  + 1 ]
	# printf $PRICE

	printf "\n________________________________________\n"
	printf "Buying orders with the address #$i\n"
	#Structure: "omni_tradecontract Adrress|PropertyId1|Amount1|PropertyId2|Amount2|Price|Action\n"
	# TRATradeBuy[$i]=$(./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_tradecontract ${ADDRess[$i]} 2147483651 ${AMOUNT} 1 1 ${PRICE} 1)
	TRATradeBuy[$i]=$(./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_tradecontract ${ADDRess[$i]} 2147483651 1 1 1 20 1)
	./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest generate 1

	printf "\n________________________________________\n"
	printf "Checking confirmation of transaction Buying #$i: confirmation = 1 and valid = true\n"
	./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_gettransaction ${TRATradeBuy[$i]}

	printf "\n________________________________________\n"
	printf "Checking OMNI balances Buyer address #$i:\n"
	./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getbalance ${ADDRess[$i]} 1

done

# TRATradeSell=()
# for (( i=${N}+1; i<=${N}; i++ ))
# do
# 	# printf "\n////////////////////////////////////////\n"
# 	# printf "Amount for sale Seller #$i\n"	
# 	# AMOUNT=$[ ( $RANDOM % 5 ) + 1 ]
# 	# printf $AMOUNT
	
# 	# printf "\n________________________________________\n"
# 	# printf "Price for sale Seller #$i\n"	
# 	# PRICE=$[ ( $RANDOM % 2 )  + 1 ]
# 	# printf $PRICE

# 	printf "\n________________________________________\n"
# 	printf "Selling orders with the address Selling #$i\n"
# 	#Structure: "omni_tradecontract Adrress|PropertyId1|Amount1|PropertyId2|Amount2|Price|Action\n"
# 	# TRATradeSell[$i]=$(./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_tradecontract ${ADDRess[$i]} 2147483651 ${AMOUNT} 1 1 ${PRICE} 2)
# 	TRATradeSell[$i]=$(./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_tradecontract ${ADDRess[$i]} 2147483651 3 1 1 2 2)
# 	./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest generate 1

# 	printf "\n________________________________________\n"
# 	printf "Checking confirmation of transaction Selling #$i: confirmation = 1 and valid = true\n"
# 	./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_gettransaction ${TRATradeSell[$i]}

# 	printf "\n________________________________________\n"
# 	printf "Checking OMNI balances Seller address #$i:\n"
# 	./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getbalance ${ADDRess[$i]} 1
# done

printf "\n________________________________________\n"
printf "   * Checking the orderbook for Future Contract 1 (selling side):\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getcontract_orderbook 2 2

printf "\n________________________________________\n"
printf "   * Checking the orderbook for Future Contract 1 (buying side):\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getcontract_orderbook 2 1

printf "\n//////////////////////////////////////////\n"
printf "Stoping omnicored and omnicore-cli:\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest stop