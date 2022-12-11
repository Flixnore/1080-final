#include "plugin.hpp"
#include <cmath>
#include <stdio.h>
#include <stdlib.h>


struct _1080final : Module {
	enum ParamId {
		POTENTIO_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		INPUT, // Not actually pitch
		INPUTS_LEN
	};
	enum OutputId {
		OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHTS_LEN
	};

	_1080final() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(POTENTIO_PARAM, 0.f, 1.f, 0.5f, "");
		configInput(INPUT, "");
		configOutput(OUTPUT, "");
	}

    float seed() {
        return std::log(rand() * rand());
    }

    float electra(float potentiometer, float pitch) {
        return std::exp(potentiometer) * std::cbrt(-10 * pitch);
    }

	void process(const ProcessArgs& args) override {
		//get potentiometer value
		float potentiometer = params[POTENTIO_PARAM].getValue();
		float pitch = inputs[INPUT].getVoltage();
		//outputs[OUTPUT].setVoltage(pitch * potentiometer);
        //float output_signal = pitch;
        //output_signal = std::sin(seed() * pitch);
        //output_signal *= std::cos(potentiometer) + std::exp(pitch);
        //output_signal *= std::exp(std::exp(potentiometer) + std::exp(pitch));
        //output_signal = std::log(output_signal);
        outputs[OUTPUT].setVoltage(electra(potentiometer, pitch));
	}
};


struct _1080finalWidget : ModuleWidget {
	_1080finalWidget(_1080final* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/1080final.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(15.24, 46.063)), module, _1080final::POTENTIO_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(8.818, 113.408)), module, _1080final::INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(21.531, 113.075)), module, _1080final::OUTPUT));
	}
};


Model* model_1080final = createModel<_1080final, _1080finalWidget>("1080final");
