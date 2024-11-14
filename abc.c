#include <LPC214x.H>
#include <stdio.h>

// Function to print to the UART0
void prt(char *arr)
{
	int i = 0;
	while(arr[i] != '\0')
	{
		while(!(U0LSR & 0x20));
		U0THR = arr[i];
		i++;
	}
	while(!(U0LSR & 0x20));
	U0THR = '\r';
}


// Function to receive a character via UART0
char UART0_RxChar(void) {
    while (!(U0LSR & 0x01));  // Wait until data is available in RBR
    return U0RBR;             // Return the received character
}


// Function to receive a string via UART0
void UART0_RxString(char *buffer, int maxLength) {
    int i = 0;
    char ch;
    while (i < maxLength - 1) {
        ch = UART0_RxChar();
        if (ch == '\r') {     // Stop receiving on Enter (carriage return)
            break;
        }
        buffer[i++] = ch;
    }
    buffer[i] = '\0';          // Null-terminate the string
}


// Function to encrypt using Caesar Cipher 
void caesar_encrypt(char *message, int shift) {
    int i=0;
    char ch;
    for (i = 0; message[i] != '\0'; ++i) {
        ch = message[i];
        if (ch >= 'A' && ch <= 'Z') {  // Uppercase letters
            ch = ch + shift;
            if (ch > 'Z') {
                ch = ch - 'Z' + 'A' - 1;
            }
            message[i] = ch;
        } else if (ch >= 'a' && ch <= 'z') {  // Lowercase letters
            ch = ch + shift;
            if (ch > 'z') {
                ch = ch - 'z' + 'a' - 1;
            }
            message[i] = ch;
        } else if (ch >= '0' && ch <= '9') {  // Digits
            ch = ch + shift;
            if (ch > '9') {
                ch = ch - '9' + '0' - 1;
            }
            message[i] = ch;
        }
    }
}


// Function to decrypt using Caesar Cipher 
void caesar_decrypt(char *message, int shift) {
    int i;
    char ch;
    for (i = 0; message[i] != '\0'; ++i) {
        ch = message[i];
        if (ch >= 'A' && ch <= 'Z') {  // Uppercase letters
            ch = ch - shift;
            if (ch < 'A') {
                ch = ch + 'Z' - 'A' + 1;
            }
            message[i] = ch;
        } else if (ch >= 'a' && ch <= 'z') {  // Lowercase letters
            ch = ch - shift;
            if (ch < 'a') {
                ch = ch + 'z' - 'a' + 1;
            }
            message[i] = ch;
        } else if (ch >= '0' && ch <= '9') {  // Digits
            ch = ch - shift;
            if (ch < '0') {
                ch = ch + '9' - '0' + 1;
            }
						else if (ch > '9') {
								ch = ch - '9' + '0' - 1;
						}
            message[i] = ch;
        }
    }
}


int main(void)
{
		while(1)
		{
		char n1[40] = "Enter your data: ";
		char n2[40] = "Encrypted data: ";
		char n3[40] = "Decrypted data: ";
		char n4[20] = " ";
		char message[40];
		int shift = 0;          // Initial shift value for Caesar Cipher
		char shift_str[5];
		int i=0;
		PINSEL0 = 0x00000005;
		U0LCR = 0x83;
		U0DLL = 97;
		U0LCR = 0x03;
			
		//data label
		prt(n1);
		UART0_RxString(message,40);
		prt(n4);
	
		while(message[i] != '\0')
		{
			//shift +=1;
			i+=1;
		}
		
		shift = i%26 + 1;				// Calculating the shift value
		/////////////////////////////////////////////////////////////
	
		///encrypt message
		caesar_encrypt(message, shift);
		prt(n2);
		prt(message);
		prt(n4);
	
		//Decrypt message
		caesar_decrypt(message, shift);
		prt(n3);
		prt(message);
		prt(n4);
		sprintf(shift_str, "Shift: %d", shift);
		prt(shift_str);
		prt(n4);
	}
}