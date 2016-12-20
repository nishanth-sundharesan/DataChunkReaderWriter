*-----------------------------------------------------------
* Title      : Compute Average from the binary file
* Written by : Nishanth Sundharesan
* Date       : 21-October-2016
* Description: This program takes an integer(section number)
*              as an input and computes the average for the
*              Byte(Char) chunk, Word(Short) chunk and Long(Int)
*              chunk for that particular section. It uses 
*              the binary file outputted from the C prgoram. 
*
*   Char = Byte
*   Short = Word
*   Int = Long
*
*	Format of the Binary File:
*	1. Number Of Sections (32 bits)
*	2. First Section Size (32 bits)
*	3. Char Data's Size (32 bits)
*	4. Char Data (8 bits for each char)
*	5. Short Data's Size (32 bits)
*	6. Short Data (16 bits for each short)
*	7. Int Data's Size (32 bits)
*	8. Int Data (32 bits for each Int)
*	9. Second Section Size (32 bits)
*	10. Repeat 3-8 for second section
*	11. Third Section Size (32 bits)
*	12. Repeat 3-8 for third section
*	13. ...
*
*-----------------------------------------------------------
*-----------------------------------------------------------
*TRAP CODES
*-----------------------------------------------------------
DISPLAY_STRING_WOCRLF_TRAP_CODE    EQU     14
DISPLAY_STRING_WCRLF_TRAP_CODE     EQU     13
DISPLAY_STRING_NUMBER_TRAP_CODE    EQU     17
DISPLAY_STR_READ_NUM_TRAP_CODE     EQU     18
DISPLAY_NUMBER                     EQU     15
DISPLAY_ASCII_CHARACTER            EQU     6
READ_NUMBER_TRAP_CODE              EQU     4
*-----------------------------------------------------------
*-----------------------------------------------------------
*CONSTANTS
*-----------------------------------------------------------
BASETEN             EQU     10
FRAC_LENGTH         EQU     100
LINE_FEED           EQU     10
CARRIAGE_RETURN     EQU     13
*-----------------------------------------------------------

    ORG    $1000
START:                                           ;First instruction of program
    lea     BinaryData,a2
    move.l  (a2)+,d7                             ;Load the number of sections
    
INPUTAGAIN:    
    lea     InputString01,a1                     ;Load the DisplayString into the address register a1
    move.l  d7,d1     
    move.b  #DISPLAY_STRING_NUMBER_TRAP_CODE,d0  ;Display the string loaded into a1 and display the number loaded in d1
    TRAP    #15           
    
    lea     InputString02,a1                     ;Load the DisplayString into the address register a1
    move.l  #DISPLAY_STR_READ_NUM_TRAP_CODE,d0   ;Display the string loaded into a1 and read the number into d1
    TRAP    #15
        
    cmp.l   #0,d1                                ;Check if the entered section number is <=0
    ble     INPUTAGAIN                           ;If yes, then get input again
    cmp.l   d7,d1                                ;Check if the entered section number is greater than the number of sections
    bgt     INPUTAGAIN                           ;If yes, then get input again
    
    move.l  d1,d7                                ;Save the section number    
    
    lea     DisplaySectionString,a1
    jsr     DisplayStringAndNumber               ;Display "Section (section number)"    
    
    jsr     GotoNewLine
    *-----------------------------------------------------------    
    *Pointing the address register to the entered section
    
INCREMENTSECTION:    
    move.l  (a2)+,d1                             ;Load the section size/section offset
    subi.l  #1,d7
    cmpi.l  #0,d7
    beq     EXITSECTION
    add.l   d1,a2                                ;Add the section size/section offset
    bra     INCREMENTSECTION
    *-----------------------------------------------------------
            
EXITSECTION:
    *-------------------------------------------------------------------------------------------------------------------------------------    
    *Computing Average for Bytes    
    move.l  (a2)+,d3                             ;Load the number of bytes
    move.l  d3,d1
    jsr     DisplayNumber                        ;Displaying the number of bytes
           
    clr.l   d0
    clr.l   d4

    cmpi.l  #0,d3
    beq     SKIPADDINGBYTES
    *-----------------------------------------------------------
    *Checking if the (number of bytes)short size is odd number
    move.b  #1,d6                                ;Set the least significant bit to 1
    and.b   d1,d6                                ;AND it to the short size
    cmpi.b  #1,d6                                ;Check if it's odd
    bne     SKIPPADDING                          ;If yes, then add an additional byte as padding(addresses cannot be odd numbers)
    add.l  #1,a2                                 ;Add 1 byte to the address
    *-----------------------------------------------------------
    
SKIPPADDING:        
    *Initializing the counter to compute the total
    move.l  d3,d5                                 
    subi.l  #1,d5                                ;Subtracting 1 from the counter as dbra subtracts and branches until the counter reaches -1
       
ADDNEXTBYTE:
    move.b  (a2)+,d0
    add.l   d0,d4                                ;Sum up the numbers to compute the total
    dbra    d5,ADDNEXTBYTE
    
    divu    d3,d4                                ;Divide total/numberofbytes to get average 
SKIPADDINGBYTES:   
    lea     DisplayByteString,a1                 ;Load the string into the address register a1 
    jsr     ComputeDisplayAverage

    *-------------------------------------------------------------------------------------------------------------------------------------
    
    jsr     GotoNewLine
    
    *-------------------------------------------------------------------------------------------------------------------------------------    
    *Computing Average for Shorts/Words
    move.l  (a2)+,d3                             ;Load the number of words
    move.l  d3,d1
    jsr     DisplayNumber                        ;Displaying the number of words
        
    clr.l   d0
    clr.l   d4
    
    cmpi.l  #0,d3
    beq     SKIPADDINGWORDS
    
    *Initializing the counter to compute the total
    move.l  d3,d5                                 
    subi.l  #1,d5                                ;Subtracting 1 from the counter as dbra subtracts and branches until the counter reaches -1      
ADDNEXTWORD:
    move.w  (a2)+,d0
    add.l   d0,d4                                ;Sum up the numbers to compute the total
    dbra    d5,ADDNEXTWORD
    
    divu    d3,d4                                ;Divide total/numberofwords to get average
SKIPADDINGWORDS:    
    lea     DisplayShortString,a1                ;Load the string into the address register a1
    jsr     ComputeDisplayAverage
    *-------------------------------------------------------------------------------------------------------------------------------------
    
    jsr     GotoNewLine
    
    *-------------------------------------------------------------------------------------------------------------------------------------
    *Computing Average for Longs
    move.l  (a2)+,d3                             ;Load the number of longs
    move.l  d3,d1
    jsr     DisplayNumber                        ;Displaying the number of longs
   
    clr.l   d0
    clr.l   d4
    
    cmpi.l  #0,d3
    beq     SKIPADDINGLONGS
    
    *Initializing the counter to compute the total
    move.l  d3,d5                                 
    subi.l  #1,d5                                ;Subtracting 1 from the counter as dbra subtracts and branches until the counter reaches -1
      
ADDNEXTLONG:
    move.l  (a2)+,d0
    add.l   d0,d4                                ;Sum up the numbers to compute the total
    dbra    d5,ADDNEXTLONG
    
    divu    d3,d4                                ;Divide total/numberoflongs to get average
SKIPADDINGLONGS:    
    lea     DisplayLongString,a1                 ;Load the string into the address register a1
    jsr     ComputeDisplayAverage
    *-------------------------------------------------------------------------------------------------------------------------------------

    SIMHALT                                      ;Halt the simulator
    
DisplayNumber
    move.l  #BASETEN,d2                          ;Setting the Base for the number
    move.l  #DISPLAY_NUMBER,d0                   ;Display the number present in d1
    TRAP    #15
    rts
    
DisplayString   
    move.l  #DISPLAY_STRING_WOCRLF_TRAP_CODE,d0
    TRAP    #15
    rts

DisplayStringAndNumber
    move.l  #DISPLAY_STRING_NUMBER_TRAP_CODE,d0
    TRAP    #15
    rts

GotoNewLine
    move.l  #LINE_FEED,d1
    move.l  #DISPLAY_ASCII_CHARACTER,d0
    TRAP    #15

    move.l  #CARRIAGE_RETURN,d1
    move.l  #DISPLAY_ASCII_CHARACTER,d0
    TRAP    #15
    rts
    
DisplayTwoDigits
    cmpi.l  #BASETEN,d1                          ;Check if the decimal number is less than 10
    bge     SKIPDISPLAYZERO                      ;If not, then skip. If yes, then display a zero and then display the decimal number
    
    movem.l d1,-(sp)                             ;Save the decimal number
    move.l  #0,d1                                ;Display the zero
    move.l  #BASETEN,d2                          ;Setting the Base for the number
    move.l  #DISPLAY_NUMBER,d0                   ;Display the number present in d1
    TRAP    #15    
    movem.l (sp)+,d1                             ;Reload the decimal number
    
SKIPDISPLAYZERO:    
    move.l  #BASETEN,d2                          ;Setting the Base for the number
    move.l  #DISPLAY_NUMBER,d0                   ;Display the number present in d1
    TRAP    #15    
    rts    

ComputeDisplayAverage
    clr.l   d1
    move.w  d4,d1
    jsr     DisplayStringAndNumber               ;Display the string loaded in a1 and the number present in d1
                                                 ;Display the whole number
    
    lea     DecimalPoint,a1 
    jsr     DisplayString                        ;Display the decimal point
    
    *Computing fractional part upto 2 decimals
    swap    d4                                   ;Swap and Extend to get the remainder
    ext.l   d4
    
    mulu    #FRAC_LENGTH,d4                      
    move.l  d3,d5   
    lsr     #1,d5                                ;Divide the divisor by 2
    add.l   d5,d4                                ;Add it to the dividend
    cmpi.l  #0,d3                                ;If the divisor is zero then skip divide
    beq     SKIPDIVIDE    
    divu    d3,d4                                ;Divide the dividend by the initial divisor, to get the fractional part       
    ext.l   d4                                   ;Get the quotient
SKIPDIVIDE:
    move.w  d4,d1
    jsr     DisplayTwoDigits                     ;Display the fractional part
    
    rts

DisplaySectionString    dc.b    'Section  ',0
DisplayByteString       dc.b    ' bytes,average value ',0 
DisplayShortString      dc.b    ' shorts,average value ',0 
DisplayLongString       dc.b    ' longs,average value ',0 
InputString01           dc.b    'Enter the section number between 1 and ',0
InputString02           dc.b    ': ',0
DecimalPoint            dc.b    '.',0
BinaryData              INCBIN  "../DataChunkReaderWriter/DataChunkReaderWriter/OutputFiles/OutputBinaryFile.dat"
    END    START        ; last line of source














*~Font name~Courier New~
*~Font size~9~
*~Tab type~1~
*~Tab size~4~
