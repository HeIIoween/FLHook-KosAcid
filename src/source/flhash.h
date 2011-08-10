#ifndef FLHASH_H
#define FLHASH_H

/*
flhash
-- function for calculating the Freelancer data nickname hash

  DWORD _stdcall flhash(char const *text, int length)

  Required headers:   flhash.h
  Required Libraries: flhash.lib
  Linked files:   flhash.dll

  Return Value: 32-bit value of the string passed

  Parameters:
    text    -   name to calculate
    length  -   length of text

  Remarks:
    flhash calculates the Freelancer data nickname hash.  The underlying code was written by:

        Sherlog <sherlog@t-online.de> for finding out the algorithm

    whereas I simply converted it to comply with Win32-DLL format.

  Example:
    dwHasCode = flhash("St01_to_St02_hole", 17);    // should return 2926089285
    dwHasCode = flhash("Li01a_to_Li01b_hole", 19);  // should return 2444625543
    
    You must pass ALL valid parameters to avoid failure within the DLL.
	*/

//DWORD flhash(char const *text, int length);



#endif  // FLHASH_H