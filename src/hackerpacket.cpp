// HackerPacket.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include <OpenGL/gl.h>
#include <time.h>
#include "c_texture.h"
#include "building.h"
#include "camera.h"
#include "tcpdump3D.h"
#include "font.h"

/*
#define Max_Num_Adapter 10


int g_width = 800, g_height = 600;
LPADAPTER  lpAdapter = 0;
LPPACKET   lpPacket = 0;

time_t curr_time;
CCamera g_Camera;
TcpDump3D tcpdump;
bool mouse_active = true;
bool just_entered = true;

void CloseApp();
void InitGraphics();
bool InitPacketSniffer();
void ClosePacketSniffer();
void UpdatePackets();


void InitGraphics()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, 4.0/3.0, 1.0, 500.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
 
	tcpdump.InitGrid(4, 4);
	LoadFontTexture("font-8bit.tga");
}

void reshape(GLint width, GLint height)
{
  glViewport(0, 0, width, height);
 	glLoadIdentity();
	gluPerspective(60.0, 4.0/3.0, 1.0, 500.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void display(void)
{
	// Clear frame buffer and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
  g_Camera.Load();
 
  tcpdump.Render(g_Camera);
	g_Camera.Unload();

  glDisable(GL_TEXTURE_2D);

  GLenum code;
  if((code = glGetError()) != GL_NO_ERROR)
    printf("%s", gluErrorString(code));

  // Make sure changes appear onscreen
	glutSwapBuffers();
}

void MouseButton(int button, int state, int x, int y)
{
}

void MouseMotion(int x, int y)
{ 
  static old_x = 0, old_y = 0;

  if(just_entered || !mouse_active)
  {    
    old_x = x; old_y = y;

    if(just_entered)
      just_entered = false;
   
    return; 
  }
  
  if(x != old_x)
    g_Camera.RotateX(x - old_x);
  
  if(y != old_y)
    g_Camera.RotateY(y - old_y);

  old_x = x;
  old_y = y;
}

void AnimateScene(void)
{
  curr_time = time(NULL);
	UpdatePackets();
	glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:             // ESCAPE key
		CloseApp();
		exit(0);
		break;
	case 'w':
		g_Camera.Move(3.0);
		break;
  case 's':
    g_Camera.Move(-3.0);
    break;
  case 'a':
    g_Camera.Strafe(-3.0);
    break;
  case 'd':
    g_Camera.Strafe(3.0);
    break;
  case 'e':
    mouse_active ^= 1;
    break;
	}
}

void CloseApp()
{
	ClosePacketSniffer();
	//  oncenic.net
}

void UpdatePackets()
{
  
	if(PacketReceivePacket(lpAdapter,lpPacket,TRUE) && lpPacket->ulBytesReceived > 0)
	{
    unsigned int off = 0, tlen = 0, tlen1 = 0;
    struct bpf_hdr *hdr;
    
    unsigned char *buf = (unsigned char *)lpPacket->Buffer, *tmp_buf = NULL;

    while(off < lpPacket->ulBytesReceived)
    {
     	hdr=(struct bpf_hdr *)(buf+off);
    	tlen1 = hdr->bh_datalen;
	  	tlen = hdr->bh_caplen;
	  	off += hdr->bh_hdrlen;

      tmp_buf = buf+off;
    
      int ip_offset = 0;
      char t = *(tmp_buf+12);
      char s = *(tmp_buf+13);    
      if(t == 0x08 && s == 0x06) // 0x0806
      {
     //   printf("ARP Packet From: ");
        ip_offset = 28;
      } 
      else if(t == 0x08 && s == 0x00) // 0x0800
      {
   //     printf("IP Packet From: ");
        ip_offset = 26;
      }
      unsigned int sec1 = *(tmp_buf+ip_offset);
      unsigned int sec2 = *(tmp_buf+ip_offset+1);
      unsigned int sec3 = *(tmp_buf+ip_offset+2);
      unsigned int sec4 = *(tmp_buf+ip_offset+3);
      char b[16];
      sprintf(b, "%d.%d.%d.%d", sec1, sec2, sec3, sec4);
 //     printf("%s\n", b);
      if(strcmp("0.0.0.0", b) == 0)
        break;
      if(!tcpdump.HostExists(b, curr_time, (char *)tmp_buf, tlen))
        tcpdump.AddHost(b, (unsigned long)curr_time, (char *)tmp_buf, tlen);

      off=Packet_WORDALIGN(off+tlen);
      if(off > 2024)
        break;
    }
  }
}

void OnFocus(int state)
{
  if(state == GLUT_ENTERED)
    just_entered = true;
}

bool InitPacketSniffer()
{
	// Prototypes
	char        AdapterList[Max_Num_Adapter][1024];
	//define a pointer to an ADAPTER structure
	int        i = 0;
	unsigned int dwErrorCode;

	DWORD dwVersion;
	DWORD dwWindowsMajorVersion;

	//unicode strings (winnt)
	WCHAR		AdapterName[8192]; // string that contains a list of the network adapters
	WCHAR		*temp,*temp1;

	//ascii strings (win95)
	char		AdapterNamea[8192]; // string that contains a list of the network adapters
	char		*tempa,*temp1a;

	int	AdapterNum=0,Open;
	ULONG	AdapterLength;

	char buffer[256000];  // buffer to hold the data coming from the driver

	// obtain the name of the adapters installed on this machine
	AdapterLength=4096;

	printf("Library version:%s\n", PacketGetVersion());

	printf("Adapters installed:\n");

	// the data returned by PacketGetAdapterNames is different in Win95 and in WinNT.
	// We have to check the os on which we are running
	dwVersion=GetVersion();
	dwWindowsMajorVersion =  (DWORD)(LOBYTE(LOWORD(dwVersion)));
	if (!(dwVersion >= 0x80000000 && dwWindowsMajorVersion >= 4))
	{  // Windows NT
		if(PacketGetAdapterNames((PSTR)AdapterName,&AdapterLength)==FALSE){
			printf("Unable to retrieve the list of the adapters!\n");
			return false;
		}
		temp=AdapterName;
		temp1=AdapterName;
		while ((*temp!='\0')||(*(temp-1)!='\0'))
		{
			if (*temp=='\0') 
			{
        memset(AdapterList[i], '\0', 8192);
				memcpy(AdapterList[i],temp1,(temp-temp1)*2);
				temp1=temp+1;
				i++;
			}
			temp++;
		}

		AdapterNum=i;
		for (i=0;i<AdapterNum;i++)
			wprintf(L"\n%d- %s\n",i+1,AdapterList[i]);
		printf("\n");
	}

	else	//windows 95
	{
		if(PacketGetAdapterNames(AdapterNamea,&AdapterLength)==FALSE){
			printf("Unable to retrieve the list of the adapters!\n");
			return -1;
		}
		tempa=AdapterNamea;
		temp1a=AdapterNamea;

		while ((*tempa!='\0')||(*(tempa-1)!='\0'))
		{
			if (*tempa=='\0') 
			{
        memset(AdapterList[i], '\0', 8192);
				memcpy(AdapterList[i],temp1a,tempa-temp1a);
				temp1a=tempa+1;
				i++;
			}
			tempa++;
		}

		AdapterNum=i;
		for (i=0;i<AdapterNum;i++)
      printf("\n%d- %s\n",i+1,AdapterList[i]);
    
      printf("\n");
	}

	do 
	{
		printf("Select the number of the adapter to open : ");
		scanf("%d",&Open);
		if (Open>AdapterNum)
			printf("\nThe number must be smaller than %d",AdapterNum); 
	} while (Open>AdapterNum);

	lpAdapter = PacketOpenAdapter(AdapterList[Open-1]);

	if (!lpAdapter || (lpAdapter->hFile == INVALID_HANDLE_VALUE))
	{
		dwErrorCode=GetLastError();
		printf("Unable to open the adapter, Error Code : %lx\n",dwErrorCode); 
		return false;
	}	

	// set the network adapter in promiscuous mode

	if(PacketSetHwFilter(lpAdapter,NDIS_PACKET_TYPE_PROMISCUOUS)==FALSE){
		printf("Warning: unable to set promiscuous mode!\n");
	}

	// set a 512K buffer in the driver
	if(PacketSetBuff(lpAdapter,512000)==FALSE){
		printf("Unable to set the kernel buffer!\n");
		return false;
	}

	// set a 1 second read timeout
	if(PacketSetReadTimeout(lpAdapter, -1)==FALSE){
		printf("Warning: unable to set the read tiemout!\n");
	}

	//allocate and initialize a packet structure that will be used to
	//receive the packets.
	if((lpPacket = PacketAllocatePacket())==NULL){
		printf("\nError: failed to allocate the LPPACKET structure.");
		return false;
	}
	PacketInitPacket(lpPacket,(char*)buffer,256000);
	return true;
}

void ClosePacketSniffer()
{
	assert(lpPacket && lpAdapter);

	struct bpf_stat stat;
	//print the capture statistics
	if(PacketGetStats(lpAdapter,&stat)==FALSE){
		printf("Warning: unable to get stats from the kernel!\n");
	}
	else
		printf("\n\n%d packets received.\n%d Packets lost",stat.bs_recv,stat.bs_drop);

	PacketFreePacket(lpPacket);

	// close the adapter and exit

	PacketCloseAdapter(lpAdapter);
}

int _tmain(int argc, _TCHAR* argv[])
{
	glutInit (&argc, argv);
	glutInitWindowSize (g_width, g_height);
	glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("HackerzPacket v1.0");

	// Initialize OpenGL graphics state
	InitGraphics();
	if(!InitPacketSniffer())
	{
		printf("main(): PacketSniffer initialization failed!\n");
		return -1;
	}

	// Register callbacks:
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMotion);
  glutPassiveMotionFunc(MouseMotion);
  glutEntryFunc(OnFocus);
	glutIdleFunc(AnimateScene);

	// Turn the flow of control over to GLUT
	glutMainLoop ();

	return 1;
}

*/

