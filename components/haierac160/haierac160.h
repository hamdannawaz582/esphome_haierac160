//
// Created by Hamdan Nawaz on 17/04/2026.
//

#ifndef ESPHOME_HAIERAC160_HAIERAC160CLIMATE_H
#define ESPHOME_HAIERAC160_HAIERAC160CLIMATE_H

#include "esphome/components/climate_ir/climate_ir.h"
#include "esphome/components/remote_base/remote_base.h"
#include "haierac160_protocol.h"

namespace esphome {
    namespace haierac160 {
        class HaierAC160Climate : public climate_ir::ClimateIR {
        public:
            HaierAC160Climate();

        protected:
            void transmit_state() override;

        private:
            void setModel(HaierAc160Protocol state);
            void setPrefix(HaierAc160Protocol state);
            void setTemperature(HaierAc160Protocol state);
            void setSwing(HaierAc160Protocol state);
            void setFan(HaierAc160Protocol state);
            void setPower(HaierAc160Protocol state);
            void setMode(HaierAc160Protocol state);
            void setButton(HaierAc160Protocol state);
            void setClean(HaierAc160Protocol state);
            void setChecksums(HaierAc160Protocol state);
        };
    } // haierac160
} // esphome

#endif //ESPHOME_HAIERAC160_HAIERAC160CLIMATE_H