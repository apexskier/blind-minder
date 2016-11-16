void wifi_setup() {
    int count = 0;

    // connect to WiFi
    Serial.print(F("Connecting to "));
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(200);
        Serial.print(F("\r  "));
        Serial.print(count++);
    }
    Serial.println();
    Serial.println(F("WiFi connected\nIP address: "));
    Serial.println(WiFi.localIP());
}
