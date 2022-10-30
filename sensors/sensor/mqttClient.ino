// /**
// * MQTT Subscriptions 
// */
// const maxTrySendMessage = 5;

// void onConnectionEstablished() {

//   client.subscribe("identificativoAUBBIALI/startRotation", [](const String& payload) {
//     if (payload == "RIGHT") {
//       checkDistance = true;
//       distanceSensor();
//     } else if (payload == "LEFT") {
//       checkDistance = true;
//       distanceSensor();
//     }
//   });

//   client.subscribe("identificativoAUBBIALI/stopRotation", [](const String& payload) {
//     checkDistance = false;
//   });
// }

// bool sendDistanceSensorStarted() {
//   sendMessage("identificativoAUBBIALI/distanceSensor", "STARTED");
// }

// bool sendMessage(String topic, String msg){
//   bool sended = false;
//   int count = 0;
  
//   while(!sended && count < maxTrySendMessage ) {
//     sended = client.publish(topic, msg);
//     count++;
//   }

//   return sended
// }