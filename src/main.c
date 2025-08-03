#include <lpc214x.h>   // LPC214x standard header file for register definitions

// Define LCD Control Pins (connected to PORT0)
#define RS (1<<8)     // P0.8 - Register Select
#define EN (1<<9)     // P0.9 - Enable
#define D4 (1<<10)    // P0.10 - Data Line D4
#define D5 (1<<11)    // P0.11 - Data Line D5
#define D6 (1<<12)    // P0.12 - Data Line D6
#define D7 (1<<13)    // P0.13 - Data Line D7

// Define Push Button Pins
#define START (1<<14) // P0.14 - Start Button
#define STOP  (1<<15) // P0.15 - Stop Button
#define RESET (1<<16) // P0.16 - Reset Button

// Stopwatch Time Variables
volatile unsigned int ms = 0, sec = 0, min = 0; // Time counters
volatile unsigned int run = 0;  // Run flag: 1 = running, 0 = stopped

// Function Prototypes
void pll_init(void);
void delay(unsigned int);
void lcd_cmd(unsigned char);
void lcd_data(unsigned char);
void lcd_init(void);
void lcd_string(char *);
void lcd_display_time(void);

// Timer0 Interrupt Service Routine (ISR)
void __irq Timer0_ISR(void) {
    if (run) {                 // If stopwatch is running
        ms++;                  // Increment milliseconds
        if (ms >= 1000) {      // Convert to seconds after 1000 ms
            ms = 0;
            sec++;
        }
        if (sec >= 60) {       // Convert to minutes after 60 seconds
            sec = 0;
            min++;
        }
        lcd_display_time();    // Update LCD with current time
    }
    T0IR = 1;                  // Clear Timer0 interrupt flag
    VICVectAddr = 0;           // End of interrupt
}

int main() {
    pll_init();  // Initialize PLL (CPU Clock = 60MHz)

    // Configure LCD Control/Data pins as Output
    IO0DIR |= RS | EN | D4 | D5 | D6 | D7;

    // Configure Push Button pins as Input
    IO0DIR &= ~(START | STOP | RESET);

    lcd_init();                          // Initialize LCD
    lcd_string(" Stopwatch Ready ");     // Welcome message
    delay(2000);                         // 2-second delay

    // Timer0 Configuration for 1ms interrupts
    T0MR0 = 59999;        // Match Register for 1ms interval (60MHz PCLK)
    T0MCR = 3;            // Interrupt & Reset on MR0 match
    T0TCR = 1;            // Enable Timer0

    // Vectored Interrupt Controller (VIC) Configuration
    VICVectAddr0 = (unsigned long)Timer0_ISR;  // ISR address
    VICVectCntl0 = 0x20 | 4;                   // Enable slot 0 for Timer0 IRQ
    VICIntEnable = (1<<4);                     // Enable Timer0 interrupt globally

    while (1) {
        // Start Button Pressed (Active Low)
        if ((IO0PIN & START) == 0) {
            run = 1;  // Start the stopwatch
        }

        // Stop Button Pressed
        if ((IO0PIN & STOP) == 0) {
            run = 0;  // Stop the stopwatch
        }

        // Reset Button Pressed
        if ((IO0PIN & RESET) == 0) {
            run = 0;                 // Stop stopwatch before reset
            ms = sec = min = 0;      // Reset time counters
            lcd_display_time();      // Update LCD with reset time
        }
    }
}

// PLL Initialization Function
void pll_init(void) {
    PLL0CON = 0x01;                 // Enable PLL
    PLL0CFG = 0x24;                 // Configure M=5, P=2
    PLL0FEED = 0xAA;                // Feed sequence
    PLL0FEED = 0x55;

    while (!(PLL0STAT & 0x00000400)); // Wait for PLL Lock (PLOCK)

    PLL0CON = 0x03;                 // Connect PLL
    PLL0FEED = 0xAA;                // Feed sequence
    PLL0FEED = 0x55;

    VPBDIV = 0x01;                  // Set PCLK = CCLK (60MHz)
}

// Software Delay Function (approximate)
void delay(unsigned int x) {
    unsigned int i, j;
    for (i = 0; i < x; i++)
        for (j = 0; j < 6000; j++);
}

// Send Command to LCD (4-bit mode)
void lcd_cmd(unsigned char cmd) {
    IO0CLR = RS;                     // RS = 0 for command
    IO0CLR = D4 | D5 | D6 | D7;      // Clear Data Lines

    // Send High Nibble
    if (cmd & 0x10) IO0SET = D4;
    if (cmd & 0x20) IO0SET = D5;
    if (cmd & 0x40) IO0SET = D6;
    if (cmd & 0x80) IO0SET = D7;
    IO0SET = EN; delay(2); IO0CLR = EN;  // Enable Pulse

    IO0CLR = D4 | D5 | D6 | D7;      // Clear Data Lines

    // Send Low Nibble
    if (cmd & 0x01) IO0SET = D4;
    if (cmd & 0x02) IO0SET = D5;
    if (cmd & 0x04) IO0SET = D6;
    if (cmd & 0x08) IO0SET = D7;
    IO0SET = EN; delay(2); IO0CLR = EN;  // Enable Pulse
}

// Send Data to LCD (4-bit mode)
void lcd_data(unsigned char data) {
    IO0SET = RS;                     // RS = 1 for data
    IO0CLR = D4 | D5 | D6 | D7;      // Clear Data Lines

    // Send High Nibble
    if (data & 0x10) IO0SET = D4;
    if (data & 0x20) IO0SET = D5;
    if (data & 0x40) IO0SET = D6;
    if (data & 0x80) IO0SET = D7;
    IO0SET = EN; delay(2); IO0CLR = EN;  // Enable Pulse

    IO0CLR = D4 | D5 | D6 | D7;      // Clear Data Lines

    // Send Low Nibble
    if (data & 0x01) IO0SET = D4;
    if (data & 0x02) IO0SET = D5;
    if (data & 0x04) IO0SET = D6;
    if (data & 0x08) IO0SET = D7;
    IO0SET = EN; delay(2); IO0CLR = EN;  // Enable Pulse
}

// Initialize LCD in 4-bit Mode
void lcd_init(void) {
    lcd_cmd(0x02);  // 4-bit mode set
    lcd_cmd(0x28);  // Function Set: 2 Lines, 5x7 Matrix
    lcd_cmd(0x0C);  // Display ON, Cursor OFF
    lcd_cmd(0x06);  // Auto Increment Cursor
    lcd_cmd(0x01);  // Clear Display
}

// Display String on LCD
void lcd_string(char *ptr) {
    while (*ptr) {
        lcd_data(*ptr++);  // Send each character
    }
}

// Display Stopwatch Time on LCD
void lcd_display_time(void) {
    char buffer[16];
    lcd_cmd(0x80);  // Move Cursor to First Line
    sprintf(buffer, "Time %02d:%02d:%03d", min, sec, ms);  // Format Time String
    lcd_string(buffer);  // Display on LCD
}
