PROGRAM=steg1ano

# Un/comment the following two lines to use/disable sanitizers (UBSan, LSan & ASan):
#CFLAGS=-fno-omit-frame-pointer -fno-common -fsanitize=undefined,leak,address -fsanitize-recover=all
#LDLIBS=-lasan -lubsan

CC=clang
ASAN_STOP_ON_ERROR=halt_on_error=1:abort_on_error=1:print_legend=0
ASAN_CONT_ON_ERROR=halt_on_error=0:abort_on_error=0:print_legend=0
ASAN_RUNTIME=$(ASAN_CONT_ON_ERROR):detect_leaks=1:leak_check_at_exit=1:symbolize=1:exitcode=1
ASAN_GDB=

CFLAGS+=-gdwarf-4 -ggdb3
LDLIBS+=-lm

# My syntax checker wants access to my compilation flags too, so:
COMPILATION_FLAGS=-Wall -Wextra -Werror=implicit -Werror=uninitialized -Iinclude
include Makefile.inc

# https://stackoverflow.com/a/18258352
rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

TESTS_REC_SRC=$(call rwildcard,test,*.c)
TESTS_REC=$(sort $(TESTS_REC_SRC:.c=))

TESTS_SRC=$(wildcard test/*.c)
TESTS=$(sort $(TESTS_SRC:.c=))

HEADERS=$(call rwildcard,include,*.h)

SRC=$(call rwildcard,src,*.c)
OBJ=$(SRC:.c=.o)

all : project/$(PROGRAM) tests

project/% : project/%.o $(OBJ) $(HEADERS) # Rebuild on header change, too
	$(CC) $(LDFLAGS) $(word 1, $^) $(OBJ) -o $@ $(LDLIBS)

tests : $(TESTS_REC)

test/% : test/%.o $(OBJ) $(HEADERS)
	$(CC) $(LDFLAGS) $(word 1, $^) $(OBJ) -o $@ $(LDLIBS)

help :
	@printf ''
	@echo "  \e[3m\e[31m>> A Fast Workflow for C on GNU/Linux\e[m with \e[1m\e[34moptions\e[m, sanitizers,\n     and even some \e[2m\e[37mless useful functions\e[m."
	@echo ''
	@echo ''
	@echo "  Hint: VS Code will launch foo@debug executions below on (Ctrl-)F5."
	@echo "  Hint: VS Code will rightfully refuse to debug a src/*.c alone."
	@echo "  Hint: Add tests command line args for VS Code in .vscode/launch.json."
	@echo "  Hint: Add your global compilation options in Makefile.inc."
	@echo ''
	@echo '  Module foo of project bar = {src,test}/foo.c + include/bar/foo.h'
	@echo ''
	@echo ''
	@echo " \e[7m\e[32m Project management \e[m"
	@echo ''
	@echo "  \e[2m\e[37mmake                        Compile project and tests\e[m"
	@echo "  \e[2m\e[37mmake project/bar            Compile project/bar.c\e[m"
	@echo '  make project/bar@check      Compile and run project/bar, check leaks'
	@echo "  make project/bar@debug      Compile and run project/bar in debugger"
	@echo ''
	@echo "  \e[2m\e[37mmake new project=bar        Create project bar (excluding unitest)\e[m"
	@echo "  make module name=foo        Add new skeleton module foo to project"
	@echo ''
	@echo "   \e[1m\e[34m New module uses unitest \e[m  unitest=yes"
	@echo ''
	@echo ''
	@echo " \e[7m\e[32m Individual tests \e[m"
	@echo ''
	@echo "  \e[2m\e[37mmake test/foo               Compile test/foo.c\e[m"
	@echo '  make test/foo@check         Compile and run test/foo, check leaks'
	@echo " \e[1m\e[41m make test/foo@debug         Compile and run test/foo in debugger \e[m"
	@echo ''
	@echo ''
	@echo " \e[7m\e[32m All tests \e[m                     \e[21mDefault: Stop on first failed test\e[m"
	@echo ''
	@echo "  \e[2m\e[37mmake tests                  Compile all tests\e[m"
	@echo "  \e[1m\e[41m make tests@check            Compile and run all tests, check leaks \e[m"
	@echo "  \e[2m\e[37mmake tests@check-recursive  Recursive version (for regression)\e[m"
	@echo "  \e[2m\e[37mmake tests@debug            Compile and run all tests in debugger\e[m"
	@echo "  \e[2m\e[37mmake tests@debug-recursive  Recursive version (for regression)\e[m"
	@echo ''
	@echo "   \e[1m\e[34m Continue tests execution \e[m all=yes"
	@echo "   \e[1m\e[34m Command line arguments \e[m   args=\"argv[1] .. argv[argc-1]\""
	@echo ''
	@echo ''
	@echo " \e[7m\e[33m Environment and setup \e[m         \e[21mWill break your existing GDB setup\e[m"
	@echo ''
	@echo '  make my-env-stylish         Call once in your life at school'
	@echo '  make my-box-stylish         Call once in your life on your PC'
	@echo ''

tests@check : $(TESTS)
	@for test in $^; do printf "\e[7m\e[36m** RUNNING $$test **\e[m\n"; make -s $$test@check args='$(args)'; if [ $$? -eq 0 ] ; then printf "\n\e[7m\e[32mSUCCESS\e[m\n" ; else printf "\n\e[6m\e[7m\e[31mFAILED\e[m\n" ; if [ -z $(all) ] ; then break ; fi ; fi ; done

tests@debug : $(TESTS)
	@for test in $^; do printf "\e[7m\e[33m** DEBUGGING $$test **\e[m\n"; make -s $$test@debug args='$(args)'; if [ $$? -eq 0 ] ; then printf "\e[7m\e[32mSUCCESS\e[m\n" ; else printf "\e[6m\e[7m\e[31mFAILED\e[m\n" ; if [ -z $(all) ] ; then break ; fi ; fi ; done

tests@check-recursive : $(TESTS_REC)
	@for test in $^; do printf "\e[7m\e[33m** RUNNING $$test **\e[m\n"; make -s $$test@check args='$(args)'; if [ $$? -eq 0 ] ; then printf "\e[7m\e[32mSUCCESS\e[m\n" ; else printf "\e[6m\e[7m\e[31mFAILED\e[m\n" ; if [ -z $(all) ] ; then break ; fi ; fi ; done

tests@debug-recursive : $(TESTS_REC)
	@for test in $^; do printf "\e[7m\e[33m** DEBUGGING $$test **\e[m\n"; make -s $$test@debug args='$(args)'; if [ $$? -eq 0 ] ; then printf "\e[7m\e[32mSUCCESS\e[m\n" ; else printf "\e[6m\e[7m\e[31mFAILED\e[m\n" ; if [ -z $(all) ] ; then break ; fi ; fi ; done

%@check : %
	@ASAN_OPTIONS=$(ASAN_RUNTIME) UBSAN_OPTIONS=$(ASAN_CONT_ON_ERROR) ./$< $(args)

%@debug : %
	@gdb -q --return-child-result -nh -x ~/.gdbinit -x test/env.gdb --args $< $(args) '-g' '-v'

clean :
	$(RM) project/$(PROGRAM) $(TESTS_REC)
	find . -name "*.o" -delete

deep-clean : clean
	$(RM) .projectile GTAGS GRTAGS GPATH
	find . -name "*~" -delete
	find . -name "._*" -delete
	$(RM) .gdb_history

asan-install :
	@if [ -z "${ASAN_OPTIONS}" ] ; then echo "export ASAN_OPTIONS=$(ASAN_RUNTIME)" >> ~/.bash_aliases ; echo "export UBSAN_OPTIONS=$(ASAN_CONT_ON_ERROR)" >> ~/.bash_aliases ; echo "\n\e[7m\e[32mASan runtime environment updated.\e[m\n\n Now either type 'source ~/.bashrc' and press Enter, or logout and get a new terminal.\n" ; fi ;

module :
	@if [ -z "$(name)" ] ; then echo "Error: Missing module name. Use 'make module name=foo'." ; exit 1 ; fi ;
	@echo '#include <$(PROGRAM)/$(name).h>' > src/$(name).c
	@echo '#ifndef _$(name)_h_\n#define _$(name)_h_\n\n\n#endif\n' > include/$(PROGRAM)/$(name).h
	@echo '#include <stdlib.h>' > test/$(name).c
	@if [ -z "$(unitest)" ] ; then echo '#include <assert.h>' >> test/$(name).c ; else echo '\n#include <$(PROGRAM)/unitest.h>' >> test/$(name).c ; fi ; >> test/$(name).c
	@echo "\n#include <$(PROGRAM)/$(name).h>\n\n" >> test/$(name).c
	@echo -n 'int main ( ' >> test/$(name).c
	@if [ "$(unitest)" ] ; then echo -n 'int argc, char *argv[] ' >> test/$(name).c ; fi ; >> test/$(name).c
	@echo ') {\n' >> test/$(name).c
	@if [ "$(unitest)" ] ; then echo '  unit_test( argc, argv );' >> test/$(name).c ; fi ; >> test/$(name).c
	@echo '\n  exit( EXIT_SUCCESS );\n}\n' >> test/$(name).c

new :
	@if [ -z "$(project)" ] ; then echo "Error: Missing project name. Use 'make new-project name=foo'." ; exit 1 ; fi ;
	@echo 'Creating project' $(project)
	@mkdir -p $(project)/include/$(project)
	@mkdir -p $(project)/src
	@mkdir -p $(project)/project
	@mkdir -p $(project)/test
	@mkdir -p $(project)/.vscode
	@echo ' - Setting up gdb environment'
	@echo 'if !$$_isvoid($$_any_caller_matches)\n\tdefine hook-stop\n\t\twhile $$_thread && $$_any_caller_matches("^__|abort|raise")\n\t\t\tup-silently\n\t\tend\n\tend\nend\n\nset environment ASAN_OPTIONS  $(ASAN_STOP_ON_ERROR):detect_leaks=0\nset environment UBSAN_OPTIONS $(ASAN_STOP_ON_ERROR):print_stacktrace=1\n\nset print pretty on\n' > $(project)/test/env.gdb
	@echo 'handle SIGALRM pass\nset confirm off\ndashboard -enabled off\nrun\ndashboard -enabled on\n\nif !$$_isvoid ($$_exitcode)\n\tquit\nend' >> $(project)/test/env.gdb
	@echo ' - Writing Makefile'
	@echo 'CFLAGS+=$(COMPILATION_FLAGS)' > $(project)/Makefile.inc
	@echo 'PROGRAM='$(project)'' > $(project)/Makefile
	@tail -n +2 Makefile >> $(project)/Makefile
	@echo ' - Linking Makefile and gdb to VS Code'
	@echo '{\n\t"C_Cpp.autocomplete": "default",\n\t"C_Cpp.default.cStandard": "c99",\n\t"C_Cpp.default.intelliSenseMode": "linux-clang-x64",\n\t"C_Cpp.formatting": "clangFormat",\n\t"debug.onTaskErrors": "showErrors",\n\t"C_Cpp.intelliSenseEngine": "default",\n\t"C_Cpp.errorSquiggles": "enabledIfIncludesResolve"\n}\n' > $(project)/.vscode/settings.json
	@echo '{\n\t"version": "2.0.0",\n\t"tasks": [\n\t\t{\n\t\t\t"label": "build-all",\n\t\t\t"type": "shell",\n\t\t\t"options": {\n\t\t\t\t"cwd": "$${workspaceFolder}",\n\t\t\t},\n\t\t\t"command": "make",\n\t\t\t"problemMatcher": [\n\t\t\t\t"$gcc"\n\t\t\t],\n\t\t\t"group": {\n\t\t\t\t"kind": "build",\n\t\t\t\t"isDefault": true\n\t\t\t}\n\t\t}\n\t\t],\n}\n' > $(project)/.vscode/tasks.json
	@echo '{\n\t"version": "0.2.0",\n\t"configurations": [\n\t\t{\n\t\t\t"name": " >> LE DÉBOGUEUR QUI VA BIEN <<",\n\t\t\t"type": "cppdbg",\n\t\t\t"request": "launch",\n\t\t\t"program": "$${fileDirname}/$${fileBasenameNoExtension}",\n\t\t\t"args": [ "-g", "-v" ],\n\t\t\t"stopAtEntry": false,\n\t\t\t"cwd": "$${fileDirname}",\n\t\t\t"externalConsole": false,\n\t\t\t"MIMode": "gdb",\n\t\t\t"miDebuggerArgs": "-nh -x $${workspaceFolder}/test/env.gdb",\n\t\t\t"preLaunchTask": "build-all"\n\t\t\t}\n\t\t]\n}\n' > $(project)/.vscode/launch.json
	@echo ' - Writing project skeleton'
	@echo '#ifndef _$(project)_h_\n#define _$(project)_h_\n\n\n\n#endif\n' > $(project)/include/$(project)/$(project).h
	@echo '#include <stdlib.h>\n#include <stdio.h>\n\n#include <$(project)/$(project).h>\n\nint main ( int argc, char *argv[] ) {\n\n  printf( "%s", "Hollow world!\\n" ); \n\n  exit( EXIT_SUCCESS );\n}\n' > $(project)/project/$(project).c
	@git init $(project)
	@make -s asan-install

my-env-stylish : # Does not require sudo for install
	@if [ ! -z "${GDB_STYLISH}" ] ; then echo '\n  Your GDB is already beautiful ;)\n' ; exit 1 ; fi ;
	@mkdir -p ~/.gdbinit.d
	@echo '$(ASAN_GDB)\nset print pretty on\n' >> ~/.gdbinit.d/init
	@echo "\e[7m\e[33m Installing GDB Dashboard \e[m"
	@mkdir -p ~/.gdb/dashboard
	@wget --quiet https://git.io/.gdbinit -P ~/.gdb/dashboard
	@echo "\e[7m\e[33m Installing Pygments \e[m"
	@pip install pygments # Should be there already
	@echo 'source ~/.gdb/dashboard/.gdbinit' >> ~/.gdbinit
	@echo 'dashboard -style discard_scrollback False' >> ~/.gdbinit
	@echo 'alias g = dashboard' >> ~/.gdbinit
	@echo 'define hookpost-up\n\tdashboard\nend\ndefine hookpost-down\n\tdashboard\nend' >> ~/.gdbinit
	@echo "\e[7m\e[33m Installing GDB Enhanced Features (GEF) \e[m"
	@mkdir -p ~/.gdb/gef
	@git clone https://github.com/hugsy/gef.git ~/.gdb/gef
	@echo 'source ~/.gdb/gef/gef.py' >> ~/.gdbinit
	@echo 'gef config context.enable False' >> ~/.gdbinit
	@echo 'export GDB_STYLISH=1' >> ~/.bash_aliases
	@echo "\e[7m\e[32m Your GDB has been pimped, enjoy! \e[m\n\n Now either type 'source ~/.bashrc' and press Enter, or logout and get a new terminal."
	@make -s asan-install

my-box-stylish :
	@echo "\e[7m\e[33m Installing C-related tools (requires sudo) \e[m"
	@sudo apt install -y clang gdb make binutils git libubsan1 libasan5 gcc valgrind # gcc and valgrind could be left out
	@if ! command -v code ; then echo "\e[7m\e[33m Installing VS Code (requires sudo) \e[m" ; wget --quiet --show-progress https://go.microsoft.com/fwlink/?LinkID=760868 -O code.deb ; sudo dpkg -i code.deb && rm -f code.deb ; fi ;
	@echo "\e[7m\e[33m Installing VS Code stuff for C dev \e[m"
	@code --install-extension ms-vscode.cpptools-extension-pack
	@code --install-extension ms-vscode.hexeditor
	@make -s my-debugger-env-stylish
