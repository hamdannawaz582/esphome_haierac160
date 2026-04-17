//
// Created by Hamdan Nawaz on 17/04/2026.
// Taken from crankyoldgit's IRremoteESP8266 Library
// https://github.com/crankyoldgit/IRremoteESP8266/blob/master/src/ir_Haier.h
//

#ifndef ESPHOME_HAIERAC160_HAIERAC160_PROTOCOL_H
#define ESPHOME_HAIERAC160_HAIERAC160_PROTOCOL_H
#include <cstdint>

const uint16_t kHaierAC160StateLength = 20;
const uint8_t kHaierAcYrw02ModelA = 0xA6;
const uint8_t kHaierAc160Prefix = 0xB5;
const uint16_t kHaierACYRW02StateLength = 14;


// swing
const uint8_t kHaierAc160SwingVOff = 0b0000;
const uint8_t kHaierAc160SwingVAuto = 0b1100;  // Airflow

// fan
const uint8_t kHaierAcYrw02FanHigh = 0b001;
const uint8_t kHaierAcYrw02FanMed =  0b010;
const uint8_t kHaierAcYrw02FanLow =  0b011;
const uint8_t kHaierAcYrw02FanAuto = 0b101;  // HAIER_AC176 uses `0` in Fan2

// buttons
const uint8_t kHaierAcYrw02ButtonPower = 0b00101;

// modes
const uint8_t kHaierAcAuto = 0;
const uint8_t kHaierAcCool = 1;
const uint8_t kHaierAcDry = 2;
const uint8_t kHaierAcHeat = 3;
const uint8_t kHaierAcFan = 4;

/// Native representation of a Haier 160 bit A/C message.
union HaierAc160Protocol{
    uint8_t raw[kHaierAC160StateLength];  ///< The state in native form
    struct {
        // Byte 0
        uint8_t Model       :8;
        // Byte 1
        uint8_t SwingV      :4;
        uint8_t Temp        :4;  // 16C~30C
        // Byte 2
        uint8_t             :5;
        uint8_t SwingH      :3;
        // Byte 3
        uint8_t             :1;
        uint8_t Health      :1;
        uint8_t             :3;
        uint8_t TimerMode   :3;
        // Byte 4
        uint8_t             :6;
        uint8_t Power       :1;
        uint8_t AuxHeating  :1;
        // Byte 5
        uint8_t OffTimerHrs :5;
        uint8_t Fan         :3;
        // Byte 6
        uint8_t OffTimerMins:6;
        uint8_t Turbo       :1;
        uint8_t Quiet       :1;
        // Byte 7
        uint8_t OnTimerHrs  :5;
        uint8_t Mode        :3;
        // Byte 8
        uint8_t OnTimerMins :6;
        uint8_t             :1;
        uint8_t Sleep       :1;
        // Byte 9
        uint8_t             :8;
        // Byte 10
        uint8_t ExtraDegreeF :1;
        uint8_t              :3;
        uint8_t Clean        :1;
        uint8_t UseFahrenheit:1;
        uint8_t              :2;
        // Byte 11
        uint8_t             :8;
        // Byte 12
        uint8_t Button      :5;
        uint8_t Lock        :1;
        uint8_t             :2;
        // Byte 13
        uint8_t Sum         :8;
        // Byte 14
        uint8_t Prefix     :8;
        // Byte 15
        uint8_t             :6;
        uint8_t Clean2      :1;
        uint8_t             :1;
        // Byte 16
        uint8_t             :5;
        uint8_t Fan2        :3;
        // Byte 17
        uint8_t             :8;
        // Byte 18
        uint8_t             :8;
        // Byte 19
        uint8_t Sum2        :8;
    };
};

#endif //ESPHOME_HAIERAC160_HAIERAC160_PROTOCOL_H