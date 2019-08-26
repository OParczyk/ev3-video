#!/bin/bash

tail -f fb.out | ./a.out $1 $2 > /dev/tcp/ev/9000