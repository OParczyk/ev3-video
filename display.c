#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <unistd.h>
#include <stdio.h>

#define WIDTH 178
#define HEIGHT 128
#define LINE_BLANK 0

int main(int argc, char **argv)
{
   int row, col, width, height, bytespp;
   unsigned int *data;

   // Öffnen des Gerätes
   int fd = open("/dev/fb0", O_RDWR);

   // Informationen über den Framebuffer einholen
   struct fb_var_screeninfo screeninfo;
   ioctl(fd, FBIOGET_VSCREENINFO, &screeninfo);

   width = screeninfo.xres;
   height = screeninfo.yres;
   bytespp = sizeof(unsigned int); //hoffentlich

   // Zeiger auf den Framebufferspeicher anfordern
   data = (unsigned int *)mmap(0, width * height * bytespp, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

   //blank
   for (int i = 0; i < WIDTH * HEIGHT; i++)
   {
      data[i] = 0xFFFFFF;
   }

   unsigned char ch, step, frame_height, frame_width, h_count = 0;
   unsigned int count = 0;
   read(STDIN_FILENO, &step, 1);         //step einlesen (subsampling)
   read(STDIN_FILENO, &frame_height, 1); //bildhoehe einlesen (fuer H-blanking)
   read(STDIN_FILENO, &frame_width, 1);  //bildbreite (v-blank)
   while (read(STDIN_FILENO, &ch, 1) > 0)
   {
      if (h_count >= frame_width)
      {
         count += width - h_count;
         h_count = 0;
      }
      if (count > WIDTH * frame_height)
         count = 0;
      if (h_count + step >= WIDTH)
      {
         for (unsigned char i = 0; i < WIDTH - h_count; i++)
         {
            data[count / WIDTH * (width + LINE_BLANK) + count % WIDTH + i] = 0x010101 * ch;
         }
         count += step;
         h_count = count % WIDTH;
         continue;
      }

      for (unsigned char i = 0; i < step; i++)
         data[count / WIDTH * (width + LINE_BLANK) + count % WIDTH + i] = 0x010101 * ch;
      count += step;
      h_count += step;
   }
   // Zeiger wieder freigeben
   munmap(data, width * height * bytespp);

   // Gerät schließen
   close(fd);
   // Rückgabewert
   return 0;
}