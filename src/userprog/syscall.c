#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

static void syscall_handler (struct intr_frame *);

/* System Calls */
static void sys_halt (void);
static void sys_exit (int status);
static pid_t sys_exec (const char *cmd_line);
static int sys_wait (pid_t pid);
static bool sys_create (const char *file, unsigned initial_size);
static bool sys_remove (const char *file);
static int sys_open (const char *file);
static int sys_filesize (int fd);
static int sys_read (int fd, void *buffer, unsigned size);
static int sys_write (int fd, const void *buffer, unsigned size);
static void sys_seek (int fd, unsigned position);
static unsigned sys_tell (int fd);
static void sys_close (int fd);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  // TODO
  printf ("system call!\n");
  thread_exit ();
}

/* sys_halt() - Terminates Pintos by calling
 * shutdown_power_off().
 */
static void
sys_halt (void)
{
  shutdown_power_off ();
}

/* sys_exit() - Terminates the current user program,
 * returning status to the kernel.
 */
static void
sys_exit (int status)
{
  thread_exit ();
}

/* sys_exec() - Runs the executable whoe name is
 * given in cmd_line, passing any given
 * arguments, and returns the new process's
 * program id (pid).
 */
static pid_t
sys_exec (const char *cmd_line)
{
  // TODO
  return 0;
}

/* sys_wait() - Waits for a child process pid and
 * retrieves the child's exit status.
 */
static int
sys_wait (pid_t pid)
{
  // TODO
  return 0;
}

/* sys_create() - Creates a new file called file
 * initially initial_size bytes in size. 
 * Returns true if successful, false otherwise.
 */
static int
sys_create (const char *file, unsigned initial_size)
{
  // TODO
  return 0;
}

/* sys_remove() - Deletes the file called file.
 * Returns true if successful, false otherwise.
 */
static bool
sys_remove (const char *file)
{
  // TODO
  return 0;
}

/* sys_open() - Opens the file called file. Returns
 * a nonnegative integer handle called a
 * "file descriptor" (fd), or -1 if the file
 * could not be opened.
 */
static int
sys_open (const char *file)
{
  // TODO
  return 0;
}

/* sys_filesize() - Returns the size, in bytes,
 * of the file open as fd.
 */
static int
sys_filesize (int fd)
{
  // TODO
  return 0;
}

/* sys_read() - Reads size bytes from the file
 * open as fd into buffer. Returns the number
 * of bytes actually read (0) at end of file),
 * or -1 if the file could not be read.
 */
static int
sys_read (int fd)
{
  // TODO
  return 0;
}

/* sys_write() - Writes size bytes from buffer
 * to the open file fd. Returns the number
 * of bytes actually written, which may be
 * less than size if some bytes could not
 * be written.
 */
static int
sys_write (int fd, const void *buffer, unsigned size)
{
  // TODO
  return 0;
}

/* sys_seek() - Changes the next byte to be
 * read or written in open file fd to
 * position, expressed in bytes from the
 * beginning of the file.
 */
static void
sys_seek (int fd, unsigned position)
{
  // TODO
  return 0;
}

/* sys_tell() - Returns the position of the
 * next byte to be read or written in open
 * file fd, expressed in bytes from the
 * beginning of the file.
 */
static unsigned
sys_tell (int fd)
{
  // TODO
  return 0;
}

/* sys_close() - Closes file descriptor fd.
 */
static void
sys_close (int fd)
{
  // TODO
  return 0;
}
