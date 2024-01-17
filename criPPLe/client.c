#include <windows.h>
#include <stdio.h>
#include "ioctl.h"
#include "core.h"

int main(int argc, char *argv[]) {

  HANDLE drv = CreateFile(L"\\\\.\\H00", GENERIC_WRITE, FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, NULL);

  if (drv == INVALID_HANDLE_VALUE) {
    
    printf("[+] Could not get handle to driver.\n");
    return -1;
    
  }

  printf("[+] Handle in my sights.\n");
  Sleep(2000);

  // Allocate memory for our struct and cast to PTARGET
  
  PTARGET target = (PTARGET)malloc(sizeof(PTARGET));

  if (target == NULL) {

    printf("[+] Could not allocate memory for TARGET.\n");
    return -1;
    
  }
  
  // Set struct data

  target->pid = atoi(argv[1]);

  printf("[+] Calling IOCTL_LOADPROT.\n");

  // Call IOCTL

  BOOL go = DeviceIoControl(drv, IOCTL_LOADPROT, target, sizeof(target), NULL, 0, NULL, NULL);

  if (go) {
    
    printf("[+] IOCTL Called.\n");
    
  } else {

    printf("[+] IOCTL Failed.\n");
    
  }

  printf("[+] Cleaning up...\n");
  CloseHandle(drv);
  free(target);
  
  return 0;
  
}
