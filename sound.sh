#!/bin/bash

rm /tmp/out.wav
touch /tmp/out.wav
tail -f /tmp/out.wav > /dev/tcp/$1/9000