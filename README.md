# ESP32 Gameconsole


## Display
The display uses a background buffer. Everything is written inside this buffer. The screen is only updated after a call to `Display::update_screen()`. However, it is possible to configure the Display to run this function periodically to keep the frame rate the same in each iteration.