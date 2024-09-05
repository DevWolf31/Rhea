|Instruction|Mnemonic          |Bytes|
|-----------|------------------|-----|
|F0         |``JC    ±offset`` |3    |
|F1         |``JC.L  $address``|2    |
|F2         |``JC.N  $address``|3    |
|F3         |``JC.H  $address``|4    |
|D0         |JC    memory    |4    |
|D1         |JC    (memory)  |4    |
|F5         |JN    ±offset   |3    |
|F6         |JN.L  $address  |2    |
|F7         |JN.N  $address  |3    |
|F8         |JN.H  $address  |4    |
|D2         |JN    memory    |4    |
|D3         |JN    (memory)  |4    |
|FA         |JZ    ±offset   |3    |
|FB         |JZ.L  $address  |2    |
|FC         |JZ.N  $address  |3    |
|FD         |JZ.H  $address  |4    |
|D4         |JZ    memory    |4    |
|D5         |JZ    (memory)  |4    |
|E0         |JNC   ±offset   |3    |
|E1         |JNC.L $address  |2    |
|E2         |JNC.N $address  |3    |
|E3         |JNC.H $address  |4    |
|D6         |JNC   memory    |4    |
|D7         |JNC   (memory)  |4    |
|E5         |JNN   ±offset   |3    |
|E6         |JNN.L $address  |2    |
|E7         |JNN.N $address  |3    |
|E8         |JNN.H $address  |4    |
|D8         |JNN   memory    |4    |
|D9         |JNN   (memory)  |4    |
|EA         |JNZ   ±offset   |3    |
|EB         |JNZ.L $address  |2    |
|EC         |JNZ.N $address  |3    |
|ED         |JNZ.H $address  |4    |
|DA         |JNZ   memory    |4    |
|DB         |JNZ   (memory)  |4    |