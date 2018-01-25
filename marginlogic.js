function marginLogic(CMPcontractDex*, CMPcontractDexMatch*, execPrice){

// Notas:
// contratos ejecutados: contratos vendidos o comprados netos (en nuestro caso nCouldBuy)
// contratos cerrados (neteados): contratos vendidos o comprados sin que el balance cruce el cero

//se obtienen los datos necesitados//////////////////////////////////////////////////////////////////////////////////////////////////
        var thisMakerTrade  = CMPcontractDex*
        var thisTakerTrade = CMPcontractDexMatch*
        var thisContract = thisMakerTrade.propertyid
        var buyAddress = “”
        var sellAddress = “”
        var amount = thisTakerTrade.amount
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// en esta parte sólo se asigna quien es el comprador y quien es el vendedor/////////////////////////////////////////////////////////
       if(thisMakerTrade.type == “buy”){
           buyAddress = thisMakerTrade.address
           sellAddress = thisTakerTrade.address
       }else{sellAddress = thisMakerTrade.address, buyAddress = thisTakerTrade.address}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        var contractInfo = CMPcontract.getContractInfo(propertyid)
        var sellAddress =
        var marginID = CMPcontract.marginProperty
        var oldBalanceBuyer = getBalancesForAddress(buyAddress, thisContract) //en nuestro caso, sacamos la informacion de los balances con la funcion getMPbalance
        var oldBalanceSeller = getBalancesForAddress(sellAddress, thisContract)
    //apply clean math from the signed int provided as parameter
        var newBalanceBuyer = oldBalanceBuyer + contractsExecuted // en nuestro caso el balance de contratos nuevo del comprador será su balance viejo más los contratos que compró (contratos ejecutados)
        var newBalanceSeller = oldBalanceSeller - contractsExecuted // balance de contratos nuevo para el vendedor será el balance viejo menos los contratos que se vendieron (contratos ejecutados)
     //set-up logic about closing gains/losses or not
        var contractsClosedBuyer = countClosed(oldBalanceBuyer,newBalanceBuyer) // contratos neteados para el comprador
        var contractsClosedSeller = countClosed(oldBalanceSeller,newBalanceSeller) // contratos neteados para el vendedor

//razonamiento respecto al trade
if(contractsClosedBuyer>0&&contractsClosedSeller>0){
	if(contractsClosedBuyer>contractsClosedSeller){
                  //match function assigns one status for the mutual amount of closing  and another for the remainder on the part of the other party
            }else if(contractsClosedSeller>contractsClosedBuyer){
                  //visa versa of the above
            }else{//both netted}
}else if(contractsClosedBuyer>0&&contractsClosedSeller=0){
      //match function assigns longNetted
}else if(contractsClosedBuyer=0&&contractsClosedSeller>0{
     //both trades/matches assigned shortNetted
}else{//match function assigns live status to both }

      //calculate reserve to free pre-loss
    var freedReserveExPNLMaker = thisMakerTrade.marginRequirement *contractsClosed // the money from RESERVE to BALANCE when contracts are netted (or closing)
     var freedReserveExPNLTaker = thisTakerTrade.marginRequirement * contractsClosed
     var makerLiveContracts = CMPcontract.getMyContracts(thisMakerTrade.address, thisContract) //calls function to return map //of contracts not netted on this address
     var takerLiveContracts = CMPcontract.getMyContracts(thisTakerTrade.address, thisContract)

     var contractsToNet	Maker = filterContracts(makerLiveContracts, contractsClosedMaker,thisMakerTrade) // ???

     var averageCostBasisMaker = getAverageBasis(contractsToNetMaker) // se calcula el valor promedio de cada contrato que posea el maker
     var contractsToNetTaker = filterContracts(takerLiveContracts, contractsClosedTaker,thisTakerTrade)
     var averageCostBasisTaker = getAverageBasis(contractsToNetTaker) // se calcula el valor promedio de cada contrato que posea el taker

     var PNLMaker = execPrice - averageCostBasis * contractsClosed // PNL del maker, excePrice is the price in the Match moment
     var oldReserveMaker = TallyMap.retrieveReserve(marginid,thisAddress)// RESERVE del maker
     var newReserveMaker = oldReserve - (freedReserveExPNL-Math.ABS(PNL))  // se hacen los descuentos al RESERVE del maker (se descuenta el freedReserveExPNL: que es el requerimiento de margen que se pide al principio.También se descuenta o agrega el PNL)
     TallyMap.updateReserve(newReserve,marginid,thisMakerTrade.address) // Se actualiza el RESERVE del maker
     var oldMarginBalanceMaker = TallyMap.retrieveBalance(marginid,thisMakerTrade.address)
     var newMarginBalanceMaker = oldMarginBalance+freedReserveExPNLMaker-Math.abs(PNL) //Cuenta de balances o cuenta de márgenes? cuál es la diferencia? (preguntar a Daniel)
     //modify TallyMap for new contract balances and margin balances
     TallyMap.updateBalance(newBalance, thisContract,thisAddress)
     TallyMap.updateBalance(newMarginBalance, marginid, thisAddress)
}



//Funciones requeridas ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


function countClosed(oldBalance, newBalance){  // esta función determina cuánto baja una posición (ya sea long o short) cuando se netean contratos

if(Math.abs(oldBalance)>Math.abs(newBalance)&&amount<oldBalance){  // si se reduce el balance pero no cruza el cero ->los contratos cerrados son sólo la diferencia
      contractsClosed = oldBalance-newBalance
}else if(Math.abs(contractsExecuted)>Math.abs(oldBalance)&&contractsExecuted+oldBalance<0){
  // caso donde el balance de contrato positivo se reduce bajo cero
      contractsClosed = oldBalance  // consideramos sólo el monto hasta el cruce de cero
}else if(Math.abs(contractsExecuted)>Math.abs(oldBalance)&&contractsExecuted+oldBalance>0){
     //caso donde el balance negativo sube sobre cero
      contractsClosed = oldBalance // consideramos sólo el monto hasta el cruce de cero
}





Function filtercontracts(contractsArray, amountToNet, originalTransaction){ // esta función aún no la entiendo
           nettedContracts = []
	for(var a = 0; a<contractsArray.length; a++){
		var thisContract = [a]
		Var liveAmount = thisContract[1]
		 //look for the newest live contracts by blockheight, add up to amountToNet
if(//fits priority criteria && liveAmount<=amountToNet)
{nettedContracts.push(thisContract), amountToNet-liveAmount
//call function to update thisContract[0] (the trade id) with new liveContract info
}else if(//fits priority && liveAmount>amountToNet){
thisContract[1] = liveAmount - amountToNet
nettedContracts.push(thisContract)
//call function to update thisContract[0] (the trade id) with new liveContract info, push this trade id to the list of netted transactions for the original transaction
}
return nettedContracts
	}


function getMyContracts(thisAddress, thisContract){  // Por lo que se ve, toma los live contracts, considerando el blockheight (el tiempo), dada una dirección y un tipo de contrato (thisContract)
  var blockheight = rpcCallGetCurrentBlockHeight() // se revisa en qué bloque estamos ahora
  var liveContracts = []
  //get the data needed
  gettradehistoryforaddress(thisAddress, thisContract,  function(data){
  data.sort(data[a].blockheight,data[b].blockheight){return a-b} // se cuenta cuántos bloques lleva el contrato activo?
  for(var i = 0; i<data.length;i++){
    var thisTrade = data[i]
    var tradetime = thisTrade.blockheight
  if(thisTrade.blockheight == blockheight){//exclude}else{
    var theseMatches = thisTrade.Matches
    for(var m =0; m<theseMatches.length;m++){
      var thisMatch = theseMatches[m]
      var execPrice = thisMatch[1]
      var amountLive = thisMatch[3]
      if(amountLive>0){
        liveContracts.push(thisTrade.id, amountLive, execPrice)
      }
    }
  }
  		}
  })
  return liveContracts
  }

function getAverageBasis(myContracts){ // se suma la multiplicación de los contratos y su respectivo precio de ejecución (cuando ocurrió el match) execPrice*liveAmount
  	vars pricesVolumes = []
    for(var i = 0; i<myContracts.length;i++){
  		 var execPrice = myContracts[i][1]  // se sacan de la base de datos
  		 var liveAmount = myContract[i][3]
  		pricesVolumes.push([execPrice, liveAmount])
  	}
    var weightedSum = 0
  	for(var p = 0; p<pricesVolumes.length;p++){ // se hace la sumatoria de  execPrice(i) * liveAmout(i)
  			weightedSum += pricesVolumes[p][0]* pricesVolumes[p][1]
  	}
    return weightedSum //se devuelve la suma
  }

// idea: cuando se da el caso NONE (cuando la posicion se agranda)en el x_trade() calcular :int64_t averageBasis = execPrice*liveAmount;
//                                 update_tally_map(sender,getProperty(),averageBasis,BASIS);
