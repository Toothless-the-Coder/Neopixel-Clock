# Neopixel-Clock

## Overview

This is a Neopixel clock that has a custom-made PCB at its core. It features an ESP32-C6 as the brains, which pulls time information from Wi-Fi and translates it into LED positions and colors.

It has a voltage level step-up to give the LEDs enough power for the signal. Along similar lines, the board is powered off of its own USB-C port instead of the one on the ESP32, because otherwise it wouldn't be able to handle the power draw of the LEDs.

There is also a power draw sensor to make sure that the LEDs don't overload the copper in the PCB or the wall adapter itself. This shouldn't be an issue, as there should only ever be three LEDs lit up at once, but just in case, this is a good idea.

## Why I Made This

I thought of this idea when looking up different things to make for Half Life Week One. I chose this one because I always lose track of time at my desk, and it looked like a super cool idea.

## Challenges and What I Learned

I had a hard time figuring out how to route the voltage level step-up and power draw sensor, as I have never used these before.

I chose to use a four-channel level step-up from Texas Instruments because it had plenty of overhead in case of a fault, so I'd be less likely to burn it up. I chose the ESP32-C6 because it is a very cost-efficient piece of hardware that also has very good performance, especially when it comes to Wi-Fi.

I learned a lot about why copper trace thicknesses matter for high-ish amp applications like this and why you can't just use the regular thin ones.

## Features

- **Wi-Fi time synchronization:** The ESP32-C6 pulls time information over Wi-Fi.
- **LED time display:** The Neopixel LED ring displays the time using LED positions and colors.
- **Dedicated USB-C power:** The PCB has its own USB-C port to handle the LEDs' power draw.
- **Voltage level shifting:** A level shifter helps the LED signal reach the required voltage.
- **Power monitoring:** A power draw sensor monitors how much power the LEDs are using so the ESP32 can react appropriately.
- **PCB designed for higher current:** The board accounts for the importance of copper trace thickness when handling higher current.

## Bill of Materials (BOM)

The following table lists the parts used in the project, what they do, and their costs.

| Part | What it's for | Qty | Unit Price | Total | Vendor |
| --- | --- | ---: | ---: | ---: | --- |
| [ESP32-C6](https://www.digikey.com/en/products/detail/seeed-technology-co-ltd/113991254/24613066) | To control everything the clock does, especially syncing the time via Wi-Fi | 1 | $5.38 | $5.38 | [DigiKey](https://www.digikey.com/en/products/detail/seeed-technology-co-ltd/113991254/24613066) |
| [LED Ring](https://www.aliexpress.us/item/3256803207953454.html) | This is the part that lights up and will show the user the time | 1 | $3.05 | $3.05 | [AliExpress](https://www.aliexpress.us/item/3256803207953454.html) |
| [USB-C Power Supply](https://www.aliexpress.us/item/3256804439390949.html) | To give the clock the electric breath of life | 1 | $3.74 | $3.74 | [AliExpress](https://www.aliexpress.us/item/3256804439390949.html) |
| [330 ohm resistor](https://www.digikey.com/en/products/detail/stackpole-electronics-inc/CF14JT330R/1741399) | To protect the LEDs from voltage spikes | 1 | $0.10 | $0.10 | [DigiKey](https://www.digikey.com/en/products/detail/stackpole-electronics-inc/CF14JT330R/1741399) |
| [1000uF Capacitor](https://www.digikey.com/en/products/detail/nichicon/UVY1E102MPD/2428012) | To protect the 5V line from voltage drops | 1 | $0.94 | $0.94 | [DigiKey](https://www.digikey.com/en/products/detail/nichicon/UVY1E102MPD/2428012) |
| [PCB](https://cart.jlcpcb.com/shopcart/cart/) | To connect everything together | 1 | $9.43 | $9.43 | [JLCPCB](https://cart.jlcpcb.com/shopcart/cart/) |
| [0.1uF Capacitor](https://www.digikey.com/en/products/detail/kemet/C320C104K5R5TA/818040) | To stabilize the power draw sensor and the voltage step-up chip | 2 | $1.49 | $2.98 | [DigiKey](https://www.digikey.com/en/products/detail/kemet/C320C104K5R5TA/818040) |
| [0.1 ohm resistor](https://www.digikey.com/en/products/detail/yageo/MFR-25FRF52-1R/9138909) | To connect IN+ and IN- on the power draw sensor and make it work | 1 | $0.11 | $0.11 | [DigiKey](https://www.digikey.com/en/products/detail/yageo/MFR-25FRF52-1R/9138909) |
| [5.1k ohm resistor](https://www.digikey.com/en/products/detail/yageo/MFR-25FTE52-5K1/9140307) | To make the USB-C port work by connecting CC1 and CC2 to ground | 2 | $0.10 | $0.20 | [DigiKey](https://www.digikey.com/en/products/detail/yageo/MFR-25FTE52-5K1/9140307) |
| [Voltage step-up](https://www.digikey.com/en/products/detail/texas-instruments/SN74AHCT125N/375798) | To increase the signal voltage from the ESP32 so the LEDs can respond properly | 1 | $1.00 | $1.00 | [DigiKey](https://www.digikey.com/en/products/detail/texas-instruments/SN74AHCT125N/375798) |
| [Power draw sensor](https://www.digikey.com/en/products/detail/texas-instruments/INA219BIDCNR/2596849) | To monitor how much power the LEDs are using and then tell the ESP32 so it can react appropriately | 1 | $2.16 | $2.16 | [DigiKey](https://www.digikey.com/en/products/detail/texas-instruments/INA219BIDCNR/2596849) |
| [USB-C Port](https://www.digikey.ca/en/products/detail/gct/USB4125-GF-A/13547384) | To allow the clock to receive wall power | 1 | $0.91 | $0.91 | [DigiKey](https://www.digikey.ca/en/products/detail/gct/USB4125-GF-A/13547384) |
| **Parts subtotal** | — | — | — | **$30.00** | — |
| **Tax & shipping** | — | — | — | **$0.00** | — |
| **Total** | — | — | — | **$30.00** | — |

## Project Images

### Project Inspiration

![Half Life Week One project inspiration](https://halflife.hackclub-assets.com/hackclub-half-life/sessions/c6Ymgfs9WFtqIB0OnENQKLKARJrHJAeg/80fc74de5ff8dcf25af338c02f701b2edc3e88b9d86cc93c09ba69cd2b52bfe6.png)

### PCB Design

<img width="938" height="914" alt="Neopixel clock PCB design" src="https://github.com/user-attachments/assets/fb30aa63-ab14-4262-88cf-9c5137b3fb08" />

## Final Thoughts

This project has been a great way to learn more about PCB design, power management, and working with components I haven't used before. It was especially interesting to learn how much trace thickness matters when you're dealing with higher current.


