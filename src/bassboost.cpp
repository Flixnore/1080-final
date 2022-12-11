#include "plugin.hpp"


struct Bassboost : Module {
	enum ParamId {
		BOOST_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		INPUT_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		OUTPUT_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHTS_LEN
	};

	Bassboost() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(BOOST_PARAM, 0.f, 1.f, 0.f, "");
		configInput(INPUT_INPUT, "");
		configOutput(OUTPUT_OUTPUT, "");
	}

  // Low-pass filter to remove high frequency components
  dsp::RCFilter lowpass;
  // Variable to store the boost amount
  float boostAmount = 0.0f;

	void process(const ProcessArgs& args) override {
		// Get the boost amount from the BOOST_PARAM parameter
		boostAmount = params[BOOST_PARAM].getValue();

		// Get the input signal
		float input = inputs[INPUT_INPUT].getVoltage();

		// Filter out high frequency components
		lowpass.setCutoffFreq(200.0f + (2000.0f * boostAmount));
		lowpass.process(input);

		// Boost the filtered signal
		float output = input * (5.0f + boostAmount);

		// Output the boosted signal
		outputs[OUTPUT_OUTPUT].setVoltage(output);
	}
};


struct BassboostWidget : ModuleWidget {
	BassboostWidget(Bassboost* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/bassboost.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundBlackKnob>(mm2px(Vec(24.349, 86.989)), module, Bassboost::BOOST_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(9.642, 114.972)), module, Bassboost::INPUT_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(39.874, 115.158)), module, Bassboost::OUTPUT_OUTPUT));
	}
};


Model* model_bassboost= createModel<Bassboost, BassboostWidget>("bassboost");