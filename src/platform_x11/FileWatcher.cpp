#include "FileWatcher.h"

#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <sys/inotify.h>
#include <unistd.h>

namespace FileWatcher
{
  int Init(const char * path) {
    int inotifyFd = inotify_init();
    if (inotifyFd < 0) {
      return -1;
    }

    int inotifyFdFlags = fcntl(inotifyFd, F_GETFL, 0);
    fcntl(inotifyFd, F_SETFL, inotifyFdFlags | O_NONBLOCK);

    int wd = inotify_add_watch(inotifyFd, path, IN_MODIFY);
    if (wd < 0) {
      return -1;
    }

    return inotifyFd;
  }

#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

  bool NeedsReload(int watch) {
    if (watch < 0) {
      return false;
    }

    char buf[BUF_LEN] __attribute__ ((aligned(8)));;
    ssize_t numRead = read(watch, buf, BUF_LEN);

    // Don't check the actual notify event - we're happy with anything.
    return numRead > 0;
  }
}
