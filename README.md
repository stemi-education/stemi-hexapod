# STEMI hexapod #

Library that includes everything you need to get your STEMI hexapod walking and dancing using your mobile app :)

### Prepare your Arduino IDE for programming of Arduino Due ###

1. Install Arduino IDE.
2. Open Arduino IDE and go to: Tools>Board>Boards Manager
3. Type in "due" and install "Arduino SAM Boards(32-bits ARM Cortex-M3)"
4. Choose the installed board in "Tools>Board>Arduino Due (programming port)"
5. Connect your STEMI hexapod to the computer using USB cable.
6. Select the port your robot is connected to in "Tools>Port>COMX (Arduino Due (programming port))"

### Use the STEMI hexapod library ###

1. Download the latest release of the library.
2. Unzip and put the "stemi_hexapod" folder in your "*/Arduino/libraries" folder.
3. Open Arduino IDE.
4. Open the example code in Arduino IDE: "File>Examples>STEMIhexapod>STEMIhexapodWifi".
5. Upload the code using "Sketch>Upload"
6. After the upload is complete, use STEMI mobile app to controll the robot.