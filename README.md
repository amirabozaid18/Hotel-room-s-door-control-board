# Hotel-room-s-door-control-board
this is an individual project using TM4C123GH6PM microcontroller and simulated on Keil 
this project merges both polling and interrupt Algorithms 
Briefly, we have 4 basic components: TM4C123GH6PM microcontroller, PC, Keypad, Solenoid
First, the PC sends room numbers via UART to the microcontroller which controls these rooms
then we have 3 possible scenarios: checkin, room cleaning, or checkout.
in case of checkin, the user uses keypad to enter room password
if the password entered by user matches with correct one, soleniod is unlocked and green led turns on
if the password entered by user didn't match with correct one, selonoid is still locked and red led tuens on
