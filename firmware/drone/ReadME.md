# Neopixel Driver

First, we need to identify the type of LED : WS2812

Neopixels will read a logic 1 if the 800khz pwm cycle recived is 64%, they will read logic 0 if the duty cycle is 29%.

APB1 Timer clock is at 80 Mhz.
So, if we have a 80 Mhz pwm and we want a 0, 8 Mhz pwm we need a 80 /0,8 = 100 preescaler

