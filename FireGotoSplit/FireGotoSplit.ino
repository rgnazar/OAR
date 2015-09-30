#include <DueTimer.h>

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

//Define o menor ciclo possivel do TIMER de interrupcao
#define InterrupcaoPulso 33



//Timer de acionamento dos passo dos motores
double VeloARMotor  = 0, TimerARMotor  = 1150, FreqARMotor = 0,
       VeloDECMotor = 0, TimerDECMotor = 1150, FreqDECMotor = 0;




void setup() {

  Serial.begin(9600);
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
  Timer3.start(InterrupcaoPulso);
  Timer3.attachInterrupt(Motor_Milis_Dir_Micro);


}

void loop() {
  if (DEBUG == 1)
  {
    Serial.println(TimerDECMotor);
    Serial.println(FreqDECMotor);
    Serial.println(InterrupcaoPulso);
  }
  // put your main code here, to run repeatedly:

}
