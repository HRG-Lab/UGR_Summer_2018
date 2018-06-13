
 
void jparse(char* foo)
{
  
 
    // Memory pool for JSON object tree.
  //
  // Inside the brackets, 200 is the size of the pool in bytes,
  // If the JSON object is more complex, you need to increase that value.
  //StaticJsonBuffer<200> jsonBuffer;
  // Where JSON_OBJECT_SIZE is the count of the number of primary keys or just keys
  // JSON_ARRAY_SIZE: gives the count value of number of arrays in a keyvalue pair. 
  //const int BUFFER_SIZE = JSON_OBJECT_SIZE(5)+JSON_ARRAY_SIZE(0); 
  //foo;
  //StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
  StaticJsonBuffer<250> jsonBuffer;
  
  JsonObject& root = jsonBuffer.parseObject(foo);
  // Test if parsing succeeds.
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }
  //if (pNum != progNum)
  //{
   
    
//    const char* deviceid = root["sensor"];
//    char starttime = root["Starttime"];
//    char  pausetime = root["Pausetime"];
//     char irrigationtime1 = root["Irrigationtime"];
//     char valvenumber = root["valvenumber"];
//     const char* deviceid = root["deviceid"];
//  long starttime = root["starttime"];
//  long pausetime = root["pausetime"];
//  long irrigationtime = root["irrigationtime"];
//  int valvenumber = root["valvenumber"];
//  int count = root["count"];
//  
  
    
     //for(i=0; i<(sizeof(munit)/sizeof(unitv)); ++i){
      // if(mzone[i].zoneid = root["devid"])  // plot id
// since we don't have many digital pins at hand we will manually allocate. 
// change:
// st - starttime
//pt - pausetime
// it - irrigation time
//cnt - count 
// et - effective time
   
     count = root["cnt"];
     mzone[0].start_time = root["st2"];
     mzone[0].pause_time = root["pt2"];
     mzone[0].irrigation_time = root["it2"];
     mzone[0].effective_time = root["et2"];
     
     mzone[1].start_time = root["st1"];
     mzone[1].pause_time = root["pt1"];
     mzone[1].irrigation_time = root["it1"];
     mzone[1].effective_time = root["et1"];
     firstdown_flag = true;
  
  
  if (countprev != count){
    Serial.println("new settings available");
    //if(deviceid == devid){
      //Serial.print("new settings updated to ");
      //Serial.println(deviceid);
      countprev=  count;
      Serial.println("download finished so deleted");
	  mzone[0].newsettings  = true;
	  mzone[1].newsettings  = true;
  Serial.println(mzone[0].start_time);
  Serial.println(mzone[1].start_time);
      //downloadstatus_upload();
  // update the settings 
    //}
  }
  else {
  Serial.println("old setings");
   mzone[0].newsettings  = false;
   mzone[1].newsettings  = false;
  }
}
 

//void downloadstatus_upload(){

//}

//void settings_download(){

//}
