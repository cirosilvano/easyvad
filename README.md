# easyvad
Extremely simple implementation of a voice activity detection algorithm.

## How does it work?

### Basics
easyvad is a quick implementation of the algorithm described by the paper "Approach for Energy-Based Voice Detector with Adaptive Scaling Factor". 
It assumes that the audio is packetized, analyzes each packet and decides whether it is a relevant voice packet or a noise packet. 

### Implementation details
The function is built to work with 8-bit, signed int encoded samples, meaning that each audio sample can range in values from -128 to 127. This is easily modifiable by changing the int8_t variable type to intX_t, with X being the size of each sample.
Since the algorithm uses packet signal energy as its sole analyzed quantity, the software can also work for unsigned (positive only) samples, as signal energy is a purely positive quantity. To use unsigned samples, simply add a "u" before the variable type, e.g. uint8_t.

## Conclusion
This software is still in development, major tweaks are to be implemented in order for it to be easily usable.

## Todo
1. Wrap external function values in a struct
2. Add how-to-use guide