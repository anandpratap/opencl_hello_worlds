#!python
import os
from SCons.Builder import Builder
altera_root = os.environ["ALTERAOCLSDKROOT"]
__root = os.environ["__ROOT"]

cxx = "g++"
ccflags = ["-Wall"]

libs = ["altera_sil_mmd", "alteracl", "elf"]

libpath = []
cpppath = []

libpath.append(os.path.join(__root, "hld/host/linux64/lib"))
libpath.append(os.path.join(altera_root, "board/dspba_sil_pcie/linux64/lib"))
libpath.append(os.path.join(altera_root, "host/linux64/lib"))

libs.append("z")
libs.append("rt")
libs.append("pthread")

cpppath.append(os.path.join(__root, "hld/host/include"))
cpppath.append("../thirdparty/")
ccflags.append("-std=c++11")
ccflags.append("-fPIC")


ccflags_debug = ["-O0", "-g", "-DDEBUG"]
ccflags_optimize = ["-O3"]


aoc_builder = Builder(action = 'aoc  -march=emulator $SOURCE -o $TARGET --board p385a_sch_ax115')
