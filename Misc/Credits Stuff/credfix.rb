$fname = "ALttP - VT_no-glitches-29_normal-open_randomized-ganon_389293820.sfc"

data = ""
open($fname, "rb") {|f| data = f.read }

data[0x01057E] = 0x4A.chr
data[0x01057F] = 0xB3.chr
data[0x010580] = 0x23.chr

open($fname, "wb") {|f| f.write(data) }