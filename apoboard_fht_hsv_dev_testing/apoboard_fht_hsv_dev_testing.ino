//Beta code for the DIY interactive electronics Apogaea PCB. Based heavily on the on the Open Music Labs Arduino FHT library (http://wiki.openmusiclabs.com/wiki/ArduinoFHT)
//This code was developed for the Apogaea workshop by the Soundpuddle crew (soundpuddle.org), May 2013

#define LOG_OUT 1 // use the log output function
#define FHT_N 256 // set to 256 point fht


#include <FHT.h> // include the library

#include <avr/interrupt.h>

//Declarations for HSV function
float H,S,L,Rval,Gval,Bval;
void HSL(float H, float S, float L, float& Rval, float& Gval, float& Bval);

//variables for smoothing function (i.e. low pass filter for RGB color channels)
float rsmoothfactor = 0.8; //this values controls the strength of smoothing. 1=no smoothing, 0=infinite smoothing
float gsmoothfactor = 0.8; //this values controls the strength of smoothing. 1=no smoothing, 0=infinite smoothing
float bsmoothfactor = 0.8; //this values controls the strength of smoothing. 1=no smoothing, 0=infinite smoothing
float Rvalsmooth;
float Gvalsmooth;
float Bvalsmooth;

float largestbin;
float largestvalue;

//define the physical pins connected to the LEDs and momentary button
int redpin = 3;
int greenpin = 9;
int bluepin = 10;
int buttonpin = 2;

//variable to keep track of what mode we're in (controlled by the momentary button)
int mode = 1;

//variable to keep track of our position in HSV color space for the rainbow cycle (mode 1)
float rainbowhue = 0;

//declarations for button debounce function
long debouncing_time = 15; //Debouncing Time in Milliseconds
volatile unsigned long last_micros;

//void setup is run once at boot time, to configure the microcontroller
void setup() {
  Serial.begin(115200); // use the serial port
  
  //declarations for the FFT functions
  TIMSK0 = 0; // turn off timer0 for lower jitter
  ADCSRA = 0xe5; // set the adc to free running mode
  ADMUX = 0x40; // use adc0
  DIDR0 = 0x01; // turn off the digital input for adc0

//configure the physical pins of the microcontroller for the LEDs and momentary button
  pinMode(redpin, OUTPUT);
  pinMode(greenpin, OUTPUT);
  pinMode(bluepin, OUTPUT);
  pinMode(buttonpin, INPUT);
  digitalWrite(buttonpin, HIGH);

  // attach our interrupt pin to it's ISR
  attachInterrupt(0, buttonpress, FALLING);

}

//void loop is the main while loop for the program
void loop() {

  //Mode 0 is the primary sound-reactive code, using the FFT function
  if (mode == 0) {

    while(1) { // reduces jitter
      cli();  // UDRE interrupt slows this way down on arduino1.0
      for (int i = 0 ; i < FHT_N ; i++) { // save 256 samples
        while(!(ADCSRA & 0x10)); // wait for adc to be ready
        ADCSRA = 0xf5; // restart adc
        byte m = ADCL; // fetch adc data
        byte j = ADCH;
        int k = (j << 8) | m; // form into an int
        k -= 0x0200; // form into a signed int
        k <<= 6; // form into a 16b signed int
        fht_input[i] = k; // put real data into bins
      }
      fht_window(); // window the data for better frequency response
      fht_reorder(); // reorder the data before doing the fht
      fht_run(); // process the data in the fht
      fht_mag_log(); // take the output of the fht
      //    fht_mag_lin();
      sei();
      //    Serial.write(255); // send a start byte
      //    Serial.write(fht_log_out, FHT_N/2); // send out the data


      //Determine the bin with the greatest value
      largestbin = 0;
      largestvalue = 0;
      for (int i = 12; i < FHT_N/4; i++) {
        if (fht_log_out[i] > largestvalue) {
          largestbin = i; 
          largestvalue = fht_log_out[i];
        }
        //    Serial.print(fht_log_out[i]);
        //    Serial.print(";");

      }

      //print out the results over the serial port (for debugging purposes)
      Serial.print(largestbin);
      Serial.print("---");
      Serial.print(largestvalue);
      Serial.print("---");
      Serial.print(mode);
      Serial.print("---");

      //determine which bin the largest, and what is the value of that bin?
      largestbin = largestbin / (FHT_N/4);
      largestbin = largestbin * 1.5;
      if (largestbin > 1) {
        largestbin = 1;
      }
      largestvalue = largestvalue / 1024;
      if (largestvalue > 1) {
        largestvalue = 1;
      }
      S=1;
      
    //convert HSV color space to RGB color space
      HSL(largestbin,S,largestvalue,Rval,Gval,Bval);

      ///Run the RGB color channels through the low pass filter
      Rvalsmooth = (Rvalsmooth * (1-rsmoothfactor)) + (rsmoothfactor * Rval);
      Gvalsmooth = (Gvalsmooth * (1-gsmoothfactor)) + (gsmoothfactor * Gval);
      Bvalsmooth = (Bvalsmooth * (1-bsmoothfactor)) + (bsmoothfactor * Bval);

      //Write the RGB color channel values to the PWM driver for each pin
      analogWrite(redpin, Rvalsmooth);         
      analogWrite(greenpin, Gvalsmooth);         
      analogWrite(bluepin, Bvalsmooth);

      Serial.println();
      if (mode != 0) {
        break;
      }

    }
  }

  //Mode 1 is a simple cycle through a rainbow in HSV color space
  else if (mode == 1) {
    
    //increment through the hues of HSV color space (range from 0 - 1)
    S = 0.99;
    L = 0.5;
    rainbowhue = rainbowhue + 0.001;
    
    //wrap around back to zero when >1
    if (rainbowhue >= 1) {
      rainbowhue = 0;
    }
    
    //convert HSV color space to RGB color space
    HSL(rainbowhue,S,L,Rval,Gval,Bval);

    ///Run the RGB color channels through the low pass filter
    Rvalsmooth = (Rvalsmooth * (1-rsmoothfactor)) + (rsmoothfactor * Rval);
    Gvalsmooth = (Gvalsmooth * (1-gsmoothfactor)) + (gsmoothfactor * Gval);
    Bvalsmooth = (Bvalsmooth * (1-bsmoothfactor)) + (bsmoothfactor * Bval);

    //Write the RGB color channel values to the PWM driver for each pin
    analogWrite(redpin, Rvalsmooth);         
    analogWrite(greenpin, Gvalsmooth);         
    analogWrite(bluepin, Bvalsmooth);
    
    //Pause the program to slow down the visual display
    delay(100);
  }

//print system information over the serial port (for debugging purposes)
  Serial.print(largestbin);
  Serial.print("---");
  Serial.print(largestvalue);
  Serial.print("---");
  Serial.print(";");
  Serial.print(Rval);
  Serial.print(";");
  Serial.print(Gval);
  Serial.print(";");
  Serial.print(Bval);
  Serial.print(";");
  Serial.print(rainbowhue);
  Serial.print(";");
  Serial.print(mode);
  Serial.println();

}

//function to convert HSV color space to RGB color space
void HSL(float H, float S, float L, float& Rval, float& Gval, float& Bval)
{
  float var_1;
  float var_2;
  float Hu=H+.33;
  float Hd=H-.33;
  if ( S == 0 )                       //HSL from 0 to 1
  {
    Rval = L * 255;                      //RGB results from 0 to 255
    Gval = L * 255;
    Bval = L * 255;
  }
  else
  {
    if ( L < 0.5 ) 
      var_2 = L * ( 1 + S );
    else           
      var_2 = ( L + S ) - ( S * L );

    var_1 = 2 * L - var_2;

    Rval = round(255 * Hue_2_RGB( var_1, var_2, Hu ));
    Gval = round(255 * Hue_2_RGB( var_1, var_2, H ));
    Bval = round(255 * Hue_2_RGB( var_1, var_2, Hd ));
    
    if (Rval < 0) {Rval = 0;}
    if (Gval < 0) {Gval = 0;}
    if (Bval < 0) {Bval = 0;}
  }

}

//function to debounce the switch input (e.g. prevent accidently count one physical button press multiple times)
void debounceInterrupt() {
  if((long)(micros() - last_micros) >= debouncing_time * 1000) {
    buttonpress();
    last_micros = micros();
  }
}

//function for what to do when a button press occurs
void buttonpress()
{
  mode++;
  Serial.print("button press"); 
  Serial.print("  mode ="); 
  Serial.print(mode); 
  Serial.println("");
  if (mode >= 2) {
    mode = 0;
  }
  //  delay(300);
}

//helper code for the HSL function
float Hue_2_RGB( float v1, float v2, float vH )             //Function Hue_2_RGB
{
  if ( vH < 0 ) 
    vH += 1;
  if ( vH > 1 ) 
    vH -= 1;
  if ( ( 6 * vH ) < 1 ) 
    return ( v1 + ( v2 - v1 ) * 6 * vH );
  if ( ( 2 * vH ) < 1 ) 
    return ( v2 );
  if ( ( 3 * vH ) < 2 ) 
    return ( v1 + ( v2 - v1 ) * (.66-vH) * 6 );
  return ( v1 );
}

//very simple low-pass filter function
int smooth(int data, float filterVal, float smoothedVal){

  if (filterVal > 1){      // check to make sure param's are within range
    filterVal = .99;
  }
  else if (filterVal <= 0){
    filterVal = 0;
  }

  smoothedVal = (data * (1 - filterVal)) + (smoothedVal  *  filterVal);

  return (int)smoothedVal;
}


