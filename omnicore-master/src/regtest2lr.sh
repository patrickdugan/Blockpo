# NUL=/dev/null

# printf "\n////////////////////////////////\n"
# printf "Cleaning the regtest folder\n"
# rm -r ~/.bitcoin/regtest

# printf "\n________________________________\n"
# printf "Preparing a test environment...\n"
# ./omnicored -datadir=/home/lihki/.bitcoin --cleancache --startclear --regtest --server --daemon

# printf "\n________________________________\n"
# printf "Waiting three seconds to start the client...\n"
# sleep 3

# printf "\n________________________________\n"
# printf "Preparing some mature regtest BTC: Mining the first 101 blocks getting 50 Bitcoins\n"
# ./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  -rpcwait generate 101 > $NUL # Es importante agregar el rpcwait que espera que el nodo e$

# printf "\n________________________________\n"
# printf "\n Checking the balance of block:\n"
# ./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  getbalance  # balance del bloque (50BTCs)

# printf "\n________________________________\n"
# printf " Obtaining address base to work with:\n"
# ADDRBase="$(./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  getnewaddress OMNIAccount)"
# printf $ADDRBase 

# printf "\n________________________________\n"
# printf " Funding the address base with some testnet BTC for fees\n"
# JSON="{\"moneyqMan7uh8FqdCA2BV5yZ8qVrc9ikLP\":10,\""$ADDRBase"\":20}"
# ./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest sendmany "" $JSON #Sending Bitcoin to every address

# printf "\n________________________________\n"
# printf " Generating one block\n"
# ./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  generate 1

# printf "\n________________________________\n"
# printf "Unspent Transaction Output address base\n"
# ./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest listunspent 

# N=2
# ADDR=()
# for (( i=1; i<=$N; i++ ))
# do
# 	printf "\n________________________________\n"
# 	printf " Obtaining address #$i to work with:\n"
# 	ADDR[$i]="$(./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  getnewaddress OMNIAccount)"
# 	printf "\n________________________________\n"
# 	printf "Sending Bitcoins to the address #$i\n"
# 	./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest sendtoaddress ${ADDR[$i]} 4 # OMNI = Ecosystem 1
# 	./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest generate 1 # Generating one block
# done

# for (( i=1; i<=$N; i++ ))
# do
# 	printf "\n________________________________\n"
# 	printf "Sending Omnies from base address\n"
# 	./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_send $ADDRBase ${ADDR[$i]} 1 "10" # OMNI = Ecosystem 1
# 	./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest generate 1 # Generating one block	
# 	printf "\n________________________________\n"
# 	printf "Checking OMNI balances of the address #$i:\n"
# 	./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getbalance ${ADDR[$i]} 1
# done

# printf "Stoping omnicored and omnicore-cli:\n"
# ./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest stop

########################################################################################################################

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

N=2
ADDRess=()
for (( i=1; i<=$N; i++ ))
do
	ADDRess[$i]=$(./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  getnewaddress OMNIAccount)
	printf "\n________________________________\n"
	printf "Printing the address #$i to work with\n"
	printf ${ADDRess[$i]}
done
##################################################################

printf "\n________________________________________\n"
printf " Funding the address with some testnet BTC for fees: 40 BTC to this address\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  sendtoaddress ${ADDRBase} 40  # enviamos 10 BTC a ADDR
printf "Generating one block\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  generate 1

printf "\n________________________________________\n"
printf "   * Participating in the Exodus crowdsale to obtain 1000 OMNIs: To get OMNIs in the first address ADDR\n"
JSON="{\"moneyqMan7uh8FqdCA2BV5yZ8qVrc9ikLP\":10,\""${ADDRBase}"\":4,\""${ADDRess[1]}"\":4,\""${ADDRess[2]}"\":4}"

for (( i=1; i<=$N; i++ ))
do 
	StringJSON+=",\""${ADDRess[i]}"\":4"	
done

printf "\n________________________________________\n"
printf "Checking JSON String\n"
StringJSONnew="{\"moneyqMan7uh8FqdCA2BV5yZ8qVrc9ikLP\":10,\""${ADDRBase}"\":4$StringJSON}"
printf "${StringJSONnew}"

printf "\n________________________________________\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest sendmany "" $JSON #Sending Bitcoin to every address
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  generate 1 # Generating one block

./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_send ${ADDRBase} ${ADDRess[1]} 1 "200" # OMNI = Ecosystem 1
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  generate 1 # Generating one block
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_send ${ADDRBase} ${ADDRess[2]} 1 "200" # OMNI = Ecosystem 1
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  generate 1 # Generating one block

printf "\n________________________________________\n"
printf "   * Checking OMNI balances ADDR:\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getbalance ${ADDRBase} 1
printf "   * Checking OMNI balances ADDR2:\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getbalance ${ADDRess[1]} 1
printf "   * Checking OMNI balances ADDR3:\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getbalance ${ADDRess[2]} 1

printf "\n________________________________________\n"
printf "   * Creating Future Contract 1:\n"
TRA=$(./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_createcontract ${ADDRBase} 2 3 1 "Derivaties" "Futures Contracts" "Future Contract 1" "www.tradelayer.org" "Futures Contracts Exchange on OmniLayer" 3 "1" 120 30 2 200 100 1 2)
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest generate 1 ## Generating one block

printf "\n________________________________________\n"
printf "   * Checking confirmation of transaction:\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_gettransaction $TRA
./omnicore-cli -datadir=/home/lihki/.bitcoin --regrest omni_listproperties

printf "\n________________________________________\n"
printf "   * Buying 10 contracts:\n"
TRA2=$(./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_tradecontract ${ADDRBase} 2147483651 10 1 1 3 1)
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest generate 1

printf "\n________________________________________\n"
printf "   * Checking confirmation of 2nd transaction: confirmation: 1 and valid: true\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_gettransaction $TRA2

printf "\n________________________________________\n"
printf "   * Checking OMNI balances Buyer:\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getbalance ${ADDRBase} 1

printf "\n________________________________________\n"
printf "   * Selling 3 contracts:\n"
TRA3=$(./omnicore-cli --regtest omni_tradecontract ${ADDRess[1]} 2147483651 3 1 1 3 2)
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest generate 1

printf "\n________________________________________\n"
printf "   * Checking confirmation of 3nd transaction:\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_gettransaction $TRA3

printf "\n________________________________________\n"
printf "   * Checking OMNI balances Seller:\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getbalance ${ADDRBase} 2

printf "\n________________________________________\n"
printf "   * Checking the orderbook for Future Contract 1 (buying side):\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getcontract_orderbook 2147483651 1

printf "\n________________________________________\n"
printf "   * Checking the orderbook for Future Contract 1 (selling side):\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getcontract_orderbook 2147483651 2

printf "\n________________________________________\n"
printf "   * Checking OMNI balances ADDR:\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getbalance ${ADDRBase} 1

printf "\n________________________________________\n"
printf "   * Checking OMNI balances ADDR[1]:\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getbalance ${ADDRess[1]} 1

printf "\n________________________________________\n"
printf "   * Checking position in Future Contract 1 for ADDRBase:\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getposition ${ADDRBase} 2147483651

printf "\n//////////////////////////////////////////\n"
printf "Stoping omnicored and omnicore-cli:\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest stop

#######################################################################################################

# NUL=/dev/null
# rm -r ~/.bitcoin/regtest
# printf "Preparing a test environment...\n"
# ./omnicored -datadir=/home/lihki/.bitcoin --cleancache --startclear --regtest --server --daemon
# sleep 3
# printf "Preparing some mature testnet BTC\n"
# ./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  -rpcwait generate 101 > $NUL # Es importante agregar el rpcwait que espera que el nodo e$
# # ./omnicore-cli --regtest generate 101 >$NUL # generamos 200 bloques, ganando 50 BTCs
# printf "\n The balance of block:\n"
# ./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  getbalance  # balance del bloque (50BTCs)
# printf "   * Obtaining one address to work with\n"
# ADDR=$(./omnicore-cli --regtest  getnewaddress OMNIAccount)
# printf "Address OMNIAccoun\n"
# printf $ADDR 
# printf "   * Funding the address with some testnet BTC for fees\n"
# ./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  sendtoaddress $ADDR 20  # enviamos 20 BTC a ADDR
# ./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  generate 1
# printf "   * Participating in the Exodus crowdsale to obtain some OMNI\n"
# JSON="{\"moneyqMan7uh8FqdCA2BV5yZ8qVrc9ikLP\":10,\""$ADDR"\":8}"
# ./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest sendmany "" $JSON
# ./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest generate 1
# printf "   * Creating Future Contract 1:\n"
# TRA=$(./omnicore-cli --regtest omni_createcontract $ADDR 2 3 1 "Derivaties" "Futures Contracts" "Future Contract 1" "www.tradelayer.org" "Futures Contracts Exchange on OmniLayer" 3 "1" 120 30 2 200 10 1 2)
# ./omnicore-cli --regtest generate 1
# printf "   * Checking confirmation of transaction:\n"
# ./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_gettransaction $TRA
# ./omnicore-cli -datadir=/home/lihki/.bitcoin --regrest omni_listproperties
# printf "   * Checking OMNI balances:\n"
# ./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getbalance $ADDR 1
# ./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getbalance $ADDR 2
# printf "   * Buying 10 contracts:\n"
# TRA2=$(./omnicore-cli --regtest omni_tradecontract $ADDR 2147483651 10 1 1 3 1)
# ./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest generate 1
# printf "   * Checking confirmation of 2nd transaction:\n"
# ./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_gettransaction $TRA2
# printf "   * Checking the orderbook for Future Contract 1:\n"
# ./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getcontract_orderbook 2147483651 1
# printf "   * Selling 3 contracts:\n"
# TRA3=$(./omnicore-cli --regtest omni_tradecontract $ADDR 2147483651 3 1 1 3 2)
# ./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest generate 1
# printf "   * Checking confirmation of 3nd transaction:\n"
# ./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_gettransaction $TRA3
# printf "   * Checking the orderbook for Future Contract 1:\n"
# ./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getcontract_orderbook 2147483651 2
# ./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest stop