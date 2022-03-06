#ifndef __LED_H_
#define __LED_H_

enum LedColor {
	LED_RED = (unsigned short)0x0001,
	LED_GREEN = (unsigned short)0x0002,
};

class Led{
public:
	bool LedInit();
	bool GetLed(unsigned short led_x) const;
	void SetLed(unsigned short led_x, bool state) const;
	void ToggleLed(unsigned short led_x) const;

public:
	static Led& GetInstance(){
		return _instance;
	}

private:
	static Led _instance;
	Led() : led_init_state_(false) {}

private:
	bool led_init_state_;
};

#endif
