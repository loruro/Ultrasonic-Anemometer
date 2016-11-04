# Ultrasonic Anemometer

<div align="center"><img src="img/outside_1.jpg" width="480"/></div>  
This is Ultrasonic Anemometer project based on AVR ATmega1284 microcontroller.

## Principle of operation

Device measures wind velocity using ultrasounds. Two ultrasonic transducers installed on the end of both pipes send 40 kHz ultrasound signal between each other. Normally the velocity of this signal is the same for both directions. It is equal to the speed of sound, which depends mainly on the air temperature. For dry air and 1000 hPa pressure it can be calculated from:
<div align="center"><img src="img/formulas/speed_of_sound.png" height="28"/></div>
where:
* vₚ - speed of sound [m/s]
* θ - air temperature [°C]

At 20 °C it is 343,96 m/s.

If there is a wind, it alters velocity of sound wave propagating in direction parallel to the wind:
<div align="center"><img src="img/formulas/wind_sound_velocity_1.png" height="56"/></div>
where:
* v₁₂ - velocity of sound wave between point 1 and point 2
* v₂₁ - velocity of sound wave between point 2 and point 1
* vₚ - speed of sound
* vᵥᵥ - speed of wind

Above equations can be transformed into:
<div align="center"><img src="img/formulas/wind_sound_velocity_2.png" height="100"/></div>

The velocity of sound wave can be acquired by measuring time of flight of this wave between ultrasonic transducers. This is what this device does. First transducer emits ultrasonic impulse and second one receives it. Time between those events is measured and later the situation is reversed. Second transducers emits and first one receives.  
Knowing distance between those transducers, velocity of sound can be finally calculated.
<div align="center"><img src="img/formulas/wind_sound_velocity_3.png" height="49"/></div>
where:
* d - distance between transducers
* t - time of flight

Velocities in previous equations can be substituted by above time formula:
<div align="center"><img src="img/formulas/wind_sound_velocity_4.png" height="107"/></div>

<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/"><img alt="Creative Commons Licence" style="border-width:0" src="https://i.creativecommons.org/l/by-sa/4.0/88x31.png" /></a><br /><span xmlns:dct="http://purl.org/dc/terms/" property="dct:title">Ultrasonic Anemometer</span> by <span xmlns:cc="http://creativecommons.org/ns#" property="cc:attributionName">Karol Leszczyński</span> is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/">Creative Commons Attribution-ShareAlike 4.0 International License</a>.
