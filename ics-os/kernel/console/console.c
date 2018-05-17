/* 
   ==========================================================================
   Console.c
   Author: Joseph Emmanuel Dayo
   Date updated:December 6, 2002
   Description: A kernel mode console that is used for debugging the kernel
   and testing new kernel features.
                
    DEX educational extensible operating system 1.0 Beta
    Copyright (C) 2004  Joseph Emmanuel DL Dayo

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. 
   ==========================================================================
*/

#include "console.h"

//end of the months
int daysInMonth[]={0,31,28,31,30,31,30,31,31,30,31,30,31};

//month formats 
char *months[]={
	" ",
	"\n\n\nJanuary",
	"\n\n\nFebruary",
	"\n\n\nMarch",
	"\n\n\nApril",
	"\n\n\nMay",
	"\n\n\nJune",
	"\n\n\nJuly",
	"\n\n\nAugust",
	"\n\n\nSeptember",
	"\n\n\nOctober",
	"\n\n\nNovember",
	"\n\n\nDecember"
};

//month for printing
char *noNewLineMonths[]={
	" ",
	"January",
	"February",
	"March",
	"April",
	"May",
	"June",
	"July",
	"August",
	"September",
	"October",
	"November",
	"December"
};

//abbreviated month format
char *abbrvMonths[]={
	" ",
	"Jan",
	"Feb",
	"March",
	"April",
	"May",
	"June",
	"July",
	"Aug",
	"Sept",
	"Oct",
	"Nov",
	"Dec"
};

//abbrv day format 
char *days[]={
	"Sun",
	"Mon",
	"Tue",
	"Wed",
	"Thurs",
	"Fri",
	"Sat"
};

//full day format
char *fullDays[]={
	"Sunday",
	"Monday",
	"Tuesday",
	"Wednesday",
	"Thursday",
	"Friday",
	"Saturday"
};


double powerOfTen(int num){
     double result = 1.0;
     if(num >= 0){
         for(int i = 0; i < num ; i++){
             result *= 10.0;
         }
     }else{
         for(int i = 0; i < (0 - num ); i++){
            result *= 0.1;
        }
    }
    return result;
 }

double squareRoot(double a)
{
     double z = a; 
     double result = 0.0;
     int max = 8;     // to define maximum digit 
     int i;
     double j = 1.0;
     for(i = max ; i > 0 ; i--){
         // value must be bigger then 0
        if(z - (( 2 * result ) + ( j * powerOfTen(i)))*( j * powerOfTen(i)) >= 0)
         {
             while( z - (( 2 * result ) + ( j * powerOfTen(i)))*( j * powerOfTen(i)) >= 0)
             {
                 j++;
                 if(j >= 10) break;

             }
             j--; //correct the extra value by minus one to j
             z -= (( 2 * result ) + ( j * powerOfTen(i)))*( j * powerOfTen(i)); //find value of z
 
             result += j * powerOfTen(i);     // find sum of a

             j = 1.0;
 
 
           }
 
      }

      for(i = 0 ; i >= 0 - max ; i--){
          if(z - (( 2 * result ) + ( j * powerOfTen(i)))*( j * powerOfTen(i)) >= 0)
          {
              while( z - (( 2 * result ) + ( j * powerOfTen(i)))*( j * powerOfTen(i)) >= 0)
              {
                  j++;

              }
              j--;
              z -= (( 2 * result ) + ( j * powerOfTen(i)))*( j * powerOfTen(i)); //find value of z
 
              result += j * powerOfTen(i);     // find sum of a
              j = 1.0;
           }
      }
      // find the number on each digit
      return result;
 }

#define LIMIT 1000   //limit of stack 

int stack[LIMIT]; //define stack array with limit of 1000
int TOS = 0; //TOS is initalized to 0


 void push(int x){
  if(TOS<LIMIT){  //if TOS  is less than the limit, you can still push
    stack[TOS] = x;  //stack[TOS] is initialized to x
    TOS++; //TOS is incremented
  }else{
    printf("\nInvalid Command!");
    exit(1);
  }
}

int pop(){
  if(TOS>0){   //if TOS > 0, you can push - stack has elements
    return stack[--TOS];   //returns the element that is being popped
  }else{
    printf("\nInvalid Command!");
    exit(1);
  }
}  

/*A console mode get string function terminates
upon receving \r */
void getstring(char *buf, DEX32_DDL_INFO *dev){
   unsigned int i=0;
   char c;
   do{
      c=getch();
      if (c=='\r' || c=='\n' || c==0xa) 
         break;

      if (c=='\b' || (unsigned char)c == 145){
         if(i>0){
            i--;
            if (Dex32GetX(dev)==0){
               Dex32SetX(dev,79);
               if (Dex32GetY(dev)>0) 
                  Dex32SetY(dev,Dex32GetY(dev)-1);
            }else{
               Dex32SetX(dev,Dex32GetX(dev)-1);
            }     
            Dex32PutChar(dev,Dex32GetX(dev),Dex32GetY(dev),' ',Dex32GetAttb(dev));
         };
      }else{
         if (i<256){  //maximum command line is only 255 characters
            Dex32PutChar(dev,Dex32GetX(dev),Dex32GetY(dev),buf[i]=c,Dex32GetAttb(dev));
            i++;
            Dex32SetX(dev,Dex32GetX(dev)+1);     
            if (Dex32GetX(dev)>79){
               Dex32SetX(dev,0);
               Dex32NextLn(dev);
            };
         };
      };

      Dex32PutChar(dev,Dex32GetX(dev),Dex32GetY(dev),' ',Dex32GetAttb(dev));
      update_cursor(Dex32GetY(dev),Dex32GetX(dev));
   }while (c!='\r');
    
   Dex32SetX(dev,0);
   Dex32NextLn(dev);
   buf[i]=0;
};

/*Show information about memory usage. This function is also useful
  for detecting memory leaks*/
void meminfo(){
   DWORD totalbytes = totalpages * 0x1000;
   DWORD freebytes = totalbytes - (totalpages - stackbase[0])* 0x1000;
   printf("=================Memory Information===============\n");
   printf("Pages available     : %10u pages\n",stackbase[0]);
   printf("Total Pages         : %10d pages\n",totalpages);
   printf("Total Memory        : %10u bytes (%10d KB)\n",totalbytes, totalbytes / 1024);
   printf("Free Memory         : %10u bytes (%10d KB)\n",freebytes, freebytes / 1024);
   printf("Used pages          : %10d pages (%10d KB)\n",totalpages-stackbase[0],
   (totalpages-stackbase[0])*0x1000);
};


int delfile(char *fname){
   int sectors;
   file_PCB *f=openfilex(fname,FILE_WRITE);
   return fdelete(f);
};

int user_fork(){
   int curval = current_process->processid;
   int childready = 0, retval = 0;
   int hdl;
   int id;
   DWORD flags;
   #ifdef DEBUG_FORK
   printf("user_fork called\n");
   #endif
    
   //enable interrupts since we want the process dispatcher to take control
   storeflags(&flags);
   startints();
   
   hdl = pd_forkmodule(current_process->processid);
    
   taskswitch();  
   id = pd_dispatched(hdl);
    //while (!(id=pd_dispatched(hdl)))
    //  ;
    
   if (curval != current_process->processid){ //this is the child
      //If this is the child process, the processid when this function
      //was called is not equal to the current processid.
      retval = 0;
   };
      
   if (curval == current_process->processid){ // this is the parent
      pd_ok(hdl);
      retval = id;
   };
      
   restoreflags(flags);
   return retval;
};

int user_execp(char *fname, DWORD mode, char *params){
   DWORD id,size;
   char *buf;
   vfs_stat filestat;
   file_PCB *f = openfilex(fname, 0);
  
   if (f!=0){
      fstat(f,&filestat);
      size = filestat.st_size;
      buf = (char*)malloc(filestat.st_size+511);
      //tell the vfs to increase buffersize to speed up reads
      vfs_setbuffer(f, 0, filestat.st_size, FILE_IOFBF);
      if (fread(buf, size, 1, f) == size){
         char temp[255];
         #ifdef DEBUG_USER_PROCESS
         printf("execp(): adding module..\n");
         #endif
         int hdl= addmodule(fname, buf, userspace, mode, params, showpath(temp), getprocessid());
        
         #ifdef DEBUG_USER_PROCESS
         printf("execp(): done.\n");
         #endif

         taskswitch();
         #ifdef DEBUG-USER_PROCESS
         printf("execp(): parent waiting for child to finish\n");
         #endif
        
         while (!(id = pd_ok(hdl))) 
            ; //process already running?
         
         fg_setmykeyboard(id);
         dex32_waitpid(id,0);

         //dex32_wait();

         fg_setmykeyboard(getprocessid());
      };
      free(buf);
      fclose(f);
      return id;
   };
   return 0;
};

int exec(char *fname, DWORD mode, char *params){
   DWORD id;
   char *buf;
   file_PCB *f=openfilex(fname,0);

   if (f!=0){
      DWORD size;
      char temp[255];
      vfs_stat fileinfo;
      fstat(f,&fileinfo);
      size = fileinfo.st_size;
     
      buf=(char*)malloc(size+511);
     
      if (fread(buf,size,1,f) == size)
         id=dex32_loader(fname, buf, userspace, mode, params, showpath(temp), getprocessid());
      
      free(buf);
      fclose(f);
      return id;
   };
   return 0;
;};

int user_exec(char *fname,DWORD mode,char *params){
   DWORD id;
   char *buf;
   file_PCB *f = openfilex(fname, FILE_READ);

   if (f != 0){
      int hdl,size;
      char temp[255];
      vfs_stat fileinfo;
      fstat(f, &fileinfo);
      size = fileinfo.st_size;
      buf=(char*)malloc(size+511);
      fread(buf, size, 1, f);
      hdl = addmodule(fname, buf, userspace, mode, params, showpath(temp), getprocessid());
      while (!pd_ok(hdl)) 
         ;
     
      free(buf);
      fclose(f);
      return id;
  };
  return 0;
};


int loadDLL(char *name, char *p){
   file_PCB *handle;
   int fsize; vfs_stat filestat;
   int hdl,libid;
   char *buf;

   handle=openfilex(name,FILE_READ);
   if (!handle) 
      return -1;
   fstat(handle,&filestat);
   vfs_setbuffer(handle,0,filestat.st_size,FILE_IOFBF);
   //get filesize and allocate memory
   fsize= filestat.st_size;
   buf=(char*)malloc(fsize);
 
   //load the file from the disk 
   fread(buf, fsize, 1, handle);
 
   /*Tell the process dispatcher to map the file into memory and
      create data structures necessary for managing dynamic libraries*/
   hdl = addmodule(name, buf, lmodeproc, PE_USERDLL, p, 0, getprocessid());
 
   //wait until the library has been loaded before we continue since addmodule returns immediately
   while (!(libid = pd_ok(hdl)))
      ;
 
   //done!
   free(buf);

   //close the file
   fclose(handle);
   return libid;
};

void loadfile(char *s,int,int);

void loadlib(char *s){
   char *buf;
   DWORD size;
   loadDLL(s,0);
};


int console_showfile(char *s, int wait){
   char *buf;
   DWORD size;
   file_PCB *handle;
   vfs_stat fileinfo;
   int i;
   DEX32_DDL_INFO *myddl;
   handle=openfilex(s, FILE_READ);

   if (!handle) 
      return -1;
   fstat(handle,&fileinfo);
   size = fileinfo.st_size;
   buf=(char*)malloc(size);
   textbackground(BLUE);
   myddl = Dex32GetProcessDevice();
   printf("Name: %s  Size: %d bytes \n", s, size);
   textbackground(BLACK);
   fread(buf, size, 1, handle);
   for (i=0; i<size; i++){
      if (buf[i]!='\r') 
         printf("%c",buf[i]);
      if (myddl->lines%25==0){ 
         char c;
         printf("\nPress any key to continue, 'q' to quit\n");
         c=getch();
         if (c=='q') 
            break;
      };
   };
   fclose(handle);
   free(buf);
   return 1;
};


//creates a virtual console for a process
DWORD alloc_console(){
   dex32_commit(0xB8000, 1, current_process->pagedirloc, PG_WR);
};

void console(){
   console_main();
};


void prompt_parser(const char *promptstr, char *prompt){
   int i, i2 = 0, i3 = 0;
   char command[10], temp[255];
   strcpy(prompt,"");
   for (i=0; promptstr[i] && i<255; i++){
      if (promptstr[i] != '%'){ //add to the prompt
         prompt[i2]=promptstr[i];
         i2++;
         prompt[i2]=0;
      }else{
         if (promptstr[i+1] != 0){
            if (promptstr[i+1] == '%'){
               prompt[i2] = '%';
               i2++;
               prompt[i2] = 0;
               i+=2;
               continue;
            };
         };
         i3=0;
         for (i2=i+1; promptstr[i2]&&i2 < 255; i2++){
            if (promptstr[i2] == '%' || i3 >= 10) 
               break;
            command[i3]=promptstr[i2];
            i3++;
         };
         i=i2;
         command[i3]=0;
         if (strcmp(command,"cdir")==0){
            strcat(prompt, showpath(temp));
            i2=strlen(prompt);
         };
      };
   };
};
  



/*An auxillary function for qsort for comparing two elements, based on size*/
int console_ls_sortsize(vfs_node *n1, vfs_node *n2){
   if (n1->size > n2->size) 
      return -1;
   if (n2->size > n1->size) 
      return 1;
   return 0;
};

/*An auxillary function for qsort for comparing two elements, based on name*/
int console_ls_sortname(vfs_node *n1, vfs_node *n2){
   if ( (n1->attb & FILE_DIRECTORY) && !(n2->attb & FILE_DIRECTORY))
      return -1;

   if ( !(n1->attb & FILE_DIRECTORY) && (n2->attb & FILE_DIRECTORY))
      return 1;
        
   return strsort(n1->name,n2->name);
};

/* ==================================================================
   console_ls(int style):
   
   *list the contents of the current directory to the screen
    style = 1      : list format 
    style = others : wide format  
*/
/*lists the files in the current directory to the console screen*/
void console_ls(int style, int sortmethod){
   vfs_node *dptr=current_process->workdir;
   vfs_node *buffer;
   int totalbytes=0, freebytes=0;
   int totalfiles=0, i;
   char cdatestr[20], mdatestr[20], temp[20];
    
   //obtain total number of files
   totalfiles = vfs_listdir(dptr, 0, 0);
    
   buffer = (vfs_node*) malloc( totalfiles * sizeof(vfs_node));

   //Place the list of files obtained from the VFS into a buffer
   totalfiles = vfs_listdir(dptr, buffer, totalfiles * sizeof(vfs_node));     
    
   //Sort the list
   if (sortmethod == SORT_NAME)
      qsort(buffer, totalfiles, sizeof(vfs_node), console_ls_sortname);
   else if (sortmethod == SORT_SIZE)
      qsort(buffer, totalfiles, sizeof(vfs_node), console_ls_sortsize);
        
   textbackground(BLUE);
   textcolor(WHITE);
    
   if (style==1)
      printf("%-25s %10s %14s %14s\n","Filename", "Size(bytes)", "Attribute", "Date Modified");
        
   textbackground(BLACK);

   for (i=0; i < totalfiles; i++){
      char fname[255];   
      if (style == 0){ //wide view style
         if (buffer[i].attb&FILE_MOUNT)
            textcolor(LIGHTBLUE);
         else if (buffer[i].attb&FILE_DIRECTORY)
            textcolor(GREEN);
         else if (buffer[i].attb&FILE_OEXE)
            textcolor(YELLOW);
         else
            textcolor(WHITE);

         strcpy(fname,buffer[i].name);
         fname[24]=0;
         printf("%-25s ",fname);
         totalbytes+=buffer[i].size;
            
         if ( (i+1)%3==0 && (i+1 < totalfiles) ) 
            printf("\n");

      };

      if (style == 1){ //list view style
         if (buffer[i].attb&FILE_MOUNT)
            textcolor(LIGHTBLUE);
         else if (buffer[i].attb&FILE_DIRECTORY)
            textcolor(GREEN);
         else if (buffer[i].attb&FILE_OEXE)
            textcolor(YELLOW);
         else
            textcolor(WHITE);
                    
         strcpy(fname,buffer[i].name);
         fname[24]=0;
         printf("%-25s ",fname);
            
         textcolor(WHITE);
         printf("%10d %14s %14s\n",buffer[i].size,
         vfs_attbstr(&buffer[i],temp), datetostr(&buffer[i].date_modified,
                       mdatestr));
                       
         totalbytes+=buffer[i].size;


         //try to make it fit the screen
         if ((i+1) % 23==0){
            char c;
            printf("Press Q to quit or any other key to continue ...");
            c=getch();
            printf("\n");
            if (c=='q'||c=='Q') 
               break;
         };
      };       
   };
    
   textcolor(WHITE);
   printf("\nTotal Files: %d  Total Size: %d bytes\n", totalfiles, totalbytes);
   free(buffer);
    
};

/* ==================================================================
   console_execute(const char *str):
   * This command is used to execute a console string.

*/
int console_execute(const char *str){
   char temp[512];
   char *u;
   int command_length = 0;
   signed char mouse_x, mouse_y, last_mouse_x=0, last_mouse_y=0;
  
   //make a copy so that strtok wouldn't ruin str
   strcpy(temp,str);
   u=strtok(temp," ");
  
   if (u == 0) 
      return;
  
   command_length = strlen(u);    
    
   //check if a pathcut command was executed
   if (u[command_length - 1] == ':'){
      char temp[512];
      sprintf(temp,"cd %s",u);            
      console_execute(temp); 
   }else 
   if (strcmp(u,"fgman") == 0){  //--  Foreground manager
      fg_set_state(1);
   }else 
   if (strcmp(u,"mouse") == 0){  //--  Activate the mouse
      while (!kb_ready()){
         get_mouse_pos(&mouse_x,&mouse_y);
         printf("Mouse (x,y): %d %d\n",mouse_x, mouse_y);
         while ((last_mouse_x == mouse_x) && (last_mouse_y==mouse_y)){
            get_mouse_pos(&mouse_x,&mouse_y);
         }
         last_mouse_x=mouse_x;
         last_mouse_y=mouse_y; 
      }
   }else 
   if (strcmp(u,"shutdown") == 0){  //-- Shuts down the system.
      sendmessage(0,MES_SHUTDOWN,0);
   }else
   if (strcmp(u,"procinfo") == 0){  //-- Show process information. Args: <pid>
      int pid;             
      u=strtok(0," ");
      if (u!=0){
         pid = atoi(u);
         show_process_stat(pid);
      };
   }else
   if (strcmp(u,"meminfo") == 0){   //-- Show memory map information.
      mem_interpretmemory(memory_map,map_length);
   }else
   if (strcmp(u,"pause") == 0){     //-- Waits for a key press
      printf("press any key to continue or 'q' to quit..\n");
      if (getch() == 'q') 
         return -1;
   }else
   if (strcmp(u,"lspcut") == 0){    //-- Shows a list of path aliases. 
      vfs_showpathcuts();
   }else
   if (strcmp(u,"pcut") == 0){      //-- Creates a path alias. Args: <alias:> [path]
      char *u2,*u3;
      u2 = strtok(0," ");
      u3 = strtok(0," ");
      if (u2 != 0){
         if (vfs_addpathcut(u2,u3) == -1){
            printf("Invalid pathcut specified.\n");
         }else{
            printf("Pathcut added.\n"); 
         }                               
      }else{
         printf("Wrong number of parameters specified.\n");
      }
   }else
   if (strcmp(u,"rmdir") == 0){     //-- Removes a directory and all its subdirectories. Args: <dirname>
      char *u2 = strtok(0," ");
      if (u2 != 0){
         char c;                
         printf("*Warning!* This will delete all files and subdirectories!\n");
         printf("Do you wish to continue? (y/n):");
         c = getch();
         if (c == 'y'){
            printf("Please wait..\n");                        
            if (rmdir(u2) != -1)
               printf("Remove directory successful!\n");
            else
               printf("Error while removing directory.\n");
         }else{
            printf("Remove directory cancelled.\n");
         }
      }else{
         printf("Invalid parameter.\n"); 
      }
   }else
   if (strcmp(u,"rempcut") == 0){   //-- Removes a path alias. Args: <alias:>
      char *u2;
      u2 = strtok(0," ");
      if (u2 != 0){
         if (vfs_removepathcut(u2) == -1)
            printf("Invalid Pathcut or pathcut not found\n");
         else
            printf("Pathcut removed.\n");   
      }else{
         printf("Wrong number of parameters specified\n");
      }               
   }else
   if (strcmp(u,"newconsole") == 0){   //-- Creates a new console.  
      //create a new console         
      console_new();
      printf("New console thread created.\n");                   
   }else  
   if (strcmp(u,"ver") == 0) {         //-- Shows version information.
      printf("%s",dex32_versionstring);
   }else
   if (strcmp(u,"cpuid") == 0){        //-- Displays CPU information. 
      hardware_cpuinfo mycpu;
      hardware_getcpuinfo(&mycpu);
      hardware_printinfo(&mycpu);
   }else            
   if (strcmp(u,"exit") == 0){         //-- Exits a console session.
      fg_exit();
      exit(0);              
   }else  
   if (strcmp(u,"echo") == 0){         //-- Displays a string. Args: <string>  
      u=strtok(0,"\n");
      if (u!=0)              
         printf("%s\n",u);
   }else  
   if (strcmp(u,"use") == 0){          //-- Tells the extension manager to use the extension: Args: <extension>  
      u=strtok(0," ");
      if (extension_override(devmgr_getdevicebyname(u),0) == -1){
         printf("Unable to install extension %s.\n",u);                
      };            
   }else        
   if (strcmp(u,"off") == 0){          //-- Power off the machine.
      dex32apm_off();
   }else
   if (strcmp(u,"files") == 0){        //-- Shows list of currently open files.
      file_showopenfiles();
   }else
   if (strcmp(u,"find") == 0){         //-- Finds a file.
      u=strtok(0," ");
      if (u != 0)
         findfile(u);
   }else
   if (strcmp(u,"kill") == 0){         //-- Kills a kernel thread. Args: <thread name>
      u=strtok(0," ");
      if (u!=0)
         dex32_killkthread_name(u);
   }else
   if (strcmp(u,"procs") == 0 || strcmp(u,"ps") == 0){  //-- List the running processes. "ps" can also be used.
      show_process();
   }else
   if (strcmp(u,"cls") == 0){          //-- Clears the screen. 
      clrscr();
   }else
   if (strcmp(u,"help") == 0){         //-- Displays this help screen.
      console_execute("type /icsos/icsos.hlp");
   }else
   if (strcmp(u,"umount") == 0){       //-- Unmounts a mounted device. Args: <mount point>
      char *u =strtok(0," ");
      if (u!=0){
         if (vfs_unmount_device(u)==-1)
            printf("umount failed.\n");
         else
            printf("%s umounted.\n",u);
      }else{
         printf("Missing parameter.\n");
      }                    
   }else
   if (strcmp(u,"mount") == 0){        //-- Mounts a device. Args: fat/cdfs <partition/device> <mount point> 
      char *fsname,*devname,*location;
      fsname=strtok(0," ");
      devname=strtok(0," ");
      location=strtok(0," ");
               
      if (vfs_mount_device(fsname, devname, location) == -1)
         printf("mount not successful.\n");
      else
         printf("mount successful.\n");  
         //fat12_mount_root(root,floppy_deviceid);
   }else
   if (strcmp(u,"path") == 0){         //-- Shows the current working directory.
      char temp[255];
      printf("%s\n",showpath(temp));
   }else
   if (strcmp(u,"lsmod") == 0){        //-- Shows the list of loaded libraries and modules. 
      showlibinfo();
   }else
   if (strcmp(u,"mem") == 0){          //-- Shows memory information.
      meminfo();
   }else
   if (strcmp(u,"mkdir") == 0){        //-- Creates a directory. Args: <directory name> 
      u=strtok(0," ");
      if (u!=0){
         if (mkdir(u) == -1)
            printf("mkdir failed.\n");
      }
   }else       
   if (strcmp(u,"run") == 0){          //-- Executes a batch file or script. Args: <script>
      u=strtok(0," ");
      if (u!=0){
         if (script_load(u) == -1){
            printf("console: Error loading script file.\n");
         };            
      }
   }else    
   if (strcmp(u,"ls") == 0||strcmp(u,"dir") == 0){ //-- Shows directory listing. Args: [-l | -osize | -oname] 
      int style=0, ordering = 0;
      char v[20];
   
      u=strtok(0," ");
      if (u != 0){
         do {
            strcpy(v,u);
            if (strcmp(v,"-l") == 0) 
               style=1;
            if (strcmp(v,"-oname") == 0) 
               ordering  = 0;
            if (strcmp(v,"-osize") == 0) 
               ordering  = 1;
            u=strtok(0," ");
         } while (u!=0);
      };
      console_ls(style, ordering);
   }else
   if (strcmp(u,"del") == 0){             //-- Deletes a files or directory. Args: <filename/dirname>
      int res;
      u=strtok(0," ");
      if (u!=0){
         char *u3=strtok(0," ");
         if (u3==0){
            delfile(u);
            printf("File deleted.\n");
         }else{
            printf("Invalid parameter.\n");
         }
      }else{
         printf("Missing parameter.\n");
      }
   }else
   if (strcmp(u,"ren") == 0){            //-- Renames a file. Args: <oldname> <newname>
      char *u2,*u3;
      u2=strtok(0," ");
      u3=strtok(0," ");               
      if (u2!=0 && u3!=0){
         if (rename(u2, u3)) 
            printf("File renamed.\n");
         else
            printf("Error renaming file.\n");
      }else{
         printf("Missing parameter.\n"); 
      }   
   }else
   if (strcmp(u,"type") == 0 || strcmp(u,"cat") == 0 ){ //-- Displays the contents of a file. Args: <filename> [-p]
      u=strtok(0," ");
      if (u!=0){
         if (console_showfile(u,0)==-1)
            printf("error opening file.\n");
      }else{
         printf("missing parameter.\n");
      }
   }else
   if (strcmp(u,"copy") == 0 || strcmp(u,"cp") == 0){ //-- Copy source to destination: Args: <source> <destination>
      u=strtok(0," ");
      if (u!=0){
         char *u2 = strtok(0," ");
         if (u2!=0){
            if (fcopy(u,u2) == -1){
               printf("Copy failed. Error while copying.\n");
               printf("Destination directory might not be present.\n");
            };
         };  
      };
   }else              
   if (strcmp(u,"cd") == 0){     //-- Changes working directory. Args: <directory>
      u=strtok(0," ");
      if (u!=0){
         if (!changedirectory(u))
            printf("cd: Cannot find directory\n");
      }else{
         changedirectory(0); //go to working directory
      } 
   }else
   if (strcmp(u,"loadmod") == 0){   //-- Loads a shared library (.dll or .so). Args: <module filename> 
      u=strtok(0," ");
      if (u!=0){
         if (loadDLL(u,str) == -1)
            printf("Unable to load %s.\n",u);
         else
            printf("Load module Successful.\n");  
      }else{
            printf("missing parameter.\n");
      }
   }else
   if (strcmp(u,"lsdev") == 0){  //-- Lists all modules currently installed and available. 
      devmgr_showdevices();
   }else
   if (strcmp(u,"lsext") == 0){  //-- Lists all extensions.
      extension_list();
   }else
   if (strcmp(u,"libinfo") == 0){ //-- Shows library information.
      u=strtok(0," ");
      module_listfxn(u);
   }else
   if (strcmp(u,"time") == 0){   //-- Displays date and time.
      printf("%d/%d/%d %d:%d.%d (%d total seconds since 1970)\n",time_systime.day,
               time_systime.month, time_systime.year,
               time_systime.hour, time_systime.min,
               time_systime.sec,time());
   }else
   if (strcmp(u,"set") == 0){    //-- Sets an environment variable. Args: <key>=<value>
      u=strtok(0," ");
      if (u==0){
         env_showenv();
      }else{
         char *name  = strtok(u,"=");
         char *value = strtok(0,"\n");
         env_setenv(name, value, 1);
      }; 
   }else         
   if (strcmp(u,"unload") == 0){ //-- Unloads a library. Args: <library name>
      u=strtok(0," ");
      if (u!=0){
         if (module_unload_library(u) == -1)
            printf("Error unloading library");
   	};
   }else
   if (strcmp(u,"demo_graphics") == 0){   //-- Runs the graphics demonstration.
      demo_graphics();
   }else
   if (u[0] == '$'){                      //-- Sends message to a device.
      int i, devid;
      char devicename[255],*cmd;

      for (i=0;i<20 && u[i+1];i++){
         devicename[i] = u[i+1];
      };
      devicename[i] = 0;
      printf("Sending command to %s\n",devicename);
      devid = devmgr_finddevice(devicename);
               
      if (devid != -1){
         if (devmgr_sendmessage(devid,DEVMGR_MESSAGESTR,str)==-1)
            printf("console: send_message() failed or not supported.\n");
      }else{
         printf("console: cannot find device.\n");
      }   
   }else
      if (strcmp(u,"cal") == 0){ // cal 2018 , displays the calendar of a specific year
            u=strtok(0," ");
            int  i = atoi (u);
            int year = i;
            int daycode = determinedaycode(year);
            determineleapyear(year);
            calendar(year, daycode);
            printf("\n");
   }else
   if (strcmp(u,"gcalccmd") == 0){   //-- gcalccmd command
      if (u!=0){  //if u is not empty
            char temp[1000]; //initialize char array temp//string
            int sqrtchecker = 0; //to check if sqrt was used or not
            temp[0] = '\0'; //initialize temp to empty 
            while(u!=NULL){   //if u is not equal to null
                  u=strtok(0," "); //gets the next value in u
                  if(strcmp(u,"sqrt") == 0){ //if it's equal to sqrt
                     u=strtok(0," ");  //gets the next value in u
                     if (u!=NULL){  //if not null,
                        float val = atoi(u);
                        int result = squareRoot(val);
                        printf("%d\n", result); //compute for sqrt
                        u[0] = '\0';
                        sqrtchecker = 1; //set sqrtchecker to 1
                     } 
                     break;
                  } else {
                     strcat(temp, u); //hoi pls potato , hoiplspotato
                     sqrtchecker = 0;
                  }
            }

            int length = strlen(temp);
            temp[length-2] = ' ';  
            temp[length-1] = '\0';  
            //printf("This iz temp: %s \n", temp);
            
            /*char temporary;
            char number[1000];
            number[0] = '\0';
            char tokenList[1000][1000];
            int count = 0;
            int countNum = 0;
            for(int j = 0; j < strlen(temp); j++){
                  temporary= temp[j]; 
                  if((temporary == '*') || (temporary == '/') || ( temporary == '+') || (temporary == '-') || (temporary == '(') || (temporary == ')') || (temporary == ' ')){
                        countNum = 0;
                        if(number[0] == '\0'){
                           tokenList[count][0]=temporary; 
                           count++;

                        }else{
                           int len = strlen(number);
                           for(int n = 0; n < len; n++){
                                 tokenList[count][n] = number[n]; 
                           }
                           number[0] = '\0'; // >:(
                           count++;
                           tokenList[count][0]=temporary; 
                           count++;
                           
                        }
                  }
                  else{
                        number[countNum] = temporary;
                        number[countNum + 1] = '\0';
                        countNum++;
                  }
            }
            count--;
            
            for(int n = 0; n < count; n++){
                  printf("TEMP %d: %s \n",n, tokenList[n]);
            }
         tokenList[0][0] = '\0';*/

              /*char postfix[20];*/
              
              int i,x,y,z;
              int result=0;
              int tempo;
              
              /*printf("Enter expression in POSTFIX FORM: ");
              scanf("%s", postfix);*/
              
              for(i=0;i<strlen(temp);i++){
                tempo = temp[i];
               
               /*
                  Insert your code here to evaluate the postfix expression.
               */

                if(tempo>=48 && tempo<=57){   //Operand
                  tempo = tempo - 48;
                  push(tempo);
                }else if(tempo == 42){   //Multiplication (* = 42)
                  x = pop();
                  y = pop();
                  z = y * x;
                  push(z);
                }else if(tempo == 43){   //Addition (+ = 43)
                  x = pop();
                  y = pop();
                  z = y + x;
                  push(z);
                }else if(tempo == 45){   //Subtraction (- = 45)
                  x = pop();
                  y = pop();
                  z = y - x;
                  push(z);
                }else if(tempo == 47){   //Division (/ = 47)
                  x = pop();
                  y = pop();
                  z = y / x;
                  push(z);
                }

              }

              result = pop();

              printf("evaluation of %s: %d\n", temp, result);
               
      }

   }else  
   if (strcmp(u,"date") == 0){   //date functions
      u=strtok(0," ");
      if (u!=0){
         	if (strcmp(u,"-d") == 0){ //-d flag
                  u=strtok(0," ");
                  if (u!=0){
                        if (strcmp(u,"now") == 0){ //date -d now
                              givenDate(0, 0, 0);
                        }
                        else if (strcmp(u,"yesterday") == 0){ //date -d yesterday
                              givenDate(-1, 0, 0);
                        }
                        else if (strcmp(u,"tomorrow") == 0){ //date -d tomorrow
                              givenDate(1, 0, 0);
                        }
                        else if (strcmp(u,"today") == 0){ //date -d today
                              givenDate(0, 0, 0);
                        }
                        else if (strcmp(u,"sunday") == 0){ //date -d sunday
                              findSunday(1);
                        }
                        else if (strcmp(u,"last-sunday") == 0){ // date -d last-sunday
                              findSunday(-1);
                        }
                        else if (strcmp(u,"last-week") == 0){ //date -d last-week
                              givenDate(-7, 0, 0);
                        }
                        else if (strcmp(u,"next-week") == 0){ //date -d next-week
                              givenDate(+7, 0, 0);
                        }
                        else if (strcmp(u,"last-month") == 0){ //date -d last-month
                              givenDate(0, -1, 0);
                        }
                        else if (strcmp(u,"last-year") == 0){ // date -d last-year
                              givenDate(0, 0, -1);
                        }
                        else if (strcmp(u,"next-year") == 0){
                              givenDate(0, 0, 1);
                        }
                        else if (strcmp(u[0],'"') == 0){ // date -d date -d "yyyy-mm-dd" +"%A" prints day of the week of the date
                              if(checkDateFormat(u) == 1){ //checks if fits format
                                    formatToDay(u);
                              }
                              else{
                                    printf("Invalid date function\n");
                              }
                        }
                        else{ // date -d 02/02/2018
                              if(checkFormat(u) == 1){
                                    returnDate(u);
                              }
                              else{
                                    printf("Invalid date function\n");
                              }
                        }
                  }else{
                        printf("Invalid date function");
                  }
            }
            else if (strcmp(u,"+") == 0){// date + string 
                  if(u!=0){
                        char temp[1000];

                        temp[0] = '\0';

                        while(u!=NULL){ //loops whole sting to find the format
                              u=strtok(0," "); 
                              strcat(temp, " ");
                              if(strcmp(u,"%a") == 0){ //abbrv weekday
                                    currentDate(1, temp);
                              }
                              else if(strcmp(u,"%B") == 0){ //month name
                                    currentDate(2, temp);
                              }
                              else if(strcmp(u,"%b") == 0){ //abbrv month name
                                    currentDate(3, temp);
                              }
                              else if(strcmp(u,"%y") == 0){ //year
                                    currentDate(4, temp);
                              }
                              else if(strcmp(u,"%V") == 0){ //week number
                                    currentDate(5, temp);
                              }
                              else if(strcmp(u,"$A") == 0){ //full weekday name
                                    currentDate(6, temp);
                              }
                              else{
                                    strcat(temp, u);
                              }
                        }
                        int length = strlen(temp);
                        temp[length-2] = ' ';  
                        temp[length-1] = '\0';  

                        printf("%s\n", temp);
                  }
            }
      }
      else{
            givenDate(0, 0, 0);
      }
   }
   else{         //treat the command as an executable
      if (u!=0){
         if (!user_execp(u, 0, str))
            printf("Undefined console command.\n");
      }
   }
   //normal termination
   return 1;
};

int checkDateFormat(char temp[]){// check if fits yyyy-mm-dd 
      int checker = 0;
      char month[3];
      char day[3];
      char year[5];
      if(temp[5] == '-' && temp[8] == '-'){

            year[0] = temp[1];
            year[1] = temp[2];
            year[2] = temp[3];
            year[3] = temp[4];
            year[4] = '\0';
            month[0] = temp[6];
            month[1] = temp[7];
            month[2] = '\0';
            day[0] = temp[9];
            day[1] = temp[10];
            day[2] = '\0';

            int monthNum = atoi(month);
            int dayNum = atoi(day);
            int yearNum = atoi(year);

            if(monthNum < 13){
                  if(dayNum < daysInMonth[monthNum] ){
                        checker = 1;
                  }
            }
      }
      return checker;
}

void formatToDay(char temp[]){//prints the weekday 
      char month[3];
      char day[3];
      char year[5];

      year[0] = temp[1];
      year[1] = temp[2];
      year[2] = temp[3];
      year[3] = temp[4];
      year[4] = '\0';
      month[0] = temp[6];
      month[1] = temp[7];
      month[2] = '\0';
      day[0] = temp[9];
      day[1] = temp[10];
      day[2] = '\0';

      int weekday, tempDay;
      int monthNum = atoi(month);
      int dayNum = atoi(day);
      int yearNum = atoi(year);
            
      weekday  = (tempDay += monthNum < 3 ? yearNum-- : yearNum - 2, 23*monthNum/9 + dayNum + 4 + yearNum/4- yearNum/100 + yearNum/400)%7;
      printf("%s\n", fullDays[weekday]);
}

void returnDate(char temp[]){ //prints out the date 
      char month[3];
      char day[3];
      char year[5];

      month[0] = temp[0];
      month[1] = temp[1];
      month[2] = '\0';
      day[0] = temp[3];
      day[1] = temp[4];
      day[2] = '\0';
      year[0] = temp[6];
      year[1] = temp[7];
      year[2] = temp[8];
      year[3] = temp[9];
      year[4] = '\0';

      int weekday, tempDay;
      int monthNum = atoi(month);
      int dayNum = atoi(day);
      int yearNum = atoi(year);
            
      weekday  = (tempDay += monthNum < 3 ? yearNum-- : yearNum - 2, 23*monthNum/9 + dayNum + 4 + yearNum/4- yearNum/100 + yearNum/400)%7;
      printf("%s %s %d %d \n", days[weekday], noNewLineMonths[monthNum], dayNum, yearNum );
}

int checkFormat(char temp[]){// checks if mm/dd/yyyy 
      int checker = 0;
      char month[3];
      char day[3];
      if(temp[2] == '/' && temp[5] == '/'){
            month[0] = temp[0];
            month[1] = temp[1];
            month[2] = '\0';
            day[0] = temp[3];
            day[1] = temp[4];
            day[2] = '\0';

            int monthNum = atoi(month);
            int dayNum = atoi(day);
            
            if(monthNum < 13){
                  if(dayNum < daysInMonth[monthNum] ){
                        checker = 1;
                  }
            }
      }
      return checker;
}

void currentDate(int formatOption, char temp[]){ //formats the string with appropriate format tags
      int monthNum, day,year, weekday, tempDay;
      monthNum = time_systime.month;
      day = time_systime.day;
      year = time_systime.year;
      weekday  = (tempDay += monthNum < 3 ? year-- : year - 2, 23*monthNum/9 + day + 4 + year/4- year/100 + year/400)%7;

      if(formatOption == 1){ // abbrv name
            strcat(temp, days[weekday]);
      }
      else if(formatOption == 2){ //month name
            strcat(temp, noNewLineMonths[monthNum]);
      }
      else if(formatOption == 3){ //abbrv month
            strcat(temp, abbrvMonths[monthNum]);
      }
      else if(formatOption == 4){ //year
            char yearString[10];
            itoa(year, yearString, 10);
            strcat(temp, yearString);
      }
      else if(formatOption == 5){ //week number
            getWeekNumber(monthNum, day, year, temp);            
      }
      else if(formatOption == 6){
            strcat(temp, fullDays[weekday]);
      }
}


void getWeekNumber(int monthNum, int day, int weekday, char temp[]){
      int dayNumber = 0;
      for(int count = 0; count < monthNum; count++){ //adds all end of the month
            dayNumber += daysInMonth[count];
      }
      dayNumber += day;
      int weekNumber = dayNumber / 7; //divide by 7 to get week number
      char weekString[10];
      itoa(weekNumber, weekString, 10);
      strcat(temp, weekString);
}

void givenDate(int addDay, int addMonth, int addYear){ //adjust date by how much days/months/years back/forward
      int monthNum, day, hour, minutes, year, tempDay, weekday;
      monthNum = time_systime.month + addMonth;
      day = time_systime.day + addDay;
      hour = time_systime.hour;
      hour = hour + 8;
      minutes = time_systime.min;
      year = time_systime.year + addYear;
      weekday  = (tempDay += monthNum < 3 ? year-- : year - 2, 23*monthNum/9 + day + 4 + year/4- year/100 + year/400)%7;

      if(day < 0){
            monthNum --;
            day = daysInMonth[monthNum] + day;
      }

      printf("%s %s %d %d:%d:%d %d \n", days[weekday], noNewLineMonths[monthNum], day, hour, minutes, time_systime.sec, year );
}

void findSunday(int sunday){ //finds the sunday given a positive (sunday) or negative (last-sunday)
      int monthNum, day, hour, minutes, year, tempDay, weekday, tempWeekday;
      
      monthNum = time_systime.month;
      day = time_systime.day;
      hour = time_systime.hour;
      hour = hour + 8;
      minutes = time_systime.min;
      year = time_systime.year;
      weekday  = (tempDay += monthNum < 3 ? year-- : year - 2, 23*monthNum/9 + day + 4 + year/4- year/100 + year/400)%7;

      if(sunday > 0){
            tempWeekday = 7 - weekday;
            day = day + tempWeekday;
            if(day > daysInMonth[monthNum]){
                  monthNum++;
                  day = day - daysInMonth[monthNum];
            }
      }
      else{
            if(weekday == 0 ){
                  day = day - 7;
            }
            else{
                  day = day - weekday;
            }
            if(day < 1){
                  monthNum --;
                  day = daysInMonth[monthNum] + day;
            }
      }

      printf("%s %s %d %d:%d:%d %d \n", noNewLineMonths[monthNum], days[0], day, hour, minutes, time_systime.sec, year );
}

int determinedaycode(int year){ //determines position of first day of january
	int daycode;
	int d1, d2, d3;

	d1 = (year - 1)/ 4.0;
	d2 = (year - 1)/ 100;
	d3 = (year - 1)/ 400;
	daycode = (year + d1 - d2 + d3) %7;
	return daycode;
}

int determineleapyear(int year){//checks if leap year
	if(year% 4 == 0 && year%100 != 0 || year%400 == 0){
		daysInMonth[2] = 29;
		return 1;
	}
	else{
		daysInMonth[2] = 28;
		return 0;
	}
}

void calendar(int year, int daycode){
	int month, day;
	for ( month = 1; month <= 12; month++ ){
		printf("%s", months[month]);
		printf("\n\nSun  Mon  Tue  Wed  Thu  Fri  Sat\n" );

		// prints out space for the correct position of first day
		for ( day = 1; day <= 1 + daycode * 5; day++ ){
			printf(" ");
		}

		// Print all the dates for one month
		for ( day = 1; day <= daysInMonth[month]; day++ ){
			printf("%2d", day );
			
			// if sunday, start new line
			if ( ( day + daycode ) % 7 > 0 )
				printf("   " );
			else
				printf("\n " );
		}
			// get new daycode for the first day
			daycode = ( daycode + daysInMonth[month] ) % 7;
	}
}

int console_new(){
   //create a new console         
   char consolename[255];
   sprintf(consolename,"dex32_console(%d)", console_first);    
   return createkthread((void*)console, consolename, 200000);
};

void console_main(){
   DEX32_DDL_INFO *myddl=0;
   fg_processinfo *myfg;
   char s[256]="";
   char temp[256]="";
   char last[256]="";
   char console_fmt[256]="%cdir% %% ";
   char console_prompt[256]="cmd >";
    
   DWORD ptr;
    
   myddl =Dex32CreateDDL();    

    
   Dex32SetProcessDDL(myddl, getprocessid());
   myfg = fg_register(myddl, getprocessid());
   fg_setforeground( myfg->id );

   clrscr();
   strcpy(last,"");
    
   if (console_first == 0) 
      script_load("/icsos/autoexec.bat");
    
   console_first++;  
   do{
      textcolor(WHITE);
      textbackground(BLACK);
      prompt_parser(console_fmt,console_prompt);
    
      textcolor(LIGHTBLUE);
      printf("%s",console_prompt);
      textcolor(WHITE);
    
      if (strcmp(s,"@@")!=0 && strcmp(s,"!!")!=0)
         strcpy(last,s);
    
      getstring(s, myddl);
   
      if (strcmp(s,"!")==0){
         sendtokeyb(last,&_q);
      }
      else if (strcmp(s,"!!")==0){
         sendtokeyb(last,&_q);
         sendtokeyb("\r",&_q);
      }
      else   
         console_execute(s);
   } while (1);
};

