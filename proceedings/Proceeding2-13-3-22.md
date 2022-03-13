<h1 align="center" class="smallcaps"> Proceeding 2 </h1>
<h4 align="center" class="smallcaps"> 13th March, 2022 </h4>

### • Current 

* Presentation is complete. 

**RESEARCH**:

The a26 has three ICs on its motherboard: 6532 RIOT (ram), 6507 CPU, TIA
controller (video). These are the three units that we have to emulate. 6507
is a 6502 (popular CPU) but in a smaller package.

Games are loaded from "cartridges". These are external ROMS. ROMS are available
in various formats (file extensions .ROM, .BIN etc.). We cannot implement all the available formats, so we implement a subset of them (maybe even one). Which to implement
is decided by the games we demonstrate and the format they are available in. Or
alternatively based on which one is simpler. 

**DEVELOPMENT**:

~/development-model.txt has been created.


### • Next

* Look into all the three ICs and learn how they communicate with each 
other.
* Look into cartridge formats. (NES related websites have a lot of info on
  them).
* Short-list the games that we would like to demonstrate.
* Update development-model.txt to reflect present changes.

### • Carry from last proceeding

* Try writing programs for a26.
* Decide Language and Libraries. (For graphics, mainly)
