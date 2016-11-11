void routes_setup() {
    server.on("/", routes_root);
    server.onNotFound(routes_404);
    server.begin();
    Serial.println("HTTP server started");
}

void routes_loop() {
    server.handleClient();
}

void routes_root() {
    String message;
    uint8_t num_args;
    switch (server.method()) {
    case HTTP_GET:
        message += "{\"blinds\":[";
        for (int i = 0; i < NUM_BLINDS; i++) {
            message += "{\"target\":";
            message += blinds[i].target_angle;
            message += ",\"current\":";
            message += blinds[i].read();
            message += ",\"obstructed\":";
            message += blinds[i].obstruction_detected ? "true" : "false";
            message += ",\"moving\":";
            message += blinds[i].moving_to_target ? "true" : "false";
            message += "}";
            if (i < NUM_BLINDS - 1) {
                message += ",";
            }
        }
        message += "],";
        message += ",\"luminance\":";
        message += luminance;
        message += "}";
        server.send(200, "application/json", message);
        break;
    case HTTP_POST:
        num_args = server.args();
        if (num_args > 0) {
            int val = 10000; // larger than valid
            for (uint8_t arg_i = 0; arg_i < num_args; arg_i++) {
                String arg_name = server.argName(arg_i);
                if (arg_name == "v") {
                    val = server.arg(arg_i).toInt();
                } else if (arg_name == "plain") {
                    // ignore TODO: find out why this exists
                } else {
                    server.send(400);
                    return;
                }
            }
            // reject values outside of servo's range
            // also will reject if val wasn't provided in call
            if (val <= MAX_DEGREES && val >= 0) {
                Serial.print("Received request to set blinds to ");
                Serial.println(val);
                for (int i = 0; i < NUM_BLINDS; i++) {
                    blinds[i].seek(val);
                }
                server.send(202);
                break;
            }
        }
        server.send(400);
        return;
    default:
        server.send(405);
        break;
    }
}

void routes_404() {
    server.send(404);
}
