this is a project to create a maiga joystick interface for usb, to use a retro amiga joystick with a modern pc and a amiga emulator.
it has two ports for two joysticks, and uses the arduino micro based on the atmega32u4 which have native usb suport and can present as a game class device.


io is as follows:


gpio 2  joystick 1, button 1
gpio 3  joystick 1, right
gpio 4  joystick 1, left
gpio 5  joystick 1, up
gpio 6  joystick 1, down

gpio a3  joystick 2, button 1
gpio a2  joystick 2, right
gpio a1  joystick 2, left
gpio a0  joystick 2, up
gpio 15  joystick 2, down


