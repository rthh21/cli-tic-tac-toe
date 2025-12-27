#include <iostream>
#include <ncurses.h>
#include <cstdint>

class Engine {
private:
    bool gameFinished;
    uint8_t table[3][3];

public:
    Engine() : gameFinished(false) {
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                table[i][j] = 0;
            }
        }
    }
    
    bool placeOnPosition(uint8_t posX, uint8_t posY, uint8_t player){ 
        if (posX > 2 || posY > 2) return false;
        
        if(table[posX][posY] == 0 && !gameFinished){
            table[posX][posY] = player;
            return true;
        }
        return false;
    }
    
    uint8_t checkIfWin(){
        // Check Rows
        for(int i=0; i < 3; i++){
            if(table[i][0] != 0 && table[i][0] == table[i][1] && table[i][1] == table[i][2]) {
                gameFinished = true;
                return table[i][0];
            }
        }
        // Check Cols
        for(int i=0; i < 3; i++){
            if(table[0][i] != 0 && table[0][i] == table[1][i] && table[1][i] == table[2][i]) {
                gameFinished = true;
                return table[0][i];
            }
        }
        // Check Diagonals
        if (table[1][1] != 0) {
            if (table[0][0] == table[1][1] && table[1][1] == table[2][2]) {
                gameFinished = true;
                return table[0][0];
            }
            if (table[0][2] == table[1][1] && table[1][1] == table[2][0]) {
                gameFinished = true;
                return table[0][0];
            }
        }

        // Check Tie
        bool tie = true;
        for(int i=0; i<3; i++) 
            for(int j=0; j<3; j++) 
                if(table[i][j] == 0){
                    tie = false;
                }
        
        if(tie == true) {
            gameFinished = true; 
            return 254;
        }

        return 0;
    }
    
    uint8_t getAt(int x, int y){
        if (x < 0 || x > 2 || y < 0 || y > 2) return 0;
        return table[x][y];
    }
    
    bool isFinished(){ return gameFinished; }
    
    void reset() {
        gameFinished = false;
        for(int i=0; i<3; i++) 
            for(int j=0; j<3; j++) 
                table[i][j] = 0;
    }
};

void drawBoard(Engine &game, int currentTurn, int winner) {
    clear(); 

    if (winner == 0) {
        mvprintw(0, 0, "Turn: %c", (currentTurn == 1 ? 'X' : 'O'));
        mvprintw(1, 0, "Click a square to place | Press 'q' to quit.");
    } else if (winner == 254){
        mvprintw(0, 0, "GAME OVER! -> TIE");
        mvprintw(1, 0, "Press 'r' to restart or 'q' to quit.");
    } else {
        mvprintw(0, 0, "GAME OVER! -> Winner: %c", (winner == 1 ? 'X' : 'O'));
        mvprintw(1, 0, "Press 'r' to restart or 'q' to quit.");
    }

    // Draw the Grid
    for (int r = 0; r < 3; r++) {
        int screenY = 3 + (r * 2); 
        for (int c = 0; c < 3; c++) {
            int screenX = 3 + (c * 4);
            
            uint8_t piece = game.getAt(r, c);
            char symbol = ' '; 
            if (piece == 1) 
                symbol = 'X';
            if (piece == 2) 
                symbol = 'O';
            
            mvprintw(screenY, screenX, "%c", symbol);
            
            if (c < 2) 
                mvprintw(screenY, screenX + 2, "|");
        }
        if (r < 2) mvprintw(screenY + 1, 2, "---+---+---");
    }
    refresh();
}

int main() {
    Engine game;
    uint8_t player = 1;
    int winner = 0;
    int moveCounter;
    
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
    mouseinterval(0); 

    MEVENT event;
    int ch;

    drawBoard(game, player, winner);

    while((ch = getch()) != 'q'){
        
        if (ch == 'r' && game.isFinished()) {
            game.reset();
            moveCounter = 1;
            player = 1;
            winner = 0;
            drawBoard(game, player, winner);
            continue;
        }

        if(ch == KEY_MOUSE && !game.isFinished()){
            if(getmouse(&event) == OK){
                if(event.bstate & (BUTTON1_CLICKED | BUTTON1_PRESSED)){
                    
                    int clickRow = -1;
                    int clickCol = -1;

                    if (event.y == 3) 
                        clickRow = 0;
                    else if (event.y == 5)
                        clickRow = 1;
                    else if (event.y == 7)
                        clickRow = 2;

                    if (event.x >= 2 && event.x <= 3) 
                        clickCol = 0;
                    else if (event.x == 7)
                        clickCol = 1;
                    else if (event.x >= 10 && event.x <=12)
                        clickCol = 2;

                    if (clickRow != -1 && clickCol != -1) {
                        if (game.placeOnPosition(clickRow, clickCol, player)) {
                            winner = game.checkIfWin();
                            if (winner == 0) {
                                player = (player == 1) ? 2 : 1;
                            }
                        }
                    }
                }
            }
            drawBoard(game, player, winner);
        }
    }      
    
    endwin();
    return 0;
}