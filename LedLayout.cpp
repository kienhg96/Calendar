#include "LedLayout.h"
#include <math.h>

static int LayoutOffsets[] = {
	0, 3, 10, 11, 18, 25, 32, 34, 41, 48, 55, 58, 65, 66, 73, 80, 87
};

int indexBits(int index) {
	switch (index) {
		case 2: case 12:
			return 1;
		case 6:
			return 2;
		case 0: case 10:
			return 3;
		default:
			return 7;
	}
}

LedLayout::LedLayout() {
	_layout = new char[LAYOUT_SIZE];
	_output = new char[LAYOUT_OUTPUT_SIZE];
	for (int i = 0; i < LAYOUT_SIZE; i++) {
		_layout[i] = 0;
	}
	for (int i = 0; i < LAYOUT_OUTPUT_SIZE; i++) {
		_output[i] = 0;
	}
}

LedLayout::~LedLayout() {
	delete[] _layout;
	delete[] _output;
}

unsigned char * LedLayout::output() {
	return _output;
}

void LedLayout::setValue(int index, char value) {
	int offset = LayoutOffsets[index];
	int bits = indexBits(index);
	setValueBits(bits, offset, value);
	updateOutput(offset, bits);
}

void LedLayout::setVisible(int index, bool visible) {
	int offset = LayoutOffsets[index];
	int bits = indexBits(index);
	if (visible) {
		updateOutput(offset, bits);
	} else {
		for (int i = 0; i < bits; i++) {
			_output[offset + i] = 0;
		}
	}
}

void LedLayout::setValueBits(int bit, int offset, char value) {
	for (int i = 0; i < bit; i++) {
		_layout[i] = (value >> i) & 1;
	}
}

void LedLayout::updateOutput(int offset, int bits) {
	int start = offset / 8;
	int end = (int) ceil((offset + bits) / 8.0f);
	if (end >= LAYOUT_OUTPUT_SIZE) {
		end = LAYOUT_OUTPUT_SIZE;
	}

	for (int i = start; i < end; i++) {
		int baseIndex = i * 8;
		unsigned char val = 0;
		for (int j = 0; j < 8; j++) {
			int index = baseIndex + j;
			char layoutVal = (index < LAYOUT_SIZE) ? _layout[index] : 0;
			val = (val << 1) + layoutVal;
		}
		_output[i] = val;
	}
}
