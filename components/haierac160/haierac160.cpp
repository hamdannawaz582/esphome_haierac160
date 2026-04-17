//
// Created by Hamdan Nawaz on 17/04/2026.
//

#include "haierac160.h"

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

            data->set_carrier_frequency(38000);

            HaierAc160Protocol state;
            std::memset(state.raw, 0, sizeof(state.raw));

            setModel(state);
            setTemperature(state);
            setSwing(state);
            setFan(state);
            setPower(state);
            setMode(state);
            setButton(state);
            setPrefix(state);
            setChecksums(state);

            const uint16_t HEADER_MARK = 3000;
            const uint16_t HEADER_SPACE = 4300;
            const uint16_t BIT_MARK = 520;
            const uint16_t ONE_SPACE = 1650;
            const uint16_t ZERO_SPACE = 650;
            const uint16_t GAP = 10000;

            data->mark(HEADER_MARK);
            data->space(HEADER_MARK);
            data->item(HEADER_MARK, HEADER_SPACE);
            for (auto i = 0; i < kHaierAC160StateLength; i++)
            {
                auto byte = state.raw[i];

                for (auto i = 7; i >= 0; --i)
                {
                    data->mark(BIT_MARK);

                    if (byte & (1 << i))
                        data->space(ONE_SPACE);
                    else
                        data->space(ZERO_SPACE);
                }
            }
            data->item(BIT_MARK, GAP);
            transmit.perform();
        }

        void HaierAC160Climate::setModel(HaierAc160Protocol& state) {
            state.Model = kHaierAcYrw02ModelA;
        }

        void HaierAC160Climate::setPrefix(HaierAc160Protocol& state) {
            state.Prefix = kHaierAc160Prefix;
        }

        void HaierAC160Climate::setTemperature(HaierAc160Protocol& state) {
            uint8_t temp = this->target_temperature - 16;
            state.Temp = temp;
        }

        void HaierAC160Climate::setSwing(HaierAc160Protocol& state) {
            auto mode = this->swing_mode;
            if (mode == climate::CLIMATE_SWING_OFF) {
                state.SwingV = kHaierAc160SwingVOff;
                state.SwingH = -1;
            } else {
                state.SwingV = kHaierAc160SwingVAuto;
                state.SwingH = -1;
            }
        }

        void HaierAC160Climate::setFan(HaierAc160Protocol& state) {
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

        void HaierAC160Climate::setPower(HaierAc160Protocol& state) {
            auto power = (this->mode == climate::CLIMATE_MODE_OFF)?(false):(true);
            state.Power = power;
        }

        void HaierAC160Climate::setMode(HaierAc160Protocol& state) {
            auto mode = this->mode;
            if (mode == climate::CLIMATE_MODE_AUTO) {
                state.Mode = kHaierAcYrw02Auto;
            } else if (mode == climate::CLIMATE_MODE_COOL) {
                state.Mode = kHaierAcYrw02Cool;
            } else if (mode == climate::CLIMATE_MODE_DRY) {
                state.Mode = kHaierAcYrw02Dry;
            } else if (mode == climate::CLIMATE_MODE_HEAT) {
                state.Mode = kHaierAcYrw02Heat;
            } else if (mode == climate::CLIMATE_MODE_FAN_ONLY) {
                state.Mode = kHaierAcYrw02Fan;
            }

            state.AuxHeating = (mode == climate::CLIMATE_MODE_HEAT);

            state.Turbo = 0;
        }

        void HaierAC160Climate::setButton(HaierAc160Protocol& state) {
            state.Button = kHaierAcYrw02ButtonPower;
        }

        void HaierAC160Climate::setClean(HaierAc160Protocol& state) {
            state.Clean = 0;
            state.Clean2 = 0;
        }

        void HaierAC160Climate::setChecksums(HaierAc160Protocol& state) {
            uint8_t checksum1 = 0, checksum2 = 0;
            const uint8_t *ptr;

            for (ptr = state.raw; ptr < state.raw + kHaierACYRW02StateLength - 1; ptr++) checksum1 += *ptr;
            for (ptr = state.raw + kHaierACYRW02StateLength; ptr < state.raw + kHaierAC160StateLength - 1; ptr++) checksum2 += *ptr;

            state.Sum = checksum1;
            state.Sum2 = checksum2;
        }
    } // haierac160
} // esphome