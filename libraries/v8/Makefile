########## smooth directory makefile ##########

# Change these variables to fit this location:
SMOOTH_PATH = ../..

include $(SMOOTH_PATH)/Makefile-options

# Disable PowerPC builds of V8 on OS X for now.
ifeq ($(BUILD_OSX),True)
	BUILD_PPC   = False
	BUILD_PPC64 = False
endif

# Change these variables to fit your project:
GLOBALDLL   = smooth-js
IMPORTLIB   = libsmooth-js.a

MYCCOPTS    = -I. -I./include

ifeq ($(BUILD_WIN32),True)
	MYCCOPTS += -Wno-unused-local-typedefs
endif

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
	MYCCOPTS += -fpermissive -Wno-format
endif

ifeq ($(BUILD_ARM),True)
#	Require ARMv5 for bkpt instruction.
	MYCCOPTS += -march=armv5
endif

# Enter object files here:
OBJECTS	    = accessors.o allocation.o allocation-site-scopes.o allocation-tracker.o api.o arguments.o assembler.o ast.o atomicops_internals_x86_gcc.o bignum.o bignum-dtoa.o bootstrapper.o builtins.o cached-powers.o checks.o codegen.o code-stubs.o code-stubs-hydrogen.o compilation-cache.o compiler.o contexts.o conversions.o counters.o cpu.o cpu-profiler.o data-flow.o date.o dateparser.o debug.o debug-agent.o defaults.o deoptimizer.o disassembler.o diy-fp.o dtoa.o elements.o elements-kind.o execution.o experimental-libraries.o factory.o fast-dtoa.o fixed-dtoa.o flags.o frames.o full-codegen.o func-name-inferrer.o gdb-jit.o global-handles.o handles.o heap.o heap-profiler.o heap-snapshot-generator.o hydrogen.o hydrogen-bce.o hydrogen-bch.o hydrogen-canonicalize.o hydrogen-check-elimination.o hydrogen-dce.o hydrogen-dehoist.o hydrogen-environment-liveness.o hydrogen-escape-analysis.o hydrogen-gvn.o hydrogen-infer-representation.o hydrogen-infer-types.o hydrogen-instructions.o hydrogen-load-elimination.o hydrogen-mark-deoptimize.o hydrogen-mark-unreachable.o hydrogen-minus-zero.o hydrogen-osr.o hydrogen-range-analysis.o hydrogen-redundant-phi.o hydrogen-removable-simulates.o hydrogen-representation-changes.o hydrogen-sce.o hydrogen-uint32-analysis.o ic.o icu_util.o incremental-marking.o interface.o interpreter-irregexp.o isolate.o jsregexp.o libraries.o lithium.o lithium-allocator.o lithium-codegen.o liveedit.o log.o log-utils.o mark-compact.o messages.o objects.o objects-printer.o objects-visiting.o once.o optimizing-compiler-thread.o parser.o preparse-data.o preparser.o profile-generator.o property.o regexp-macro-assembler.o regexp-macro-assembler-irregexp.o regexp-stack.o rewriter.o runtime.o runtime-profiler.o safepoint-table.o sampler.o scanner.o scanner-character-streams.o scopeinfo.o scopes.o serialize.o snapshot-common.o snapshot-empty.o spaces.o store-buffer.o string-search.o string-stream.o strtod.o stub-cache.o sweeper-thread.o token.o transitions.o type-info.o types.o typing.o unicode.o utils.o v8.o v8-counters.o v8conversions.o v8threads.o v8utils.o variables.o version.o zone.o

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

ifeq ($(BUILD_GNU),True)
	OBJECTS += platform-hurd.o platform-posix.o
endif

ifeq ($(BUILD_X86),True)
	OBJECTS += ia32/assembler-ia32.o ia32/builtins-ia32.o ia32/codegen-ia32.o ia32/code-stubs-ia32.o ia32/cpu-ia32.o ia32/debug-ia32.o ia32/deoptimizer-ia32.o ia32/disasm-ia32.o ia32/frames-ia32.o ia32/full-codegen-ia32.o ia32/ic-ia32.o ia32/lithium-ia32.o ia32/lithium-codegen-ia32.o ia32/lithium-gap-resolver-ia32.o ia32/macro-assembler-ia32.o ia32/regexp-macro-assembler-ia32.o ia32/simulator-ia32.o ia32/stub-cache-ia32.o
endif

ifeq ($(BUILD_X86_64),True)
	OBJECTS += x64/assembler-x64.o x64/builtins-x64.o x64/codegen-x64.o x64/code-stubs-x64.o x64/cpu-x64.o x64/debug-x64.o x64/deoptimizer-x64.o x64/disasm-x64.o x64/frames-x64.o x64/full-codegen-x64.o x64/ic-x64.o x64/lithium-x64.o x64/lithium-codegen-x64.o x64/lithium-gap-resolver-x64.o x64/macro-assembler-x64.o x64/regexp-macro-assembler-x64.o x64/simulator-x64.o x64/stub-cache-x64.o
endif

ifeq ($(BUILD_ARM),True)
	OBJECTS += arm/assembler-arm.o arm/builtins-arm.o arm/codegen-arm.o arm/code-stubs-arm.o arm/constants-arm.o arm/cpu-arm.o arm/debug-arm.o arm/deoptimizer-arm.o arm/disasm-arm.o arm/frames-arm.o arm/full-codegen-arm.o arm/ic-arm.o arm/lithium-arm.o arm/lithium-codegen-arm.o arm/lithium-gap-resolver-arm.o arm/macro-assembler-arm.o arm/regexp-macro-assembler-arm.o arm/simulator-arm.o arm/stub-cache-arm.o
endif

ifeq ($(BUILD_MIPS),True)
	OBJECTS += mips/assembler-mips.o mips/builtins-mips.o mips/codegen-mips.o mips/code-stubs-mips.o mips/constants-mips.o mips/cpu-mips.o mips/debug-mips.o mips/deoptimizer-mips.o mips/disasm-mips.o mips/frames-mips.o mips/full-codegen-mips.o mips/ic-mips.o mips/lithium-mips.o mips/lithium-codegen-mips.o mips/lithium-gap-resolver-mips.o mips/macro-assembler-mips.o mips/regexp-macro-assembler-mips.o mips/simulator-mips.o mips/stub-cache-mips.o
endif

ifeq ($(BUILD_PPC),True)
ifeq ($(BUILD_OSX),True)
	MYCCOPTS += -Xarch_ppc
endif
	MYCCOPTS += -DV8_TARGET_ARCH_ARM

	OBJECTS += arm/assembler-arm.o arm/builtins-arm.o arm/codegen-arm.o arm/code-stubs-arm.o arm/constants-arm.o arm/cpu-arm.o arm/debug-arm.o arm/deoptimizer-arm.o arm/disasm-arm.o arm/frames-arm.o arm/full-codegen-arm.o arm/ic-arm.o arm/lithium-arm.o arm/lithium-codegen-arm.o arm/lithium-gap-resolver-arm.o arm/macro-assembler-arm.o arm/regexp-macro-assembler-arm.o arm/simulator-arm.o arm/stub-cache-arm.o
endif

ifeq ($(BUILD_PPC64),True)
ifeq ($(BUILD_OSX),True)
	MYCCOPTS += -Xarch_ppc64
endif
	MYCCOPTS += -DV8_TARGET_ARCH_ARM

	OBJECTS += arm/assembler-arm.o arm/builtins-arm.o arm/codegen-arm.o arm/code-stubs-arm.o arm/constants-arm.o arm/cpu-arm.o arm/debug-arm.o arm/deoptimizer-arm.o arm/disasm-arm.o arm/frames-arm.o arm/full-codegen-arm.o arm/ic-arm.o arm/lithium-arm.o arm/lithium-codegen-arm.o arm/lithium-gap-resolver-arm.o arm/macro-assembler-arm.o arm/regexp-macro-assembler-arm.o arm/simulator-arm.o arm/stub-cache-arm.o
endif

OBJECTS += extensions/externalize-string-extension.o extensions/gc-extension.o extensions/statistics-extension.o
OBJECTS += platform/condition-variable.o platform/mutex.o platform/semaphore.o platform/socket.o platform/time.o
OBJECTS += utils/random-number-generator.o

# Enter additional library dependencies here
ifeq ($(BUILD_WIN32),True)
	LIBS = -lwinmm -lws2_32
endif

ifeq ($(BUILD_LINUX),True)
	LIBS = -lrt
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

libraries.cc: runtime.js v8natives.js array.js string.js uri.js math.js messages.js apinatives.js debug-debugger.js mirror-debugger.js liveedit-debugger.js date.js json.js regexp.js arraybuffer.js typedarray.js macros.py
ifeq ($(BUILD_HAIKU),True)
	python ./js2c.py libraries.cc CORE off runtime.js v8natives.js array.js string.js uri.js math.js messages.js apinatives.js debug-debugger.js mirror-debugger.js liveedit-debugger.js date.js json.js regexp.js arraybuffer.js typedarray.js macros.py
else
	chmod a+x ./js2c.py
	./js2c.py libraries.cc CORE off runtime.js v8natives.js array.js string.js uri.js math.js messages.js apinatives.js debug-debugger.js mirror-debugger.js liveedit-debugger.js date.js json.js regexp.js arraybuffer.js typedarray.js macros.py
endif

experimental-libraries.cc: symbol.js proxy.js collection.js object-observe.js generator.js array-iterator.js harmony-string.js harmony-array.js harmony-math.js macros.py
ifeq ($(BUILD_HAIKU),True)
	python ./js2c.py experimental-libraries.cc EXPERIMENTAL off symbol.js proxy.js collection.js object-observe.js generator.js array-iterator.js harmony-string.js harmony-array.js harmony-math.js macros.py
else
	chmod a+x ./js2c.py
	./js2c.py experimental-libraries.cc EXPERIMENTAL off symbol.js proxy.js collection.js object-observe.js generator.js array-iterator.js harmony-string.js harmony-array.js harmony-math.js macros.py
endif
