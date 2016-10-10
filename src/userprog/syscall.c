#include "userprog/process.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "filesys/file.h"
#include "filesys/filesys.h"

typedef int pid_t;
static int (*syscall_handlers[20]) (struct intr_frame *); /* Array of syscall functions */

static void syscall_handler (struct intr_frame *);


/* Reads a byte at user virtual address UADDR.
   UADDR must be below PHYS_BASE.
   Returns the byte value if successful, -1 if a segfault
   occurred. */
static int
get_user (const uint8_t *uaddr)
{
  if(!is_user_vaddr(uaddr))
	 return -1;
  int result;
  asm ("movl $1f, %0; movzbl %1, %0; 1:"
  : "=&a" (result) : "m" (*uaddr));
  return result;
}

/* Writes BYTE to user address UDST.
   UDST must be below PHYS_BASE.
   Returns true if successful, false if a segfault occurred. */
static bool
put_user (uint8_t *udst, uint8_t byte)
{
  if(!is_user_vaddr(udst))
    return false;
  int error_code;
  asm ("movl $1f, %0; movb %b2, %1; 1:"
  : "=&a" (error_code), "=m" (*udst) : "q" (byte));
  return error_code != -1;
}

static bool 
is_valid_pointer(void * esp, uint8_t argc)
{
  uint8_t i = 0;
  for (; i < argc; ++i)
  {
    if (get_user(((uint8_t *)esp)+i) == -1)
    {
	  return false;
    }
  }
    return true;
}

static void
kill_program(void)
{
  thread_exit(-1);
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  // Must check if it's a valid pointer
  if (!is_valid_pointer(f->esp, 4))
  {
    kill_program();
    return;
  }
  
  int syscall_num = * (int *)f->esp;

  // Make sure it's within the array size
  if (syscall_num < 0 || syscall_num >= 20)
  {
    kill_program();
	return;
  }
  
  // Usually when -1 is return, the operation failed
  // Therefore, we exit accordingly
  if(syscall_handlers[syscall_num](f) == -1)
  {
    kill_program();
	return;
  }
}

/* Project 2*/

/* TODO: Modify to own implementation
   Extern function for sys_exit */
static int
sys_exit (int status)
{
  thread_exit (status);
}

void 
syscall_exit (int status)
{
  sys_exit (status);
}


static int
syscall_myexit(struct intr_frame *f)
{
  int status;
  if (is_valid_pointer(f->esp + 4, 4))
    status = *((int*)f->esp+1);
  else
    return -1;
  sys_exit(status);
  return 0;
}

static int
syscall_write(struct  intr_frame *f)
{
  if (!is_valid_pointer(f->esp + 4, 12))
  {
     return -1;
  }
  int fd = *(int *)(f->esp + 4);
  void *buffer = *(char**)(f->esp + 8);
  unsigned size = *(unsigned *)(f->esp + 12);
  if (!is_valid_pointer(buffer, 1) || !is_valid_pointer(buffer + size,1))
  {
	return -1;
  }
  int written_size = process_write(fd, buffer, size);
  f->eax = written_size;
  return 0;
} 

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
  syscall_handlers[SYS_EXIT] = &syscall_myexit;
  syscall_handlers[SYS_WRITE] = &syscall_write;
}

