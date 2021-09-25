#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <mem.h>

#define SB_RESET 0x6
#define SB_READ_DATA 0xA
#define SB_READ_DATA_STATUS 0xE
#define SB_WRITE_DATA 0xC
#define SB_VERSION 0xE1
#define SB_ENABLE_SPEAKER 0xD1
#define SB_DISABLE_SPEAKER 0xD3
#define SB_PAUSE_PLAYBACK 0xD5

#define DMA_SIZE 8192
#define DMA_PAGE_5 0x8B
#define DMA_PAGE_6 0x89
#define DMA_PAGE_7 0x8A
#define DMA_ADDR_5 0xC4
#define DMA_ADDR_6 0xC8
#define DMA_ADDR_7 0xCC
#define DMA_COUNT_5 0xC6
#define DMA_COUNT_6 0xCA
#define DMA_COUNT_7 0xCE
#define DMA_MASK_REGISTER 0xD4
#define DMA_MODE_REGISTER 0xD6
#define DMA_FLIP_FLOP_REGISTER 0xD8

#define MONO 1
#define STEREO 2

struct SB
{
  short detected;
  short base;
  int irq;
  int dma;
} sb;

struct DMA
{
  unsigned char *buffer;
  long address;
} dma;

struct RESTORE
{
  void interrupt (*irq)(void);
  int pic;
} restore;

struct RAW
{
  FILE *file;
  unsigned short channel;
  unsigned long frequency;
  unsigned long size;
} raw;

void DSPInit()
{
  int DMA_PAGE, DMA_ADDR, DMA_COUNT;
  int page, offset, temp;
  int dmaBit = sb.dma;

  switch (dmaBit)
  {
  case 5:
    DMA_PAGE = DMA_PAGE_5;
    DMA_ADDR = DMA_ADDR_5;
    DMA_COUNT = DMA_COUNT_5;
    break;
  case 6:
    DMA_PAGE = DMA_PAGE_6;
    DMA_ADDR = DMA_ADDR_6;
    DMA_COUNT = DMA_COUNT_6;
    break;
  case 7:
    DMA_PAGE = DMA_PAGE_7;
    DMA_ADDR = DMA_ADDR_7;
    DMA_COUNT = DMA_COUNT_7;
    break;
  }

  dmaBit -= 4;

  page = dma.address >> 16;
  offset = dma.address & 0xFFFF;
  temp = page & 0x0001;
  temp <<= 15;
  offset >>= 1;
  offset &= 0x7FFF;
  offset |= temp;

  outportb(DMA_MASK_REGISTER, dmaBit | 4);
  outportb(DMA_FLIP_FLOP_REGISTER, 0);
  outportb(DMA_MODE_REGISTER, dmaBit | 0x58);
  outportb(DMA_COUNT, (DMA_SIZE / 2 - 1) & 0xFF);
  outportb(DMA_COUNT, (DMA_SIZE / 2 - 1) >> 8);
  outportb(DMA_PAGE, page);
  outportb(DMA_ADDR, offset & 0xFF);
  outportb(DMA_ADDR, offset >> 8);
  outportb(DMA_MASK_REGISTER, dmaBit);
}

int DSPReset(short port)
{
  int version;
  outportb(port + SB_RESET, 1);
  delay(10);
  outportb(port + SB_RESET, 0);
  delay(10);
  if (((inportb(port + SB_READ_DATA_STATUS) & 0x80) == 0x80) && (inportb(port + SB_READ_DATA) == 0x0AA))
  {
    outportb(port + SB_WRITE_DATA, SB_VERSION);
    while ((inportb(port + SB_READ_DATA_STATUS) & 0x80) == 0)
      ;
    version = inportb(port + SB_READ_DATA);
    inportb(port + SB_READ_DATA);
    if (version < 4)
      return 1;
    sb.base = port;
    return 1;
  }
  return 0;
}

void DSPWrite(unsigned char command)
{
  while ((inportb(sb.base + SB_WRITE_DATA) & 0x80) == 0x80)
    ;
  outportb(sb.base + SB_WRITE_DATA, command);
}

void bufferInit()
{
  unsigned char *temp;
  short page1, page2;

  temp = (unsigned char *)malloc(DMA_SIZE);
  dma.address = FP_SEG(temp);
  dma.address = (dma.address << 4) + FP_OFF(temp);
  page1 = dma.address >> 16;
  page2 = (dma.address + DMA_SIZE - 1) >> 16;
  if (page1 != page2)
  {
    dma.buffer = (unsigned char *)malloc(DMA_SIZE);
    dma.address = FP_SEG(dma.buffer);
    dma.address = (dma.address << 4) + FP_OFF(dma.buffer);
    free(temp);
  }
  else
    dma.buffer = temp;
}

void bufferPlay()
{
  unsigned int size;

  if (raw.size < DMA_SIZE)
  {
    fread(dma.buffer, raw.size, 1, raw.file);
    size = raw.size;
    raw.size = 0;
  }
  else
  {
    fread(dma.buffer, DMA_SIZE, 1, raw.file);
    size = DMA_SIZE;
    raw.size -= DMA_SIZE;
  }

  if (size <= 1)
    size = 2;

  DSPWrite(0x41);
  DSPWrite((raw.frequency & 0xFF00) >> 8);
  DSPWrite(raw.frequency & 0xFF);
  DSPWrite(0xB0);
  DSPWrite((raw.channel == MONO) ? 0x10 : 0x30);
  DSPWrite((size / 2 - 1) & 0xFF);
  DSPWrite((size / 2 - 1) >> 8);
}

void interrupt IRQHandle(void)
{
  int status;
  outportb(sb.base + 4, 0x82);
  status = inportb(sb.base + 5);
  if (status & 2)
    inportb(sb.base + 0xF);
  outportb(0x20, 0x20);
  if (raw.size > 0)
    bufferPlay();
}

void IRQInit()
{
  restore.irq = getvect(sb.irq + 8);
  setvect(sb.irq + 8, IRQHandle);
  restore.pic = inportb(0x21);
  outportb(0x21, (restore.pic & !(1 << sb.irq)));
}

void IRQDeinit()
{
  outportb(0x21, restore.pic);
  setvect(sb.irq + 8, restore.irq);
}

void soundDetect()
{
  char temp;
  char *BLASTER;
  sb.detected = 0;
  sb.base = 0;

  for (temp = 1; temp < 9; temp++)
    if (temp != 7 && DSPReset(0x200 + (temp << 4)))
      break;
  if (temp == 9)
    return;

  BLASTER = getenv("BLASTER");
  sb.dma = 0;
  for (temp = 0; temp < strlen(BLASTER); temp++)
    if ((BLASTER[temp] | 32) == 'h')
      sb.dma = BLASTER[temp + 1] - '0';

  for (temp = 0; temp < strlen(BLASTER); temp++)
    if ((BLASTER[temp] | 32) == 'i')
    {
      sb.irq = BLASTER[temp + 1] - '0';
      if (BLASTER[temp + 2] != ' ')
        sb.irq = sb.irq * 10 + BLASTER[temp + 2] - '0';
    }

  sb.detected = sb.base != 0;
}

void soundInit()
{
  memset(&sb, 0, sizeof(sb));
  memset(&dma, 0, sizeof(dma));
  memset(&restore, 0, sizeof(restore));
  soundDetect();
  if (sb.detected)
  {
    IRQInit();
    DSPWrite(SB_ENABLE_SPEAKER);
  }
}

void soundDeinit()
{
  if (sb.detected)
  {
    if (raw.file)
    {
      while (raw.size > 0)
        ;
      fclose(raw.file);
    }
    DSPWrite(SB_DISABLE_SPEAKER);
    free(dma.buffer);
    IRQDeinit();
  }
}

void soundStop()
{
  if (raw.file)
  {
    DSPWrite(SB_PAUSE_PLAYBACK);
    fclose(raw.file);
  }
  memset(&raw, 0, sizeof(raw));
  free(dma.buffer);
  bufferInit();
  DSPInit();
}

void soundPlay(const char *fileName, short numberOfChannels, long frequency, long size, long startAt)
{
  if (sb.detected)
  {
    soundStop();
    raw.file = fopen(fileName, "rb");
    if (!raw.file)
      return;

    raw.channel = numberOfChannels;
    raw.frequency = frequency;
    raw.size = size;
    fseek(raw.file, startAt, SEEK_SET);

    bufferPlay();
  }
}