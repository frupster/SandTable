#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

const int ROW_NUM    = 4; // four rows
const int COLUMN_NUM = 4; // four columns

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};

byte pin_rows[ROW_NUM] = {9, 8, 7, 6};      
byte pin_column[COLUMN_NUM] = {5, 4, 3, 2}; 

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );
LiquidCrystal_I2C lcd(0x27, 16, 2); 

int column = 0;

void setup(){
  lcd.init(); 
  lcd.backlight();
}

void loop(){
  char key = keypad.getKey();

  if (key) {
    lcd.setCursor(column, 0); 
    lcd.print(key);                 

    column++;                  
    if(column == 5) {       
      lcd.clear();
      column = 0;
    }
  }
}
