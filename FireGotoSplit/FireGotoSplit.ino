#include <DueTimer.h>
#include <flash_efc.h>
#include <efc.h>
#include <DueFlashStorage.h>
#include <Time.h>

#define DEBUG 2

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
#define InterrupcaoPulso 67


//Variaveis de controle para ler comandos LX200  ----------------------------------------------------------------------------------------------------------------
boolean cmdComplete = false, doispontos = true; // whether the string is complete
char buffercmd[30];
char inputcmd[30];// a string to hold incoming data
int pontBuffer = 0;
int pontCommand = 0;
int numCommand = 0;


//Virtual microseconds
double segundodecimal = 0, tempmicroseconds = 0;
int oldsegundo = 0, tempsegundo = 0;


//Informações Basicas do ambiente
int NumeroPassoDEC = 1850000, NumeroPassoRA = 1850000;
int VeloMaxDEC = 1150, VeloMaxRA = 1150;
double latitude = -25.40, longitude = -49.20;
int DataHora = 0, UTC = 0;
//    char local[20] = "Meu Observatorio";

//Vaiaveis da montagem
double EixoRAGra = 0, EixoDECGra = 0;


//Variaveis de controle
double currentMillis = 0;


//Variaveis de controle da execucao ativa de comando
double millisComondoInicial = 0;
int ultimoComando = 10;

//Timer de acionamento dos passo dos motores

double IrqMotorRA = 0;
double TimerMotorRA = 0;
double IrqMotorDEC = 0;
double TimerMotorDEC = 0;
boolean RampaRAAtiva = true;
double RampaRACount = 0;
boolean RampaDECAtiva = true;
double RampaDECCount = 0;
double VeloDECMotor = 100.01, VeloRAMotor = 100.01;
double TimerRampa = 0;

//Controle do motor e direcao
boolean  RALESTE = true, DECNORTE = false, STOPDEC = false, STOPRA = false;
long PassoDEC = 0, PassoRA = 0;
double GrauDecPorPassoRA = 0, GrauDecPorPassoDEC = 0;

//Micors Segundo por passo e acompanhamento
boolean AcompanhamentoAtivo = false;
double MicroSegporPassoSideralDEC = 0.1, MicroSegporPassoSideralRA = 0.1;

//Armazenamento permanente de variaveis
DueFlashStorage dueFlashStorage;

// Estrutura de armazenamento permanente.
struct Configuration {
  int32_t PNumeroPassoRA;
  int32_t PNumeroPassoDEC;
  int32_t PVeloMaxRA;
  int32_t PVeloMaxDEC;
  uint32_t PDataHora;
  double Platitude;
  double Plongitude;
  int32_t PUTC;
  char* PLocal;
};
Configuration configuration;
Configuration configurationFromFlash; // Cria uma estrutura temporaria



void setup() {
  //Iniciando as portas seriais
  Serial.begin(9600);
  Serial3.begin(9600);
  //Verifica se e primeira execucao
  uint8_t codeRunningForTheFirstTime = dueFlashStorage.read(0); // flash bytes will be 255 at first run
  Serial.print("Primeira Execucao ? : ");
  if (codeRunningForTheFirstTime) {
    SerialPrint("sim");
    /* Sim primeira execucao carraga valores iniciais do ambiente na memoria permanente*/
    configuration.PNumeroPassoDEC = NumeroPassoDEC;
    configuration.PNumeroPassoRA = NumeroPassoRA;
    configuration.PVeloMaxDEC = VeloMaxDEC;
    configuration.PVeloMaxRA = VeloMaxRA;
    configuration.Platitude = latitude;
    configuration.Plongitude = longitude;
    setTime(22, 13, 35, 23, 10, 2015);
    configuration.PDataHora = now();
    configuration.PUTC = UTC;
    //    configuration.PLocal = Local;
    // write configuration struct to flash at adress 4
    byte b2[sizeof(Configuration)]; // create byte array to store the struct
    memcpy(b2, &configuration, sizeof(Configuration)); // copy the struct to the byte array
    dueFlashStorage.write(4, b2, sizeof(Configuration)); // write byte array to flash
    // write 0 to address 0 to indicate that it is not the first time running anymore
    dueFlashStorage.write(0, 0);
  }
  else {
    SerialPrint("nao");
    /* Não valores ja carregados anteriormente */
  }
  //carraga valores do ambiente da memoria permanente
  byte* b = dueFlashStorage.readAddress(4); // byte array which is read from flash at adress 4
  memcpy(&configurationFromFlash, b, sizeof(Configuration)); // copy byte array to temporary struct
  NumeroPassoRA = configurationFromFlash.PNumeroPassoRA;
  NumeroPassoDEC = configurationFromFlash.PNumeroPassoDEC;
  VeloMaxDEC = configurationFromFlash.PVeloMaxDEC;
  VeloMaxRA = configurationFromFlash.PVeloMaxRA;
  latitude = configurationFromFlash.Platitude;
  longitude = configurationFromFlash.Plongitude;
  UTC = configurationFromFlash.PUTC;
  setTime(configurationFromFlash.PDataHora);
  // Local = configurationFromFlash.PLocal;

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
  digitalWrite(MotorRA_M2, HIGH);
  digitalWrite(MotorRA_M1, HIGH);
  digitalWrite(MotorRA_M0, HIGH);
  digitalWrite(MotorRA_Ativa, LOW);
  digitalWrite(MotorDEC_Direcao, LOW);
  digitalWrite(MotorDEC_Passo, LOW);
  digitalWrite(MotorDEC_Sleep, HIGH);
  digitalWrite(MotorDEC_Reset, HIGH);
  digitalWrite(MotorDEC_M2, HIGH);
  digitalWrite(MotorDEC_M1, HIGH);
  digitalWrite(MotorDEC_M0, HIGH);
  digitalWrite(MotorDEC_Ativa, LOW);

  //Inicia o Timer do motor
  CalculaPassoSidral(); //Função que define quantos microsegundo deve ter cada passo

  //Calcula quantos grau por passo
  GrauDecPorPassoRA = 360.0 / NumeroPassoRA;
  GrauDecPorPassoDEC = 360.0 / NumeroPassoDEC;

  Timer0.start(InterrupcaoPulso);
  Timer0.attachInterrupt(Motor_Milis_Dir_Micro);
  //Contador de millis
  currentMillis = millis();
}

void loop() {
  currentMillis = millis();
  AcompanhamentoAtivo = true;
  MovimentoSideral();
  CalculaPosicaoMontagememGraus();
  printRAmount();



  if (DEBUG == 1)
  { /*
        Serial.print("horadec: ");
        Serial.println(horadec);
      /*  Serial.print("VeloMaxRA: ");
        Serial.println(VeloMaxRA);
        Serial.print("VeloRAMotor: ");
        Serial.println(VeloRAMotor);
     Serial.print("MicroSegporPassoSideralRA= ");
     Serial.println(MicroSegporPassoSideralRA);
     Serial.print("GrauPorPassoRA :");
     Serial.println(EixoRAGra, 5);
     //SerialPrint(String(TimerMotorDEC))*/;

  }

  //Timer de acionamento da rampa
  if (TimerRampa < currentMillis)
  {
    TimerRampa = currentMillis + 20;
    RampaDEC();
    RampaRA();
  }


  //Este IF manda um comando "00:00:00#" para iniciar a comunicacao de forma ativa
  if (millisComondoInicial < currentMillis)
  {
    millisComondoInicial = currentMillis + 497;
    if (ultimoComando > 10)
    {
      SerialPrint("00:00:00#");
    }
    else {
      ultimoComando++;
    }
  }

}
