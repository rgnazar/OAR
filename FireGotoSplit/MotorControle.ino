void Motor_Milis_Dir_Micro()
{

  if (STOPRA )
  {
    digitalWrite(MotorRA_Ativa, HIGH);
  }
  else {
    if (IrqMotorRA > TimerMotorRA) {
      digitalWrite(MotorRA_Ativa, LOW);
      digitalWrite(MotorRA_Passo, HIGH);
      TimerMotorRA = VeloRAMotor + TimerMotorRA;
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
    if (IrqMotorDEC > TimerMotorDEC) {
      digitalWrite(MotorDEC_Ativa, LOW);
      digitalWrite(MotorDEC_Passo, HIGH);
      TimerMotorDEC = VeloDECMotor + TimerMotorDEC;
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
  IrqMotorRA = IrqMotorRA + InterrupcaoPulso;
  IrqMotorDEC = IrqMotorDEC + InterrupcaoPulso;

  //Vitual micro segicrosegundo
  tempsegundo = second();
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
  STOPRA = true;
  STOPDEC = true;
  //Zera Contador dos morotes
  IrqMotorRA = IrqMotorDEC = TimerMotorRA = TimerMotorDEC = 0;
}
void AtivaTudo()
{
  STOPRA = false;
  STOPDEC = false;

}
void RampaRA() //Gerencia Rampa de aceleracao do motor RA
{
  if (RampaRAAtiva)
  {
    RampaRACount++;
    double tempVeloRAMotor = VeloMaxRA * 100 / (25 + RampaRACount);
    if (tempVeloRAMotor < VeloMaxRA)
    {
      VeloRAMotor = VeloMaxRA;
    }
    else
    {
      VeloRAMotor = tempVeloRAMotor;
    }
  }
  else
  {
    RampaRACount = 0;
  }
}

void RampaDEC() //Gerencia Rampa de aceleracao do motor RA
{
  if (RampaDECAtiva)
  {
    RampaDECCount++;
    double tempVeloDECMotor = VeloMaxDEC * 100 / (25 + RampaDECCount);
    if (tempVeloDECMotor < VeloMaxDEC)
    {
      VeloDECMotor = VeloMaxDEC;
    }
    else
    {
      VeloDECMotor = tempVeloDECMotor;
    }
  }
  else
  {
    RampaDECCount = 0;
  }
}

void MovimentoSideral()
{
  if (AcompanhamentoAtivo)
  {
    VeloRAMotor = MicroSegporPassoSideralRA;
    VeloDECMotor = MicroSegporPassoSideralDEC;
    RampaDECAtiva = false;
    RampaRAAtiva = false;
  }
}

void CalculaPassoSidral()
{
  //Numeros total de passo para 360° / pelo dia sideral 23 horas, 56 minutos e 4 segundos (86164 Seg / 86400 / 86636)

  MicroSegporPassoSideralRA = (999999.999999999999999999999999 / NumeroPassoRA) * 86164;
  MicroSegporPassoSideralDEC = (999999.999999999999999999999999 / NumeroPassoDEC) * 86164;
}

void CalculaPosicaoMontagememGraus()
{
  EixoRAGra = PassoRA * 360.0 / NumeroPassoRA;
  EixoDECGra = PassoDEC * 360.0 / NumeroPassoDEC;
}


