# Arduino-musical-lightshow

This is project I worked on a few years ago, It was inspired by this project I found online:
https://maker.pro/arduino/projects/christmas-light-show-with-arduino

It's a christmas light show with arduino. The system works with a MIDI file stored in a SD card. The data on the SD card gets read by the arduino program, which converts each note to a corresponding integer 0-23, which then maps to an output of one of the shift registers. The signals from the shift registers activate a relay which turns on a light connected to. Furthemore, I made a secondary circuit that connects with the main one using a bluetooth module, and uses an ultrasound sensor to detect the people passing by to activate the show.

# Here are some images of the results:
<img src="https://github.com/user-attachments/assets/09470eca-5f52-4239-9a4e-02f0d2abb6a4" alt="drawing" width="200"/>
<img src="https://github.com/user-attachments/assets/add61727-73d2-4a29-9ff9-eb817ccec4a6" alt="drawing" width="200"/>
<img src="https://github.com/user-attachments/assets/8e82802e-13f0-442f-937f-1f5e42108dc6" alt="drawing" width="200"/>
<img src="https://github.com/user-attachments/assets/879d4179-30d7-4f46-a981-cfb958cd1907" alt="drawing" width="200"/>
<video src='https://github.com/user-attachments/assets/a95a327e-0200-415a-b9ee-fcf9cb70deff' width=200/>
