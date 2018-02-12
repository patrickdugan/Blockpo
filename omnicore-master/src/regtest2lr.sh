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
amountbitcoin_baseaddr=40
amountbitcoin_manyaddr=4
amountbitcoin_moneyaddr=10

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
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  sendtoaddress ${ADDRBase} ${amountbitcoin_baseaddr}  # enviamos 10 BTC a ADDR
printf "Generating one block\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  generate 1

printf "\n________________________________________\n"
printf "   * Participating in the Exodus crowdsale to obtain 1000 OMNIs: To get OMNIs in the first address ADDR\n"

for (( i=1; i<=$N; i++ ))
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

printf "\n________________________________________\n"
printf "Sending OMNIs from base address to many addresses\n"
for (( i=1; i<=$N; i++ ))
do
	./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_send ${ADDRBase} ${ADDRess[$i]} 1 "200" # OMNI = Ecosystem 1
	./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest  generate 1 # Generating one block
done

printf "\n________________________________________\n"
printf "Checking OMNI balances for base address:\n"
./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getbalance ${ADDRBase} 1

for (( i=1; i<=$N; i++ ))
do
	printf "\n________________________________________\n"
	printf "Checking OMNI balances for the address #$i:\n"
	./omnicore-cli -datadir=/home/lihki/.bitcoin --regtest omni_getbalance ${ADDRess[$i]} 1
done

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
printf "   * Checking confirmation of 3ra transaction:\n"
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