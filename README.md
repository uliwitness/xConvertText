# xConvertText
HyperCard XCMD for converting text files from MacRoman with CR to UTF8 with LF line breaks (and for converting files it converted to UTF8 back to MacRoman).

# Prerequisites
To build this, you need

* CodeWarrior 10 Gold for Macintosh System 7.0
* A Mac to run it on (or an emulator like Basilisk II or SheepShaver)
* StuffIt 5 to unpack the `.sit` files
* The HyperCard stack in the `CheapVersionControl.sit` archive in this repository
  to copy the `.rsrc` files back from the data fork into the resource fork, and
  to convert the source files back from UTF8 with LF to MacRoman with CR line endings.
  (Its "Revert" button can be used to do this)
