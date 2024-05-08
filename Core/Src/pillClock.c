#include "main.h"
#include "liquidcrystal_i2c.h"
#include <stdio.h>
#include <stdbool.h>

extern RTC_TimeTypeDef Time;

void L_SetTime();
void L_SetVolume();
void L_SetBlink();
void L_Add_Med();
void L_Delete_Med(int i);
int GetNextMed(int start , bool isUp);
void EmptyList();
int FindEmpty();
GetDifference(int hours, int minutes);
int abs(int i);

int HOLDERFORFUNNYSTUFF = 0;

//Adding Med
int Alarm_Hours = 0;
int Alarm_Minutes = 0;

//Med Storage
int medTime[8][2];
int NumOfAlarms;
int NextMedIndex = -1;

// Alarm
bool AlarmOn = false;
int lastAlarm = -1;
int lastmin = -1;

//Basic Display
int SlideNum;
bool inSetting;
bool Block_Toggle = false;
bool GoingUp = false;
bool StartUp = true;
int startIndex = 0;
int toggleCount = 1;

// Basic Settings
int isBlinkON;
bool isLoud;

//Button Pressed Variables
bool isSelect;
bool isUp;
bool isDown;

/**
 * Runs the setup process before the RunLoop() function
 */
void RunSetUp() {
	Display_Init(2);
	EmptyList();
	NumOfAlarms = 0;
	SlideNum = 0;
	inSetting = false;
	StartUp = true;
	AlarmOn = false;
	startIndex = 0;
	Block_Toggle = false;
	isLoud = false;
	isBlinkON = 0;

	isSelect = false;
	isUp = false;
	isDown = false;

}

/**
 * Runs the main loop of the program.
 *
 * This function is responsible for managing the main loop of the program,
 * which includes processing user inputs, controlling display modes, managing alarms,
 * and playing sounds.
 */
void RunLoop() {

	// Read button states
	isSelect = SelectPressed();
	isUp = UpPressed();
	isDown = DownPressed();

	// Initialization phase when the program starts
	if (StartUp) {
		toggleCount = 2;
		ChooseDisplay(startIndex);
		if (isSelect) {
			startIndex++;
		}
		if (startIndex == 2) {
			StartUp = false;
			toggleCount = 1;
		}
		return;
	}

	// Handle navigation and display mode selection
	if (isUp && !inSetting && !AlarmOn) {
		SlideNum++;
		if (SlideNum > 3 + NumOfAlarms) SlideNum = 0;
		GoingUp = true;
	}
	if (isDown && !inSetting && !AlarmOn) {
		SlideNum--;
		if (SlideNum < 0 ) SlideNum = 3 + NumOfAlarms;
		GoingUp = false;
	}

	// Choose display mode
	if (!AlarmOn) {
		ChooseDisplay(SlideNum);

	}

	// Manage alarms
	int NextMedIndex = nextMed();
	if (nextMed() != -1 && NextMedIndex != lastAlarm) {
		toggleCount = 1;
		inSetting = false;
		AlarmOn = true;
		PlaySound(isLoud, true);
	}

	// Handle alarm activation
	if (AlarmOn) {
		G_TakeMed(NextMedIndex + 1, isBlinkON);
		if (isSelect) {
			AlarmOn = false;
			lastAlarm = NextMedIndex;
			lastmin = Time.Minutes;
			PlaySound(isLoud, false);
		}
	}
	if (lastmin != Time.Minutes) lastAlarm = -1;
}

/**
 * Choose display mode based on the given index.
 *
 * This function selects the display mode based on the provided index. It toggles
 * between different display modes depending on the value of toggleCount. It performs
 * specific actions based on the selected index:
 *   - 0: Sets time
 *   - 1: Sets blinking mode on both displays and toggles blink state on display
 *   - 2: Sets volume on the display and adjusts volume in code.
 *   - 3: Adds or deletes medication information
 *   - Other: Displays or deletes medication information on the global display
 *            based on the direction of navigation and current mode.
 *
 * @param i The index indicating the display mode.
 */
void ChooseDisplay(int i) {
	if (toggleCount % 2 == 0) {
		inSetting = true;
		Display_Blink();
	}
	else {
		inSetting = false;
		Display_NoBlink();
	}

	switch(i) {
		case 0:
			L_SetTime();
			G_SetTime();
			break;
		case 1:
			L_SetBlink();
			G_SetBlink(isBlinkON);
			break;
		case 2:
			L_SetVolume();
			G_SetVolume(isLoud);
			break;
		case 3:
			if (!inSetting) G_Add_Med();
			if (inSetting) L_Add_Med();
			break;
		default:
			int current_med = GetNextMed(i , GoingUp);
			if (!inSetting && current_med > -1) {
				G_Display_Med(current_med + 1, medTime[current_med][0], medTime[current_med][1]);
			}
			else if (current_med > -1) {
				G_Delete_Med(current_med + 1);
				L_Delete_Med(current_med);
			}
	}

	if (isSelect && !Block_Toggle) toggleCount++;
}


/**
 * Sets the time in the code.
 *
 * This function sets the time on the code if inSetting flag is true.
 * It adjusts the time based on the direction (up or down) and updates the display accordingly.
 */
void L_SetTime() {
	if (!inSetting) return;

	// adds a minute unless time goes over 59 minutes, then adds an hour
	if (isUp) {
		uint8_t min = Time.Minutes + 1;
		uint8_t ho = Time.Hours;
		if (min > 59) {
			ho++;
			min = 0;
		}
		SetTime(ho, min);
	}
	// remove a minute while making sure you don't go under 0
	else if (isDown) {
		if (Time.Hours >= 0) {
			if (Time.Minutes > 0) {
				SetTime(Time.Hours, Time.Minutes - 0x1);
			}
		}
	}
}

/**
 * Sets the volume in code.
 *
 * This function sets the volume in code if inSetting flag is true.
 * It adjusts the volume based on the direction (up or down) by toggling the isLoud flag.
 */
void L_SetVolume() {
	if (!inSetting) return;
	if (isUp) {
		isLoud = true;
	}
	else if (isDown) {
		isLoud = false;
	}
}

/**
 * Sets the blinking mode on the local display.
 *
 * This function sets the blinking mode in the code
 * It adjusts the blinking mode based on the direction (up or down) by toggling the isBlinkON flag.
 *
 * @note This function assumes that the inSetting, isUp, and isDown variables are properly initialized elsewhere.
 */
void L_SetBlink() {
	if (!inSetting) return;
	if (isUp) {
		isBlinkON = 1;
	}
	else if (isDown) {
		isBlinkON = 0;
	}
}

/**
 * It allows incrementing or decrementing the alarm time based on the direction (up or down).
 * Once the alarm time is set, the function displays the alarm on the local display and stores
 * the alarm time in the medTime array if the select button is pressed.
 *
 * @note This function assumes that the inSetting, isUp, isDown, isSelect, Alarm_Hours, Alarm_Minutes,
 *       Block_Toggle, NumOfAlarms, and medTime variables are properly initialized elsewhere.
 */
void L_Add_Med() {
	if (!inSetting) return;
	Block_Toggle = true;

	// Adjust the medication alarm time based on the direction
	if (isUp) {
		Alarm_Minutes++;
	}
	else if (isDown) {
		Alarm_Minutes--;
	}

	// Handle rollover for minutes and hours
	if (Alarm_Minutes > 59) {
		Alarm_Minutes = 0;
		Alarm_Hours++;
	}
	if (Alarm_Minutes < 0) {
		Alarm_Minutes = 59;
		Alarm_Hours--;
	}
	if (Alarm_Hours < 0) Alarm_Hours = 0;

	// Find an empty slot for storing the medication alarm
	int i = FindEmpty();

	G_Display_Med(i + 1, Alarm_Hours, Alarm_Minutes);

	// If the select button is pressed, store the alarm time and update counters
	if (isSelect) {
		medTime[i][0] = Alarm_Hours;
		medTime[i][1] = Alarm_Minutes;
		Block_Toggle = false;
		NumOfAlarms++;
		Alarm_Minutes = 0;
		Alarm_Hours = 0;
	}
}

/**
 * This function deletes a medication alarm if inSetting flag is true
 * and the direction is downwards. It marks the alarm time in the medTime array as empty
 * and updates the alarm count.
 *
 * @param i The index of the medication alarm to delete.
 *
 * @note This function assumes that the inSetting, isDown, NumOfAlarms, and medTime variables
 *       are properly initialized elsewhere.
 */
void L_Delete_Med(int i) {
	if (!inSetting) return;
	if (isDown) {
		medTime[i][0] = -1;
		NumOfAlarms--;
		toggleCount++;
	}
}

/**
 * Empties the medication alarm list.
 *
 * This function sets all elements of the medication alarm list to -1,
 *
 * -1 representing that it is an empty index of the array .
 */
void EmptyList() {
	for (int i = 0; i < 8; i++) {
		medTime[i][0] = -1;
	}
}

/**
 * Finds an empty slot in the medication alarm list.
 *
 * @return The index of the first empty slot found in the medication alarm list.
 *         If no empty slot is found, returns -1.
 */
int FindEmpty() {
	for(int i = 0; i < 8; i++) {
		if (medTime[i][0] == -1) return i;
	}
}

/**
 * Gets the index of the next medication alarm relative to a starting index.
 *
 * @param start The starting index.
 * @param isUp Boolean indicating whether to search upwards (true) or downwards (false).
 * @return The index of the next medication alarm relative to the starting index.
 *         If no medication alarm is found within the range, returns -1.
 *
 * @note This function assumes that the medTime array is properly initialized elsewhere.
 */
int GetNextMed(int start , bool isUp) {
	int i = start - 4;
	while (medTime[i][0] == -1) {
		if (isUp) i++;
		if (!isUp) i--;

		if (i < 0) {
			SlideNum = 3;
			return -1;
		}
		if (i > (8 - 1)) {
			SlideNum = 0;
			return -1;
		}
	}
	return i;
}

/**
 * Finds the index of the next medication alarm.
 *
 *
 * @return The index of the next medication alarm based on the current time.
 *         If no matching alarm is found, returns -1.
 *
 * @note This function assumes that the Time and medTime variables are properly initialized elsewhere.
 */
int nextMed() {
	for(int i = 0; i < 8; i++) {
		if (medTime[i][0] == Time.Hours && medTime[i][1] == Time.Minutes) {
			return i;
		}
	}
	return -1;
}

/**
 * Calculates the time difference between the current time and the given time.
 *
 * @param hours The hours component of the specified time.
 * @param minutes The minutes component of the specified time.
 * @return The absolute time difference in minutes between the current time and the specified time.
 *
 * @note This function assumes that the Time variable is properly initialized elsewhere.
 */
int GetDifference(int hours, int minutes) {
	int diff = Time.Hours*60 + Time.Minutes - (hours*60 + minutes);
	if (diff > 0) {
		diff = hours*60 + minutes + 24*60 - (Time.Hours*60 + Time.Minutes);
	}
	return abs(diff);
}

/**
 * Calculates the absolute value of an integer.
 *
 * @param i The input integer.
 * @return The absolute value of the input integer.
 */
int abs(int i) {
	if (i < 0) return 1*-1;
	return i;
}

