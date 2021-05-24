#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<stdbool.h>
#include<assert.h>
#include<time.h>
#include<conio.h>
#include<windows.h>

//Array
#define Apple 0
#define Player 1
#define column 0
#define row 1
//Settings
#define DefaultSpeed 100
#define DefaultWidth 24
#define DefaultHeight 16
#define MaxTail 1344
#define MaxBlocks 1344
#define MaxTries 10
//Directions
#define LEFT 1
#define UP 2
#define RIGHT 3
#define DOWN 4

/*---------------------SCREENSIZE/FULLSCREEN-----------------------*/
void maximizeScreen(){
    HWND hwnd = GetConsoleWindow();       //Get HWND
    
    ShowWindow(hwnd, SW_MAXIMIZE);        //Maximize Window (I do not know how it works i just know it does)
}

void fullscreen(){
    keybd_event(VK_MENU,0x38,0,0);                      //Use alt key
    keybd_event(VK_RETURN,0x1c,0,0);                    //Use Enter key
    keybd_event(VK_MENU,0x38,KEYEVENTF_KEYUP,0);        //Release alt Key
    keybd_event(VK_RETURN,0x1c,KEYEVENTF_KEYUP,0);      //Release Enter Key
}
/*-----------------------------------------------------------------*/

/*------------------------MENUS-------------------------*/
int menusInput(int cursor, int options, bool IsPause, int SoundSetting){
    printf("\x1b[?25l");                //Turn off cursor
    int keypress;

    do{
        printf("\x1b[s");               //Save cursor position
        keypress = getch();
        switch(keypress){
            case 72: //Up key
                if(cursor > 1){
                    printf(" \x1b[1D");                 //Clean game Cursor (">")
                    printf("\x1b[1D");                  //Move Cursor left 1 column
                    printf("\x1b[1A");                  //Move Cursor Up 1 line
                    printf("\x1b[5m>\x1b[0m");          //Draw game Cursor (">")
                    printf("\x1b[1D");                  //Move Cursor left 1 column

                    cursor--;                           //Change Cursor value

                    if(SoundSetting)                    //Menu Sound
                        Beep(880, 90); 
                }else{                                  //loop back/Go to last option
                    cursor = options;
                    printf(" \x1b[%dB", options-1);     //Move cursor the max lines/to the last option
                    printf("\x1b[1D");                  //Move Cursor left 1 column
                    printf("\x1b[5m>\x1b[0m");          //Draw game Cursor (">")
                    printf("\x1b[1D");                  //Move Cursor left 1 column
                }
                break;
            case 80: //Down key
                if(cursor < options){
                    printf(" \x1b[1D");                 //Clean game Cursor (">")
                    printf("\x1b[1D");                  //Move Cursor left 1 column
                    printf("\x1b[1B");                  //Move Cursor Up 1 line
                    printf("\x1b[5m>\x1b[0m");          //Draw game Cursor (">")
                    printf("\x1b[1D");                  //Move Cursor left 1 column

                    cursor++;                           //Change Cursor value
                    
                    if(SoundSetting)                    //Menu Sound
                        Beep(880, 90);   
                }else{
                    cursor = 1;
                    printf(" \x1b[%dA", options-1);     //Go to the second line/First option
                    printf("\x1b[1D");                  //Move Cursor left 1 column
                    printf("\x1b[5m>\x1b[0m");          //Draw game Cursor (">")
                    printf("\x1b[1D");                  //Move Cursor left 1 column
                }
                break;
            case 27: //[ESC] key
                if(IsPause)                         //Detects if its in pause menu
                    cursor = 1;                     //Continue Game
                else
                    cursor = options;               //Last option normally go back/quit
                
                keypress = '\r';                    //Set key as enter
                break;
        }
    }while(keypress != '\r');                       //While user doesnt press enter
    printf(" >\x1b[2D");                            //Move game cursor to the right
    
    if(SoundSetting)                                //Select sound
        Beep(587, 90);

    return cursor; //return the value/what option selected
}

int mainMenu(int colorHead, int colorTail, int gameVersion, int patchVersion, int SoundSetting){
    printf("\x1b[?25l");
    system("cls || clear");         //Clean screen

    /*---------------------TITLE-----------------------*/
    printf("\n\x1b[%dmSquare ", colorTail);             //Title
    printf("\x1b[%dmBracket ", colorHead);
    printf("\x1b[%dmSnake ", colorTail);
    printf("\x1b[0m");                                  //Clear effects/colors
    printf("[%d.%d]\n", gameVersion, patchVersion);     //Game/Patch Version
    
    /*-------------------------MAIN MENU OPTIONS-------------------------*/
    printf("\x1b[s");                   //save cursor position
    printf("\x1b[5m>\x1b[0m");          //Create game cursor with blink ("Only works in some cmds")
    //Options
    printf(" Play");
    printf("\n  Settings");
    printf("\n  Quit");
    printf("\x1b[u");                   //Go back to last saved position 

    int cursor;                         //cursor value

    cursor = menusInput(1, 3, false, SoundSetting); //Use function menusInput to get what option the user wants, using the arrow keys
    
    return cursor;
}

int printSubeSetting(int cursor, int TextSelector, int color, int leftColor, int rightColor, int leftBlink, int rightBlink, char **ScreenSettingText, char **SoundSettingText, char **SpeedSettingText, char **SizeSettingText, char **ModeSettingText, char **Colors, char **Shapes){
    printf("\x1b[%d;%dm< ", leftBlink, leftColor);      //Left arrow
    //(leftColor defines if its green(32) or red(31), leftBlink defines if its blinking (5) or not (1))
    printf("\x1b[0m");      //Clean Colors and effects (Blink)

    printf("\x1b[%dm", color);  //Color of the option
    printf("%s", ((TextSelector == 1) ? ScreenSettingText[cursor] :                     //If option is 1 show screen sub options
                    (TextSelector == 2) ? SoundSettingText[cursor] :                    //If option is 2 show Sound sub options
                        (TextSelector == 3) ? SpeedSettingText[cursor] :                //If option is 3 show Speed sub options
                            (TextSelector == 4) ? SizeSettingText[cursor] :             //If option is 4 show Size sub options
                                (TextSelector == 5) ? ModeSettingText[cursor] :         //If option is 5 show Mode sub options
                                    (TextSelector == 6) ? Colors[cursor-30] :           //If option is 6 show Apple sub options
                                        (TextSelector == 7) ? Colors[cursor-30] :       //If option is 7 show Head sub options
                                            (TextSelector == 8) ? Colors[cursor-30] :   //If option is 8 show Tail sub options
                                                Shapes[cursor]));                       //If option is 9 show Shape sub options
    
    printf(" \x1b[%d;%dm>", rightBlink, rightColor);               //Right Arrow
    //(rightColor defines if its green(32) or red(31), rightBlink if its blinking (5) or not (1))
    
    printf("\x1b[0m");      //Clean Colors and effects (Blink)
}

int sideInput(int keypress, char *text, int cursor, int minOption, int maxOption, int TextSelector, int SoundSetting, char **ScreenSettingText, char **SoundSettingText, char **SpeedSettingText, char **SizeSettingText, char **ModeSettingText, char **Colors, char **Shapes){
    printf("\x1b[?25l");
    int color;  //Color of text

    printf(" \x1b[32m>\x1b[0m");            //Selected option
    printf("%s - ", text);                  //Setting text
    switch(keypress){
        case 75: //Left
            if(cursor > minOption){
                cursor--;                   //Cursor
                
                color = (cursor >= 30) ? cursor : 0;           //Check if it needs color, if it needs, had the correct value
   
                printSubeSetting(cursor, TextSelector, color, 33, 33, 5, 0, ScreenSettingText, SoundSettingText, SpeedSettingText, SizeSettingText, ModeSettingText, Colors, Shapes);
                
                printf("\x1b[K");               //Delete Rest of the line
                
                if(SoundSetting)
                    Beep(987, 90);
            }else{
                color = (cursor >= 30) ? cursor : 0;            //Check if it needs color, if it needs, had the correct value
                
                printSubeSetting(cursor, TextSelector, color, 31, 33, 5, 0, ScreenSettingText, SoundSettingText, SpeedSettingText, SizeSettingText, ModeSettingText, Colors, Shapes);
                printf("\x1b[K");               //Delete Rest of the line

                if(SoundSetting)
                    Beep(698, 90);
            }
            break;
        case 77: //Right
            if(cursor < maxOption){
                cursor++;
                color = (cursor >= 30) ? cursor : 0;            //Check if it needs color, if it needs, had the correct value

                printSubeSetting(cursor, TextSelector, color, 33, 33, 0, 5, ScreenSettingText, SoundSettingText, SpeedSettingText, SizeSettingText, ModeSettingText, Colors, Shapes);

                printf("\x1b[K");               //Delete Rest of the line

                if(SoundSetting)
                    Beep(987, 90);
            }else{
                color = (cursor >= 30) ? cursor : 0;            //Check if it needs color, if it needs, had the correct value
                
                printSubeSetting(cursor, TextSelector, color, 33, 31, 0, 5, ScreenSettingText, SoundSettingText, SpeedSettingText, SizeSettingText, ModeSettingText, Colors, Shapes);

                printf("\x1b[K");               //Delete Rest of the line
                
                if(SoundSetting)
                    Beep(698, 90);
            }
            break;
        case 27:    //ESC
        case 13:    //Enter
            color = (cursor >= 30) ? cursor : 0;                //Check if it needs color, if it needs, had the correct value

            printf("\x1b[u");
            printf("> ");       //Option Selected
            printf("%s - ", text);  //text of Selected //Left arrow

            printSubeSetting(cursor, TextSelector, color, 0, 0, 1, 1, ScreenSettingText, SoundSettingText, SpeedSettingText, SizeSettingText, ModeSettingText, Colors, Shapes);

            if(SoundSetting)
                Beep(587, 90);
            break;
    }
    printf("\x1b[0m");              //Clear colors
    printf("\x1b[u");               //Reset Cursor position

    return cursor;
}

int settings(int *velocity, int *width , int * height, bool *TotalReset, int *ScreenSetting, int *SoundSetting, int *SpeedSetting, int *SizeSetting, int *ModeSetting, int *colorApple, int *colorHead, int *colorTail, char **Shapes, int *ShapeSetting, int highScore){
    printf("\x1b[?25l");    //Turn off Console Cursor

    printf("\x1b[H\x1b[J\x1b[s");       //Go to 0;0, Clean Screen, Save Cursor Position
    printf("\nSettings\n");
    
    int cursor = 1, keypress, maxOption;
    bool AlreadyFullScreen = (*ScreenSetting) ? true : false;           //Just a bool to detect if screen is already Fullscreen (not the actual screen, but the setting on the code)

    //All Settings text, this is for design
    char *ScreenSettingText[2] = {"Windowed", "FullScreen"};
    char *SoundSettingText[2] = {"Off", "On"};
    char *SpeedSettingText[4] = {"Slow", "Normal", "Fast", "Caffeine Addiction"};
    char *SizeSettingText[3] = {"Small", "Normal", "Big"};
    char *ModeSettingText[4] = {"Walls", "No Walls", "Blocks", "Zen"};
    char *Colors[8] = {"Black", "Red", "Green", "Yellow", "Blue", "Magenta", "Cyan", "White"};

    //This string array has all the settings, Some need space to align correctly
    char *ExistingSettings[9] = {"Screen", "Sound ", "Speed ", "Size  ", "Mode  ", "Apple ", "Head  ", "Tail  ", "Shape "};

    /*Draws the options dor the users*/
    maxOption = 7;  //How many options are drawn already counting reset and back
    printf("\x1b[s");
    printf("\x1b[5m>\x1b[0m ");                                                 //Game cursor
    printf("Screen - < %s >", ScreenSettingText[*ScreenSetting]);               //Screen setting
    printf("\n  Sound  - < %s >", SoundSettingText[*SoundSetting]);             //Sound setting
    printf("\n  Speed  - < %s >", SpeedSettingText[*SpeedSetting]);             //Speed setting
    printf("\n  Size   - < %s >", SizeSettingText[*SizeSetting]);               //Size setting
    printf("\n  Mode   - < %s >", ModeSettingText[*ModeSetting]);               //Mode setting
    
    //Gets highscore to create more options
    if(highScore >= 10){
        maxOption = 10; //How many options are drawn already counting reset and back
        printf("\n  Apple  - < \x1b[%dm%s\x1b[0m >", *colorApple, Colors[*colorApple-30]);  //Apple color
        printf("\n  Head   - < \x1b[%dm%s\x1b[0m >", *colorHead, Colors[*colorHead-30]);    //Head color
        printf("\n  Tail   - < \x1b[%dm%s\x1b[0m >", *colorTail, Colors[*colorTail-30]);    //Tail color
    }
    if(highScore >= 30){
        maxOption = 11; //How many options are drawn already counting reset and back
        printf("\n  Shape  - < %s >", Shapes[*ShapeSetting]);   //Shape setting
    }

    //Last 2 options THIS ONES NEVER CHANGE AND NEED TO BE ALWAYS IN THE END
    printf("\n  Reset");        //Last setting
    printf("\n  Back\x1b[u");   //Back to menu
    
    do{
        cursor = menusInput(cursor, maxOption, false, *SoundSetting);   //Get the user input and what option they want
        for(int i = 1; i < maxOption-1; ++i){           //Loops for every option execept last and second-last
            if(cursor == i){        //Checks if the option the user selected is any of the options that exist
                
                /*-------------------------------------UI WITH OPTION SELECTED-----------------------------------------------*/
                printf(" \x1b[32m>");   //Game cursor green and "selected" state
                printf("\x1b[0m%s - ", ExistingSettings[i-1]);       //Text of the option selected
                
                printf("\x1b[32;5m<\x1b[0m "); //Left arrow blinking GREEN
                printf("\x1b[%dm", (i == 6) ? *colorApple : (i == 7) ? *colorHead : (i == 8) ? *colorTail : 0); //get color if needed, and with the correct value
               
                //Writes the current subOption of Selected setting (For example: SOund - it can show "On" or "Off" this is where it writes that text)
                printf("%s", (i == 1) ? ScreenSettingText[*ScreenSetting] : 
                                (i == 2) ? SoundSettingText[*SoundSetting] :
                                    (i == 3) ? SpeedSettingText[*SpeedSetting] : 
                                        (i == 4) ? SizeSettingText[*SizeSetting] :
                                            (i == 5) ? ModeSettingText[*ModeSetting] :
                                                 (i == 6) ? Colors[*colorApple-30] :
                                                    (i == 7) ? Colors[*colorHead-30] :
                                                        (i == 8) ? Colors[*colorTail-30] : 
                                                            Shapes[*ShapeSetting]);
                
                printf(" \x1b[32;5m>\x1b[0m"); //Mostra o segundo indicador de lado (">") com a cor verde e a piscar
                printf("\x1b[u"); //Envia o cursor para a ultima localização guardada, neste caso para o inicio da linha que acabou de desenhar (defenido em menusInput())
                /*------------------------------------------------------------------------------------------------------------------*/

                do{
                    keypress = getch();
                    if(keypress == 27)
                        keypress = '\r';


                    //This gets the sideInput of the user and shows the settings changing (THIS CAN PROBABLY BEEN DONE IN A BETTER WAY, but after 6 months working on this i just gave up)
                    if(i == 1)
                        *ScreenSetting = sideInput(keypress, "Screen", *ScreenSetting, 0, 1, cursor, *SoundSetting, ScreenSettingText, SoundSettingText, SpeedSettingText, SizeSettingText, ModeSettingText, Colors, Shapes);
                    else if(i == 2)
                        *SoundSetting = sideInput(keypress, "Sound ", *SoundSetting, 0, 1, cursor, *SoundSetting, ScreenSettingText, SoundSettingText, SpeedSettingText, SizeSettingText, ModeSettingText, Colors, Shapes);
                    else if(i == 3)
                        *SpeedSetting = sideInput(keypress, "Speed ", *SpeedSetting, 0, 3, cursor, *SoundSetting, ScreenSettingText, SoundSettingText, SpeedSettingText, SizeSettingText, ModeSettingText, Colors, Shapes);
                    else if(i == 4)
                        *SizeSetting = sideInput(keypress, "Size  ", *SizeSetting, 0, 2, cursor, *SoundSetting, ScreenSettingText, SoundSettingText, SpeedSettingText, SizeSettingText, ModeSettingText, Colors, Shapes);
                    else if(i == 5)
                        *ModeSetting = sideInput(keypress, "Mode  ", *ModeSetting, 0, 3, cursor, *SoundSetting, ScreenSettingText, SoundSettingText, SpeedSettingText, SizeSettingText, ModeSettingText, Colors, Shapes);
                    else if(i == 6)
                        *colorApple = sideInput(keypress, "Apple ", *colorApple, 30, 37, cursor, *SoundSetting, ScreenSettingText, SoundSettingText, SpeedSettingText, SizeSettingText, ModeSettingText, Colors, Shapes); 
                    else if(i == 7)
                        *colorHead = sideInput(keypress, "Head  ", *colorHead, 30, 37, cursor, *SoundSetting, ScreenSettingText, SoundSettingText, SpeedSettingText, SizeSettingText, ModeSettingText, Colors, Shapes);
                    else if(i == 8)
                        *colorTail = sideInput(keypress, "Tail  ", *colorTail, 30, 37, cursor, *SoundSetting, ScreenSettingText, SoundSettingText, SpeedSettingText, SizeSettingText, ModeSettingText, Colors, Shapes);
                    else if(i == 9)
                        *ShapeSetting = sideInput(keypress, "Shape ", *ShapeSetting, 0, 4, cursor, *SoundSetting, ScreenSettingText, SoundSettingText, SpeedSettingText, SizeSettingText, ModeSettingText, Colors, Shapes);
                
                }while(keypress != '\r');
                printf("\x1b[u");

                //If to change the screen mode
                if(i == 1){
                    if(AlreadyFullScreen){
                        if(*ScreenSetting == false){
                            fullscreen();
                            AlreadyFullScreen = false;
                        }
                    }else{
                        if(*ScreenSetting){
                            fullscreen();
                            AlreadyFullScreen = true;
                        }
                    }
                }
            }
        }

        //RESET OPTION
        if(cursor == maxOption-1){
            printf(" \x1b[32m>\x1b[0m%s - Are you Sure? (Press [ESC] to cancel)", "Reset");     //ALERT OF RESET
            do{
                keypress = getch();
                if(keypress == '\r'){               //User pressed enter
                    *TotalReset = true;             //Total reset ON
                    cursor = maxOption;             //Leave Setting Menu
                    keypress = 27;                  //Simulate ESC
                    Beep(rand() % 493 + 261,100);   //Beep to let USer know that something happened
                
                }else if(keypress == 27){           //User pressed ESC
                    printf("\x1b[u> Reset\x1b[K\x1b[u");    //Draw option normally
                    if(*SoundSetting)           //If Sound is on, play it
                        Beep(587, 100);
                }
            }while(keypress != 27);     //Loop looking for key press while user doesnt use ESC
        }
    }while(cursor != maxOption);        //Only leave Setting Menu when the cursor is equal to the last option
}

int pause(int height, int SoundSetting){
    printf("\x1b[?25l");    //Turn off Console Cursor
    
    printf("\x1b[u\x1b[%dBPause\n", height+4);      //Draw At the end of Map "Pause"
    printf("\x1b[s\x1b[5m>\x1b[0m Resume\n  Restart\n  Main Menu\x1b[u");   //Show options
    
    int cursor;

    if(SoundSetting)        //Sound
        Beep(523,100);
    
    cursor = menusInput(1, 3, true, SoundSetting);  //Get User input
    if(cursor == 2)         //If its the second option
        cursor = 4;         //Leaves game, goes back to Menu but quickly resets to game
    
    if(cursor == 3)         //If its the last Option
        cursor = 0;         //Go to the menu
    
    printf("\x1b[H\x1b[%dB\n", height+3); //Back to top left then down to pause
    printf("\x1b[J\x1b[H\x1b[s"); //clear next lines go back to the top, save cursor
    return cursor;
}
/*------------------------------------------------------*/


/*--------------------Draw Map/Apple/Tail and Clean---------------------------*/
int drawReset(int i, int y, int Location[2][2], int colorApple, int colorHead, char **Shapes, int ShapeSetting){
    printf("\x1b[?25l");       //Turn off Console Cursor

    if(Location[Apple][column] == i && Location[Apple][row] == y){  //If the current line and column is equal to Apple Location
        printf("\x1b[%dm[]", colorApple);   //Draw apple with color
    }else if(Location[Player][column] == i && Location[Player][row] == y){ //If the current line and column is equal to Player Location
        printf("\x1b[%dm%s", colorHead, Shapes[ShapeSetting]);      //Draw player with color and shape
    }else{  //Rest
        printf("  ");   //Draw Nothing basically
    }
}

int drawApple(int Location[2][2], int TailLocation[MaxTail][2], int BlocksLocation[MaxBlocks][2], int tails, int numBlocks, int colorApple, int width, int height, int *SpawnTries){
    if(*SpawnTries == MaxTries){return 0;}      //If function fails multiple tries it will never draw the apple
    printf("\x1b[?25l");     //Turn off Console cursor
    
    //Random location of the apple
    Location[Apple][column] = rand() % width;
    Location[Apple][row] = rand() % height;

    //Ifs to detect overlap between player, tail and blocks
    if(Location[Apple][row] == Location[Player][row] && Location[Apple][column] == Location[Player][column]){
        drawApple(Location, TailLocation, BlocksLocation, tails, numBlocks, colorApple, width, height, SpawnTries);
        *SpawnTries += 1;
        return 0;
    }

    for(int i = 0; i <= tails; i++){
        if(Location[Apple][row] == TailLocation[i][row] && Location[Apple][column] == TailLocation[i][column]){
            drawApple(Location, TailLocation, BlocksLocation, tails, numBlocks, colorApple, width, height, SpawnTries);
            *SpawnTries += 1;
            return 0;
        }
    }

    for(int i = 0; i <= numBlocks; i++){
        if(Location[Apple][row] == BlocksLocation[i][row] && Location[Apple][column] == BlocksLocation[i][column]){
            drawApple(Location, TailLocation, BlocksLocation, tails, numBlocks, colorApple, width, height, SpawnTries);
            *SpawnTries += 1;
            return 0;
        }
    }

    //If it passed the tests clear colors and draw the apple
    printf("\x1b[0m");
    printf("\x1b[u\x1b[%dB\x1b[%dC", Location[Apple][row]+2, (Location[Apple][column]*2)+1);
    printf("\x1b[%dm[]\x1b[0m", colorApple);
    printf("\x1b[u");
    *SpawnTries = 0;    //Tries back to 0
}

int drawTrail(int Location[2][2], int TailLocation[MaxTail][2], int tails, int colorTail, char **Shapes, int ShapeSetting){
    printf("\x1b[?25l");    //Turn off Console cursor
    
    //Set Positon of the tail to the previous in the array
    for(int i = tails; i > 0; --i){
        TailLocation[i][row] = TailLocation[i-1][row];
        TailLocation[i][column] = TailLocation[i-1][column];
    }
    TailLocation[0][row] = Location[Player][row];       //Set first tail location the same as the player
    TailLocation[0][column] = Location[Player][column];

    //Draw the tails on the console
    for(int i = 0; i < tails; i++){
        printf("\x1b[u\x1b[%dB\x1b[%dC", TailLocation[i][row]+2, (TailLocation[i][column]*2)+1);    //Go to the location of the tail
        printf("\x1b[%dm%s\x1b[0m", colorTail, Shapes[ShapeSetting]); //Draw block of tail
        
        //If its the second-last tail
        if(i == tails-1){
            printf("\x1b[u\x1b[%dB\x1b[%dC", TailLocation[i][row]+2, (TailLocation[i][column]*2)+1);
            printf("\x1b[%d;2m%s\x1b[0m", colorTail, Shapes[ShapeSetting]); //Draw the tail with a darker color
            
            //If different than the Player location
            if(TailLocation[tails][row] != Location[Player][row] || TailLocation[tails][column] != Location[Player][column]) 
                printf("\x1b[u\x1b[%dB\x1b[%dC  ", TailLocation[tails][row]+2, (TailLocation[tails][column]*2)+1);  //Clear all blocks after the last tail

            //Check all tails and ignore it
            for(int y = 0; y < tails; y++){
                if(TailLocation[tails][row] == TailLocation[y][row] && TailLocation[tails][column] == TailLocation[y][column]){
                    printf("\x1b[u\x1b[%dB\x1b[%dC", TailLocation[tails][row]+2, (TailLocation[tails][column]*2)+1);
                    printf("\x1b[%dm%s\x1b[0m", colorTail, Shapes[ShapeSetting]); //draw replacing tail
                }
            }
        }
        printf("\x1b[u");   //Restore position
    }

}

int drawPlayer(int colorHead, char **Shapes, int ShapeSetting){
    printf("\x1b[%dm", colorHead);          //Head Color
    printf("%s", Shapes[ShapeSetting]);     //Stile of the head
    printf("\x1b[0m");                      //Delete colors
}

int drawBlocks(int Location[2][2], int TailLocation[MaxTail][2], int BlocksLocation[MaxBlocks][2], int tails, int numBlocks, int width, int height, int *SpawnTries){
    if(*SpawnTries == MaxTries){return 0;}      //If function fails multiple tries it will never draw the blocks
    printf("\x1b[?25l");     //Turn off Console cursor

    //Random location of the new block
    BlocksLocation[numBlocks][column] = rand() % width;
    BlocksLocation[numBlocks][row] = rand() % height;

    //Ifs to detect overlap between player, tail, apple and other blocks
    if(BlocksLocation[numBlocks][row] == Location[Player][row] && BlocksLocation[numBlocks][column] == Location[Player][column]){
        drawBlocks(Location, TailLocation, BlocksLocation, tails, numBlocks, width, height, SpawnTries); 
        *SpawnTries += 1;
        return 0;
    }

    if(BlocksLocation[numBlocks][row] == Location[Apple][row] && BlocksLocation[numBlocks][column] == Location[Apple][column]){
        drawBlocks(Location, TailLocation, BlocksLocation, tails, numBlocks, width, height, SpawnTries); 
        *SpawnTries += 1;
        return 0;
    }

    for(int i = 0; i <= tails; i++){
        if(BlocksLocation[numBlocks][row] == TailLocation[i][row] && BlocksLocation[numBlocks][row] == TailLocation[i][column]){
            drawBlocks(Location, TailLocation, BlocksLocation, tails, numBlocks, width, height, SpawnTries); 
            *SpawnTries += 1;
            return 0;
        }
    }

    for(int i = 0; i < numBlocks; i++){
        if(BlocksLocation[numBlocks][row] == BlocksLocation[i][row] && BlocksLocation[numBlocks][column] == BlocksLocation[i][column]){
            drawBlocks(Location, TailLocation, BlocksLocation, tails, numBlocks, width, height, SpawnTries); 
            *SpawnTries += 1;
            return 0;
        }
    }
    
    //If it passed the tests, clear colors and draw the block
    printf("\x1b[0m");
    printf("\x1b[u\x1b[%dB\x1b[%dC\x1b[37m||\x1b[0m", BlocksLocation[numBlocks][row]+2, (BlocksLocation[numBlocks][column]*2)+1);
    printf("\x1b[u");   //Reset cursor Position
    *SpawnTries = 0;    //Tries back to 0
}

int cleanPlayer(int Location[2][2]){
    printf("\x1b[u");                                           //Reset Cursor 
    printf("\x1b[%dB", Location[Player][row]+2);                //Go to line/row
    printf("\x1b[%dC  ", (Location[Player][column]*2)+1);       //Go to column and clean
}
/*---------------------------------------------------------------------*/


/*-------------------Math/Death Screen and Music/Reset of Game-------------------------*/
int checkSettings(int *velocity, int *width, int *height, bool *SpeedChange, bool *walls, bool *blocks, bool *zen, int *SpeedSetting, int *SizeSetting, int *ModeSetting){
    printf("\x1b[?25l");
    
    //Check speedSetting
    switch(*SpeedSetting){
        case 0:
            *velocity = DefaultSpeed+100;
            *SpeedChange = false;
        break;
        case 1:
            *velocity = DefaultSpeed;
            *SpeedChange = false;
        break;
        case 2:
            *velocity = DefaultSpeed-50;
            *SpeedChange = false;
        break;
        case 3:
            *velocity = DefaultSpeed;
            *SpeedChange = true;        //Change speed is used to change the speed evertime you get a point
    }
    
    //Check SizeSetting
    switch(*SizeSetting){
        case 0:
            *width = DefaultWidth/2;
            *height = DefaultHeight/2;
        break;
        case 1:
            *width = DefaultWidth;
            *height = DefaultHeight;
        break;
        case 2:
            *width = (DefaultWidth*2)-4;
            *height = (DefaultHeight*2)-8;
        break;
    }
    
    //Check ModeSetting
    *zen = false;
    *blocks = false;
    switch(*ModeSetting){
        case 0:
            *walls = true;  //Set walls true, player cant move bewteen sides
        break;
        case 1:
            *walls = false; //Set walls true, player can move bewteen sides
        break;
        case 2:
            *walls = true;  //Set walls true, player cant move bewteen sides
            *blocks = true; //Set Blocks mode true, creating blocks in random places
        break;
        case 3:
            *walls = false; //Set walls true, player cant move bewteen sides
            *zen = true;    //gamemode creative, player wont die
        break;
    }
}

int reset(int Location[2][2], int TailLocation[MaxTail][2], int BlocksLocation[MaxBlocks][2],  int *points, int *tails, int *numBlocks, int *direction, int *SpawnTries, int *velocity, int *width, int *height, int *highScore, bool *SpeedChange, bool *walls, bool *blocks, bool *zen, int ScreenSetting, int *SpeedSetting, int *SizeSetting, int *ModeSetting, int colorApple, int colorHead, char **Shapes, int ShapeSetting){
    printf("\x1b[?25l");
    //Check settings
    checkSettings(velocity, width, height, SpeedChange, walls, blocks, zen, SpeedSetting, SizeSetting, ModeSetting);
    
    Location[Apple][column] = rand() % *width;              //Reset APPLE location (x), random number from 0 until max Width
    Location[Apple][row] = rand() % *height;                //Reset APPLE location (y), random number from 0 until max Height
    Location[Player][column] = 3;                           //Reset PLAYER location (x), 3rd row
    Location[Player][row] = *height/2;                      //Reset PLAYER location (y), middle row
    *points = 0;                                            //Points reset
    *tails = 2;                                             //Number of tails 2
    *numBlocks = 0;                                         //0 blocks
    SpawnTries = 0;                                         //0 Spawn tries
    *direction = 0;                                         //Direction of snake set to 0 so it doesnt move until player says so
    
    //Tail position reset
    for(int i = 0; i < *tails; ++i){                        
        for(int y = 0; y < 2; ++y){
            TailLocation[i][y] = Location[Player][y];
        }
    }
    //1st Block position reset    
    for(int y = 0; y < 2; ++y){
        BlocksLocation[0][y] = -20;
    }

    printf("\x1b[H\x1b[J\x1b[s");                                //clean screen and go to 0,0, save cursor
    
    //Draw scoreboard, grey if its in zen mode
    printf("\x1b[uPoints: %d \x1b[%dm| High Score: %d \x1b[0m\n", *points, (*zen ? 2 : 37), *highScore);
    
    //Space to center upper wall
    printf(" ");
    //Draw upper wall
    for(int i = 0; i <= *width; ++i){
        printf("\x1b[%dm", (*walls) ? 0 : 32); //If the mode has walls make it white, if not make it green
        printf("--");
    }
    
    //New line
    printf("\n");
    //Number of lines code needs to do
    for(int y = 0; y < *height+1; ++y){
        printf("|");                                                    //Left Wall
        for(int i = 0; i <= *width; ++i){
            drawReset(i, y, Location, colorApple, colorHead, Shapes, ShapeSetting);        //Draw PLAYER, APPLE, and blank spaces
        }
        printf("\x1b[%dm", (*walls) ? 0 : 32);                          //Walls color this is needed because draw reset uses new colors, and that could break how the walls look
        printf("|\n");                                                  //Right Wall
    }
    
    //Space to center lower wall
    printf(" ");
    //Draw lower wall, no need for colors because the last line will awlways define the color needed for the lower wall
    for(int i = 0; i <= *width; ++i){
        printf("--");
    }
    printf("\x1b[0m\x1b[u");                                            //Stop colors and go to last saved cursor position (0,0)
}

int deathMusic(char *message, int height, int width, bool *waitMusic, int SoundSetting){
    printf("\x1b[?25l");

    printf("\x1b[u\x1b[%dB\x1b[%dC", height/2+2, width-(strlen(message)/2)+2);      //Move cursor to middle of map
    printf("%s\n", message);    //Write message 
    *waitMusic = true;          //Stops the code from doing anything while the music plays
    
    //play death music with random notes
    if(SoundSetting){
        Beep(rand() % 391 + 381,200);
        Beep(rand() % 329 + 319,200);
        Beep(rand() % 293 + 283,200);
        Beep(rand() % 261 + 251,200);
    }

    *waitMusic = false;     //Code can work again
}

int defeatScreen(char *message, int height, int width, int SoundSetting){
    printf("\x1b[?25l");
    int cursor;
    bool waitMusic = false;     //Wait for music in death music
    
    deathMusic(message, height, width, &waitMusic, SoundSetting);       //Call death music beep boop beep
    if(!waitMusic){
        printf("\x1b[u\x1b[%dBTry again?\n", height+4);                         //Cursor after the map of the game
        printf("\x1b[s\x1b[5m>\x1b[0m Try Again\n  Main Menu\n  Quit\x1b[u");   //Write funny options

        cursor = menusInput(1, 3, false, SoundSetting);                 //User input for menu

        if(cursor == 2)                  //Go to menu if its the second option
            cursor = 0;
    }

    return cursor;
}

int conditions(int Location[2][2], int TailLocation[MaxTail][2], int BlocksLocation[MaxBlocks][2], int *points, int *highScore, int *tails, int *numBlocks, int *gameState, int *direction, int *SpawnTries, int *velocity, int *width, int *height, int ScreenSetting, int *SpeedSetting, int *SizeSetting, int *ModeSetting, int colorApple, int colorHead, bool *walls, bool *blocks, bool *zen, bool *SpeedChange, char **Shapes, int ShapeSetting, int SoundSetting){
    printf("\x1b[?25l");

    // IF the player is in the same location as the walls/outside map
    if(Location[Player][row] == *height+1 || Location[Player][row] == -1 || Location[Player][column] == -1 || Location[Player][column] == *width+1){
        //Are the walls "real"
        if(*walls){ 
            printf("\x1b[u\x1b[%dB\x1b[%dC", Location[Player][row]+2, (Location[Player][column]*2)+1);      //Move cursor to player location
            //Draw the walls again in red
            printf("\x1b[31m%s\x1b[0m", (Location[Player][row] == *height+1 || Location[Player][row] == -1) ? "--" :
                                    (Location[Player][column] == *width+1) ? "| " : "|");   //This conditions are needed to know what side it is and the right needs an extra space to clean the whole player
            
            *gameState = defeatScreen("Game  Over", *height, *width, SoundSetting);        //Call the death screen and death music
            
            if(*gameState == 1)  //If player chooses the first option game starts again
                reset(Location, TailLocation, BlocksLocation, points, tails, numBlocks, direction, SpawnTries, velocity, width, height, highScore, SpeedChange, walls, blocks, zen, ScreenSetting, SpeedSetting, SizeSetting, ModeSetting, colorApple, colorHead, Shapes, ShapeSetting);
        }else{
            printf("\x1b[u\x1b[%dB\x1b[%dC", Location[Player][row]+2, (Location[Player][column]*2)+1);      //Localização do player
            //Draw the walls in green
            printf("\x1b[32m%s\x1b[0m", (Location[Player][row] == *height+1 || Location[Player][row] == -1) ? "--" :
                                    (Location[Player][column] == *width+1) ? "| " : "|");   //This conditions are needed to know what side it is and the right needs an extra space to clean the whole player

            /**
             *Invert player position
            **/
            if(Location[Player][row] == *height+1){
                Location[Player][row] = 0;
            }
            if(Location[Player][row] == -1){
                Location[Player][row] = *height;
            }
            if(Location[Player][column] == *width+1){
                Location[Player][column] = 0;
            }
            if(Location[Player][column] == -1){
                Location[Player][column] = *width;
            }

            printf("\x1b[u\x1b[%dB\x1b[%dC", Location[Player][row]+2, (Location[Player][column]*2)+1);  //Locate player
            printf("\x1b[%dm%s", colorHead, Shapes[ShapeSetting]);                                     //Draw new head
            printf("\x1b[0m");                                                                          //Stop all costumization
            
            //Call conditions again in case there is a tail or an apple in that new location
            conditions(Location, TailLocation, BlocksLocation, points, highScore, tails, numBlocks, gameState, direction, SpawnTries, velocity, width, height, ScreenSetting, SpeedSetting, SizeSetting, ModeSetting, colorApple, colorHead, walls, blocks, zen, SpeedChange, Shapes, ShapeSetting, SoundSetting);
            return 0; //This breaks the whole function so it doesnt repeat itself
        }
    // IF the player is in the same location as an apple  
    }else if(Location[Player][column] == Location[Apple][column] && Location[Player][row] == Location[Apple][row]){
        //Draw new apple and get new location
        drawApple(Location, TailLocation, BlocksLocation, *tails, *numBlocks, colorApple, *width, *height, SpawnTries);
    
        *points += 1;                                                       //Add points

        if(SoundSetting)                                                    //Play sound effect
            Beep(rand() % 327 + 314,100);

        if(*highScore < *points && !*zen)                                   //If highscore lower than the points make it the same
            *highScore = *points;                                           //Also dont do it if its zen mode to prevent big points when you are basically in creative

        *tails += 1;                                                        //TAIL +1

        //If blocks mode is on make a new block
        if(*blocks){
            drawBlocks(Location, TailLocation, BlocksLocation, *tails, *numBlocks, *width, *height, SpawnTries);
            *numBlocks += 1;
        }

        if(*SpeedChange == true && *velocity > 20)                          //If speed mode is on change the snake speed
            *velocity -= 5;                                                 //"Faster" speed is actually faster timeOut

        //Change scoreboard / if in Zen make Highscore grey
        printf("\x1b[uPoints: %d \x1b[%dm| High Score: %d \x1b[0m", *points, (*zen ? 2 : 0), *highScore); 
        printf("\x1b[u");
    }

    //Blocks mode
    if(*blocks){
        for(int i = 0; i < *numBlocks; i++){
            if(Location[Player][column] == BlocksLocation[i][column] && Location[Player][row] == BlocksLocation[i][row]){
                /*----Design-----*/
                printf("\x1b[u\x1b[%dB\x1b[%dC", Location[Player][row]+2, (Location[Player][column]*2)+1);  //Locate the funny snake
                printf("\x1b[31m%s\x1b[0m", Shapes[ShapeSetting]);                        //Draw head
                printf("\x1b[u");
                /*---------------*/
                *gameState = defeatScreen("Game  Over", *height, *width, SoundSetting);                    //You lost screen call
                
                if(*gameState == 1)  //Reset game if user wants to
                    reset(Location, TailLocation, BlocksLocation, points, tails, numBlocks, direction, SpawnTries, velocity, width, height, highScore, SpeedChange, walls, blocks, zen, ScreenSetting, SpeedSetting, SizeSetting, ModeSetting, colorApple, colorHead, Shapes, ShapeSetting);
            }
        }
    }

    //Win condition
    //This part is hella buggy needs a big fix
    if(*tails == ((*width) * (*height)+30) || *SpawnTries == MaxTries){
        *gameState = defeatScreen("You  Win", *height, *width, SoundSetting);   //Death screen
                    
        if(*gameState == 1)  //Reset if player wants
            reset(Location, TailLocation, BlocksLocation, points, tails, numBlocks, direction, SpawnTries , velocity, width, height, highScore, SpeedChange, walls, blocks, zen, ScreenSetting, SpeedSetting, SizeSetting, ModeSetting, colorApple, colorHead, Shapes, ShapeSetting);
    }

    //This is for same tail position
    //If creative mode / Zen mode
    if(*zen){
        //Just draw the head thats it
        /*----Design-----*/
        printf("\x1b[u\x1b[%dB\x1b[%dC", Location[Player][row]+2, (Location[Player][column]*2)+1);
        printf("\x1b[%dm%s\x1b[0m\x1b[u", colorHead, Shapes[ShapeSetting]);         
        /*---------------*/
    }else{
        //Loop all the tails
        for(int i = 0; i < *tails; i++){
            //Check position and number of tails (It needs to be higher than 2 so player doesnt die on the start)
            if(*tails > 2 && Location[Player][row] == TailLocation[i][row] && Location[Player][column] == TailLocation[i][column]){
                /*----Design-----*/
                printf("\x1b[u\x1b[%dB\x1b[%dC", Location[Player][row]+2, (Location[Player][column]*2)+1);  //Locate player
                printf("\x1b[31m%s\x1b[0m\x1b[u", Shapes[ShapeSetting]);                        //Draw head
                /*---------------*/
                
                *gameState = defeatScreen("Game  Over", *height, *width, SoundSetting);                    //Death screen and ask what user wants to do next
                    
                if(*gameState == 1)  //Start again if needed
                    reset(Location, TailLocation, BlocksLocation, points, tails, numBlocks, direction, SpawnTries, velocity, width, height, highScore, SpeedChange, walls, blocks, zen, ScreenSetting, SpeedSetting, SizeSetting, ModeSetting, colorApple, colorHead, Shapes, ShapeSetting);
            }
        }
        //Draw head of player if its in the same position has the clearing tail (the one that clears all the lost blocks/ last tail/ invisible tail)
        if(Location[Player][row] == TailLocation[*tails][row] && Location[Player][column] == TailLocation[*tails][column]){
            printf("\x1b[u\x1b[%dB\x1b[%dC", Location[Player][row]+2, (Location[Player][column]*2)+1);  //Locate player
            printf("\x1b[%dm%s\x1b[0m\x1b[u", colorHead, Shapes[ShapeSetting]);                        //Draw head
        }
    }
    
}
/*-------------------------------------------------------------------------------------*/

/*--------------------Inputs for the game----------------------*/
int playerInput(int *direction, int height, int *gameState, bool *EnableInput, int SoundSetting){
    printf("\x1b[?25l");
    int keypress;

    //Waits for a key input
    if (kbhit()){
        keypress = getch();
        switch(keypress){
            case 65: //A key (Caps)
            case 97: //A key
            case 75: //Arrow Key - Left
                if(*direction == RIGHT){    //Cancel input if player going in the opposite direction
                    break;
                }  
                *direction = LEFT;          //Set new direction
                *EnableInput = false;       //Input enable again
                break;
            case 87: //W key (Caps)
            case 119: //W key
            case 72: //Arrow Key - UP
                if(*direction == DOWN){
                    break;
                }  
                *direction = UP;
                *EnableInput = false;
                break;
            case 68: //D key (Caps)
            case 100://D key
            case 77: //Arrow Key - RIGHT
                if(*direction == LEFT){
                    break;
                }  
                *direction = RIGHT;
                *EnableInput = false;
                break;
            case 83: //S key (Caps)
            case 115://S key
            case 80: //Arrow Key - DOWN
                if(*direction == UP){
                    break;
                }  
                *direction = DOWN;
                *EnableInput = false;
                break;
            case 27:    //ESC
                *gameState = pause(height, SoundSetting);        //Pause screen
        }
    }
}

void setTimeout(int milliseconds, int *direction, int height, int *gameState, int SoundSetting){
    printf("\x1b[?25l");
    bool EnableInput;
    
    //Input false turn it on again please thank you
    if(!EnableInput)
        EnableInput = true;


    //HeHe stolen function from stackoverflow
    // If milliseconds is less or equal to 0
    // will be simple return from function without throw error
    if (milliseconds <= 0) {
        fprintf(stderr, "Count milliseconds for timeout is less or equal to 0\n");
        return;
    }

    // a current time of milliseconds
    int milliseconds_since = clock() * 1000 / CLOCKS_PER_SEC;

    // needed count milliseconds of return from this timeout
    int end = milliseconds_since + milliseconds;

    // wait while until needed time comes
    do {
        milliseconds_since = clock() * 1000 / CLOCKS_PER_SEC;
        
        //Part i actually did in here
        if(EnableInput) //If input is enable call the playerInput so he can use the funny snake
            playerInput(direction, height, gameState, &EnableInput, SoundSetting);
    } while (milliseconds_since <= end);
}

int snakeMovement(int Location[2][2], int TailLocation[MaxTail][2], int tails, int *direction, int velocity, int height, int *gameState, int colorHead, int colorTail, char **Shapes, int ShapeSetting, int SoundSetting){
    printf("\x1b[?25l");
    setTimeout(velocity, direction, height, gameState, SoundSetting);    //Call timer for funny snake

    if(*gameState == 0)  //If player used pause and pressed the second option it needs to quit le funny game fast
        return 0;   //Ignores rest of the code

    //if the snake is moving clean the block
    if(*direction != 0)
        cleanPlayer(Location);                  //Moves cursor to player location and writes "  "
    
    switch(*direction){
        case 1: //Left
            printf("\x1b[%dD", 4);              //go 4 columns to the left

            drawPlayer(colorHead, Shapes, ShapeSetting);                                //Draw the player
            drawTrail(Location, TailLocation, tails, colorTail, Shapes, ShapeSetting);  //Draw tails

            Location[Player][column] -= 1;      //Change code column location
            break;
        case 2: //Up
                printf("\x1b[%dD", 2);          //Go 2 columns to the left
                printf("\x1b[%dA", 1);          //And 1 line up

                drawPlayer(colorHead, Shapes, ShapeSetting);                                //Draw the player
                drawTrail(Location, TailLocation, tails, colorTail, Shapes, ShapeSetting);  //Draw tails

                Location[Player][row] -= 1;         //Change code row location
            break;
        case 3: //Right
                //No need to move cursor
               
                drawPlayer(colorHead, Shapes, ShapeSetting);                                //Draw the player
                drawTrail(Location, TailLocation, tails, colorTail, Shapes, ShapeSetting);  //Draw tails
                
                Location[Player][column] += 1;      //Change code column location
            break;
        case 4: //Down
                printf("\x1b[%dD", 2);              //Go 2 columns to the left
                printf("\x1b[%dB", 1);              //And 1 line down
                
                drawPlayer(colorHead, Shapes, ShapeSetting);                                //Draw the player
                drawTrail(Location, TailLocation, tails, colorTail, Shapes, ShapeSetting);  //Draw tails
                
                Location[Player][row] += 1;         //Change code row location
            break;
    }
    printf("\x1b[u");
}
/*-----------------------------------------------------*/

/*-------------------------------------------------MEMORY MANGEMENT-------------------------------------------------*/
int cleanMemory(bool clearHighScore, int *highScore, int *ScreenSetting, int *SoundSetting, int *SpeedSetting, int *SizeSetting, int *ModeSetting, int *colorApple, int *colorHead, int *colorTail, int *ShapeSetting, bool *TotalReset){
    /**
     * Total reset of the game
    **/
    
    printf("\x1b[?25l");
    if(clearHighScore)
        *highScore = 0;
    *ScreenSetting = 1;
    *SoundSetting = 1;
    *SpeedSetting = 1;
    *SizeSetting = 1;
    *ModeSetting = 0;
    *colorApple = 31;
    *colorHead = 33;
    *colorTail = 32;
    *ShapeSetting = 0;
    *TotalReset = false;
}

int readMemory(FILE * fPtrRead, int gameVersion, int patchVersion, int *checkVersion, int *checkPatch, int *highScore, int *ScreenSetting, int *SoundSetting, int *SpeedSetting, int *SizeSetting, int *ModeSetting, int *colorApple, int *colorHead, int *colorTail, int *ShapeSetting, bool *TotalRset){  
    /**
     * Read save file
    **/

    fscanf(fPtrRead, "Version - %d\n", checkVersion);
    fscanf(fPtrRead, "Patch - %d\n", checkPatch);
    if(*checkVersion < gameVersion || *checkPatch < patchVersion){  //Check if versions of the save file are below the game
        fscanf(fPtrRead, "HighScore - %d\n", highScore);            //Get highscore
        //Clear save file, but keep the highscore
        cleanMemory(false, highScore, ScreenSetting, SoundSetting, SpeedSetting, SizeSetting, ModeSetting, colorApple, colorHead, colorTail, ShapeSetting, TotalRset);
    }else{
        fscanf(fPtrRead, "HighScore - %d\n", highScore);
        fscanf(fPtrRead, "Screen - %d\n", ScreenSetting);
        fscanf(fPtrRead, "Sound - %d\n", SoundSetting);
        fscanf(fPtrRead, "Speed - %d\n", SpeedSetting);
        fscanf(fPtrRead, "Size - %d\n", SizeSetting);
        fscanf(fPtrRead, "Mode - %d\n", ModeSetting);
        fscanf(fPtrRead, "Apple - %d\n", colorApple);
        fscanf(fPtrRead, "Head - %d\n", colorHead);
        fscanf(fPtrRead, "Tail - %d\n", colorTail);
        fscanf(fPtrRead, "Shape - %d\n", ShapeSetting);
    }

    if(fPtrRead == NULL){       //If it cant read anything
        system("md %LOCALAPPDATA%\\SBSnakeFiles\\");        //Tries to create game folder
        //Clear save file
        cleanMemory(true, highScore, ScreenSetting, SoundSetting, SpeedSetting, SizeSetting, ModeSetting, colorApple, colorHead, colorTail, ShapeSetting, TotalRset);
    }

    fclose(fPtrRead);       //Close read
}

int writeMemory(FILE * fPtrWrite, int gameVersion, int patchVersion, int highScore, int ScreenSetting, int SoundSetting, int SpeedSetting, int SizeSetting, int ModeSetting, int colorApple, int colorHead, int colorTail, int ShapeSetting){   
    /**
     * Write to save file
    **/

    fprintf(fPtrWrite, "Version - %d\n", gameVersion);
    fprintf(fPtrWrite, "Patch - %d\n", patchVersion);
    fprintf(fPtrWrite, "HighScore - %d\n", highScore);
    fprintf(fPtrWrite, "Screen - %d\n", ScreenSetting);
    fprintf(fPtrWrite, "Sound - %d\n", SoundSetting);
    fprintf(fPtrWrite, "Speed - %d\n", SpeedSetting);
    fprintf(fPtrWrite, "Size - %d\n", SizeSetting);
    fprintf(fPtrWrite, "Mode - %d\n", ModeSetting);
    fprintf(fPtrWrite, "Apple - %d\n", colorApple);
    fprintf(fPtrWrite, "Head - %d\n", colorHead);
    fprintf(fPtrWrite, "Tail - %d\n", colorTail);
    fprintf(fPtrWrite, "Shape - %d\n", ShapeSetting);

    fclose(fPtrWrite);      //Close write
}
/*------------------------------------------------------------------------------------------------------------------*/

/*--------------------Main Code------------------*/
int main(){
    printf("\x1b[?25l");        //Turn off console cursor

    srand((unsigned) time(0));  //Time

    FILE * fPtrWrite, * fPtrRead;   //File vars
    
    //This sets the current game version and game patch, if the file version is below this one it will save the highscore and clean the rest
    int gameVersion = 1;
    int patchVersion = 0;
    
    int checkVersion, checkPatch;

    //All vars of the game (game states, points, direction, etc.)
    int gameState, width, height, velocity, points, highScore, direction, SpawnTries, tails, numBlocks, ScreenSetting, SoundSetting, SpeedSetting, SizeSetting, ModeSetting, colorApple, colorHead, colorTail, ShapeSetting;
    //WOAH BOOLS
    bool walls = true, blocks = false, zen = false, SpeedChange = false, TotalReset = false;
    //Arrays
    int Location[2][2], TailLocation[MaxTail][2], BlocksLocation[MaxBlocks][2];
    char *Shapes[6] = {"[]", "()", "{}", "<>", "::", "AA"};

    /*--------------Create path for the save file-------------*/
    char *appdata = getenv("localappData");
    if (!appdata) { /* error */ }
    char buffer[0x400];
    snprintf(buffer, sizeof(buffer), "%s/SBSnakeFiles/snake.sav", appdata);
    /*---------------------------------------------------------------------------------*/

    /*---------------Read save file--------------*/
    fPtrRead = fopen(buffer, "r");
    readMemory(fPtrRead, gameVersion, patchVersion, &checkVersion, &checkPatch, &highScore, &ScreenSetting, &SoundSetting, &SpeedSetting, &SizeSetting, &ModeSetting, &colorApple, &colorHead, &colorTail, &ShapeSetting, &TotalReset);
    /*--------------------------------------------*/
    

    maximizeScreen();           //Maximize screen

    if(ScreenSetting)
        fullscreen();               //Set fullscreen if needed

    /*--------------------------------Game-------------------------------*/
    do{
        /*
            Very hard to understand loop because i did it in a bad way
            Basically it reads a single var called gameState and thats what defines if it should be in the menu, settings, playing or leave the game
            0 is menu
            1 is playing
            2 is settings
            3 is leave
            4 is restart game and play again
        */
        if(gameState != 4)
            gameState = mainMenu(colorHead, colorTail, gameVersion, patchVersion, SoundSetting);
        if(gameState == 1 || gameState == 4){
            /*
                Playing game
            */
            reset(Location, TailLocation, BlocksLocation, &points, &tails, &numBlocks, &direction, &SpawnTries, &velocity, &width, &height, &highScore, &SpeedChange, &walls, &blocks, &zen, ScreenSetting, &SpeedSetting, &SizeSetting, &ModeSetting, colorApple, colorHead, Shapes, ShapeSetting);
            gameState = 1;
            do{
                snakeMovement(Location, TailLocation, tails, &direction, velocity, height, &gameState, colorHead, colorTail, Shapes, ShapeSetting, SoundSetting);
                conditions(Location, TailLocation, BlocksLocation, &points, &highScore, &tails, &numBlocks, &gameState, &direction, &SpawnTries, &velocity, &width, &height, ScreenSetting, &SpeedSetting, &SizeSetting, &ModeSetting, colorApple, colorHead, &walls, &blocks, &zen, &SpeedChange, Shapes, ShapeSetting, SoundSetting);
            }while(gameState == 1);
        
        }else if(gameState == 2){
            /*
                Settings
            */
            settings(&velocity, &width, &height, &TotalReset, &ScreenSetting, &SoundSetting, &SpeedSetting, &SizeSetting, &ModeSetting, &colorApple, &colorHead, &colorTail, Shapes, &ShapeSetting, highScore);
            
            //Reset savefile and game if needed
            if(TotalReset){ 
                if(ScreenSetting ==  false)
                    fullscreen();
                cleanMemory(true, &highScore, &ScreenSetting, &SoundSetting, &SpeedSetting, &SizeSetting, &ModeSetting, &colorApple, &colorHead, &colorTail, &ShapeSetting, &TotalReset);
            }
        }
    }while(gameState != 3);
    /*-------------------------------------------------------------------*/

    /*--------------------------Save game--------------------------*/
    fPtrWrite = fopen(buffer, "w");
    writeMemory(fPtrWrite, gameVersion, patchVersion, highScore, ScreenSetting, SoundSetting, SpeedSetting, SizeSetting, ModeSetting, colorApple, colorHead, colorTail, ShapeSetting);
    /*-------------------------------------------------------------------------*/
    
    //Turn on cursor
    printf("\x1b[?25h");
    //Clean screen
    printf("\x1b[H\x1b[J\x1b[s");
    
    //Leave fullscreen
    if(ScreenSetting)
        fullscreen();
}
/*-----------------------------------------------------*/