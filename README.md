# Remote Control Project
The goal of our project was to make a distance elevator calling and light control. In future implementation, it was thought as a software & hardware, working as a mobile application or a website for your house smart control. No matter where you are, you can control your IoT devices from there, with just mobile data or wi-fi access. So, we created prototype, and tested it on distance control of Block #7 elevator and light in our lab.
The whole project contains Arduino scheme, with 2 servo motors, and 2 leds, Remote Server, web client and mobile application.
  
#  How it works
The user is interacting with mobile application or website (elevator.nudl.kz). On each command, client sends a POST request to the remote server on one of the few routes. Server is hosted and accessed from any IP address via network. When some changes came from a client, server changes it in its database. And, because Arduino Module W5100 and server are listening to each other, because of requests sent by module every 0.5-1 seconds (as a loop() method implemented), the Arduino scheme gets data, and knows if some of the LEDs or motors were just turned on. The data comes as a xml file, stored locally on server. Then this xml is parsed in C, and the needed data is retrieved. Based on that the scheme passes HIGH or LOW to whatever it should.
    
#    Attached files
index.php – main server file, written on PHP. It redirects the coming requests and responses on the GET route.
xmlupdate.php – server file, responses on client requests to change data in xml file
test.xml – file, containing all information regarding states of the motors and LEDs
project.ino – file, containing the code for wi-fi module, motors and LEDs. It connects module to the internet, makes get requests to the server. Gets the response as xml file, then parses it, and according to the retrieved data turns on/off light/motors.
elevator – package, containing code for android application for remote control

