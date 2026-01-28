#include <WiFi.h>

// ====== WIFI CONFIG ======
const char* ssid = "ExoloraTec Plataforma de Escaner 3D";
const char* password = "12345678";

WiFiServer server(80);

// ====== PINES ======
#define EN_PIN    25
#define DIR_PIN   26
#define STEP_PIN  27
#define HALL_PIN 34

// ====== HALL ======
int HallState = 0;
const int HallMicrosecondDelay = 200;

// ====== MOTOR ======
const int StepsPerRev = 3200;
const int XLVdgr = StepsPerRev / 8;
const int XVdgr  = StepsPerRev / 24;

int MotorPos = NULL;

// ===== TIME =====
const int MinMicrosecondDelay = 100;
const int MaxMicrosecondDelay = 2000;
int ActualMicrosecondsDelay = 1000;
int DegreeStepDelay = 7000;

int CurMicroseconds = int(millis());
int StartMicroseconds = NULL;

// ====== ESTADOS ======
bool button45 = false;
bool button30 = false;
bool button15 = false;
bool button5  = false;
bool buttonGo = false;
bool buttonMode= true;
bool Right = false;

bool bplay = false;
bool bpause = false;

bool needRefresh = false;


// ====== WIFI EVENTOS ======
void WiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info) {
  switch (event) {

    case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
      Serial.println("Cliente conectado");
      break;

    case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:
      Serial.println("Cliente desconectado -> RESET ESTADO");

      bpause = true;

      digitalWrite(EN_PIN, HIGH); // detener motor
      break;

    default:
      break;
  }
}

// ====== MOVER PASOS ======
void moveSteps(int steps, int microSecondsDelay) {
  for (int i = 0; i < steps; i++) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds( microSecondsDelay);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds( microSecondsDelay);
  }
 
   if(Right) MotorPos = MotorPos + steps;
   else MotorPos = MotorPos - steps;

   Serial.println(MotorPos);
   
}

// ====== DETENCION =======
void bstop(){
     button45 = false;
     button30 = false;
     button15 = false;
     button5  = false;
     buttonGo = false;
     bplay = false;
     bpause = false;
     buttonMode= true;
     Right = false;
     delay(1000);
     MotorPos = NULL;
     calibration();

     needRefresh = true;   // <<< NUEVO (OPCION 1)
  
}

// ====== CALIBRACION =====
void calibration(){

 while (analogRead(HALL_PIN) > 1000 && analogRead(HALL_PIN) < 3500)   {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(HallMicrosecondDelay);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(HallMicrosecondDelay);
    Serial.println(analogRead(HALL_PIN));
  }
 
    MotorPos = 0;
    Serial.print("El motor se encuentra en:");
    Serial.println(MotorPos);
}

// ====== HTML ======
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="es">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<meta http-equiv="Cache-Control" content="no-store">
<title>Plataforma Escaner 3D</title>
<style>
html { font-family: monospace; text-align: center; }
.row { display: flex; justify-content: center; gap: 20px; margin: 20px 0; }
.col { display: flex; flex-direction: column; gap: 5px; }
.button { background: yellowgreen; border: none; color: white; padding: 10px 15px; font-size: 20px; border-radius: 6px; }
.button2 { background: gray; }
.hidden { display:none; }
</style>
</head>
<body>
<h1>ExploraTec</h1>
<h2>Control Plataforma 3D</h2>

<div class="row">
<button id="btn1" class="button" onclick="togglePin(modeState,1);updateUI()">Grados</button>
<button id="btn2" class="button button2" onclick="togglePin(modeState,2);updateUI()">Velocidad</button>
</div>

<div id="mode1" class="col ">
<div class="row">
<button id="btn16" class="button button2" onclick="togglePin(pinState,16)">5°</button>
<button id="btn17" class="button button2" onclick="togglePin(pinState,17)">15°</button>
<button id="btn18" class="button button2" onclick="togglePin(pinState,18)">30°</button>
<button id="btn19" class="button button2" onclick="togglePin(pinState,19)">45°</button>
</div>

<h4>Segundos por paso</h4>

<input type="range" min="1" max="10" value="30" list="tickmarks" id="DegreeDelay">
<datalist id="tickmarks">
<option value="1" label="1"></option>
<option value="2" label="2"></option>
<option value="3" label="3"></option>
<option value="4" label="4"></option>
<option value="5" label="5"></option>
<option value="6" label="6"></option>
<option value="7" label="7"></option>
<option value="8" label="8"></option>
<option value="9" label="9"></option>
<option value="10" label="10"></option>
</datalist>
<span id="delayVal"></span>
</div>

<div id="mode2" class="col ">
<button id="btn20" class="button button2" onclick="togglePin(pinState,20)">Girar</button>
<input type="range" id="motorSpeed">
<span id="speedVal"></span>
</div>

<div id="cbtn" class="col ">

<div class="row">
<button id="btn3" class="button button2" onclick="togglePin(clockState,3)"><--</button>
<button id="btn4" class="button" onclick="togglePin(clockState,4)">--></button>
</div>
<h4>Estado de Control</h4>
<div class="row">
<button id="btn5" class="button button2" onclick="togglePin(playState,5)">▶︎</button>
<button id="btn6" class="button button2" onclick="togglePin(playState,6)" disabled="" style="opacity: 0.5; pointer-events: none;">⏸</button>
<button id="btn7" class="button button2" onclick="togglePin(playState,7)" disabled="" style="opacity: 0.5; pointer-events: none;">⏹</button>
</div>

</div>

<script>
const pinState   = {16:false,17:false,18:false,19:false,20:false};
const clockState = {3:false,4:true};
const playState  = {5:false, 6:false, 7:false}
const modeState  = {1:true  ,2:false};

function lockButtons(lock){
  const buttons = document.querySelectorAll("button");
  const sliders = document.querySelectorAll('input[type="range"]');

  buttons.forEach(btn => {
    const id = btn.id.replace("btn","");
    if(id != "6" && id != "7" && id!="20"){
      btn.disabled = lock;
      btn.style.opacity = lock ? "0.5" : "1";
      btn.style.pointerEvents = lock ? "none" : "auto";
    if(id == "3"&& modeState[2] || id == "4" && modeState[2]){
      btn.disabled = false;
        btn.style.opacity = false ? "0.5" : "1";
        btn.style.pointerEvents = false ? "none" : "auto";
    }
    }
  else{
    btn.disabled = !lock;
      btn.style.opacity = !lock ? "0.5" : "1";
      btn.style.pointerEvents = !lock ? "none" : "auto";
  
  }
  });

  sliders.forEach(slider => {
    if( modeState[2] != true){
    slider.disabled = lock;
    slider.style.opacity = lock ? "0.5" : "1";
    }
  });
}

function lockplay(lock) {
  const btn = document.getElementById("btn5");

  btn.disabled = lock;
  btn.style.opacity = lock ? "0.5" : "1";
  btn.style.pointerEvents = lock ? "none" : "auto";
}


function togglePin(State,pin){
  for(let p in State){
    if(p!=pin && State[p]){
      State[p]=false;
      updateButton(State,p);
      fetch(`/${p}/off`);
    }
  }

  State[pin]=!State[pin];
  updateButton(State,pin);
  fetch(`/${pin}/${State[pin]?'on':'off'}`);

  if(pin == 5 && State[pin]){
    lockButtons(true);
  }
  if((pin == 6) && State[pin]){
    lockplay(false);
  }

  if((pin == 7) && State[pin]){
    lockButtons(false);
  }
  
}

function updateButton(State,pin){
  const b=document.getElementById("btn"+pin);
  if(State[pin]) b.classList.remove("button2");
  else b.classList.add("button2");
}

function updateUI(){

  mode1.classList.toggle("hidden",!modeState[1]);
  mode2.classList.toggle("hidden",!modeState[2]);
  cbtn.classList.toggle("hidden",!(modeState[1]||modeState[2]));
  
}

const speedSlider=document.getElementById("motorSpeed");
const speedVal=document.getElementById("speedVal");

const DegreeSlider=document.getElementById("DegreeDelay");
const delayVal=document.getElementById("delayVal");

speedSlider.oninput=()=>speedVal.innerText=speedSlider.value;
speedSlider.onchange=()=>fetch(`/dslider/${speedSlider.value}`);

DegreeSlider.oninput=()=>delayVal.innerText=DegreeSlider.value;
DegreeSlider.onchange=()=>fetch(`/eslider/${DegreeSlider.value}`);

setInterval(() => {
  fetch("/state")
    .then(r => r.json())
    .then(d => {
      if (d.refresh) {
        loadState();
        updateUI();
        window.location.href = window.location.href.split('?')[0] + '?noCache=' + new Date().getTime();
      }
    });
}, 500);

function loadState(){
  fetch("/state").then(r=>r.json()).then(d=>{
    pinState[16]=d.b5;
    pinState[17]=d.b15;
    pinState[18]=d.b30;
    pinState[19]=d.b45;
    pinState[20]=d.go;

    modeState[1]= d.deg;
    modeState[2]= !d.deg;

    clockState[3]=!d.dir;
    clockState[4]= d.dir;

    playState[5]=d.play;
    playState[6]=d.pause;
    playState[7]=d.stop;

    speedSlider.value=d.speed;
    speedVal.innerText=d.speed;

    DegreeSlider.value=d.drdelay;
    delayVal.innerText=d.drdelay;

    for(let p in pinState) updateButton(pinState,p);
    for(let p in modeState) updateButton(modeState,p);
    for(let p in clockState) updateButton(clockState,p);
    for(let p in playState) updateButton(playState,p);

    if(playState[5] || playState[6]) lockButtons(true);
    if(playState[6]) lockplay(false);
     updateUI();
  });
}
window.onload  = function() {
  loadState();
};
</script>
</body>
</html>
)rawliteral";

// ====== SETUP ======
void setup() {
  Serial.begin(115200);

  pinMode(EN_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(HALL_PIN, INPUT);
  digitalWrite(EN_PIN, LOW);
  calibration();
  
  WiFi.onEvent(WiFiEvent);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  server.begin();
  Serial.println(WiFi.softAPIP());
}

// ====== LOOP ======
void loop() {
   CurMicroseconds = int(millis());
  // CONTROL MOTOR
  if(bplay && !bpause && (button5 || button15 || button30 || button45 || buttonGo)){
    digitalWrite(EN_PIN, LOW);  // Enable driver
    if((MotorPos >=3200 || MotorPos <=-3200) && !buttonGo){
      bstop();
    }
    if(button5){
      if(CurMicroseconds-StartMicroseconds > DegreeStepDelay){
      moveSteps(XVdgr/3, HallMicrosecondDelay);
      StartMicroseconds = int(millis());
      }
    }
    else if(button15){ 
      if(CurMicroseconds-StartMicroseconds > DegreeStepDelay){
      moveSteps(XVdgr, HallMicrosecondDelay);
      StartMicroseconds = int(millis());
      }
    }
    else if(button30){ 
      if(CurMicroseconds-StartMicroseconds > DegreeStepDelay){
      moveSteps(XVdgr*2, HallMicrosecondDelay);
      StartMicroseconds = int(millis());
      }
      
    }
    else if(button45){ 
      if(CurMicroseconds-StartMicroseconds > DegreeStepDelay){
      moveSteps(XLVdgr, HallMicrosecondDelay);
      StartMicroseconds = int(millis());
      }
    }
    else if(buttonGo){
      moveSteps(1, ActualMicrosecondsDelay);
    
    }
  }

  WiFiClient client = server.available();
  if(!client) return;

  String header="";
  while(client.connected()){
    if(client.available()){
      char c=client.read();
      header+=c;

      if(c=='\n'){
        if(header.indexOf("GET /state")>=0){
          String json="{";
          json+="\"b5\":"+String(button5?"true":"false")+",";
          json+="\"b15\":"+String(button15?"true":"false")+",";
          json+="\"b30\":"+String(button30?"true":"false")+",";
          json+="\"b45\":"+String(button45?"true":"false")+",";
          json+="\"go\":"+String(buttonGo?"true":"false")+",";
          json+="\"deg\":"+String(buttonMode?"true":"false")+",";
          json+="\"play\":"+String(bplay?"true":"false")+",";
          json+="\"pause\":"+String(bpause?"true":"false")+",";
          json+="\"dir\":"+String(digitalRead(DIR_PIN)?"true":"false")+",";

          json+="\"speed\":"+String(map(ActualMicrosecondsDelay, MinMicrosecondDelay,MaxMicrosecondDelay, 100,0))+",";

          json+="\"drdelay\":"+String(map(DegreeStepDelay, 1000,10000, 1,10)) + ",";

          json+="\"refresh\":"+String(needRefresh?"true":"false"); // <<< NUEVO
          json+="}";

          needRefresh = false; 

          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: application/json");
          client.println();
          client.println(json);
          break;
        }

       if(header.indexOf("GET /dslider/")>=0){
          ActualMicrosecondsDelay = header.substring(header.indexOf("/dslider/")+9).toInt();
          ActualMicrosecondsDelay = map(ActualMicrosecondsDelay, 0,100, MaxMicrosecondDelay,MinMicrosecondDelay);
          Serial.println(ActualMicrosecondsDelay);
        }

        if(header.indexOf("GET /eslider/")>=0){
          DegreeStepDelay = header.substring(header.indexOf("/eslider/")+9).toInt();
          DegreeStepDelay = map(DegreeStepDelay, 1,10, 1000,10000);
          Serial.println(DegreeStepDelay);
        }

        if(header.indexOf("GET /1/on")>=0) buttonMode=true;
        if(header.indexOf("GET /1/off")>=0) buttonMode=false;
        
        if(header.indexOf("GET /3/on")>=0){
          digitalWrite(DIR_PIN, LOW);
          Right = false;
          Serial.println("Left");
        }
        if(header.indexOf("GET /4/on")>=0){
          
          digitalWrite(DIR_PIN, HIGH);
           Right = true;
          Serial.println("Right");
        }

        if(header.indexOf("GET /5/on")>=0){
          bpause = false;
          bplay = true;
          StartMicroseconds = -999999;
        }
        if(header.indexOf("GET /5/off")>=0) bplay=false;
        if(header.indexOf("GET /6/on")>=0){
          bplay = false;
          bpause = true;  
        }
        if(header.indexOf("GET /6/off")>=0) bpause=false;
        if(header.indexOf("GET /7/on")>=0) bstop();
        
        if(header.indexOf("GET /16/on")>=0) button5=true;
        if(header.indexOf("GET /16/off")>=0) button5=false;
        if(header.indexOf("GET /17/on")>=0) button15=true;
        if(header.indexOf("GET /17/off")>=0) button15=false;
        if(header.indexOf("GET /18/on")>=0) button30=true;
        if(header.indexOf("GET /18/off")>=0) button30=false;
        if(header.indexOf("GET /19/on")>=0) button45=true;
        if(header.indexOf("GET /19/off")>=0) button45=false;
        if(header.indexOf("GET /20/on")>=0) buttonGo=true;
        if(header.indexOf("GET /20/off")>=0) buttonGo=false;

        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println();
        client.print(index_html);
        break;
      }
    }
  }
  client.stop();
}
