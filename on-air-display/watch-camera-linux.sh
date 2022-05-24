#!/usr/bin/env bash
echo 'Running Script'
camera_on=false

while :
do

  until [ $camera_on = true ]
  do
    camera_on=$(lsmod | awk '{print $1,$3}' | grep uvcvideo | awk '{if ($2 == 1) print "true"; else print "false";}')
    sleep 2
  done

  mosquitto_pub --topic sensor/camera/laptop -h 192.168.86.245 --message "{\"onair\": 1}"

  until [ $camera_on = false ]
  do
    camera_on=$(lsmod | awk '{print $1,$3}' | grep uvcvideo | awk '{if ($2 == 1) print "true"; else print "false";}')
    sleep 2
  done

  mosquitto_pub --topic sensor/camera/laptop -h 192.168.86.245 --message "{\"onair\": 0}"

done
