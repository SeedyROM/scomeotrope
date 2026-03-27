/* ------------------------------------------------------------
name: "scomeotrope"
Code generated with Faust 2.79.3 (https://faust.grame.fr)
Compilation options: -lang cpp -ct 1 -cn ScomeotropeDSP -scn  -es 1 -mcd 0 -mdd 1024 -mdy 33 -uim -single -ftz 0 -vec -lv 1 -vs 32
------------------------------------------------------------ */

#ifndef  __ScomeotropeDSP_H__
#define  __ScomeotropeDSP_H__

#include "FaustDefs.h"

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

/* link with : "" */
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS ScomeotropeDSP
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

#if defined(_WIN32)
#define RESTRICT __restrict
#else
#define RESTRICT __restrict__
#endif

struct ScomeotropeDSPSIG0 {
	int IOTA0;
	int iVec1[2];
	int iRec20[2];
	
	int getNumInputsScomeotropeDSPSIG0() {
		return 0;
	}
	int getNumOutputsScomeotropeDSPSIG0() {
		return 1;
	}
	
	void instanceInitScomeotropeDSPSIG0(int sample_rate) {
		(void)sample_rate;
		IOTA0 = 0;
		for (int l26 = 0; l26 < 2; l26 = l26 + 1) {
			iVec1[l26] = 0;
		}
		for (int l27 = 0; l27 < 2; l27 = l27 + 1) {
			iRec20[l27] = 0;
		}
	}
	
	void fillScomeotropeDSPSIG0(int count, float* table) {
		for (int i1 = 0; i1 < count; i1 = i1 + 1) {
			iVec1[IOTA0 & 1] = 1;
			iRec20[IOTA0 & 1] = (iVec1[(IOTA0 - 1) & 1] + iRec20[(IOTA0 - 1) & 1]) % 65536;
			table[i1] = std::sin(9.58738e-05f * float(iRec20[IOTA0 & 1]));
			IOTA0 = IOTA0 + 1;
		}
	}

};

static ScomeotropeDSPSIG0* newScomeotropeDSPSIG0() { return (ScomeotropeDSPSIG0*)new ScomeotropeDSPSIG0(); }
static void deleteScomeotropeDSPSIG0(ScomeotropeDSPSIG0* dsp) { delete dsp; }

static float ScomeotropeDSP_faustpower2_f(float value) {
	return value * value;
}
static float ftbl0ScomeotropeDSPSIG0[65536];

struct ScomeotropeDSP {
	
	int iVec0[64];
	int iVec0_idx;
	int iVec0_idx_save;
	int fSampleRate;
	float fConst0;
	float fConst1;
	float fRec0[64];
	int fRec0_idx;
	int fRec0_idx_save;
	FAUSTFLOAT fVslider0;
	float fRec1[64];
	int fRec1_idx;
	int fRec1_idx_save;
	FAUSTFLOAT fVslider1;
	float fRec3[64];
	int fRec3_idx;
	int fRec3_idx_save;
	float fConst2;
	float fRec2[64];
	int fRec2_idx;
	int fRec2_idx_save;
	float fConst3;
	float fConst4;
	float fRec4[64];
	int fRec4_idx;
	int fRec4_idx_save;
	float fConst5;
	float fConst6;
	float fConst7;
	float fConst8;
	float fConst9;
	float fRec8[64];
	int fRec8_idx;
	int fRec8_idx_save;
	float fConst10;
	float fConst11;
	float fRec7[64];
	int fRec7_idx;
	int fRec7_idx_save;
	int iRec11[64];
	int iRec11_idx;
	int iRec11_idx_save;
	float fYec0[64];
	int fYec0_idx;
	int fYec0_idx_save;
	float fRec10[64];
	int fRec10_idx;
	int fRec10_idx_save;
	float fConst12;
	float fConst13;
	float fYec1[64];
	int fYec1_idx;
	int fYec1_idx_save;
	float fConst14;
	float fRec9[64];
	int fRec9_idx;
	int fRec9_idx_save;
	float fRec13[64];
	int fRec13_idx;
	int fRec13_idx_save;
	float fYec2[64];
	int fYec2_idx;
	int fYec2_idx_save;
	float fRec14[64];
	int fRec14_idx;
	int fRec14_idx_save;
	float fConst15;
	float fConst16;
	float fConst17;
	float fConst18;
	float fConst19;
	float fConst20;
	float fRec12[64];
	int fRec12_idx;
	int fRec12_idx_save;
	FAUSTFLOAT fVslider2;
	FAUSTFLOAT fVslider3;
	float fConst21;
	FAUSTFLOAT fVslider4;
	FAUSTFLOAT fVslider5;
	FAUSTFLOAT fVslider6;
	FAUSTFLOAT fVslider7;
	float fRec15[64];
	int fRec15_idx;
	int fRec15_idx_save;
	float fRec18[64];
	int fRec18_idx;
	int fRec18_idx_save;
	float fRec17[64];
	int fRec17_idx;
	int fRec17_idx_save;
	float fRec16[64];
	int fRec16_idx;
	int fRec16_idx_save;
	FAUSTFLOAT fVslider8;
	FAUSTFLOAT fVslider9;
	FAUSTFLOAT fVbargraph0;
	float fYec3[64];
	int fYec3_idx;
	int fYec3_idx_save;
	float fYec4[64];
	int fYec4_idx;
	int fYec4_idx_save;
	float fRec6[64];
	int fRec6_idx;
	int fRec6_idx_save;
	float fRec5[1024];
	int fRec5_idx;
	int fRec5_idx_save;
	FAUSTFLOAT fVslider10;
	float fRec19[64];
	int fRec19_idx;
	int fRec19_idx_save;
	FAUSTFLOAT fVslider11;
	FAUSTFLOAT fVslider12;
	FAUSTFLOAT fCheckbox0;
	float fYec5[512];
	int fYec5_idx;
	int fYec5_idx_save;
	float fRec21[64];
	int fRec21_idx;
	int fRec21_idx_save;
	FAUSTFLOAT fVbargraph1;
	float fYec6[64];
	int fYec6_idx;
	int fYec6_idx_save;
	float fYec7[64];
	int fYec7_idx;
	int fYec7_idx_save;
	float fRec23[64];
	int fRec23_idx;
	int fRec23_idx_save;
	float fRec22[1024];
	int fRec22_idx;
	int fRec22_idx_save;
	float fYec8[512];
	int fYec8_idx;
	int fYec8_idx_save;
	
	ScomeotropeDSP() {
	}
	
	void metadata(Meta* m) { 
		m->declare("aanl.lib/ADAA1:author", "Dario Sanfilippo");
		m->declare("aanl.lib/ADAA1:copyright", "Copyright (C) 2021 Dario Sanfilippo     <sanfilippo.dario@gmail.com>");
		m->declare("aanl.lib/ADAA1:license", "MIT License");
		m->declare("aanl.lib/name", "Faust Antialiased Nonlinearities");
		m->declare("aanl.lib/tanh1:author", "Dario Sanfilippo");
		m->declare("aanl.lib/tanh1:copyright", "Copyright (C) 2021 Dario Sanfilippo     <sanfilippo.dario@gmail.com>");
		m->declare("aanl.lib/tanh1:license", "MIT License");
		m->declare("aanl.lib/version", "1.4.1");
		m->declare("basics.lib/bypass1:author", "Julius Smith");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/parallelMin:author", "Bart Brouns");
		m->declare("basics.lib/parallelMin:copyright", "Copyright (c) 2020 Bart Brouns <bart@magnetophon.nl>");
		m->declare("basics.lib/parallelMin:licence", "GPL-3.0");
		m->declare("basics.lib/parallelOp:author", "Bart Brouns");
		m->declare("basics.lib/parallelOp:copyright", "Copyright (c) 2020 Bart Brouns <bart@magnetophon.nl>");
		m->declare("basics.lib/parallelOp:licence", "GPL-3.0");
		m->declare("basics.lib/version", "1.21.0");
		m->declare("compile_options", "-lang cpp -ct 1 -cn ScomeotropeDSP -scn  -es 1 -mcd 0 -mdd 1024 -mdy 33 -uim -single -ftz 0 -vec -lv 1 -vs 32");
		m->declare("compressors.lib/FFcompressor_N_chan:author", "Bart Brouns");
		m->declare("compressors.lib/FFcompressor_N_chan:license", "GPLv3");
		m->declare("compressors.lib/name", "Faust Compressor Effect Library");
		m->declare("compressors.lib/peak_compression_gain_N_chan_db:author", "Bart Brouns");
		m->declare("compressors.lib/peak_compression_gain_N_chan_db:license", "GPLv3");
		m->declare("compressors.lib/peak_compression_gain_mono_db:author", "Bart Brouns");
		m->declare("compressors.lib/peak_compression_gain_mono_db:license", "GPLv3");
		m->declare("compressors.lib/version", "1.6.0");
		m->declare("delays.lib/fdelay5:author", "Julius O. Smith III");
		m->declare("delays.lib/fdelayltv:author", "Julius O. Smith III");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "1.1.0");
		m->declare("filename", "scomeotrope.dsp");
		m->declare("filters.lib/dcblocker:author", "Julius O. Smith III");
		m->declare("filters.lib/dcblocker:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/dcblocker:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/fir:author", "Julius O. Smith III");
		m->declare("filters.lib/fir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/fir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/iir:author", "Julius O. Smith III");
		m->declare("filters.lib/iir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/pole:author", "Julius O. Smith III");
		m->declare("filters.lib/pole:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/pole:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf1:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf1s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1s:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf2:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf2s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2s:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/version", "1.7.1");
		m->declare("filters.lib/zero:author", "Julius O. Smith III");
		m->declare("filters.lib/zero:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/zero:license", "MIT-style STK-4.3 license");
		m->declare("interpolators.lib/interpolate_linear:author", "Stéphane Letz");
		m->declare("interpolators.lib/interpolate_linear:licence", "MIT");
		m->declare("interpolators.lib/name", "Faust Interpolator Library");
		m->declare("interpolators.lib/version", "1.4.0");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.8.1");
		m->declare("misceffects.lib/dryWetMixerConstantPower:author", "David Braun, revised by Stéphane Letz");
		m->declare("misceffects.lib/name", "Misc Effects Library");
		m->declare("misceffects.lib/version", "2.5.1");
		m->declare("name", "scomeotrope");
		m->declare("noises.lib/name", "Faust Noise Generator Library");
		m->declare("noises.lib/version", "1.4.1");
		m->declare("oscillators.lib/lf_sawpos:author", "Bart Brouns, revised by Stéphane Letz");
		m->declare("oscillators.lib/lf_sawpos:licence", "STK-4.3");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "1.6.0");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "1.2.0");
		m->declare("signals.lib/name", "Faust Signal Routing Library");
		m->declare("signals.lib/onePoleSwitching:author", "Jonatan Liljedahl, revised by Dario Sanfilippo");
		m->declare("signals.lib/onePoleSwitching:licence", "STK-4.3");
		m->declare("signals.lib/version", "1.6.0");
	}

	virtual int getNumInputs() {
		return 2;
	}
	virtual int getNumOutputs() {
		return 2;
	}
	
	static void classInit(int sample_rate) {
		(void)sample_rate;
		ScomeotropeDSPSIG0* sig0 = newScomeotropeDSPSIG0();
		sig0->instanceInitScomeotropeDSPSIG0(sample_rate);
		sig0->fillScomeotropeDSPSIG0(65536, ftbl0ScomeotropeDSPSIG0);
		deleteScomeotropeDSPSIG0(sig0);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
		fConst1 = 17.0f / fConst0;
		fConst2 = 1.0f / fConst0;
		fConst3 = std::exp(-(6283.1855f / fConst0));
		fConst4 = 1.0f - fConst3;
		fConst5 = std::tan(942.4778f / fConst0);
		fConst6 = 1.0f / fConst5;
		fConst7 = 1.0f / ((fConst6 + 1.4142135f) / fConst5 + 1.0f);
		fConst8 = (fConst6 + -1.4142135f) / fConst5 + 1.0f;
		fConst9 = 2.0f * (1.0f - 1.0f / ScomeotropeDSP_faustpower2_f(fConst5));
		fConst10 = std::exp(-(5.0f / fConst0));
		fConst11 = std::exp(-(2e+01f / fConst0));
		fConst12 = 1.0f / std::tan(9424.778f / fConst0);
		fConst13 = 1.0f / (fConst12 + 1.0f);
		fConst14 = 1.0f - fConst12;
		fConst15 = std::tan(4712.389f / fConst0);
		fConst16 = 1.0f / fConst15;
		fConst17 = (fConst16 + 1.4142135f) / fConst15 + 1.0f;
		fConst18 = 1.0f / fConst17;
		fConst19 = (fConst16 + -1.4142135f) / fConst15 + 1.0f;
		fConst20 = 2.0f * (1.0f - 1.0f / ScomeotropeDSP_faustpower2_f(fConst15));
		fConst21 = 0.05f / fConst17;
	}
	
	virtual void instanceResetUserInterface() {
		fVslider0 = FAUSTFLOAT(0.5f);
		fVslider1 = FAUSTFLOAT(1.0f);
		fVslider2 = FAUSTFLOAT(0.5f);
		fVslider3 = FAUSTFLOAT(0.15f);
		fVslider4 = FAUSTFLOAT(-39.89f);
		fVslider5 = FAUSTFLOAT(8.0f);
		fVslider6 = FAUSTFLOAT(0.021f);
		fVslider7 = FAUSTFLOAT(0.022f);
		fVslider8 = FAUSTFLOAT(16.65f);
		fVslider9 = FAUSTFLOAT(1.0f);
		fVslider10 = FAUSTFLOAT(1.0f);
		fVslider11 = FAUSTFLOAT(1.6f);
		fVslider12 = FAUSTFLOAT(0.01f);
		fCheckbox0 = FAUSTFLOAT(0.0f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 64; l0 = l0 + 1) {
			iVec0[l0] = 0;
		}
		iVec0_idx = 0;
		iVec0_idx_save = 0;
		for (int l1 = 0; l1 < 64; l1 = l1 + 1) {
			fRec0[l1] = 0.0f;
		}
		fRec0_idx = 0;
		fRec0_idx_save = 0;
		for (int l2 = 0; l2 < 64; l2 = l2 + 1) {
			fRec1[l2] = 0.0f;
		}
		fRec1_idx = 0;
		fRec1_idx_save = 0;
		for (int l3 = 0; l3 < 64; l3 = l3 + 1) {
			fRec3[l3] = 0.0f;
		}
		fRec3_idx = 0;
		fRec3_idx_save = 0;
		for (int l4 = 0; l4 < 64; l4 = l4 + 1) {
			fRec2[l4] = 0.0f;
		}
		fRec2_idx = 0;
		fRec2_idx_save = 0;
		for (int l5 = 0; l5 < 64; l5 = l5 + 1) {
			fRec4[l5] = 0.0f;
		}
		fRec4_idx = 0;
		fRec4_idx_save = 0;
		for (int l6 = 0; l6 < 64; l6 = l6 + 1) {
			fRec8[l6] = 0.0f;
		}
		fRec8_idx = 0;
		fRec8_idx_save = 0;
		for (int l7 = 0; l7 < 64; l7 = l7 + 1) {
			fRec7[l7] = 0.0f;
		}
		fRec7_idx = 0;
		fRec7_idx_save = 0;
		for (int l8 = 0; l8 < 64; l8 = l8 + 1) {
			iRec11[l8] = 0;
		}
		iRec11_idx = 0;
		iRec11_idx_save = 0;
		for (int l9 = 0; l9 < 64; l9 = l9 + 1) {
			fYec0[l9] = 0.0f;
		}
		fYec0_idx = 0;
		fYec0_idx_save = 0;
		for (int l10 = 0; l10 < 64; l10 = l10 + 1) {
			fRec10[l10] = 0.0f;
		}
		fRec10_idx = 0;
		fRec10_idx_save = 0;
		for (int l11 = 0; l11 < 64; l11 = l11 + 1) {
			fYec1[l11] = 0.0f;
		}
		fYec1_idx = 0;
		fYec1_idx_save = 0;
		for (int l12 = 0; l12 < 64; l12 = l12 + 1) {
			fRec9[l12] = 0.0f;
		}
		fRec9_idx = 0;
		fRec9_idx_save = 0;
		for (int l13 = 0; l13 < 64; l13 = l13 + 1) {
			fRec13[l13] = 0.0f;
		}
		fRec13_idx = 0;
		fRec13_idx_save = 0;
		for (int l14 = 0; l14 < 64; l14 = l14 + 1) {
			fYec2[l14] = 0.0f;
		}
		fYec2_idx = 0;
		fYec2_idx_save = 0;
		for (int l15 = 0; l15 < 64; l15 = l15 + 1) {
			fRec14[l15] = 0.0f;
		}
		fRec14_idx = 0;
		fRec14_idx_save = 0;
		for (int l16 = 0; l16 < 64; l16 = l16 + 1) {
			fRec12[l16] = 0.0f;
		}
		fRec12_idx = 0;
		fRec12_idx_save = 0;
		for (int l17 = 0; l17 < 64; l17 = l17 + 1) {
			fRec15[l17] = 0.0f;
		}
		fRec15_idx = 0;
		fRec15_idx_save = 0;
		for (int l18 = 0; l18 < 64; l18 = l18 + 1) {
			fRec18[l18] = 0.0f;
		}
		fRec18_idx = 0;
		fRec18_idx_save = 0;
		for (int l19 = 0; l19 < 64; l19 = l19 + 1) {
			fRec17[l19] = 0.0f;
		}
		fRec17_idx = 0;
		fRec17_idx_save = 0;
		for (int l20 = 0; l20 < 64; l20 = l20 + 1) {
			fRec16[l20] = 0.0f;
		}
		fRec16_idx = 0;
		fRec16_idx_save = 0;
		for (int l21 = 0; l21 < 64; l21 = l21 + 1) {
			fYec3[l21] = 0.0f;
		}
		fYec3_idx = 0;
		fYec3_idx_save = 0;
		for (int l22 = 0; l22 < 64; l22 = l22 + 1) {
			fYec4[l22] = 0.0f;
		}
		fYec4_idx = 0;
		fYec4_idx_save = 0;
		for (int l23 = 0; l23 < 64; l23 = l23 + 1) {
			fRec6[l23] = 0.0f;
		}
		fRec6_idx = 0;
		fRec6_idx_save = 0;
		for (int l24 = 0; l24 < 1024; l24 = l24 + 1) {
			fRec5[l24] = 0.0f;
		}
		fRec5_idx = 0;
		fRec5_idx_save = 0;
		for (int l25 = 0; l25 < 64; l25 = l25 + 1) {
			fRec19[l25] = 0.0f;
		}
		fRec19_idx = 0;
		fRec19_idx_save = 0;
		for (int l28 = 0; l28 < 512; l28 = l28 + 1) {
			fYec5[l28] = 0.0f;
		}
		fYec5_idx = 0;
		fYec5_idx_save = 0;
		for (int l29 = 0; l29 < 64; l29 = l29 + 1) {
			fRec21[l29] = 0.0f;
		}
		fRec21_idx = 0;
		fRec21_idx_save = 0;
		for (int l30 = 0; l30 < 64; l30 = l30 + 1) {
			fYec6[l30] = 0.0f;
		}
		fYec6_idx = 0;
		fYec6_idx_save = 0;
		for (int l31 = 0; l31 < 64; l31 = l31 + 1) {
			fYec7[l31] = 0.0f;
		}
		fYec7_idx = 0;
		fYec7_idx_save = 0;
		for (int l32 = 0; l32 < 64; l32 = l32 + 1) {
			fRec23[l32] = 0.0f;
		}
		fRec23_idx = 0;
		fRec23_idx_save = 0;
		for (int l33 = 0; l33 < 1024; l33 = l33 + 1) {
			fRec22[l33] = 0.0f;
		}
		fRec22_idx = 0;
		fRec22_idx_save = 0;
		for (int l34 = 0; l34 < 512; l34 = l34 + 1) {
			fYec8[l34] = 0.0f;
		}
		fYec8_idx = 0;
		fYec8_idx_save = 0;
	}
	
	virtual void init(int sample_rate) {
		classInit(sample_rate);
		instanceInit(sample_rate);
	}
	
	virtual void instanceInit(int sample_rate) {
		instanceConstants(sample_rate);
		instanceResetUserInterface();
		instanceClear();
	}
	
	virtual ScomeotropeDSP* clone() {
		return new ScomeotropeDSP();
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openHorizontalBox("CHL03: Tape Channel");
		ui_interface->openHorizontalBox("Dynamics");
		ui_interface->declare(&fVslider2, "0", "");
		ui_interface->declare(&fVslider2, "id", "comp_strength");
		ui_interface->addVerticalSlider("Strength", &fVslider2, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.5f), FAUSTFLOAT(0.001f));
		ui_interface->declare(&fVslider4, "1", "");
		ui_interface->declare(&fVslider4, "id", "comp_threshold");
		ui_interface->declare(&fVslider4, "unit", "dB");
		ui_interface->addVerticalSlider("Threshold", &fVslider4, FAUSTFLOAT(-39.89f), FAUSTFLOAT(-9e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fVslider5, "2", "");
		ui_interface->declare(&fVslider5, "id", "comp_knee");
		ui_interface->declare(&fVslider5, "unit", "dB");
		ui_interface->addVerticalSlider("Knee", &fVslider5, FAUSTFLOAT(8.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(4e+01f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fVslider6, "3", "");
		ui_interface->declare(&fVslider6, "id", "comp_attack");
		ui_interface->declare(&fVslider6, "unit", "sec");
		ui_interface->addVerticalSlider("Attack", &fVslider6, FAUSTFLOAT(0.021f), FAUSTFLOAT(0.005f), FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0001f));
		ui_interface->declare(&fVslider7, "4", "");
		ui_interface->declare(&fVslider7, "id", "comp_release");
		ui_interface->declare(&fVslider7, "unit", "sec");
		ui_interface->addVerticalSlider("Release", &fVslider7, FAUSTFLOAT(0.022f), FAUSTFLOAT(0.005f), FAUSTFLOAT(0.1f), FAUSTFLOAT(0.0001f));
		ui_interface->declare(&fVslider9, "5", "");
		ui_interface->declare(&fVslider9, "id", "comp_mix");
		ui_interface->addVerticalSlider("Mix", &fVslider9, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0001f));
		ui_interface->declare(&fVslider8, "6", "");
		ui_interface->declare(&fVslider8, "id", "comp_makeup");
		ui_interface->declare(&fVslider8, "unit", "dB");
		ui_interface->addVerticalSlider("Make Up", &fVslider8, FAUSTFLOAT(16.65f), FAUSTFLOAT(0.1f), FAUSTFLOAT(32.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fVbargraph0, "7", "");
		ui_interface->declare(&fVbargraph0, "id", "comp_gr");
		ui_interface->declare(&fVbargraph0, "tooltip", "gain reduction in dB");
		ui_interface->declare(&fVbargraph0, "unit", "dB");
		ui_interface->addVerticalBargraph("GR", &fVbargraph0, FAUSTFLOAT(-2e+01f), FAUSTFLOAT(0.0f));
		ui_interface->declare(&fVbargraph1, "7", "");
		ui_interface->declare(&fVbargraph1, "id", "comp_gr");
		ui_interface->declare(&fVbargraph1, "tooltip", "gain reduction in dB");
		ui_interface->declare(&fVbargraph1, "unit", "dB");
		ui_interface->addVerticalBargraph("GR", &fVbargraph1, FAUSTFLOAT(-2e+01f), FAUSTFLOAT(0.0f));
		ui_interface->closeBox();
		ui_interface->openHorizontalBox("Machine Decay");
		ui_interface->declare(&fVslider1, "0", "");
		ui_interface->declare(&fVslider1, "id", "vibrato_speed");
		ui_interface->addVerticalSlider("Speed", &fVslider1, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0001f), FAUSTFLOAT(13.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fVslider0, "1", "");
		ui_interface->declare(&fVslider0, "id", "vibrato_amount");
		ui_interface->addVerticalSlider("Amount", &fVslider0, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fVslider10, "2", "");
		ui_interface->declare(&fVslider10, "id", "vibrato_mix");
		ui_interface->addVerticalSlider("Mix", &fVslider10, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fCheckbox0, "3", "");
		ui_interface->declare(&fCheckbox0, "id", "vibrato_bypass");
		ui_interface->addCheckButton("Bypass", &fCheckbox0);
		ui_interface->closeBox();
		ui_interface->declare(&fVslider3, "1", "");
		ui_interface->declare(&fVslider3, "id", "noise_amount");
		ui_interface->addVerticalSlider("Noise", &fVslider3, FAUSTFLOAT(0.15f), FAUSTFLOAT(0.0f), FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0001f));
		ui_interface->declare(&fVslider12, "2", "");
		ui_interface->declare(&fVslider12, "id", "flutter");
		ui_interface->addVerticalSlider("Flutter", &fVslider12, FAUSTFLOAT(0.01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(0.032f), FAUSTFLOAT(0.001f));
		ui_interface->declare(&fVslider11, "3", "");
		ui_interface->declare(&fVslider11, "id", "output_gain");
		ui_interface->addVerticalSlider("Output Gain", &fVslider11, FAUSTFLOAT(1.6f), FAUSTFLOAT(1.0f), FAUSTFLOAT(12.0f), FAUSTFLOAT(0.001f));
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0_ptr = inputs[0];
		FAUSTFLOAT* input1_ptr = inputs[1];
		FAUSTFLOAT* output0_ptr = outputs[0];
		FAUSTFLOAT* output1_ptr = outputs[1];
		int iZec0[32];
		float fZec1[32];
		float fSlow0 = 0.01f * float(fVslider0);
		float fSlow1 = 0.01f * float(fVslider1);
		float fZec2[32];
		float fZec3[32];
		float fZec4[32];
		int iZec5[32];
		float fZec6[32];
		float fSlow2 = float(fVslider2);
		float fSlow3 = float(fVslider3);
		float fZec7[32];
		float fZec8[32];
		float fZec9[32];
		float fSlow4 = float(fVslider4);
		float fSlow5 = float(fVslider5);
		float fSlow6 = 0.5f * fSlow5;
		float fSlow7 = fSlow4 - fSlow6;
		float fSlow8 = fSlow4 + fSlow6;
		int iZec10[32];
		float fZec11[32];
		float fSlow9 = 0.5f / std::max<float>(1.1920929e-07f, fSlow5);
		float fZec12[32];
		float fSlow10 = float(fVslider6);
		int iSlow11 = std::fabs(fSlow10) < 1.1920929e-07f;
		float fSlow12 = ((iSlow11) ? 0.0f : std::exp(-(fConst2 / ((iSlow11) ? 1.0f : fSlow10))));
		float fSlow13 = float(fVslider7);
		int iSlow14 = std::fabs(fSlow13) < 1.1920929e-07f;
		float fSlow15 = ((iSlow14) ? 0.0f : std::exp(-(fConst2 / ((iSlow14) ? 1.0f : fSlow13))));
		float fZec13[32];
		float fZec14[32];
		float fZec15[32];
		float fZec16[32];
		int iZec17[32];
		float fZec18[32];
		float fZec19[32];
		float fZec20[32];
		float fSlow16 = 0.70710677f * std::pow(1e+01f, 0.05f * float(fVslider8));
		float fSlow17 = 1.5707964f * float(fVslider9);
		float fSlow18 = std::cos(fSlow17);
		float fSlow19 = std::sin(fSlow17);
		float fZec21[32];
		float fZec22[32];
		float fZec23[32];
		float fZec24[32];
		float fSlow20 = 0.001f * float(fVslider10);
		float fSlow21 = float(fVslider11);
		float fSlow22 = float(fVslider12);
		float fZec25[32];
		float fZec26[32];
		float fZec27[32];
		float fZec28[32];
		float fZec29[32];
		float fZec30[32];
		float fZec31[32];
		float fZec32[32];
		float fZec33[32];
		float fZec34[32];
		float fZec35[32];
		float fZec36[32];
		float fZec37[32];
		float fZec38[32];
		float fZec39[32];
		float fZec40[32];
		int iZec41[32];
		int iZec42[32];
		int iZec43[32];
		float fZec44[32];
		float fZec45[32];
		int iZec46[32];
		float fZec47[32];
		int iZec48[32];
		float fZec49[32];
		int iZec50[32];
		float fZec51[32];
		int iZec52[32];
		float fSlow23 = 1.5707964f * (1.0f - float(fCheckbox0));
		float fZec53[32];
		float fZec54[32];
		float fZec55[32];
		int iZec56[32];
		int iZec57[32];
		int iZec58[32];
		float fZec59[32];
		float fZec60[32];
		int iZec61[32];
		float fZec62[32];
		int iZec63[32];
		float fZec64[32];
		int iZec65[32];
		float fZec66[32];
		int iZec67[32];
		float fZec68[32];
		float fZec69[32];
		float fZec70[32];
		for (int vindex = 0; vindex < count; vindex = vindex + 32) {
			FAUSTFLOAT* input0 = &input0_ptr[vindex];
			FAUSTFLOAT* input1 = &input1_ptr[vindex];
			FAUSTFLOAT* output0 = &output0_ptr[vindex];
			FAUSTFLOAT* output1 = &output1_ptr[vindex];
			int vsize = std::min<int>(32, count - vindex);
			/* Vectorizable loop 0 */
			/* Pre code */
			iVec0_idx = (iVec0_idx + iVec0_idx_save) & 63;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iVec0[(i + iVec0_idx) & 63] = 1;
			}
			/* Post code */
			iVec0_idx_save = vsize;
			/* Recursive loop 1 */
			/* Pre code */
			iRec11_idx = (iRec11_idx + iRec11_idx_save) & 63;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iRec11[(i + iRec11_idx) & 63] = 1103515245 * iRec11[(i + iRec11_idx - 1) & 63] + 12345;
			}
			/* Post code */
			iRec11_idx_save = vsize;
			/* Vectorizable loop 2 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec0[i] = 1 - iVec0[(i + iVec0_idx - 1) & 63];
			}
			/* Vectorizable loop 3 */
			/* Pre code */
			fYec0_idx = (fYec0_idx + fYec0_idx_save) & 63;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fYec0[(i + fYec0_idx) & 63] = float(iRec11[(i + iRec11_idx) & 63]);
			}
			/* Post code */
			fYec0_idx_save = vsize;
			/* Recursive loop 4 */
			/* Pre code */
			fRec13_idx = (fRec13_idx + fRec13_idx_save) & 63;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec4[i] = ((iZec0[i]) ? 0.0f : fRec13[(i + fRec13_idx - 1) & 63] + fConst2 * (9.313226e-10f * fYec0[(i + fYec0_idx) & 63] + 0.3f));
				fRec13[(i + fRec13_idx) & 63] = fZec4[i] - std::floor(fZec4[i]);
			}
			/* Post code */
			fRec13_idx_save = vsize;
			/* Vectorizable loop 5 */
			/* Pre code */
			fYec2_idx = (fYec2_idx + fYec2_idx_save) & 63;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fYec2[(i + fYec2_idx) & 63] = fRec13[(i + fRec13_idx) & 63] - fRec13[(i + fRec13_idx - 1) & 63];
			}
			/* Post code */
			fYec2_idx_save = vsize;
			/* Vectorizable loop 6 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec5[i] = (fYec2[(i + fYec2_idx - 1) & 63] <= 0.0f) & (fYec2[(i + fYec2_idx) & 63] > 0.0f);
			}
			/* Recursive loop 7 */
			/* Pre code */
			fRec10_idx = (fRec10_idx + fRec10_idx_save) & 63;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec10[(i + fRec10_idx) & 63] = 0.5221894f * fRec10[(i + fRec10_idx - 3) & 63] + 4.656613e-10f * fYec0[(i + fYec0_idx) & 63] + 2.494956f * fRec10[(i + fRec10_idx - 1) & 63] - 2.0172658f * fRec10[(i + fRec10_idx - 2) & 63];
			}
			/* Post code */
			fRec10_idx_save = vsize;
			/* Recursive loop 8 */
			/* Pre code */
			fRec14_idx = (fRec14_idx + fRec14_idx_save) & 63;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec14[(i + fRec14_idx) & 63] = fRec14[(i + fRec14_idx - 1) & 63] * float(1 - iZec5[i]) + 4.656613e-10f * fYec0[(i + fYec0_idx) & 63] * float(iZec5[i]);
			}
			/* Post code */
			fRec14_idx_save = vsize;
			/* Recursive loop 9 */
			/* Pre code */
			fRec8_idx = (fRec8_idx + fRec8_idx_save) & 63;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec8[(i + fRec8_idx) & 63] = float(input0[i]) - fConst7 * (fConst8 * fRec8[(i + fRec8_idx - 2) & 63] + fConst9 * fRec8[(i + fRec8_idx - 1) & 63]);
			}
			/* Post code */
			fRec8_idx_save = vsize;
			/* Vectorizable loop 10 */
			/* Pre code */
			fYec1_idx = (fYec1_idx + fYec1_idx_save) & 63;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fYec1[(i + fYec1_idx) & 63] = 0.049922034f * fRec10[(i + fRec10_idx) & 63] + 0.0506127f * fRec10[(i + fRec10_idx - 2) & 63] - (0.095993534f * fRec10[(i + fRec10_idx - 1) & 63] + 0.004408786f * fRec10[(i + fRec10_idx - 3) & 63]);
			}
			/* Post code */
			fYec1_idx_save = vsize;
			/* Vectorizable loop 11 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec6[i] = 0.5f * (fRec14[(i + fRec14_idx) & 63] + 1.0f);
			}
			/* Recursive loop 12 */
			/* Pre code */
			fRec18_idx = (fRec18_idx + fRec18_idx_save) & 63;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec18[(i + fRec18_idx) & 63] = float(input1[i]) - fConst7 * (fConst8 * fRec18[(i + fRec18_idx - 2) & 63] + fConst9 * fRec18[(i + fRec18_idx - 1) & 63]);
			}
			/* Post code */
			fRec18_idx_save = vsize;
			/* Recursive loop 13 */
			/* Pre code */
			fRec3_idx = (fRec3_idx + fRec3_idx_save) & 63;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec3[(i + fRec3_idx) & 63] = fSlow1 + 0.99f * fRec3[(i + fRec3_idx - 1) & 63];
			}
			/* Post code */
			fRec3_idx_save = vsize;
			/* Vectorizable loop 14 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec3[i] = std::fabs(fConst7 * (fRec8[(i + fRec8_idx - 2) & 63] + fRec8[(i + fRec8_idx) & 63] + 2.0f * fRec8[(i + fRec8_idx - 1) & 63]));
			}
			/* Recursive loop 15 */
			/* Pre code */
			fRec9_idx = (fRec9_idx + fRec9_idx_save) & 63;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec9[(i + fRec9_idx) & 63] = fConst13 * (0.05f * (fYec1[(i + fYec1_idx) & 63] + fYec1[(i + fYec1_idx - 1) & 63]) - fConst14 * fRec9[(i + fRec9_idx - 1) & 63]);
			}
			/* Post code */
			fRec9_idx_save = vsize;
			/* Recursive loop 16 */
			/* Pre code */
			fRec12_idx = (fRec12_idx + fRec12_idx_save) & 63;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec12[(i + fRec12_idx) & 63] = 4.656613e-10f * fYec0[(i + fYec0_idx - 1) & 63] * float((fRec13[(i + fRec13_idx) & 63] >= fZec6[i]) * (fRec13[(i + fRec13_idx - 1) & 63] < fZec6[i])) - fConst18 * (fConst19 * fRec12[(i + fRec12_idx - 2) & 63] + fConst20 * fRec12[(i + fRec12_idx - 1) & 63]);
			}
			/* Post code */
			fRec12_idx_save = vsize;
			/* Vectorizable loop 17 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec14[i] = std::fabs(fConst7 * (fRec18[(i + fRec18_idx - 2) & 63] + fRec18[(i + fRec18_idx) & 63] + 2.0f * fRec18[(i + fRec18_idx - 1) & 63]));
			}
			/* Recursive loop 18 */
			/* Pre code */
			fRec1_idx = (fRec1_idx + fRec1_idx_save) & 63;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec1[(i + fRec1_idx) & 63] = fSlow0 + 0.99f * fRec1[(i + fRec1_idx - 1) & 63];
			}
			/* Post code */
			fRec1_idx_save = vsize;
			/* Recursive loop 19 */
			/* Pre code */
			fRec2_idx = (fRec2_idx + fRec2_idx_save) & 63;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec2[i] = ((iZec0[i]) ? 0.0f : fRec2[(i + fRec2_idx - 1) & 63] + fConst2 * fRec3[(i + fRec3_idx) & 63]);
				fRec2[(i + fRec2_idx) & 63] = fZec2[i] - std::floor(fZec2[i]);
			}
			/* Post code */
			fRec2_idx_save = vsize;
			/* Recursive loop 20 */
			/* Pre code */
			fRec7_idx = (fRec7_idx + fRec7_idx_save) & 63;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec7[(i + fRec7_idx) & 63] = fRec7[(i + fRec7_idx - 1) & 63] + (fZec3[i] - fRec7[(i + fRec7_idx - 1) & 63]) * (1.0f - ((fZec3[i] > fRec7[(i + fRec7_idx - 1) & 63]) ? fConst11 : fConst10));
			}
			/* Post code */
			fRec7_idx_save = vsize;
			/* Vectorizable loop 21 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec7[i] = fRec9[(i + fRec9_idx) & 63] - fConst21 * (fRec12[(i + fRec12_idx - 2) & 63] + fRec12[(i + fRec12_idx) & 63] + 2.0f * fRec12[(i + fRec12_idx - 1) & 63]);
			}
			/* Recursive loop 22 */
			/* Pre code */
			fRec17_idx = (fRec17_idx + fRec17_idx_save) & 63;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec17[(i + fRec17_idx) & 63] = fRec17[(i + fRec17_idx - 1) & 63] + (fZec14[i] - fRec17[(i + fRec17_idx - 1) & 63]) * (1.0f - ((fZec14[i] > fRec17[(i + fRec17_idx - 1) & 63]) ? fConst11 : fConst10));
			}
			/* Post code */
			fRec17_idx_save = vsize;
			/* Recursive loop 23 */
			/* Pre code */
			fRec0_idx = (fRec0_idx + fRec0_idx_save) & 63;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec1[i] = ((iZec0[i]) ? 0.0f : fConst1 + fRec0[(i + fRec0_idx - 1) & 63]);
				fRec0[(i + fRec0_idx) & 63] = fZec1[i] - std::floor(fZec1[i]);
			}
			/* Post code */
			fRec0_idx_save = vsize;
			/* Vectorizable loop 24 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec8[i] = float(input0[i]) + fSlow3 * (std::min<float>(fRec7[(i + fRec7_idx) & 63], 1.0f) + 0.3f) * fZec7[i];
			}
			/* Vectorizable loop 25 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec15[i] = float(input1[i]) + fSlow3 * fZec7[i] * (std::min<float>(fRec17[(i + fRec17_idx) & 63], 1.0f) + 0.3f);
			}
			/* Vectorizable loop 26 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec33[i] = 256.0f * (fRec1[(i + fRec1_idx) & 63] * ftbl0ScomeotropeDSPSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec2[(i + fRec2_idx) & 63]), 65535))] + 1.0f);
			}
			/* Vectorizable loop 27 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec9[i] = 2e+01f * std::log10(std::max<float>(1.1754944e-38f, std::fabs(fZec8[i])));
			}
			/* Vectorizable loop 28 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec16[i] = 2e+01f * std::log10(std::max<float>(1.1754944e-38f, std::fabs(fZec15[i])));
			}
			/* Vectorizable loop 29 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec25[i] = 256.0f * (fSlow22 * ftbl0ScomeotropeDSPSIG0[std::max<int>(0, std::min<int>(int(65536.0f * fRec0[(i + fRec0_idx) & 63]), 65535))] + 1.0f);
			}
			/* Vectorizable loop 30 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec34[i] = fZec33[i] + -1.999995f;
			}
			/* Vectorizable loop 31 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec10[i] = (fZec9[i] > fSlow7) + (fZec9[i] > fSlow8);
			}
			/* Vectorizable loop 32 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec11[i] = fZec9[i] - fSlow4;
			}
			/* Vectorizable loop 33 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec17[i] = (fZec16[i] > fSlow7) + (fZec16[i] > fSlow8);
			}
			/* Vectorizable loop 34 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec18[i] = fZec16[i] - fSlow4;
			}
			/* Vectorizable loop 35 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec26[i] = fZec25[i] + -1.999995f;
			}
			/* Vectorizable loop 36 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec35[i] = std::floor(fZec34[i]);
			}
			/* Vectorizable loop 37 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec44[i] = fZec33[i] + (-1.0f - fZec35[i]);
			}
			/* Vectorizable loop 38 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec12[i] = std::max<float>(0.0f, ((iZec10[i] == 0) ? 0.0f : ((iZec10[i] == 1) ? fSlow9 * ScomeotropeDSP_faustpower2_f(fSlow6 + fZec11[i]) : fZec11[i])));
			}
			/* Vectorizable loop 39 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec19[i] = std::max<float>(0.0f, ((iZec17[i] == 0) ? 0.0f : ((iZec17[i] == 1) ? fSlow9 * ScomeotropeDSP_faustpower2_f(fSlow6 + fZec18[i]) : fZec18[i])));
			}
			/* Vectorizable loop 40 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec27[i] = std::floor(fZec26[i]);
			}
			/* Vectorizable loop 41 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec40[i] = fZec33[i] - fZec35[i];
			}
			/* Vectorizable loop 42 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec45[i] = fZec40[i] * fZec44[i];
			}
			/* Vectorizable loop 43 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec59[i] = fZec25[i] + (-1.0f - fZec27[i]);
			}
			/* Recursive loop 44 */
			/* Pre code */
			fRec15_idx = (fRec15_idx + fRec15_idx_save) & 63;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec13[i] = ((-(fSlow2 * fZec12[i]) > fRec15[(i + fRec15_idx - 1) & 63]) ? fSlow15 : fSlow12);
				fRec15[(i + fRec15_idx) & 63] = fRec15[(i + fRec15_idx - 1) & 63] * fZec13[i] - fSlow2 * fZec12[i] * (1.0f - fZec13[i]);
			}
			/* Post code */
			fRec15_idx_save = vsize;
			/* Recursive loop 45 */
			/* Pre code */
			fRec16_idx = (fRec16_idx + fRec16_idx_save) & 63;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec20[i] = ((-(fSlow2 * fZec19[i]) > fRec16[(i + fRec16_idx - 1) & 63]) ? fSlow15 : fSlow12);
				fRec16[(i + fRec16_idx) & 63] = fRec16[(i + fRec16_idx - 1) & 63] * fZec20[i] - fSlow2 * fZec19[i] * (1.0f - fZec20[i]);
			}
			/* Post code */
			fRec16_idx_save = vsize;
			/* Vectorizable loop 46 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec32[i] = fZec25[i] - fZec27[i];
			}
			/* Vectorizable loop 47 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec39[i] = fZec33[i] + (-2.0f - fZec35[i]);
			}
			/* Vectorizable loop 48 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec47[i] = fZec45[i] * fZec39[i];
			}
			/* Vectorizable loop 49 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec60[i] = fZec32[i] * fZec59[i];
			}
			/* Vectorizable loop 50 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec21[i] = std::min<float>(fRec15[(i + fRec15_idx) & 63], fRec16[(i + fRec16_idx) & 63]);
			}
			/* Recursive loop 51 */
			/* Pre code */
			fRec19_idx = (fRec19_idx + fRec19_idx_save) & 63;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec19[(i + fRec19_idx) & 63] = fSlow20 + 0.999f * fRec19[(i + fRec19_idx - 1) & 63];
			}
			/* Post code */
			fRec19_idx_save = vsize;
			/* Vectorizable loop 52 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec31[i] = fZec25[i] + (-2.0f - fZec27[i]);
			}
			/* Vectorizable loop 53 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec38[i] = fZec33[i] + (-3.0f - fZec35[i]);
			}
			/* Vectorizable loop 54 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec41[i] = int(fZec34[i]);
			}
			/* Vectorizable loop 55 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec49[i] = fZec47[i] * fZec38[i];
			}
			/* Vectorizable loop 56 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec53[i] = fSlow23 * fRec19[(i + fRec19_idx) & 63];
			}
			/* Vectorizable loop 57 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec62[i] = fZec60[i] * fZec31[i];
			}
			/* Vectorizable loop 58 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec68[i] = fRec16[(i + fRec16_idx) & 63] + 0.888f * (fZec21[i] - fRec16[(i + fRec16_idx) & 63]);
			}
			/* Vectorizable loop 59 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec22[i] = fRec15[(i + fRec15_idx) & 63] + 0.888f * (fZec21[i] - fRec15[(i + fRec15_idx) & 63]);
			}
			/* Vectorizable loop 60 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec30[i] = fZec25[i] + (-3.0f - fZec27[i]);
			}
			/* Vectorizable loop 61 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec37[i] = fZec33[i] + (-4.0f - fZec35[i]);
			}
			/* Vectorizable loop 62 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec43[i] = std::min<int>(512, std::max<int>(0, iZec41[i]));
			}
			/* Vectorizable loop 63 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec46[i] = std::min<int>(512, std::max<int>(0, iZec41[i] + 2));
			}
			/* Vectorizable loop 64 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec48[i] = std::min<int>(512, std::max<int>(0, iZec41[i] + 3));
			}
			/* Vectorizable loop 65 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec50[i] = std::min<int>(512, std::max<int>(0, iZec41[i] + 4));
			}
			/* Vectorizable loop 66 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec51[i] = fZec49[i] * fZec37[i];
			}
			/* Vectorizable loop 67 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec52[i] = std::min<int>(512, std::max<int>(0, iZec41[i] + 5));
			}
			/* Vectorizable loop 68 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec54[i] = std::sin(fZec53[i]);
			}
			/* Vectorizable loop 69 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec55[i] = std::cos(fZec53[i]);
			}
			/* Vectorizable loop 70 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec56[i] = int(fZec26[i]);
			}
			/* Vectorizable loop 71 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec64[i] = fZec62[i] * fZec30[i];
			}
			/* Recursive loop 72 */
			/* Pre code */
			fRec21_idx = (fRec21_idx + fRec21_idx_save) & 63;
			fYec6_idx = (fYec6_idx + fYec6_idx_save) & 63;
			fYec7_idx = (fYec7_idx + fYec7_idx_save) & 63;
			fRec23_idx = (fRec23_idx + fRec23_idx_save) & 63;
			fRec22_idx = (fRec22_idx + fRec22_idx_save) & 1023;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec21[(i + fRec21_idx) & 63] = fConst3 * fRec21[(i + fRec21_idx - 1) & 63] + fConst4 * fRec22[(i + fRec22_idx - 1) & 1023];
				fVbargraph1 = FAUSTFLOAT(fZec68[i]);
				fZec69[i] = fSlow16 * fZec15[i] * (fSlow18 + fSlow19 * std::pow(1e+01f, 0.05f * fZec68[i])) - 0.3f * fRec21[(i + fRec21_idx - 1) & 63];
				fYec6[(i + fYec6_idx) & 63] = (((0.5f * fZec69[i]) >= 0.0f) ? 0.5f * (fZec69[i] / (0.3f * fZec69[i] + 1.0f)) : 0.5f * (fZec69[i] / (1.0f - 0.075f * fZec69[i])));
				fZec70[i] = fYec6[(i + fYec6_idx) & 63] - fYec6[(i + fYec6_idx - 1) & 63];
				fYec7[(i + fYec7_idx) & 63] = ((std::fabs(fZec70[i]) <= 0.001f) ? tanhf(0.5f * (fYec6[(i + fYec6_idx) & 63] + fYec6[(i + fYec6_idx - 1) & 63])) : (std::log(std::min<float>(3.4028235e+38f, coshf(fYec6[(i + fYec6_idx) & 63]))) - std::log(std::min<float>(3.4028235e+38f, coshf(fYec6[(i + fYec6_idx - 1) & 63])))) / fZec70[i]);
				fRec23[(i + fRec23_idx) & 63] = 0.995f * fRec23[(i + fRec23_idx - 1) & 63] + 2.0f * (fYec7[(i + fYec7_idx) & 63] - fYec7[(i + fYec7_idx - 1) & 63]);
				fRec22[(i + fRec22_idx) & 1023] = fRec23[(i + fRec23_idx) & 63];
			}
			/* Post code */
			fYec6_idx_save = vsize;
			fYec7_idx_save = vsize;
			fRec23_idx_save = vsize;
			fRec21_idx_save = vsize;
			fRec22_idx_save = vsize;
			/* Recursive loop 73 */
			/* Pre code */
			fRec4_idx = (fRec4_idx + fRec4_idx_save) & 63;
			fYec3_idx = (fYec3_idx + fYec3_idx_save) & 63;
			fYec4_idx = (fYec4_idx + fYec4_idx_save) & 63;
			fRec6_idx = (fRec6_idx + fRec6_idx_save) & 63;
			fRec5_idx = (fRec5_idx + fRec5_idx_save) & 1023;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fRec4[(i + fRec4_idx) & 63] = fConst3 * fRec4[(i + fRec4_idx - 1) & 63] + fConst4 * fRec5[(i + fRec5_idx - 1) & 1023];
				fVbargraph0 = FAUSTFLOAT(fZec22[i]);
				fZec23[i] = fSlow16 * fZec8[i] * (fSlow18 + fSlow19 * std::pow(1e+01f, 0.05f * fZec22[i])) - 0.3f * fRec4[(i + fRec4_idx - 1) & 63];
				fYec3[(i + fYec3_idx) & 63] = (((0.5f * fZec23[i]) >= 0.0f) ? 0.5f * (fZec23[i] / (0.3f * fZec23[i] + 1.0f)) : 0.5f * (fZec23[i] / (1.0f - 0.075f * fZec23[i])));
				fZec24[i] = fYec3[(i + fYec3_idx) & 63] - fYec3[(i + fYec3_idx - 1) & 63];
				fYec4[(i + fYec4_idx) & 63] = ((std::fabs(fZec24[i]) <= 0.001f) ? tanhf(0.5f * (fYec3[(i + fYec3_idx) & 63] + fYec3[(i + fYec3_idx - 1) & 63])) : (std::log(std::min<float>(3.4028235e+38f, coshf(fYec3[(i + fYec3_idx) & 63]))) - std::log(std::min<float>(3.4028235e+38f, coshf(fYec3[(i + fYec3_idx - 1) & 63])))) / fZec24[i]);
				fRec6[(i + fRec6_idx) & 63] = 0.995f * fRec6[(i + fRec6_idx - 1) & 63] + 2.0f * (fYec4[(i + fYec4_idx) & 63] - fYec4[(i + fYec4_idx - 1) & 63]);
				fRec5[(i + fRec5_idx) & 1023] = fRec6[(i + fRec6_idx) & 63];
			}
			/* Post code */
			fYec3_idx_save = vsize;
			fYec4_idx_save = vsize;
			fRec6_idx_save = vsize;
			fRec4_idx_save = vsize;
			fRec5_idx_save = vsize;
			/* Vectorizable loop 74 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec29[i] = fZec25[i] + (-4.0f - fZec27[i]);
			}
			/* Vectorizable loop 75 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec36[i] = fZec33[i] + (-5.0f - fZec35[i]);
			}
			/* Vectorizable loop 76 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec42[i] = std::min<int>(512, std::max<int>(0, iZec41[i] + 1));
			}
			/* Vectorizable loop 77 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec57[i] = std::min<int>(512, std::max<int>(0, iZec56[i] + 1));
			}
			/* Vectorizable loop 78 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec58[i] = std::min<int>(512, std::max<int>(0, iZec56[i]));
			}
			/* Vectorizable loop 79 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec61[i] = std::min<int>(512, std::max<int>(0, iZec56[i] + 2));
			}
			/* Vectorizable loop 80 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec63[i] = std::min<int>(512, std::max<int>(0, iZec56[i] + 3));
			}
			/* Vectorizable loop 81 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec65[i] = std::min<int>(512, std::max<int>(0, iZec56[i] + 4));
			}
			/* Vectorizable loop 82 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec66[i] = fZec64[i] * fZec29[i];
			}
			/* Vectorizable loop 83 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				iZec67[i] = std::min<int>(512, std::max<int>(0, iZec56[i] + 5));
			}
			/* Vectorizable loop 84 */
			/* Pre code */
			fYec8_idx = (fYec8_idx + fYec8_idx_save) & 511;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fYec8[(i + fYec8_idx) & 511] = fZec54[i] * (fZec36[i] * (fZec37[i] * (fZec38[i] * (fZec39[i] * (0.041666668f * fZec40[i] * fRec22[(i + fRec22_idx - iZec42[i]) & 1023] - 0.008333334f * fZec44[i] * fRec22[(i + fRec22_idx - iZec43[i]) & 1023]) - 0.083333336f * fZec45[i] * fRec22[(i + fRec22_idx - iZec46[i]) & 1023]) + 0.083333336f * fZec47[i] * fRec22[(i + fRec22_idx - iZec48[i]) & 1023]) - 0.041666668f * fZec49[i] * fRec22[(i + fRec22_idx - iZec50[i]) & 1023]) + 0.008333334f * fZec51[i] * fRec22[(i + fRec22_idx - iZec52[i]) & 1023]) + fRec22[(i + fRec22_idx) & 1023] * fZec55[i];
			}
			/* Post code */
			fYec8_idx_save = vsize;
			/* Vectorizable loop 85 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fZec28[i] = fZec25[i] + (-5.0f - fZec27[i]);
			}
			/* Vectorizable loop 86 */
			/* Pre code */
			fYec5_idx = (fYec5_idx + fYec5_idx_save) & 511;
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				fYec5[(i + fYec5_idx) & 511] = (fZec36[i] * (fZec37[i] * (fZec38[i] * (fZec39[i] * (0.041666668f * fZec40[i] * fRec5[(i + fRec5_idx - iZec42[i]) & 1023] - 0.008333334f * fRec5[(i + fRec5_idx - iZec43[i]) & 1023] * fZec44[i]) - 0.083333336f * fZec45[i] * fRec5[(i + fRec5_idx - iZec46[i]) & 1023]) + 0.083333336f * fZec47[i] * fRec5[(i + fRec5_idx - iZec48[i]) & 1023]) - 0.041666668f * fZec49[i] * fRec5[(i + fRec5_idx - iZec50[i]) & 1023]) + 0.008333334f * fZec51[i] * fRec5[(i + fRec5_idx - iZec52[i]) & 1023]) * fZec54[i] + fRec5[(i + fRec5_idx) & 1023] * fZec55[i];
			}
			/* Post code */
			fYec5_idx_save = vsize;
			/* Vectorizable loop 87 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output1[i] = FAUSTFLOAT(tanhf(fSlow21 * (fZec28[i] * (fZec29[i] * (fZec30[i] * (fZec31[i] * (0.029462783f * fZec32[i] * fYec8[(i + fYec8_idx - iZec57[i]) & 511] - 0.0058925566f * fZec59[i] * fYec8[(i + fYec8_idx - iZec58[i]) & 511]) - 0.058925565f * fZec60[i] * fYec8[(i + fYec8_idx - iZec61[i]) & 511]) + 0.058925565f * fZec62[i] * fYec8[(i + fYec8_idx - iZec63[i]) & 511]) - 0.029462783f * fZec64[i] * fYec8[(i + fYec8_idx - iZec65[i]) & 511]) + 0.0058925566f * fZec66[i] * fYec8[(i + fYec8_idx - iZec67[i]) & 511])));
			}
			/* Vectorizable loop 88 */
			/* Compute code */
			for (int i = 0; i < vsize; i = i + 1) {
				output0[i] = FAUSTFLOAT(tanhf(fSlow21 * (fZec28[i] * (fZec29[i] * (fZec30[i] * (fZec31[i] * (0.029462783f * fZec32[i] * fYec5[(i + fYec5_idx - iZec57[i]) & 511] - 0.0058925566f * fYec5[(i + fYec5_idx - iZec58[i]) & 511] * fZec59[i]) - 0.058925565f * fZec60[i] * fYec5[(i + fYec5_idx - iZec61[i]) & 511]) + 0.058925565f * fZec62[i] * fYec5[(i + fYec5_idx - iZec63[i]) & 511]) - 0.029462783f * fZec64[i] * fYec5[(i + fYec5_idx - iZec65[i]) & 511]) + 0.0058925566f * fZec66[i] * fYec5[(i + fYec5_idx - iZec67[i]) & 511])));
			}
		}
	}

};

#ifdef FAUST_UIMACROS
	
	#define FAUST_FILE_NAME "scomeotrope.dsp"
	#define FAUST_CLASS_NAME "ScomeotropeDSP"
	#define FAUST_COMPILATION_OPIONS "-lang cpp -ct 1 -cn ScomeotropeDSP -scn  -es 1 -mcd 0 -mdd 1024 -mdy 33 -uim -single -ftz 0 -vec -lv 1 -vs 32"
	#define FAUST_INPUTS 2
	#define FAUST_OUTPUTS 2
	#define FAUST_ACTIVES 14
	#define FAUST_PASSIVES 2

	FAUST_ADDVERTICALSLIDER("CHL03: Tape Channel/Dynamics/Strength", fVslider2, 0.5f, 0.0f, 1.5f, 0.001f);
	FAUST_ADDVERTICALSLIDER("CHL03: Tape Channel/Dynamics/Threshold", fVslider4, -39.89f, -9e+01f, 0.0f, 0.01f);
	FAUST_ADDVERTICALSLIDER("CHL03: Tape Channel/Dynamics/Knee", fVslider5, 8.0f, 0.0f, 4e+01f, 0.01f);
	FAUST_ADDVERTICALSLIDER("CHL03: Tape Channel/Dynamics/Attack", fVslider6, 0.021f, 0.005f, 0.5f, 0.0001f);
	FAUST_ADDVERTICALSLIDER("CHL03: Tape Channel/Dynamics/Release", fVslider7, 0.022f, 0.005f, 0.1f, 0.0001f);
	FAUST_ADDVERTICALSLIDER("CHL03: Tape Channel/Dynamics/Mix", fVslider9, 1.0f, 0.0f, 1.0f, 0.0001f);
	FAUST_ADDVERTICALSLIDER("CHL03: Tape Channel/Dynamics/Make Up", fVslider8, 16.65f, 0.1f, 32.0f, 0.01f);
	FAUST_ADDVERTICALBARGRAPH("CHL03: Tape Channel/Dynamics/GR", fVbargraph0, -2e+01f, 0.0f);
	FAUST_ADDVERTICALBARGRAPH("CHL03: Tape Channel/Dynamics/GR", fVbargraph1, -2e+01f, 0.0f);
	FAUST_ADDVERTICALSLIDER("CHL03: Tape Channel/Machine Decay/Speed", fVslider1, 1.0f, 0.0001f, 13.0f, 0.01f);
	FAUST_ADDVERTICALSLIDER("CHL03: Tape Channel/Machine Decay/Amount", fVslider0, 0.5f, 0.0f, 1.0f, 0.01f);
	FAUST_ADDVERTICALSLIDER("CHL03: Tape Channel/Machine Decay/Mix", fVslider10, 1.0f, 0.0f, 1.0f, 0.01f);
	FAUST_ADDCHECKBOX("CHL03: Tape Channel/Machine Decay/Bypass", fCheckbox0);
	FAUST_ADDVERTICALSLIDER("CHL03: Tape Channel/Noise", fVslider3, 0.15f, 0.0f, 0.5f, 0.0001f);
	FAUST_ADDVERTICALSLIDER("CHL03: Tape Channel/Flutter", fVslider12, 0.01f, 0.0f, 0.032f, 0.001f);
	FAUST_ADDVERTICALSLIDER("CHL03: Tape Channel/Output Gain", fVslider11, 1.6f, 1.0f, 12.0f, 0.001f);

	#define FAUST_LIST_ACTIVES(p) \
		p(VERTICALSLIDER, Strength, "CHL03: Tape Channel/Dynamics/Strength", fVslider2, 0.5f, 0.0f, 1.5f, 0.001f) \
		p(VERTICALSLIDER, Threshold, "CHL03: Tape Channel/Dynamics/Threshold", fVslider4, -39.89f, -9e+01f, 0.0f, 0.01f) \
		p(VERTICALSLIDER, Knee, "CHL03: Tape Channel/Dynamics/Knee", fVslider5, 8.0f, 0.0f, 4e+01f, 0.01f) \
		p(VERTICALSLIDER, Attack, "CHL03: Tape Channel/Dynamics/Attack", fVslider6, 0.021f, 0.005f, 0.5f, 0.0001f) \
		p(VERTICALSLIDER, Release, "CHL03: Tape Channel/Dynamics/Release", fVslider7, 0.022f, 0.005f, 0.1f, 0.0001f) \
		p(VERTICALSLIDER, Mix, "CHL03: Tape Channel/Dynamics/Mix", fVslider9, 1.0f, 0.0f, 1.0f, 0.0001f) \
		p(VERTICALSLIDER, Make_Up, "CHL03: Tape Channel/Dynamics/Make Up", fVslider8, 16.65f, 0.1f, 32.0f, 0.01f) \
		p(VERTICALSLIDER, Speed, "CHL03: Tape Channel/Machine Decay/Speed", fVslider1, 1.0f, 0.0001f, 13.0f, 0.01f) \
		p(VERTICALSLIDER, Amount, "CHL03: Tape Channel/Machine Decay/Amount", fVslider0, 0.5f, 0.0f, 1.0f, 0.01f) \
		p(VERTICALSLIDER, Mix, "CHL03: Tape Channel/Machine Decay/Mix", fVslider10, 1.0f, 0.0f, 1.0f, 0.01f) \
		p(CHECKBOX, Bypass, "CHL03: Tape Channel/Machine Decay/Bypass", fCheckbox0, 0.0f, 0.0f, 1.0f, 1.0f) \
		p(VERTICALSLIDER, Noise, "CHL03: Tape Channel/Noise", fVslider3, 0.15f, 0.0f, 0.5f, 0.0001f) \
		p(VERTICALSLIDER, Flutter, "CHL03: Tape Channel/Flutter", fVslider12, 0.01f, 0.0f, 0.032f, 0.001f) \
		p(VERTICALSLIDER, Output_Gain, "CHL03: Tape Channel/Output Gain", fVslider11, 1.6f, 1.0f, 12.0f, 0.001f) \

	#define FAUST_LIST_PASSIVES(p) \
		p(VERTICALBARGRAPH, GR, "CHL03: Tape Channel/Dynamics/GR", fVbargraph0, 0.0, -2e+01f, 0.0f, 0.0) \
		p(VERTICALBARGRAPH, GR, "CHL03: Tape Channel/Dynamics/GR", fVbargraph1, 0.0, -2e+01f, 0.0f, 0.0) \

#endif

#endif
