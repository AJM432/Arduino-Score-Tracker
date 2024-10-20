// Helper module for score tracker

int isDigit(String c) { return (c >= "0") && (c <= "8"); } // can add up to 9 if requred

void ledON()
{
  if (ledEnabled)
  {
    digitalWrite(gLEDPin, HIGH);
    greenTimer = millis();
  }
}

void buzz(int note)
{
  if (buzzEnabled)
  {
    tone(buzzerPin, note, duration);
    delay(2 * duration);
  }
}

void update_t1(int new_score)
{
  ledON();
  buzz(NOTE_C5);
  if (new_score > t1)
  {
    buzz(NOTE_C6);
  }
  t1 = new_score;
}

void update_t2(int new_score)
{
  ledON();
  buzz(NOTE_B5);
  if (new_score > t2)
  {
    buzz(NOTE_C6);
  }
  t2 = new_score;
}

void showLCDMsg(String line1, String line2, int duration)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
  delay(duration);
}

String translateIR() // takes action based on IR code received
{
  if (millis() - irTimer < 500)
  {
    return "";
  }
  // Check if it is a repeat IR code
  if (irrecv.decodedIRData.flags)
  {
    // set the current decodedRawData to the last decodedRawData
    irrecv.decodedIRData.decodedRawData = last_decodedRawData;
  }
  else
  {
    // output the IR code on the serial monitor
  }

  // map the IR code to the remote key
  switch (irrecv.decodedIRData.decodedRawData)
  {
  case 0xBA45FF00:
    return "POWER";
  case 0xB847FF00:
    return "FUNC/STOP";
  case 0xB946FF00:
    return "VOL+";
  case 0xBB44FF00:
    return "FAST BACK";
  case 0xBF40FF00:
    return "PAUSE";
  case 0xBC43FF00:
    return "FAST FORWARD";
  case 0xF807FF00:
    helpLocation -= 2;
    return "DOWN";
  case 0xEA15FF00:
    return "VOL-";
  case 0xF609FF00:
    helpLocation += 2;
    return "UP";
  case 0xE619FF00:
    return "EQ";
  case 0xF20DFF00:
    return "ST/REPT";
  case 0xE916FF00:
    return "0";
  case 0xF30CFF00:
    return "1";
  case 0xE718FF00:
    return "2";
  case 0xA15EFF00:
    return "3";
  case 0xF708FF00:
    return "4";
  case 0xE31CFF00:
    return "5";
  case 0xA55AFF00:
    return "6";
  case 0xBD42FF00:
    return "7";
  case 0xAD52FF00:
    return "8";
  case 0xB54AFF00:
    helpEnabled ^= 1;
    return "9";
  default:
    return "";
  } // End Case
  // store the last decodedRawData
  last_decodedRawData = irrecv.decodedIRData.decodedRawData;
  irTimer = millis();
} // END translateIR

void power_cycle()
{
  t1 = 0;
  t2 = 0;
  for (int i = 0; i < 4; i++)
  {
    playersArray[i] = 0;
  }
  numPlayers = 0;
  helpLocation = 0;
  lcd.clear();
  for (int i = 0; buzzEnabled && (i < 8); i++)
  {
    tone(buzzerPin, melody[i], duration);
    delay(duration);
  }
  lcd.setCursor(0, 1);
  for (int i = 25; i > 0; i--)
  {
    if (ledEnabled && (i % 5 == 0))
    {
      digitalWrite(gLEDPin, HIGH);
    }
    showLCDMsg("POWERCYCLE: " + String(i), "", 25);
    digitalWrite(gLEDPin, LOW);
    lcd.clear();
  }
}

void displayHelp()
{
  state = 2;
  const String options[] = {
      "Help Menu", "9",
      "Down", "DOWN",
      "UP", "UP",
      "T1+", "VOL+",
      "T1-", "PAUSE",
      "T2+", "FUNC/STOP",
      "T2-", "FAST FORWARD",
      "Play", "FAST BACK",
      "Finish", "VOL-",
      "Buzzer", "EQ",
      "LED", "ST/REPT",
      "Contrast+", "8",
      "Contrast-", "7"};

  int arraySize = sizeof(options) / sizeof(String);
  if (helpLocation < 0)
  {
    helpLocation = arraySize;
  }
  showLCDMsg(options[helpLocation % arraySize], options[(helpLocation + 1) % arraySize], 0);
}

void complete_game()
{
  state = 1;
  if (numPlayers == 2)
  {
    if (t1 > t2)
    {
      String p0 = players[playersArray[0]];
      showLCDMsg(p0 + " Wins!", "", 0);
    }
    else if (t1 < t2)
    {
      String p1 = players[playersArray[1]];
      showLCDMsg(p1 + " Wins!", "", 0);
    }
    else
    {
      showLCDMsg("Tie", "", 0);
    }
  }
  else if (numPlayers == 4)
  {
    if (t1 > t2)
    {
      String p0 = players[playersArray[0]].substring(0, 5);
      String p1 = players[playersArray[1]].substring(0, 5);
      showLCDMsg(p0 + "/" + p1 + " Wins!", "", 0);
    }
    else if (t1 < t2)
    {
      String p2 = players[playersArray[2]].substring(0, 5);
      String p3 = players[playersArray[3]].substring(0, 5);
      showLCDMsg(p2 + "/" + p3 + " Wins!", "", 0);
    }
    else
    {
      showLCDMsg("Tie", "", 0);
    }
  }
}

void play()
{
  state = 0;
  if (numPlayers == 2)
  {
    String p0 = players[playersArray[0]];
    String p1 = players[playersArray[1]];
    showLCDMsg(p0 + ": " + String(t1), p1 + ": " + String(t2), 0);
  }
  else if (numPlayers == 4)
  {
    String p0 = players[playersArray[0]].substring(0, 5);
    String p1 = players[playersArray[1]].substring(0, 5);
    String p2 = players[playersArray[2]].substring(0, 5);
    String p3 = players[playersArray[3]].substring(0, 5);
    showLCDMsg(p0 + "/" + p1 + ": " + String(t1), p2 + "/" + p3 + ": " + String(t2), 0);
  }
  else
  {
  }
}

void displayStart()
{

  showLCDMsg("Singles: 1", "Doubles: 2", 0);

  lcd.blink();
  while (true)
  {
    if (irrecv.decode())
    {
      String ret = translateIR();
      if (ret == "1")
      {
        numPlayers = 2;
        break; // Exit the loop when "0" is pressed
      }
      else if (ret == "2")
      {
        numPlayers = 4;
        break; // Exit the loop when "0" is pressed
      }
      irrecv.resume(); // Receive the next value
    }
  }
  irrecv.resume(); // Receive the next value
  lcd.noBlink();

  for (int i = 0; i < numPlayers; i++)
  {
    int playerIndex = -1;
    showLCDMsg("Enter player #" + String(i), "", 0);
    lcd.blink();

    while (true)
    {
      if (irrecv.decode())
      {
        String ret = translateIR();
        if (isDigit(ret))
        {
          playerIndex = ret.toInt();
          playersArray[i] = playerIndex;
          lcd.print(players[playerIndex]);
          delay(400);
          break;
        }
        irrecv.resume(); // Receive the next value
      }
    }
    lcd.noBlink();
    irrecv.resume(); // Receive the next value
  }
  play();
}

void return_to_previous_screen()
{
  switch (state)
  {
  case 0:
    play();
    return;
  case 1:
    complete_game();
    return;
  case 2:
    displayHelp();
    return;
  default:
    return;
  }
}