#ifndef __LED_LAYOUT_H__
#define __LED_LAYOUT_H__

#define LAYOUT_SIZE 94
#define LAYOUT_OUTPUT_SIZE 12

class LedLayout {
public:
	LedLayout();
	~LedLayout();
	unsigned char * output();
	void setValue(int index, char value);
	void setVisible(int index, bool visible);
private:
	void setValueBits(int bit, int offset, char value);
	void updateOutput(int offset, int bits);
private:
	char * _layout;
	unsigned char * _output;
};

#endif
