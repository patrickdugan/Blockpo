
#SRC=$HOME/omni/Blockpo/omnicore-master/src
#DATADIR=$HOME/contractstest
SRC=/home/ale/Desktop/BlockPo/omnicore-bin/omnicore-master/src
DATADIR=/home/ale/.bitcoin
echo '  ___  __  __ _   _ ___ _        _ __   _______ ____ '
echo ' / _ \|  \/  | \ | |_ _| |      / \\ \ / / ____|  _ \ '
echo '| | | | |\/| |  \| || || |     / _ \\ V /|  _| | |_) |'
echo '| |_| | |  | | |\  || || |___ / ___ \| | | |___|  _ < '
echo ' \___/|_|  |_|_| \_|___|_____/_/   \_\_| |_____|_| \_\'
echo ''
echo 'Starting ...'
echo ''

SUM=0
PROM=0
N=10
amountbitcoin_baseaddr=10
amountbitcoin_manyaddr=2
amountbitcoin_moneyaddr=1
amountusds_manyaddr=1000000
NUL=/dev/null
COL=2147483652
#SRC=$HOME/omni/Blockpo/omnicore-master/src
#DATADIR=$HOME/contractstest
#DATADIR=$HOME/.bitcoin
rm -r -f $DATADIR/regtest

#If the omnicored is already on
$SRC/omnicore-cli --regtest --datadit=$DATADIR stop &>/dev/null
sleep 3
$SRC/omnicored --regtest --datadir=$DATADIR --cleancache --startclear --daemon
#Preparing some mature regtest BTC\n"
$SRC/omnicore-cli -datadir=$DATADIR --regtest  -rpcwait generate 101 > $NUL # Es importante agregar el rpcwait que espera que el nodo e$

#The main address
ADDR=$($SRC/omnicore-cli -datadir=$DATADIR --regtest  getnewaddress OMNIAccount) > /dev/null

#Funding the address with some testnet BTC for fees
$SRC/omnicore-cli -datadir=$DATADIR --regtest  sendtoaddress ${ADDR} ${amountbitcoin_baseaddr} >/dev/null  # enviamos 10 BTC a ADDR
$SRC/omnicore-cli -datadir=$DATADIR --regtest  generate 1 > /dev/null

#Getting some address
ADDRess=()
for (( i=1; i<=${N}; i++ ))
do
	ADDRess[$i]=$($SRC/omnicore-cli -datadir=$DATADIR --regtest  getnewaddress OMNIAccount) > /dev/null
	# printf "\n________________________________\n"
	# printf "Printing the address #$i to work with\n"
	# printf ${ADDRess[$i]}
done

for (( i=1; i<=${N}; i++ ))
do
	StringJSON+=",\""${ADDRess[i]}"\":${amountbitcoin_manyaddr}"
done


JSON="{\"moneyqMan7uh8FqdCA2BV5yZ8qVrc9ikLP\":${amountbitcoin_moneyaddr},\""${ADDR}"\":${amountbitcoin_manyaddr}$StringJSON}"


#Sending bitcoins to all
$SRC/omnicore-cli -datadir=$DATADIR --regtest sendmany "" $JSON > /dev/null
$SRC/omnicore-cli -datadir=$DATADIR --regtest  generate 1 > /dev/null

echo "Your new address is $ADDR with 1000000 Synthetic USDs available"
echo ""
echo "Creating the Contract:"
echo ""
echo "(Just ENTER for default options)"
echo ""
echo -n "Enter contract name [Default : Future Contract 1]: "
read par1
: ${par1:="Future Contract 1"}
echo -n "Enter the blocks until expiration [Default : 1000]: "
read par2
: ${par2:=1}


	    while [[ ! ${par2} =~ ^[0-9]+$ ]]; do
		echo "Please enter a number:"
		read par2
	    done
            echo -n "Enter the notional size [Default : 10]: "
            read par3
: ${par3:=10}

            while [[ ! ${par3} =~ ^[0-9]+$ ]]; do
                echo "Please enter a number:"
                read par3
            done

echo -n "Enter id of collateral currency [Default : (USD)]: "
read par4
: ${par4:=2147483652}

	    while [[ ! ${par4} =~ ^[0-9]+$ ]]; do
		echo "Please enter a number:"
		read par4
	    done
echo -n "Enter margin requirement for one contract  [Default : 2 (USD)]: "
read par5
: ${par5:=2}

            while [[ ! ${par5} =~ ^[0-9]+$ ]]; do
                echo "Please enter a number:"
                read par5
            done


#Creo el futuro
TRA=$($SRC/omnicore-cli -datadir=$DATADIR --regtest omni_createcontract $ADDR 2 3 1 "Derivaties" "Futures Contracts" "$par1" "www.tradelayer.org" "Futures Contracts Exchange on OmniLayer" 3 "1" 120 30 2 $par2 $par3 $par4 $par5)
$SRC/omnicore-cli -datadir=$DATADIR --regtest generate 1 >/dev/null

# Creating Divisible tokens ( Synthetic USD)
#printf "Creating an Divisible Token:\n"   # TODO: see the indivisible/divisible troubles (amountToReserve < nBalance) in logicMath_Contra$
$SRC/omnicore-cli --regtest omni_sendissuancemanaged ${ADDR} 2 2 0 "Tether" "Tether" "Tether" "" "" >/dev/null
$SRC/omnicore-cli --regtest generate 1 >/dev/null
$SRC/omnicore-cli --regtest omni_gettransaction $TRA

#Sending synthetic USDs to all
$SRC/omnicore-cli -datadir=$DATADIR --regtest omni_sendgrant ${ADDR} ${ADDR} ${COL} "${amountusds_manyaddr}" > /dev/null
$SRC/omnicore-cli -datadir=$DATADIR --regtest  generate 1 >/dev/null

for (( i=1; i<=${N}; i++ ))
do
        # printf "\n////////////////////////////////////////\n"
        # printf "Sending USDs from base address to the addresses #$i\n"
        $SRC/omnicore-cli -datadir=$DATADIR --regtest omni_sendgrant ${ADDR} ${ADDRess[$i]} ${COL} "${amountusds_manyaddr}" > /dev/null
        $SRC/omnicore-cli -datadir=$DATADIR --regtest  generate 1 > /dev/null # Generating one block
        # printf "\n________________________________________\n"
        # printf "Checking USDs balances for the address #$i:\n"
        $SRC/omnicore-cli -datadir=$DATADIR --regtest omni_getbalance ${ADDRess[$i]} ${COL} >/dev/null
done

#$SRC/omnicore-cli -datadir=$DATADIR --regtest omni_getbalance ${ADDR} ${COL} > /dev/null

#Generando ordenes!
echo ""
echo -n "Putting some orders in Orderbook..."

for (( i=1; i<=${N}; i++ ))
do
        NUMBER=$[ 10539 - i ]
        SUM=$[ SUM + NUMBER ]
        #echo "NUMBER : ${NUMBER}"
        #echo "SUM : ${SUM}"
        #printf "\n________________________________________\n"
        #printf "Buying orders with the address #$i\n"
        if [ ${i} -lt 6 ]
        then
            TRATradeBuy[$i]=$($SRC/omnicore-cli -datadir=$DATADIR --regtest omni_tradecontract ${ADDRess[$i]} 2147483651 3 1 1 ${NUMBER} 2) >/dev/null
        else
            TRATradeBuy[$i]=$($SRC/omnicore-cli -datadir=$DATADIR --regtest omni_tradecontract ${ADDRess[$i]} 2147483651 3 1 1 ${NUMBER} 1) >/dev/null
        fi
        $SRC/omnicore-cli -datadir=$DATADIR --regtest generate 1 >/dev/null
done
PROM=$[ $SUM/$N ]
#echo "promedio: $PROM"

while true; do

        echo -e "\n"
	echo "*********"
	echo "1 - Trade"
	echo "2 - Get Balance"
	echo "3 - Get Positions"
	echo "4 - Get Order Book"
	echo "5 - Deposit on Balance"
	echo "6 - Exit"
	echo "*********"
        echo ""
        echo -n "Choose the operation : "
	read opt
            while [[ ! ${opt} =~ ^[0-9]+$ ]]; do
               echo "Please enter a correct option:"
            read opt
            done

            while [ ${opt} -gt 6 ] || [ ${opt} -lt 1 ]; do
               echo "Please enter a correct option:"
            read opt
            done

	    if [ "$opt" == "2" ]
	    then
		balance=$($SRC/omnicore-cli -datadir=$DATADIR --regtest omni_getcontract_balance ${ADDR} ${par4})
		amountbalance=`echo "$balance" | jq ."balance"`
		amountreserve=`echo "$balance" | jq ."reserved"`
		amountbalance=${amountbalance%.*}
		amountbalance="${amountbalance#\"}"
		amountreserve=${amountreserve%.*}
		amountreserve="${amountreserve#\"}"

		echo -e "\n"
		echo "Balance :"           $amountbalance
		echo "Contract Reserved :" $amountreserve
		continue

	    fi

	    if [ "$opt" == "3" ]
	    then
		position=$($SRC/omnicore-cli -datadir=$DATADIR --regtest omni_getposition ${ADDR} 2147483651)
		shortPosition=`echo "$position" | jq ."shortPosition"`
		longPosition=`echo "$position" | jq ."longPosition"`
		shortPosition=${shortPosition%.*}
		shortPosition="${shortPosition#\"}"
		longPosition=${longPosition%.*}
		longPosition="${longPosition#\"}"

		echo -e "\n"
		echo "Short Positions :" $shortPosition
		echo "Long Positions :" $longPosition
		continue

	    fi

	    if [ "$opt" == "4" ]
	    then



		echo -e "\n"
		printf "\x1b[31m    SELL ORDER BOOK\x1b[0m\n"
		vendebook=$($SRC/omnicore-cli -datadir=$DATADIR --regtest omni_getcontract_orderbook 2147483651 2)
    $SRC/omnicore-cli -datadir=$DATADIR --regtest omni_getcontract_orderbook 2147483651 2

		total_rows_venta=`echo "$vendebook" | jq '. | length'`


		printf "\x1b[31m Price         Volumen\x1b[0m\n"
		idx2=0
		while true; do

			if [ "$idx2" == "$total_rows_venta" ]
			then
			    break
			fi


      amountforsale=`echo "$vendebook" | jq '.['"$idx2"']' | jq ."amountforsale"`
			effectiveprice=`echo "$vendebook" | jq '.['"$idx2"']' | jq ."effectiveprice"`
                        #txid=`echo "$vendebook" | jq '.['"$idx2"']' | jq ."block"`
			amountforsale=${amountforsale%.*}
			amountforsale="${amountforsale#\"}"
			effectiveprice=${effectiveprice%.*}
			effectiveprice="${effectiveprice#\"}"
                        address=`echo "$vendebook" | jq '.['"$idx2"']' | jq ."address"`
                        amountforsale=${amountforsale%.*}
                        amountforsale="${amountforsale#\"}"
                        effectiveprice=${effectiveprice%.*}
                        effectiveprice="${effectiveprice#\"}"
                        address=${address%.*}
                        address="${address#\"}"
                        message="<--your order"
                        con='"'
                        AD=$ADDR$con
                        #echo "${AD}"
                        #echo "${address}"
                        if [ "${AD}" == "${address}" ]
                        then
                           printf "\x1b[31m\" $effectiveprice                 $amountforsale \" $message \x1b[0m\n"
                        else
                           printf "\x1b[31m\" $effectiveprice                 $amountforsale \"\x1b[0m\n"
                        fi

		        idx2=$((idx2+1))


		done


		echo -e "\n"
		printf "\x1b[32m    BUY ORDER BOOK\x1b[0m\n"
		comprabook=$($SRC/omnicore-cli -datadir=$DATADIR --regtest omni_getcontract_orderbook 2147483651 1)
		$SRC/omnicore-cli -datadir=$DATADIR --regtest omni_getcontract_orderbook 2147483651 1
		total_rows_compra=`echo "$comprabook" | jq '. | length'`
		total_rows_compra=$((total_rows_compra+1))

                printf "\x1b[32m Price         Volumen\x1b[0m\n"
		idx=0
		while true; do

			if [ "$idx" == "$total_rows_compra" ]
			then
			    break
			fi

                        amountforsalec=`echo "$comprabook" | jq '.['"$idx"']' | jq ."amountforsale"`
			effectivepricec=`echo "$comprabook" | jq '.['"$idx"']' | jq ."effectiveprice"`
			#txidc=`echo "$comprabook" | jq '.['"$idx"']' | jq ."block"`
			amountforsalec=${amountforsalec%.*}
			amountforsalec="${amountforsalec#\"}"
			effectivepricec=${effectivepricec%.*}
			effectivepricec="${effectivepricec#\"}"
                        addressc=`echo "$comprabook" | jq '.['"$idx"']' | jq ."address"`
                        addressc=${addressc%.*}
                        addressc="${addressc#\"}"
                        message="<--your order"
                        conc='"'
                        ADc=$ADDR$conc
                        #echo "${ADc}"
                        #echo "${addressc}"
                        if [ "${ADc}" == "${addressc}" ]
                        then
                           printf "\x1b[32m\" $effectivepricec                 $amountforsalec \" $message \x1b[0m\n"
                        else
                           printf "\x1b[32m\" $effectivepricec                 $amountforsalec \"\x1b[0m\n"
                        fi
		        idx=$((idx+1))


		done



		continue

	    fi

	    if [ "$opt" == "5" ]
	    then
	    echo "Please enter amount of USD that you want:"
            read amount
      $SRC/omnicore-cli -datadir=$DATADIR --regtest omni_sendgrant $ADDR $ADDR 2147483652 ${amount} >/dev/null
	    $SRC/omnicore-cli -datadir=$DATADIR --regtest generate 1 >/dev/null
            echo "Your new balance is:"
            balance=$($SRC/omnicore-cli -datadir=$DATADIR --regtest omni_getcontract_balance $ADDR 2147483652)
            amountbalance=`echo "$balance" | jq ."balance"`
            amountreserve=`echo "$balance" | jq ."reserved"`
            amountbalance=${amountbalance%.*}
            amountbalance="${amountbalance#\"}"
            amountreserve=${amountreserve%.*}
            amountreserve="${amountreserve#\"}"
            echo -e "\n"
            echo "Balance :"           $amountbalance
            echo "Contract Reserved :" $amountreserve
            continue
	    fi

	    if [ "$opt" == "6" ]
	    then
 		     $SRC/omnicore-cli -datadir=$DATADIR --regtest stop
		     exit 1
	    fi
            if [ "$opt" == "1" ]
            then
		   echo "TRADE : "
		   echo "1 - BUY"
		   echo "2 - SELL"
		   echo -n "Select Operation : "
		   read tipop

		   while [[ ! ${tipop} =~ ^[1-2]+$ ]]; do
		       echo "Please enter a valid operation:"
		       read tipop
		   done

		   echo -n "Insert Price : "
		   read price

		   while [[ ! ${price} =~ ^[0-9]+$ ]]; do
	               echo "Please enter a number:"
		       read price
		   done

		   echo -n "Insert Volume : "
		   read volumen

		   while [[ ! ${volumen} =~ ^[0-9]+$ ]]; do
		      echo "Please enter a number:"
	              read volumen
		   done


		   #Hacer el trade
		   $SRC/omnicore-cli -datadir=$DATADIR --regtest omni_tradecontract $ADDR 2147483651 $volumen 1 1 $price $tipop
		   $SRC/omnicore-cli -datadir=$DATADIR  --regtest generate 1

                   #SUM=$[ SUM + price ]
                   #N=$[ N + 1 ]
                   #PROM=$[ $SUM/$N ]
                   #echo "SUM : ${SUM}"
                   #echo "N : ${N}"
                   #echo "PROM: ${PROM}"

                   #for i in {1..10}
                   #do
                   #RAN=$[ RANDOM%3 + 1 ]
                   #RAN2=$[ RANDOM%2 ]
                   #nprice=$[ price + RAN ]
                   #nprice1=$[price - RAN ]
                   #echo "RAN : ${RAN}"
                   #echo "RAN2 : ${RAN2}"
                   #echo "nprice : ${nprice}"
                   #echo "nprice1 : ${nprice1}"
                   #if [ "$RAN2" == "0" ]
                   #then
                   #   TRA3=$($SRC/omnicore-cli -datadir=$DATADIR --regtest omni_tradecontract ${ADDRess[$i]} 2147483651 2 1 1 ${nprice1} 1)
                   #   #echo "Direccion : ${ADDRess[$i]}"
                   #elif [ "$RAN2" == "1" ]
                   #then
                   #   TRA3=$($SRC/omnicore-cli -datadir=$DATADIR --regtest omni_tradecontract ${ADDRess[$i]} 2147483651 1 1 1 ${nprice} 2)
                   #   #echo "Direccion : ${ADDRess[$i]}"
                   #fi
                   #$SRC/omnicore-cli -datadir=$DATADIR --regtest omni_sendgrant $ADDR ${ADDRess[$i]} 2147483652 2 "" >/dev/null
                   #$SRC/omnicore-cli -datadir=$DATADIR --regtest generate 1 >/dev/null
                   #done

		   #Obtengo el libro de ordenes de compra y venta
		   echo -e "\n"
		   printf "\x1b[31m    SELL ORDER BOOK\x1b[0m\n"
		   vendebook=$($SRC/omnicore-cli -datadir=$DATADIR --regtest omni_getcontract_orderbook 2147483651 2)
			 $SRC/omnicore-cli -datadir=$DATADIR --regtest omni_getcontract_orderbook 2147483651 2
		   total_rows_venta=`echo "$vendebook" | jq '. | length'`


		   printf "\x1b[31m Price         Volumen\x1b[0m\n"
		   idx2=0
		   while true; do

			if [ "$idx2" == "$total_rows_venta" ]
			then
			    break
			fi


                        amountforsale=`echo "$vendebook" | jq '.['"$idx2"']' | jq ."amountforsale"`
			effectiveprice=`echo "$vendebook" | jq '.['"$idx2"']' | jq ."effectiveprice"`
                        address=`echo "$vendebook" | jq '.['"$idx2"']' | jq ."address"`
			amountforsale=${amountforsale%.*}
			amountforsale="${amountforsale#\"}"
			effectiveprice=${effectiveprice%.*}
			effectiveprice="${effectiveprice#\"}"
                        address=${address%.*}
                        address="${address#\"}"
                        message="<--your order"
                        con='"'
                        AD=$ADDR$con
                        #echo "${AD}"
                        #echo "${address}"
                        if [ "${AD}" == "${address}" ]
                        then
                           printf "\x1b[31m\" $effectiveprice                 $amountforsale \" $message \x1b[0m\n"
                        else
			   printf "\x1b[31m\" $effectiveprice                 $amountforsale \"\x1b[0m\n"
                        fi
		        idx2=$((idx2+1))


		  done

		  echo -e "\n"
		  printf "\x1b[32m    BUY ORDER BOOK\x1b[0m\n"
      comprabook=$($SRC/omnicore-cli -datadir=$DATADIR --regtest omni_getcontract_orderbook 2147483651 1)
			$SRC/omnicore-cli -datadir=$DATADIR --regtest omni_getcontract_orderbook 2147483651 1
		  total_rows_compra=`echo "$comprabook" | jq '. | length'`

		  total_rows_compra=$((total_rows_compra+1))

                  printf "\x1b[32m Price         Volumen\x1b[0m\n"
		  idx=0
		  while true; do

			if [ "$idx" == "$total_rows_compra" ]
			then
			    break
			fi
		        amountforsalec=`echo "$comprabook" | jq '.['"$idx"']' | jq ."amountforsale"`
                        effectivepricec=`echo "$comprabook" | jq '.['"$idx"']' | jq ."effectiveprice"`
                        address1=`echo "$comprabook" | jq '.['"$idx"']' | jq ."address"`
                        amountforsalec=${amountforsalec%.*}
                        amountforsalec="${amountforsalec#\"}"
                        effectivepricec=${effectivepricec%.*}
                        effectivepricec="${effectivepricec#\"}"
                        address1=${address1%.*}
                        address1="${address1#\"}"
                        message="<--your order"
                        conc='"'
                        ADc=$ADDR$conc
                        #echo "${ADc}"
                        #echo "${address1}"
                        if [ "${ADc}" == "${address1}" ]
                        then
                           printf "\x1b[32m\" $effectivepricec                 $amountforsalec $message \x1b[0m\n"
                        else
                           printf "\x1b[32m\" $effectivepricec                 $amountforsalec \"\x1b[0m\n"
                        fi
		        idx=$((idx+1))
                 done
             fi

done
