//
// Created by Hamdan Nawaz on 17/04/2026.
//

#include "HaierAC160Climate.h"

namespace esphome {
    namespace haierac160 {
        HaierAC160Climate::HaierAC160Climate() : ClimateIR(
                16, // minimum temperature
                30, // maximum temperature
                1, // temperature steps
                true, // supports dry
                true, // supports fan only
                {climate::CLIMATE_FAN_AUTO, climate::CLIMATE_FAN_LOW, climate::CLIMATE_FAN_MEDIUM, climate::CLIMATE_FAN_HIGH}, // supported fan modes
                {climate::CLIMATE_SWING_OFF, climate::CLIMATE_SWING_VERTICAL} // supported swing modes
            ) { }

        void HaierAC160Climate::transmit_state() {
            auto transmit = this->transmitter_->transmit();
            auto * data = transmit.get_data();

            HaierAc160Protocol state;
            std::memset(state.raw, 0, sizeof(state.raw));

            setModel(state);
            setTemperature(state);
            setSwing(state);
            setFan(state);
            setPower(state);
            setMode(state);
            setButton(state);
            setChecksums(state);

            for (auto i = 0; i < kHaierAC160StateLength; i++)
            {
                auto byte = state.raw[i];

                for (auto i = 7; i >= 0; --i)
                {
                    data->mark(563);

                    if (byte & (1 << i))
                        data->space(1690);
                    else
                        data->space(538);
                }
            }

            transmit.perform();
        }

        void HaierAC160Climate::setModel(HaierAc160Protocol state) {
            state.Model = kHaierAcYrw02ModelA;
        }

        void HaierAC160Climate::setPrefix(HaierAc160Protocol state) {
            state.Prefix = kHaierAc160Prefix;
        }

        void HaierAC160Climate::setTemperature(HaierAc160Protocol state) {
            auto temp = this->target_temperature - 16;
            state.Temp = temp;
        }

        void HaierAC160Climate::setSwing(HaierAc160Protocol state) {
            auto mode = this->swing_mode;
            if (mode == climate::CLIMATE_SWING_OFF) {
                state.SwingV = kHaierAc160SwingVOff;
                state.SwingH = -1;
            } else {
                state.SwingV = kHaierAc160SwingVAuto;
                state.SwingH = -1;
            }
        }

        void HaierAC160Climate::setFan(HaierAc160Protocol state) {
            auto mode = this->fan_mode;
            if (mode == climate::CLIMATE_FAN_AUTO) {
                state.Fan = kHaierAcYrw02FanAuto;
                state.Fan2 = 0;
            } else if (mode == climate::CLIMATE_FAN_LOW) {
                state.Fan = kHaierAcYrw02FanLow;
                state.Fan2 = state.Fan;
            } else if (mode == climate::CLIMATE_FAN_MEDIUM) {
                state.Fan = kHaierAcYrw02FanMed;
                state.Fan2 = state.Fan;
            } else if (mode == climate::CLIMATE_FAN_HIGH) {
                state.Fan = kHaierAcYrw02FanHigh;
                state.Fan2 = state.Fan;
            }
        }

        void HaierAC160Climate::setPower(HaierAc160Protocol state) {
            auto power = (this->mode == climate::CLIMATE_MODE_OFF)?(false):(true);
            state.Power = power;
        }

        void HaierAC160Climate::setMode(HaierAc160Protocol state) {
            auto mode = this->mode;
            if (mode == climate::CLIMATE_MODE_AUTO) {
                state.Mode = kHaierAcAuto;
            } else if (mode == climate::CLIMATE_MODE_COOL) {
                state.Mode = kHaierAcCool;
            } else if (mode == climate::CLIMATE_MODE_DRY) {
                state.Mode = kHaierAcDry;
            } else if (mode == climate::CLIMATE_MODE_HEAT) {
                state.Mode = kHaierAcHeat;
            } else if (mode == climate::CLIMATE_MODE_FAN_ONLY) {
                state.Mode = kHaierAcFan;
            }

            state.Turbo = 0;
        }

        void HaierAC160Climate::setButton(HaierAc160Protocol state) {
            state.Button = kHaierAcYrw02ButtonPower;
        }

        void HaierAC160Climate::setClean(HaierAc160Protocol state) {
            state.Clean = 0;
            state.Clean2 = 0;
        }

        void HaierAC160Climate::setChecksums(HaierAc160Protocol state) {
            uint8_t checksum1 = 0, checksum2 = 0;
            const uint8_t *ptr = state.raw;

            for (ptr = state.raw; ptr < state.raw + kHaierACYRW02StateLength - 1; ptr++) checksum1 += *ptr;
            for (ptr = state.raw + kHaierACYRW02StateLength; ptr < state.raw + kHaierAC160StateLength - kHaierACYRW02StateLength - 1; ptr++) checksum2 += *ptr;

            state.Sum = checksum1;
            state.Sum2 = checksum2;
        }
    } // haierac160
} // esphome