#include<reg51.h>
#include<string.h>
#include<stdio.h>
#define lcdport P1
sbit col1 = P2 ^ 0;
sbit col2 = P2 ^ 1;
sbit col3 = P2 ^ 2;
sbit col4 = P2 ^ 3;
sbit row1 = P2 ^ 4;
sbit row2 = P2 ^ 5;
sbit row3 = P2 ^ 6;
sbit row4 = P2 ^ 7;
sbit rs = P1 ^ 0;
sbit rw = P1 ^ 1;
sbit en = P1 ^ 2;
sbit m1 = P3 ^ 1;
sbit m2 = P3 ^ 2;
sbit buzzer = P3 ^ 3;
char i, rx_data[50];
char rfid[12], ch = 0;
char pass[4];

void delay(int m) {
   int i, j;
   for ( i = 0; i < m; i++) {
      for ( j = 0; j < 1275; j++) {
      }
   }
}
void dataen() {
   rs = 1;
   rw = 0;
   en = 1;
   delay(10);
   en = 0;
}

void lcddata(unsigned char ch) {
   lcdport = ch & 0xf0;
   dataen();
   lcdport = (ch << 4) & 0xf0;
   dataen();
}

void command()
{
   rs = 0;
   rw = 0;
   en = 1;
   delay(10);
   en = 0;
}

void lcdcmd(unsigned char ch ) {
   lcdport = ch & 0xf0;
   command();
   lcdport = (ch << 4) & 0xf0;
   command();
}

void lcdstring(char *str)
{
   while (*str) {
      lcddata(*str);
      str++;
   }
}
void lcd_init(void)
{  delay(10);
   lcdcmd(0x02);
   delay(10);
   lcdcmd(0x28);
   delay(10);
   lcdcmd(0x0e);
   delay(10);
   lcdcmd(0x01);
   delay(10);
}

void uart() {
   TMOD = 0x20;
   SCON = 0x50;
   TH1 = 0xfd;
   TR1 = 1;
}

char rxdata()
{
   while (!RI);
   ch = SBUF;
   RI = 0;
   return ch;
}
void keypad()

{

   lcdcmd(1);
   lcdstring("Enter Ur Passkey");
   lcdcmd(0xc0);
   i = 0;
   while (i < 4)
   {
      col1 = 0;
      col2 = col3 = col4 = 1;
      if (!row1)
      {
         lcddata('1');
         pass[i++] = '1';
         while (!row1);
      }
      else if (!row2)
      {
         lcddata('4');
         pass[i++] = '4';
         while (!row2);
      }
      else if (!row3)
      {
         lcddata('7');
         pass[i++] = '7';
         while (!row3);
      }
      else if (!row4)
      {
         lcddata('*');
         pass[i++] = '*';
         while (!row4);
      }
      col2 = 0;
      col1 = col3 = col4 = 1;
      if (!row1)
      {
         lcddata('2');
         pass[i++] = '2';
         while (!row1);
      }
      else if (!row2)
      {
         lcddata('5');
         pass[i++] = '5';
         while (!row2);
      }
      else if (!row3)
      {
         lcddata('8');
         pass[i++] = '8';
         while (!row3);
      }
      else if (!row4)
      {
         lcddata('0');
         pass[i++] = '0';
         while (!row4);
      }
      col3 = 0;
      col1 = col2 = col4 = 1;
      if (!row1)
      {
         lcddata('3');
         pass[i++] = '3';
         while (!row1);
      }
      else if (!row2)
      {
         lcddata('6');
         pass[i++] = '6';
         while (!row2);
      }
      else if (!row3)
      {
         lcddata('9');
         pass[i++] = '9';
         while (!row3);
      }
      else if (!row4)
      {
         lcddata('#');
         pass[i++] = '#';
         while (!row4);
      }
      col4 = 0;
      col1 = col3 = col2 = 1;
      if (!row1)
      {
         lcddata('A');
         pass[i++] = 'A';
         while (!row1);
      }
      else if (!row2)
      {
         lcddata('B');
         pass[i++] = 'B';
         while (!row2);
      }
      else if (!row3)
      {
         lcddata('C');
         pass[i++] = 'C';
         while (!row3);
      }
      else if (!row4)
      {
         lcddata('D');
         pass[i++] = 'D';
         while (!row4);
      }
   }
}


void accept()
{
   lcdcmd(1);
   lcdstring("Welcome");
   lcdcmd(192);
   lcdstring("Password accept");
   delay(200);
}

void wrong() {
   buzzer = 0;
   lcdcmd(1);
   lcdstring("Wrong Passkey");
   lcdcmd(192);
   lcdstring("PLZ Try Again");
   delay(1000);
   buzzer = 1;
}

void main()

{
   buzzer = 1;
   uart();
   lcd_init();
   lcdstring("  RFID Based    ");
   lcdcmd(0xc0);
   lcdstring("Security system ");
   delay(400);
   while (1)

   {
      lcdcmd(1);
      lcdstring("Place Your Card:");
      lcdcmd(0xc0);
      i = 0;
      for (i = 0; i < 12; i++) {
         rfid[i] = rxdata();
      }
      rfid[i] = '\0';
      lcdcmd(1);
      lcdstring("Your ID No. is:");
      lcdcmd(0xc0);
      for (i = 0; i < 12; i++) {
         lcddata(rfid[i]);
      }

      delay(100);
      if (strncmp(rfid, "3E00EA7CAE06", 12) == 0)
      {
         keypad();
         if (strncmp(pass, "1473", 4) == 0)
         {
            accept();
            lcdcmd(1);
            lcdstring("Access Granted ");
            lcdcmd(0xc0);
            lcdstring("Person1");
            m1 = 1;
            m2 = 0;
            delay(300);
            m1 = 0;
            m2 = 0;
            delay(200);
            m1 = 0;
            m2 = 1;
            delay(300);
            m1 = 0;
            m2 = 0;
         }
         else
            wrong();
      }
      else if (strncmp(rfid, "3E00EA7CC961", 12) == 0)
      {
         keypad();
         if (strncmp(pass, "1369", 4) == 0)
         {
            accept();
            lcdcmd(1);
            lcdstring("Access Granted ");
            lcdcmd(0xc0);
            lcdstring("Person2");
            m1 = 1;
            m2 = 0;
            delay(300);
            m1 = 0;
            m2 = 0;
            delay(200);
            m1 = 0;
            m2 = 1;
            delay(300);
            m1 = 0;
            m2 = 0;
         }
         else
            wrong();
      }
      else if (strncmp(rfid, "3E00EA7CAD05", 12) == 0)
      {
         keypad();
         if (strncmp(pass, "7413", 4) == 0)
         {
            accept();
            lcdcmd(1);
            lcdstring("Access Granted ");
            lcdcmd(0xc0);
            lcdstring("Person3");
            m1 = 1;
            m2 = 0;
            delay(300);
            m1 = 0;
            m2 = 0;
            delay(200);
            m1 = 0;
            m2 = 1;
            delay(300);
            m1 = 0;
            m2 = 0;
         }
         else
            wrong();
      }
      else
      {
         lcdcmd(1);
         lcdstring("Access Denied");
         buzzer = 0;
         delay(100);
         buzzer = 1;
      }
   }
}