**Project Description:**
---
In this senior Capstone Design project, we will be developing a CubeSat to venture into Low Earth Orbit and gather data an radiation waves 
(Gamma rays and X-rays) while we are in orbit. This GitHub repo is for all of the software that will be developed for the constructed payload 
on the TES-22 and the various commands and features that were built into it.

**Libraries that are used:**
---
Google Protocol Buffers - 
  - We will be using a Google Protocol Buffer .proto file to define how our structure is going to look, an define how the data we will be getting will be stored.

Nanopb -
  - Nanopb library will be used in order to successfully encode and decode the protobuf messages with ease.

We will be using the LoRa_E220 library by Renzo Mischianti 
  - This provides us with code for the LoRa to help us get the LoRa module up an running.
  - Library information can be found on his library GitHub repo [here](https://github.com/xreef/EByte_LoRa_E220_Series_Library?tab=readme-ov-file)
