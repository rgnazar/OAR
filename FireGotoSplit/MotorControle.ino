void Motor_Milis_Dir_Micro()
{
  if (FreqARMotor > VeloARMotor) {
    digitalWrite(MotorRA_Passo, HIGH);
    VeloARMotor = VeloARMotor + TimerARMotor;
  }
  if (FreqDECMotor > VeloDECMotor) {
    digitalWrite(MotorDEC_Passo, HIGH);
    VeloDECMotor = VeloDECMotor + TimerDECMotor;
  }
  FreqARMotor = FreqARMotor + InterrupcaoPulso;
  FreqDECMotor = FreqDECMotor + InterrupcaoPulso;
  delayMicroseconds(15);
  digitalWrite(MotorDEC_Passo, LOW);
  digitalWrite(MotorRA_Passo, LOW);

}
