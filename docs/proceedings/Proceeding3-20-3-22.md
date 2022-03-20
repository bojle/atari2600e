<h1 align="center" class="smallcaps"> Proceeding 2 </h1>
<h4 align="center" class="smallcaps"> 13th March, 2022 </h4>

### • Current 

**RESEARCH**:

A lot has been uncovered this week. Discovered stella has a
debugger/dis-assembler which will be in-dispensable during development.
The .bin file is, as far as i know a "raw" data file. It contains only that
data which the programmer asked the assembler to assemble. This should be the
first file/cartride format we write. I now understand at a deeper level how the 
TIA works.

**DEVELOPMENT**:

* A SDL_demo program which takes an array of pixels and displays it in a window
  has been written and executed successfully. I have a feeling the graphics part
of this emulator won't be as intimidating as I was initially led to believe.
* Some rudimentary code to parse .bin file have been written.
* Major re-structuring of the root directory. Documents now belong to ~/docs and 
source files to ~/src.
* The `test` directory in the stella source has a lot of test roms that we
  could use.

### • Next

* Try to start writing code by next proceeding.

### • Carry from last proceeding

* Try writing programs for a26.
* Look into cartridge formats. (NES related websites have a lot of info on
  them).
* Communication b/w the IC's.
