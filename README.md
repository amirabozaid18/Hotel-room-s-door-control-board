# Hotel-room's door control board
This is an individual project using TM4C123GH6PM microcontroller and simulated on Keil.
This project merges both polling and interrupt Algorithms.
Briefly, we have 4 basic components: TM4C123GH6PM microcontroller, PC, Keypad, Solenoid.
First,the PC sends room numbers via UART to the microcontroller which controls these rooms, this is setup mode.
Then we have 3 possible scenarios: checkin, room cleaning, or checkout.
In case of checkin, the user uses keypad to enter room password,
if the password entered by user matches with correct one, soleniod is unlocked and green led turns on,
else if the password entered by user didn't match with correct one, selonoid is still locked and red led tuens on.
In case of room cleaning, the solenoid is unlocked without asking for password and green led turns on.
In case of checkout, the solenoid is locked whether you entered a correct password, wrong password, or didn't enter any thing.
hjjkihhjki
