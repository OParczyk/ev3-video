#!/bin/bash
#usage: playhq.sh <ip> <file> <pixel-width> <height>

ssh root@$1 -i ~/.ssh/brick_id_rsa "pkill -f brickman"
screen -dmS a ssh root@$1 -i ~/.ssh/brick_id_rsa "nc -vv -l 9001 | aplay -v -f s16_le -r 44100 -M -c 1 -"
scp ./b.out root@$1:~/
screen -mS b ssh root@$1 -i ~/.ssh/brick_id_rsa "nc -vv -l 9000 | ./b.out"
screen -dmS c ./filter.sh $3 $4

mkfifo ./fb.out

cvlc --aout=afile --quiet-synchro --audiofile-file=- --audiofile-format=s16 --audiofile-channels=1 --no-audiofile-wav --sout-mono-downmix --sout-mono-channel=0  --sout-transcode-width=178 --sout-transcode-height=128 --vout=yuv --yuv-file=./fb.out --yuv-chroma=J420 --no-yuv-yuv4mpeg2 --video-filter=fps --fps-fps=5 $2 > /dev/tcp/$1/9001

echo -n "cleanup?"
read

#cleanup
ssh root@$1 -i ~/.ssh/brick_id_rsa "pkill -f aplay"
ssh root@$1 -i ~/.ssh/brick_id_rsa "pkill -f nc"
screen -S c -p 0 -X quit
ssh root@$1 -i ~/.ssh/brick_id_rsa "screen -dmS brickman brickman"