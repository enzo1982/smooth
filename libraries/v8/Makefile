########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../..

include $(SMOOTH_PATH)/Makefile-options

# Change these variables to fit your project:
GLOBALDLL   = smooth-js
IMPORTLIB   = libsmooth-js.a

MYCCOPTS    = -I. -I./include

ifeq ($(BUILD_FREEBSD),True)
	MYCCOPTS += -I/usr/local/include
endif

ifeq ($(BUILD_OPENBSD),True)
	MYCCOPTS += -fpermissive -I/usr/local/include
endif

ifeq ($(BUILD_SOLARIS),True)
	MYCCOPTS += -D__C99FEATURES__
endif

ifeq ($(BUILD_HAIKU),True)
	MYCCOPTS += -fpermissive
endif

# Enter object files here:
OBJECTS	    = accessors.o allocation.o api.o assembler.o ast.o atomicops_internals_x86_gcc.o bignum.o bignum-dtoa.o bootstrapper.o builtins.o cached-powers.o checks.o circular-queue.o codegen.o code-stubs.o compilation-cache.o compiler.o contexts.o conversions.o counters.o cpu-profiler.o data-flow.o date.o dateparser.o debug.o debug-agent.o deoptimizer.o disassembler.o diy-fp.o dtoa.o elements.o execution.o experimental-libraries.o factory.o fast-dtoa.o fixed-dtoa.o flags.o frames.o full-codegen.o func-name-inferrer.o gdb-jit.o global-handles.o handles.o heap.o heap-profiler.o hydrogen.o hydrogen-instructions.o ic.o incremental-marking.o inspector.o interface.o interpreter-irregexp.o isolate.o jsregexp.o libraries.o lithium.o lithium-allocator.o liveedit.o liveobjectlist.o log.o log-utils.o mark-compact.o messages.o objects.o objects-printer.o objects-visiting.o once.o parser.o preparse-data.o preparser.o profile-generator.o property.o regexp-macro-assembler.o regexp-macro-assembler-irregexp.o regexp-stack.o rewriter.o runtime.o runtime-profiler.o safepoint-table.o scanner.o scanner-character-streams.o scopeinfo.o scopes.o serialize.o snapshot-common.o snapshot-empty.o spaces.o store-buffer.o string-search.o string-stream.o strtod.o stub-cache.o token.o type-info.o unicode.o utils.o v8.o v8-counters.o v8conversions.o v8threads.o v8utils.o variables.o version.o zone.o

ifeq ($(BUILD_WIN32),True)
	OBJECTS += platform-win32.o win32-math.o
endif

ifeq ($(BUILD_LINUX),True)
	OBJECTS += platform-linux.o platform-posix.o
endif

ifeq ($(BUILD_FREEBSD),True)
	OBJECTS += platform-freebsd.o platform-posix.o
endif

ifeq ($(BUILD_OPENBSD),True)
	OBJECTS += platform-openbsd.o platform-posix.o
endif

ifeq ($(BUILD_NETBSD),True)
	OBJECTS += platform-openbsd.o platform-posix.o
endif

ifeq ($(BUILD_SOLARIS),True)
	OBJECTS += platform-solaris.o platform-posix.o
endif

ifeq ($(BUILD_OSX),True)
	OBJECTS += platform-macos.o platform-posix.o
endif

ifeq ($(BUILD_QNX),True)
	OBJECTS += platform-qnx.o platform-posix.o
endif

ifeq ($(BUILD_HAIKU),True)
	OBJECTS += platform-haiku.o platform-posix.o
endif

ifeq ($(BUILD_X64),True)
	OBJECTS += x64/assembler-x64.o x64/builtins-x64.o x64/codegen-x64.o x64/code-stubs-x64.o x64/cpu-x64.o x64/debug-x64.o x64/deoptimizer-x64.o x64/disasm-x64.o x64/frames-x64.o x64/full-codegen-x64.o x64/ic-x64.o x64/lithium-x64.o x64/lithium-codegen-x64.o x64/lithium-gap-resolver-x64.o x64/macro-assembler-x64.o x64/regexp-macro-assembler-x64.o x64/stub-cache-x64.o
else
	OBJECTS += ia32/assembler-ia32.o ia32/builtins-ia32.o ia32/codegen-ia32.o ia32/code-stubs-ia32.o ia32/cpu-ia32.o ia32/debug-ia32.o ia32/deoptimizer-ia32.o ia32/disasm-ia32.o ia32/frames-ia32.o ia32/full-codegen-ia32.o ia32/ic-ia32.o ia32/lithium-ia32.o ia32/lithium-codegen-ia32.o ia32/lithium-gap-resolver-ia32.o ia32/macro-assembler-ia32.o ia32/regexp-macro-assembler-ia32.o ia32/stub-cache-ia32.o
endif

OBJECTS += extensions/externalize-string-extension.o extensions/gc-extension.o

# Enter additional library dependencies here
ifeq ($(BUILD_WIN32),True)
	LIBS = -lwinmm -lws2_32
endif

ifeq ($(BUILD_FREEBSD),True)
	LIBS = -L/usr/local/lib -lexecinfo
endif

ifeq ($(BUILD_OPENBSD),True)
	LIBS = -L/usr/local/lib -lexecinfo
endif

# Additional things to do on clean:
CLEANCMD1 = rm -f experimental-libraries.cc libraries.cc jsmin.pyc

## Do not change anything below this line. ##

include $(SMOOTH_PATH)/Makefile-directories
include $(SMOOTH_PATH)/Makefile-commands

libraries.cc: runtime.js v8natives.js array.js string.js uri.js math.js messages.js apinatives.js date.js regexp.js json.js liveedit-debugger.js mirror-debugger.js debug-debugger.js macros.py
	chmod a+x ./js2c.py
	./js2c.py libraries.cc CORE off runtime.js v8natives.js array.js string.js uri.js math.js messages.js apinatives.js date.js regexp.js json.js liveedit-debugger.js mirror-debugger.js debug-debugger.js macros.py

experimental-libraries.cc: proxy.js collection.js macros.py
	chmod a+x ./js2c.py
	./js2c.py experimental-libraries.cc EXPERIMENTAL off proxy.js collection.js macros.py
