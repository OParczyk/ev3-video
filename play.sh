#!/bin/bash

ssh root@$1 -i ~/.ssh/brick_id_rsa "pkill -f brickman"

screen -dmS a ssh root@$1 -i ~/.ssh/brick_id_rsa "nc -vv -l 9000 | aplay -v -"
screen -mS b ssh root@$1 -i ~/.ssh/brick_id_rsa "nc -vv -l 9001 > /dev/tty1"

screen -dmS c ./sound.sh $1

mpv -vo=tct -ao=pcm --ao-pcm-file=/tmp/out.wav --vo-tct-width=43 --vo-tct-height=21 --ao-pcm-append=no --video-sync=desync --audio-samplerate=12000 --initial-audio-sync $2 > /dev/tcp/$1/9001


echo -n "cleanup?"
read

#cleanup
ssh root@$1 -i ~/.ssh/brick_id_rsa "pkill -f aplay"
ssh root@$1 -i ~/.ssh/brick_id_rsa "pkill -f nc"
screen -S c -p 0 -X quit
ssh root@$1 -i ~/.ssh/brick_id_rsa "screen -dmS brickman brickman"