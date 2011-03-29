########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../..

include $(SMOOTH_PATH)/Makefile-options

# Change these variables to fit your project:
GLOBALDLL   = smooth-js
IMPORTLIB   = libsmooth-js.a

MYCCOPTS    = -I. -I./include

ifeq ($(BUILD_SOLARIS),True)
	MYCCOPTS += -D_STDC_C99 -D_XPG6
endif

# Enter object files here:
OBJECTS	    = accessors.o allocation.o api.o assembler.o ast.o bignum.o bignum-dtoa.o bootstrapper.o builtins.o cached-powers.o checks.o circular-queue.o codegen.o code-stubs.o compilation-cache.o compiler.o contexts.o conversions.o counters.o cpu-profiler.o data-flow.o dateparser.o debug.o debug-agent.o disassembler.o diy-fp.o dtoa.o execution.o factory.o fast-dtoa.o fixed-dtoa.o flags.o frame-element.o frames.o full-codegen.o func-name-inferrer.o global-handles.o handles.o hashmap.o heap.o heap-profiler.o ic.o interpreter-irregexp.o jsregexp.o jump-target.o libraries.o liveedit.o log.o log-utils.o mark-compact.o messages.o objects.o objects-visiting.o oprofile-agent.o parser.o preparse-data.o preparser.o profile-generator.o property.o regexp-macro-assembler.o regexp-macro-assembler-irregexp.o regexp-stack.o register-allocator.o rewriter.o runtime.o scanner.o scanner-base.o scopeinfo.o scopes.o serialize.o snapshot-common.o snapshot-empty.o spaces.o string-search.o string-stream.o strtod.o stub-cache.o token.o top.o type-info.o unicode.o utils.o v8.o v8-counters.o v8threads.o variables.o version.o virtual-frame.o zone.o

ifeq ($(BUILD_WIN32),True)
	OBJECTS += platform-win32.o
endif

ifeq ($(BUILD_LINUX),True)
	OBJECTS += platform-linux.o platform-posix.o
endif

ifeq ($(BUILD_FREEBSD),True)
	OBJECTS += platform-freebsd.o platform-posix.o
endif

ifeq ($(BUILD_SOLARIS),True)
	OBJECTS += platform-solaris.o platform-posix.o
endif

ifeq ($(BUILD_OSX),True)
	OBJECTS += platform-macos.o platform-posix.o
endif

ifeq ($(BUILD_X64),True)
	OBJECTS += jump-target-heavy.o virtual-frame-heavy.o
	OBJECTS += x64/assembler-x64.o x64/builtins-x64.o x64/codegen-x64.o x64/code-stubs-x64.o x64/cpu-x64.o x64/debug-x64.o x64/disasm-x64.o x64/frames-x64.o x64/full-codegen-x64.o x64/ic-x64.o x64/jump-target-x64.o x64/macro-assembler-x64.o x64/regexp-macro-assembler-x64.o x64/register-allocator-x64.o x64/stub-cache-x64.o x64/virtual-frame-x64.o
else
	OBJECTS += jump-target-heavy.o virtual-frame-heavy.o
	OBJECTS += ia32/assembler-ia32.o ia32/builtins-ia32.o ia32/codegen-ia32.o ia32/code-stubs-ia32.o ia32/cpu-ia32.o ia32/debug-ia32.o ia32/disasm-ia32.o ia32/frames-ia32.o ia32/full-codegen-ia32.o ia32/ic-ia32.o ia32/jump-target-ia32.o ia32/macro-assembler-ia32.o ia32/regexp-macro-assembler-ia32.o ia32/register-allocator-ia32.o ia32/stub-cache-ia32.o ia32/virtual-frame-ia32.o
endif

OBJECTS += extensions/externalize-string-extension.o extensions/gc-extension.o

# Enter additional library dependencies here
ifeq ($(BUILD_WIN32),True)
	LIBS = -lwinmm -lws2_32
endif

ifeq ($(BUILD_FREEBSD),True)
	LIBS = -lexecinfo
endif

# Additional things to do on clean:
CLEANCMD1 = rm -f libraries.cc libraries-empty.cc jsmin.pyc

## Do not change anything below this line. ##

include $(SMOOTH_PATH)/Makefile-directories
include $(SMOOTH_PATH)/Makefile-commands

libraries.cc: runtime.js v8natives.js array.js string.js uri.js math.js messages.js apinatives.js date.js regexp.js json.js liveedit-debugger.js mirror-debugger.js debug-debugger.js macros.py
	python js2c.py libraries.cc libraries-empty.cc CORE runtime.js v8natives.js array.js string.js uri.js math.js messages.js apinatives.js date.js regexp.js json.js liveedit-debugger.js mirror-debugger.js debug-debugger.js macros.py
