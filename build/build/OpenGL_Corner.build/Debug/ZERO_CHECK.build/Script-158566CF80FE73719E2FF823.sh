#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Users/chmosquera/2023/OpenGL_Corner/OpenGL_Corner/build
  make -f /Users/chmosquera/2023/OpenGL_Corner/OpenGL_Corner/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Users/chmosquera/2023/OpenGL_Corner/OpenGL_Corner/build
  make -f /Users/chmosquera/2023/OpenGL_Corner/OpenGL_Corner/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Users/chmosquera/2023/OpenGL_Corner/OpenGL_Corner/build
  make -f /Users/chmosquera/2023/OpenGL_Corner/OpenGL_Corner/build/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Users/chmosquera/2023/OpenGL_Corner/OpenGL_Corner/build
  make -f /Users/chmosquera/2023/OpenGL_Corner/OpenGL_Corner/build/CMakeScripts/ReRunCMake.make
fi

