# EV3-Video

Using these scripts you can play Videos on your EV3-Brick via WiFi. 
There are two available methods:

### Using mpv

Install the dependencies:

```sh
$ sudo apt-get install mpv screen openssh
```

You need a key-file for root access to the Brick as

```sh
~/.ssh/brick_id_rsa
```

Then you can play the file like this:

```sh
$ ./play.sh <ip> <filename>
ie: $ ./play.sh 172.30.1.27 video.mp4
```

A screen will be opened, you need to wait until a line appears, telling you your Brick is ready to listen to incoming connections.
After the line appering you need to press Ctrl+A and then D.

Soundfiles are able to be played like this aswell, in that case you need to wait before cleanup.

### Better quality using VLC

You need a ssh key for root access to your brick just like in the previous chapter.

Install the dependencies:

```sh
$ sudo apt-get install vlc screen openssh
```
In an Environment not using a graphical interface on the host, these should suffice:
```sh
$ sudo apt-get install vlc-nox screen openssh
```

#### Compilation of the display-driver and filter

Neccessary packages:
```sh
$ sudo apt-get install gcc libc-dev-bin libc6-dev-armel-cross gcc-arm-linux-gnueabi
```

Compilation:

```sh
$ gcc filter.c -o a.out
$ arm-linux-gnueabi-gcc display.c -o b.out
```
then put the display driver on the brick
```sh
$ scp b.out root@<brick-ip>:~/
```

#### Convert videos

Videos need to be in 178 width, as there are problems using non-full-width at the moment. As at least my vlc ignores width and height parameters, conversion sadly is neccessary.
for about 16:9 aspect ratio this should work:
```sh
$ ffmpeg -i <input> -vf scale=w=178:h=128:force_original_aspect_ratio=decrease,"pad=ceil(iw/2)*2:ceil(ih/2)*2":color=white <output>
```

#### Play

You need to know the vertical resolution for this step.

```sh
$ ./playhq.sh <ip> <file> <pixel-width> <height>
```
A useful pixel-width has proved to be around 3 to 7.


## Contribute

Please hit me up for improvements - or create a pull request.

