#include "liquidcrystal_i2c.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


extern char TimeInChar[12];
extern RTC_TimeTypeDef Time;

void Update_Display();
void SetDisplay(const char a[], const char b[]);


char Line_1[16];
char Line_2[16];

bool update_display = false;

/**
 * Sets the time display.
 */
void G_SetTime() {
	SetDisplay("Set Time:", TimeInChar);
}

/**
 * Sets the volume display.
 *
 * This function sets the volume display to indicate whether the
 * volume is high or low.
 *
 * @param isVol Boolean indicating whether the volume is high (true) or low (false).
 */
void G_SetVolume(bool isVol) {
	if (isVol) {
		SetDisplay("Volume is HIGH", "Up/Down>HIGH/LOW");
	}
	else {
		SetDisplay("Volume is LOW", "Up/Down>HIGH/LOW");
	}
}

/**
 * Sets the blinking mode display.
 *
 * This function sets the blinking mode display to indicate whether the
 * blinking mode is on or off. =
 *
 * @param isBlink Integer indicating whether the blinking mode is on (1) or off (0).
 */
void G_SetBlink(int isBlink) {
	if (isBlink == 1) {
		SetDisplay("Blink is ON", "Up/Down->ON/OFF");
	}
	else {
		SetDisplay("Blink is OFF", "Up/Down->ON/OFF");
	}
}

/**
 * Sets the display for adding a medication alarm.
 *
 * This function sets the display to prompt the user to click the select button
 * to add a new medication alarm.
 */
void G_Add_Med() {
	SetDisplay("Click Select To", "Add Alarm");
}

/**
 * Displays medication alarm information.
 *
 * This function displays the information of a medication alarm, including its number,
 * hours, and minutes, on the display.
 *
 * @param num The number of the medication alarm.
 * @param hours The hours component of the medication alarm time.
 * @param minutes The minutes component of the medication alarm time.
 */
void G_Display_Med(int num, int hours, int minutes) {
	char MedChar[16];
	sprintf(MedChar, "Med %d at %02d:%02d", num, hours, minutes);
	SetDisplay(TimeInChar, MedChar);
}

/**
 * Sets the display for deleting a medication alarm.
 *
 * This function sets the display to prompt the user to click the down button
 * to delete the specified medication alarm.
 *
 * @param num The number of the medication alarm to delete.
 */
void G_Delete_Med(int num) {
	char MedChar[16];
	sprintf(MedChar, "Delete Alarm %d", num);
	SetDisplay("Click Down To", MedChar);
}

/**
 * Displays a medication alarm and blinks if required.
 *
 * This function displays the medication alarm with the specified number
 * and toggles the backlight to create a blinking effect if Blink is set to 1.
 *
 * @param num The number of the medication alarm.
 * @param Blink Integer indicating whether the blinking effect should be applied (1 for yes, 0 for no).
 */
void G_TakeMed(int num, int Blink) {
	char MedChar[16];
	sprintf(MedChar, "Take Med %d", num);
	SetDisplay(TimeInChar, MedChar);
	if (Blink == 1) {
		Display_NoBacklight();
		HAL_Delay(167);
		Display_Backlight();
	}
}

/**
 * Sets the content of the display.
 *
 * This function sets the content of the display with the provided strings for Line 1 and Line 2.
 *
 * @param a The string for Line 1.
 * @param b The string for Line 2.
 */
void SetDisplay(const char a[], const char b[]) {
	strcpy(Line_1, a);
	strcpy(Line_2, b);
	Update_Display();
}

/**
 * Updates the display with the current content.
 *
 * This function clears the display and sets the cursor position to display
 * the content of Line 1 and Line 2. It then sets the cursor position to the end of Line 1.
 */
void Update_Display() {
	Display_Clear();
	Display_SetCursor(0, 0);
	Display_PrintStr(Line_1);
	Display_SetCursor(0, 1);
	Display_PrintStr(Line_2);
	Display_SetCursor(15, 0);
}
