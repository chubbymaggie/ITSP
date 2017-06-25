Author: Claire Le Goues
Modified by: Zak Fry
Contact: zpf5a@virginia.edu, legoues@cs.virginia.edu
Date Created: July 11, 2008
Date Modified: October 24, 2013

Current publicly-available export of GenProg: svn rev 1469

This README contains a trail-map for the use and extension of
"repair", used for the GenProg experiments that appear ASE 2013 (and
others, and certainly all results thereafter, at least for now).

This README describes the use of GenProg v3.0, a.k.a. "repair."
Previous versions exist and are described elsewhere.  These
instructions closely mimic those associated with the previous version
of repair (v2.0 and v2.3).  The command line options and code for
previous versions of repair remain in this version of the code and
should work as they did previously -- please see README's from the
previous releases for more detailed explanations of the relevant
options.

These instructions primarily address the repair of C programs using
the updated genetic algorithm (see our ASE 2013 paper).  Other search
strategies and language front-ends exist.  In the interest of
expediency, I am focusing on the type of repair I understand the best
and for which results are appearing soonest.  Many of these
instructions translate directly to different language front-ends,
however, so you should be able to figure out ASM/ELF level repair
pretty trivially if you understand C-level repair.  If you have
questions about neutral-space search or ASM/ELF-level repair, I
encourage you to email Eric Schulte (eschulte@cs.unm.edu).  There is
also a README.asm in the src/ directory, though I have no idea how
accurate it is.  The code is commented, which may help if you need
more info on any aspect of repair.

These instructions include mention of how to compile for shader repair, but I
unfortunately do not know how to run those experiments.  Email Wes for pointers
to who to ask (weimer@cs.virginia.edu)

Refer to the README associated with the benchmarks for information and examples
regarding the use of modify/repair on particular examples.

Caveat 0: Read the entirety of this README before diving into a particular
benchmark because I do not repeat instructions applying to all experiments in
the benchmark-specific READMEs.

Caveat 1: GenProg operates on pre-processed code, meaning that the generated
patches are sometimes non-obvious.  Compare pre-processed code to original code
to get a handle on what is going on. 

Caveat 2: These instructions are not comprehensive, for which I apologize.
repair has a huge number of command-line options and implemented behaviors.
I've tried to include enough info to get you started.

*********
* 0. Benchmark program tarballs
*********

If you'd like to use the tarballs associated with the repair scenarios
we ran for ICSE 2012 (we used similar scenarios but with different
parameters for GECCO 2012) and ASE 2013 "out of the box", you will
need to use them with the VM image we provide, as they assume a
certain directory structure.  Check out the instructions associated
with the disk image we provide on genprog.cs.virginia.edu on how to
set up the VirtualBox image.

*********
* 1. Basics
*********

The genprog prototype assumes bash scripting and standard utilities;
win32 most likely requires cygwin.  Some number of experiments can be
performed in OS X, with some extra legwork (non-exhaustive list:
compiling CIL for OS X is notably annoying; you'll need to use gcc-4.0
of the default gcc-4.2 when compiling variants for coverage).

Ensure that sh is symlinked to bash, not dash (as is the default on
Ubuntu), on your machine.

Our prototype is written in OCaml.  It should work for releases
starting from at least 3.09.3; the most recent version of OCaml is
4.01.x.  Our code has been shown to work with versions through 3.12.x,
but it can cause some compilation shenanigans with the CIL library.
GenProg may build with the newest version of OCaml, but we have not
specifically tested it -- use at your own risk.  If you have a
standard linux distro use your package manager. If not, it is
available here:

    http://caml.inria.fr/ocaml/release.en.html

*********
* 2. Building
*********

0) CIL

We use CIL to parse C and manipulate ASTs. Anything newer than CIL 1.3.7 should
work:

    http://hal.cs.berkeley.edu/cil/

Getting CIL up and running (once you have ocaml) should be as simple as:

        cd cil
        ./configure
        make 
        make cillib

"make cillib" is the important part, so if "make" chokes, try just "make
cillib." 

Set the environment variable CIL to point to whereever cil.spec ends
up, or put it in your shell startup script:

        export CIL=/home/claire/cil

Claire notes: I have had trouble getting later versions of CIL and/or OCaml to
build CIL in native code (producing .cmx and .cmxa).  A hack solution to this is
to put NATIVECAML := 1 somewhere near the top of the Makefile, unguarded by any
ifdefs.   

A source code tarball for CIL version 1.3.7 is included with this package.
cil-cg.tar.gz is a version of CIL with extensions to support the parsing of
OpenGL shaders.  Compile in the same way as you do for 1.3.7, change the CIL
variable, make clean, export USE_PELLACINI=true, and re-make repair to use it.

1) Building repair

Once CIL is installed and the environment variable set, "make" in the
either the top level or the genprog-code/src directory should do the trick.  
Make sure the OCAML_OPTIONS line that points to the obj directory for CIL 
is referencing your distro (that is, change x86_LINUX to x86_DARWIN if 
you're on OSX).

The build process will produce several artifacts:
  -> repair: the main GenProg repair program 
  -> nhtserver: the server for the networked hash table (optional)
  -> distserver: the server for the distributed GA search (optional)
  -> dll_elf_stubs.so, lib_elf_stubs.a, libelf.o: utilities for elf manipulation

Additional build targets are:
  -> doc: requires ocamldoc, generates html documentation on the API (useful if
     you want to understand or extend the code) in doc/ 
  -> testsuite: runs the tests in test/.  The testsuite is a work in progress
     and thus I make no guarantees about its functionality; this documentation
     will be updated as it is.

*********
* 3. General "repair" roadmap
********

I will first outline the DEFAULT (virtually all behaviors may be overridden)
behavior of repair as run fresh on a new C program, assuming all goes well:

0) sanity check:  repair will compile the program and run it on all positive
and negative test cases to check that the behavior is as expected (i.e.,
compiles, passes the positive test cases, fails the negative test cases).
1) If no path files are found and no other localization strategy is specified,
repair will instrument the program for coverage information and compile and
run the instrumented program on the test cases to generate positive and
negative paths for localization.
2) Repair then computes the comprehensive set of single-edit
mutations.  From there, it computes approximate program equivalence
between said mutations to remove redundant potential fixes. 
3) The repair process then proceeds by iteratively selecting the most
promising mutations (as dictated by the search strategy -- default is
fault localization weight followed by the probability that it will
fail tests early) and combining them into a "super mutant".  Super
mutants incorporate many switch-guarded mutations into a single
compilable program so that compilation only has to be performed once
to test many mutations.
4) Mutations are then tested in the determined search order to check
for repairs.  The order that tests are run is also prioritized (by the
likelihood a test will fail given historical data).  Mutations are
tested to first failure.
5) The process iterates until either the entire reduced set of
semantically unique mutations have been evaluated or until a repair is found.
6) If a repair is found, the source code for that variant will be printed to
disk either in repair/ or repair.c (depending on whether the source code is one
file or many).  If minimization is specified (not by default), the repair will
then be minimized, and related files will be output Minimization_Files/.

repair produces the following artifacts by default:

  -> program.cache: caches the representation with localization information.  If
     a cache is found in the directory, and repair is not told otherwise, it
     will load the repair from this cache.  By default this skips the
     localization/coverage step and the sanity step.
  -> repair.cache: the test cache
  -> coverage.path.pos and coverage.path.neg: the path files used for localization.
  -> repair.debug.N where N is the seed used for the random-number generator.
     All output from repair is sent both to standard out and repair.debug.N

If the caches and/or paths are found on a run on the program, they will be used.
You can turn off this loading behavior with --no-rep-cache and/or
--no-test-cache and/or --regen-paths (if you're using path-based localization
but want to regenerate them).  You can also specify alternative names for the repair
cache file using --rep-cache X.

Thus, to run repair on any program, AT THE VERY LEAST, you will need:

  -> (C) Source code.  This may be in one or many files, but it *must* be
     preprocessed.  (Section 3.2)
  -> compile script(s) (Section 3.3)
  -> test script(s) (Section 3.4)

You might also want:
  -> localization info: there are several options here; check the output of
  ./repair --help, in particular near --fault-scheme and --fix-scheme for
  options and how to use them.

I highly recommend that you stop between acquiring the program source and the
compile and test scripts and make sure that you can run them manually first.
Check the permissions on those scripts in particular as they must be executable.

The test directory also contains gcd-test/, an example repair scenario
for the gcd program; you may find it useful as a reference.

*******
* 3.1. Search options
*******

This version of GenProg is being released in tandem with our ASE 2013
paper in which we explore exhaustively enumerating all single-edit
mutations and various search space reduction techniques.  As such,
this section will mostly cover this functionality.  All previous
search functionality is still present in the code however (e.g. most
notably we used [--search "ga"] in the past).

For the most recent instantiation of GenProg, the search type should
be specified as follows: 
   --search ww

The following optimizations can be used to facilitate the repair process:
   --ignore-dead-code  -- do not make known-dead mutations.
   --ignore-standard-headers  -- do not mutate C library #include headers. 
   --ignore-equiv-appends  -- do not make equivalent append mutations.
   --ignore-string-equiv-fixes  -- do not consider string-equivalent fixes twice.
   --ignore-untyped-returns  -- do not insert 'return' if the types mismatch.
By default, we include all of these in our ASE 2013 experiments.

An additional optimization is that of super-mutants, explained above.
The necessary options to enable super mutants are as follows:
   --super-mutant true
   --super-mutant-size X
We use a value of 50 for X in the associated ASE 2013 experiments.

*******
* 3.2. Input program
*******

Relevant command-line options:

  --program X (required)
  --rep X (optional but encouraged)
  --prefix X (necessary for multi-file repair) 

You need the source code of a program with a deterministic bug that you can
expose with test cases (one or many).  

0) Preprocessing

repair can operate on either a single C file or multiple C files, but they will need to be preprocessed.  There are several possibilities for where to get such preprocessed code:

A) If you're running a scenario in a VM as downloaded from the genprog website,
the preprocessed code is included and does not need to be regenerated.  The
other benchmarks from previous papers include in their READMEs the mechanisms we used to generate the preprocessed source; you will almost certainly need to
regenerate them as preprocessed source tends to be machine-specific.

B) The original source code sometimes suffices (only true for the smallest of
programs, such as GCD).

C) A single source file can often be preprocessed by passing -E to gcc: gcc -E
uniq.c > uniq.i

(uniq.i is the preprocessed source version of uniq.c)

D) If a benchmark involves modifying one file or module of a larger program
(e.g., openldap), the preprocessed source can be obtained by hijacking the
benchmark's original build process.  Build the original benchmark source, search
the compiler output for the line that compiles the file you need, copy it, cd
into the appropriate directory, add "--save-temps" as a flag to gcc in the line,
producing foo.i in that directory, where foo.i is your input source code.

E) If a benchmark consists of more than one source file and repair is to be run
on the entire (combined) program (e.g., nullhttpd), use CIL to "combine" the
source code into one file (turning all the nullhttpd source code into, for
example, httpd_comb.c). To do this, use "cilly" instead of "gcc". For nullhttpd:

        cd nullhttpd-0.5.0/src
        make CC="/home/weimer/src/cil/bin/cilly --merge --keepmerged"

This will generate httpd_comb.c in nullhttpd-0.5.0/httpd/bin

1) Specifying the program

The program is specified with --program.  By default, repair will try to figure
out what kind of program you're trying to repair, be it C, asm, etc, based on
the argument passed to --program.  You can be explicit by adding the --rep
file_type argument; this switch provides a number of options even within one
language family (cilpatch vs cilast, for example; patch is the default).  The
--rep argument is important if you are using multiple C files because repair
uses the extension of the argument passed to --program to automatically guess
the type of program under repair, and it might get confused if given a .txt
absent further instructions.

In the single-file case, you specify the source code with the --program command
line option.

In the multi-file case: put all preprocessed files in one directory.  List all
files, one per line, in a text file; do not include the top-level directory in
which they are placed.  Specify --prefix top-level-directory-name and --program
list_of_files.txt 

For example, if you have a.i, b.i, and c.i, put them all in one directory:
> ls preprocessed/
    a.i
    b.i
    c.i

Create a text file, e.g., source.txt:
> cat source.txt
a.i
b.i
c.i

To repair, include the following flags:
--program source.txt
--rep c
--prefix preprocessed

********
* 3.3 Compilation
*******

--compiler compiler-name
--compiler-command "compilation command"

compilation is governed by the compiler command and the specified compiler.  The
default compiler command is: 

"__COMPILER_NAME__ -o __EXE_NAME__ __SOURCE_NAME__ __COMPILER_OPTIONS__ 2>/dev/null >/dev/null"

where each of the __KEYWORDS__ is replaced at each compilation with the
associated concrete instance.  __COMPILER_NAME__ is "gcc" by default.  You can
change either just the compiler name (--compiler) or the entire command
(--compiler-command) to be whatever you want, bearing in mind that the key words
in the default are the only ones currently available.  Feel free to add your own
by modifying the source code if you want, though we have yet to need to.
Anything more complex than gcc -o foo foo.c can usually be accomplished with a
shell script; examples appear in the ICSE 2012 tarballs.  But, for example, if
you have a compile.sh that you've written to do anything more complicated than
gcc -o foo foo.c, you might say:

--compiler "./compile.sh"
--compiler-command "__COMPILER_NAME__ __SOURCE_NAME__"

Or similar.

*******
* 3.4 Testing
******

0) Scripts

Relevant command-line options:

--test-script script-name
--test-command "test command"

Testing is governed by a similar mechanism.  The default test script is "./test.sh"

The default test command is:
"__TEST_SCRIPT__ __EXE_NAME__ __TEST_NAME__ __PORT__ __SOURCE_NAME__ __FITNESS_FILE__ 1>/dev/null 2>/dev/null" 

It may be modified as with the compiler command above, with slightly different
key words.  It will almost certainly need at least __TEST_NAME__.  The test
names are "p#" and "n#" where # is the number of the test case and p is for
positive test cases and n is for negative test cases.  You may also specify s,
for "single test case" which should write the fitness as a (potentially list of)
floating point number(s) to the fitness file.  This is less common and used
primarily for graphics-shader based experiments.

Suggestion: your test scripts should likely include commands like ulimit or
another utility to limit runtime and memory consumption for your program.  We
have a small C utility called "limit" that we use often use for the purpose of
avoiding infinite loops, but any similar mechanism will work as well.

1) Other concerns 

Sanity checking:
  --sanity yes
  --skip-failed-sanity-tests
Running a sanity check before checking any potential repair candidates
ensures that test suite behaves as intended on the original program
(i.e. all positive tests pass and all negative tests fail).  In
general sanity checking is a good practice, however we have found
certain tests cases that behave inconsistently (perhaps due to
nondeterminism), and thus include the --skip-failed-sanity-tests
option.  Any tests that fail sanity should generally be checked
manually to examine the cause of said failures, but should the user
want to discard any failed tests by default, this option can be used.

Testing:
  --pos-tests N
  --neg-tests N
  --fitness-in-parallel N 
  --sample X

--pos-tests and --neg-tests specify the number of positive and negative tests
  respectively.

If --fitness-in-parallel is > 1, more than one test case will be run in
parallel.

--sample X sets the sample size of the positive test cases.  < 1.0 (the default)
  uses sampling.  

There are a number of other options relevant here (--samp-strat, for example);
consult ./repair --help for more.

******
* 4 Other command-line options
*****

repair has a large number of other command-line options controlling setup, GA
parameters, search type, minimization...the works.  You may specify them either
at the command line or using a config file (or both!  They are parsed in order,
so the later ones take precedence if there are repeats).  I highly recommend
downloading the tarballs of the experimental setup for the ICSE 2012 benchmark
set and consulting the configuration files associated with each scenario to get
an idea of what they are, or calling ./repair --help I have tried to make their
descriptions somewhat indicative, and thus I omit additional instructions here
for the sake of brevity.

CAVEAT: as of 5/10/12, I have not yet regenerated the configuration files for
those benchmarks to make use of the refactored version of the command line
options.  Thus, the scenario tarball config files contain a number of deprecated
options.  HOWEVER: repair handles a large number of deprecated options, so you
can still run repair on those scenarios, just be mindful that not all of those
options are currently available in those exact forms.  I will fix this soon.

*******
* 5 Misc observations
******

Permissions are funny. Sometimes they are set so that "sh test.sh" executes but
"./test-good.sh" gives a "permission denied".  Check this, and then use chmod.

Use the --seed flag to specify the random seed for reproducible runs.

If a repair is found, the code output to repair.c is not the minimized repair.
Minimization can be performed on the initial repair by passing --minimization in
the config file.

The --continue flag bypasses modify's default behavior to quit when it finds the
first fixed variant.
