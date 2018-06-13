#include "Arduino.h"
#include <ArduinoJson.h>
#include <String.h>

class zone{
  
   public: 
   int zoneid;
    
   unsigned long start_time;
   unsigned long irrigation_time;
   unsigned long  pause_time;
   unsigned long effective_time;
   boolean newsettings;

   unsigned long last_finishedtime;  // to have a log for the finish time. 
   boolean irr_finished; // t/f to see if the irrigation is completed
   boolean irr_start; // t/f to indicate if its time to start
   boolean irrigating; // to see if its irrigating
   int effective; // current effective irrigatin time reached
   bool waitto_irrigate;
   unsigned long runoff_stoptime;
   
   unsigned long currtemp;
   
   unsigned long runoff_starttime;  // when it started
   boolean runoff_wait;   // is the system waiting to get ON
   boolean runoff_status; // on/off
   
   int sensor_pin;  // hardware pin numbers
   int valve_pin;  // hardware pin numbers
    
    // hese built-in pullup resistors are accessed by setting the pinMode() as INPUT_PULLUP. 
    //This effectively inverts the behavior of the INPUT mode, where HIGH means the sensor is off,
   // and LOW means the sensor is on.
   boolean valve_status;  // valve is off. initialize to high so that it remains off
    
   void  valve_setup(int pin, int valve)  // check the input_pullup in pinmode with the sensor we have.  
   {
     if(pin<20 && pin >0){
       sensor_pin = pin;
       pinMode(sensor_pin,INPUT);  // check for the controlled plot
     }
     valve_pin = valve;
     pinMode(valve_pin,OUTPUT);
     valve_status = LOW;
     
   }  
    //////////////////////////////////////////////////////////////////////////
    boolean	runoff_detected_already; // this to help take the interval of the runoff. If its true implies that the timer has already started. 
    boolean	runoff_stopped_already; // this flag helps to know if the runoff has already been stopped
    boolean sensor_check() // function to check the sensor 
    { 
      float total=0;
      int temp=0;
      
      for (int i=0;i<20;i++){
        temp = digitalRead(sensor_pin);
        delay(10);
        total = total +temp;
      }
      return runoff_status = ((total/20)>0.5); // ON when the average is greater than 0.5
    } 
   ////////////////////////////////////////////////////////////////////////////  
   
   void valve_on() // to on  the valve
   {
     digitalWrite(valve_pin,LOW); // for this valve we need to invert 
     valve_status = LOW;
   }
   
   void valve_off() // to off  the valve
   {
     digitalWrite(valve_pin,HIGH); // for this valve we need to invert 
     valve_status = HIGH;
   } 
   
   void valve_test() //this is to check if the valves are working
   {
     //unsigned long prevmill = 0;
     //unsigned long currmill = millis();
    //unsigned long runtime  =1000; //10 seconds
    
    if(valve_status == HIGH)
    {
       
        digitalWrite(valve_pin,LOW);
        valve_status = LOW;
        delay(5000);
    
    }
    if(valve_status == LOW){
        digitalWrite(valve_pin,HIGH);
        valve_status = HIGH;
        //prevmill = currmill;
        delay(5000);
    } 
   }
};
//////////////////////////////////////////////////////////////////////////

bool setuptime_last;
bool uploadfail;   //to check if there was an uplaod failure!! 
boolean firstdown_flag= false;  // this is a variable to check if the first time download was successuful. else it will keep downlaoding! 
boolean IR_timeup = false; // it is both the and opeator of irr_finish 
// for parsing
int count, countprev;
boolean newsettings = false;
zone mzone[2];
//zone mzone0(2,5);
  
//zone mzone1(3,6);   //define the pins
 
//////////////////////////////////////////////////////////////////////////
void setup() {
   Serial.begin(9600);
   Serial1.begin(9600); //Serial1
   mzone[1].valve_setup(3,5);
    mzone[1].valve_test();
   delay(5000);
   mzone[0].valve_setup(2,6);   // works with st1,pt1,
     mzone[0].valve_test();
   delay(5000);
   
   ///////////////////////////////////////////////////////////////////////
   //// For Initial Time setup////////////////////////////////////////////
   
   // Please Open "a_timefunctions" tab and set the current time. 
   // Uncomment the below line if you don't need to reset the current time.
    settime();
   
   
   
   ///////////////////////////////////////////////////////////////////////
   
   
   
   
   Serial.println(curtime());
   // Code to program datetime when needed. using server. 
   do{
     Serial.println("download looping in setup");
     settings_download();
     
   }while(!firstdown_flag); //check for the first download when rebooted. 
   Serial.println(curtime());
}

void loop() 
{
  //initialize
  //settings_download();
  for (int i=0;i<2;i++){
   mzone[i].irr_finished = false; // t/f to see if the irrigation is completed
   mzone[i].irr_start = false; // t/f to indicate if its time to start
   mzone[i].irrigating = false; // to see if its irrigating
   mzone[i].effective = 0; // current effective irrigatin time reached
   mzone[i].runoff_starttime=0;  // when it started
   mzone[i].runoff_wait=false; // is the system waiting to get ON
   mzone[i].runoff_status=false; // on/off
   mzone[i].runoff_detected_already =false; //first time should be false. 
   mzone[i].runoff_stopped_already = true;
   mzone[i].last_finishedtime =0;
   mzone[i].runoff_stoptime = 0;
   mzone[i].newsettings  = true;
   mzone[i].waitto_irrigate = false;
   mzone[i].zoneid = i;
   mzone[i].currtemp = 0;
   
  }
  unsigned long currentmills;  // used below for the millis function
  unsigned long tempmills =0;
   while(1)
   {
     waittoirrigate(); // checks when to start irrigation
     
     if(mzone[0].irr_start || mzone[1].irr_start)
     {
       
       while(!IR_timeup)
       {
         for(int i=0;i<2;i++){
           mzone[i].currtemp = curtime();
           if(mzone[i].irr_start)
            {
              mzone[i].valve_on(); //turnon the valve when its time
              mzone[i].irrigating = true; // initialize to true after started. 
              Serial.print("Zone has started irrigating");  /////
				Serial.println(mzone[i].zoneid);  /////
            }
            if(mzone[i].irrigating && (!mzone[i].runoff_wait))
            {
              mzone[i].valve_on(); //turnon the valve when its time
              //mzone[i].irrigating = true;
              mzone[i].irr_start = false;  // check to see if this works when set to false
              mzone[i].runoff_wait = false; 
			  Serial.print("zone is irrigating");  /////
				Serial.println(mzone[i].zoneid);  /////
            }
            if(mzone[i].sensor_check() && !mzone[i].valve_status) // when runoff detected
            {
              mzone[i].valve_off(); //turnon the valve when its time
              mzone[i].runoff_wait=true; // is the system waiting to get ON
				if(!mzone[i].runoff_detected_already)
				{
					mzone[i].runoff_starttime=curtime(); // check the function to get the time. Just to note down the time for the first time it detects
					mzone[i].runoff_detected_already = true; // this flag makes the time noting more accurate.
					mzone[i].runoff_stopped_already = false; 
                                      status_upload(mzone[i].runoff_status, mzone[i].valve_status, mzone[i].zoneid, mzone[i].irrigating , mzone[i].runoff_starttime); //uploading 1,0,Di,runoff_starttime
				}
              
			  
              
              //mzone[i].irrigating = true;
               
              //mzone[i].runoff_status=true;  // because the sensor_check() function does that already. 
            
              
		 Serial.print("Runoff has been detected for ");  /////
		Serial.println(mzone[i].zoneid);  /////
            }
		if((!mzone[i].sensor_check())&& (mzone[i].valve_status) && (!mzone[i].runoff_stopped_already) && (mzone[i].runoff_wait)) // this is when the runoff has stopped for the first time. 
			{
				mzone[i].runoff_stopped_already =true;
				mzone[i].runoff_stoptime = curtime(); // note down the end of runoff. 
				//mzone[i].runoff_detected_already = false; // set the runoff detected flag to false after the runoff stopped.
                                
				Serial.print("This is to find when the runoff has stopped for ");  /////
				Serial.println(mzone[i].zoneid);  /////
                            status_upload(mzone[i].runoff_status, mzone[i].valve_status, mzone[i].zoneid,mzone[i].irrigating , mzone[i].runoff_stoptime);
			}
            
            // This one is to check if the effective irrigation has been achieved and to decide if it has to turn on or not coming from a pause time. 
            if(mzone[i].runoff_wait && (mzone[i].effective > mzone[i].effective_time)) // why does it needs to be a runoff_Wait here??
            {
              mzone[i].valve_off();
              mzone[i].irrigating = false;
              mzone[i].runoff_wait=false;
              //mzone[i].runoff_status=false;  // check I think this is not needed 
              mzone[i].irr_finished = true;
             
              mzone[i].last_finishedtime = curtime(); // noting down the time of finish
              
			  Serial.print("Effective irrigation time achieved while paused for");  /////
				Serial.println(mzone[i].zoneid);  /////
              status_upload(mzone[i].runoff_status, mzone[i].valve_status, mzone[i].zoneid, mzone[i].irrigating , mzone[i].last_finishedtime);
            }
            
            if((mzone[i].effective > mzone[i].effective_time) && mzone[i].irrigating) // checking to see if the effective irrigation is achieved while irrigating
            {
              mzone[i].valve_off();
              mzone[i].irrigating = false;
              mzone[i].runoff_wait=false;
              //mzone[i].runoff_status=false;  // check I think this is not needed 
              mzone[i].irr_finished = true;
             
              mzone[i].last_finishedtime = curtime(); // noting down the time of finish
              
				Serial.print("Effective irrigation time achieved while irrigating for");  /////
				Serial.println(mzone[i].zoneid);  /////
                            Serial.println(mzone[i].effective);
                            Serial.println(mzone[i].last_finishedtime);
              status_upload(mzone[i].runoff_status, mzone[i].valve_status, mzone[i].zoneid, mzone[i].irrigating , mzone[i].last_finishedtime);
            }
            
            if((curtime()>=mzone[i].runoff_starttime + mzone[i].pause_time) && mzone[i].runoff_wait)
            {
              if(!mzone[i].sensor_check())
              {
                mzone[i].valve_on();
                mzone[i].runoff_wait = false;
				Serial.println("Pasue time finished");  /////
              }
              else
              {
                mzone[i].valve_off();
                mzone[i].runoff_starttime = curtime();
				Serial.println("still needed to be paused");  /////
              }
            }
            
            
          
            
         } // for (i)
         currentmills = millis();
         
         if((currentmills - tempmills >2000) || (currentmills < tempmills))
         {
           int increment =2;
           for(int i=0;i<2;i++)
           {
             //int increment = curtime()-mzone[i].currtemp; // in seconds
             
             if((mzone[i].irrigating) && (!mzone[i].valve_status))
             {
               mzone[i].effective = mzone[i].effective + increment;
	       Serial.print("Incrementing the effective irrigation ");  /////
              Serial.println(mzone[i].effective);
              Serial.print("Time from irrigation startted ");  /////
              Serial.println(curtime()-mzone[i].start_time);
	       mzone[i].irr_finished = ((curtime()-mzone[i].start_time)>= mzone[i].irrigation_time);
                
		if(mzone[i].irr_finished)
		{       mzone[i].valve_off();
			// noting down the time when it has completed the irrigation
			mzone[i].last_finishedtime = curtime();
			mzone[i].effective =0;
			mzone[i].irrigating  = false;
                                   
		        Serial.print(mzone[i].zoneid);  /////
			Serial.print(" Finished at ");  /////
                        Serial.println(mzone[i].last_finishedtime);  /////
                        status_upload(mzone[i].runoff_status, mzone[i].valve_status, mzone[i].zoneid, mzone[i].irrigating , mzone[i].last_finishedtime);
				}
			   
             }
             
           }
           tempmills = millis();
         
         } //if 2seconds
         //  note down the time    
                                                                                                                                                                                                                                                                                              
         IR_timeup = (mzone[0].irr_finished && mzone[1].irr_finished);
         // tthis is to check if the irrigation of one finishes and to set the start of the secoind plot high.
         if((mzone[0].irr_finished || mzone[1].irr_finished) && (!IR_timeup) && (!mzone[0].irrigating) &&  (!mzone[1].irrigating))
         {
           waittoirrigate();
		   Serial.println("going to waiting when on the valves finished irrigating. ");  /////
         
         }
         
       } // while (!IR_timeup)
		   Serial.println("IR_timeup is true");  /////
       for(int i=0;i<2;i++)
       {
         if(mzone[i].valve_status)
           mzone[i].valve_off();
	   Serial.println(" Turnig off valves if ON for the last time.");  /////
       }
       
     
     } //if(mzone[0].irr_start || mzone[1].irr_start)
	Serial.println("None is due to start any sooner");  /////
			 
   
   }// while(1) loop
}

   
   


void waittoirrigate(){
    unsigned long currentmills;  // used below for the millis function
    unsigned long tempmills;
    int min30_counter = 0;

    currentmills = millis();
    tempmills = currentmills;
    int currenttime;
    
    
    while(1)
    {
      currenttime = curtime();
      
      
      for(int i=0;i<2;i++)
      {
        if( currenttime -  mzone[i].last_finishedtime >= 86400 && mzone[i].newsettings) // to have a small check to see if the time between the euns is more than 24 hours
        {
          mzone[i].waitto_irrigate = true;  // set the wait to irrigate flag true. this is to ensure that it dowesnt irrigate twice a day. 
          // revert back the new settings to false; 
		  mzone[i].newsettings = false;
      Serial.println("Waiting to irrigate is true and one day has to be passed away");
        }
         
		 // below condition checks if the current time is more than the starttime and if the irr_start and irrigating flags are false and is it waiting to irrigate that day. 
        if(currenttime>=mzone[i].start_time && (!mzone[i].irr_start) && mzone[i].waitto_irrigate && (!mzone[i].irrigating)) 
        {
          // write the condition to set the start_time to true. when the condition satisfies.  
          mzone[i].irr_start = true;
		  IR_timeup = false;
		  mzone[i].irr_finished = false;
		  mzone[i].effective =0;
		  mzone[i].waitto_irrigate = false; // once its going to start irrigation you should stop waiting to irrigate. 
		  Serial.print("going to irrigate: ");
		  Serial.println(mzone[i].zoneid); ////////////
		  return; // this would exit the waittoirrigate functiion
        }
        Serial.println("waiting in the start loop");
      }
      currentmills = millis();

      // loop to downlaod settings if not irrigating for every half hour
      if((currentmills - tempmills > 2000) || (currentmills < tempmills)) // this is true for evrey two seconds. 
      {
		  Serial.println("inside waiting loop.");  /////
       min30_counter = min30_counter +2;
       if((min30_counter >= 1800) && (!mzone[0].irrigating) && (!mzone[1].irrigating))
        {
		  Serial.println("downloading settings.");  /////
          settings_download();
          min30_counter = 0;
		  
        }
        
       tempmills = millis(); 
        
      }
    
    }
}
