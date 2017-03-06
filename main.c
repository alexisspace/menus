/* Tigershark example menus*/


#include <msp430f5529.h>
#include "grlib.h"
#include "button.h"
#include "imageButton.h"
#include "radioButton.h"
#include "checkbox.h"
#include "driverlib.h"
#include "touch_F5529LP.h"
#include "LcdDriver/ILI9341_Driver.h"
#include "Images/images.h"
#include <math.h>

// Colors for graphics
#define TITLE_COLOR    GRAPHICS_COLOR_DARK_ORANGE//GRAPHICS_COLOR_AQUA//GRAPHICS_COLOR_FOREST_GREEN
#define BAR_COLOR       GRAPHICS_COLOR_DARK_ORANGE//GRAPHICS_COLOR_AQUA //GRAPHICS_COLOR_FOREST_GREEN
#define DATA_COLOR      GRAPHICS_COLOR_WHITE
#define NORMAL_COLOR    GRAPHICS_COLOR_FOREST_GREEN
#define WARN_COLOR      GRAPHICS_COLOR_YELLOW//GRAPHICS_COLOR_ORANGE
#define DANGER_COLOR    GRAPHICS_COLOR_RED

// Main Menus Colors
#define MMENU_TITLE_COLOR               GRAPHICS_COLOR_BLACK//GRAPHICS_COLOR_WHITE
#define MMENU_BACKGROUND_COLOR          GRAPHICS_COLOR_BLACK//0x00C5D1D7
#define MMENU_TITLE_BACKGROUND_COLOR    GRAPHICS_COLOR_DARK_ORANGE//GRAPHICS_COLOR_DARK_RED
#define MMENU_BOTTOM_BAR_COLOR          GRAPHICS_COLOR_DARK_ORANGE//GRAPHICS_COLOR_DARK_RED//0x008CADAE//GRAPHICS_COLOR_DARK_GRAY//GRAPHICS_COLOR_LIGHT_SEA_GREEN
#define MMENU_LEFT_BAR_COLOR            GRAPHICS_COLOR_DARK_RED
#define MMENU_DATA_FONT_COLOR           GRAPHICS_COLOR_WHITE

// Screen position constants
#define CONTEXT_MENU_BAR_POS        122
#define CONTEXT_TOTAL_SPACE         115


//Touch screen context
touch_context g_sTouchContext;
Graphics_ImageButton imageButton;
Graphics_Button yesButton;

// Graphic library context
Graphics_Context g_sContext;

//Flag to know if a demo was run
bool g_ranDemo = false;

void Delay();
void boardInit(void);
void clockInit(void);
void timerInit(void);
void initializeDemoButtons(void);
void drawInitScreen(void);
void drawMainView(void);
void drawSystemSetupMenu(void);
void drawInitContexMenu(void);
void drawContextMenu(void);
void DelayCycles(unsigned int k);
void runPrimitivesDemo(void);
void runImagesDemo(void);
void drawRestarDemo(void);

#if defined(__IAR_SYSTEMS_ICC__)
int16_t __low_level_init(void) {
    // Stop WDT (Watch Dog Timer)
    WDTCTL = WDTPW + WDTHOLD;
    return(1);
}

#endif

void main(void)
{
    // Initialization routines
    boardInit();
    clockInit();
    timerInit();
    initializeDemoButtons();

    // LCD setup using Graphics Library API calls
    ILI9341_DriverInit();
    Graphics_initContext(&g_sContext, &g_sILI9341_Driver);

    drawInitScreen();
    Delay();

    // Example main menu
    drawSystemSetupMenu();
    Delay();

    //Enter main view
    drawMainView();
    Delay();

    // Effect for drawing context menu
    drawInitContexMenu();

    //runImagesDemo();
    //runPrimitivesDemo();
    while(1){
        _nop();
    }	// End program here

}

void drawInitScreen(void)
{
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(&g_sContext, &g_sFontCmss20b);
    Graphics_clearDisplay(&g_sContext);

    // Draw welcome logo
    Graphics_drawImage(&g_sContext, &jmi_logo4BPP_COMP_RLE4, 100,10);

    Graphics_drawStringCentered(&g_sContext, "Juergensen Marine Inc.",
                                AUTO_STRING_LENGTH,
                                159,
                                140,
                                GRAPHICS_OPAQUE_TEXT);

}


void drawMainView(void)
{
    Graphics_Rectangle hBar = { 0, 0, 319, 2}; // 10 pixel width horizontal bar

    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setForegroundColor(&g_sContext, TITLE_COLOR);
    Graphics_setFont(&g_sContext, &g_sFontCmss16b);
    Graphics_clearDisplay(&g_sContext);

/*
    // Status information area
    Graphics_drawString(&g_sContext, "Status: ",
                                AUTO_STRING_LENGTH, 0, 0, GRAPHICS_TRANSPARENT_TEXT);
*/

    // First line of information
    Graphics_setFont(&g_sContext, &g_sFontCmss18b);
    Graphics_drawStringCentered(&g_sContext, "Surface Time",
                                AUTO_STRING_LENGTH, 53, 9, GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, "Dive Time",
                                AUTO_STRING_LENGTH, 155, 9, GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, "Depth",
                                AUTO_STRING_LENGTH, 255, 9, GRAPHICS_TRANSPARENT_TEXT);

    // First line data
    Graphics_setForegroundColor(&g_sContext, DATA_COLOR);
    Graphics_setFont(&g_sContext, &g_sFontCmss22b);
    Graphics_drawStringCentered(&g_sContext, "999:99",
                                AUTO_STRING_LENGTH, 53, 29, GRAPHICS_TRANSPARENT_TEXT);

    Graphics_setFont(&g_sContext, &g_sFontCmss28b);
    Graphics_drawStringCentered(&g_sContext, "99:99",
                                AUTO_STRING_LENGTH, 155, 32, GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, "9999",
                                AUTO_STRING_LENGTH, 255, 32, GRAPHICS_TRANSPARENT_TEXT);

    Graphics_setForegroundColor(&g_sContext, TITLE_COLOR);
    Graphics_setFont(&g_sContext, &g_sFontCmss18b);
    Graphics_drawString(&g_sContext, "ft",
                                AUTO_STRING_LENGTH, 290, 28, GRAPHICS_TRANSPARENT_TEXT);

    // ppO2 sensors readings
    Graphics_setForegroundColor(&g_sContext, BAR_COLOR);
    hBar.yMin = 55; //
    hBar.yMax = 57;
    Graphics_fillRectangle(&g_sContext, &hBar);

    Graphics_setForegroundColor(&g_sContext, TITLE_COLOR);
    Graphics_setFont(&g_sContext, &g_sFontCmss18b);
    Graphics_drawStringCentered(&g_sContext, "ppO2 Readings",
                                AUTO_STRING_LENGTH, 150, 70, GRAPHICS_TRANSPARENT_TEXT);

    Graphics_setForegroundColor(&g_sContext, NORMAL_COLOR);
    Graphics_setFont(&g_sContext, &g_sFontCmss44b);
    Graphics_drawStringCentered(&g_sContext, "1.00",
                                AUTO_STRING_LENGTH, 48, 100, GRAPHICS_TRANSPARENT_TEXT);

    Graphics_setForegroundColor(&g_sContext, WARN_COLOR);
    Graphics_drawStringCentered(&g_sContext, "1.00",
                                AUTO_STRING_LENGTH, 156, 100, GRAPHICS_TRANSPARENT_TEXT);
    Graphics_setForegroundColor(&g_sContext, DANGER_COLOR);
    Graphics_drawStringCentered(&g_sContext, "1.00",
                                AUTO_STRING_LENGTH, 262, 100, GRAPHICS_TRANSPARENT_TEXT);

    Graphics_setForegroundColor(&g_sContext, BAR_COLOR);
    hBar.yMin = 122; //
    hBar.yMax = 124;
    Graphics_fillRectangle(&g_sContext, &hBar);

    // Middle space
    Graphics_setForegroundColor(&g_sContext, TITLE_COLOR);
    Graphics_setFont(&g_sContext, &g_sFontCmss18b);
    Graphics_drawStringCentered(&g_sContext, "Max Depth",
                                AUTO_STRING_LENGTH, 45, 135, GRAPHICS_TRANSPARENT_TEXT);
    Graphics_setForegroundColor(&g_sContext, DATA_COLOR);
    Graphics_setFont(&g_sContext, &g_sFontCmss28b);
    Graphics_drawStringCentered(&g_sContext, "9999",
                                AUTO_STRING_LENGTH, 35, 158, GRAPHICS_TRANSPARENT_TEXT);
    Graphics_setForegroundColor(&g_sContext, TITLE_COLOR);
    Graphics_setFont(&g_sContext, &g_sFontCmss18b);
    Graphics_drawString(&g_sContext, "ft",
                                AUTO_STRING_LENGTH, 70, 154, GRAPHICS_TRANSPARENT_TEXT);

    // Third line of information
    Graphics_setForegroundColor(&g_sContext, DATA_COLOR);
    Graphics_setFont(&g_sContext, &g_sFontCmss18b);
    Graphics_drawStringCentered(&g_sContext, "-0.4-",
                                AUTO_STRING_LENGTH, 90, 202, GRAPHICS_TRANSPARENT_TEXT);

    Graphics_setForegroundColor(&g_sContext, TITLE_COLOR);
    Graphics_drawStringCentered(&g_sContext, "Batt.",
                                AUTO_STRING_LENGTH, 20, 202, GRAPHICS_TRANSPARENT_TEXT);

    Graphics_drawStringCentered(&g_sContext, "TTS",
                                AUTO_STRING_LENGTH, 160, 202, GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, "NO STOP",
                                AUTO_STRING_LENGTH, 234, 202, GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, "NDL",
                                AUTO_STRING_LENGTH, 298, 202, GRAPHICS_TRANSPARENT_TEXT);

    // Third line of data
    Graphics_setForegroundColor(&g_sContext, DATA_COLOR);
    Graphics_setFont(&g_sContext, &g_sFontCmss22b);
    Graphics_drawStringCentered(&g_sContext, " 1.5",
                                AUTO_STRING_LENGTH, 20, 223, GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, "999° C",
                                AUTO_STRING_LENGTH, 90, 223, GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, "999",
                                AUTO_STRING_LENGTH, 160, 223, GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, "9@999",
                                AUTO_STRING_LENGTH, 234, 223, GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, "5",
                                AUTO_STRING_LENGTH, 298, 223, GRAPHICS_TRANSPARENT_TEXT);

    // Bottom bar
    Graphics_setForegroundColor(&g_sContext, BAR_COLOR);
    hBar.yMin = 237; //
    hBar.yMax = 239;
    Graphics_fillRectangle(&g_sContext, &hBar);

/*
    Graphics_drawLineH(&g_sContext, 0, 339, 237);
    Graphics_drawLineH(&g_sContext, 0, 339, 238);
    Graphics_drawLineH(&g_sContext, 0, 339, 239);
*/



}
void drawSystemSetupMenu(void)
{
    Graphics_Rectangle rectangle1 = { 0, 0, 319, 239};


    //********** Draw outline **************************************
    Graphics_setForegroundColor(&g_sContext, MMENU_BACKGROUND_COLOR);
    Graphics_setBackgroundColor(&g_sContext, MMENU_BACKGROUND_COLOR);

    // Draw background
    Graphics_clearDisplay(&g_sContext);

    // Draw screen border
    //Graphics_drawRectangle (&g_sContext, &rectangle1);

    // Draw title rectangle
    rectangle1.xMin = 0;
    rectangle1.xMax = 319;
    rectangle1.yMin = 0;
    rectangle1.yMax = 54;
    Graphics_setForegroundColor(&g_sContext, MMENU_TITLE_BACKGROUND_COLOR);
    Graphics_fillRectangle (&g_sContext, &rectangle1);

    // Draw title line
    Graphics_setForegroundColor(&g_sContext, MMENU_BACKGROUND_COLOR);
    Graphics_drawLineH (&g_sContext, 0, 319, 50);
/*
    // Draw left rectangle
    rectangle1.xMin = 0;
    rectangle1.xMax = 9;
    rectangle1.yMin = 55;
    rectangle1.yMax = 209;
    Graphics_setForegroundColor(&g_sContext, MMENU_LEFT_BAR_COLOR);
    Graphics_fillRectangle (&g_sContext, &rectangle1);
*/

    // Draw bottom rectangle
    rectangle1.xMin = 0;
    rectangle1.xMax = 319;
    rectangle1.yMin = 210;
    rectangle1.yMax = 239;
    Graphics_setForegroundColor(&g_sContext, MMENU_BOTTOM_BAR_COLOR);
    Graphics_fillRectangle (&g_sContext, &rectangle1);

    // Draw background center circle
    Graphics_setForegroundColor(&g_sContext, MMENU_TITLE_BACKGROUND_COLOR);
    Graphics_fillCircle (&g_sContext, 160, 50, 14);
    // Draw outer center circle
    Graphics_setForegroundColor(&g_sContext, MMENU_BACKGROUND_COLOR);
    Graphics_drawCircle (&g_sContext, 160, 50, 12);
    // Draw inner center circle
    Graphics_drawCircle (&g_sContext, 160, 50, 10);

    //********** Draw information **************************************

    // Switch which menu is being drawing
    // Menu Title
    Graphics_setForegroundColor(&g_sContext, MMENU_TITLE_COLOR);
    Graphics_setFont(&g_sContext, &g_sFontCmss30b);
    Graphics_drawStringCentered(&g_sContext, "System Setup",
                                AUTO_STRING_LENGTH, 160, 19, GRAPHICS_TRANSPARENT_TEXT);

    // Menu Information
    Graphics_setForegroundColor(&g_sContext, MMENU_DATA_FONT_COLOR);
    Graphics_setFont(&g_sContext, &g_sFontCmss20b);
    Graphics_drawString(&g_sContext, "Circuit Mode",
                                AUTO_STRING_LENGTH, 25, 64, GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext, "Solenoid Mode",
                                AUTO_STRING_LENGTH, 25, 88, GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext, "Units Mode",
                                AUTO_STRING_LENGTH, 25, 112, GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext, "Controller Mode",
                                AUTO_STRING_LENGTH, 25, 136, GRAPHICS_TRANSPARENT_TEXT);

    // Menu active item
    Graphics_setForegroundColor(&g_sContext, MMENU_DATA_FONT_COLOR);
    Graphics_fillCircle (&g_sContext, 17, 98, 4);

    // Menu Data
    Graphics_setForegroundColor(&g_sContext, MMENU_DATA_FONT_COLOR);
    Graphics_drawString(&g_sContext, "CC",
                                AUTO_STRING_LENGTH, 200, 64, GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext, "Juergensen",
                                AUTO_STRING_LENGTH, 200, 88, GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext, "Imperial",
                                AUTO_STRING_LENGTH, 200, 112, GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext, "Primary",
                                AUTO_STRING_LENGTH, 200, 136, GRAPHICS_TRANSPARENT_TEXT);
    // Underline active data
    rectangle1.xMin = 200;
    rectangle1.xMax = 310;
    rectangle1.yMin = 108;
    rectangle1.yMax = 110;
    Graphics_setForegroundColor(&g_sContext, MMENU_DATA_FONT_COLOR);
    Graphics_fillRectangle (&g_sContext, &rectangle1);

    // Draw button function
    Graphics_setForegroundColor(&g_sContext, MMENU_TITLE_COLOR);
    Graphics_drawStringCentered(&g_sContext, "Next",
                                AUTO_STRING_LENGTH, 35, 225, GRAPHICS_TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, "Select",
                                AUTO_STRING_LENGTH, 285, 225, GRAPHICS_TRANSPARENT_TEXT);


}

void drawInitContexMenu(void)
{
    unsigned int k, k_delay;
    // Bottom bar raising effect
    for(k = 1; k <= CONTEXT_TOTAL_SPACE; k++){
        Graphics_setForegroundColor(&g_sContext, BAR_COLOR);
        Graphics_drawLineH (&g_sContext, 0, 319, 237 - k);
        Graphics_drawLineH (&g_sContext, 0, 319, 238 - k);
        Graphics_drawLineH (&g_sContext, 0, 319, 239 - k);
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_drawLineH (&g_sContext, 0, 319, 240 - k);
        // Apply exponential delay at the end
        if(k > 103){
            k_delay =  exp(-0.721*k) - 1;
        }else{
            k_delay = k;
        }
        DelayCycles(k_delay);
    }
}

void drawContextMenu(void)
{

}


void initializeDemoButtons(void)
{

    // Initiliaze images Demo Button
    imageButton.xPosition = 82;
    imageButton.yPosition = 0;
    imageButton.borderWidth = 5;
    imageButton.selected = false;
    imageButton.imageWidth = Images_Button4BPP_UNCOMP.xSize;
    imageButton.imageHeight = Images_Button4BPP_UNCOMP.ySize;
    imageButton.borderColor = GRAPHICS_COLOR_WHITE;
    imageButton.selectedColor = GRAPHICS_COLOR_RED;
    imageButton.image = &Images_Button4BPP_UNCOMP;

    yesButton.xMin = 0;
    yesButton.xMax = 49;
    yesButton.yMin = 0;
    yesButton.yMax = 18;
    yesButton.borderWidth = 1;
    yesButton.selected = false;
    yesButton.fillColor = GRAPHICS_COLOR_GRAY;
    yesButton.borderColor = GRAPHICS_COLOR_BLACK;
    yesButton.selectedColor = GRAPHICS_COLOR_BLACK;
    yesButton.textColor = GRAPHICS_COLOR_BLACK;
    yesButton.selectedTextColor = GRAPHICS_COLOR_WHITE;
    yesButton.textXPos = 0;
    yesButton.textYPos = 0;
    yesButton.text = "YES";
    yesButton.font = &g_sFontCm18b;

}

void boardInit(void)
{
    // Setup XT1 and XT2
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P5,
        GPIO_PIN2 + GPIO_PIN3 +
        GPIO_PIN4 + GPIO_PIN5
        );
}

void clockInit(void)
{
    UCS_setExternalClockSource(
        32768,
        0);

    // Set Vcore to accomodate for max. allowed system speed
    PMM_setVCore(
        PMM_CORE_LEVEL_3
        );

    // Use 32.768kHz XTAL as reference
    UCS_turnOnLFXT1(
        UCS_XT1_DRIVE_3,
        UCS_XCAP_3
        );

    // Set system clock to max (25MHz)
    UCS_initFLLSettle(
        25000,
        762
        );

    SFR_enableInterrupt(
        SFR_OSCILLATOR_FAULT_INTERRUPT
        );
}

void timerInit()
{
    Timer_A_initUpModeParam timerAUpModeParams =
    {
        TIMER_A_CLOCKSOURCE_SMCLK,
        TIMER_A_CLOCKSOURCE_DIVIDER_64,
        UINT16_MAX,
        TIMER_A_TAIE_INTERRUPT_DISABLE,
        TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE,
        TIMER_A_SKIP_CLEAR
    };
    //Configure timer A to count cycles/64
    Timer_A_initUpMode(
        TIMER_A1_BASE,&timerAUpModeParams);
}

void Delay(){
    __delay_cycles(SYSTEM_CLOCK_SPEED );
}
void DelayCycles(unsigned int k)
{
    while (k--){
        __delay_cycles(1);
    }
}
