#pragma once
#include <Arduino.h>

namespace Common {

// ---------------- UART Communication Protocol -------------------

enum MessageType
{
    REQUEST = 0,
    RESPONSE,
    INFO,
    ERROR
};

enum ResponseResult
{
    DONE = 0,
    PENDING,
    BUSY_IGNORED,
    FEATURE_DISABLED,
    FEATURE_NOT_YET_IMPLEMENTED,
    OUT_OF_RANGE
};

enum ErrorID
{
    NO_MESSAGE_TYPE = -100,
    NO_MESSAGE_ID,
    WRONG_MESSAGE_TYPE,
    WRONG_COMMAND_ID,
    UNCOMPLETED_REQUEST,
    JSON_PARSE_ERROR,
    HARDWARE_ERROR
};

enum CommandID
{
    GET_SENSORS = 0,
    SET_FAN_CPS,
    SET_FAN_PWM,
    SET_FAN_PWM_PERCENT,
    GET_FAN_INFO,
    SET_LEDS_PWM,
    SET_LEDS_SHADE,
    GET_LEDS_PWM,
    GET_LEDS_SHADE,
    REQ_HATCH_RECALIBRATE,
    SET_HATCH_POS,
    GET_HATCH_POS
};

constexpr auto RESPONSE_RESULT_PROP     = "Result";
constexpr auto MESSAGE_TYPE_PROP        = "Type";
constexpr auto MESSAGE_ID_PROP          = "ID";
constexpr auto CPS_PROP                 = "CPS";
constexpr auto PWM_PROP                 = "PWM";
constexpr auto RED_COLOR_PROP           = "R";
constexpr auto GREEN_COLOR_PROP         = "G";
constexpr auto BLUE_COLOR_PROP          = "B";
constexpr auto BRIGHTNESS_PROP          = "Br";
constexpr auto HATCH_POS_PROP           = "HPos";
constexpr auto INFO_PROP                = "Info";
constexpr auto TEMPERATURE1_PROP        = "Temp1";
constexpr auto TEMPERATURE2_PROP        = "Temp2";
constexpr auto LIGHT1_PROP              = "Light1";
constexpr auto LIGHT2_PROP              = "Light2";

/*
Request examples:
    {"Type":0,"ID":0}                           ---->   Get all sensors data

    {"Type":0,"ID":10,"HPos":67}                ---->   Open hatch for 67%

    {"Type":0,"ID":1,"CPS":160}                 ---->   Set Fan speed to 160 CPS

    {"Type":0,"ID":6,"R":255,                   ---->   Set light color to RGB(255, 100, 100) and brightness to 95%
     "G":100,"B":100,"Br":95}   

Response examples:
    {"Type":1,"ID":0,"Result":0,"Temp1":22.43,  ---->   Response to successful("Result":0) request GET_SENSORS,
     "Temp2":21.05,"Light1":347,"Light2":402}           send all sensors data in response body

    {"Type":1,"ID":10,"Result":1}               ---->   Response for SET_HATCH_POS, "Result" is 1(PENDING), it means:
                                                        successful start of execution of the command(the hatch began to move)

    {"Type":1,"ID":10,"Result":2}               ---->   "Result" is BUSY_IGNORED - cannot execute the command,
                                                         hatch is already in move and command ignored                                                 

    {"Type":1,"ID":1,"Result":1}                ---->   Successful start to set fan speed

    {"Type":1,"ID":6,"Result":0}                ---->   The lighting parameters have been set successfully

Info messages examples:
    {"Type":2,"ID":10,"HPos":67}                ---->   The hatch reached requested position - 67%

    {"Type":2,"ID":1,"CPS":160}                 ---->   The fan speed was reached and stabilized at requested level - 160 cps

Error messages examples:
    {"Type":3,"ID":-98,"Info":                  ---->   Wrong type in message (property "Info" is optional)
     "Wrong message type: 13"}

    {"Type":3,"ID":-95}                         ---->   Wrong data received(not a Json)
*/


// ------------------------------ Status Led and buzzer ------------------------------
enum Status
{
  READY,
  HATCH_MOVING,
  HATCH_MOVE_DENY
};

// --------------------------------- Pins definition ------------------------------
constexpr auto STATUS_R_PIN   = 22;
constexpr auto STATUS_G_PIN   = 23;
constexpr auto STATUS_B_PIN   = 24;

constexpr auto FAN_PWM_PIN    = 6;
constexpr auto FAN_SPEED_PIN  = 3;

constexpr auto LED_R_PWM_PIN  = 7;
constexpr auto LED_G_PWM_PIN  = 8;
constexpr auto LED_B_PWM_PIN  = 9;

constexpr auto H_MOTOR_PIN1   = 25;
constexpr auto H_MOTOR_PIN2   = 26;
constexpr auto H_MOTOR_PIN3   = 27;
constexpr auto H_MOTOR_PIN4   = 28;

constexpr auto H_OPEN_PIN     = 35;
constexpr auto H_CLOSE_PIN    = 34;

constexpr auto T1_SENSOR_PIN  = 37;
constexpr auto T2_SENSOR_PIN  = 36;

constexpr auto L1_SENSOR_PIN  = A8;
constexpr auto L2_SENSOR_PIN  = A9;

constexpr auto DEBUG_PIN_1    = A0;
constexpr auto DEBUG_PIN_2    = A1;
constexpr auto DEBUG_PIN_3    = A2;

// ---------------------------- Intervals -----------------------
constexpr auto SENSORS_UPDATE_INTERVAL_MS = 800;
constexpr auto FAN_TIMER_INTERVAL_MS = 250;
constexpr auto HATCH_TIMER_INTERVAL_MS = 10;


// ---------------------------- Defaults values -----------------------
constexpr auto STEPS                = 100;

constexpr auto T_INIT_VALUE         = -200.0;
constexpr auto T_WRONG_INDEX_VALUE  = -300.0;
constexpr auto T_READ_ERROR_VALUE   = -400.0;

constexpr auto L_INIT_VALUE         = -1;
constexpr auto L_WRONG_INDEX_VALUE  = -10;

class Counter
{
public:
    Counter()
        : m_count(0)
        , m_counter(0)
    {}

    Counter(const size_t count)
        : m_count(count)
        , m_counter(0)
    {}

    size_t count() const { return m_count; }
    void setCount(const size_t count) 
    { 
        m_count = count;
        m_counter = 0;
    }

    void reset() { m_counter = 0; }

    bool isReached()
    {
        const auto result = m_counter == m_count;
        m_counter = result ? 0 : m_counter + 1;
        return result;
    }

private:
    size_t m_count;
    size_t m_counter;
};

}