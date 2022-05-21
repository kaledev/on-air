#!/bin/bash
log stream | awk '
                    /ISP_PowerOnCamera: powered on camera/  { system("/opt/homebrew/bin/mosquitto_pub --topic sensor/camera/laptop -h 192.168.86.245 --message \"{ \\\"onair\\\": 1 }\"") }
                    /PowerOffCamera : ISP_PowerOffCamera/   { system("/opt/homebrew/bin/mosquitto_pub --topic sensor/camera/laptop -h 192.168.86.245 --message \"{ \\\"onair\\\": 0 }\"") }'
