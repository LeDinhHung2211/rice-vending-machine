var host = "broker.emqx.io";
var port = 8083;
var topic1 = "data/dataConfig";
var topic2 = "data/sensorData";


// Create mqtt client1 
var client = new Paho.MQTT.Client(host, port, "tan");

client.destinationName = topic1;
client.qos = 1;
client.retained = true;

// set callback handlers
client.onConnectionLost = onConnectionLost;
client.onMessageArrived = onMessageArrived;

// connect the client
client.connect({onSuccess:onConnect});


// called when the client connects
function onConnect() {
  // Once a connection has been made, make a subscription and send a message.
  console.log("onConnect");
  client.subscribe(topic1);  

}

var count =0;
function publish_client1()
{
    var green = document.getElementById("green_id").value;
    var white = document.getElementById("white_id").value;
    var others = document.getElementById("others_id").value;
    if (green == "" || white == "" || others == "")
      alert("Wrong format!");   
    else
    {
      if (green <0|| green > 255)
          alert("Wrong Green!");
      else if (white <0|| white > 255)
          alert("Wrong White");
      else if (others <0|| others > 255)
          alert("Wrong Others!");
      else{
          message = new Paho.MQTT.Message(green +":"+ white +":"+ others);
          message.destinationName = topic1;
          message.qos = 1;
          console.log(message);
          client.send(message);
        }
    }
}

// called when the client loses its connection
function onConnectionLost(responseObject) {
  if (responseObject.errorCode !== 0) {
    console.log("onConnectionLost:"+responseObject.errorMessage);
  }
}

// called when a message arrives
function onMessageArrived(message) {

    
    console.log("onMessageArrived from " + topic1 + ":" + message.payloadString);
}
//---- End client 1 -------//



// Create mqtt client2
var client2 = new Paho.MQTT.Client(host, port, "tan_2");

client2.destinationName = topic2;
client2.qos = 1;
client2.retained = false;

// set callback handlers
client2.onConnectionLost = onConnectionLost_2;
client2.onMessageArrived = onMessageArrived_2;

// connect the client2
client2.connect({ onSuccess: onConnect2 });


// called when the client2 connects
function onConnect2() {
  // Once a connection has been made, make a subscription and send a message.
  console.log("onConnect");
  client2.subscribe(topic2);

}

// var count2 = 0;
function publish_client2() {
  message = new Paho.MQTT.Message('{"color":"white","weight":200}');
  // count2++;
  message.destinationName = topic2;
  message.qos = 1;
  client2.send(message);
}

// called when the client2 loses its connection
function onConnectionLost_2(responseObject) {
  if (responseObject.errorCode !== 0) {
    console.log("onConnectionLost:" + responseObject.errorMessage);
  }
}

// called when a message arrives
function onMessageArrived_2(message) {
    var rawData = message.payloadString;
    const obj = JSON.parse(rawData);

    document.getElementById("bien_color_id").value= obj.color; 
    document.getElementById("bien_kl_id").value= obj.weight;
    // console.log("onMessageArrived from " + topic2 + ":" + message.payloadString);
    console.log("color: " + obj.color + " kl: " + obj.weight)




    if(obj.color=='green')
    {
      document.getElementById("green_chart_id").value= ++rgb[0]; 
      document.getElementById("white_chart_id").value= rgb[1];
      document.getElementById("others_chart_id").value= rgb[2];
    }

    else if(obj.color=='white')
    {
      document.getElementById("green_chart_id").value= rgb[0]; 
      document.getElementById("white_chart_id").value= ++rgb[1];
      document.getElementById("others_chart_id").value= rgb[2];
    }

    else if(obj.color=='others')
    {
      document.getElementById("green_chart_id").value= rgb[0]; 
      document.getElementById("white_chart_id").value= rgb[1];
      document.getElementById("others_chart_id").value= ++rgb[2];
    }

    $(document).ready(function(){
      //code xu ly form
      //sql config  
      // $(document).on('submit','#form',function(){
      //     $.post("addData_config.php",$('#form').serialize(), function(){
      //         // alert("data sent")
      //     });
      //     return false;
      // });
      //sql pi
      $.post("add_value_box.php",$('#value_chart').serialize(), function(){
          // alert("abc")
      });
      return false;
      // code update table
      
    });

      
      
      
    

    $(document).ready(function(){
      //code xu ly form
      //sql config  
      // $(document).on('submit','#form',function(){
      //     $.post("addData_config.php",$('#form').serialize(), function(){
      //         // alert("data sent")
      //     });
      //     return false;
      // });
      //sql pi
      $.post("addData_ras.php",$('#ax_test').serialize(), function(){
          // alert("abc")
      });
    return false;
      // code update table
      
  });
    
}
//---- End client 2 -------//