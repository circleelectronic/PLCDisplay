#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);  //sometimes the LCD adress is not 0x3f. Change to 0x27 if it dosn't work.

int counter = 0; 
int page=1;
int Ready=1;
int menu=0;
int submenu=0;
int last_counter = 0; 
bool clk_State;
bool Last_State; 
bool dt_State;  
int pushed = 0;
int buzzer = 6;
int var;

#define push 10
#define RELAY1 7 //out1 open
#define RELAY2 5 //out2 close
#define RELAY3 4 //out3 mode
#define RELAY4 3 //out4 time 

void openingMusic(){
  digitalWrite(buzzer, HIGH);
  delay(300);
  digitalWrite(buzzer, LOW);
  delay(200);
  digitalWrite(buzzer, HIGH);
  delay(100);
  digitalWrite(buzzer, LOW);
  delay(200);
  digitalWrite(buzzer, HIGH);
  delay(600);
  digitalWrite(buzzer, LOW);
}
void buzzersetup() {
  digitalWrite(buzzer, HIGH);
  delay(10);
  digitalWrite(buzzer, LOW);
  delay(40);
  digitalWrite(buzzer, HIGH);
  delay(60);
  digitalWrite(buzzer, LOW);
  delay(40);
  digitalWrite(buzzer, HIGH);
  delay(80);
  digitalWrite(buzzer, LOW);
  delay(40);
  digitalWrite(buzzer, HIGH);
  delay(120);
  digitalWrite(buzzer, LOW);
}
void buzzermenuclick() {
  digitalWrite(buzzer, HIGH);
  delay(40);
  digitalWrite(buzzer, LOW);
  delay(120);
  digitalWrite(buzzer, HIGH);
  delay(80);
  digitalWrite(buzzer, LOW);
}
void buzzermenuaction() {
  digitalWrite(buzzer, HIGH);
  delay(40);
  digitalWrite(buzzer, LOW);
}

void setup() {
  pinMode (push, OUTPUT);
  pinMode (buzzer, OUTPUT);
  pinMode (RELAY1, OUTPUT);
  pinMode (RELAY2, OUTPUT);
  pinMode (RELAY3, OUTPUT);
  pinMode (RELAY4, OUTPUT);
  
  lcd.init();                 //Init the LCD
  lcd.backlight();            //Activate backlight
  lcd.home();                 //Home the LCD
  
  PCICR |= (1 << PCIE0);    //enable PCMSK0 scan                                                 
  PCMSK0 |= (1 << PCINT0);  //Set pin D8 trigger an interrupt on state change. 
  PCMSK0 |= (1 << PCINT1);  //Set pin D9 trigger an interrupt on state change.  
  DDRB &= B11111100;        //8, 9 as input for the encoder clock and data pins

  Last_State =   (PINB & B00000001); //pin 8 state (clock pin)? 
  
  //Print the initial text. Delete these lines if you don't want that
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Circle Electronic");
  lcd.setCursor(0, 1);
  lcd.print(" PLC User Interface ");
  lcd.setCursor(11, 3);
  lcd.print("Ver 1.24");
  openingMusic();
  delay(1600);

  //Print the first page menu.
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Open the Gate");
  lcd.setCursor(0, 1);
  lcd.print("Close the Gate");
  lcd.setCursor(0, 2);
  lcd.print("Set Closing Time");
  lcd.setCursor(0, 3);
  lcd.print("Change the Mode");
}

void loop() { 
  
  if((last_counter > counter) || (last_counter < counter)  || pushed) //Only print on the LCD when a step is detected or the button is pushed.
  {
    Ready=1;
    if(menu == 0)//First page of the menu
    {  
     var=1;
        while(var==1){
          buzzermenuaction();
          break;
        }
        var=0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(">Open the Gate");
        lcd.setCursor(0, 1);
        lcd.print("Close the Gate");
        lcd.setCursor(0, 2);
        lcd.print("Set Closing Time");
        lcd.setCursor(0, 3);
        lcd.print("Change the Mode");
        page=1;
        if (pushed)
        {
          pushed = 0;
        }
      }
    
      if(5 < counter && counter < 10)
      {
       while(var==1){
          buzzermenuaction();
          break;
        }
        var=0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Open the Gate");
        lcd.setCursor(0, 1);
        lcd.print(">Close the Gate");
        lcd.setCursor(0, 2);
        lcd.print("Set Closing Time");
        lcd.setCursor(0, 3);
        lcd.print("Change the Mode");
        page=2;
        if(pushed)
        {
          pushed=0;
        }     
      }
      
      if(10 < counter && counter < 15)
      {
        while(var==1){
          buzzermenuaction();
          break;
        }
        var=0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Open the Gate");
        lcd.setCursor(0, 1);
        lcd.print("Close the Gate");
        lcd.setCursor(0, 2);
        lcd.print(">Set Closing Time");
        lcd.setCursor(0, 3);
        lcd.print("Change the Mode");
        page=3;
        if(pushed)
        {
          pushed=0;
        }       
      }
    
      if(15 < counter && counter < 20)
      {
        while(var==1){
          buzzermenuaction();
          break;
        }
        var=0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Open the Gate");
        lcd.setCursor(0, 1);
        lcd.print("Close the Gate");
        lcd.setCursor(0, 2);
        lcd.print("Set Closing Time");
        lcd.setCursor(0, 3);
        lcd.print(">Change the Mode");
        page=4;
        if(pushed)
        {
          pushed=0;
        }  
     }
  }//menu = 0;

    //Second page of the menu
    if(submenu == 1)
      {  
        if(0 <= counter && counter < 5)
        {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Are You Sure?");
        lcd.setCursor(0, 1);
        lcd.print(">Yes");
        lcd.setCursor(0, 2);
        lcd.print("No");       
        page = 1;
        pushed = 0;  
        }     
        if(5 < counter && counter < 10)
        {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Are You Sure?");
        lcd.setCursor(0, 1);
        lcd.print("Yes");
        lcd.setCursor(0, 2);
        lcd.print(">No");       
        page = 2;
        pushed = 0;                 
        }            
      }//submenu = 1;

    //Third page of the menu
    if(submenu == 2)
    {  
      if(0 <= counter && counter < 5)
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Are You Sure?");
        lcd.setCursor(0, 1);
        lcd.print(">Yes");
        lcd.setCursor(0, 2);
        lcd.print("No");       
        page = 1;
        pushed = 0;  
        }     
        if(5 < counter && counter < 10)
        {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Are You Sure?");
        lcd.setCursor(0, 1);
        lcd.print("Yes");
        lcd.setCursor(0, 2);
        lcd.print(">No");       
        page = 2;
        pushed = 0;               
      }
    }//submenu = 2;

    //Forth page of the menu
    if(submenu == 3)
    {  
       if (0 <= counter && counter < 5)
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(">2 Seconds");
        lcd.setCursor(0, 1);
        lcd.print("5 Seconds");
        lcd.setCursor(0, 2);
        lcd.print("10 Seconds");    
        lcd.setCursor(0, 3);
        lcd.print("20 Seconds");    
        page = 1;
        pushed = 0;
      }
      if (5 < counter && counter < 10)
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("2 Seconds");
        lcd.setCursor(0, 1);
        lcd.print(">5 Seconds");
        lcd.setCursor(0, 2);
        lcd.print("10 Seconds");   
        lcd.setCursor(0, 3);
        lcd.print("20 Seconds");    
        page = 2;
        pushed = 0;
      } 
      if (10 < counter && counter < 15)
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("2 Seconds");
        lcd.setCursor(0, 1);
        lcd.print("5 Seconds");
        lcd.setCursor(0, 2);
        lcd.print(">10 Seconds");    
        lcd.setCursor(0, 3);
        lcd.print("20 Seconds");    
        page = 3;
        pushed = 0;
      } 
      if (15 < counter && counter < 20)
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("2 Seconds");
        lcd.setCursor(0, 1);
        lcd.print("5 Seconds");
        lcd.setCursor(0, 2);
        lcd.print("10 Seconds");    
        lcd.setCursor(0, 3);
        lcd.print(">20 Seconds");    
        page = 4;
        pushed = 0;
      } 
    }
    //submenu = 3;
    
    if(submenu == 4)
    {  
      if (0 <= counter && counter < 5)
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(">Automatic Mode");
        lcd.setCursor(0, 1);
        lcd.print("Manuel Mode");     
        page = 1;
        pushed = 0;
      }
      if (5 < counter && counter < 10)
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Automatic Mode");
        lcd.setCursor(0, 1);
        lcd.print(">Manuel Mode");     
        page = 2;
        pushed = 0;
      } 
    }//submenu = 4;

   //end of the MENU prints on the LCD
 
 last_counter = counter; //Save the value of the last state

//Now we detect when we push the button
if(digitalRead(push))
  {    
     if(submenu == 1)
    {    
       if(page==1)
       {
         var=1;
        while(var==1){
          buzzersetup();
          break;
        }
        var=0;
        menu=0;
        submenu=0;
        counter=1;
        pushed=0;
        Ready=0;        
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Gate is Opening...");
        lcd.setCursor(0, 2);
        lcd.print("      Wait      ");
        digitalWrite(RELAY1, HIGH);
        delay(2000);
        digitalWrite(RELAY1, LOW);
        delay(4000);
       }
    
       if(page==2)
       {
        var=1;
        while(var==1){
          buzzermenuclick();
          break;
        }
        var=0;
        menu=0;                
        submenu=0;
        counter=0;
        pushed=0;
        Ready=0;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(" Going Back...  ");
        lcd.setCursor(0, 2);
        lcd.print("      Wait      ");
        delay(4000);
       }     
    }//end of submenu 1

    if(submenu == 2)
    {    
       if(page==1)
       {
         var=1;
        while(var==1){
          buzzersetup();
          break;
      }
        var=0;
        menu=0;
        submenu=0;
        counter=1;
        pushed=0;
        Ready=0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Gate is Closing");
        lcd.setCursor(0, 2);
        lcd.print("      Wait      ");
        digitalWrite(RELAY2, HIGH);
        delay(2000);
        digitalWrite(RELAY2, LOW);
        delay(4000);
       }
    
       if(page==2)
       {
        var=1;
        while(var==1){
          buzzermenuclick();
          break;
        }
        var=0;
        menu=0;
        submenu=0;
        counter=0;
        pushed=0;
        Ready=0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Going Back");
        lcd.setCursor(0, 2);
        lcd.print("      Wait      ");
        delay(4000);
       }    
    }//end of submenu 1

    if(submenu == 3)
    {    
       if(page==1)
       {
        var=1;
        while(var==1){
          buzzermenuclick();
          break;
        }
        menu=0;
        submenu=0;
        counter=1;
        pushed=0;
        Ready=0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Closing Time 2 Sec ");
        lcd.setCursor(0, 2);
        lcd.print("      Wait      ");
        digitalWrite(RELAY3, HIGH);
        delay(2000);
        digitalWrite(RELAY3, LOW);
        delay(4000);
       }
        if(page==2)
       {
        var=1;
        while(var==1){
          buzzermenuclick();
          break;
        }
        menu=0;
        submenu=0;
        counter=0;
        pushed=0;
        Ready=0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Closing Time 5 Sec ");
        lcd.setCursor(0, 2);
        lcd.print("      Wait      ");
        digitalWrite(RELAY3, HIGH);
        delay(5000);
        digitalWrite(RELAY3, LOW);
        delay(4000);
       }    
       if(page==3)
       {
        var=1;
        while(var==1){
          buzzermenuclick();
          break;
        }
        menu=0;
        submenu=0;
        counter=0;
        pushed=0;
        Ready=0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Closing Time 10 Sec ");
        lcd.setCursor(0, 2);
        lcd.print("      Wait      ");
        digitalWrite(RELAY3, HIGH);
        delay(10000);
        digitalWrite(RELAY3, LOW);
        delay(4000);
       }
       if(page==4)
       {
        var=1;
        while(var==1){
          buzzermenuclick();
          break;
        }
        menu=0;
        submenu=0;
        counter=0;
        pushed=0;
        Ready=0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Closing Time 20 Sec ");
        lcd.setCursor(0, 2);
        lcd.print("      Wait      ");
        digitalWrite(RELAY3, HIGH);
        delay(20000);
        digitalWrite(RELAY3, LOW);
        delay(4000);
       }           
    }//end of submenu 1
    
    if(submenu == 4)
    {    
       if(page==1)
       {
        var=1;
        while(var==1){
          buzzermenuclick();
          break;
        }
        menu=0;
        submenu=0;
        counter=0;
        pushed=0;
        Ready=0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Automatic Mode");
        lcd.setCursor(0, 2);
        lcd.print("      Wait      ");
        digitalWrite(RELAY4, HIGH);      
        delay(4000);
       }
    
       if(page==2)
       {
        var=1;
        while(var==1){
          buzzermenuclick();
          break;
        }
        menu=0;
        submenu=0;
        counter=0;
        pushed=0;
        Ready=0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Manuel Mode");
        lcd.setCursor(0, 2);
        lcd.print("      Wait      ");
        digitalWrite(RELAY4, LOW);      
        delay(4000);
       }        
    }//end of submenu 1
   if(menu == 0 && Ready==1)
    {    
       if(page==1)
       {
        var=1;
        while(var==1){
        buzzermenuclick();
        break;
        }
        var=0;
        submenu=1;
        counter=0;
        pushed=1;
        delay(500);       
       }
    
       if(page==2)
       {
        var=1;
        while(var==1){
        buzzermenuclick();
        break;
        }
        var=0;
        submenu=2;
        counter=0;
        pushed=1;
        delay(500);
       }
    
       if(page==3)
       {
        var=1;
        while(var==1){
        buzzermenuclick();
        break;
        }
        var=0;
        submenu=3;
        counter=0;
        pushed=1
        ;delay(500);
       }
    
       if(page==4)
       {
        var=1;
        while(var==1){
        buzzermenuclick();
        break;
        }
        var=0;
        submenu=4;
        counter=0;
        pushed=1;
        delay(500);
       }
    }//end of menu 0
    
  }


  //Add limit for the counter. Each line of the menu has 5 points. Since my menu has 4 lines the maximum counter will be from 0 to 20
  //If you add more lines for the menu, increase this value
  if(counter > 20)
  {
    counter=20;
  }
  if(counter < 0)
  {
    counter=0;
  }
}//end void



//Interruption vector

ISR(PCINT0_vect){
  
  clk_State =   (PINB & B00000001); //pin 8 state, clock pin? 
  dt_State  =   (PINB & B00000010); 
  if (clk_State != Last_State){     
     // If the data state is different to the clock state, that means the encoder is rotating clockwise
     if (dt_State != clk_State) { 
       counter ++;
     }
     else {
       counter --;
     } 
   } 
   Last_State = clk_State; // Updates the previous state of the data with the current state
  
}
