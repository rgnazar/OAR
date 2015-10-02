void Motor_Milis_Dir_Micro()
{

  if (STOPRA )
  {
    digitalWrite(MotorRA_Ativa, HIGH);
  }
  else {
    if (FreqARMotor > VeloARMotor) {
      digitalWrite(MotorRA_Ativa, LOW);
      digitalWrite(MotorRA_Passo, HIGH);
      VeloARMotor = VeloARMotor + TimerARMotor;
      if (RALESTE )
      {
        digitalWrite(MotorRA_Direcao, HIGH);
        PassoRA++;
      }
      else
      {
        digitalWrite(MotorRA_Direcao, LOW);
        PassoRA--;
      }
    }
  }
  if (STOPDEC )
  {
    digitalWrite(MotorDEC_Ativa, HIGH);
  }
  else {
    if (FreqDECMotor > VeloDECMotor) {
      digitalWrite(MotorDEC_Ativa, LOW);
      digitalWrite(MotorDEC_Passo, HIGH);
      VeloDECMotor = VeloDECMotor + TimerDECMotor;
      if (DECNORTE )
      {
        digitalWrite(MotorDEC_Direcao, HIGH);
        PassoDEC++;
      }
      else
      {
        digitalWrite(MotorDEC_Direcao, LOW);
        PassoDEC--;
      }
    }
  }
  FreqARMotor = FreqARMotor + InterrupcaoPulso;
  FreqDECMotor = FreqDECMotor + InterrupcaoPulso;

  //Vitual micro segicrosegundo
  tempsegundo=second();
  if (tempmicroseconds < (1000000 - InterrupcaoPulso) || (tempsegundo == oldsegundo))
  {

    tempmicroseconds = tempmicroseconds + InterrupcaoPulso;
    segundodecimal = tempsegundo + (tempmicroseconds / 1000000);

  }
  else
  {
    oldsegundo = tempsegundo;
    segundodecimal = tempsegundo;
    tempmicroseconds = 0;
  }

  delayMicroseconds(5);
  digitalWrite(MotorDEC_Passo, LOW);
  digitalWrite(MotorRA_Passo, LOW);

}

void ParaTudo()
{
  STOPRA=true;
  STOPDEC=true;
}
void AtivaTudo()
{
  STOPRA=false;
  STOPDEC=false;
}
