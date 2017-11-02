var names = require('random-name')

var trades = []
var addresses = []

var averagePrice = 5500
var averageRange = 0.0025
var randomWalk = 0.0065

var tradeTotal = 10
var addrsTotal = 10

function getRandomInt(min, max) {
  min = Math.ceil(min);
  max = Math.floor(max);
  return Math.floor(Math.random() * (max - min)) + min; 
}

function generateAddressIdentifiers(total){
    for(var i = 0;i<total;i++){
        var name = names.first() + " " + names.place()
        console.log(addresses.push(name))
    }
}
generateAddressIdentifiers(addrsTotal)

function generateTrades(tradeTotal){
    for(var i = 0;i<tradeTotal;i++){
        var price = averagePrice*averageRange*Math.random()
        var address1 = getRandomInt(0,addresses.length)
        var address2 = getRandomInt(0,addresses.length)
        if(address1 == address2){ address2 = address1 + 1 }
        trades.push({"price": price, "addr1": addresses[address1], "addr2": addresses[address2]})
        var maxDrop = averagePrice - averagePrice * randomWalk
        var maxRise = averagePrice + averagePrice * randomWalk
        averagePrice = getRandomInt(maxDrop,maxRise)
    }
}

function generateAddresfArray(tradeTotal){
    var p = []
    for(var i = 0; i < trades.length; i++) {
        var obj = trades[i];
	p.push(obj.addr1);
    }
    return p;
}

function calculateSettlements(tradeTotal){    

    var vector = generateAddresfArray(tradeTotal);
    vector.sort();
    var current = null;
    var cnt = 0;
    
    for (var i = 0; i < vector.length; i++) {
    	if (vector[i] != current) {
            if (cnt > 0) {
                console.log(current + ': Traded --> ' + cnt + ' times');
            }
            current = vector[i];
            cnt = 1;
        } else {
            cnt++;
        }
    }
    if (cnt > 0) {
        console.log(current + ': Traded --> ' + cnt + ' times');
    }   
}

setTimeout(function(){
    generateTrades(tradeTotal)
    calculateSettlements(tradeTotal)
    setTimeout(function(){ 
        console.log(trades)
    },5000)
},1500)
