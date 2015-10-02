#include <DueTimer.h>
#include <math.h>
#include <Time.h>
#include <DueFlashStorage.h>

#define DEBUG 1

#define MotorRA_Direcao 23
#define MotorRA_Passo 25
#define MotorRA_Sleep 27
#define MotorRA_Reset 29
#define MotorRA_M2 31
#define MotorRA_M1 33
#define MotorRA_M0 35
#define MotorRA_Ativa 37
#define MotorDEC_Direcao 39
#define MotorDEC_Passo 41
#define MotorDEC_Sleep 43
#define MotorDEC_Reset 45
#define MotorDEC_M2 47
#define MotorDEC_M1 49
#define MotorDEC_M0 51
#define MotorDEC_Ativa 53

//Define o menor ciclo possivel do TIMER de interrupcao Primos 11 13 17 19 23 29 31 37 41 43 47 53 59 61 67 71 73 79 83 89  97 
#define InterrupcaoPulso 43

//Virtual microseconds
double segundodecimal=0, tempmicroseconds=0;
int oldsegundo=0, tempsegundo=0;

//Timer de acionamento dos passo dos motores
double VeloARMotor  = 0, TimerARMotor  = 1150, FreqARMotor = 0,
       VeloDECMotor = 0, TimerDECMotor = 1150, FreqDECMotor = 0;

//Controle do motor e direcao  
boolean  RALESTE=true, DECNORTE=false, STOPDEC=false, STOPRA=true; 
long PassoDEC=0, PassoRA=0;

//Armazenamento permanente de variaveis
dueFlashStorage dueFlashStorage;

// Estrutura de armazenamento permanente.
struct Configuration {
  int32_t MaxPassoAlt;
  int32_t MaxPassoAz;
  int32_t MinTimer;
  uint32_t DataHora;
  double latitude;
  double longitude;
  int32_t UTC;
  char* Local;
};
Configuration configuration;
Configuration configurationFromFlash; // Cria uma estrutura temporaria



void setup() {
//Iniciando as portas seriais
  Serial.begin(9600);
  Serial2.begin(9600);
  
  
  
  /* Flash is erased every time new code is uploaded. Write the default configuration to flash if first time */
  // running for the first time?
  uint8_t codeRunningForTheFirstTime = dueFlashStorage.read(0); // flash bytes will be 255 at first run
  Serial.print("Primeira Execucao ? : ");
  if (codeRunningForTheFirstTime) {
    Serial.println("yes");
    /* OK first time running, set defaults */
    configuration.MaxPassoAlt = 1856000;
    configuration.MaxPassoAz = 1856000;
    configuration.MinTimer = 180;
    configuration.latitude = -25.40;;
    configuration.longitude = -49.20;
    setTime(22, 00, 00, 23, 03, 2015);
    MilissegundoSeg = second();
    configuration.DataHora = now();
    configuration.UTC = 0;
    configuration.Local = "Minha Casa";
    // write configuration struct to flash at adress 4
    byte b2[sizeof(Configuration)]; // create byte array to store the struct
    memcpy(b2, &configuration, sizeof(Configuration)); // copy the struct to the byte array
    dueFlashStorage.write(4, b2, sizeof(Configuration)); // write byte array to flash
    // write 0 to address 0 to indicate that it is not the first time running anymore
    dueFlashStorage.write(0, 0);
  }
  else {
    Serial.println("no");
  }
  byte* b = dueFlashStorage.readAddress(4); // byte array which is read from flash at adress 4
  memcpy(&configurationFromFlash, b, sizeof(Configuration)); // copy byte array to temporary struct
  MaxPassoAlt = configurationFromFlash.MaxPassoAlt;
  MaxPassoAz = configurationFromFlash.MaxPassoAz;
  MinTimer = configurationFromFlash.MinTimer;
  latitude = configurationFromFlash.latitude;
  longitude = configurationFromFlash.longitude;
  UTC = configurationFromFlash.UTC;
  setTime(configurationFromFlash.DataHora);
  iniciapmotores();
  SerialPrint("00:00:00#"); //RTA para leitura do driver ASCOM da MEADE autostar I
  delay (200);
  previousMillis = millis();
  PCommadMillis = previousMillis;
  ErroAlt = ErroAz = 44.0;
  ResolucaoeixoAltGrausDecimal = 360.0 / MaxPassoAlt ;
  ResolucaoeixoAzGrausDecimal = 360.0 / MaxPassoAz ;

  
  //Iniciar as variaveis do motor de passo
  pinMode(MotorRA_Direcao, OUTPUT);
  pinMode(MotorRA_Passo, OUTPUT);
  pinMode(MotorRA_Sleep, OUTPUT);
  pinMode(MotorRA_Reset, OUTPUT);
  pinMode(MotorRA_M2, OUTPUT);
  pinMode(MotorRA_M1, OUTPUT);
  pinMode(MotorRA_M0, OUTPUT);
  pinMode(MotorRA_Ativa, OUTPUT);
  pinMode(MotorDEC_Direcao, OUTPUT);
  pinMode(MotorDEC_Passo, OUTPUT);
  pinMode(MotorDEC_Sleep, OUTPUT);
  pinMode(MotorDEC_Reset, OUTPUT);
  pinMode(MotorDEC_M2, OUTPUT);
  pinMode(MotorDEC_M1, OUTPUT);
  pinMode(MotorDEC_M0, OUTPUT);
  pinMode(MotorDEC_Ativa, OUTPUT);

//Aciona os pinos por padrão
  digitalWrite(MotorRA_Direcao, HIGH);
  digitalWrite(MotorRA_Passo, LOW);
  digitalWrite(MotorRA_Sleep, HIGH);
  digitalWrite(MotorRA_Reset, HIGH);
  digitalWrite(MotorRA_M2, LOW);
  digitalWrite(MotorRA_M1, LOW);
  digitalWrite(MotorRA_M0, LOW);
  digitalWrite(MotorRA_Ativa, LOW);
  digitalWrite(MotorDEC_Direcao, LOW);
  digitalWrite(MotorDEC_Passo, LOW);
  digitalWrite(MotorDEC_Sleep, HIGH);
  digitalWrite(MotorDEC_Reset, HIGH);
  digitalWrite(MotorDEC_M2, LOW);
  digitalWrite(MotorDEC_M1, LOW);
  digitalWrite(MotorDEC_M0, LOW);
  digitalWrite(MotorDEC_Ativa, LOW);

  //Inicia o Timer do motor
  Timer0.start(InterrupcaoPulso);
  Timer0.attachInterrupt(Motor_Milis_Dir_Micro);


}

void loop() {
  if (DEBUG == 1)
  {
   // Serial.println(TimerDECMotor);
   // Serial.println(FreqDECMotor);
    Serial.print(now());
    Serial.println(segundodecimal,6);
    Serial.println(PassoDEC);
    Serial.println(PassoRA);

  }
  RampaDEC();
  RampaRA();
  currentMillis = millis();
  if (PCommadMillis < currentMillis)
  {
  //  PrintLocalHora();
    PCommadMillis = PCommadMillis + 1497;
  }

}
