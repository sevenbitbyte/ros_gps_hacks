var NMEA = require('nmea')
var ROSLIB = require('roslib')

var seq=0;
var ros = new ROSLIB.Ros();
var uri='ws://localhost:4000';

ros.connect(uri);

ros.on('error', function(error) {
  console.log('error: ' + error)
});

ros.on('close', function() {
  console.log('closed')
});

ros.on('connection', function() {
  //connection
  console.log('connected')

  nmeaTopic = new ROSLIB.Topic({
    ros: ros,
    name: 'nmea_sentence',
    messageType: 'nmea_msgs/Sentence'
  })


  var lineReader = require('readline').createInterface({
    input: require('fs').createReadStream('034e112708ea79cd_20160202.nmea')
  });

  lineReader.on('line', function (line) {

    if(line.length > 0) {
      try{
        var nmeaPkt = NMEA.parse(line)
        //console.log('Line from file:', JSON.stringify(nmeaPkt, null, 2));

        var nmeaMsg = {
          header : {
            seq : seq++,
            frame_id : 'odom',
            stamp : {
              secs : 0,
              nsecs : 0
            }
          },
          sentence: line
        };

        nmeaTopic.publish(new ROSLIB.Message(nmeaMsg));
      }
      catch(exec){

      }
    }
  });


});
