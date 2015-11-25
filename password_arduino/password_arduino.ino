/* Arduino Password
 * 
 * This program reads a password sequence from
 * the sensors on the Esplora, and if the sequence
 * is correct, it unlocks.
 *
 * Author: Owen Brasier
 * Date: November 2015
 */

#include <Esplora.h>

#define SIZE 6  // the number of buttons in our password

const int password[SIZE] = {4, 3, 2, 1, 2, 1}; // this is the password - the button sequence

// canRead checks if each button can be read
// canRead[0] == canRead button 1 etc.
// The reason we have that when you push down a button, it will read it 
// many times before you let go of the button
// having the booleans means once we read the button once
// we STOP reading it until the button is released.
// you could add more if you wanted to read different sensors
bool canRead[] = {true, true, true, true};     

int count = 0;
bool unlock = false; // unlock the arduino if the password is correct

void setup() {
  Serial.begin(9600);
  reset();
}

void loop() {
  if (unlock) {
    unlocked();
  }
  else {
    checkPassword();
  }
}

/* 
 * Reset to start the password again
 */
void reset() {
  Serial.println("Resetting, try again!");
  count = 0;
  unlock = false;
  Esplora.writeRGB(255, 0, 0);
}

/* checkButton function
 * Check to see if the correct button is pushed,
 * and if so increment count
 */
void checkButton(int n) {
  // check if the CAN read that button 
  // n is the button number, n-1 is the INDEX of the canRead array
  // with the corresponding button
  if (Esplora.readButton(n) == LOW && canRead[n-1]) { 
    canRead[n-1] = false;
    if (password[count] == n) {   // check this button is the correct one in the sequence
      count++;
      Serial.print("count is: "); // sanity print statement
      Serial.println(count);
    }
    else {   // this is NOT the correct one in the sequence, reset
      reset();  
    }
  }
  else if (Esplora.readButton(n) == HIGH) { // the button we're checking is RELEASED - we can now read it again
    canRead[n-1] = true;
  }
}

/*
 * This is the checkPassword function... we read through all the sensors
 * we want. In this case, we check each of the buttons.
 */
void checkPassword() {
  // loop through all the buttons
  for(int i = 1; i <= 4; i++) {
    checkButton(i);
  }
  if (count == SIZE) { // We got the correct sequence - UNLOCK!
    unlock = true;
    Esplora.writeRGB(0, 255, 0);
  }
}

/*
 * We are unlocked, now you can actually run the code here.
 */ 
void unlocked() {
  Serial.println("Unlocked!");
}

