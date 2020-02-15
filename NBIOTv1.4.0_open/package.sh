#!/bin/bash

rm -rf releasebin
mkdir releasebin

cp -f ./calfile/band_1_3_5_8_value.cal ./releasebin
cp -f ./calfile/RF3515_band_5_8_m1.cal ./releasebin
cp -f ./calfile/MQ.cal                 ./releasebin
cp -f ./out/mt2625_evb/nbiot_m2m_demo/flash_download.cfg ./releasebin
cp -f ./out/mt2625_evb/nbiot_m2m_demo/*.bin ./releasebin
cp -f ./out/mt2625_evb/nbiot_m2m_demo/*.dec ./releasebin

