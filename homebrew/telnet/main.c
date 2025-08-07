#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <time.h>
#include <math.h>
#include <termios.h>
#include <sys/select.h>
#include <stdint.h>
#include <errno.h>
#include <sys/statvfs.h>
#include <sys/ioctl.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>

#include "font.h"
#include "ui.h"


const char header[] = "This is insecure PAX tcp shell server, /data/app/MAINAPP/busybox must be a copy of busybox.\nThe shell server will terminate when this session closes.\n\n";
char date_str[24]; // Dostatečně velké pole
typedef unsigned short color_t;

color_t black = 0x0000;
color_t white = 0xFFFF;
color_t blue = 0x001F;  // Modrá pro sekundovou ručičku
color_t red = 0xF800;   // Červená pro minutovou ručičku
color_t yellow = 0xFFE0; // Žlutá pro hodinovou ručičku



int _init() {

  printf("Starting insecure PAX telnet server...\n");

  int socket_desc, client_sock, c;
  struct sockaddr_in server, client;
  

printf("Stage 1: Opening framebuffer device...\n");


int fb_fd = open(FB_PATH, O_RDWR);

if (fb_fd == -1) {
    printf("Error opening framebuffer device");
    return EXIT_FAILURE;
}

printf("Stage 2: Reading variable screen info...\n");

struct fb_var_screeninfo vinfo;
if (ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo)) {
    printf("Error reading variable information");
    close(fb_fd);
    return EXIT_FAILURE;
}

printf("Stage 3: Reading fixed screen info...\n");

color_t *fbp = (color_t *)mmap(0, SCREEN_WIDTH * SCREEN_HEIGHT * BPP, PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, 0);
if ((intptr_t)fbp == -1) {
    printf("Error mapping framebuffer device to memory");
    close(fb_fd);
    return EXIT_FAILURE;
}

printf("Stage 4: Allocating memory for buffer...\n");

color_t *buffer = (color_t *)malloc(SCREEN_WIDTH * SCREEN_HEIGHT * BPP);
if (!buffer) {
    printf("Error allocating memory for buffer");
    munmap(fbp, SCREEN_WIDTH * SCREEN_HEIGHT * BPP);
    close(fb_fd);
    return EXIT_FAILURE;
    }

    printf("Stage 5: Clearing buffer...\n");

    clear_buffer(buffer, black); // Vyčištění bufferu
    
    draw_text(buffer, 10,  10, "This is insecure PAX",red);
    draw_text(buffer, 10,  20, "tcp shell server", red);  
    draw_text(buffer, 10,  30, "/data/app/MAINAPP/bin/busybox", blue);   
    draw_text(buffer, 10,  40, "must be a copy of busybox", blue);  
	  draw_text(buffer, 10,  70, "try ls -la ;", white);  
    draw_text(buffer, 10,  80, "Happy hacking .....", white);  
    draw_text(buffer, 10,  90, "IP adresses and IFaces:", white);  
   	draw_text(buffer, 10,  200,"TELNET is running ", white);  
   	draw_text(buffer, 10,  210,"     on port 2325 ", white); 
   	draw_text(buffer, 10,  230,"Escape character is '^]'", white);  
   	draw_text(buffer, 10,  240,"to close session use it", white);  
   	draw_text(buffer, 10,  250,"and command close  :-)", white);  


    int line = 110;
    struct ifaddrs *ifaddr, *ifa;
    struct sockaddr_in *sa;
    char *addr;

    printf("Stage 6: Getting network interfaces...\n");
    if (getifaddrs(&ifaddr) == -1) {
        printf("getifaddrs");
        exit(EXIT_FAILURE);
    }
    printf("Stage 7: Iterating through interfaces...\n");

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL)
            continue;

        if (ifa->ifa_addr->sa_family == AF_INET) {
            sa = (struct sockaddr_in *) ifa->ifa_addr;
            addr = inet_ntoa(sa->sin_addr);
          //  printf("%s: %s\n", ifa->ifa_name, addr);
                snprintf(date_str, sizeof(date_str), "Interface: %s", ifa->ifa_name);
    		// draw_text(buffer, 10, line, date_str, white); // Zobrazení jmena iface
    		line = line + 10;
    		snprintf(date_str, sizeof(date_str), "IP: %s", addr);
    		// draw_text(buffer, 10, line, date_str, white); // Zobrazení jmena iface
    		line = line + 20;
    		
        }
    }
    printf("Stage 8: Freeing interface addresses...\n");

    freeifaddrs(ifaddr);
  //  fflush(stdout);
     memcpy(fbp, buffer, SCREEN_WIDTH * SCREEN_HEIGHT * BPP);

  printf("Stage 9: Setting up socket...\n");
  //Create socket
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_desc == -1) {
    exit(1);
  }

  printf("Stage 10: Binding socket...\n");

  //Prepare the sockaddr_in structure
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(2323);

  // set SO_REUSEADDR on a socket to true (1):
  int optval = 1;
  setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

  //Bind
  if (bind(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0) {
    //print the error message
    printf("bind failed. Error");
    return 1;
  }

  //Listen
  listen(socket_desc, 3);

  //Accept and incoming connection
  c = sizeof(struct sockaddr_in);

  //accept connection from an incoming client
  client_sock =
      accept(socket_desc, (struct sockaddr *) &client, (socklen_t *) &c);
  if (client_sock < 0) {
    exit(1);
  }

  write(client_sock, header, strlen(header));


  // Redirecting stdout, stderr, and stdin to the client socket
  dup2(client_sock, 0); // Redirecting stdin to client socket
  dup2(client_sock, 1); // Redirecting stdout to client socket
  dup2(client_sock, 2); // Redirecting stderr to client socket


// Hiner
 // char *const argv[] = {"/data/app/MAINAPP/bin/busybox", "sh", "-i", NULL};
 // execve("/data/app/MAINAPP/bin/busybox", argv, NULL);

//Andy
  char *const path = "/data/app/MAINAPP/bin/busybox";
  char *const argv[] = {path, "sh", "-is", NULL};
  char *const envp[] = {"PATH=/sbin:/usr/sbin:/bin:/usr/bin:/data/app/MAINAPP/bin:/data/app/MAINAPP", NULL};
  execve(path, argv, envp);



free(buffer);
munmap(fbp, SCREEN_WIDTH * SCREEN_HEIGHT * BPP);
close(fb_fd);
return 0;
}

