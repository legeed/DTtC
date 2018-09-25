//https://github.com/Rodot/UFO-Race-META/blob/master/UFO-Race/highscore.ino


int highscore[NUM_HIGHSCORE];
char name[NUM_HIGHSCORE][NAMELENGTH+1];

const uint16_t highscore_sound[] PROGMEM = {
  0x0005,0x140,0x150,0x15C,0x170,0x180,0x16C,0x154,0x160,0x174,0x184,0x14C,0x15C,0x168,0x17C,0x18C,0x0000};

void initHighscore(){
  for(byte thisScore = 0; thisScore < NUM_HIGHSCORE; thisScore++){
    highscore[thisScore] = gb.save.get(2*thisScore);
    gb.save.get(2*thisScore + 1, name[thisScore], NAMELENGTH+1);
  }
}

void saveHighscore(unsigned int score){
  gb.display.fill(DARKGRAY);
  if(score < highscore[NUM_HIGHSCORE-1]){//if it's a highscore
    if(drawNewHighscore(score)){
      gb.getDefaultName(name[NUM_HIGHSCORE-1]);
      gb.gui.keyboard("NEW HIGHSCORE!NAME?", name[NUM_HIGHSCORE-1]);
      highscore[NUM_HIGHSCORE-1] = score;
      for(byte i=NUM_HIGHSCORE-1; i>0; i--){ //bubble sorting FTW
        if(highscore[i-1] > highscore[i]){
          char tempName[NAMELENGTH];
          strcpy(tempName, name[i-1]);
          strcpy(name[i-1], name[i]);
          strcpy(name[i], tempName);
          unsigned int tempScore;
          tempScore = highscore[i-1];
          highscore[i-1] = highscore[i];
          highscore[i] = tempScore;
        }
        else{
          break;
        }
      }
      for(byte thisScore = 0; thisScore < NUM_HIGHSCORE; thisScore++){
        gb.save.set(2*thisScore, highscore[thisScore]);
        gb.save.set(2*thisScore + 1, name[thisScore]);
      }
      drawHighScores();
    }
  }
  else{
      String scoreStr = "Score: " + String(score);
      gb.gui.popup(scoreStr.c_str(), 20);
  }
}

void drawHighScores(){
  gb.display.fill(DARKGRAY);
  while(true){
    if(gb.update()){
      gb.display.clear();
      gb.display.cursorX = 9+random(0,2);
      gb.display.cursorY = 0+random(0,2);
      gb.display.println("BEST SCORE");
      gb.display.textWrap = false;
      gb.display.cursorX = 0;
      gb.display.cursorY = gb.display.fontHeight*2;
      for(byte thisScore=0; thisScore<NUM_HIGHSCORE; thisScore++){
        if(highscore[thisScore]==0)
          gb.display.print('-');
        else
          gb.display.print(name[thisScore]);
        gb.display.cursorX = gb.display.width()-4*gb.display.fontWidth;
        gb.display.cursorY = gb.display.fontHeight*2 + gb.display.fontHeight*thisScore;
        gb.display.println(highscore[thisScore]);
      }
      if(gb.buttons.released(BUTTON_A) || gb.buttons.released(BUTTON_B) || gb.buttons.released(BUTTON_C)){
        gb.sound.playOK();
        break;
      }
    }
  }
}

boolean drawNewHighscore(unsigned int score){
  gb.display.fill(DARKGRAY);
  //gb.sound.playPattern(highscore_sound, 0);
  gb.sound.playOK();
  int timer = 150;
  while(1){
    if(gb.update()){
      timer--;
      if(!timer){
        return true;
      }
      if(gb.buttons.released(BUTTON_B)){
        return true;
      }
      gb.display.clear();
      gb.display.cursorX = 1;
      gb.display.cursorY = 1;
      gb.display.print("NEW HIGHSCORE!");
      gb.display.cursorX = 1+random(0,2);
      gb.display.cursorY = 16+random(0,2);
      gb.display.setFontSize(3);
      gb.display.println(score);
      gb.display.setFontSize(1);
      
      gb.display.cursorX = 0;
      gb.display.cursorY = gb.display.height() - (gb.display.fontHeight * 5);
      gb.display.setColor(LIGHTGREEN);
      gb.display.print("\n Best  ");
      gb.display.print(highscore[0]);
      gb.display.setColor(RED);
      gb.display.print("\n Worst ");
      gb.display.print(highscore[NUM_HIGHSCORE-1]);
      
      gb.display.cursorX = 0;
      gb.display.cursorY = gb.display.height() - (gb.display.fontHeight * 1);
      gb.display.setColor(GRAY);
      gb.display.print(" B: SAVE");
    }
  }
}
