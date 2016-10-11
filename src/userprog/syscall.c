#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/synch.h"
#include "filesys/directory.h"
#include "filesys/filesys.h"
#include "filesys/file.h"

static void syscall_handler (struct intr_frame *);

/* System calls */
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

static struct lock filesys_lock;

/* file_entry - When a file is opened, an
 * entry is created to represent that file and
 * store the information about that opened file.
 */
struct opened_file
{
  // The file opened
  struct file *file;
  
  // The file descriptor
  int fd;

  // Embed LIST_ELEM into this structure (list.h line:103)
  struct list_elem element;
}

/* get_entry() - Returns the opened_file
 * entry with the matching file descriptor.
 */
static struct opened_file *
get_entry (int fd)
{
  struct thread *c = thread_current ();
  struct list_elem *curr_element;
  for (curr_element = list_begin (&c->opened_files_list);
	curr_element != list_end (&c->opened_files_list);
	curr_element = list_next (curr_element))
  {
    struct opened_file *entry;
    entry = list_entry (curr_element, struct opened_file, element)
    if (entry->fd == fd)
    {
      return entry;
    }
  }

  return NULL;
}

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
  // free the current process' resources
  process_exit ();
  thread_exit (status);
}

/* sys_exec() - Runs the executable whose name is
 * given in cmd_line, passing any given
 * arguments, and returns the new process's id.
 */
static tid_t
sys_exec (const char *cmd_line)
{
  tid_t tid;

  lock_acquire (&filesys_lock);
  tid = process_execute (cmd_line);
  lock_release (&filesys_lock);

  return tid;
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
  bool success;

  lock_acquire (&filesys_lock);
  success = filesys_create (file, initial_size);
  lock_release (&filesys_lock);

  return success;
}

/* sys_remove() - Deletes the file called file.
 * Returns true if successful, false otherwise.
 */
static bool
sys_remove (const char *file)
{
  bool success;

  lock_acquire (&filesys_lock);
  success = filesys_remove (file);
  lock_release (&filesys_lock);

  return success;
}

/* sys_open() - Opens the file called file. Returns
 * a nonnegative integer handle called a
 * "file descriptor" (fd), or -1 if the file
 * could not be opened.
 */
static int
sys_open (const char *file)
{
  // create a new entry
  struct opened_file *entry;
  
  // file descriptor
  int fd = -1;

  entry = malloc (sizeof *entry);
  if (entry != NULL)
  {
    lock_acquire (&filesys_lock);
    entry->file = filesys_open (file);
    if (entry->file != NULL)
    {
      struct thread *c = thread_current ();
      fd = c->available_handle;
      entry->fd = fd;
      c->available_handle++;
      list_push_front (&c->opened_files_list, &entry->element);
    }
    else
    {
      free (entry);
    }
    lock_release (&filesys_lock);
  }
  
  return fd;
}

/* sys_filesize() - Returns the size, in bytes,
 * of the file open as fd.
 */
static int
sys_filesize (int fd)
{
  struct opened_file *entry = get_entry (fd);
  if (entry != NULL)
  {
    lock_acquire (&filesys_lock);
    int size = file_length (entry->file);
    lock_release (&filesys_lock);

    return size;
  }
  else {
    return -1;
  }
}

/* sys_read() - Reads size bytes from the file
 * open as fd into buffer. Returns the number
 * of bytes actually read (0) at end of file),
 * or -1 if the file could not be read.
 */
static int
sys_read (int fd, void *buffer, unsigned size)
{
  struct opened_file *entry = get_entry (fd);
  int bytes_read;

  lock_acquire (&filesys_lock);
  bytes_read = file_read (entry->file, buffer, size);
  lock_release (&filesys_lock);

  return bytes_read;
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
  struct opened_file *entry = get_entry (fd);
  int bytes_written;

  lock_acquire (&filesys_lock);
  bytes_written = file_write (entry->file, buffer, size);
  lock_release (&filesys_lock);

  return bytes_written;
}

/* sys_seek() - Changes the next byte to be
 * read or written in open file fd to
 * position, expressed in bytes from the
 * beginning of the file.
 */
static void
sys_seek (int fd, unsigned position)
{
  struct opened_file *entry = get_entry (fd);

  lock_acquire (&filesys_lock);
  file_seek (entry->file, position);
  lock_release (&filesys_lock);
}

/* sys_tell() - Returns the position of the
 * next byte to be read or written in open
 * file fd, expressed in bytes from the
 * beginning of the file.
 */
static unsigned
sys_tell (int fd)
{
  struct opened_file *entry = get_entry (fd);
  unsigned next_byte;

  lock_acquire (&filesys_lock);
  next_byte = file_tell (entry->file);
  lock_release (&filesys_lock);

  return next_byte;
}

/* sys_close() - Closes file descriptor fd.
 */
static void
sys_close (int fd)
{
  struct opened_file *entry = get_entry (fd);

  lock_acquire (&filesys_lock);
  file_close (entry->file);
  lock_release (&filesys_lock);
  
  list_remove (entry->element);
  free (entry);
}
