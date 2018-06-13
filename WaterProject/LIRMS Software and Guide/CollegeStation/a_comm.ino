//Communication modules


char data[512];

    char tempdata[512];
    int counter1;
    char s;
    int x;
    char aux_str[100];
    char aux;

    int data_size;
    
      int8_t answer;

/// This function is used to upload the runoff status when called. You need an char input stating whether it is a ON or OFf based on the char 
void status_upload(boolean rstat, boolean vstat, int zoneid, boolean irrstat,  unsigned long timestat){

  //detachInterrupt(SENSOR);
  char runoffstat, valvestat;
  unsigned long time_event = timestat;
  String timetemp = "&t=";
  String timeval = timetemp + time_event;
  
  char irrigationstatus;
  
  
  
  char zonetemp;
  if (zoneid==1)
  {
    zonetemp = '1';
    
  }
  else{
    zonetemp ='0';
  }
  String Zone_id = "40"+(String)zonetemp; // dallas starts with "10"
  
  
  //runoff status
  if(rstat)
  {
    runoffstat = '1'; 
    //valvestat = '0';
    
  }
  else 
  { 
    runoffstat = '0';
   // valvestat = '0';
  }
  
  //valve status
   if(vstat)
  {
    valvestat = '1'; 
    //valvestat = '0';
    
  }
  else 
  { 
    valvestat = '0'; 
   // valvestat = '0';
  }
  
  //Irrigation status 
   if(irrstat)
  {
    irrigationstatus = '1'; 
    //valvestat = '0';
    
  }
  else 
  { 
    irrigationstatus = '0'; 
   // valvestat = '0';
  }
  
  
     
    // string that will be uploaded to the server. 
   String runstatus = "zid=" + Zone_id + "&rst="+ (String)runoffstat +"&vst="+(String)valvestat +"&ist="+(String)irrigationstatus+ timeval;
   Serial.println(runstatus);
  
  sendATcommand("AT+CPIN?", "OK", 2000);
   Serial.println("debug1");
    delay(3000);

    while (sendATcommand2("AT+CREG?", "+CREG: 0,1", "+CREG: 0,5", 2000) == 0);
  Serial.println("debug2");

     sendATcommand("AT+SAPBR=3,1,\"Contype\",\"GPRS\"", "OK", 2000);
    Serial.println("debug31");
    sendATcommand("AT+SAPBR=3,1,\"APN\",\"fast.t-mobile.com\"", "OK", 2000);
    Serial.println("debug41");
   
    int badcount = 0;
    while (sendATcommand("AT+SAPBR=1,1", "OK", 20000) == 0)
    {  
        badcount++;
        if(badcount>4){
          Serial.println("debug51-its bad!");
          uploadfail = true;
          break;
        
        }
        else
        {
          uploadfail = false;
        }//delay(5000);
        
    }
    Serial.println("debug6");


  // Initializes HTTP service
    answer = sendATcommand("AT+HTTPINIT", "OK", 10000);
    Serial.println("debug7");
    if (answer == 1)
    {
        // Sets CID parameter
        answer = sendATcommand("AT+HTTPPARA=\"CID\",1", "OK", 5000);
        Serial.println("debug8");
        if (answer == 1)
        {
            // Sets url 
            //answer = sendATcommand("AT+HTTPPARA=\"URL\",\"http://lirms.tamu.edu/dlogger.php?log=trislfsd\"", "OK", 5000);
            //String jsod = "\{\"deviceid\":\"lirms1\",\"runoffstat\":\"1\",\}";
            
           
          Serial1.println("AT+HTTPPARA=\"URL\",\"http://lirms.tamu.edu/cs/dbloggercs.php?" + runstatus+"\"");
            //Serial1.println("AT+HTTPPARA=\"URL\",\"http://lirms.tamu.edu/dlogger.php?log=" + runstatus+"\"");
             delay(10000);
                 ShowSerialData();
            Serial.println("debug9");
            //if (answer == 1)
            //{
               
                 //Serial.println("debug1011");
                 //if (answer==1){
                //answer = sendATcommand("AT+HTTPACTION=1", "+HTTPACTION:0,200", 10000);
                 Serial1.println("AT+HTTPACTION=0");
                 delay(10000);
                 ShowSerialData();
                
                   
  }

        }
    

    sendATcommand("AT+HTTPTERM", "OK", 5000);
    

    delay(1000);

    sendATcommand("AT+SAPBR=0,1", "OK", 2000);
    
 // attachInterrupt ((SENSOR), sensed, RISING);  // attach interrupt handler
    
}



/// This function is used to upload the runoff status when called. You need an char input stating whether it is a ON or OFf based on the char 
/*void runoffstatus_upload(boolean rstat, boolean istat, int devid){

  //detachInterrupt(SENSOR);
  char runoffstat, valvestat;
  
  //runoff status
  if(rstat)
  {
    runoffstat = '1'; 
    //valvestat = '0';
    
  }
  else 
  { 
    runoffstat = '0';
   // valvestat = '0';
  }
  
  //irrigation status
   if(istat)
  {
    valvestat = '1'; 
    //valvestat = '0';
    
  }
  else 
  { 
    valvestat = '0'; 
   // valvestat = '0';
  }
  
  
     
    // string that will be uploaded to the server. 
   String runstatus = "devid=" + (String)devid + "&rst="+ (String)runoffstat +"&ist="+(String)valvestat;
   Serial.println(runstatus);
  
  sendATcommand("AT+CPIN?", "OK", 2000);
   Serial.println("debug1");
    delay(3000);

    while (sendATcommand2("AT+CREG?", "+CREG: 0,1", "+CREG: 0,5", 2000) == 0);
  Serial.println("debug2");

     sendATcommand("AT+SAPBR=3,1,\"Contype\",\"GPRS\"", "OK", 2000);
    Serial.println("debug31");
    sendATcommand("AT+SAPBR=3,1,\"APN\",\"fast.t-mobile.com\"", "OK", 2000);
    Serial.println("debug41");
   
    int badcount = 0;
    while (sendATcommand("AT+SAPBR=1,1", "OK", 20000) == 0)
    {  
        badcount++;
        if(badcount>4){
          Serial.println("debug51-its bad!");
          uploadfail = true;
          break;
        
        }
        else
        {
          uploadfail = false;
        }//delay(5000);
        
    }
    Serial.println("debug6");


  // Initializes HTTP service
    answer = sendATcommand("AT+HTTPINIT", "OK", 10000);
    Serial.println("debug7");
    if (answer == 1)
    {
        // Sets CID parameter
        answer = sendATcommand("AT+HTTPPARA=\"CID\",1", "OK", 5000);
        Serial.println("debug8");
        if (answer == 1)
        {
            // Sets url 
            //answer = sendATcommand("AT+HTTPPARA=\"URL\",\"http://lirms.tamu.edu/dlogger.php?log=trislfsd\"", "OK", 5000);
            //String jsod = "\{\"deviceid\":\"lirms1\",\"runoffstat\":\"1\",\}";
            
           
          Serial1.println("AT+HTTPPARA=\"URL\",\"http://lirms.tamu.edu/data/dblogger.php?" + runstatus+"\"");
            //Serial1.println("AT+HTTPPARA=\"URL\",\"http://lirms.tamu.edu/dlogger.php?log=" + runstatus+"\"");
             delay(10000);
                 ShowSerialData();
            Serial.println("debug9");
            //if (answer == 1)
            //{
               
                 //Serial.println("debug1011");
                 //if (answer==1){
                //answer = sendATcommand("AT+HTTPACTION=1", "+HTTPACTION:0,200", 10000);
                 Serial1.println("AT+HTTPACTION=0");
                 delay(10000);
                 ShowSerialData();
                
                   
  }

        }
    

    sendATcommand("AT+HTTPTERM", "OK", 5000);
    

    delay(1000);

    sendATcommand("AT+SAPBR=0,1", "OK", 2000);
    
 // attachInterrupt ((SENSOR), sensed, RISING);  // attach interrupt handler
    
}
    

*/


///////////////////////////GPRS feedback////////////////////////////////////////


void downloadstatus_upload(){

 // detachInterrupt(SENSOR);
  String downloadstatus = "1";
     
    // string that will be uploaded to the server. 
  // String runstatus = "devid=" + (String)devid + "&rst="+ (String)runoffstat +"&ist="+(String)valvestat;
 //  Serial.println(runstatus);
  
  sendATcommand("AT+CPIN?", "OK", 2000);
   Serial.println("debug71");
    delay(3000);

    while (sendATcommand2("AT+CREG?", "+CREG: 0,1", "+CREG: 0,5", 2000) == 0);
  Serial.println("debug72");

     sendATcommand("AT+SAPBR=3,1,\"Contype\",\"GPRS\"", "OK", 2000);
    Serial.println("debug731");
    sendATcommand("AT+SAPBR=3,1,\"APN\",\"fast.t-mobile.com\"", "OK", 2000);
    Serial.println("debug741");
   
    int badcount = 0;
    while (sendATcommand("AT+SAPBR=1,1", "OK", 20000) == 0)
    {  
        badcount++;
        if(badcount>4){
          Serial.println("debug51-its bad!");
          
          break;
        
        }
         
        
    }
    Serial.println("debug76");


  // Initializes HTTP service
    answer = sendATcommand("AT+HTTPINIT", "OK", 10000);
    Serial.println("debug77");
    if (answer == 1)
    {
        // Sets CID parameter
        answer = sendATcommand("AT+HTTPPARA=\"CID\",1", "OK", 5000);
        Serial.println("debug87");
        if (answer == 1)
        {
            // Sets url 
            //answer = sendATcommand("AT+HTTPPARA=\"URL\",\"http://lirms.tamu.edu/dlogger.php?log=trislfsd\"", "OK", 5000);
            //String jsod = "\{\"deviceid\":\"lirms1\",\"runoffstat\":\"1\",\}";
            
           
          Serial1.println("AT+HTTPPARA=\"URL\",\"http://lirms.tamu.edu/cs/downlogcs.php?downs=" + downloadstatus+"\"");
            //Serial1.println("AT+HTTPPARA=\"URL\",\"http://lirms.tamu.edu/dlogger.php?log=" + runstatus+"\"");
             delay(10000);
                 ShowSerialData();
            Serial.println("debug79");
            //if (answer == 1)
            //{
               
                 //Serial.println("debug1011");
                 //if (answer==1){
                //answer = sendATcommand("AT+HTTPACTION=1", "+HTTPACTION:0,200", 10000);
                 Serial1.println("AT+HTTPACTION=0");
                 delay(10000);
                 ShowSerialData();
                
                   
  }

        }
    

    sendATcommand("AT+HTTPTERM", "OK", 5000);
    

    delay(1000);

    sendATcommand("AT+SAPBR=0,1", "OK", 2000);
 //    attachInterrupt ((SENSOR), sensed, RISING);  // attach interrupt handler
}
 
 

////////////////////////GPRS feedback ///////////////////////////////////////////




///////////////////// GPRS Download ////////////////////////////////

void settings_download()
{
 //  detachInterrupt(SENSOR);
  int downloadsize;
  //data = '';
 sendATcommand("AT+CPIN?", "OK", 2000);
   Serial.println("debug1");
    //delay(3000);

    while (sendATcommand2("AT+CREG?", "+CREG: 0,1", "+CREG: 0,5", 2000) == 0);
  Serial.println("debug2");

    sendATcommand("AT+SAPBR=3,1,\"Contype\",\"GPRS\"", "OK", 2000);
    Serial.println("debug3");
    sendATcommand("AT+SAPBR=3,1,\"APN\",\"fast.t-mobile.com\"", "OK", 2000);
    Serial.println("debug4");
   // sendATcommand("AT+SAPBR=3,1,\"USER\",\"user_name\"", "OK", 2000);
    //sendATcommand("AT+SAPBR=3,1,\"PWD\",\"password\"", "OK", 2000);


  int c = 0;
    while (sendATcommand("AT+SAPBR=1,1", "OK", 20000) == 0)
    {
        //delay(5000);
        // Serial.println("debug5-its bad!");
        c++;
        if (c>=4)
          break;
    }
    Serial.println("debug6");

  
    // Initializes HTTP service
    answer = sendATcommand("AT+HTTPINIT", "OK", 10000);
    Serial.println("debug7");
    if (answer == 1)
    {
        // Sets CID parameter
        answer = sendATcommand("AT+HTTPPARA=\"CID\",1", "OK", 5000);
        Serial.println("debug8");
        if (answer == 1)
        {
            // Sets url 
            answer = sendATcommand("AT+HTTPPARA=\"URL\",\"http://lirms.tamu.edu/cs/settingcs\"", "OK", 5000); //check and change to settingdown
            Serial.println("debug9");
            if (answer == 1)
            {
                // Starts GET action
                answer = sendATcommand("AT+HTTPACTION=0", "+HTTPACTION:0,200", 5000);
                Serial.println("debug10");
                if (answer == 1)
                {
                    x=0;
                    do{
                        sprintf(aux_str, "AT+HTTPREAD=%d,200", x);
                        if (sendATcommand2(aux_str, "+HTTPREAD:", "ERROR", 30000) == 1)
                        {
                            data_size = 0;
                            while(Serial1.available()==0);
                            aux = Serial1.read();
                            do{
                                data_size *= 10;
                                data_size += (aux-0x30);
                                while(Serial1.available()==0);
                                aux = Serial1.read();        
                            }while(aux != 0x0D);

                            Serial1.print("Data received: ");
                            Serial.println(data_size);

                            if (data_size > 0)
                            {
                                while(Serial1.available() < data_size);
                                Serial1.read();

                                for (int y = 0; y < data_size; y++)
                                {
                                    data[x] = Serial1.read();
                                    x++;
                                }
                                data[x] = '\0';
                            }
                            else
                            {
                                Serial.println("Download finished");  
                               
                                Serial.println(data);  
                               
                                //GPRS_up('1'); //calling the data uploads
                                 
                            }
                        }
                        else if (answer == 2)
                        {
                            Serial.println("Error from HTTP");
                        }
                        else
                        {
                            Serial.println("Error getting the url");
                            data_size = 0;
                        }
                        
                        sendATcommand("", "+HTTPACTION:0,200", 2000);
                    }while (data_size > 0);
                    Serial.println("debug - 2");
                    Serial.println("Data received: ");
                    Serial.println(data);
                   downloadsize = x; // this is what is printed as the download size. 
                     
                }
                else
                {
                    Serial.println("Error getting the url");
                }
            }
            else
            {
                Serial.println("Error setting the url");
            }
        }
        else
        {
            Serial.println("Error setting the CID");
        }    
    }
    else
    {
        Serial.println("Error initializating");
    }
    
    sendATcommand("AT+HTTPTERM", "OK", 2000);
    

    //delay(1000);

    sendATcommand("AT+SAPBR=0,1", "OK", 2000);

//Serial.println("download complete last");
//Serial.println(data); 

//delay(1000);
//char temp[10];

//for (int i=;i<=sizeof(data);i++){
  //tempdata[i] = data[i];
  
//}
//Serial.println("download tempdata");
//Serial.println(tempdata);
//Serial.println(temp);

//if (data[6]=='"'){

  //currenttime = 100;
  //Serial.println(currenttime);

//}
//Serial.println(data);
//Serial.println(data_size);
//just to confirm if we downloaded any data or not!! check for the perfectly programmed one!! 
if(downloadsize>=70){
  Serial.println("now parsing");
    jparse(data);
   //firstdown_flag = true;  // this is for checking the status of the download in the setup(); 
}
 
//attachInterrupt ((SENSOR), sensed, RISING);  // attach interrupt handler
}
///////////////////////////////////////////////////////////////////////////////////////////////////

//////////// PSerial Printing//////////
void ShowSerialData()
{
 while(Serial1.available()!=0)
 Serial.write(Serial1.read());
}



int8_t sendATcommand(char* ATcommand, char* expected_answer1, unsigned int timeout){

    uint8_t x=0,  answer=0;
    char response[100];
    unsigned long previous;

    memset(response, '\0', 100);    // Initialize the string

    delay(100);

    while( Serial1.available() > 0) Serial1.read();    // Clean the input buffer

    Serial1.println(ATcommand);    // Send the AT command 


        x = 0;
    previous = millis();

    // this loop waits for the answer
    do{
        if(Serial1.available() != 0){    
            response[x] = Serial1.read();
            x++;
            // check if the desired answer is in the response of the module
            if (strstr(response, expected_answer1) != NULL)    
            {
                answer = 1;
            }
        }
        // Waits for the asnwer with time out
    }
    while((answer == 0) && ((millis() - previous) < timeout));    

    return answer;
}



int8_t sendATcommand2(char* ATcommand, char* expected_answer1, char* expected_answer2, unsigned int timeout)
{

    uint8_t x=0,  answer=0;
    char response[100];
    unsigned long previous;

    memset(response, '\0', 100);    // Initialize the string

    delay(100);

    while( Serial1.available() > 0) Serial1.read();    // Clean the input buffer

    Serial1.println(ATcommand);    // Send the AT command 


        x = 0;
    previous = millis();

    // this loop waits for the answer
    do{        
        if(Serial1.available() != 0){    
            response[x] = Serial1.read();
            x++;
            // check if the desired answer 1 is in the response of the module
            if (strstr(response, expected_answer1) != NULL)    
            {
                answer = 1;
            }
            // check if the desired answer 2 is in the response of the module
            if (strstr(response, expected_answer2) != NULL)    
            {
                answer = 2;
            }
        }
        // Waits for the asnwer with time out
    }while((answer == 0) && ((millis() - previous) < timeout));    

    return answer;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
