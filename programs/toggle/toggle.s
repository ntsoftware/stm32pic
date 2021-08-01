000:  a01  goto    0x001
001:  064  clrf    0x04
002:  beb  goto    0x1eb
1eb:  c00  movlw   0x00
1ec:  006  tris    0x6
1ed:  cff  movlw   0xff
1ee:  026  movwf   0x06
1ef:  cc7  movlw   0xc7
1f0:  027  movwf   0x07
1f1:  bf2  goto    0x1f2
1f2:  2e7  decfsz  0x07, 0x1
1f3:  bf1  goto    0x1f1
1f4:  bf5  goto    0x1f5
1f5:  bf6  goto    0x1f6
1f6:  066  clrf    0x06
1f7:  cc7  movlw   0xc7
1f8:  027  movwf   0x07
1f9:  bfa  goto    0x1fa
1fa:  2e7  decfsz  0x07, 0x1
1fb:  bf9  goto    0x1f9
1fc:  bfd  goto    0x1fd
1fd:  bfe  goto    0x1fe
1fe:  bed  goto    0x1ed
