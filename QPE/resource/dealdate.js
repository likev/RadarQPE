var content = $("#content-text").val();
//console.log(content);

var words = content.split(/\s+/);
console.log(words[words.length-1]);

var result = '';
for(var i=0; i<words.length;i+=4)
{
  var date = new Date(1970,0,words[i],8,0,0,words[i+1]);
//console.log(date)

  result += moment(date).format("YYYY-M-D HH:mm:ss, ")
  result += words[i+3]+'<br>';
}
$("#alert-content").html(result);


//-----------------------------------------------------
var content = $("#content-text").val();
$("#help-text").text(content).hide();
//-----------------------------------------------------
var rainstr = $("#help-text").text();

var words = rainstr.split(/[\n,]/);
//console.log(words[words.length-1]);

var rain = {};
for(var i=0; i<words.length;i+=3)
{
   rain[ words[i] ] = words[i+2];
}

var getR = function(min, hour3str){
  
  var spans = 12;
  
  var start = 120+min*2-spans;
    var mstr = hour3str.slice(start, start+ 2*spans);
  
  //console.log(mstr);
  
  var avgrain = 0;
  for(var i = 0; i<2*spans; i+=2){
    avgrain += +mstr.slice(i,i+2)
  }
  //console.log(avgrain);
  
  return avgrain*6/spans ;
}

var getRfromDate = function(datestr){
    
  var dateradar = moment(datestr);
  var min = dateradar.minute();
  //console.log(min);
  
  var daterain = moment(dateradar);
  
  //daterain.set({'minute': 0, 'second': 0});
  
  var h3str = rain[daterain.format("YYYY-MM-DD HH:00:00") ];
  daterain.add(1, 'hour');
  h3str += rain[daterain.format("YYYY-MM-DD HH:00:00") ];
  
  daterain.add(1, 'hour');
  
  h3str += rain[daterain.format("YYYY-MM-DD HH:00:00") ];
  
  return getR(min, h3str );
}

//var h3str = rain['2015-10-25 14:00:00']+rain['2015-10-25 15:00:00']+rain['2015-10-25 16:00:00'];
//var r = getR(0, h3str );
//$('#alert-edit-date').text(h3str);
//console.log(r);

console.log(getRfromDate('2015-10-25 15:01:00'));

var content = $("#content-text").val();
//console.log(content);

words2= content.split(/\s+/);
console.log(words2[words2.length-1]);

var result = '';
for(var i=0; i<words2.length;i+=4)
{
  var date = moment(new Date(1970,0,words2[i],8,0,0,words2[i+1]) );
   var avgrain = getRfromDate(date);
  
  if(avgrain === 0) continue;

  result += date.format("YYYY-M-D HH:mm:ss, ")
  result += words2[i+2]+', ';
  
 
  result += avgrain + ', ';
  result += Math.log10(avgrain) + '<br>';
}
$("#alert-content").html(result);