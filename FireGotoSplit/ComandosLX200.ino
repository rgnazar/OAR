void executecommand()
{
  if (DEBUG == 2)
  {
    Serial.print("Commando: ");
    Serial.println(inputcmd);
    // SerialPrint(String(segundodecimal));
    //SerialPrint(String(PassoDEC));
    //SerialPrint(String(TimerMotorDEC));

  }
  if (cmdComplete) {
    //Para o envio ativo comando
    ultimoComando = 0;

    //Comeca a tratar os comandos
    if (inputcmd[1] == '$') {
      switch (inputcmd[2]) {
        case 'B':
          switch (inputcmd[3]) {
            case 'D':
              //              setDECbacklash();
              break;
            case 'R':
              //              setRAbacklash();
              break;
          }

      }
    }
    if (inputcmd[1] == 'C') {
      switch (inputcmd[2]) {
        case 'S':
          //          synctelescope();
          break;
        case 'M':
          //         synctelescopeString();
          break;

      }
    }
    if (inputcmd[1] == 'G') {
      switch (inputcmd[2]) {
        case 'A':
          //         printALTmount();
          break;
        case 'C':
          //          printDataLocal();
          break;
        case 'D':
          printDECmount();
          break;
        case 'g':
          //          printlongitude();
          break;
        case 'G':
          //          printUTC();
          break;
        case 'h':
          //          printAlturaLimite();
          break;
        case 'L':
          //          PrintLocalHora();
          break;
        case 'M':
          //           printObservatorioNome();
          break;
        case 'o':
          //          printHorizonteLimite();
          break;
        case 'R':
          printRAmount();
          break;
        case 'S':
          printSideralHora();
          break;
        case 't':
          //          printlatitude();
          break;
        case 'T':
          //           printSIderalRate();
          break;
        case 'U':
          //         printOperatocao();
          break;
        case 'W':
          //         printalinhamento();
          break;
        case 'Z':
          //          printAZmount();
          break;
        case 'V':
          switch (inputcmd[3]) {
            case 'D':
              //             printFirmwareDate();
              break;
            case 'F':
              //              printFirmware();
              break;
            case 'N':
              //              printFirmwareNumber();
              break;
            case 'P':
              //              printFirmwareNamne();
              break;
            case 'T':
              //             printFirmwareTime();
              break;
          }
          break;
      }
    }
  }
}
void printRAmount() //:GR# Get Telescope RA Returns: HH:MM.T# or HH:MM:SS#
{
  double horadec = Hora2DecHora(hour(), minute(), segundodecimal) ;
  double jdia = JulianoDay (year(), month(), day(), horadec) ;
  double HSA = HoraSideralAngulo(jdia);
  //Serial.println(HSA, 10);

  double HSAL = HoraSideraAngulollocal(longitude, HSA) ;
  //Serial.println(HSAL, 10);

  double RA = HSAL-EixoRAGra;

  if (RA < 0)
  {
    RA = RA + 360;
  }
    Serial.print("TimerMotorRA :");
    Serial.println(TimerMotorRA, 10);
    Serial.print("MicroSegporPassoSideralRA :");
    Serial.println(MicroSegporPassoSideralRA);

    Serial.print("RA :");
    Serial.println(RA, 10);

  int tmp = DecDeg2HoursHH(RA);
  if (tmp < 10) {
    SerialPrint("0");
  }
  SerialPrint(String(tmp));
  SerialPrint(":");
  tmp = DecDeg2HoursMM(RA);
  if (tmp < 10) {
    SerialPrint("0");
  }
  SerialPrint(String(tmp));
  SerialPrint(":");
  tmp = DecDeg2HoursSEC(RA);
  if (tmp < 10) {
    SerialPrint("0");
  }
  SerialPrint(String(tmp));
  SerialPrint("#");


}

void printDECmount() //:GD# Get Telescope Declination. Returns: sDD*MM# or sDD*MM'SS#
{
  int Ddeg = abs((int)DecDegtoDeg(EixoDECGra));
  int Min = abs((int)DecDegtoMin(EixoDECGra));
  int Sec = abs((int)DecDegtoSec(EixoDECGra));
  char str[9];

  if (EixoDECGra < 0) {
    sprintf(str, "-%02d*%02d:%02d#", int(Ddeg), int(Min), int(Sec));
  } else {
    sprintf(str, "+%02d*%02d:%02d#", int(Ddeg), int(Min), int(Sec));
  }
  // str[6] = 223;
  SerialPrint(str);
}

void printSideralHora()
{
  double horadec = Hora2DecHora(hour(), minute(), segundodecimal) ;
  //Serial.println(horadec,10);
  double jdia = JulianoDay (year(), month(), day(), horadec) ;
  //Serial.println((jdia+2451545.0),10);
  double HSA = HoraSideralAngulo(jdia);

  char str[10];
  sprintf(str, "%02d:%02d:%02d#", int(DecHour2HoursHH(HSA)), int(DecHour2HoursMM(HSA)), int(DecHour2HoursSEC(HSA)));
  SerialPrint(str);
}

