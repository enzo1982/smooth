########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../..

# Change these variables to fit your project:
GLOBALDLL   = smooth-js
IMPORTLIB   = libv8.a

MYCCOPTS    = -I.

# Enter object files here:
OBJECTS	    = accessors.o allocation.o api.o assembler.o ast.o bootstrapper.o builtins.o checks.o circular-queue.o codegen.o code-stubs.o compilation-cache.o compiler.o contexts.o conversions.o counters.o cpu-profiler.o data-flow.o dateparser.o debug.o debug-agent.o disassembler.o diy-fp.o dtoa.o dtoa-config.o execution.o factory.o fast-dtoa.o fixed-dtoa.o flags.o flow-graph.o frame-element.o frames.o full-codegen.o func-name-inferrer.o global-handles.o handles.o hashmap.o heap.o heap-profiler.o ic.o interpreter-irregexp.o jsregexp.o jump-target.o libraries.o liveedit.o log.o log-utils.o mark-compact.o messages.o objects.o oprofile-agent.o parser.o platform-win32.o profile-generator.o property.o regexp-macro-assembler.o regexp-macro-assembler-irregexp.o regexp-stack.o register-allocator.o rewriter.o runtime.o scanner.o scopeinfo.o scopes.o serialize.o snapshot-common.o snapshot-empty.o spaces.o string-stream.o stub-cache.o token.o top.o type-info.o unicode.o utils.o v8.o v8-counters.o v8threads.o variables.o version.o virtual-frame.o vm-state.o zone.o

ifeq ($(BUILD_X64),True)
	OBJECTS += fast-codegen.o jump-target-heavy.o virtual-frame-heavy.o
	OBJECTS += x64/assembler-x64.o x64/builtins-x64.o x64/codegen-x64.o x64/cpu-x64.o x64/debug-x64.o x64/disasm-x64.o x64/fast-codegen-x64.o x64/frames-x64.o x64/full-codegen-x64.o x64/ic-x64.o x64/jump-target-x64.o x64/macro-assembler-x64.o x64/regexp-macro-assembler-x64.o x64/register-allocator-x64.o x64/stub-cache-x64.o x64/virtual-frame-x64.o
else
	OBJECTS += jump-target-heavy.o virtual-frame-heavy.o
	OBJECTS += ia32/assembler-ia32.o ia32/builtins-ia32.o ia32/codegen-ia32.o ia32/cpu-ia32.o ia32/debug-ia32.o ia32/disasm-ia32.o ia32/fast-codegen-ia32.o ia32/frames-ia32.o ia32/full-codegen-ia32.o ia32/ic-ia32.o ia32/jump-target-ia32.o ia32/macro-assembler-ia32.o ia32/regexp-macro-assembler-ia32.o ia32/register-allocator-ia32.o ia32/stub-cache-ia32.o ia32/virtual-frame-ia32.o
endif

# Enter additional library dependencies here
LIBS	  = -lwinmm -lws2_32

# Additional things to do on clean:
CLEANCMD1 = rm -f libraries.cc libraries-empty.cc jsmin.pyc

## Do not change anything below this line. ##

include $(SMOOTH_PATH)/Makefile-directories
include $(SMOOTH_PATH)/Makefile-commands

libraries.cc: runtime.js v8natives.js array.js string.js uri.js math.js messages.js apinatives.js date.js regexp.js json.js liveedit-debugger.js mirror-debugger.js debug-debugger.js macros.py
	python js2c.py libraries.cc libraries-empty.cc CORE runtime.js v8natives.js array.js string.js uri.js math.js messages.js apinatives.js date.js regexp.js json.js liveedit-debugger.js mirror-debugger.js debug-debugger.js macros.py
