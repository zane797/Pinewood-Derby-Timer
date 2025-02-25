# Pinewood-Derby-Timer
A firmware and case design for a DIY pinewood derby timer. Long term this will integrate with the Derby Net race management software.

# Bill of Materials

## Main Control Box

|Description|Part|Link|
|-|-|-|
|Main Microcontroller|Raspberry Pi Pico W|[Aliexpress](https://www.aliexpress.us/item/3256805317728363.html?spm=a2g0o.productlist.main.3.22611e4fjFEMLK&algo_pvid=39610c00-e1b9-4aec-9cec-0bbfb9983eaa&algo_exp_id=39610c00-e1b9-4aec-9cec-0bbfb9983eaa-2&pdp_ext_f=%7B%22order%22%3A%228%22%2C%22eval%22%3A%221%22%7D&pdp_npi=4%40dis%21USD%2110.61%216.90%21%21%2110.61%216.90%21%40%2112000033341226132%21sea%21US%211904804129%21X&curPageLogUid=GuRSvJT1bYrf&utparam-url=scene%3Asearch%7Cquery_from%3A)|
|Wifi and Derby Net Host|Raspberry Pi Zero W|[Aliexpress](https://www.aliexpress.us/item/3256805605866860.html?spm=a2g0o.productlist.main.1.4f4c65cfn10L15&algo_pvid=21984dbf-73e7-457b-8d44-a15aec5beb58&algo_exp_id=21984dbf-73e7-457b-8d44-a15aec5beb58-0&pdp_ext_f=%7B%22order%22%3A%223151%22%2C%22eval%22%3A%221%22%7D&pdp_npi=4%40dis%21USD%2116.78%2116.78%21%21%2116.78%2116.78%21%40%2112000034369562374%21sea%21US%211904804129%21X&curPageLogUid=47UVyV3l2OUC&utparam-url=scene%3Asearch%7Cquery_from%3A)
|Timer Screen|P4 64x32 LED Matrix|[Aliexpress](https://www.aliexpress.us/item/3256804589638597.html?spm=a2g0o.order_list.order_list_main.63.1b071802zhrFcb&gatewayAdapt=glo2usa)|
|5V PSU|Meanwell LRS-100-5|[Aliexpress](https://www.aliexpress.us/item/3256805881564763.html?spm=a2g0o.productlist.main.5.3197SrTtSrTtHK&algo_pvid=1ca8c4f1-d31b-49ff-9ce3-bbb0e1239ea2&algo_exp_id=1ca8c4f1-d31b-49ff-9ce3-bbb0e1239ea2-4&pdp_ext_f=%7B%22order%22%3A%22-1%22%2C%22eval%22%3A%221%22%7D&pdp_npi=4%40dis%21USD%2112.60%2112.60%21%21%2191.00%2191.00%21%40%2112000035579855872%21sea%21US%211904804129%21X&curPageLogUid=PKbuf6nNPCEw&utparam-url=scene%3Asearch%7Cquery_from%3A)|
|Pin Connections|Assorted 4 pin JST 2.54 Headers|[Aliexpress](https://www.aliexpress.us/item/3256806894018733.html?spm=a2g0o.order_list.order_list_main.178.1b071802zhrFcb&gatewayAdapt=glo2usa)|
|Sensor Connections|3.5 MM Jack|[Aliexpress](https://www.aliexpress.us/item/3256803435015141.html?spm=a2g0o.order_list.order_list_main.10.63f81802t4SPXu&gatewayAdapt=glo2usa)|
|Voltage Regulator|LD1117V33|[Aliexpress](https://www.aliexpress.us/item/3256806308155209.html?spm=a2g0o.productlist.main.1.5bb46025Ul0rMu&algo_pvid=a7ad22ea-ed0c-4a1c-9f27-c7c9674e1e62&algo_exp_id=a7ad22ea-ed0c-4a1c-9f27-c7c9674e1e62-0&pdp_ext_f=%7B%22order%22%3A%2257%22%2C%22eval%22%3A%221%22%7D&pdp_npi=4%40dis%21USD%218.60%212.49%21%21%2162.09%2118.01%21%40%2112000037812808697%21sea%21US%211904804129%21X&curPageLogUid=uiYgSwbJUKlw&utparam-url=scene%3Asearch%7Cquery_from%3A)|
|Starting Button|16 mm Round Push Button|[Aliexpress](https://www.aliexpress.us/item/2255800895415537.html?spm=a2g0o.productlist.main.7.51c1VwILVwILsh&algo_pvid=ee3e8d61-f0d7-4586-89a3-7286ecbe6f2f&algo_exp_id=ee3e8d61-f0d7-4586-89a3-7286ecbe6f2f-6&pdp_ext_f=%7B%22order%22%3A%22461%22%2C%22eval%22%3A%221%22%7D&pdp_npi=4%40dis%21USD%211.74%211.74%21%21%211.74%211.74%21%40%2110000014240381776%21sea%21US%211904804129%21X&curPageLogUid=q6Y7iC2eUG0V&utparam-url=scene%3Asearch%7Cquery_from%3A)|
|Power Plug|C14 Plug Socket|[Aliexpress](https://www.aliexpress.us/item/2251832772820835.html?spm=a2g0o.productlist.main.2.3d591da2HOevWD&algo_pvid=301f694e-7d78-4547-8c4a-70b1e5217175&algo_exp_id=301f694e-7d78-4547-8c4a-70b1e5217175-1&pdp_ext_f=%7B%22order%22%3A%22107%22%2C%22eval%22%3A%221%22%7D&pdp_npi=4%40dis%21USD%211.77%211.77%21%21%211.77%211.77%21%40%2166368851735%21sea%21US%211904804129%21X&curPageLogUid=OQS6Zu5POJlK&utparam-url=scene%3Asearch%7Cquery_from%3A)


## Sensor Module

|Description|Part|Link|
|-|-|-|
|TOF Sensor|VL6180X|[Aliexpress](https://www.aliexpress.us/item/3256806030301281.html?spm=a2g0o.order_list.order_list_main.148.1b071802zhrFcb&gatewayAdapt=glo2usa)|
|Sensor Microcontroller|ESP8266-ESP01S|[Aliexpress](https://www.aliexpress.us/item/3256805668764137.html?spm=a2g0o.productlist.main.7.2642a9UQa9UQw8&algo_pvid=16431d19-637c-4e04-bde9-98033749f50c&algo_exp_id=16431d19-637c-4e04-bde9-98033749f50c-6&pdp_ext_f=%7B%22order%22%3A%2225%22%2C%22eval%22%3A%221%22%7D&pdp_npi=4%40dis%21USD%210.67%210.67%21%21%210.67%210.67%21%40%2112000034586676462%21sea%21US%211904804129%21X&curPageLogUid=OGikxrhcl3al&utparam-url=scene%3Asearch%7Cquery_from%3A)
|Main Board Connection|3.5 mm TRRS Female|[Aliexpress](https://www.aliexpress.us/item/3256806426250837.html?spm=a2g0o.order_list.order_list_main.153.1b071802zhrFcb&gatewayAdapt=glo2usa)|
|Pin Connections|Assorted 4 pin JST 2.54 Headers|[Aliexpress](https://www.aliexpress.us/item/3256806894018733.html?spm=a2g0o.order_list.order_list_main.178.1b071802zhrFcb&gatewayAdapt=glo2usa)|
|Main Board Cable|3.5 MM TRRS Cable 1ft|[Aliexpress](https://www.aliexpress.us/item/2255799918035646.html?spm=a2g0o.productlist.main.11.42f71f99Hc2X7e&algo_pvid=1045a3ea-71c7-4901-af9f-98f3649a2b68&algo_exp_id=1045a3ea-71c7-4901-af9f-98f3649a2b68-10&pdp_ext_f=%7B%22order%22%3A%22618%22%2C%22eval%22%3A%221%22%7D&pdp_npi=4%40dis%21USD%211.11%211.11%21%21%211.11%211.11%21%40%2110000000271066829%21sea%21US%211904804129%21X&curPageLogUid=soE38LRGgXtO&utparam-url=scene%3Asearch%7Cquery_from%3A)

Not included are assorted resistors and capacitors, as are documented in the circuit schematic.

# Overall Design

## Main Control Box

The goal for this box was to create a replacement Pinewood Derby Track Timer and add a race management software called DerbyNet. The core of this is a Raspberry Pi Pico W with a Raspberry Pi Zero W. The goal is to create a system that reliably times differences of less than 50 ms. The sensors read at ~100Hz and require 2 successive detections of a car, to avoid false readings at the high speed. This means the effective sensing time is roughly .02 seconds. This meets the design goal of sensing times of less than 0.05 seconds. 

The original design included an I2C multiplexer routed directly to the VL6180Xs in the sensor modules. This led to sensing issues and bad data reads. I suspect this was due to using I2C through several connections and wire types. This was removed and in it's place I am using simple digital command and input signal lines to command the ESP-01S modules accordingly. These have been much more reliable and once the architecture change happened reliability went up significantly.

The display is a LED matrix screen using a HUB75 interface which makes the wiring more painful than the I2C interface most lightweight hobbyists like myself are used to for project displays. I think if I were to make a version 2, I would create a custom PCB for the PCB and include a HUB75 socket to make the wiring process much easier. Aside from the interface, the LED Matrix as a display worked incredibly well for my. It's easily readable even in well lit rooms, and fit perfectly in my model. I think the only thing I would change is pulling in the display a bit so it doesn't protrude from the chassis.

## Sensor Module

The sensor module started as just a VL6180X on a protoboard, with a couple pull up resistors and a JST connector for the wire. This turned out to be a reliability nightmare and for the initial release I updated it to have a dedicated ESP8266 in a ESP-01S package, which had exactly the number of IO pins for what I needed. The ESP-01S interfaced with the VL6180X directly and signaled the main control box when a car was detected.

