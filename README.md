# 48D24X Assembly:

### Instructions: ###
`F0h ⟩ JC` - Jumps to the specified address when the `C` flag is set.
|Code   |Mnemonic                 |
|-------|-------------------------|
|F020h  |JC.B  $address           |
|F022h  |JC.S  $address           |
|F024h  |JC.L  $address           |
|F040h  |JC.B  ($address)         |
|F042h  |JC.S  ($address)         |
|F044h  |JC.L  ($address)         |
|F060h  |JC.B  ($address, %DI)    |
|F062h  |JC.S  ($address, %DI)    |
|F064h  |JC.L  ($address, %DI)    |
|F070h  |JC.B  ($address), %DI    |
|F072h  |JC.S  ($address), %DI    |
|F074h  |JC.L  ($address), %DI    |
|F0A0h  |JC.B  +/-offset          |
|F0A2h  |JC.S  +/-offset          |

---
`F1h ⟩ JNC` - Jumps to the specified address when the `C` flag is unset.
|Code   |Mnemonic                 |
|-------|-------------------------|
|F120h  |JNC.B  $address          |
|F122h  |JNC.S  $address          |
|F124h  |JNC.L  $address          |
|F140h  |JNC.B  ($address)        |
|F142h  |JNC.S  ($address)        |
|F144h  |JNC.L  ($address)        |
|F160h  |JNC.B  ($address, %DI)   |
|F162h  |JNC.S  ($address, %DI)   |
|F164h  |JNC.L  ($address, %DI)   |
|F170h  |JNC.B  ($address), %DI   |
|F172h  |JNC.S  ($address), %DI   |
|F174h  |JNC.L  ($address), %DI   |
|F1A0h  |JNC.B  +/-offset         |
|F1A2h  |JNC.S  +/-offset         |

---
`F3h ⟩ JQ` - Jumps to the specified address when the `Q` flag is set.
|Code   |Mnemonic                 |
|-------|-------------------------|
|F320h  |JQ.B  $address           |
|F322h  |JQ.S  $address           |
|F324h  |JQ.L  $address           |
|F340h  |JQ.B  ($address)         |
|F342h  |JQ.S  ($address)         |
|F344h  |JQ.L  ($address)         |
|F360h  |JQ.B  ($address, %DI)    |
|F362h  |JQ.S  ($address, %DI)    |
|F364h  |JQ.L  ($address, %DI)    |
|F370h  |JQ.B  ($address), %DI    |
|F372h  |JQ.S  ($address), %DI    |
|F374h  |JQ.L  ($address), %DI    |
|F3A0h  |JQ.B  +/-offset          |
|F3A2h  |JQ.S  +/-offset          |

---
`F4h ⟩ JNQ` - Jumps to the specified address when the `Q` flag is unset.
|Code   |Mnemonic                 |
|-------|-------------------------|
|F420h  |JNQ.B  $address          |
|F422h  |JNQ.S  $address          |
|F424h  |JNQ.L  $address          |
|F440h  |JNQ.B  ($address)        |
|F442h  |JNQ.S  ($address)        |
|F444h  |JNQ.L  ($address)        |
|F460h  |JNQ.B  ($address, %DI)   |
|F462h  |JNQ.S  ($address, %DI)   |
|F464h  |JNQ.L  ($address, %DI)   |
|F470h  |JNQ.B  ($address), %DI   |
|F472h  |JNQ.S  ($address), %DI   |
|F474h  |JNQ.L  ($address), %DI   |
|F4A0h  |JNQ.B  +/-offset         |
|F4A2h  |JNQ.S  +/-offset         |

---