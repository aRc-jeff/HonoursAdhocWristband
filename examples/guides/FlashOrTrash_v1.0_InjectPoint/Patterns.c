#include "Patterns.h"
#include "timex.h"
#include "ztimer.h"
#include "Effects.h"

#define SCREEN_WIDTH 4
#define SCREEN_HEIGHT 5

static void delay(int ms)
{
    ztimer_sleep(ZTIMER_USEC, ms * 1000);
}

void setColour(uint8_t RED, uint8_t GREEN, uint8_t BLUE){
    cmd_t message = cmd_create(SET_COLOUR, RED, GREEN, BLUE, 1, 1, SCREEN_WIDTH, SCREEN_HEIGHT, false);
    sendMessage(message);
}

void sweep(uint8_t RED, uint8_t GREEN, uint8_t BLUE){
    cmd_t message;
    for (int y = 1; y <= 5; y++){
        for (int x = 1; x <= 4; x++){
            message = cmd_create(SET_COLOUR, RED, GREEN, BLUE, x, y, x, y, false);
            sendMessage(message);
        }
    }

    for (int y = 1; y <= 5; y++){
        for (int x = 1; x <= 4; x++){
            message = cmd_create(SET_COLOUR, 0, 0, 0, x, y, x, y, false);
            sendMessage(message);
        }
    }
}

void long_colour_sweep(void){
    cmd_t message;
    for (int r = 0; r < 256; r+=50){
        for (int g = 0; g < 256; g+=50){
            for (int b = 0; b < 256; b+=50){
                for (int y = 1; y <= 5; y++){
                    for (int x = 1; x <= 4; x++){
                    message = cmd_create(SET_COLOUR, r, g, b, x, y, x, y, false);
                    sendMessage(message);
                    }
                }
            }
        }
    }
}

void vertical_lines_alternate(void){
    // vertical lines alternate
    // blank display
    cmd_t message = cmd_create(SET_COLOUR, 0, 0, 0, 1, 1, 4, 5, false);
    sendMessage(message);
    // for repeat 10 times - pick random colour
    for (int i = 0; i < 100; i++) {
        int randRED = rand255();
        int randGREEN = rand255();
        int randBLUE = rand255();
        //col1 - col3 on
        message = cmd_create(SET_COLOUR, randRED, randGREEN, randBLUE, 1, 1, 1, 5, false);
        sendMessage(message);
        message = cmd_create(SET_COLOUR, randRED, randGREEN, randBLUE, 3, 1, 3, 5, false);
        sendMessage(message);
        // wait 100ms
        delay(100);
        //col2 - col4 on
        message = cmd_create(SET_COLOUR, randRED, randGREEN, randBLUE, 2, 1, 2, 5, false);
        sendMessage(message);
        message = cmd_create(SET_COLOUR, randRED, randGREEN, randBLUE, 4, 1, 4, 5, false);
        sendMessage(message);
        // wait 100ms
        delay(100);
    }
}

void horizontal_lines_alternate(void){
    // horizontal lines alternate
    // blank display
    cmd_t message = cmd_create(SET_COLOUR, 0, 0, 0, 1, 1, 4, 5, false);
    sendMessage(message);
    // for repeat 10 times - pick random colour
    for (int i = 0; i < 100; i++) {
        int randRED = rand255();
        int randGREEN = rand255();
        int randBLUE = rand255();
        //row1 - row3 - row5 on
        message = cmd_create(SET_COLOUR, randRED, randGREEN, randBLUE, 1, 1, 4, 1, false);
        sendMessage(message);
        message = cmd_create(SET_COLOUR, randRED, randGREEN, randBLUE, 1, 3, 4, 3, false);
        sendMessage(message);
        message = cmd_create(SET_COLOUR, randRED, randGREEN, randBLUE, 1, 5, 4, 5, false);
        sendMessage(message);
        // wait 100ms
        delay(100);
        //row2 - row4 on
        message = cmd_create(SET_COLOUR, randRED, randGREEN, randBLUE, 1, 2, 4, 2, false);
        sendMessage(message);
        message = cmd_create(SET_COLOUR, randRED, randGREEN, randBLUE, 1, 4, 4, 4, false);
        sendMessage(message);
        // wait 100ms
        delay(100);
    }
}

void vertical_lines_fade_through(void){
    // vertical lines fade through
    // blank display
    cmd_t message = cmd_create(SET_COLOUR, 0, 0, 0, 1, 1, 4, 5, false);
    sendMessage(message);
    // for repeat 5 times - change colour each loop
    for (int i = 0; i < 5; i++) {
        int randRED = rand255();
        int randGREEN = rand255();
        int randBLUE = rand255();
        //col1 fade on 2000
        message = cmd_create(FADE_10S, randRED, randGREEN, randBLUE, 1, 1, 1, 5, true);
        sendMessage(message);
        delay(100);
        //col2 fade on 2000
        message = cmd_create(FADE_10S, randRED, randGREEN, randBLUE, 2, 1, 2, 5, true);
        sendMessage(message);
        delay(100);
        //col3 fade on 2000
        message = cmd_create(FADE_10S, randRED, randGREEN, randBLUE, 3, 1, 3, 5, true);
        sendMessage(message);
        delay(100);
        //col4 fade on 2000
        message = cmd_create(FADE_10S, randRED, randGREEN, randBLUE, 4, 1, 4, 5, true);
        sendMessage(message);
        delay(100);

        //col1 fade off 2000
        message = cmd_create(FADE_10S, 0, 0, 0, 1, 1, 1, 5, true);
        sendMessage(message);
        delay(100);
        //col2 fade off 2000
        message = cmd_create(FADE_10S, 0, 0, 0, 2, 1, 2, 5, true);
        sendMessage(message);
        delay(100);
        //col3 fade off 2000
        message = cmd_create(FADE_10S, 0, 0, 0, 3, 1, 3, 5, true);
        sendMessage(message);
        delay(100);
        //col4 fade off 2000
        message = cmd_create(FADE_10S, 0, 0, 0, 4, 1, 4, 5, true);
        sendMessage(message);
        delay(100);

        randRED = rand255();
        randGREEN = rand255();
        randBLUE = rand255();
        //col1 fade on 1000
        message = cmd_create(FADE_5S, randRED, randGREEN, randBLUE, 1, 1, 1, 5, true);
        sendMessage(message);
        delay(100);
        //col2 fade on 1000
        message = cmd_create(FADE_5S, randRED, randGREEN, randBLUE, 2, 1, 2, 5, true);
        sendMessage(message);
        delay(100);
        //col3 fade on 1000
        message = cmd_create(FADE_5S, randRED, randGREEN, randBLUE, 3, 1, 3, 5, true);
        sendMessage(message);
        delay(100);
        //col4 fade on 1000
        message = cmd_create(FADE_5S, randRED, randGREEN, randBLUE, 4, 1, 4, 5, true);
        sendMessage(message);
        delay(100);

        //col1 fade off 1000
        message = cmd_create(FADE_5S, 0, 0, 0, 1, 1, 1, 5, true);
        sendMessage(message);
        delay(100);
        //col2 fade off 1000
        message = cmd_create(FADE_5S, 0, 0, 0, 2, 1, 2, 5, true);
        sendMessage(message);
        delay(100);
        //col3 fade off 1000
        message = cmd_create(FADE_5S, 0, 0, 0, 3, 1, 3, 5, true);
        sendMessage(message);
        delay(100);
        //col4 fade off 1000
        message = cmd_create(FADE_5S, 0, 0, 0, 4, 1, 4, 5, true);
        sendMessage(message);
        delay(100);

        randRED = rand255();
        randGREEN = rand255();
        randBLUE = rand255();
        //col1 fade on 500
        message = cmd_create(FADE_2S, randRED, randGREEN, randBLUE, 1, 1, 1, 5, true);
        sendMessage(message);
        delay(100);
        //col2 fade on 500
        message = cmd_create(FADE_2S, randRED, randGREEN, randBLUE, 2, 1, 2, 5, true);
        sendMessage(message);
        delay(100);
        //col3 fade on 500
        message = cmd_create(FADE_2S, randRED, randGREEN, randBLUE, 3, 1, 3, 5, true);
        sendMessage(message);
        delay(100);
        //col4 fade on 500
        message = cmd_create(FADE_2S, randRED, randGREEN, randBLUE, 4, 1, 4, 5, true);
        sendMessage(message);
        delay(100);

        //col1 fade off 500
        message = cmd_create(FADE_2S, 0, 0, 0, 1, 1, 1, 5, true);
        sendMessage(message);
        delay(100);
        //col2 fade off 500
        message = cmd_create(FADE_2S, 0, 0, 0, 2, 1, 2, 5, true);
        sendMessage(message);
        delay(100);
        //col3 fade off 500
        message = cmd_create(FADE_2S, 0, 0, 0, 3, 1, 3, 5, true);
        sendMessage(message);
        delay(100);
        //col4 fade off 500
        message = cmd_create(FADE_2S, 0, 0, 0, 4, 1, 4, 5, true);
        sendMessage(message);
        delay(100);
    }
}

void horizontal_lines_fade_through(void){
    // horizontal lines fade through
    // blank display
    cmd_t message = cmd_create(SET_COLOUR, 0, 0, 0, 1, 1, 4, 5, false);
    sendMessage(message);
    // for repeat 5 times - change colour each loop
    for (int i = 0; i < 5; i++) {
        int randRED = rand255();
        int randGREEN = rand255();
        int randBLUE = rand255();
        //row1 fade on 2000
        message = cmd_create(FADE_10S, randRED, randGREEN, randBLUE, 1, 1, 4, 1, true);
        sendMessage(message);
        delay(100);
        //row2 fade on 2000
        message = cmd_create(FADE_10S, randRED, randGREEN, randBLUE, 1, 2, 4, 2, true);
        sendMessage(message);
        delay(100);
        //row3 fade on 2000
        message = cmd_create(FADE_10S, randRED, randGREEN, randBLUE, 1, 3, 4, 3, true);
        sendMessage(message);
        delay(100);
        //row4 fade on 2000
        message = cmd_create(FADE_10S, randRED, randGREEN, randBLUE, 1, 4, 4, 4, true);
        sendMessage(message);
        delay(100);
        //row5 fade on 2000
        message = cmd_create(FADE_10S, randRED, randGREEN, randBLUE, 1, 5, 4, 5, true);
        sendMessage(message);
        delay(100);

        //row1 fade off 2000
        message = cmd_create(FADE_10S, 0, 0, 0, 1, 1, 4, 1, true);
        sendMessage(message);
        delay(100);
        //row2 fade off 2000
        message = cmd_create(FADE_10S, 0, 0, 0, 1, 2, 4, 2, true);
        sendMessage(message);
        delay(100);
        //row3 fade off 2000
        message = cmd_create(FADE_10S, 0, 0, 0, 1, 3, 4, 3, true);
        sendMessage(message);
        delay(100);
        //row4 fade off 2000
        message = cmd_create(FADE_10S, 0, 0, 0, 1, 4, 4, 4, true);
        sendMessage(message);
        delay(100);
        //row5 fade off 2000
        message = cmd_create(FADE_10S, 0, 0, 0, 1, 5, 4, 5, true);
        sendMessage(message);
        delay(100);

        randRED = rand255();
        randGREEN = rand255();
        randBLUE = rand255();
        
        //row1 fade on 1000
        message = cmd_create(FADE_5S, randRED, randGREEN, randBLUE, 1, 1, 4, 1, true);
        sendMessage(message);
        delay(100);
        //row2 fade on 1000
        message = cmd_create(FADE_5S, randRED, randGREEN, randBLUE, 1, 2, 4, 2, true);
        sendMessage(message);
        delay(100);
        //row3 fade on 1000
        message = cmd_create(FADE_5S, randRED, randGREEN, randBLUE, 1, 3, 4, 3, true);
        sendMessage(message);
        delay(100);
        //row4 fade on 1000
        message = cmd_create(FADE_5S, randRED, randGREEN, randBLUE, 1, 4, 4, 4, true);
        sendMessage(message);
        delay(100);
        //row5 fade on 1000
        message = cmd_create(FADE_5S, randRED, randGREEN, randBLUE, 1, 5, 4, 5, true);
        sendMessage(message);
        delay(100);

        //row1 fade off 1000
        message = cmd_create(FADE_5S, 0, 0, 0, 1, 1, 4, 1, true);
        sendMessage(message);
        delay(100);
        //row2 fade off 1000
        message = cmd_create(FADE_5S, 0, 0, 0, 1, 2, 4, 2, true);
        sendMessage(message);
        delay(100);
        //row3 fade off 1000
        message = cmd_create(FADE_5S, 0, 0, 0, 1, 3, 4, 3, true);
        sendMessage(message);
        delay(100);
        //row4 fade off 1000
        message = cmd_create(FADE_5S, 0, 0, 0, 1, 4, 4, 4, true);
        sendMessage(message);
        delay(100);
        //row5 fade off 1000
        message = cmd_create(FADE_5S, 0, 0, 0, 1, 5, 4, 5, true);
        sendMessage(message);
        delay(100);

        randRED = rand255();
        randGREEN = rand255();
        randBLUE = rand255();

        //row1 fade on 500
        message = cmd_create(FADE_2S, randRED, randGREEN, randBLUE, 1, 1, 4, 1, true);
        sendMessage(message);
        delay(100);
        //row2 fade on 500
        message = cmd_create(FADE_2S, randRED, randGREEN, randBLUE, 1, 2, 4, 2, true);
        sendMessage(message);
        delay(100);
        //row3 fade on 500
        message = cmd_create(FADE_2S, randRED, randGREEN, randBLUE, 1, 3, 4, 3, true);
        sendMessage(message);
        delay(100);
        //row4 fade on 500
        message = cmd_create(FADE_2S, randRED, randGREEN, randBLUE, 1, 4, 4, 4, true);
        sendMessage(message);
        delay(100);
        //row5 fade on 500
        message = cmd_create(FADE_2S, randRED, randGREEN, randBLUE, 1, 5, 4, 5, true);
        sendMessage(message);
        delay(100);

        //row1 fade off 500
        message = cmd_create(FADE_2S, 0, 0, 0, 1, 1, 4, 1, true);
        sendMessage(message);
        delay(100);
        //row2 fade off 500
        message = cmd_create(FADE_2S, 0, 0, 0, 1, 2, 4, 2, true);
        sendMessage(message);
        delay(100);
        //row3 fade off 500
        message = cmd_create(FADE_2S, 0, 0, 0, 1, 3, 4, 3, true);
        sendMessage(message);
        delay(100);
        //row4 fade off 500
        message = cmd_create(FADE_2S, 0, 0, 0, 1, 4, 4, 4, true);
        sendMessage(message);
        delay(100);
        //row5 fade off 500
        message = cmd_create(FADE_2S, 0, 0, 0, 1, 5, 4, 5, true);
        sendMessage(message);
        delay(100);
    }
}

void vertical_lines_fade_through_short(void){
    // vertical lines fade through
    // blank display
    cmd_t message = cmd_create(SET_COLOUR, 0, 0, 0, 1, 1, 4, 5, false);
    sendMessage(message);
    // for repeat 5 times - change colour each loop
    for (int i = 0; i < 50; i++) {
        int randRED = rand255();
        int randGREEN = rand255();
        int randBLUE = rand255();
        //col1 fade on 500
        message = cmd_create(FADE_2S, randRED, randGREEN, randBLUE, 1, 1, 1, 5, true);
        sendMessage(message);
        delay(100);
        //col2 fade on 500
        message = cmd_create(FADE_2S, randRED, randGREEN, randBLUE, 2, 1, 2, 5, true);
        sendMessage(message);
        delay(100);
        //col3 fade on 500
        message = cmd_create(FADE_2S, randRED, randGREEN, randBLUE, 3, 1, 3, 5, true);
        sendMessage(message);
        delay(100);
        //col4 fade on 500
        message = cmd_create(FADE_2S, randRED, randGREEN, randBLUE, 4, 1, 4, 5, true);
        sendMessage(message);
        delay(100);

        //col1 fade off 500
        message = cmd_create(FADE_2S, 0, 0, 0, 1, 1, 1, 5, true);
        sendMessage(message);
        delay(100);
        //col2 fade off 500
        message = cmd_create(FADE_2S, 0, 0, 0, 2, 1, 2, 5, true);
        sendMessage(message);
        delay(100);
        //col3 fade off 500
        message = cmd_create(FADE_2S, 0, 0, 0, 3, 1, 3, 5, true);
        sendMessage(message);
        delay(100);
        //col4 fade off 500
        message = cmd_create(FADE_2S, 0, 0, 0, 4, 1, 4, 5, true);
        sendMessage(message);
        delay(100);
    }
}

void horizontal_lines_fade_through_short(void){
    // horizontal lines fade through
    // blank display
    cmd_t message = cmd_create(SET_COLOUR, 0, 0, 0, 1, 1, 4, 5, false);
    sendMessage(message);
    // for repeat 5 times - change colour each loop
    for (int i = 0; i < 50; i++) {
        int randRED = rand255();
        int randGREEN = rand255();
        int randBLUE = rand255();

        //row1 fade on 500
        message = cmd_create(FADE_2S, randRED, randGREEN, randBLUE, 1, 1, 4, 1, true);
        sendMessage(message);
        delay(100);
        //row2 fade on 500
        message = cmd_create(FADE_2S, randRED, randGREEN, randBLUE, 1, 2, 4, 2, true);
        sendMessage(message);
        delay(100);
        //row3 fade on 500
        message = cmd_create(FADE_2S, randRED, randGREEN, randBLUE, 1, 3, 4, 3, true);
        sendMessage(message);
        delay(100);
        //row4 fade on 500
        message = cmd_create(FADE_2S, randRED, randGREEN, randBLUE, 1, 4, 4, 4, true);
        sendMessage(message);
        delay(100);
        //row5 fade on 500
        message = cmd_create(FADE_2S, randRED, randGREEN, randBLUE, 1, 5, 4, 5, true);
        sendMessage(message);
        delay(100);

        //row1 fade off 500
        message = cmd_create(FADE_2S, 0, 0, 0, 1, 1, 4, 1, true);
        sendMessage(message);
        delay(100);
        //row2 fade off 500
        message = cmd_create(FADE_2S, 0, 0, 0, 1, 2, 4, 2, true);
        sendMessage(message);
        delay(100);
        //row3 fade off 500
        message = cmd_create(FADE_2S, 0, 0, 0, 1, 3, 4, 3, true);
        sendMessage(message);
        delay(100);
        //row4 fade off 500
        message = cmd_create(FADE_2S, 0, 0, 0, 1, 4, 4, 4, true);
        sendMessage(message);
        delay(100);
        //row5 fade off 500
        message = cmd_create(FADE_2S, 0, 0, 0, 1, 5, 4, 5, true);
        sendMessage(message);
        delay(100);
    }
}

void fluke_snake(void){
    cmd_t message;
    for (int x = 1; x <= 4; x++) {
        message = cmd_create(SET_COLOUR, 255, 255, 255, x, 1, x, 5, false);
        sendMessage(message);

        delay(150);

        message = cmd_create(SET_COLOUR, 0, 0, 0, x, 1, x, 5, false);
        sendMessage(message);
    }



    for (int x = 1; x <= 4; x++) {
        switch (x) {
            case 1:
                message = cmd_create(SET_COLOUR, COLOUR_RED, x, 1, x, 5, false);
                break;
            case 2:
                message = cmd_create(SET_COLOUR, COLOUR_ORANGE, x, 1, x, 5, false);
                break;
            case 3:
                message = cmd_create(SET_COLOUR, COLOUR_GREEN, x, 1, x, 5, false);
                break;
            case 4:
                message = cmd_create(SET_COLOUR, COLOUR_BLUE, x, 1, x, 5, false);
                break;
        }

        sendMessage(message);
        delay(200);

        message = cmd_create(SET_COLOUR, COLOUR_OFF, x, 1, x, 5, false);
        sendMessage(message);
    }


    for (int i = 0; i < 10; i++) {
        int x = (rand255() % 4) + 1;

        int colourPick = rand255() % 7;  
        switch (colourPick) {
            case 0: message = cmd_create(SET_COLOUR, COLOUR_RED, x, 1, x, 5, false); break;
            case 1: message = cmd_create(SET_COLOUR, COLOUR_ORANGE, x, 1, x, 5, false); break;
            case 2: message = cmd_create(SET_COLOUR, COLOUR_YELLOW, x, 1, x, 5, false); break;
            case 3: message = cmd_create(SET_COLOUR, COLOUR_GREEN, x, 1, x, 5, false); break;
            case 4: message = cmd_create(SET_COLOUR, COLOUR_CYAN, x, 1, x, 5, false); break;
            case 5: message = cmd_create(SET_COLOUR, COLOUR_BLUE, x, 1, x, 5, false); break;
            case 6: message = cmd_create(SET_COLOUR, COLOUR_PURPLE, x, 1, x, 5, false); break;
        }

        sendMessage(message);

        delay(100);

        message = cmd_create(SET_COLOUR, COLOUR_OFF, x, 1, x, 5, false);
        sendMessage(message);

        delay(50);
    }


    int path[][2] = {
        {1,1}, {1,2}, {1,3}, {1,4}, {1,5},
        {2,5}, {3,5}, {4,5},
        {4,4}, {4,3}, {4,2}, {4,1},
        {3,1}, {2,1}
    };

    int pathLength = sizeof(path) / sizeof(path[0]);

    for (int i = 0; i < pathLength + 2; i++) {
        if (i < pathLength) {
            int col = path[i][0];
            int row = path[i][1];
            message = cmd_create(SET_COLOUR, COLOUR_GREEN, col, row, col, row, false);
            sendMessage(message);
        }

        int tailIndex = i - 2;
        if (tailIndex >= 0) {
            int col = path[tailIndex][0];
            int row = path[tailIndex][1];
            message = cmd_create(SET_COLOUR, COLOUR_OFF, col, row, col, row, false);
            sendMessage(message);
        }

        delay(150);
    }
}

void snake(uint8_t RED, uint8_t GREEN, uint8_t BLUE){
    cmd_t message;
    int path[][2] = {
        {1,1}, {1,2}, {1,3}, {1,4}, {1,5},
        {2,5}, {3,5}, {4,5}, {4,4}, {4,3}, {4,2}, {4,1}, {3,1}, {2,1},
        {2,2}, {3,2}, {4,2}, {4,3}, {3,3}, {2,3}, {2,4}, {3,4}, {4,4}, {4,5},
        {3,5}, {2,5}, {2,4}, {1,4}, {1,3}, {1,2}, {2,2}, {3,2}, {3,1}, {2,1},
        {1,1}, {1,2}, {1,3}, {1,4}, {1,5}, {2,5}, {3,5}, {4,5}, {4,4}, {4,3},
        {4,2}, {4,1}, {3,1}, {2,1}, {2,2}, {3,2}, {4,2}, {4,3}, {3,3}, {2,3},
        {2,4}, {3,4}, {4,4}, {4,5}, {3,5}, {2,5}, {1,5}, {1,4}, {1,3}, {1,2},
        {2,2}, {3,2}, {3,1}, {2,1}, {1,1}, {1,2}, {1,3}, {1,4}, {1,5}, {2,5},
        {3,5}, {4,5}, {4,4}, {4,3}, {4,2}, {4,1}, {3,1}, {2,1}, {2,2}, {3,2},
        {4,2}, {4,3}, {3,3}, {2,3}, {2,4}, {3,4}, {4,4}, {4,5}, {3,5}, {2,5}
    };

    int pathLength = sizeof(path) / sizeof(path[0]);

    for (int i = 0; i < pathLength + 2; i++) {
        if (i < pathLength) {
            int col = path[i][0];
            int row = path[i][1];
            message = cmd_create(SET_COLOUR, RED, GREEN, BLUE, col, row, col, row, false);
            sendMessage(message);
        }

        int tailIndex = i - 2;
        if (tailIndex >= 0) {
            int col = path[tailIndex][0];
            int row = path[tailIndex][1];
            message = cmd_create(SET_COLOUR, COLOUR_OFF, col, row, col, row, false);
            sendMessage(message);
        }

        delay(100);
    }
}

void rain(uint8_t RED, uint8_t GREEN, uint8_t BLUE, int cycles, int speedMS){
    cmd_t message;
    
    int column = rand255() % SCREEN_WIDTH + 1;
    int newColumn = rand255() % SCREEN_WIDTH + 1;
    
    for (int i = 0; i < cycles; i++) {
        while (column == newColumn){
            newColumn = rand255() % SCREEN_WIDTH + 1;
        }
        column = newColumn;
        for (int j = 1; j <= SCREEN_HEIGHT; j++){
            message = cmd_create(BLINK_500MS, RED, GREEN, BLUE, column, j, column, j, false);
            sendMessage(message);
            delay(speedMS);
        }
    }
}

void horizontal_zappies(uint8_t RED, uint8_t GREEN, uint8_t BLUE, int cycles, int speedMS){
    cmd_t message;
    
    int row = rand255() % SCREEN_HEIGHT + 1;
    int newRow = rand255() % SCREEN_HEIGHT + 1;
    
    for (int i = 0; i < cycles; i++) {
        while (row == newRow){
            newRow = rand255() % SCREEN_HEIGHT + 1;
        }
        row = newRow;
        if (i % 2 == 0){
            for (int j = 1; j <= SCREEN_WIDTH; j++){
                message = cmd_create(BLINK_250MS, RED, GREEN, BLUE, j, row, j, row, false);
                sendMessage(message);
                delay(speedMS);
            }
        }
        else {
            for (int j = SCREEN_WIDTH; j >= 1; j--){
                message = cmd_create(BLINK_250MS, RED, GREEN, BLUE, j, row, j, row, false);
                sendMessage(message);
                delay(speedMS);
            }
        }
    }
}

void spiral_fill_in(void) {
    cmd_t message;
    int coords[][2] = {
        {1,1},{2,1},{3,1},{4,1},
        {4,2},{4,3},{4,4},{4,5},
        {3,5},{2,5},{1,5},
        {1,4},{1,3},{1,2},
        {2,2},{3,2},{3,3},{2,3},{2,4},{3,4}
    };
    int total = sizeof(coords)/sizeof(coords[0]);

    int r = rand255(), g = rand255(), b = rand255();

    for (int i = 0; i < total; i++) {
        int x = coords[i][0];
        int y = coords[i][1];
        message = cmd_create(SET_COLOUR, r, g, b, x, y, x, y, false);
        sendMessage(message);
        delay(100);
    }

    for (int i = total - 1; i >= 0; i--) {
        int x = coords[i][0];
        int y = coords[i][1];
        message = cmd_create(SET_COLOUR, COLOUR_OFF, x, y, x, y, false);
        sendMessage(message);
        delay(60);
    }
}

void diagonal_sweep(void) {
    cmd_t message;
    uint8_t r = rand255();
    uint8_t g = rand255();
    uint8_t b = rand255();

    // Grid is 4 rows x 5 columns
    for (int i = 2; i <= 9; i++){
        for (int row = 1; row <= 5; row++) {
            for (int col = 1; col <= 4; col++) {
                if (row + col == i) { 
                    // Diagonal elements
                    message = cmd_create(FADE_2S, r, g, b, col, row, col, row, false);
                    sendMessage(message);
                }
            }
        }
    }
    for (int i = 2; i <= 9; i++){
        for (int row = 1; row <= 5; row++) {
            for (int col = 1; col <= 4; col++) {
                if (row + col == i) { 
                    // Diagonal elements
                    message = cmd_create(FADE_2S, 0, 0, 0, col, row, col, row, false);
                    sendMessage(message);
                }
            }
        }
    }
    delay(2000);
}


