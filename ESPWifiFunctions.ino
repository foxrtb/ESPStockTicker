void connectWifi()
{
  Serial.println(F("connectWifi()..."));
  
  //[ssid][password]
  // 5 rows with 2 columns (64+32) wide
  char wifis[MAX_WIFI_NETWORKS][2][96];

  int networks = readWifiInfo(wifis);
  if(networks)
  {
    
    //no point in trying to connect if empty
    char msg [50];
    
    for(int i = 0; i < MAX_WIFI_NETWORKS; i++)
    {
      if(strlen(wifis[i][0]))
      {
        Serial.printf_P(PSTR("\tAdding wifi no: %d\n"), i);
        wifiMulti.addAP(wifis[i][0], wifis[i][1]);
      }
    }
    wifiMulti.run();
    delay(1000);
  }
  yield();
  Serial.println(F("connectWifi()...done"));
}

void startSoftAP()
{
  if(wifiMulti.run() == WL_CONNECTED && soft_AP_started)
  {
    Serial.println(F("\tstartSoftAP(): Disabling softAP..."));
    WiFi.softAPdisconnect(true);
    soft_AP_started = false;

  }
  else if(wifiMulti.run() != WL_CONNECTED && !soft_AP_started)
  {
    soft_AP_started = true;
    Serial.printf_P(PSTR("\tstartSoftAP(): Starting softAP: %s\n"), SOFT_AP_NAME);
    
    WiFi.softAP(SOFT_AP_NAME);
    String softIP = WiFi.softAPIP().toString();
    Serial.printf_P(PSTR("\tstartSoftAP(): Soft-AP IPAddress: %s\n"), softIP.c_str());
  
    char msg [170];
    printMsg(F("Wifi not connected yet."), true);
    printMsg(F("Give it a minute, then try"));
    sprintf(msg, "to connect to '%s'", SOFT_AP_NAME);
    printMsg(msg);
    printMsg(F("WIFI network. Then open"));
    sprintf(msg, "http://%s/wifi", softIP.c_str());
    printMsg(msg);
    printMsg(F("to configure settings."));
  }
}
