// ·············································································
//	xConvertText HyperCard XCMD
// ·············································································

// -----------------------------------------------------------------------------
//	Headers
// -----------------------------------------------------------------------------

#include <HyperXCmd.h>
#include <Types.h>
#include <Memory.h>
#include <OSUtils.h>
#include <ToolUtils.h>
#include <Files.h>
#include <Aliases.h>
#include <Errors.h>
#include <string.h>
#include <limits.h>
#include "XCmdUtils.h"


// Table of C-strings for converting the non-ASCII MacRoman characters (above 127)
//	into the requisite UTF8 byte sequences:
unsigned char	sMacRomanToUTF8Table[128][5] =
{
	{ 0xc3, 0x84, 0x00, 0x00 }, { 0xc3, 0x85, 0x00, 0x00 }, { 0xc3, 0x87, 0x00, 0x00 }, { 0xc3, 0x89, 0x00, 0x00 },
	{ 0xc3, 0x91, 0x00, 0x00 }, { 0xc3, 0x96, 0x00, 0x00 }, { 0xc3, 0x9c, 0x00, 0x00 }, { 0xc3, 0xa1, 0x00, 0x00 },
	{ 0xc3, 0xa0, 0x00, 0x00 }, { 0xc3, 0xa2, 0x00, 0x00 }, { 0xc3, 0xa4, 0x00, 0x00 }, { 0xc3, 0xa3, 0x00, 0x00 },
	{ 0xc3, 0xa5, 0x00, 0x00 }, { 0xc3, 0xa7, 0x00, 0x00 }, { 0xc3, 0xa9, 0x00, 0x00 }, { 0xc3, 0xa8, 0x00, 0x00 },
	{ 0xc3, 0xaa, 0x00, 0x00 }, { 0xc3, 0xab, 0x00, 0x00 }, { 0xc3, 0xad, 0x00, 0x00 }, { 0xc3, 0xac, 0x00, 0x00 },
	{ 0xc3, 0xae, 0x00, 0x00 }, { 0xc3, 0xaf, 0x00, 0x00 }, { 0xc3, 0xb1, 0x00, 0x00 }, { 0xc3, 0xb3, 0x00, 0x00 },
	{ 0xc3, 0xb2, 0x00, 0x00 }, { 0xc3, 0xb4, 0x00, 0x00 }, { 0xc3, 0xb6, 0x00, 0x00 }, { 0xc3, 0xb5, 0x00, 0x00 },
	{ 0xc3, 0xba, 0x00, 0x00 }, { 0xc3, 0xb9, 0x00, 0x00 }, { 0xc3, 0xbb, 0x00, 0x00 }, { 0xc3, 0xbc, 0x00, 0x00 },
	{ 0xe2, 0x80, 0xa0, 0x00 }, { 0xc2, 0xb0, 0x00, 0x00 }, { 0xc2, 0xa2, 0x00, 0x00 }, { 0xc2, 0xa3, 0x00, 0x00 },
	{ 0xc2, 0xa7, 0x00, 0x00 }, { 0xe2, 0x80, 0xa2, 0x00 }, { 0xc2, 0xb6, 0x00, 0x00 }, { 0xc3, 0x9f, 0x00, 0x00 },
	{ 0xc2, 0xae, 0x00, 0x00 }, { 0xc2, 0xa9, 0x00, 0x00 }, { 0xe2, 0x84, 0xa2, 0x00 }, { 0xc2, 0xb4, 0x00, 0x00 },
	{ 0xc2, 0xa8, 0x00, 0x00 }, { 0xe2, 0x89, 0xa0, 0x00 }, { 0xc3, 0x86, 0x00, 0x00 }, { 0xc3, 0x98, 0x00, 0x00 },
	{ 0xe2, 0x88, 0x9e, 0x00 }, { 0xc2, 0xb1, 0x00, 0x00 }, { 0xe2, 0x89, 0xa4, 0x00 }, { 0xe2, 0x89, 0xa5, 0x00 },
	{ 0xc2, 0xa5, 0x00, 0x00 }, { 0xc2, 0xb5, 0x00, 0x00 }, { 0xe2, 0x88, 0x82, 0x00 }, { 0xe2, 0x88, 0x91, 0x00 },
	{ 0xe2, 0x88, 0x8f, 0x00 }, { 0xcf, 0x80, 0x00, 0x00 }, { 0xe2, 0x88, 0xab, 0x00 }, { 0xc2, 0xaa, 0x00, 0x00 },
	{ 0xc2, 0xba, 0x00, 0x00 }, { 0xce, 0xa9, 0x00, 0x00 }, { 0xc3, 0xa6, 0x00, 0x00 }, { 0xc3, 0xb8, 0x00, 0x00 },
	{ 0xc2, 0xbf, 0x00, 0x00 }, { 0xc2, 0xa1, 0x00, 0x00 }, { 0xc2, 0xac, 0x00, 0x00 }, { 0xe2, 0x88, 0x9a, 0x00 },
	{ 0xc6, 0x92, 0x00, 0x00 }, { 0xe2, 0x89, 0x88, 0x00 }, { 0xe2, 0x88, 0x86, 0x00 }, { 0xc2, 0xab, 0x00, 0x00 },
	{ 0xc2, 0xbb, 0x00, 0x00 }, { 0xe2, 0x80, 0xa6, 0x00 }, { 0xc2, 0xa0, 0x00, 0x00 }, { 0xc3, 0x80, 0x00, 0x00 },
	{ 0xc3, 0x83, 0x00, 0x00 }, { 0xc3, 0x95, 0x00, 0x00 }, { 0xc5, 0x92, 0x00, 0x00 }, { 0xc5, 0x93, 0x00, 0x00 },
	{ 0xe2, 0x80, 0x93, 0x00 }, { 0xe2, 0x80, 0x94, 0x00 }, { 0xe2, 0x80, 0x9c, 0x00 }, { 0xe2, 0x80, 0x9d, 0x00 },
	{ 0xe2, 0x80, 0x98, 0x00 }, { 0xe2, 0x80, 0x99, 0x00 }, { 0xc3, 0xb7, 0x00, 0x00 }, { 0xe2, 0x97, 0x8a, 0x00 },
	{ 0xc3, 0xbf, 0x00, 0x00 }, { 0xc5, 0xb8, 0x00, 0x00 }, { 0xe2, 0x81, 0x84, 0x00 }, { 0xe2, 0x82, 0xac, 0x00 },
	{ 0xe2, 0x80, 0xb9, 0x00 }, { 0xe2, 0x80, 0xba, 0x00 }, { 0xef, 0xac, 0x81, 0x00 }, { 0xef, 0xac, 0x82, 0x00 },
	{ 0xe2, 0x80, 0xa1, 0x00 }, { 0xc2, 0xb7, 0x00, 0x00 }, { 0xe2, 0x80, 0x9a, 0x00 }, { 0xe2, 0x80, 0x9e, 0x00 },
	{ 0xe2, 0x80, 0xb0, 0x00 }, { 0xc3, 0x82, 0x00, 0x00 }, { 0xc3, 0x8a, 0x00, 0x00 }, { 0xc3, 0x81, 0x00, 0x00 },
	{ 0xc3, 0x8b, 0x00, 0x00 }, { 0xc3, 0x88, 0x00, 0x00 }, { 0xc3, 0x8d, 0x00, 0x00 }, { 0xc3, 0x8e, 0x00, 0x00 },
	{ 0xc3, 0x8f, 0x00, 0x00 }, { 0xc3, 0x8c, 0x00, 0x00 }, { 0xc3, 0x93, 0x00, 0x00 }, { 0xc3, 0x94, 0x00, 0x00 },
	{ 0xef, 0xa3, 0xbf, 0x00 }, { 0xc3, 0x92, 0x00, 0x00 }, { 0xc3, 0x9a, 0x00, 0x00 }, { 0xc3, 0x9b, 0x00, 0x00 },
	{ 0xc3, 0x99, 0x00, 0x00 }, { 0xc4, 0xb1, 0x00, 0x00 }, { 0xcb, 0x86, 0x00, 0x00 }, { 0xcb, 0x9c, 0x00, 0x00 },
	{ 0xc2, 0xaf, 0x00, 0x00 }, { 0xcb, 0x98, 0x00, 0x00 }, { 0xcb, 0x99, 0x00, 0x00 }, { 0xcb, 0x9a, 0x00, 0x00 },
	{ 0xc2, 0xb8, 0x00, 0x00 }, { 0xcb, 0x9d, 0x00, 0x00 }, { 0xcb, 0x9b, 0x00, 0x00 }, { 0xcb, 0x87, 0x00, 0x00 }
};


static const unsigned char*	UniCharFromMacRoman(unsigned char c) {
	if( c >= 128 ) {
		return sMacRomanToUTF8Table[ c -128 ];
	} else if( c == 0x11 ) {
		static unsigned char	commandKey[4] = { 0xe2, 0x8c, 0x98, 0 };	// Unicode 0x2318
		return commandKey;
	} else if( c == 0x0D ) { // Most OSes today don't see CR as a line break, so swap that and LF:
		static unsigned char	commandKey[2] = { 0x0A, 0 };
		return commandKey;
	} else if( c == 0x0A ) { // Most OSes today don't see CR as a line break, so swap that and LF:
		static unsigned char	commandKey[2] = { 0x0D, 0 };
		return commandKey;
	} else {
		static unsigned char	asciiStr[2] = { 0, 0 };
		asciiStr[0] = c;
		return asciiStr;
	}
}


static unsigned char MacRomanFromUniChar(unsigned char* *currPos, long *lengthLeft) {
	unsigned char macRomanChar = 0;
	long x = 0;
	unsigned char currCh = (**currPos);
	if (currCh < 128) {
		++(*currPos);
		--(*lengthLeft);
		switch (currCh) {
			case 0x0D:
				return 0x0A;
				break;
			case 0x0A:
				return 0x0D;
				break;
			default:
				return currCh;
				break;
		}
	}
	
	for(macRomanChar = 0; macRomanChar < 128; ++macRomanChar) {
		unsigned char* utf8Str = sMacRomanToUTF8Table[macRomanChar];
		for(x = 0; ((*lengthLeft) - x) > 1; ++x) {
			if (utf8Str[x] == 0) { // Full match.
				(*currPos) += x;
				(*lengthLeft) -= x;
				return 128 + macRomanChar;
			} 
			if (utf8Str[x] != (*currPos)[x]) {
				break; // No match, try next UTF8 sequence by going back to outer loop.
			}
		}
	}
	
	(*currPos) += 1;
	(*lengthLeft) -= 1;
	return '?'; // UTF8 sequence with no equivalent in MacRoman.
}


static OSErr GetFSSpecFromPath(CharsHandle filePath, FSSpec *outFile) {
	OSErr		err = noErr;
	AliasHandle	alias = NULL;
	Boolean		wasChanged = false;
	CInfoPBRec	catInfo = {0};
	Str255		fileNameBuffer = {0};
		
	HLock(filePath);
	err = NewAliasMinimalFromFullPath(strlen(*filePath), *filePath, NULL, NULL, &alias);
	HUnlock(filePath);
	
	if (err != noErr) {
		return err;
	}
	
	err = ResolveAlias(NULL, alias, outFile, &wasChanged);
	
	DisposeHandle((Handle)alias);
	alias = NULL;

	if (err != noErr) {
		return err;
	} else if (outFile->vRefNum == 0) { // MoreFiles says this is a bug.
		return nsvErr;
	}
		
	return noErr;
}



// -----------------------------------------------------------------------------
//	xcmdmain
// -----------------------------------------------------------------------------

void xcmdmain(void) {
	Str255 task = {};
	Handle srcPath = NULL;
	FSSpec srcSpec = {};
	Handle dstPath = NULL;
	FSSpec dstSpec = {};
	short srcRefNum = -1;
	short dstRefNum = -1;
	OSErr err = noErr;
	FInfo fileInfo = {};
	long fileLength = 0;
	unsigned char* sourceData = NULL;
	
	SetReturnValue("\p");
	
	if (!GetIndXParameter255(1, task)) {
		AppendReturnValue("\pExpected \"toUTF8\" or \"toMacRoman\" as first parameter.");
		return;
	}
	
	if (EqualString("\p?", task, true, true)) {
		AppendReturnValue("\pSyntax: put|get xConvertText({\"toUTF8\"|\"toMacRoman\"}, <source path>, <dest path>)");
		return;
	} else if (EqualString("\p!", task, true, true)) {
		AppendReturnValue("\p(c) Copyright 2022 by Uli Kusterer, all rights reserved.");
		return;
	}
	
	if ((srcPath = GetIndXParameter(2)) == NULL) {
		AppendReturnValue("\pExpected file path to convert as second parameter.");
		return;
	}
	
	if ((dstPath = GetIndXParameter(3)) == NULL) {
		AppendReturnValue("\pExpected file path to write converted text to as third parameter.");
		return;
	}
	
	err = GetFSSpecFromPath(srcPath, &srcSpec);
	if (err != noErr) {
		AppendReturnValue("\pCouldn't find source file \"");
		AppendReturnValueChars(srcPath);
		AppendReturnValue("\p\" (");
		AppendReturnValueNum(err);
		AppendReturnValue("\p).");
		return;
	}
	err = GetFSSpecFromPath(dstPath, &dstSpec);
	if (err != noErr && err != fnfErr) { // We will create the file, of course it'll be fnf.
		AppendReturnValue("\pCouldn't find location for destination file \"");
		AppendReturnValueChars(dstPath);
		AppendReturnValue("\p\" (");
		AppendReturnValueNum(err);
		AppendReturnValue("\p).");
		return;
	}

	
	// Read entire source file into RAM (we're prolly in an emulator anyway):
	err = FSpOpenDF(&srcSpec, fsRdPerm, &srcRefNum);
	if (err != noErr) {
		AppendReturnValue("\pCouldn't open source file \"");
		AppendReturnValueChars(srcPath);
		AppendReturnValue("\p\" for reading (");
		AppendReturnValueNum(err);
		AppendReturnValue("\p).");
		return;
	}

	err = GetEOF(srcRefNum, &fileLength);
	if (err != noErr) {
		AppendReturnValue("\pCouldn't measure length of source file \"");
		AppendReturnValueChars(srcPath);
		AppendReturnValue("\p\" (");
		AppendReturnValueNum(err);
		AppendReturnValue("\p).");
		FSClose(srcRefNum);
		return;
	}

	sourceData = (unsigned char*) NewPtr(fileLength);
	err = MemError();
	if (err != noErr) {
		AppendReturnValue("\pNot enough memory to read source file \"");
		AppendReturnValueChars(srcPath);
		AppendReturnValue("\p\" (");
		AppendReturnValueNum(err);
		AppendReturnValue("\p).");
		FSClose(srcRefNum);
		return;
	}

	err = FSRead(srcRefNum, &fileLength, sourceData);
	if (err != noErr) {
		AppendReturnValue("\pError reading source file \"");
		AppendReturnValueChars(srcPath);
		AppendReturnValue("\p\" (");
		AppendReturnValueNum(err);
		AppendReturnValue("\p).");
		FSClose(srcRefNum);
		return;
	}
	FSClose(srcRefNum);
	srcRefNum = -1;

	// Get creator/type code of source so we can apply it to destination too:
	err = FSpGetFInfo(&srcSpec, &fileInfo);
	if (err != noErr) {
		AppendReturnValue("\pError determining type/creator of source file \"");
		AppendReturnValueChars(srcPath);
		AppendReturnValue("\p\" (");
		AppendReturnValueNum(err);
		AppendReturnValue("\p).");
		return;
	}
	
	// Create dest file:
	err = FSpCreate(&dstSpec, fileInfo.fdCreator, fileInfo.fdType, smSystemScript);
	if (err != noErr) {
		AppendReturnValue("\pError creating destination file \"");
		AppendReturnValueChars(dstPath);
		AppendReturnValue("\p\" (");
		AppendReturnValueNum(err);
		AppendReturnValue("\p).");
		return;
	}
	err = FSpOpenDF(&dstSpec, fsWrPerm, &dstRefNum);
	if (err != noErr) {
		AppendReturnValue("\pCouldn't open destination file \"");
		AppendReturnValueChars(dstPath);
		AppendReturnValue("\p\" for writing (");
		AppendReturnValueNum(err);
		AppendReturnValue("\p).");
		return;
	}
	
	// MacRoman -> UTF8:
	if (EqualString("\ptoUTF8", task, false, false)) {
		long x = 0;
		for(x = 0; x < fileLength; ++x) {
			const unsigned char* utfStr = UniCharFromMacRoman(sourceData[x]);
			long writeAmount = strlen((char*)utfStr);
			err = FSWrite(dstRefNum, &writeAmount, utfStr);
			if (err != noErr) {
				AppendReturnValue("\pError writing to destination file \"");
				AppendReturnValueChars(dstPath);
				AppendReturnValue("\p\" for writing (");
				AppendReturnValueNum(err);
				AppendReturnValue("\p).");
				FSClose(dstRefNum);
				return;
			}
		}
	// UTF8 -> MacRoman:
	} else if (EqualString("\ptoMacRoman", task, false, false)) {
		unsigned char* currPos = sourceData;
		long lengthLeft = fileLength;
		unsigned char macRomanCh = 0;
		while(lengthLeft > 0 && err == noErr) {
			long writeAmount = 1;
			macRomanCh = MacRomanFromUniChar(&currPos, &lengthLeft);
			err = FSWrite(dstRefNum, &writeAmount, &macRomanCh);
			if (err != noErr) {
				AppendReturnValue("\pError writing to destination file \"");
				AppendReturnValueChars(dstPath);
				AppendReturnValue("\p\" for writing (");
				AppendReturnValueNum(err);
				AppendReturnValue("\p).");
				FSClose(dstRefNum);
				return;
			}
		}
	} else {
		AppendReturnValue("\pExpected \"toUTF8\" or \"toMacRoman\" as first parameter.");
		DisposePtr((Ptr) sourceData);
		FSClose(dstRefNum);
		dstRefNum = -1;
		return;
	}
	
	DisposePtr((Ptr) sourceData);
	FSClose(dstRefNum);
	dstRefNum = -1;
}