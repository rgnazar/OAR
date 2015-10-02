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
  }
}

