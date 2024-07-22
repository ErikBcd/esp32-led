#include "../website_handler.h"

bool WebsiteHandler::params_exist(AsyncWebServerRequest *request, String *params, size_t length) {
        for (size_t i = 0; i < length; i++) {
                if (!request->hasParam(params[i])) {
                        return false;
                }
        }
        return true;
}

void WebsiteHandler::off() {
        Serial.println("Shutting down devices.");
        led_command c = {};
        uint8_t data[TURN_OFF_SIZE];
        led_message::create_message_turn_off(data);
        c.broadcast = true;
        c.length = TURN_OFF_SIZE;
        c.msg = data;
        manager->add_command(c);
}

void WebsiteHandler::color(uint8_t r, uint8_t g, uint8_t b) {
        CRGB colors[3] = {CRGB(r,g,b), CRGB::Black, CRGB::Black};
        led_command c = {};
        uint8_t data[CHANGE_COLORS_SIZE];
        led_message::create_message_change_colors(colors, data);
        c.broadcast = true;
        c.length = CHANGE_COLORS_SIZE;
        c.msg = data;
        manager->add_command(c);
}

void WebsiteHandler::color(CRGB colors[3]) {
        led_command c = {};
        uint8_t data[CHANGE_COLORS_SIZE];
        led_message::create_message_change_colors(colors, data);
        c.broadcast = true;
        c.length = CHANGE_COLORS_SIZE;
        c.msg = data;
        manager->add_command(c);
}

void WebsiteHandler::mode_change(uint8_t mode) {
        if (FIRST_MODE <= mode && mode <= LAST_MODE) {
                //Serial.println("Changing mode only");
                led_command c = {};
                uint8_t data[CHANGE_ONLY_MODE_SIZE];
                //Serial.println("data created");
                led_message::create_message_change_mode((MODE) mode, data);
                c.broadcast = true;
                c.length = CHANGE_ONLY_MODE_SIZE;
                c.msg = data;
                //Serial.println("command created");
                manager->add_command(c);
        }
}

void WebsiteHandler::mode_change(uint8_t mode, uint8_t r, uint8_t g, uint8_t b) {
        CRGB colors[3] = {CRGB(r,g,b), CRGB::Black, CRGB::Black};
        mode_change(mode, colors);
}

void WebsiteHandler::mode_change(uint8_t mode, CRGB colors[3]) {
        if (FIRST_MODE <= mode && mode <= LAST_MODE) {
                led_command c = {};
                uint8_t data[CHANGE_MODE_SIZE];
                led_message::create_message_change_mode(mode, colors, data);
                c.broadcast = true;
                c.length = CHANGE_MODE_SIZE;
                c.msg = data;
                manager->add_command(c);
        }
}

/*
String sliderValue1 = "128";
String sliderValue2 = "128";
String sliderValue3 = "128";
const char* PARAM_INPUT_1 = "value1";
const char* PARAM_INPUT_2 = "value2";
const char* PARAM_INPUT_3 = "value3";
*/

/* <!DOCTYPE HTML><html><head>
  <title>ESP Input Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <form action="/get">
    input1: <input type="text" name="input1">
    <input type="submit" value="Submit">
  </form><br>
  <form action="/get">
    input2: <input type="text" name="input2">
    <input type="submit" value="Submit">
  </form><br>
  <form action="/get">
    input3: <input type="text" name="input3">
    <input type="submit" value="Submit">
  </form>
</body></html> */

// HTML web page to handle 3 input fields (input1, input2, input3)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP Web Server</title>
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 2.3rem;}
    p {font-size: 1.9rem;}
    body {max-width: 400px; margin:0px auto; padding-bottom: 25px;}
    .slider { -webkit-appearance: none; margin: 14px; width: 360px; height: 25px; background: #FFD65C;
      outline: none; -webkit-transition: .2s; transition: opacity .2s;}
    .slider::-webkit-slider-thumb {-webkit-appearance: none; appearance: none; width: 35px; height: 35px; background: #003249; cursor: pointer;}
    .slider::-moz-range-thumb { width: 35px; height: 35px; background: #003249; cursor: pointer; } 
  </style>
</head>
<body>
  <h2>ESP Web Server</h2>
  <div class="dropdown">
    <select id="menu" onChange="changeMenu()">
      <option value="1"> Off LED</option>
      <option value="2">Static</option>
      <option value="3">Rainbow</option>
      <option value="4">Flicker</option>
      <option value="5">Blend</option>
    </select>
  </div>
  <div id="slider1" style="display:none">
    <p><span id="textSliderValue1">r1</span></p>
    <p><input type="range" onchange="updateSliderPWM(this)" id="pwmSlider1" min="0" max="255" value1="r1" step="1" class="slider"></p>
    <p><span id="textSliderValue2">g1</span></p>
    <p><input type="range" onchange="updateSliderPWM(this)" id="pwmSlider2" min="0" max="255" value2="g1" step="1" class="slider"></p>
    <p><span id="textSliderValue3">b1</span></p>
    <p><input type="range" onchange="updateSliderPWM(this)" id="pwmSlider3" min="0" max="255" value3="b1" step="1" class="slider"></p>
  </div>
  <div id="slider2" style="display:none">
        <p><span id="textSliderValue4">r2</span></p>
        <p><input type="range" onchange="updateSliderPWM(this)" id="pwmSlider4" min="0" max="255" value1="r2" step="1" class="slider"></p>
        <p><span id="textSliderValue5">g2</span></p>
        <p><input type="range" onchange="updateSliderPWM(this)" id="pwmSlider5" min="0" max="255" value2="g2" step="1" class="slider"></p>
        <p><span id="textSliderValue6">b2</span></p>
        <p><input type="range" onchange="updateSliderPWM(this)" id="pwmSlider6" min="0" max="255" value3="b2" step="1" class="slider"></p>
  </div>
  <div id="slider3" style="display:none">
        <p><span id="textSliderValue7">r3</span></p>
        <p><input type="range" onchange="updateSliderPWM(this)" id="pwmSlider7" min="0" max="255" value1="r3" step="1" class="slider"></p>
        <p><span id="textSliderValue8">g3</span></p>
        <p><input type="range" onchange="updateSliderPWM(this)" id="pwmSlider8" min="0" max="255" value2="g3" step="1" class="slider"></p>
        <p><span id="textSliderValue9">b3</span></p>
        <p><input type="range" onchange="updateSliderPWM(this)" id="pwmSlider9" min="0" max="255" value3="b3" step="1" class="slider"></p>
  </div>
<script>

function changeMenu(){

  var menu = document.getElementById("menu");

  var slider1 = document.getElementById("slider1");
  var slider2 = document.getElementById("slider2");
  var slider3 = document.getElementById("slider3");
  
  var selected = menu.addEventListener("change", changing);

  var xhr = new XMLHttpRequest();
  
  function changing(event){
    if (menu.value == '1') {
       slider1.style.display = "none";
       slider2.style.display = "none";
       slider3.style.display = "none";
       xhr.open("GET", "/off", true);
       xhr.send();
    }
    else if (menu.value == '2') {
       slider1.style.display = "inline";
       slider2.style.display = "none";
       slider3.style.display = "none";
       xhr.open("GET", "/mode?mode=B", true);
       xhr.send();
    }
    else if (menu.value == '3') {
       slider1.style.display = "none";
       slider2.style.display = "none";
       slider3.style.display = "none";
       xhr.open("GET", "/mode?mode=D", true);
       xhr.send();
    }
    else if (menu.value == '4') {
       slider1.style.display = "none";
       slider2.style.display = "none";
       slider3.style.display = "none";
       xhr.open("GET", "/mode?mode=E", true);
       xhr.send();
    }
    else if (menu.value == '5') {
       slider1.style.display = "inline";
       slider2.style.display = "inline";
       slider3.style.display = "inline";
       xhr.open("GET", "/mode?mode=F", true);
       xhr.send();
    }
  };

}

function updateSliderPWM(element) {
  var sliderValue1 = document.getElementById("pwmSlider1").value;
  var sliderValue2 = document.getElementById("pwmSlider2").value;
  var sliderValue3 = document.getElementById("pwmSlider3").value;
  var sliderValue4 = document.getElementById("pwmSlider4").value;
  var sliderValue5 = document.getElementById("pwmSlider5").value;
  var sliderValue6 = document.getElementById("pwmSlider6").value;
  var sliderValue7 = document.getElementById("pwmSlider7").value;
  var sliderValue8 = document.getElementById("pwmSlider8").value;
  var sliderValue9 = document.getElementById("pwmSlider9").value;
  var xhr = new XMLHttpRequest();
  if (menu.value == '2') {
        xhr.open("GET", "/mode?mode=B&r1="+sliderValue1+"&g1="+sliderValue2+"&b1="+sliderValue3, true);
        xhr.send();
  } else if (menu.value == '5') {
        xhr.open("GET", "/mode?mode=F&r1="+sliderValue1+"&g1="+sliderValue2+"&b1="+sliderValue3+"&r2="+sliderValue4+"&g2="+sliderValue5+"&b2="+sliderValue6+"&r3="+sliderValue7+"&g3="+sliderValue8+"&b3="+sliderValue9, true);
        xhr.send();
  }
}
</script>
</body>
</html>
)rawliteral";

WebsiteHandler::WebsiteHandler(AsyncWebServer *server, LED_Manager *mngr)
{
        // note: If we want to filter requests by AP/STA, use ON_STA_FILTER(request)/ON_AP_FILTER(request)
        webserver = server;
        manager = mngr;
        // Root website
        server->on("/test", HTTP_GET, [](AsyncWebServerRequest *request) {
                request->send(200, "text/html", 
                        "<!DOCTYPE html><html>\n"
                        "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
                        "<link rel=\"icon\" href=\"data:,\">\n"
                        "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n"
                        "</style></head>\n"
                        "<body>\n"
                        "<h1> This is a great test!</h1>\n"
                        "<p> <a href=\"/off\">Turn LEDs off</a> </p>\n"
                        "<p> <a href=\"/mode?mode=B&r1=255&g1=128&b1=255\">Static mode</a> </p>\n"
                        "<p> <a href=\"/mode?mode=D&r1=255&g1=128&b1=255\">Rainbow mode</a> </p>"
                        "<p> <a href=\"/mode?mode=E&r1=255&g1=128&b1=255\">Flickering</a> </p>"
                        "<p> <a href=\"/mode?mode=F&r1=113&g1=28&b1=145&r2=234&g2=0&b2=217&r3=10&g3=189&b3=198\">Blend</a> </p>\n"
                        "<p> <a href=\"/color?r1=0&g1=0&b1=255\">Change color to blue</a> </p>\n"
                        "<p> <a href=\"/color?r1=255&g1=0&b1=255\">Change color to purple</a> </p>\n"
                        "</body></html>");
        });

        server->on("/", HTTP_GET, [](AsyncWebServerRequest *request)
        {
             request->send_P(200, "text/html", index_html);
             request->send(200, "text/plain", "OK");   
        });

        server->on("/hello", HTTP_GET, [](AsyncWebServerRequest *request)
        {
                if (ON_STA_FILTER(request)) {
                        if (request->params() >= 1) 
                                Serial.printf("Got a request! %s = %s\n", request->getParam(0),request->getParam(0)->value());
                        request->send(200, "text/plain", "Hello from STA");
                        return;
                } else if (ON_AP_FILTER(request)) {
                        request->send(200, "text/plain", "Hello from AP");
                        return;
                }
                request->send(200, "text/plain", "Hello from undefined"); 
        });

        server->on("/mode", HTTP_GET, [this](AsyncWebServerRequest *request) {
                Serial.printf("Got a mode change request! Params: %d\n", request->params());
                if (request->params() == 1) {
                        String params[1] = {"mode"};
                        if (!params_exist(request, params, 1)) {
                                request->send(404, "text/plain", "One or more parameters didn't exist!");
                                return;
                        }
                        uint8_t mode = request->getParam("mode")->value()[0];
                        Serial.printf("Mode switch: %c\n", mode);
                        mode_change(mode);
                        request->send(200, "text/html", "<head>"
                                "<meta http-equiv='refresh' content='0; URL=/'>"
                                "</head>");
                } else if (request->params() == 4) {
                        // check if all params exist
                        String params[4] = {"mode", "r1", "g1", "b1"};
                        if (!params_exist(request, params, 4)) {
                                request->send(404, "text/plain", "One or more parameters didn't exist!");
                                return;
                        }
                        uint8_t mode = request->getParam("mode")->value()[0];
                        uint8_t r = atoi(request->getParam("r1")->value().c_str());
                        uint8_t g = atoi(request->getParam("g1")->value().c_str());
                        uint8_t b = atoi(request->getParam("b1")->value().c_str());

                        Serial.printf("Mode switch: %c %d %d %d\n", mode, r, g, b);
                        mode_change(mode, r, g, b);

                        request->send(200, "text/html", "<head>"
                                "<meta http-equiv='refresh' content='0; URL=/'>"
                                "</head>"); 
                } else if (request->params() == 10) {
                        String params[10] = {"mode", "r1", "g1", "b1", "r2", "g2", "b2", "r3", "g3", "b3"};
                        if (!params_exist(request, params, 10)) {
                                request->send(404, "text/plain", "One or more parameters didn't exist!");
                                return;
                        }

                        uint8_t mode = request->getParam("mode")->value()[0];
                        uint8_t r1 = atoi(request->getParam("r1")->value().c_str());
                        uint8_t g1 = atoi(request->getParam("g1")->value().c_str());
                        uint8_t b1 = atoi(request->getParam("b1")->value().c_str());
                        uint8_t r2 = atoi(request->getParam("r2")->value().c_str());
                        uint8_t g2 = atoi(request->getParam("g2")->value().c_str());
                        uint8_t b2 = atoi(request->getParam("b2")->value().c_str());
                        uint8_t r3 = atoi(request->getParam("r3")->value().c_str());
                        uint8_t g3 = atoi(request->getParam("g3")->value().c_str());
                        uint8_t b3 = atoi(request->getParam("b3")->value().c_str());
                        
                        CRGB colors[3] = {CRGB(r1, g1, b1), CRGB(r2, g2, b2), CRGB(r3, g3, b3)};
                        mode_change(mode, colors);

                        request->send(200, "text/html", "<head>"
                                "<meta http-equiv='refresh' content='0; URL=/'>"
                                "</head>"); 
                }
        });

        server->on("/color", HTTP_GET, [this](AsyncWebServerRequest *request) {
                Serial.printf("Got a mode change request! Params: %d\n", request->params());
                if (request->params() == 3) {
                        String params[3] = {"r1", "g1", "b1"};
                        if (!params_exist(request, params, 3)) {
                                request->send(404, "text/plain", "One or more parameters didn't exist!");
                                return;
                        }
                        uint8_t r = atoi(request->getParam("r1")->value().c_str());
                        uint8_t g = atoi(request->getParam("g1")->value().c_str());
                        uint8_t b = atoi(request->getParam("b1")->value().c_str());

                        Serial.printf("Color change: %d %d %d\n", r, g, b);
                        color(r, g, b);

                        request->send(200, "text/html", "<head>"
                                "<meta http-equiv='refresh' content='0; URL=/'>"
                                "</head>"); 
                } else if (request->params() == 9) {
                        String params[9] = {"r1", "g1", "b1", "r2", "g2", "b2", "r3", "g3", "b3"};
                        if (!params_exist(request, params, 9)) {
                                request->send(404, "text/plain", "One or more parameters didn't exist!");
                                return;
                        }
                        uint8_t r1 = atoi(request->getParam("r1")->value().c_str());
                        uint8_t g1 = atoi(request->getParam("g1")->value().c_str());
                        uint8_t b1 = atoi(request->getParam("b1")->value().c_str());
                        uint8_t r2 = atoi(request->getParam("r2")->value().c_str());
                        uint8_t g2 = atoi(request->getParam("g2")->value().c_str());
                        uint8_t b2 = atoi(request->getParam("b2")->value().c_str());
                        uint8_t r3 = atoi(request->getParam("r3")->value().c_str());
                        uint8_t g3 = atoi(request->getParam("g3")->value().c_str());
                        uint8_t b3 = atoi(request->getParam("b3")->value().c_str());
                        
                        CRGB colors[3] = {CRGB(r1, g1, b1), CRGB(r2, g2, b2), CRGB(r3, g3, b3)};
                        color(colors);

                        request->send(200, "text/html", "<head>"
                                "<meta http-equiv='refresh' content='0; URL=/'>"
                                "</head>"); 
                }
        });

        server->on("/off", HTTP_GET, [this](AsyncWebServerRequest *request) {
                Serial.println("Got /off request!");
                off();
                request->send(200, "text/html", "<head>"
                                "<meta http-equiv='refresh' content='0; URL=/'>"
                                "</head>"); 
        });
}

void WebsiteHandler::onRequest(AsyncWebServerRequest *request)
{
        if (ON_STA_FILTER(request))
        {
                request->send(200, "text/plain", "Hello from STA");
                return;
        }
        else if (ON_AP_FILTER(request))
        {
                request->send(200, "text/plain", "Hello from AP");
                return;
        }

        request->send(200, "text/plain", "Hello from undefined");
}