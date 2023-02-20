#include <mega32.h>
#include <delay.h>
#include <alcd.h>
#include <string.h>
#include <stdlib.h>
#define dp 5

int i, j, row, len = 0;
char display_output[80];
float first_num = 0, second_num = 0, result_flag = 0;
int operation = 0;

char number_data[][10] = {
  "7", "4", "1", "CLEAR",
  "8", "5", "2", "0",
  "9", "6", "3", "=",
  "/", "x",  "-", "+"
};

int numbers[11] = {7, 4, 1, 0, 8, 5, 2, 0, 9, 6, 3};

void display(int n){

    switch(n) {
        case 0:
        case 1:
        case 2:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
            if(!operation) {
                if(!len) {
                    first_num = numbers[n];
                }
                else {
                    if(!result_flag) {
                        first_num = (first_num * 10) + numbers[n];
                    }
                    else {
                        strcpy(display_output, "");
                        len = 0;
                        operation = 0;
                        first_num = numbers[n];
                        second_num = 0;
                        result_flag = 0;    
                    }
                }
            }
            else{
                if(!len) {
                    second_num = numbers[n];
                }
                else {
                    second_num = (second_num * 10) + numbers[n]; 
                }
            }
            len++;
            strcat(display_output, number_data[n]);
            break;
        case 11:
            if(len) {
                if(operation) {
                    switch(operation) {
                        case 15:
                            first_num = first_num + second_num;
                            break;
                        case 14:
                            first_num = first_num - second_num;
                            break;
                        case 13:
                            first_num = first_num * second_num;
                            break;
                        case 12:
                            first_num = first_num / second_num;
                            break;
                    }
                    result_flag = 1;
                }
                operation = 0;
                strcpy(display_output, "");
                ftoa(first_num, dp, display_output);
            }
            break;
        case 3:
            strcpy(display_output, "");
            len = 0;
            operation = 0;
            first_num = 0;
            second_num = 0;
            break;
        case 12:
        case 13:
        case 14:
        case 15:
            if(len) {
                strcat(display_output, number_data[n]);
                operation = n;
                len = 0;
            }
            break;
    }
    
    lcd_clear();
    lcd_gotoxy(0, 0);
    lcd_puts(display_output);
}

void data_input(void) {
  PORTB = 0b11101111;
  for (i = 0; i < 4; i++) {
    row = 0b11111110;
    for (j = 0; j < 4; j++) {
      if (PINB == (row & PORTB)) {
        display(4 * i + j);
      }
      row = ~(~row << 1);
    }
    PORTB = ~(~PORTB << 1);
  }
  delay_ms(200);
}

void main(void) {

    DDRB = 0xF0;
    lcd_init(16);      
    lcd_printf("WELCOME!");    
    delay_ms(1000);    
    lcd_clear();    
    lcd_gotoxy(0, 0);
    lcd_printf("PLEASE ENTER THE EXPRESSION");
    
    while (1) {  
        data_input();  
    }
}