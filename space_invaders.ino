#include <gamma.h>
#include <RGBmatrixPanel.h>
#include <Adafruit_GFX.h>

//igiaconi, tmoncada, saramfox, mjmal 

// define the wiring of the LED screen
const uint8_t CLK  = 8;
const uint8_t LAT = A3;
const uint8_t OE = 9;
const uint8_t A = A0;
const uint8_t B = A1;
const uint8_t C = A2;

// define the wiring of the inputs
const int POTENTIOMETER_PIN_NUMBER = 5;
const int BUTTON_PIN_NUMBER = 10;

// global constant for the number of Hearts in the game
const int NUM_ENEMIES = 38;
int x;
int y;

// a global variable that represents the LED screen
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

// the following functions are for printing messages
void print_level(int level);
void print_lives(int lives);
void game_over();

class Color {
  public:
    int red;
    int green;
    int blue;
    Color() {
      red = 0;
      green = 0;
      blue = 0;
    }
    Color(int r, int g, int b) {
      red = r;
      green = g;
      blue = b;
    }
    uint16_t to_333() const {
      return matrix.Color333(red, green, blue);
    }
};

const Color BLACK(0, 0, 0);
const Color RED(4, 0, 0);
const Color ORANGE(6, 1, 0);
const Color YELLOW(4, 4, 0);
const Color GREEN(0, 4, 0);
const Color BLUE(0, 0, 4);
const Color PURPLE(1, 0, 2);
const Color WHITE(4, 4, 4);
const Color LIME(2, 4, 0);
const Color AQUA(0, 4, 4);
const Color PINK(255, 192, 203);
const Color BROWN(107, 50, 0);
const Color LIGHTBLUE(0, 0, 255);

class Heart {
  public:
    // Constructors
    Heart() {
      x = 0;
      y = 0;
      strength = 0;
    }
    // sets values for private date members x and y
    Heart(int x_arg, int y_arg) {
      x = x_arg;
      y = y_arg;
      strength = 0;
    }
    // sets values for private data members
    Heart(int x_arg, int y_arg, int strength_arg) {
      x = x_arg;
      y = y_arg;
      strength = strength_arg;
    }
    // sets values for private data members
    void initialize(int x_arg, int y_arg, int strength_arg) {
      x = x_arg;
      y = y_arg;
      strength = strength_arg;
    }
    
    // getters
    int get_x() const {
      return x;
    }
    int get_y() const {
      return y;
    }
    int get_strength() const {
      return strength;
    }

    // Moves the Hearts across the screen by one row
    // Modifies: y
    void move() { 
      x -= 1;
    }
    
    // draws the Hearts if its strength is greater than 0
    // calls: draw_with_rgb
    void draw() {
      if (strength > 0) {
        if (strength == 7) {
          draw_with_rgb(RED);
        }
        else if (strength == 6) {
          draw_with_rgb(WHITE);
        }
        else if (strength == 5) {
          draw_with_rgb(AQUA);
        }
        else if (strength == 4) {
          draw_with_rgb(BLUE);
        }
        else if (strength == 3) {
          draw_with_rgb(PURPLE);
        }
        else if (strength == 2) {
          draw_with_rgb(PINK);
        }
        else if (strength == 1) {
          draw_with_rgb(RED);
        }
      }
    }
    
    // draws black where the Hearts used to be
    // calls: draw_with_rgb
    void erase() {
      draw_with_rgb(BLACK);
    }    
    
    // Heart is hit by a arrow.
    // Modifies: strength
    // calls: draw, erase
    void hit() {
      strength--;
      if (strength == 0) {
        erase();
      }
      else {
        draw();
      }
    }

    void hit_level2() { 
      strength--;
    }
          

  private:
    int x;
    int y;
    int strength;
    
    // draws the Heart
    void draw_with_rgb(Color body_color) {
      matrix.drawPixel(x - 1, y, body_color.to_333());
      matrix.drawPixel(x - 3, y, body_color.to_333());

      matrix.drawPixel(x, y + 1, body_color.to_333());
      matrix.drawPixel(x - 1, y + 1, body_color.to_333());
      matrix.drawPixel(x - 2, y + 1, body_color.to_333());
      matrix.drawPixel(x - 3, y + 1, body_color.to_333());
      matrix.drawPixel(x - 4, y + 1, body_color.to_333());

      matrix.drawPixel(x - 1, y + 2, body_color.to_333());
      matrix.drawPixel(x - 2, y + 2, body_color.to_333());
      matrix.drawPixel(x - 3, y + 2, body_color.to_333());
      
      matrix.drawPixel(x - 2, y + 3, body_color.to_333());
    }
};

class Arrow {
  public:
    Arrow() {
      x = 0;
      y = 0;
      fired = false;
    }
    
    // resets private data members to initial values
    void reset() {
      x = 0;
      y = 0;
      fired = false;
    }
    
    // getters
    int get_x() const {
      return x;
    }
    int get_y() const {
      return y;
    }
    bool has_been_fired() const {
      return fired;
    }
    
    // sets private data members
    void fire(int x_arg, int y_arg) {
      x = x_arg;
      y = y_arg;
      fired = true;
    }
    
    // moves the Arrow and detects if it goes off the screen
    // Modifies: y, fired
    void move() {
      x++;
      if (x > 31) {
       fired = false;
      }
    }
    
    // resets private data members to initial values
    void hit() {
      x = -3;
      y = 0;
      fired = false;
    }
    
    // draws the arrow, if it is fired
    void draw() {
      matrix.drawPixel(x, y, BROWN.to_333());
      matrix.drawPixel(x + 1, y, BROWN.to_333());

      matrix.drawPixel(x + 2, y, LIGHTBLUE.to_333());
      matrix.drawPixel(x + 1, y - 1, LIGHTBLUE.to_333());
      matrix.drawPixel(x + 1, y + 1, LIGHTBLUE.to_333());
    }
    
    // draws black where the Arrow used to be
    void erase() {
      matrix.drawPixel(x, y, BLACK.to_333());
      matrix.drawPixel(x + 1, y, BLACK.to_333());

      matrix.drawPixel(x + 2, y, BLACK.to_333());
      matrix.drawPixel(x + 1, y - 1, BLACK.to_333());
      matrix.drawPixel(x + 1, y + 1, BLACK.to_333());
    }

  private:
    int x;
    int y;
    bool fired;
};

class Bow {
  public:
    Bow() {
      x = 0;
      y = 0;
      lives = 3;
    }
    
    // getters
    int get_x() const {
      return x;
    }
    int get_y() const {
      return y;
    }
    int get_lives() const {
      return lives;
    }
    
    // setter
    void set_x(int x_arg) {
      x = x_arg;
    }

    void set_y(int y_arg) {
      y = y_arg;
    }
    
    // Modifies: lives
    void die() {
      lives--;
    }
    
    // draws the bow
    // calls: draw_with_rgb
    void draw() {
      draw_with_rgb(BROWN, WHITE);
    }
    
    // draws black where the bow used to be
    // calls: draw_with_rgb
    void erase() {
      draw_with_rgb(BLACK, BLACK);
    }

  private:
    int x;
    int y;
    int lives;

    // sets private data members x and y to initial values
    void initialize(int x_arg, int y_arg) {
      x = x_arg;
      y = y_arg;
    }
    
    // draws the bow
    void draw_with_rgb(Color bowColor, Color tipColor) {
      matrix.drawPixel(x, y - 2, bowColor.to_333());
      matrix.drawPixel(x + 1, y - 2, bowColor.to_333());

      matrix.drawPixel(x + 2, y - 1, bowColor.to_333());
      matrix.drawPixel(x + 2, y, bowColor.to_333());
      matrix.drawPixel(x + 2, y + 1, bowColor.to_333());

      matrix.drawPixel(x, y + 2, bowColor.to_333());
      matrix.drawPixel(x + 1, y + 2, bowColor.to_333());

      matrix.drawPixel(x, y - 1, tipColor.to_333());
      matrix.drawPixel(x, y, tipColor.to_333());
      matrix.drawPixel(x, y + 1, tipColor.to_333());
    }
};

class Cupid { 
  public: 
    Cupid() {
      x = 31;
      y = 0;
      strength = 0;
    }
    // sets values for private date members x and y
    Cupid(int x_arg, int y_arg) {
      x = x_arg;
      y = y_arg;
      strength = 0;
    }
    // sets values for private data members
    Cupid(int x_arg, int y_arg, int strength_arg) {
      x = x_arg;
      y = y_arg;
      strength = strength_arg;
    } 

    void set_x(int x_arg) {
      x = x_arg;
    }

    void set_y(int y_arg) {
      y = y_arg;
    }

    void set_strength(int strength_arg) {
      strength = strength_arg;
    }

    int get_strength() { 
      return strength;       
    }

    int get_x() { 
      return x;       
    }
    
    int get_y() { 
      return y;       
    }    

    void draw_without_arrow() {
      if (strength > 0) {
        if (strength == 8) {
          draw_with_rgb_without_arrow(PINK, WHITE);
        }
        else if (strength == 7) {
           draw_with_rgb_without_arrow(ORANGE, WHITE);
        }
        else if (strength == 6) {
           draw_with_rgb_without_arrow(YELLOW, WHITE);
        }
        else if (strength == 5) {
           draw_with_rgb_without_arrow(LIME, WHITE);
        }
        else if (strength == 4) {
           draw_with_rgb_without_arrow(GREEN, WHITE);
        }
        else if (strength == 3) {
           draw_with_rgb_without_arrow(AQUA, WHITE);
        }
        else if (strength == 2) {
           draw_with_rgb_without_arrow(BLUE, WHITE);
        }
        else if (strength == 1) {
           draw_with_rgb_without_arrow(PURPLE, WHITE);
        }
      }
    }

    void draw() {
      draw_with_rgb(RED, PINK, WHITE);
    }


    void erase() {
      draw_with_rgb_without_arrow(BLACK, BLACK);
    }

    void hit() {
      strength--;
      if (strength == 0) {
        erase();
      }
      else {
        draw_without_arrow();
      }
    }


    void move() { 
      x -= 1; 
    }

    void initialize (int x_arg, int y_arg, int strength_arg) {
      x = x_arg; 
      y = y_arg; 
      strength = strength_arg; 
    }

  private: 
    int x; 
    int y; 
    int strength; 

    void draw_with_rgb_without_arrow(Color outfit, Color body) {
      matrix.drawPixel(x - 10, y, outfit.to_333()); 

      matrix.drawPixel(x - 9, y + 1, outfit.to_333());
      matrix.drawPixel(x - 7, y + 1, body.to_333());
      matrix.drawPixel(x - 6, y + 1, body.to_333());
      matrix.drawPixel(x - 5, y + 1, body.to_333());

      matrix.drawPixel(x - 10, y + 2, outfit.to_333());
      matrix.drawPixel(x - 8, y + 2, body.to_333());
      matrix.drawPixel(x - 7, y + 2, body.to_333());
      matrix.drawPixel(x - 6, y + 2, body.to_333());
      matrix.drawPixel(x - 5, y + 2, body.to_333());
      matrix.drawPixel(x - 4, y + 2, body.to_333());
      
      matrix.drawPixel(x - 10, y + 3, outfit.to_333());
      matrix.drawPixel(x - 8, y + 3, body.to_333());
      matrix.drawPixel(x - 6, y + 3, body.to_333());
      matrix.drawPixel(x - 4, y + 3, body.to_333());      

      matrix.drawPixel(x - 11, y + 4, outfit.to_333());
      matrix.drawPixel(x - 8, y + 4, body.to_333());
      matrix.drawPixel(x - 7, y + 4, body.to_333());
      matrix.drawPixel(x - 6, y + 4, body.to_333());
      matrix.drawPixel(x - 5, y + 4, body.to_333());
      matrix.drawPixel(x - 4, y + 4, body.to_333());
      matrix.drawPixel(x -2, y + 4, outfit.to_333());
      matrix.drawPixel(x - 1, y + 4, outfit.to_333());
      matrix.drawPixel(x, y + 4, outfit.to_333());

      matrix.drawPixel(x - 11, y + 5, outfit.to_333());
      matrix.drawPixel(x - 7, y + 5, body.to_333());
      matrix.drawPixel(x - 6, y + 5, body.to_333());
      matrix.drawPixel(x - 5, y + 5, body.to_333());
      matrix.drawPixel(x - 3, y + 5, outfit.to_333());
      matrix.drawPixel(x - 2, y + 5, outfit.to_333());
      matrix.drawPixel(x - 1, y + 5, outfit.to_333());

      matrix.drawPixel(x - 11, y + 6, outfit.to_333());
      matrix.drawPixel(x - 8, y + 6, body.to_333());
      matrix.drawPixel(x - 7, y + 6, body.to_333());
      matrix.drawPixel(x - 6, y + 6, body.to_333());
      matrix.drawPixel(x - 5, y + 6, body.to_333());
      matrix.drawPixel(x - 4, y + 6, outfit.to_333());
      matrix.drawPixel(x - 3, y + 6, outfit.to_333());
      matrix.drawPixel(x - 2, y + 6, outfit.to_333());

      matrix.drawPixel(x - 11, y + 7, outfit.to_333());
      matrix.drawPixel(x - 7, y + 7, body.to_333());
      matrix.drawPixel(x - 5, y + 7, body.to_333());
      matrix.drawPixel(x - 5, y + 7, body.to_333());
      matrix.drawPixel(x - 4, y + 7, body.to_333());
      matrix.drawPixel(x - 3, y + 7, outfit.to_333());
      matrix.drawPixel(x - 2, y + 7, outfit.to_333());
      matrix.drawPixel(x - 1, y + 7, outfit.to_333());

      matrix.drawPixel(x - 11, y + 8, outfit.to_333());
      matrix.drawPixel(x - 9, y + 8, body.to_333());
      matrix.drawPixel(x - 8, y + 8, body.to_333());
      matrix.drawPixel(x - 7, y + 8, body.to_333());
      matrix.drawPixel(x - 6, y + 8, body.to_333());
      matrix.drawPixel(x - 5, y + 8, body.to_333());
      matrix.drawPixel(x - 4, y + 8, body.to_333());      

      matrix.drawPixel(x - 10, y + 9, outfit.to_333());
      matrix.drawPixel(x - 8, y + 9, outfit.to_333());
      matrix.drawPixel(x - 7, y + 9, outfit.to_333());
      matrix.drawPixel(x - 6, y + 9, outfit.to_333());
      matrix.drawPixel(x - 5, y + 9, outfit.to_333());
      matrix.drawPixel(x - 4, y + 9, outfit.to_333());

      matrix.drawPixel(x - 10, y + 10, outfit.to_333());
      matrix.drawPixel(x - 8, y + 10, body.to_333());
      matrix.drawPixel(x - 7, y + 10, body.to_333());
      matrix.drawPixel(x - 6, y + 10, body.to_333());
      matrix.drawPixel(x - 5, y + 10, body.to_333());
      matrix.drawPixel(x - 4, y + 10, body.to_333());

      matrix.drawPixel(x - 9, y + 11, outfit.to_333());
      matrix.drawPixel(x - 7, y + 11, body.to_333());
      matrix.drawPixel(x - 4, y + 11, body.to_333());
      matrix.drawPixel(x - 2, y + 11, body.to_333());
      matrix.drawPixel(x - 1, y + 11, body.to_333());

      matrix.drawPixel(x - 10, y + 12, outfit.to_333());
      matrix.drawPixel(x - 7, y + 12, body.to_333());
      matrix.drawPixel(x - 4, y + 12, body.to_333());
      matrix.drawPixel(x - 3, y + 12, body.to_333());
      matrix.drawPixel(x - 2, y + 12, body.to_333());

      matrix.drawPixel(x - 7, y + 13, body.to_333());
      matrix.drawPixel(x - 6, y + 13, body.to_333());

      matrix.drawPixel(x - 6, y + 14, body.to_333());
      
      matrix.drawPixel(x - 6, y + 15, body.to_333());
    }  

    void draw_with_rgb(Color outfit, Color arrow, Color body) {
      matrix.drawPixel(x - 10, y, outfit.to_333()); 

      matrix.drawPixel(x - 9, y + 1, outfit.to_333());
      matrix.drawPixel(x - 7, y + 1, body.to_333());
      matrix.drawPixel(x - 6, y + 1, body.to_333());
      matrix.drawPixel(x - 5, y + 1, body.to_333());

      matrix.drawPixel(x - 10, y + 2, outfit.to_333());
      matrix.drawPixel(x - 8, y + 2, body.to_333());
      matrix.drawPixel(x - 7, y + 2, body.to_333());
      matrix.drawPixel(x - 6, y + 2, body.to_333());
      matrix.drawPixel(x - 5, y + 2, body.to_333());
      matrix.drawPixel(x - 4, y + 2, body.to_333());
      
      matrix.drawPixel(x - 10, y + 3, outfit.to_333());
      matrix.drawPixel(x - 8, y + 3, body.to_333());
      matrix.drawPixel(x - 6, y + 3, body.to_333());
      matrix.drawPixel(x - 4, y + 3, body.to_333());      

      matrix.drawPixel(x - 13, y + 4, arrow.to_333());
      matrix.drawPixel(x - 11, y + 4, outfit.to_333());
      matrix.drawPixel(x - 8, y + 4, body.to_333());
      matrix.drawPixel(x - 7, y + 4, body.to_333());
      matrix.drawPixel(x - 6, y + 4, body.to_333());
      matrix.drawPixel(x - 5, y + 4, body.to_333());
      matrix.drawPixel(x - 4, y + 4, body.to_333());
      matrix.drawPixel(x -2, y + 4, outfit.to_333());
      matrix.drawPixel(x - 1, y + 4, outfit.to_333());
      matrix.drawPixel(x, y + 4, outfit.to_333());

      matrix.drawPixel(x - 14, y + 5, arrow.to_333());
      matrix.drawPixel(x - 11, y + 5, outfit.to_333());
      matrix.drawPixel(x - 7, y + 5, body.to_333());
      matrix.drawPixel(x - 6, y + 5, body.to_333());
      matrix.drawPixel(x - 5, y + 5, body.to_333());
      matrix.drawPixel(x - 3, y + 5, outfit.to_333());
      matrix.drawPixel(x - 2, y + 5, outfit.to_333());
      matrix.drawPixel(x - 1, y + 5, outfit.to_333());

      matrix.drawPixel(x - 15, y + 6, arrow.to_333());
      matrix.drawPixel(x - 14, y + 6, arrow.to_333());
      matrix.drawPixel(x - 13, y + 6, arrow.to_333());
      matrix.drawPixel(x - 12, y + 6, arrow.to_333());
      matrix.drawPixel(x - 11, y + 6, outfit.to_333());
      matrix.drawPixel(x - 10, y + 6, arrow.to_333());
      matrix.drawPixel(x - 9, y + 6, arrow.to_333());
      matrix.drawPixel(x - 8, y + 6, body.to_333());
      matrix.drawPixel(x - 7, y + 6, body.to_333());
      matrix.drawPixel(x - 6, y + 6, body.to_333());
      matrix.drawPixel(x - 5, y + 6, body.to_333());
      matrix.drawPixel(x - 4, y + 6, outfit.to_333());
      matrix.drawPixel(x - 3, y + 6, outfit.to_333());
      matrix.drawPixel(x - 2, y + 6, outfit.to_333());

      matrix.drawPixel(x - 14, y + 7, arrow.to_333());
      matrix.drawPixel(x - 11, y + 7, outfit.to_333());
      matrix.drawPixel(x - 7, y + 7, body.to_333());
      matrix.drawPixel(x - 5, y + 7, body.to_333());
      matrix.drawPixel(x - 5, y + 7, body.to_333());
      matrix.drawPixel(x - 4, y + 7, body.to_333());
      matrix.drawPixel(x - 3, y + 7, outfit.to_333());
      matrix.drawPixel(x - 2, y + 7, outfit.to_333());
      matrix.drawPixel(x - 1, y + 7, outfit.to_333());

      matrix.drawPixel(x - 13, y + 8, arrow.to_333());
      matrix.drawPixel(x - 11, y + 8, outfit.to_333());
      matrix.drawPixel(x - 9, y + 8, body.to_333());
      matrix.drawPixel(x - 8, y + 8, body.to_333());
      matrix.drawPixel(x - 7, y + 8, body.to_333());
      matrix.drawPixel(x - 6, y + 8, body.to_333());
      matrix.drawPixel(x - 5, y + 8, body.to_333());
      matrix.drawPixel(x - 4, y + 8, body.to_333());      

      matrix.drawPixel(x - 10, y + 9, outfit.to_333());
      matrix.drawPixel(x - 8, y + 9, outfit.to_333());
      matrix.drawPixel(x - 7, y + 9, outfit.to_333());
      matrix.drawPixel(x - 6, y + 9, outfit.to_333());
      matrix.drawPixel(x - 5, y + 9, outfit.to_333());
      matrix.drawPixel(x - 4, y + 9, outfit.to_333());

      matrix.drawPixel(x - 10, y + 10, outfit.to_333());
      matrix.drawPixel(x - 8, y + 10, body.to_333());
      matrix.drawPixel(x - 7, y + 10, body.to_333());
      matrix.drawPixel(x - 6, y + 10, body.to_333());
      matrix.drawPixel(x - 5, y + 10, body.to_333());
      matrix.drawPixel(x - 4, y + 10, body.to_333());

      matrix.drawPixel(x - 9, y + 11, outfit.to_333());
      matrix.drawPixel(x - 7, y + 11, body.to_333());
      matrix.drawPixel(x - 4, y + 11, body.to_333());
      matrix.drawPixel(x - 2, y + 11, body.to_333());
      matrix.drawPixel(x - 1, y + 11, body.to_333());

      matrix.drawPixel(x - 10, y + 12, outfit.to_333());
      matrix.drawPixel(x - 7, y + 12, body.to_333());
      matrix.drawPixel(x - 4, y + 12, body.to_333());
      matrix.drawPixel(x - 3, y + 12, body.to_333());
      matrix.drawPixel(x - 2, y + 12, body.to_333());

      matrix.drawPixel(x - 7, y + 13, body.to_333());
      matrix.drawPixel(x - 6, y + 13, body.to_333());

      matrix.drawPixel(x - 6, y + 14, body.to_333());
      
      matrix.drawPixel(x - 6, y + 15, body.to_333());
    }
};

void bigHeartScreen() {
  x = 0;
  y = 0;
  matrix.fillScreen(BLACK.to_333());
  matrix.drawPixel(x + 5, y + 6, RED.to_333());
  matrix.drawPixel(x + 6, y + 6, RED.to_333());
  matrix.drawPixel(x + 10, y + 6, RED.to_333());
  matrix.drawPixel(x + 11, y + 6, RED.to_333());

  matrix.drawPixel(x + 4, y + 7, RED.to_333());
  matrix.drawPixel(x + 5, y + 7, RED.to_333());
  matrix.drawPixel(x + 6, y + 7, RED.to_333());
  matrix.drawPixel(x + 7, y + 7, RED.to_333());
  matrix.drawPixel(x + 9, y + 7, RED.to_333());
  matrix.drawPixel(x + 10, y + 7, RED.to_333());
  matrix.drawPixel(x + 11, y + 7, RED.to_333());
  matrix.drawPixel(x + 12, y + 7, RED.to_333());

  for (int i = 3; i < 14; i++) {
    for(int j = 8; j < 11; j++) {
       matrix.drawPixel(x + i, y + j, RED.to_333());
    }
  }
  
  matrix.drawPixel(x + 4, y + 11, RED.to_333());
  matrix.drawPixel(x + 5, y + 11, RED.to_333());
  matrix.drawPixel(x + 6, y + 11, RED.to_333());
  matrix.drawPixel(x + 7, y + 11, RED.to_333());
  matrix.drawPixel(x + 8, y + 11, RED.to_333());
  matrix.drawPixel(x + 9, y + 11, RED.to_333());
  matrix.drawPixel(x + 10, y + 11, RED.to_333());
  matrix.drawPixel(x + 11, y + 11, RED.to_333());
  matrix.drawPixel(x + 12, y + 11, RED.to_333());

  matrix.drawPixel(x + 5, y + 12, RED.to_333());
  matrix.drawPixel(x + 6, y + 12, RED.to_333());
  matrix.drawPixel(x + 7, y + 12, RED.to_333());
  matrix.drawPixel(x + 8, y + 12, RED.to_333());
  matrix.drawPixel(x + 9, y + 12, RED.to_333());
  matrix.drawPixel(x + 10, y + 12, RED.to_333());
  matrix.drawPixel(x + 11, y + 12, RED.to_333());

  matrix.drawPixel(x + 6, y + 13, RED.to_333());
  matrix.drawPixel(x + 7, y + 13, RED.to_333());
  matrix.drawPixel(x + 8, y + 13, RED.to_333());
  matrix.drawPixel(x + 9, y + 13, RED.to_333());
  matrix.drawPixel(x + 10, y + 13, RED.to_333());

  matrix.drawPixel(x + 7, y + 14, RED.to_333());
  matrix.drawPixel(x + 8, y + 14, RED.to_333());
  matrix.drawPixel(x + 9, y + 14, RED.to_333());

  matrix.drawPixel(x + 8, y + 15, RED.to_333());
}

void bigHeart() {
  x = 0;
  y = 0;
  matrix.fillScreen(BLACK.to_333());
  matrix.drawPixel(x + 12, y + 3, RED.to_333());
  matrix.drawPixel(x + 13, y + 3, RED.to_333());
  matrix.drawPixel(x + 17, y + 3, RED.to_333());
  matrix.drawPixel(x + 18, y + 3, RED.to_333());

  matrix.drawPixel(x + 11, y + 4, RED.to_333());
  matrix.drawPixel(x + 12, y + 4, RED.to_333());
  matrix.drawPixel(x + 13, y + 4, RED.to_333());
  matrix.drawPixel(x + 14, y + 4, RED.to_333());
  matrix.drawPixel(x + 16, y + 4, RED.to_333());
  matrix.drawPixel(x + 17, y + 4, RED.to_333());
  matrix.drawPixel(x + 18, y + 4, RED.to_333());
  matrix.drawPixel(x + 19, y + 4, RED.to_333());

  for (int i = 10; i < 21; i++) {
    for(int j = 5; j < 8; j++) {
       matrix.drawPixel(x + i, y + j, RED.to_333());
    }
  }
  
  matrix.drawPixel(x + 11, y + 8, RED.to_333());
  matrix.drawPixel(x + 12, y + 8, RED.to_333());
  matrix.drawPixel(x + 13, y + 8, RED.to_333());
  matrix.drawPixel(x + 14, y + 8, RED.to_333());
  matrix.drawPixel(x + 15, y + 8, RED.to_333());
  matrix.drawPixel(x + 16, y + 8, RED.to_333());
  matrix.drawPixel(x + 17, y + 8, RED.to_333());
  matrix.drawPixel(x + 18, y + 8, RED.to_333());
  matrix.drawPixel(x + 19, y + 8, RED.to_333());

  matrix.drawPixel(x + 12, y + 9, RED.to_333());
  matrix.drawPixel(x + 13, y + 9, RED.to_333());
  matrix.drawPixel(x + 14, y + 9, RED.to_333());
  matrix.drawPixel(x + 15, y + 9, RED.to_333());
  matrix.drawPixel(x + 16, y + 9, RED.to_333());
  matrix.drawPixel(x + 17, y + 9, RED.to_333());
  matrix.drawPixel(x + 18, y + 9, RED.to_333());

  matrix.drawPixel(x + 13, y + 10, RED.to_333());
  matrix.drawPixel(x + 14, y + 10, RED.to_333());
  matrix.drawPixel(x + 15, y + 10, RED.to_333());
  matrix.drawPixel(x + 16, y + 10, RED.to_333());
  matrix.drawPixel(x + 17, y + 10, RED.to_333());

  matrix.drawPixel(x + 14, y + 11, RED.to_333());
  matrix.drawPixel(x + 15, y + 11, RED.to_333());
  matrix.drawPixel(x + 16, y + 11, RED.to_333());

  matrix.drawPixel(x + 15, y + 12, RED.to_333());
}

class CupidArrow {
  public:
  CupidArrow() {
      x = 0;
      y = 0;
      fired = false;
    }
    
    // resets private data members to initial values
    void reset() {
      x = 0;
      y = 0;
      fired = false;
    }
    
    // getters
    int get_x() const {
      return x;
    }
    int get_y() const {
      return y;
    }
    bool has_been_fired() const {
      return fired;
    }
    
    // sets private data members
    void fire(int x_arg, int y_arg) {
      x = x_arg;
      y = y_arg;
      fired = true;
    }
    
    // moves the Arrow and detects if it goes off the screen
    // Modifies: y, fired
    void move() {
      x--;
      if (x < -4) {
       fired = false;
      }
    }
    
    // resets private data members to initial values
    void hit() {
      x = -10;
      y = 0;
      fired = false;
    }

    void set_y(int y_arg) {
      y = y_arg;
    }
    
    void set_x(int x_arg) {
      x = x_arg;
    }

    // draws the arrow, if it is fired
    void draw() {
      matrix.drawPixel(x - 9, y + 6, WHITE.to_333());
      matrix.drawPixel(x - 10, y + 6, WHITE.to_333());
      matrix.drawPixel(x - 11, y + 6, WHITE.to_333());
      matrix.drawPixel(x - 12, y + 6, WHITE.to_333());
      matrix.drawPixel(x - 13, y + 6, RED.to_333());
      

      matrix.drawPixel(x - 12, y + 7, RED.to_333());
      matrix.drawPixel(x - 12, y + 5, RED.to_333());

    }
    
    // draws black where the Arrow used to be
    void erase() {
      matrix.drawPixel(x - 9, y + 6, BLACK.to_333());
      matrix.drawPixel(x - 10, y + 6, BLACK.to_333());
      matrix.drawPixel(x - 11, y + 6, BLACK.to_333());
      matrix.drawPixel(x - 12, y + 6, BLACK.to_333());
      matrix.drawPixel(x - 13, y + 6, BLACK.to_333());
      

      matrix.drawPixel(x - 12, y + 7, BLACK.to_333());
      matrix.drawPixel(x - 12, y + 5, BLACK.to_333());
    }

  private:
    int x;
    int y;
    bool fired;
};




class Game {
  public:
    Game() {
      level = 1;
      time = 0;
      time2 = 0;
      time_arrow = 0; 
      time_cupid_arrow = 0;
      time_heart = 2000; 
      time_heart2 = 2000;
    }
    
    // sets up a new game of Heartbreaker
    // Modifies: global variable matrix
    void setupGame() {
      matrix.fillScreen(BLACK.to_333());
      matrix.setCursor(0, 0);
      cupid.set_x(31);
      cupid.set_y(0);
      cupid.draw();
      bigHeartScreen();
      cupid.draw();
      print_level(level);
      delay(2500);
      matrix.drawPixel(x + 4, y + 1, BLACK.to_333());
      matrix.drawPixel(x + 4, y + 2, BLACK.to_333());
      matrix.drawPixel(x + 4, y + 3, BLACK.to_333());
      matrix.drawPixel(x + 4, y + 4, BLACK.to_333());
      cupid.draw();
      bigHeartScreen();
      cupid.draw();
      print_lives(bow.get_lives());
      delay(2500);
      matrix.fillScreen(BLACK.to_333());
      matrix.setCursor(0, 0);
      bow.set_x(0);
      bow.set_y(10);
      bow.draw();
      make_level(level);
    }
    
    // advances the game simulation one step and renders the graphics
    // see spec for details of game
    // Modifies: global variable matrix
    void update(int potentiometer_value, bool button_pressed) {
      time = millis();
      if (bow.get_lives() == 0) {
        game_over();
        delay(4000);
        matrix.fillScreen(BLACK.to_333());
      }
      else {
        if (level == 1) {
        //move bow
          bow.erase();
          bow.set_y(((16) * potentiometer_value) / 1024);
          bow.draw();

          //move arrow
          if (button_pressed && !arrow.has_been_fired()) {
            arrow.fire(bow.get_x(), bow.get_y());
            time_arrow = time; 
          }
          if (arrow.has_been_fired() && (time + 10 > time_arrow)) {   
            arrow.erase();    
            arrow.move(); 
            if (check_collision_arrow_heart(arrow.get_x(), arrow.get_y())) {
              arrow.hit();
              arrow.erase();
            }
            if (arrow.get_x() >= 31) {
              arrow.erase();
            }
            else {
            arrow.draw();
            time_arrow = time + 15; 
            }
          }          
          //move Hearts
          bool bowHit = false;
          unsigned long currentTime = millis();    
          if (currentTime > time_heart) {
            for (int i = 0; i < 36; i++) {
              if (enemies[i].get_strength() > 0) {
                enemies[i].draw();
                enemies[i].erase();
                enemies[i].move();
                enemies[i].erase();
                enemies[i].draw();
                matrix.setCursor(32, 0);
              }
            }
            time_heart = currentTime + 1500;
            if (check_collision_heart()) {
              bow.die();
              delay(1000);
              reset_level();
            }
          }
          if (level_cleared(level)) { 
            level++; 
            reset_level(); 
          }
        }
        else {
          //move bow
          bow.erase();
          bow.set_y(((16) * potentiometer_value) / 1024);
          bow.draw();

          //move arrow
          if (button_pressed && !arrow.has_been_fired()) {
            arrow.fire(bow.get_x(), bow.get_y());
            time_arrow = time; 
          }
          if (arrow.has_been_fired() && (time + 10 > time_arrow)) {   
            arrow.erase();    
            arrow.move(); 
            if (check_collision_arrow_heart(arrow.get_x(), arrow.get_y())) {
              arrow.hit();
              arrow.erase();
            }
            else if (check_collision_cupid_to_arrow(arrow.get_x(), arrow.get_y())) {
               arrow.hit();
              arrow.erase();
            }
            else {
            arrow.draw();
            time_arrow = time + 22; 
            }
          }

          //move cupid
          unsigned long currentTime2 = millis(); 
          if (currentTime2 > time_cupid) { 
            if (cupid.get_strength() > 0) {
              cupid.draw_without_arrow(); 
              cupid.erase();
              cupid.move();
              cupid.draw_without_arrow();
            }
            time_cupid = currentTime2 + 900;
            if (check_collision_cupid(bow.get_x(), bow.get_y())) {
              bow.die();
              delay(1000);
              reset_level();
            }
          }

          //move cupid arrow
          if (!cupid_arrow.has_been_fired()) {
            cupid_arrow.fire(cupid.get_x(), cupid.get_y());
            time_cupid_arrow = time;
          }
          if ((cupid_arrow.has_been_fired()) && (time + 15 > time_cupid_arrow)) {
            cupid_arrow.draw(); 
            cupid_arrow.erase();    
            cupid_arrow.move(); 
            cupid_arrow.draw();
            time_cupid_arrow = time + 85; 
            if (check_collision_cupid_arrow(cupid_arrow.get_x(), cupid_arrow.get_y())) {
              bow.die();
              delay(500);
              reset_level();
            }
          }

          //move mini hearts
          unsigned long currentTime3 = millis(); 
          if (currentTime3 > time_heart2) {
            for (int i = 36; i < 38; i++) {
              if (enemies[i].get_strength() > 0) {
                enemies[i].draw();
                enemies[i].erase();
                enemies[i].move();
                enemies[i].erase();
                enemies[i].draw();
              }
            }
            time_heart2 = currentTime3 + 1000;
            if (check_collision_heart()) {
                bow.die();
                delay(1000);
                reset_level();
            }
          }
          if (level_cleared(level)) { 
            final_message();
          }           
        }
      }
    }

  private:
    int level;
    unsigned long time;
    unsigned long time2;
    unsigned long time_arrow; 
    unsigned long time_heart;
    unsigned long time_heart2;
    unsigned long time_cupid;
    unsigned long time_cupid_arrow;
    Bow bow;
    Arrow arrow;
    Cupid cupid;
    CupidArrow cupid_arrow;
    Heart enemies[NUM_ENEMIES];

    void make_level(int lvl) {
      //level 1
      if (lvl == 1) {
        enemies[0].initialize(21, 0, 1);
        enemies[1].initialize(21, 4, 2);
        enemies[2].initialize(21, 8, 1);
        enemies[3].initialize(21, 12, 2);
        enemies[4].initialize(26, 0, 2);
        enemies[5].initialize(26, 4, 1);
        enemies[6].initialize(26, 8, 2);
        enemies[7].initialize(26, 12, 1);

        enemies[8].initialize(31, 0, 3);
        enemies[9].initialize(31, 4, 2);
        enemies[10].initialize(31, 8, 3);
        enemies[11].initialize(31, 12, 2);
        enemies[12].initialize(36, 0, 2);
        enemies[13].initialize(36, 4, 3);
        enemies[14].initialize(36, 8, 2);
        enemies[15].initialize(36, 12, 3);

        enemies[16].initialize(41, 0, 3);
        enemies[17].initialize(41, 4, 4);
        enemies[18].initialize(41, 8, 3);
        enemies[19].initialize(41, 12, 4);
        enemies[20].initialize(46, 0, 4);
        enemies[21].initialize(46, 4, 3);
        enemies[22].initialize(46, 8, 4);
        enemies[23].initialize(46, 12, 3);

        enemies[24].initialize(51, 0, 5);
        enemies[25].initialize(51, 4, 4);
        enemies[26].initialize(51, 8, 5);
        enemies[27].initialize(51, 12, 4);
        enemies[28].initialize(56, 0, 4);
        enemies[29].initialize(56, 4, 5);
        enemies[30].initialize(56, 8, 4);
        enemies[31].initialize(56, 12, 5);

        enemies[32].initialize(61, 0, 5);
        enemies[33].initialize(61, 4, 6);
        enemies[34].initialize(61, 8, 5);
        enemies[35].initialize(61, 12, 6);

        for (int i = 0; i < NUM_ENEMIES; i++) {
          enemies[i].draw();
        }
      }
      else if (lvl == 2) {
        cupid.initialize(31, 0, 8);
        enemies[36].initialize(15, 0, 7);
        enemies[37].initialize(15, 12, 7);
        cupid.draw_without_arrow();
        enemies[36].draw();
        enemies[37].draw();
      }
    }

    //done
    bool check_collision_arrow_heart(int arrow_x, int arrow_y) { 
      for (int i = 0; i < NUM_ENEMIES; i++) {
        if (enemies[i].get_strength() > 0 ) { 
          if ((arrow_y == enemies[i].get_y()) && ((arrow_x + 2) >= (enemies[i].get_x() - 3))) { 
            enemies[i].hit();    
            return true;
          }
          else if ((arrow_y == (enemies[i].get_y() + 1)) && ((arrow_x + 2) >= (enemies[i].get_x() - 4))) {
            enemies[i].hit(); 
            return true;
          }
          else if ((arrow_y == (enemies[i].get_y() + 2)) && ((arrow_x + 2) >= (enemies[i].get_x() - 3))) {
            enemies[i].hit(); 
            return true;
          }
          else if ((arrow_y == (enemies[i].get_y() + 3)) && ((arrow_x + 2) >= (enemies[i].get_x() - 2))) {
            enemies[i].hit(); 
            return true;
          }
          else if ((arrow_y + 1 == (enemies[i].get_y())) && ((arrow_x + 1) >= (enemies[i].get_x() - 1))) {
            enemies[i].hit(); 
            return true;
          }
          else if ((arrow_y - 1 == (enemies[i].get_y() + 3)) && ((arrow_x + 1) >= (enemies[i].get_x() - 2))) {
            enemies[i].hit(); 
            return true;
          }
        }
      }
      return false;
    }

    //done
    bool check_collision_cupid_to_arrow(int arrow_x, int arrow_y) {
      if (cupid.get_strength() > 0) { 
        if ((arrow_y == cupid.get_y()) && ((arrow_x + 2) >= (cupid.get_x() - 10))) { 
          cupid.hit();    
          return true;
        }
        else if ((arrow_y == (cupid.get_y() + 1)) && ((arrow_x + 2) >= (cupid.get_x() - 9))) {
          cupid.hit(); 
          return true;
        }
        else if ((arrow_y == (cupid.get_y() + 2)) && ((arrow_x + 2) >= (cupid.get_x() - 10))) {
          cupid.hit(); 
          return true;
        }
       else if ((arrow_y == (cupid.get_y() + 3)) && ((arrow_x + 2) >= (cupid.get_x() - 10))) {
          cupid.hit(); 
          return true;
        }    
        else if ((arrow_y == (cupid.get_y() + 4)) && ((arrow_x + 2) >= (cupid.get_x() - 13))) {
          cupid.hit(); 
          return true;
        }
        else if ((arrow_y == (cupid.get_y() + 5)) && ((arrow_x + 2) >= (cupid.get_x() - 14))) {
          cupid.hit(); 
          return true;
        }
        else if ((arrow_y == (cupid.get_y() + 6)) && ((arrow_x + 2) >= (cupid.get_x() - 15))) {
          cupid.hit(); 
          return true;
        }
        else if ((arrow_y == (cupid.get_y() + 7)) && ((arrow_x + 2) >= (cupid.get_x() - 14))) {
          cupid.hit(); 
          return true;
        }       
        else if ((arrow_y == (cupid.get_y() + 8)) && ((arrow_x + 2) >= (cupid.get_x() - 13))) {
          cupid.hit(); 
          return true;
        } 
        else if ((arrow_y == (cupid.get_y() + 9)) && ((arrow_x + 2) >= (cupid.get_x() - 10))) {
          cupid.hit(); 
          return true;
        } 
        else if ((arrow_y == (cupid.get_y() + 10)) && ((arrow_x + 2) >= (cupid.get_x() - 10))) {
          cupid.hit(); 
          return true;
        }
        else if ((arrow_y == (cupid.get_y() + 11)) && ((arrow_x + 2) >= (cupid.get_x() - 9))) {
          cupid.hit(); 
          return true;
        }  
       else if ((arrow_y == (cupid.get_y() + 12)) && ((arrow_x + 2) >= (cupid.get_x() - 10))) {
          cupid.hit(); 
          return true;
        }   
        else if ((arrow_y == (cupid.get_y() + 13)) && ((arrow_x + 2) >= (cupid.get_x() - 7))) {
          cupid.hit(); 
          return true;
        } 
        else if ((arrow_y == (cupid.get_y() + 14)) && ((arrow_x + 2) >= (cupid.get_x() - 6))) {
          cupid.hit(); 
          return true;
        }
        else if ((arrow_y == (cupid.get_y() + 15)) && ((arrow_x + 2) >= (cupid.get_x() - 6))) {
          cupid.hit(); 
          return true;
        }                                 
      }
      return false;
    }

    //done
    bool check_collision_cupid(int bow_x, int bow_y) { 
      if (cupid.get_strength() > 0) {
        if ((bow_y == cupid.get_y()) && ((bow_x + 2) == (cupid.get_x() - 10))) {
          return true;
        }
        else if (((bow_y - 1) == cupid.get_y()) && ((bow_x + 2) == (cupid.get_x() - 10)) &&
              (((bow_y + 1) == cupid.get_y() + 2) && ((bow_x + 2) == (cupid.get_x() - 10)))) {
          return true;
        }
        else if (((bow_y) == cupid.get_y() + 2) && ((bow_x + 2) == (cupid.get_x() - 10)) &&
                (((bow_y + 1) == cupid.get_y() + 3) && ((bow_x + 2) == (cupid.get_x() - 10))) &&
                (((bow_y + 2) == cupid.get_y() + 4) && ((bow_x + 1) == (cupid.get_x() - 11)))) {
          return true;
        }
        else if (((bow_y) == cupid.get_y() + 10) && ((bow_x + 2) == (cupid.get_x() - 10)) &&
                (((bow_y - 1) == cupid.get_y() + 9) && ((bow_x + 2) == (cupid.get_x() - 10))) &&
                (((bow_y - 2) == cupid.get_y() + 8) && ((bow_x + 1) == (cupid.get_x() - 11)))) {
          return true;
        }
        else if (((bow_y - 1) == cupid.get_y() + 10) && ((bow_x + 2) == (cupid.get_x() - 10)) &&
                (((bow_y + 1) == cupid.get_y() + 12) && ((bow_x + 2) == (cupid.get_x() - 10)))) {
          return true;
        }
        else if ((bow_y == cupid.get_y() + 12) && ((bow_x + 2) == (cupid.get_x() - 10))) {
          return true;
        }
        else if (((bow_y - 1) == cupid.get_y() + 12) && ((bow_x + 2) == (cupid.get_x() - 10))) {
          return true;
        }
        else if (((bow_y - 2) == cupid.get_y() + 12) && ((bow_x + 1) == (cupid.get_x() - 10))) {
          return true;
        }
        else if (((bow_y) == cupid.get_y() + 15) && ((bow_x + 2) == (cupid.get_x() - 7)) &&
                (((bow_y - 1) == cupid.get_y() + 14) && ((bow_x + 2) == (cupid.get_x() - 7))) &&
                (((bow_y - 2) == cupid.get_y() + 13) && ((bow_x + 1) == (cupid.get_x() - 8)))) {
          return true;
        }
      }
        return false;                                 
    }

    //done
    bool check_collision_heart() { 
      if (level == 1) {
        for (int i = 0; i < 36; i++) {
          if ((enemies[i].get_x() - 4) <= 0) {
            return true;
          }
          else if ((((enemies[i].get_x() - 5) == bow.get_x() + 1) && ((enemies[i].get_y() + 1) == (bow.get_y() - 2))) &&
                  (((enemies[i].get_x() - 4) == bow.get_x() + 2) && ((enemies[i].get_y() + 2) == (bow.get_y() - 1)))) {
            return true;
          }
          else if ((((enemies[i].get_x() - 4) == bow.get_x() + 1) && ((enemies[i].get_y() + 2) == (bow.get_y() - 2))) &&
                (((enemies[i].get_x() - 3) == bow.get_x() + 2) && ((enemies[i].get_y() + 3) == (bow.get_y() - 1)))) {
            return true;
          }
          else if (((enemies[i].get_x() - 3) == bow.get_x() + 1) && ((enemies[i].get_y() + 3) == (bow.get_y() - 2))) {
            return true;
          }
          else if (((enemies[i].get_x() - 4) == bow.get_x() + 2) && ((enemies[i].get_y() == (bow.get_y() + 1))) &&
                (((enemies[i].get_x() - 5) == bow.get_x() + 1) && ((enemies[i].get_y() + 1) == (bow.get_y() + 2)))) {
            return true;
          }
          else if (((enemies[i].get_x() - 4) == bow.get_x() + 1) && ((enemies[i].get_y() == (bow.get_y() + 2)))) {
            return true;
          }
          else if (((enemies[i].get_x() - 5) == bow.get_x() + 2) && (enemies[i].get_y() + 1 == bow.get_y() - 1)) {
            return true;
          }
          else if (((enemies[i].get_x() - 5) == bow.get_x() + 2) && (enemies[i].get_y() + 1 == bow.get_y())) {
            return true;
          }
          else if (((enemies[i].get_x() - 5) == bow.get_x() + 2) && (enemies[i].get_y() + 1 == bow.get_y() + 1)) {
            return true;
          }
        }
        return false;
      }
      else if (level == 2) {
        for (int i = 36; i < 38; i++) {
          if ((enemies[i].get_x() - 4) <= 0) {
            return true;
          }
          else if ((((enemies[i].get_x() - 5) == bow.get_x() + 1) && ((enemies[i].get_y() + 1) == (bow.get_y() - 2))) &&
                  (((enemies[i].get_x() - 4) == bow.get_x() + 2) && ((enemies[i].get_y() + 2) == (bow.get_y() - 1)))) {
            return true;
          }
          else if ((((enemies[i].get_x() - 4) == bow.get_x() + 1) && ((enemies[i].get_y() + 2) == (bow.get_y() - 2))) &&
                (((enemies[i].get_x() - 3) == bow.get_x() + 2) && ((enemies[i].get_y() + 3) == (bow.get_y() - 1)))) {
            return true;
          }
          else if (((enemies[i].get_x() - 3) == bow.get_x() + 1) && ((enemies[i].get_y() + 3) == (bow.get_y() - 2))) {
            return true;
          }
          else if (((enemies[i].get_x() - 4) == bow.get_x() + 2) && ((enemies[i].get_y() == (bow.get_y() + 1))) &&
                (((enemies[i].get_x() - 5) == bow.get_x() + 1) && ((enemies[i].get_y() + 1) == (bow.get_y() + 2)))) {
            return true;
          }
          else if (((enemies[i].get_x() - 4) == bow.get_x() + 1) && ((enemies[i].get_y() == (bow.get_y() + 2)))) {
            return true;
          }
          else if (((enemies[i].get_x() - 5) == bow.get_x() + 2) && (enemies[i].get_y() + 1 == bow.get_y() - 1)) {
            return true;
          }
          else if (((enemies[i].get_x() - 5) == bow.get_x() + 2) && (enemies[i].get_y() + 1 == bow.get_y())) {
            return true;
          }
          else if (((enemies[i].get_x() - 5) == bow.get_x() + 2) && (enemies[i].get_y() + 1 == bow.get_y() + 1)) {
            return true;
          }
        }
        return false;
      }
    }

    //done
    bool check_collision_cupid_arrow(int cupid_arrow_x, int cupid_arrow_y) { 
      if (cupid.get_strength() > 0) { 
        if ((((cupid_arrow_y + 6) == (bow.get_y() - 2)) && ((cupid_arrow_x - 13) == (bow.get_x() + 1))) && 
            (((cupid_arrow_y + 7) == (bow.get_y() - 1)) && ((cupid_arrow_x - 11) == (bow.get_x() + 2)))) {   
          return true;
        }
        else if (((cupid_arrow_y + 7) == (bow.get_y() - 2)) && ((cupid_arrow_x - 12) == (bow.get_x() + 2))) { 
          return true;
        }
        else if ((((cupid_arrow_y + 5) == (bow.get_y() + 1)) && ((cupid_arrow_x - 12) == (bow.get_x() + 2))) && 
                (((cupid_arrow_y + 6) == (bow.get_y() + 2) ) && ((cupid_arrow_x - 13) == (bow.get_x() + 1)))) {
          return true;
        }          
        else if (((cupid_arrow_y + 5) == (bow.get_y() + 2)) && ((cupid_arrow_x - 12) == (bow.get_x() + 2))) { 
          return true;
        }
        else if (((cupid_arrow_y + 6) == (bow.get_y() + 1)) && ((cupid_arrow_x - 13) == (bow.get_x() + 2))) { 
          return true;
        }
        else if (((cupid_arrow_y + 6) == (bow.get_y() - 1)) && ((cupid_arrow_x - 13) == (bow.get_x() + 2))) { 
          return true;
        }
        else if (((cupid_arrow_y + 6) == bow.get_y()) && ((cupid_arrow_x - 13) == (bow.get_x() + 2))) { 
          return true;
        } 
          
      }
      return false;
    }


    // check if bow defeated all Hearts in current level
    bool level_cleared(int lvl) {
      if (lvl == 1) { 
        for (int i = 0; i < NUM_ENEMIES; i++) { 
          if (enemies[i].get_strength() > 0) { 
            return false; 
          }            
        }
      }
      else { 
        if (cupid.get_strength() > 0) { 
          return false; 
        }
      }
      return true;
    }

    // set up a level
    void reset_level() {
      if (bow.get_lives() > 0) {
        setupGame();
      }
    }

};

// a global variable that represents the game HeartBreakers
Game game;

// see https://www.arduino.cc/reference/en/language/structure/sketch/setup/
void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN_NUMBER, INPUT);
  matrix.begin();
  matrix.fillScreen(BLACK.to_333());
  matrix.setCursor(0, 0);
  initial_message();
  delay(3000);
  game.setupGame();
}

// see https://www.arduino.cc/reference/en/language/structure/sketch/loop/
void loop() {
  unsigned long timer = millis();  
  bool button_pressed;
  bool timeArrow;
  bool smooth_button = true;

  // smoothing fluctuating button_pressed values
  while ((millis() - timer) <= (1000/26)) {
    button_pressed = (digitalRead(BUTTON_PIN_NUMBER) == HIGH);
    if (!button_pressed) {
      smooth_button = false;
    }
  }
  
  int potentiometer_value = analogRead(POTENTIOMETER_PIN_NUMBER);
  game.update(potentiometer_value, smooth_button);
}

// displays Level
void print_level(int level) {
  x = 0;
  y = 0;
  //L
  matrix.drawPixel(x + 4, y + 1, WHITE.to_333());
  matrix.drawPixel(x + 4, y + 2, WHITE.to_333());
  matrix.drawPixel(x + 4, y + 3, WHITE.to_333());
  matrix.drawPixel(x + 4, y + 4, WHITE.to_333());
  matrix.drawPixel(x + 5, y + 4, WHITE.to_333());
  //V
  matrix.drawPixel(x + 7, y + 1, WHITE.to_333());
  matrix.drawPixel(x + 7, y + 2, WHITE.to_333());
  matrix.drawPixel(x + 7, y + 3, WHITE.to_333());
  matrix.drawPixel(x + 8, y + 4, WHITE.to_333());
  matrix.drawPixel(x + 9, y + 3, WHITE.to_333());
  matrix.drawPixel(x + 9, y + 2, WHITE.to_333());
  matrix.drawPixel(x + 9, y + 1, WHITE.to_333());
  //L
  matrix.drawPixel(x + 11, y + 1, WHITE.to_333());
  matrix.drawPixel(x + 11, y + 2, WHITE.to_333());
  matrix.drawPixel(x + 11, y + 3, WHITE.to_333());
  matrix.drawPixel(x + 11, y + 4, WHITE.to_333());
  matrix.drawPixel(x + 12, y + 4, WHITE.to_333());
  if (level == 1) {
    //1
    matrix.drawPixel(x + 6, y + 9, WHITE.to_333());
    matrix.drawPixel(x + 7, y + 8, WHITE.to_333());
    matrix.drawPixel(x + 8, y + 8, WHITE.to_333());
    matrix.drawPixel(x + 8, y + 9, WHITE.to_333());
    matrix.drawPixel(x + 8, y + 10, WHITE.to_333());
    matrix.drawPixel(x + 8, y + 11, WHITE.to_333());
    matrix.drawPixel(x + 8, y + 12, WHITE.to_333());
    matrix.drawPixel(x + 7, y + 12, WHITE.to_333());
    matrix.drawPixel(x + 6, y + 12, WHITE.to_333());
    matrix.drawPixel(x + 9, y + 12, WHITE.to_333());
    matrix.drawPixel(x + 10, y + 12, WHITE.to_333());
  }
  else if (level == 2) {
    matrix.drawPixel(x + 7, y + 8, BLACK.to_333());
    matrix.drawPixel(x + 6, y + 9, BLACK.to_333());
    matrix.drawPixel(x + 7, y + 10, BLACK.to_333());
    matrix.drawPixel(x + 8, y + 10, BLACK.to_333());
    matrix.drawPixel(x + 9, y + 11, BLACK.to_333());
    matrix.drawPixel(x + 8, y + 12, BLACK.to_333());
    matrix.drawPixel(x + 7, y + 13, BLACK.to_333());
    matrix.drawPixel(x + 8, y + 14, BLACK.to_333());
    matrix.fillScreen(BLACK.to_333());
    matrix.setTextColor(PINK.to_333());
    matrix.print("FINAL BOSS");
  }
}

// displays number of lives
void print_lives(int lives) {
  x = 0;
  y = 0;
  //L
  matrix.drawPixel(x + 2, y + 1, WHITE.to_333());
  matrix.drawPixel(x + 2, y + 2, WHITE.to_333());
  matrix.drawPixel(x + 2, y + 3, WHITE.to_333());
  matrix.drawPixel(x + 2, y + 4, WHITE.to_333());
  matrix.drawPixel(x + 3, y + 4, WHITE.to_333());
  //I
  matrix.drawPixel(x + 5, y + 1, WHITE.to_333());
  matrix.drawPixel(x + 5, y + 2, WHITE.to_333());
  matrix.drawPixel(x + 5, y + 3, WHITE.to_333());
  matrix.drawPixel(x + 5, y + 4, WHITE.to_333());
  //V
  matrix.drawPixel(x + 7, y + 1, WHITE.to_333());
  matrix.drawPixel(x + 7, y + 2, WHITE.to_333());
  matrix.drawPixel(x + 7, y + 3, WHITE.to_333());
  matrix.drawPixel(x + 8, y + 4, WHITE.to_333());
  matrix.drawPixel(x + 9, y + 3, WHITE.to_333());
  matrix.drawPixel(x + 9, y + 2, WHITE.to_333());
  matrix.drawPixel(x + 9, y + 1, WHITE.to_333());
  //E
  matrix.drawPixel(x + 11, y + 1, WHITE.to_333());
  matrix.drawPixel(x + 11, y + 2, WHITE.to_333());
  matrix.drawPixel(x + 11, y + 3, WHITE.to_333());
  matrix.drawPixel(x + 11, y + 4, WHITE.to_333());
  matrix.drawPixel(x + 12, y + 1, WHITE.to_333());
  matrix.drawPixel(x + 12, y + 2, WHITE.to_333());
  matrix.drawPixel(x + 12, y + 4, WHITE.to_333());
  //S
  matrix.drawPixel(x + 14, y + 1, WHITE.to_333());
  matrix.drawPixel(x + 14, y + 2, WHITE.to_333());
  matrix.drawPixel(x + 14, y + 4, WHITE.to_333());
  matrix.drawPixel(x + 15, y + 1, WHITE.to_333());
  matrix.drawPixel(x + 15, y + 3, WHITE.to_333());
  if (lives == 3) {
    matrix.drawPixel(x + 7, y + 9, WHITE.to_333());
    matrix.drawPixel(x + 8, y + 9, WHITE.to_333());
    matrix.drawPixel(x + 9, y + 10, WHITE.to_333());
    matrix.drawPixel(x + 8, y + 11, WHITE.to_333());
    matrix.drawPixel(x + 9, y + 12, WHITE.to_333());
    matrix.drawPixel(x + 8, y + 13, WHITE.to_333());
    matrix.drawPixel(x + 7, y + 13, WHITE.to_333());
  }
  if (lives == 2) {
    matrix.drawPixel(x + 6, y + 9, WHITE.to_333());
    matrix.drawPixel(x + 7, y + 8, WHITE.to_333());
    matrix.drawPixel(x + 8, y + 8, WHITE.to_333());
    matrix.drawPixel(x + 9, y + 9, WHITE.to_333());
    matrix.drawPixel(x + 8, y + 10, WHITE.to_333());
    matrix.drawPixel(x + 7, y + 11, WHITE.to_333());
    matrix.drawPixel(x + 6, y + 12, WHITE.to_333());
    matrix.drawPixel(x + 7, y + 12, WHITE.to_333());
    matrix.drawPixel(x + 8, y + 12, WHITE.to_333());
    matrix.drawPixel(x + 9, y + 12, WHITE.to_333());
  }
  if (lives == 1) {
    matrix.drawPixel(x + 6, y + 9, WHITE.to_333());
    matrix.drawPixel(x + 7, y + 8, WHITE.to_333());
    matrix.drawPixel(x + 8, y + 8, WHITE.to_333());
    matrix.drawPixel(x + 8, y + 9, WHITE.to_333());
    matrix.drawPixel(x + 8, y + 10, WHITE.to_333());
    matrix.drawPixel(x + 8, y + 11, WHITE.to_333());
    matrix.drawPixel(x + 8, y + 12, WHITE.to_333());
    matrix.drawPixel(x + 7, y + 12, WHITE.to_333());
    matrix.drawPixel(x + 6, y + 12, WHITE.to_333());
    matrix.drawPixel(x + 9, y + 12, WHITE.to_333());
    matrix.drawPixel(x + 10, y + 12, WHITE.to_333());
  }
}

// displays "game over"
void game_over() {
  String game_over = "game over     ";
  matrix.fillScreen(BLACK.to_333());
  matrix.setTextColor(RED.to_333());
  matrix.print(game_over);
}

//display "HEART BREAKER"
void initial_message() { 
  String initialString = "HEARTBREAKER"; 
  matrix.fillScreen(BLACK.to_333());
  matrix.setTextColor(PINK.to_333()); 
  matrix.print(initialString); 
}

void final_message() { 
  String finalString = "HEARTMAKER"; 
  matrix.fillScreen(BLACK.to_333());
  matrix.setTextColor(PINK.to_333()); 
  matrix.print(finalString); 
  delay(3000);
  bigHeart();
  delay(4000);
}
