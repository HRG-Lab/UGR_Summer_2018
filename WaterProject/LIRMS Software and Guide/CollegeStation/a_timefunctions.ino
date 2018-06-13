char timeBuf[11];


void settime(){
//Display the date time before setting
//  system("date > /dev/ttyGS0");
//  
//  //Linux command for setting system date time
//  String dateCmd="date +'%Y%m%d %H:%M:%S' -s ";
//  
//  
//  ///////////////////////////////////////////////////////////////////////
//  //////////////////// Function to update the current time///////////////
//  //Change time in the next line to configure time 
//  //in your Galileo
//  //    YYYYMMDD HH:MM:SS
//  dateCmd+="'20171010 11:53:10'";
//    
//  ///////////////////////////////////////////////////////////////////////
//  
//  
//  //Display the response from setting command. 
//  //So we know if anything went wrong
//  dateCmd+=" > /dev/ttyGS0";
//  
//  //Excute the setting command
//  system(dateCmd.buffer);
//  
//  //Sync system time with RTC clock. It's necessary
//  //For getting the RTC clock to work.
//  system("hwclock -w");
//  
//  //Display the date time after setting.
//  system("date > /dev/ttyGS0");
//  
  // date montt hour minute year
  //mmddhhmmyyyy
  system("date 111313332017");
 
}


//get current time
int curtime(){
  // We get the GMT time
  //get current time in the format- hours:minutes:secs 
  //and save in text file time.txt located in /tmp
  //tmp is temporaray folder located in RAM, so it's 
  //not wearing out the sd card.
  system("date '+%s' > /tmp/time.txt");  
  FILE *fp;
  fp = fopen("/tmp/time.txt", "r");
  fgets(timeBuf, 11, fp);
  //Serial.println(timeBuf);
  fclose(fp);
  
  String t = (String)timeBuf;
  int times = t.toInt();
//  Serial.print("The time in Epoch at GMT but local ");
//  Serial.println(times);
  return times; 
}

boolean compareTime(int starttime){
  //Compare if the hour and minutes are the same
   int cutime = curtime();
   
  return (cutime >= starttime);
}
