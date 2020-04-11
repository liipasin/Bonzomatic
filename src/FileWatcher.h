#ifndef FILEWATCHER_H
#define FILEWATCHER_H

namespace FileWatcher
{
  int Init(const char * path);
  bool NeedsReload(int watch);
}

#endif
