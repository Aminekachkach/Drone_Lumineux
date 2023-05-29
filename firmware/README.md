# Drone_Lumineux

## ESC configuration

Pour une configuration du PWM avec une période de 20 ms, soit 50 Hz :

- Période = (PSC + 1) * (ARR + 1) / Fréquence du clock du timer
Soit PSC = 79 , ARR = 19999 pour une Fréquence du clock du timer Fct = 80 Mhz

## PWM Pulse



